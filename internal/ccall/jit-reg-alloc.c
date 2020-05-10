/*
 * jit-reg-alloc.c - Register allocation routines for the JIT.
 *
 * Copyright (C) 2004  Southern Storm Software, Pty Ltd.
 *
 * This file is part of the libjit library.
 *
 * The libjit library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The libjit library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the libjit library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "jit-internal.h"
#include "jit-reg-alloc.h"
#include <jit/jit-dump.h>
#include <stdio.h>
#include <string.h>

/*@

The @code{libjit} library provides a number of functions for
performing register allocation within basic blocks so that you
mostly don't have to worry about it:

@*/

/*
 * Dump debug information about the register allocation state.
 */
#undef JIT_REG_DEBUG

/*
 * Minimum number of times a candidate must be used before it
 * is considered worthy of putting in a global register.
 */
#define	JIT_MIN_USED		3

/*
 * Use is_register_occupied() function.
 */
#undef IS_REGISTER_OCCUPIED

/*
 * Check if the register is on the register stack.
 */
#ifdef JIT_REG_STACK
#define IS_STACK_REG(reg)	((jit_reg_flags(reg) & JIT_REG_IN_STACK) != 0)
#else
#define IS_STACK_REG(reg)	(0)
#endif

/* The cost value that precludes using the register in question. */
#define COST_TOO_MUCH		1000000

#define COST_COPY		4
#define COST_SPILL_DIRTY	16
#define COST_SPILL_DIRTY_GLOBAL	4
#define COST_SPILL_CLEAN	1
#define COST_SPILL_CLEAN_GLOBAL	1
#define COST_GLOBAL_BIAS	2
#define COST_THRASH		100
#define COST_CLOBBER_GLOBAL	1000

#ifdef JIT_BACKEND_X86
# define ALLOW_CLOBBER_GLOBAL	1
#else
# define ALLOW_CLOBBER_GLOBAL	0
#endif

/* Value usage flags. */
#define VALUE_INPUT		1
#define VALUE_USED		2
#define VALUE_LIVE		4
#define VALUE_DEAD		8

/* Clobber flags. */
#define CLOBBER_NONE		0
#define CLOBBER_INPUT_VALUE	1
#define CLOBBER_REG		2
#define CLOBBER_OTHER_REG	4

#ifdef JIT_REG_DEBUG
#include <stdlib.h>

static void dump_regs(jit_gencode_t gen, const char *name)
{
	int reg, index;
	jit_value_t value;

	printf("%s:\n", name);
	for(reg = 0; reg < JIT_NUM_REGS; ++reg)
	{
		if(gen->contents[reg].num_values == 0 && !(gen->contents[reg].used_for_temp))
		{
			continue;
		}
		printf("\t%s: ", jit_reg_name(reg));
		if(gen->contents[reg].num_values > 0)
		{
			for(index = 0; index < gen->contents[reg].num_values; ++index)
			{
				value = gen->contents[reg].values[index];
				if(index)
				{
					fputs(", ", stdout);
				}
				jit_dump_value(stdout, jit_value_get_function(value), value, 0);
			}
			if(gen->contents[reg].used_for_temp)
			{
				printf(", used_for_temp");
			}
		}
		else if(gen->contents[reg].used_for_temp)
		{
			printf("used_for_temp");
		}
		else
		{
			printf("free");
		}
		putc('\n', stdout);
	}
#ifdef JIT_REG_STACK
	printf("stack_top: %d\n", gen->reg_stack_top);
#endif
	fflush(stdout);
}
#endif

/*
 * Find the start register of a register pair given the end register.
 */
static int
get_long_pair_start(int other_reg)
{
	int reg;
	for(reg = 0; reg < JIT_NUM_REGS; reg++)
	{
		if(other_reg == jit_reg_other_reg(reg))
		{
			return reg;
		}
	}
	return -1;
}

/*
 * Check if two values are known to be equal.
 */
static int
are_values_equal(_jit_regdesc_t *desc1, _jit_regdesc_t *desc2)
{
	if(desc1 && desc2 && desc1->value && desc2->value)
	{
		if(desc1->value == desc2->value)
		{
			return 1;
		}
		if(desc1->value->in_register && desc2->value->in_register)
		{
			return desc1->value->reg == desc2->value->reg;
		}
	}
	return 0;
}

/*
 * Exchange the content of two value descriptors.
 */
static void
swap_values(_jit_regdesc_t *desc1, _jit_regdesc_t *desc2)
{
	_jit_regdesc_t tdesc;
	tdesc = *desc1;
	*desc1 = *desc2;
	*desc2 = tdesc;
}

/*
 * Get value usage and liveness information. The accurate liveness data is
 * only available for values used by the current instruction.
 *
 * VALUE_INPUT flag is set if the value is one of the instruction's inputs.
 *
 * VALUE_LIVE and VALUE_USED flags are set for input values only according
 * to the liveness flags provided along with the instruction.
 *
 * VALUE_DEAD flag is set in two cases. First, it is always set for output
 * values. Second, it is set for input values that are neither live nor used.
 *
 * These flags are used when spilling a register. In this case we generally
 * do not know if the values in the register are used by the instruction. If
 * the VALUE_INPUT flag is present then it is so and the value has to be held
 * in the register for the instruction to succeed. If the VALUE_DEAD flag is
 * present then there is no need to spill the value and it may be discarded.
 * Otherwise the value must be spilled.
 *
 * The VALUE_LIVE and VALUE_USED flags may only be set for input values of
 * the instruction. For other values these flags are not set even if they are
 * perfectly alive. These flags are used as a hint for spill cost calculation.
 *
 * NOTE: The output value is considered to be dead because the instruction is
 * just about to recompute it so there is no point to save it.
 *
 * Generally, a value becomes dead just after the instruction that used it
 * last time. The allocator frees dead values after each instruction so it
 * might seem that there is no chance to find any dead value on the current
 * instruction. However if the value is used by the current instruction both
 * as the input and output then it was alive after the last instruction and
 * hence was not freed. And just in case if some dead values may creep through
 * the allocator's checks...
 */
static int
value_usage(_jit_regs_t *regs, jit_value_t value)
{
	int flags;

	flags = 0;
	if(value->is_constant)
	{
		flags |= VALUE_DEAD;
	}
	if(!regs)
	{
		return flags;
	}
	if(value == regs->descs[0].value)
	{
		if(regs->ternary)
		{
			flags |= VALUE_INPUT;
			if(regs->descs[0].used)
			{
				flags |= VALUE_LIVE | VALUE_USED;
			}
			else if(regs->descs[0].live)
			{
				flags |= VALUE_LIVE;
			}
			else
			{
				flags |= VALUE_DEAD;
			}
		}
		else
		{
			flags |= VALUE_DEAD;
		}
	}
	if(value == regs->descs[1].value)
	{
		flags |= VALUE_INPUT;
		if(regs->descs[1].used)
		{
			flags |= VALUE_LIVE | VALUE_USED;
		}
		else if(regs->descs[1].live)
		{
			flags |= VALUE_LIVE;
		}
		else
		{
			flags |= VALUE_DEAD;
		}
	}
	if(value == regs->descs[2].value)
	{
		flags |= VALUE_INPUT;
		if(regs->descs[2].used)
		{
			flags |= VALUE_LIVE | VALUE_USED;
		}
		else if(regs->descs[2].live)
		{
			flags |= VALUE_LIVE;
		}
		else
		{
			flags |= VALUE_DEAD;
		}
	}
	return flags;
}

/*
 * Check if the register contains any live values.
 */
static int
is_register_alive(jit_gencode_t gen, _jit_regs_t *regs, int reg)
{
	int index, usage;

	if(reg < 0)
	{
		return 0;
	}

	/* Assume that a global register is always alive unless it is to be
	   computed right away. */
	if(jit_reg_is_used(gen->permanent, reg))
	{
		if(!regs->ternary
		   && regs->descs[0].value
		   && regs->descs[0].value->has_global_register
		   && regs->descs[0].value->global_reg == reg)
		{
			return 0;
		}
		return 1;
	}

	if(gen->contents[reg].is_long_end)
	{
		reg = get_long_pair_start(reg);
	}
	for(index = 0; index < gen->contents[reg].num_values; index++)
	{
		usage = value_usage(regs, gen->contents[reg].values[index]);
		if((usage & VALUE_DEAD) == 0)
		{
			return 1;
		}
	}

	return 0;
}

#ifdef IS_REGISTER_OCCUPIED
/*
 * Check if the register contains any values either dead or alive
 * that may need to be evicted from it.
 */
static int
is_register_occupied(jit_gencode_t gen, _jit_regs_t *regs, int reg)
{
	if(reg < 0)
	{
		return 0;
	}

	/* Assume that a global register is always occupied. */
	if(jit_reg_is_used(gen->permanent, reg))
	{
		return 1;
	}

	if(gen->contents[reg].is_long_end)
	{
		reg = get_long_pair_start(reg);
	}
	if(gen->contents[reg].num_values)
	{
		return 1;
	}

	return 0;
}
#endif

/*
 * Determine the effect of using a register for a value. This includes the
 * following:
 *  - whether the value is clobbered by the instruction;
 *  - whether the previous contents of the register is clobbered.
 *
 * The value is clobbered by the instruction if it is used as input value
 * and the output value will go to the same register and these two values
 * are not equal. Or the instruction has a side effect that destroys the
 * input value regardless of the output. This is indicated with the
 * CLOBBER_INPUT_VALUE flag.
 *
 * The previous content is clobbered if the register contains any non-dead
 * values that are destroyed by loading the input value, by computing the
 * output value, or as a side effect of the instruction.
 *
 * The previous content is not clobbered if the register contains only dead
 * values or it is used for input value that is already in the register so
 * there is no need to load it and at the same time the instruction has no
 * side effects that destroy the input value or the register is used for
 * output value and the only value it contained before is the same value.
 *
 * The flag CLOBBER_REG indicates if the previous content of the register is
 * clobbered. The flag CLOBBER_OTHER_REG indicates that the other register
 * in a long pair is clobbered.
 *
 */
static int
clobbers_register(jit_gencode_t gen, _jit_regs_t *regs, int index, int reg, int other_reg)
{
	int flags;

	if(!regs->descs[index].value)
	{
		return CLOBBER_NONE;
	}

	if(regs->ternary || !regs->descs[0].value)
	{
		/* this is either a ternary or binary or unary note */
		if(regs->descs[index].clobber)
		{
			flags = CLOBBER_INPUT_VALUE;
		}
#ifdef JIT_REG_STACK
		else if(IS_STACK_REG(reg) && !regs->no_pop)
		{
			flags = CLOBBER_INPUT_VALUE;
		}
#endif
		else
		{
			flags = CLOBBER_NONE;
		}
	}
	else if(index == 0)
	{
		/* this is the output value of a binary or unary op */

		/* special case: a copy operation. Check if we could coalesce
		   the destination value with the source. */
		if(regs->copy
		   && regs->descs[1].value
		   && regs->descs[1].value->in_register
		   && regs->descs[1].value->reg == reg
		   && ((regs->descs[0].value->in_register && regs->descs[0].value->reg == reg)
		       || gen->contents[reg].num_values < JIT_MAX_REG_VALUES
		       || !(regs->descs[1].used || regs->descs[1].live)))
		{
			return CLOBBER_NONE;
		}

		flags = CLOBBER_NONE;
#ifdef IS_REGISTER_OCCUPIED
		if(is_register_occupied(gen, regs, reg))
		{
			flags |= CLOBBER_REG;
		}
		if(is_register_occupied(gen, regs, other_reg))
		{
			flags |= CLOBBER_OTHER_REG;
		}
#else
		if(is_register_alive(gen, regs, reg))
		{
			flags |= CLOBBER_REG;
		}
		if(is_register_alive(gen, regs, other_reg))
		{
			flags |= CLOBBER_OTHER_REG;
		}
#endif
		return flags;
	}
	else if(regs->copy)
	{
		flags = CLOBBER_NONE;
	}
#ifdef JIT_REG_STACK
	else if(IS_STACK_REG(reg) && !regs->no_pop)
	{
		/* this is a binary or unary stack op -- the input value
		   is either popped or overwritten by the output */
		flags = CLOBBER_INPUT_VALUE;
	}
#endif
	else if(reg == regs->descs[0].reg
		|| reg == regs->descs[0].other_reg
		|| other_reg == regs->descs[0].reg)
	{
		/* the input value of a binary or unary op is clobbered
		   by the output value */
		flags = CLOBBER_INPUT_VALUE;
	}
	else if(regs->descs[index].clobber)
	{
		flags = CLOBBER_INPUT_VALUE;
	}
	else
	{
		flags = CLOBBER_NONE;
	}

	if(flags == CLOBBER_NONE)
	{
		if(regs->descs[index].value->has_global_register
		   && regs->descs[index].value->global_reg == reg)
		{
			return CLOBBER_NONE;
		}
		if(regs->descs[index].value->in_register
		   && regs->descs[index].value->reg == reg)
		{
			return CLOBBER_NONE;
		}
	}

#ifdef IS_REGISTER_OCCUPIED
	if(is_register_occupied(gen, regs, reg))
	{
		flags |= CLOBBER_REG;
	}
	if(is_register_occupied(gen, regs, other_reg))
	{
		flags |= CLOBBER_OTHER_REG;
	}
#else
	if(is_register_alive(gen, regs, reg))
	{
		flags |= CLOBBER_REG;
	}
	if(is_register_alive(gen, regs, other_reg))
	{
		flags |= CLOBBER_OTHER_REG;
	}
#endif
	return flags;
}

/*
 * Assign scratch register.
 */
static void
set_scratch_register(jit_gencode_t gen, _jit_regs_t *regs, int index, int reg)
{
	if(reg >= 0)
	{
		regs->scratch[index].reg = reg;

		jit_reg_set_used(gen->touched, reg);
		jit_reg_set_used(regs->clobber, reg);
		jit_reg_set_used(regs->assigned, reg);
	}
}

/*
 * Set value information.
 */
static void
set_regdesc_value(
	_jit_regs_t *regs,
	int index,
	jit_value_t value,
	int flags,
	_jit_regclass_t *regclass,
	int live,
	int used)
{
  	_jit_regdesc_t *desc;

	desc = &regs->descs[index];
	desc->value = value;
	desc->clobber = ((flags & (_JIT_REGS_CLOBBER | _JIT_REGS_EARLY_CLOBBER)) != 0);
	desc->early_clobber = ((flags & _JIT_REGS_EARLY_CLOBBER) != 0);
	desc->regclass = regclass;
	desc->live = live;
	desc->used = used;
}

/*
 * Assign register to a value.
 */
static void
set_regdesc_register(jit_gencode_t gen, _jit_regs_t *regs, int index, int reg, int other_reg)
{
	int assign;
	if(reg >= 0)
	{
		assign = (index > 0 || regs->ternary || regs->descs[0].early_clobber);

		regs->descs[index].reg = reg;
		regs->descs[index].other_reg = other_reg;

		jit_reg_set_used(gen->touched, reg);
		if(assign)
		{
			jit_reg_set_used(regs->assigned, reg);
		}
		if(other_reg >= 0)
		{
			jit_reg_set_used(gen->touched, other_reg);
			if(assign)
			{
				jit_reg_set_used(regs->assigned, other_reg);
			}
		}
	}
}

/*
 * Determine value flags.
 */
static void
set_regdesc_flags(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
  	_jit_regdesc_t *desc;
	int reg, other_reg;
	int clobber, clobber_input;
	int is_input, is_live_input, is_used_input;

#ifdef JIT_REG_DEBUG
	printf("set_regdesc_flags(index = %d)\n", index);
#endif

	desc = &regs->descs[index];
	if(desc->reg < 0 || desc->duplicate)
	{
		return;
	}

	/* See if the value clobbers the register it is assigned to. */
	clobber = clobbers_register(gen, regs, index, desc->reg, desc->other_reg);
#ifdef JIT_REG_DEBUG
	if((clobber & CLOBBER_INPUT_VALUE) != 0)
	{
		printf("clobber input\n");
	}
	if((clobber & CLOBBER_REG) != 0)
	{
		printf("clobber reg\n");
	}
	if((clobber & CLOBBER_OTHER_REG) != 0)
	{
		printf("clobber other reg\n");
	}
#endif

	/* See if this is an input value and whether it is alive. */
	if(regs->ternary)
	{
		is_input = 1;
		is_live_input = desc->live;
		is_used_input = desc->used;
	}
	else if(index > 0)
	{
		is_input = 1;
		if(regs->descs[0].value == desc->value)
		{
			is_live_input = is_used_input = 0;
		}
		else
		{
			is_live_input = desc->live;
			is_used_input = desc->used;
		}
	}
	else
	{
		is_input = is_live_input = is_used_input = 0;
	}

	if(is_input)
	{
		/* Find the register the value is already in (if any). */
		if(desc->value->in_register)
		{
			reg = desc->value->reg;
			if(gen->contents[reg].is_long_start)
			{
				other_reg = jit_reg_other_reg(reg);
			}
			else
			{
				other_reg = -1;
			}
		}
		else
		{
			reg = -1;
			other_reg = -1;
		}

		/* See if the input value is thrashed by other inputs. The allocator
		   tries to avoid thrashing so it may only take place if the register
		   is assigned explicitly. For x87 registers the problem of thrashing
		   may be best solved with fxch but as the stack registers are never
		   assigned explicitely there is no such problem for them at all. */
		if(reg >= 0)
		{
			if(index != 0 && regs->ternary
			   && !are_values_equal(desc, &regs->descs[0]))
			{
				if(reg == regs->descs[0].reg
				   || reg == regs->descs[0].other_reg
				   || (other_reg >= 0
				       && (other_reg == regs->descs[0].reg
					   || other_reg == regs->descs[0].other_reg)))
				{
					desc->thrash = 1;
				}
			}
			if(index != 1 && !are_values_equal(desc, &regs->descs[1]))
			{
				if(reg == regs->descs[1].reg
				   || reg == regs->descs[1].other_reg
				   || (other_reg >= 0
				       && (other_reg == regs->descs[1].reg
					   || other_reg == regs->descs[1].other_reg)))
				{
					desc->thrash = 1;
				}
			}
			if(index != 2 && !are_values_equal(desc, &regs->descs[2]))
			{
				if(reg == regs->descs[2].reg
				   || reg == regs->descs[2].other_reg
				   || (other_reg >= 0
				       && (other_reg == regs->descs[2].reg
					   || other_reg == regs->descs[2].other_reg)))
				{
					desc->thrash = 1;
				}
			}

			if(desc->thrash)
			{
				reg = -1;
				other_reg = -1;
			}
		}

		/* See if the value needs to be loaded or copied or none. */
		if(reg != desc->reg)
		{
			if(desc->value->has_global_register)
			{
				desc->copy = (desc->value->global_reg != desc->reg);
			}
			else if(reg < 0)
			{
				desc->load = 1;
			}
			else
			{
				desc->copy = 1;
			}
		}

		/* See if the input value needs to be stored before the
		   instruction and if it stays in the register after it. */
		if(desc->value->is_constant)
		{
			desc->kill = 1;
		}
		else if(!is_used_input)
		{
			desc->store = is_live_input;
			desc->kill = 1;
		}
		else
		{
			/* See if the input value is destroyed by the instruction. */
			clobber_input = 0;
			if(!desc->copy)
			{
				if(jit_reg_is_used(regs->clobber, desc->reg)
				   || (desc->other_reg >= 0
				       && jit_reg_is_used(regs->clobber, desc->other_reg)))
				{
					clobber_input = 1;
				}
				else if ((clobber & CLOBBER_INPUT_VALUE) != 0)
				{
					clobber_input = 1;
				}
			}
			else if(reg >= 0)
			{
				if(jit_reg_is_used(regs->clobber, reg)
				   || (other_reg >= 0
				       && jit_reg_is_used(regs->clobber, other_reg)))
				{
					clobber_input = 1;
				}
				else if(!regs->ternary
					&& regs->descs[0].value
					&& (reg == regs->descs[0].reg
					    || reg == regs->descs[0].other_reg
					    || other_reg == regs->descs[0].reg))
				{
					clobber_input = 1;
				}
			}

			if(clobber_input)
			{
				desc->store = 1;
				desc->kill = 1;
			}
		}

		/* Store the value if it is going to be thrashed by another one. */
		if(desc->thrash)
		{
			desc->store = 1;
		}

#ifdef JIT_REG_STACK
		/* Count stack registers. */
		if(IS_STACK_REG(desc->reg))
		{
			++(regs->wanted_stack_count);
			if(!desc->load && !desc->copy)
			{
				++(regs->loaded_stack_count);
			}
		}
#endif
	}

	/* See if the value clobbers a global register. In this case the global
	   register is pushed onto stack before the instruction and popped back
	   after it. */
	if(!desc->copy
	   && (!desc->value->has_global_register || desc->value->global_reg != desc->reg)
	   && (jit_reg_is_used(gen->permanent, desc->reg)
	       || (desc->other_reg >= 0 && jit_reg_is_used(gen->permanent, desc->other_reg))))
	{
		desc->kill = 1;
	}

	/* Set clobber flags (this indicates registers to be spilled). */
	if((clobber & CLOBBER_REG) != 0)
	{
		jit_reg_set_used(regs->clobber, desc->reg);
	}
	if((clobber & CLOBBER_OTHER_REG) != 0)
	{
		jit_reg_set_used(regs->clobber, desc->other_reg);
	}

#ifdef JIT_REG_DEBUG
	printf("value = ");
	jit_dump_value(stdout, jit_value_get_function(desc->value), desc->value, 0);
	printf("\n");
	printf("value->in_register = %d\n", desc->value->in_register);
	printf("value->reg = %d\n", desc->value->reg);
	printf("value->has_global_register = %d\n", desc->value->has_global_register);
	printf("value->in_global_register = %d\n", desc->value->in_global_register);
	printf("value->global_reg = %d\n", desc->value->global_reg);
	printf("value->in_frame = %d\n", desc->value->in_frame);
	printf("reg = %d\n", desc->reg);
	printf("other_reg = %d\n", desc->other_reg);
	printf("live = %d\n", desc->live);
	printf("used = %d\n", desc->used);
	printf("clobber = %d\n", desc->clobber);
	printf("early_clobber = %d\n", desc->early_clobber);
	printf("duplicate = %d\n", desc->duplicate);
	printf("thrash = %d\n", desc->thrash);
	printf("store = %d\n", desc->store);
	printf("load = %d\n", desc->load);
	printf("copy = %d\n", desc->copy);
	printf("kill = %d\n", desc->kill);
#endif
}

/*
 * Compute the register spill cost. The register spill cost is computed as
 * the sum of spill costs of individual values the register contains. The
 * spill cost of a value depends on the following factors:
 *
 * 1. Values that are not used after the current instruction may be safely
 *    discareded so their spill cost is taken to be zero.
 * 2. Values that are spilled to global registers are cheaper than values
 *    that are spilled into stack frame.
 * 3. Clean values are cheaper than dirty values.
 *
 * NOTE: A value is clean if it was loaded from the stack frame or from a
 * global register and has not changed since then. Otherwise it is dirty.
 * There is no need to spill clean values. However their spill cost is
 * considered to be non-zero so that the register allocator will choose
 * those registers that do not contain live values over those that contain
 * live albeit clean values.
 *
 * For global registers this function returns the cost of zero. So global
 * registers have to be handled separately.
 */
static int
compute_spill_cost(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg)
{
	int cost, index, usage;
	jit_value_t value;

	if(gen->contents[reg].is_long_end)
	{
		reg = get_long_pair_start(reg);
	}

	cost = 0;
	for(index = 0; index < gen->contents[reg].num_values; index++)
	{
		value = gen->contents[reg].values[index];
		usage = value_usage(regs, value);
		if((usage & VALUE_DEAD) != 0)
		{
			/* the value is not spilled */
			continue;
		}
		if((usage & VALUE_LIVE) != 0 && (usage & VALUE_USED) == 0)
		{
			/* the value has to be spilled anyway */
			/* NOTE: This is true for local register allocation,
			   review for future global allocator. */
			continue;
		}
		if(value->has_global_register)
		{
			if(value->in_global_register)
			{
				cost += COST_SPILL_CLEAN_GLOBAL;
			}
			else
			{
				cost += COST_SPILL_DIRTY_GLOBAL;
			}
		}
		else
		{
			if(value->in_frame)
			{
				cost += COST_SPILL_CLEAN;
			}
			else
			{
				cost += COST_SPILL_DIRTY;
			}
		}
	}

	if(gen->contents[reg].is_long_start)
	{
		return cost * 2;
	}

	if(other_reg >= 0)
	{
		for(index = 0; index < gen->contents[other_reg].num_values; index++)
		{
			value = gen->contents[other_reg].values[index];
			usage = value_usage(regs, value);
			if((usage & VALUE_DEAD) != 0)
			{
				/* the value is not spilled */
				continue;
			}
			if((usage & VALUE_LIVE) != 0 && (usage & VALUE_USED) == 0)
			{
				/* the value has to be spilled anyway */
				/* NOTE: This is true for local register allocation,
				   review for future global allocator. */
				continue;
			}
			if(value->has_global_register)
			{
				if(value->in_global_register)
				{
					cost += COST_SPILL_CLEAN_GLOBAL;
				}
				else
				{
					cost += COST_SPILL_DIRTY_GLOBAL;
				}
			}
			else
			{
				if(value->in_frame)
				{
					cost += COST_SPILL_CLEAN;
				}
				else
				{
					cost += COST_SPILL_DIRTY;
				}
			}
		}
	}

	return cost;
}

static int
thrashes_value(jit_gencode_t gen,
	       _jit_regdesc_t *desc, int reg, int other_reg,
	       _jit_regdesc_t *desc2)
{
	int reg2, other_reg2;

#if ALLOW_CLOBBER_GLOBAL
	if(desc2->value->has_global_register)
	{
		if(desc2->value->global_reg == reg)
		{
			if(desc && desc2->value == desc->value)
			{
				return 0;
			}
			return 1;
		}
		if(desc2->value->global_reg == other_reg)
		{
			return 1;
		}
	}
#endif

	if(desc2->value->in_register)
	{
		reg2 = desc2->value->reg;
		if(reg2 == reg)
		{
			if(are_values_equal(desc2, desc))
			{
				return 0;
			}
			return 1;
		}
		if(reg2 == other_reg)
		{
			return 1;
		}
		if(gen->contents[reg2].is_long_start)
		{
			other_reg2 = jit_reg_other_reg(reg2);
			if(other_reg2 == reg /*|| other_reg2 == other_reg*/)
			{
				return 1;
			}
		}
	}

	return 0;
}

static void
choose_scratch_register(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regclass_t *regclass;
	int reg_index, reg;
	int use_cost;
	int suitable_reg;
	int suitable_cost;
	int suitable_age;

#ifdef JIT_REG_DEBUG
	printf("choose_scratch_register(%d)\n", index);
#endif

	regclass = regs->scratch[index].regclass;

	suitable_reg = -1;
	suitable_cost = COST_TOO_MUCH;
	suitable_age = -1;
	for(reg_index = 0; reg_index < regclass->num_regs; reg_index++)
	{
		reg = regclass->regs[reg_index];
		if(jit_reg_is_used(regs->assigned, reg))
		{
			continue;
		}

		if(jit_reg_is_used(gen->permanent, reg))
		{
#if ALLOW_CLOBBER_GLOBAL
			use_cost = COST_CLOBBER_GLOBAL;
#else
			continue;
#endif
		}
		else
		{
			use_cost = 0;
		}

#if 0
		if(regs->ternary && regs->descs[0].value
		   && thrashes_value(gen, 0, reg, -1, &regs->descs[0]))
		{
			use_cost += COST_THRASH;
		}
		else if(regs->descs[1].value
			&& thrashes_value(gen, 0, reg, -1, &regs->descs[1]))
		{
			use_cost += COST_THRASH;
		}
		else if(regs->descs[2].value
			&& thrashes_value(gen, 0, reg, -1, &regs->descs[2]))
		{
			use_cost += COST_THRASH;
		}
#endif

		if(!jit_reg_is_used(regs->clobber, reg))
		{
			use_cost += compute_spill_cost(gen, regs, reg, -1);
		}

#ifdef JIT_REG_DEBUG
		printf("reg = %d, use_cost = %d\n", reg, use_cost);
#endif

		if(use_cost < suitable_cost
		   || (use_cost == suitable_cost
		       && gen->contents[reg].num_values > 0
		       && (IS_STACK_REG(reg)
			   || gen->contents[reg].age < suitable_age)))
		{
			suitable_reg = reg;
			suitable_cost = use_cost;
			suitable_age = gen->contents[reg].age;
		}
	}

	if(suitable_reg >= 0)
	{
		set_scratch_register(gen, regs, index, suitable_reg);
	}
	else
	{
		jit_exception_builtin(JIT_RESULT_COMPILE_ERROR);
	}
}

static void
choose_output_register(jit_gencode_t gen, _jit_regs_t *regs)
{
	_jit_regclass_t *regclass;
	int assigned_inreg1, assigned_inreg2;
	int suitable_inreg1, suitable_inreg2;
	int reg_index, reg, other_reg;
	int use_cost;
	int suitable_reg, suitable_other_reg;
	int suitable_cost;
	int suitable_age;

#ifdef JIT_REG_DEBUG
	printf("choose_output_register()\n");
#endif

	regclass = regs->descs[0].regclass;

	assigned_inreg1 = suitable_inreg1 = -1;
	if(regs->descs[1].value)
	{
		if(regs->descs[1].reg >= 0)
		{
			assigned_inreg1 = suitable_inreg1 = regs->descs[1].reg;
		}
		else if (regs->descs[1].value->in_register)
		{
			suitable_inreg1 = regs->descs[1].value->reg;
		}
	}

	assigned_inreg2 = suitable_inreg2 = -1;
	if(regs->descs[2].value)
	{
		if(regs->descs[2].reg >= 0)
		{
			assigned_inreg2 = suitable_inreg2 = regs->descs[2].reg;
		}
		else if (regs->descs[2].value->in_register)
		{
			suitable_inreg2 = regs->descs[2].value->reg;
		}
	}

	suitable_reg = -1;
	suitable_other_reg = -1;
	suitable_cost = COST_TOO_MUCH;
	suitable_age = -1;
	for(reg_index = 0; reg_index < regclass->num_regs; reg_index++)
	{
		reg = regclass->regs[reg_index];
		if(jit_reg_is_used(gen->inhibit, reg))
		{
			continue;
		}

		other_reg = jit_reg_get_pair(regs->descs[0].value->type, reg);
		if(other_reg >= 0 && jit_reg_is_used(gen->inhibit, other_reg))
		{
			continue;
		}

		if(jit_reg_is_used(gen->permanent, reg))
		{
			if(!regs->descs[0].value->has_global_register
			   || regs->descs[0].value->global_reg != reg)
			{
				/* It is not allowed to assign an output value to a global
				   register unless it is the very value the global register
				   contains. */
				continue;
			}
			if(regs->free_dest)
			{
				if(regs->descs[0].early_clobber
				   && regs->descs[0].value->in_global_register)
				{
					if(regs->descs[0].value == regs->descs[1].value)
					{
						continue;
					}
					if(regs->descs[0].value == regs->descs[2].value)
					{
						continue;
					}
				}
				use_cost = 0;
			}
			else if(regs->descs[0].value->in_global_register)
			{
				if(regs->descs[0].value == regs->descs[1].value)
				{
					use_cost = 0;
				}
				else if(regs->descs[0].value == regs->descs[2].value)
				{
					if(regs->commutative)
					{
						/* This depends on choose_input_order()
						   doing its job on the next step. */
						use_cost = 0;
					}
					else
					{
						continue;
					}
				}
				else
				{
					use_cost = COST_COPY;
				}
			}
			else
			{
				use_cost = COST_COPY;
			}
		}
		else
		{
			if(other_reg >= 0 && jit_reg_is_used(gen->permanent, other_reg))
			{
				continue;
			}
			if(regs->free_dest)
			{
				if(regs->descs[0].early_clobber
				   && (reg == suitable_inreg1 || reg == suitable_inreg2))
				{
					continue;
				}
				use_cost = 0;
			}
			else if(reg == assigned_inreg1)
			{
				use_cost = 0;
			}
			else if(reg == assigned_inreg2)
			{
				continue;
			}
			else if(reg == suitable_inreg1)
			{
				use_cost = 0;
			}
			else if(reg == suitable_inreg2)
			{
				if(regs->commutative)
				{
					/* This depends on choose_input_order()
					   doing its job on the next step. */
					use_cost = 0;
				}
#ifdef JIT_REG_STACK
				else if(regs->reversible && regs->no_pop)
				{
					/* This depends on choose_input_order()
					   doing its job on the next step. */
					use_cost = 0;
				}
#endif
				else
				{
					use_cost = COST_THRASH;
				}
			}
			else
			{
				use_cost = COST_COPY;
			}
			if(regs->descs[0].value->has_global_register)
			{
				use_cost += COST_GLOBAL_BIAS;
			}
		}

		if(!jit_reg_is_used(regs->clobber, reg)
		   && !(other_reg >= 0 && jit_reg_is_used(regs->clobber, other_reg)))
		{
			use_cost += compute_spill_cost(gen, regs, reg, other_reg);
		}

#ifdef JIT_REG_DEBUG
		printf("reg = %d, other_reg = %d, use_cost = %d\n", reg, other_reg, use_cost);
#endif

		if(use_cost < suitable_cost
		   || (use_cost == suitable_cost
		       && gen->contents[reg].num_values > 0
		       && gen->contents[reg].age < suitable_age))
		{
			suitable_reg = reg;
			suitable_other_reg = other_reg;
			suitable_cost = use_cost;
			suitable_age = gen->contents[reg].age;
		}
	}

	if(suitable_reg >= 0)
	{
		set_regdesc_register(gen, regs, 0, suitable_reg, suitable_other_reg);
	}
	else
	{
		jit_exception_builtin(JIT_RESULT_COMPILE_ERROR);
	}
}

/*
 * Select the best argument order for binary ops. The posibility to select
 * the order exists only for commutative ops and for some x87 floating point
 * instructions. Those x87 instructions have variants with reversed
 * destination register.
 */
static void
choose_input_order(jit_gencode_t gen, _jit_regs_t *regs)
{
	jit_value_t value;

	value = regs->descs[2].value;
	if(value && value != regs->descs[1].value
	   && ((value->in_register
		&& value->reg == regs->descs[0].reg)
	       || (value->in_global_register
		   && value->global_reg == regs->descs[0].reg)))
	{
#ifdef JIT_REG_STACK
		if(regs->reversible && regs->no_pop)
		{
			regs->dest_input_index = 2;
		}
		else
#endif
		{
			if(regs->commutative)
			{
				swap_values(&regs->descs[1], &regs->descs[2]);
			}
			regs->dest_input_index = 1;
		}
	}
	else if(regs->descs[1].value)
	{
		regs->dest_input_index = 1;
	}
	else
	{
		regs->dest_input_index = 0;
	}
}

static void
choose_input_register(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regclass_t *regclass;
	_jit_regdesc_t *desc;
	_jit_regdesc_t *desc2;
	int reg_index, reg, other_reg;
	int use_cost;
	int suitable_reg, suitable_other_reg;
	int suitable_cost;
	int suitable_age;
	int clobber;

#ifdef JIT_REG_DEBUG
	printf("choose_input_register(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value)
	{
		jit_exception_builtin(JIT_RESULT_COMPILE_ERROR);
	}

	regclass = regs->descs[index].regclass;

	if(index == regs->dest_input_index)
	{
		desc2 = &regs->descs[0];
	}
	else
	{
		desc2 = desc;
	}

	suitable_reg = -1;
	suitable_other_reg = -1;
	suitable_cost = COST_TOO_MUCH;
	suitable_age = -1;
	for(reg_index = 0; reg_index < regclass->num_regs; reg_index++)
	{
		reg = regclass->regs[reg_index];
		if(jit_reg_is_used(regs->assigned, reg))
		{
			continue;
		}

		other_reg = jit_reg_get_pair(desc->value->type, reg);
		if(other_reg >= 0 && jit_reg_is_used(regs->assigned, other_reg))
		{
			continue;
		}

		if((desc->value->in_global_register && desc->value->global_reg == reg)
		   || (desc->value->in_register && desc->value->reg == reg))
		{
			use_cost = 0;
		}
		else
		{
			use_cost = COST_COPY;
		}
		if(desc2->value->has_global_register && desc2->value->global_reg != reg)
		{
			use_cost += COST_GLOBAL_BIAS;
		}

		if(index != 0 && regs->ternary && regs->descs[0].value
		   && thrashes_value(gen, desc, reg, other_reg, &regs->descs[0]))
		{
			use_cost += COST_THRASH;
		}
		else if(index != 1 && regs->descs[1].value
			&& thrashes_value(gen, desc, reg, other_reg, &regs->descs[1]))
		{
			use_cost += COST_THRASH;
		}
		else if(index != 2 && regs->descs[2].value
			&& thrashes_value(gen, desc, reg, other_reg, &regs->descs[2]))
		{
			use_cost += COST_THRASH;
		}

		clobber = clobbers_register(gen, regs, index, reg, other_reg);
		if((clobber & CLOBBER_INPUT_VALUE) != 0)
		{
			if(desc->used)
			{
				use_cost += COST_SPILL_CLEAN;
			}
		}
		if((clobber & (CLOBBER_REG | CLOBBER_OTHER_REG)) != 0)
		{
			if(jit_reg_is_used(gen->permanent, reg))
			{
				continue;
			}
			if(other_reg >= 0 && jit_reg_is_used(gen->permanent, other_reg))
			{
#if ALLOW_CLOBBER_GLOBAL
				use_cost += COST_CLOBBER_GLOBAL;
#else
				continue;
#endif
			}
			if(!jit_reg_is_used(regs->clobber, reg)
			   && !(other_reg >= 0 && jit_reg_is_used(regs->clobber, other_reg)))
			{
				use_cost += compute_spill_cost(gen, regs, reg, other_reg);
			}
		}

#ifdef JIT_REG_DEBUG
		printf("reg = %d, other_reg = %d, use_cost = %d\n", reg, other_reg, use_cost);
#endif

		if(use_cost < suitable_cost
		   || (use_cost == suitable_cost
		       && gen->contents[reg].num_values > 0
		       && gen->contents[reg].age < suitable_age))
		{
			/* This is the oldest suitable register of this type */
			suitable_reg = reg;
			suitable_other_reg = other_reg;
			suitable_cost = use_cost;
			suitable_age = gen->contents[reg].age;
		}
	}

	if(suitable_reg >= 0)
	{
		set_regdesc_register(gen, regs, index, suitable_reg, suitable_other_reg);
	}
	else
	{
		jit_exception_builtin(JIT_RESULT_COMPILE_ERROR);
	}
}

/*
 * Assign diplicate input value to the same register if possible.
 * The first value has to be already assigned. The second value
 * is assigned to the same register if it is equal to the first
 * and neither of them is clobbered.
 */
static void
check_duplicate_value(_jit_regs_t *regs, _jit_regdesc_t *desc1, _jit_regdesc_t *desc2)
{
	if(desc2->reg < 0 && desc1->reg >= 0 && are_values_equal(desc1, desc2)
#ifdef JIT_REG_STACK
	   && (!IS_STACK_REG(desc1->reg) || regs->x87_arith)
#endif
	   && !desc1->early_clobber && !desc2->early_clobber)
	{
		desc2->reg = desc1->reg;
		desc2->other_reg = desc1->other_reg;
		desc2->duplicate = 1;
	}
}

#ifdef JIT_REG_STACK

/*
 * For x87 instructions choose between pop and no-pop variants.
 */
static void
select_nopop_or_pop(jit_gencode_t gen, _jit_regs_t *regs)
{
	int keep1, keep2;

	if(!regs->x87_arith || !regs->descs[1].value || !regs->descs[2].value)
	{
		return;
	}

	/* Equal values should be assigned to one register and this is
	   going to work only with no-pop instructions. */
	if(are_values_equal(&regs->descs[1], &regs->descs[2]))
	{
		regs->no_pop = 1;
		return;
	}

	/* Determine if we might want to keep input values in registers
	   after the instruction completion. */
	if(regs->descs[1].value->in_register)
	{
		keep1 = is_register_alive(gen, regs, regs->descs[1].value->reg);
	}
	else
	{
		keep1 = (regs->descs[1].used
			 && (regs->descs[1].value != regs->descs[0].value)
			 && !regs->descs[1].clobber);
	}
	if(regs->descs[2].value->in_register)
	{
		keep2 = is_register_alive(gen, regs, regs->descs[2].value->reg);
	}
	else
	{
		keep2 = (regs->descs[2].used
			 && (regs->descs[2].value != regs->descs[0].value)
			 && !regs->descs[2].clobber);
	}

	regs->no_pop = (keep1 || keep2);
}

static void
select_stack_order(jit_gencode_t gen, _jit_regs_t *regs)
{
	_jit_regdesc_t *desc1;
	_jit_regdesc_t *desc2;
	int top_index;

#ifdef JIT_REG_DEBUG
	printf("select_stack_order()\n");
#endif

	if(!regs->x87_arith || regs->wanted_stack_count != 2)
	{
		return;
	}

	desc1 = &regs->descs[1];
	desc2 = &regs->descs[2];

	/* Choose instruction that results into fewer exchanges. If either
	   of two arguments may be on the stack top choose the second to be
	   on top. */
	/* TODO: See if the next instruction wants the output or remaining
	   input to be on the stack top. */
 	if(desc2->copy || desc2->load)
	{
		/* the second argument is going to be on the top */
		top_index = 2;
	}
	else if(desc1->copy || desc1->load)
	{
		/* the first argument is going to be on the top */
		top_index = 1;
	}
	else if(desc2->value->reg == (gen->reg_stack_top - 1))
	{
		/* the second argument is already on the top */
		top_index = 2;
	}
	else if(desc1->value->reg == (gen->reg_stack_top - 1))
	{
		/* the first argument is already on the top */
		top_index = 1;
	}
	else
	{
		top_index = 2;
	}

	if(regs->no_pop)
	{
		regs->flip_args = (top_index == 2);
	}
	else if(regs->reversible)
	{
		if(top_index == 2)
		{
			regs->flip_args = 1;
			regs->dest_input_index = 1;
		}
		else
		{
			regs->flip_args = 0;
			regs->dest_input_index = 2;
		}
	}
	else /*if(regs->commutative)*/
	{
		regs->flip_args = 1;
		regs->dest_input_index = 1;

		if(top_index != 2)
		{
			swap_values(&regs->descs[1], &regs->descs[2]);
		}
	}

#ifdef JIT_REG_DEBUG
	printf("top_index = %d, flip_args = %d, dest_input_index = %d\n",
	       top_index, regs->flip_args, regs->dest_input_index);
#endif
}

static void
adjust_assignment(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regdesc_t *desc, *desc2;

#ifdef JIT_REG_DEBUG
	printf("adjust_assignment(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value || !IS_STACK_REG(desc->reg))
	{
		return;
	}

	if(regs->wanted_stack_count == 0)
	{
		/* an op with stack dest and non-stack args */
#ifdef JIT_REG_DEBUG
		if(index != 0 || regs->ternary)
		{
			printf("*** Wrong stack register count! ***\n");
			abort();
		}
#endif
		desc->reg = gen->reg_stack_top;
	}
	else if(regs->wanted_stack_count == 1)
	{
		/* either a unary op or a binary op with duplicate value */
		desc->reg = gen->reg_stack_top - regs->loaded_stack_count;
	}
	else if(regs->wanted_stack_count == 2)
	{
		/* a binary op */

		/* find the input value the output goes to */
		if(index == 0)
		{
			if(regs->x87_arith)
			{
				index = regs->dest_input_index;
			}
			else
			{
				index = 2;
			}
			desc2 = &regs->descs[index];
		}
		else
		{
			desc2 = desc;
		}

		if(regs->flip_args)
		{
			if(regs->x87_arith && index == 1
			   && desc2->value->in_register && !desc2->copy)
			{
				desc->reg = desc2->value->reg;
			}
			else
			{
				desc->reg = (gen->reg_stack_top
					     - regs->loaded_stack_count
					     + index - 1);
			}
		}
		else
		{
			if(regs->x87_arith && index == 2
			   && desc2->value->in_register && !desc2->copy)
			{
				desc->reg = desc2->value->reg;
			}
			else
			{
				desc->reg = (gen->reg_stack_top
					     - regs->loaded_stack_count
					     + regs->wanted_stack_count
					     - index);
			}
		}
	}

#ifdef JIT_REG_DEBUG
	printf("reg = %d\n", desc->reg);
	if(desc->reg < JIT_REG_STACK_START || desc->reg > JIT_REG_STACK_END)
	{
		printf("*** Invalid stack register! ***\n");
		abort();
	}
#endif
}

#endif

/*
 * Associate a temporary with register.
 */
static void
bind_temporary(jit_gencode_t gen, int reg, int other_reg)
{
#ifdef JIT_REG_DEBUG
	printf("bind_temporary(reg = %d, other_reg = %d)\n", reg, other_reg);
#endif

	gen->contents[reg].num_values = 0;
	gen->contents[reg].age = 0;
	gen->contents[reg].used_for_temp = 1;
	gen->contents[reg].is_long_end = 0;
	gen->contents[reg].is_long_start = 0;
	if(other_reg >= 0)
	{
		gen->contents[other_reg].num_values = 0;
		gen->contents[other_reg].age = 0;
		gen->contents[other_reg].used_for_temp = 1;
		gen->contents[other_reg].is_long_end = 0;
		gen->contents[other_reg].is_long_start = 0;
	}
}

/*
 * Associate value with register.
 */
static void
bind_value(jit_gencode_t gen, jit_value_t value, int reg, int other_reg, int still_in_frame)
{
#ifdef JIT_REG_DEBUG
	printf("bind_value(value = ");
	jit_dump_value(stdout, jit_value_get_function(value), value, 0);
	printf(", reg = %d, other_reg = %d, still_in_frame = %d)\n",
	       reg, other_reg, still_in_frame);
#endif

	if(value->has_global_register && value->global_reg == reg)
	{
		value->in_register = 0;
		value->in_global_register = 1;
		return;
	}

	if(value->is_constant)
	{
		still_in_frame = 0;
	}

#ifdef JIT_REG_DEBUG
	if(gen->contents[reg].num_values == JIT_MAX_REG_VALUES)
	{
		printf("*** Too many values for one register! ***\n");
		abort();
	}
#endif

	gen->contents[reg].values[gen->contents[reg].num_values] = value;
	++(gen->contents[reg].num_values);
	gen->contents[reg].age = gen->current_age;
	gen->contents[reg].used_for_temp = 0;
	gen->contents[reg].is_long_end = 0;
	if(other_reg == -1)
	{
		gen->contents[reg].is_long_start = 0;
	}
	else
	{
		gen->contents[reg].is_long_start = 1;
		gen->contents[other_reg].num_values = 0;
		gen->contents[other_reg].age = gen->current_age;
		gen->contents[other_reg].used_for_temp = 0;
		gen->contents[other_reg].is_long_start = 0;
		gen->contents[other_reg].is_long_end = 1;
	}
	++(gen->current_age);

	/* Adjust the value to reflect that it is in "reg", and maybe the frame */
	value->in_register = 1;
	if(value->has_global_register)
	{
		value->in_global_register = still_in_frame;
	}
	else
	{
		value->in_frame = still_in_frame;
	}
	value->reg = reg;
}

/*
 * Disassociate value with register.
 */
static void
unbind_value(jit_gencode_t gen, jit_value_t value, int reg, int other_reg)
{
	int index;

#ifdef JIT_REG_DEBUG
	printf("unbind_value(value = ");
	jit_dump_value(stdout, jit_value_get_function(value), value, 0);
	printf(", reg = %d, other_reg = %d)\n", reg, other_reg);
#endif

	if(!value->in_register || value->reg != reg)
	{
		return;
	}

	value->in_register = 0;
	value->reg = -1;

	for(index = gen->contents[reg].num_values - 1; index >= 0; --index)
	{
		if(gen->contents[reg].values[index] == value)
		{
			--(gen->contents[reg].num_values);
			for(; index < gen->contents[reg].num_values; index++)
			{
				gen->contents[reg].values[index] = gen->contents[reg].values[index + 1];
			}
			break;
		}
	}

	if(gen->contents[reg].num_values == 0 && other_reg >= 0)
	{
		gen->contents[reg].is_long_start = 0;
		gen->contents[other_reg].is_long_end = 0;
	}
}

/*
 * Swap the contents of a register and the top of the register stack. If
 * the register is not a stack register then the function has no effect.
 */
#ifdef JIT_REG_STACK
static void
exch_stack_top(jit_gencode_t gen, int reg, int pop)
{
	int top, index;
	int num_values, used_for_temp, age;
	jit_value_t value1, value2;

#ifdef JIT_REG_DEBUG
	printf("exch_stack_top(reg = %d, pop = %d)\n", reg, pop);
#endif

	if(!IS_STACK_REG(reg))
	{
		return;
	}

	/* Find the top of the stack. */
	top = gen->reg_stack_top - 1;

	if(pop)
	{
		/* Generate move/pop-top instruction. */
		_jit_gen_move_top(gen, reg);
		--(gen->reg_stack_top);
	}
	else
	{
		/* Generate exchange instruction. */
		_jit_gen_exch_top(gen, reg);
	}

	/* Update information about the contents of the registers.  */
	for(index = 0;
	    index < gen->contents[reg].num_values || index < gen->contents[top].num_values;
	    index++)
	{
		value1 = (index < gen->contents[top].num_values
			  ? gen->contents[top].values[index] : 0);
		value2 = (index < gen->contents[reg].num_values
			  ? gen->contents[reg].values[index] : 0);

		if(value1)
		{
			value1->reg = reg;
		}
		gen->contents[reg].values[index] = value1;

		if(pop)
		{
			if(value2)
			{
				value2->in_register = 0;
				value2->reg = -1;
			}
			gen->contents[top].values[index] = 0;
		}
		else
		{
			if(value2)
			{
				value2->reg = top;
			}
			gen->contents[top].values[index] = value2;
		}
	}

	if(pop)
	{
		num_values = 0;
		used_for_temp = 0;
		age = 0;
	}
	else
 	{
		num_values = gen->contents[reg].num_values;
		used_for_temp = gen->contents[reg].used_for_temp;
		age = gen->contents[reg].age;
	}
	gen->contents[reg].num_values = gen->contents[top].num_values;
	gen->contents[reg].used_for_temp = gen->contents[top].used_for_temp;
	gen->contents[reg].age = gen->contents[top].age;
	gen->contents[top].num_values = num_values;
	gen->contents[top].used_for_temp = used_for_temp;
	gen->contents[top].age = age;
}
#endif

/*
 * Drop value from the register and optionally bind a temporary value in place of it.
 */
static void
free_value(jit_gencode_t gen, jit_value_t value, int reg, int other_reg, int temp)
{
#ifdef JIT_REG_DEBUG
	printf("free_value(value = ");
	jit_dump_value(stdout, jit_value_get_function(value), value, 0);
	printf(", reg = %d, other_reg = %d, temp = %d)\n", reg, other_reg, temp);
#endif

	/* Never free global registers. */
	if(value->has_global_register && value->global_reg == reg)
	{
		return;
	}

	if(gen->contents[reg].num_values == 1)
	{
		if(temp)
		{
			unbind_value(gen, value, reg, other_reg);
			bind_temporary(gen, reg, other_reg);
			return;
		}
#ifdef JIT_REG_STACK
		if(IS_STACK_REG(reg))
		{
			/* Free stack register. */
			exch_stack_top(gen, reg, 1);
			return;
		}
#endif
	}

	unbind_value(gen, value, reg, other_reg);
}

/*
 * Save the value from the register into its frame position and optionally free it.
 * If the value is already in the frame or is a constant then it is not saved but
 * the free option still applies to them.
 */
static void
save_value(jit_gencode_t gen, jit_value_t value, int reg, int other_reg, int free)
{
#ifdef JIT_REG_DEBUG
	printf("save_value(value = ");
	jit_dump_value(stdout, jit_value_get_function(value), value, 0);
	printf(", reg = %d, other_reg = %d, free=%d)\n", reg, other_reg, free);
#endif
	/* First take care of values that reside in global registers. */
	if(value->has_global_register)
	{
		/* Never free global registers. */
		if(value->global_reg == reg)
		{
			return;
		}

		if(!value->in_global_register)
		{
			_jit_gen_spill_reg(gen, reg, other_reg, value);
			value->in_global_register = 1;
		}
		if(free)
		{
			unbind_value(gen, value, reg, other_reg);
		}
		return;
	}

	/* Take care of constants and values that are already in frame. */
	if(value->is_constant || value->in_frame)
	{
		if(free)
		{
			free_value(gen, value, reg, other_reg, 0);
		}
		return;
	}

	/* Now really save the value into the frame. */
#ifdef JIT_REG_STACK
	if(IS_STACK_REG(reg))
	{
		int top;

		/* Find the top of the stack. */
		top = gen->reg_stack_top - 1;

		/* Move the value on the stack top if it is not already there. */
		if(top != reg)
		{
			exch_stack_top(gen, reg, 0);
		}

		if(free)
		{
			if(gen->contents[top].num_values == 1)
			{
				_jit_gen_spill_top(gen, top, value, 1);
				--(gen->reg_stack_top);
			}
			else
			{
				_jit_gen_spill_top(gen, top, value, 0);
			}
			unbind_value(gen, value, top, 0);
		}
		else
		{
			_jit_gen_spill_top(gen, top, value, 0);
		}
	}
	else
#endif
	{
		_jit_gen_spill_reg(gen, reg, other_reg, value);
		if(free)
		{
			unbind_value(gen, value, reg, other_reg);
		}
	}

	value->in_frame = 1;
}

/*
 * Spill a specific register.
 */
static void
spill_register(jit_gencode_t gen, int reg)
{
	int other_reg, index;
	jit_value_t value;

#ifdef JIT_REG_DEBUG
	printf("spill_register(reg = %d)\n", reg);
#endif

	/* Find the other register in a long pair */
	if(gen->contents[reg].is_long_start)
	{
		other_reg = jit_reg_other_reg(reg);
	}
	else if(gen->contents[reg].is_long_end)
	{
		other_reg = reg;
		reg = get_long_pair_start(reg);
	}
	else
	{
		other_reg = -1;
	}

	for(index = gen->contents[reg].num_values - 1; index >= 0; --index)
	{
		value = gen->contents[reg].values[index];
		save_value(gen, value, reg, other_reg, 1);
	}
}

/*
 * Spill a register clobbered by the instruction.
 */
static void
spill_clobbered_register(jit_gencode_t gen, _jit_regs_t *regs, int reg)
{
	int other_reg, index, usage;
	jit_value_t value;

#ifdef JIT_REG_DEBUG
	printf("spill_clobbered_register(reg = %d)\n", reg);
#endif

#ifdef JIT_REG_STACK
	/* For a stack register spill it in two passes. First drop values that
	   reqiure neither spilling nor a generation of the free instruction.
	   Then lazily exchange the register with the top and spill or free it
	   as necessary. This approach might save a exch/free instructions in
	   certain cases. */
	if(IS_STACK_REG(reg))
	{
		for(index = gen->contents[reg].num_values - 1; index >= 0; --index)
		{
			if(gen->contents[reg].num_values == 1)
			{
				break;
			}

			value = gen->contents[reg].values[index];
			usage = value_usage(regs, value);
			if((usage & VALUE_INPUT) != 0)
			{
				continue;
			}
			if((usage & VALUE_DEAD) != 0 || value->in_frame)
			{
				unbind_value(gen, value, reg, -1);
			}
		}
		for(index = gen->contents[reg].num_values - 1; index >= 0; --index)
		{
			int top;

			value = gen->contents[reg].values[index];
			usage = value_usage(regs, value);
			if((usage & VALUE_INPUT) != 0)
			{
				if((usage & VALUE_DEAD) != 0 || value->in_frame)
				{
					continue;
				}

				top = gen->reg_stack_top - 1;
				if(reg != top)
				{
					exch_stack_top(gen, reg, 0);
					reg = top;
				}

				save_value(gen, value, reg, -1, 0);
			}
			else
			{
				top = gen->reg_stack_top - 1;
				if(reg != top)
				{
					exch_stack_top(gen, reg, 0);
					reg = top;
				}

				if((usage & VALUE_DEAD) != 0 || value->in_frame)
				{
					free_value(gen, value, reg, -1, 0);
				}
				else
				{
					save_value(gen, value, reg, -1, 1);
				}
			}
		}
	}
	else
#endif
	{
		/* Find the other register in a long pair */
		if(gen->contents[reg].is_long_start)
		{
			other_reg = jit_reg_other_reg(reg);
		}
		else if(gen->contents[reg].is_long_end)
		{
			other_reg = reg;
			reg = get_long_pair_start(reg);
		}
		else
		{
			other_reg = -1;
		}

		for(index = gen->contents[reg].num_values - 1; index >= 0; --index)
		{
			value = gen->contents[reg].values[index];
			usage = value_usage(regs, value);
			if((usage & VALUE_DEAD) == 0)
			{
				if((usage & VALUE_INPUT) == 0)
				{
					save_value(gen, value, reg, other_reg, 1);
				}
				else
				{
					save_value(gen, value, reg, other_reg, 0);
				}
			}
			else
			{
				if((usage & VALUE_INPUT) == 0)
				{
					free_value(gen, value, reg, other_reg, 0);
				}
			}
		}
	}
}

static void
update_age(jit_gencode_t gen, _jit_regdesc_t *desc)
{
	int reg, other_reg;

	reg = desc->value->reg;
	if(gen->contents[reg].is_long_start)
	{
		other_reg = jit_reg_other_reg(reg);
	}
	else
	{
		other_reg = -1;
	}

	gen->contents[reg].age = gen->current_age;
	if(other_reg >= 0)
	{
		gen->contents[other_reg].age = gen->current_age;
	}
	++(gen->current_age);
}

static void
save_input_value(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regdesc_t *desc;
	int reg, other_reg;

#ifdef JIT_REG_DEBUG
	printf("save_input_value(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value || !desc->value->in_register || !desc->store)
	{
		return;
	}

	reg = desc->value->reg;
	if(gen->contents[reg].is_long_start)
	{
		other_reg = jit_reg_other_reg(reg);
	}
	else
	{
		other_reg = -1;
	}

	if(desc->thrash)
	{
		save_value(gen, desc->value, reg, other_reg, 1);
	}
	else
	{
		save_value(gen, desc->value, reg, other_reg, 0);
	}
}

static void
free_output_value(jit_gencode_t gen, _jit_regs_t *regs)
{
	_jit_regdesc_t *desc;
	int reg, other_reg;

#ifdef JIT_REG_DEBUG
	printf("free_output_value()\n");
#endif

	desc = &regs->descs[0];
	if(!(desc->value && desc->value->in_register))
	{
		return;
	}
	if(desc->value == regs->descs[1].value || desc->value == regs->descs[2].value)
	{
		return;
	}

	reg = desc->value->reg;
	if(gen->contents[reg].is_long_start)
	{
		other_reg = jit_reg_other_reg(reg);
	}
	else
	{
		other_reg = -1;
	}

	free_value(gen, desc->value, reg, other_reg, 0);
}

static void
load_input_value(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regdesc_t *desc;

#ifdef JIT_REG_DEBUG
	printf("load_input_value(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value || desc->duplicate)
	{
		return;
	}

	if(desc->value->has_global_register)
	{
		if(desc->value->in_global_register && desc->value->global_reg == desc->reg)
		{
			return;
		}
		if(desc->value->in_register && desc->value->reg == desc->reg)
		{
			update_age(gen, desc);
			return;
		}
		_jit_gen_load_value(gen, desc->reg, desc->other_reg, desc->value);
	}
	else if(desc->value->in_register)
	{
		if(desc->value->reg == desc->reg)
		{
			update_age(gen, desc);
			if(IS_STACK_REG(desc->reg))
			{
				desc->stack_reg = desc->reg;
			}
			return;
		}

#ifdef JIT_REG_STACK
		if(IS_STACK_REG(desc->reg))
		{
			_jit_gen_load_value(gen, gen->reg_stack_top, -1, desc->value);
			desc->stack_reg = gen->reg_stack_top++;
			bind_temporary(gen, desc->stack_reg, -1);
		}
		else
#endif
		{
			_jit_gen_load_value(gen, desc->reg, desc->other_reg, desc->value);
			bind_temporary(gen, desc->reg, desc->other_reg);
		}
	}
	else
	{
#ifdef JIT_REG_STACK
		if(IS_STACK_REG(desc->reg))
		{
			_jit_gen_load_value(gen, gen->reg_stack_top, -1, desc->value);
			desc->stack_reg = gen->reg_stack_top++;
			bind_value(gen, desc->value, desc->stack_reg, -1, 1);
		}
		else
#endif
		{
			_jit_gen_load_value(gen, desc->reg, desc->other_reg, desc->value);
			bind_value(gen, desc->value, desc->reg, desc->other_reg, 1);
		}
	}
}

#ifdef JIT_REG_STACK
static void
move_input_value(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regdesc_t *desc;
	int src_reg, dst_reg;

#ifdef JIT_REG_DEBUG
	printf("move_input_value(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value || desc->duplicate || !desc->value->in_register)
	{
		return;
	}
	if(!IS_STACK_REG(desc->value->reg))
	{
		return;
	}

	if(desc->copy)
	{
		src_reg = desc->stack_reg;
		if(src_reg < 0)
		{
			return;
		}
	}
	else
	{
		src_reg = desc->value->reg;
	}

	if(desc->reg < gen->reg_stack_top)
	{
		dst_reg = desc->reg;
	}
	else
	{
		dst_reg = gen->reg_stack_top - 1;
	}

	if(src_reg != dst_reg)
	{
		if(src_reg != (gen->reg_stack_top - 1))
		{
			exch_stack_top(gen, src_reg, 0);
		}
		if(dst_reg != (gen->reg_stack_top - 1))
		{
			exch_stack_top(gen, dst_reg, 0);
		}
	}
}
#endif

#ifdef JIT_REG_STACK
static void
pop_input_value(jit_gencode_t gen, _jit_regs_t *regs, int index)
{
	_jit_regdesc_t *desc;

#ifdef JIT_REG_DEBUG
	printf("pop_input_value(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value || desc->duplicate)
	{
		return;
	}

	if(IS_STACK_REG(desc->reg))
	{
		if(desc->copy)
		{
			gen->contents[desc->reg].used_for_temp = 0;
		}
		else
		{
			unbind_value(gen, desc->value, desc->reg, 0);
		}
		--(gen->reg_stack_top);
	}
}
#endif

static void
commit_input_value(jit_gencode_t gen, _jit_regs_t *regs, int index, int killed)
{
	_jit_regdesc_t *desc;
	int reg, other_reg;

#ifdef JIT_REG_DEBUG
	printf("commit_input_value(%d)\n", index);
#endif

	desc = &regs->descs[index];
	if(!desc->value || desc->duplicate)
	{
		return;
	}

#ifdef JIT_REG_STACK
	if(!IS_STACK_REG(desc->reg))
	{
		killed = 0;
	}
#endif

	if(desc->copy)
	{
#ifdef JIT_REG_STACK
		if(killed)
		{
			killed = 0;
		}
		else
#endif
		{
			gen->contents[desc->reg].used_for_temp = 0;
			if(desc->other_reg >= 0)
			{
				gen->contents[desc->other_reg].used_for_temp = 0;
			}
		}
	}

#ifdef JIT_REG_STACK
	if(!killed && desc->kill && desc->value->in_register)
#else
	if(desc->kill && desc->value->in_register)
#endif
	{
		reg = desc->value->reg;
		if(gen->contents[reg].is_long_start)
		{
			other_reg = jit_reg_other_reg(reg);
		}
		else
		{
			other_reg = -1;
		}
		free_value(gen, desc->value, reg, other_reg, 0);
	}

#ifdef JIT_REG_DEBUG
	printf("value = ");
	jit_dump_value(stdout, jit_value_get_function(desc->value), desc->value, 0);
	printf("\n");
	printf("value->in_register = %d\n", desc->value->in_register);
	printf("value->reg = %d\n", desc->value->reg);
	printf("value->in_global_register = %d\n", desc->value->in_global_register);
	printf("value->global_reg = %d\n", desc->value->global_reg);
	printf("value->in_frame = %d\n", desc->value->in_frame);
#endif
}

static void
commit_output_value(jit_gencode_t gen, _jit_regs_t *regs, int push_stack_top)
{
	_jit_regdesc_t *desc;

#ifdef JIT_REG_DEBUG
	printf("commit_output_value()\n");
#endif

	desc = &regs->descs[0];
	if(!desc->value)
	{
		return;
	}

#ifdef JIT_REG_STACK
	if(IS_STACK_REG(desc->reg) && push_stack_top)
	{
		++(gen->reg_stack_top);
	}
#endif
	bind_value(gen, desc->value, desc->reg, desc->other_reg, 0);

	if(!desc->used)
	{
		if(desc->live)
		{
			save_value(gen, desc->value, desc->reg, desc->other_reg, 1);
		}
		else
		{
			free_value(gen, desc->value, desc->reg, desc->other_reg, 0);
		}
	}
	else if(desc->kill)
	{
		save_value(gen, desc->value, desc->reg, desc->other_reg, 1);
	}

#ifdef JIT_REG_DEBUG
	printf("value = ");
	jit_dump_value(stdout, jit_value_get_function(desc->value), desc->value, 0);
	printf("\n");
	printf("value->in_register = %d\n", desc->value->in_register);
	printf("value->reg = %d\n", desc->value->reg);
	printf("value->in_global_register = %d\n", desc->value->in_global_register);
	printf("value->global_reg = %d\n", desc->value->global_reg);
	printf("value->in_frame = %d\n", desc->value->in_frame);
#endif
}

/*@
 * @deftypefun void _jit_regs_lookup (char *name)
 * Get the pseudo register by its name.
 * @end deftypefun
@*/
int
_jit_regs_lookup(char *name)
{
	int reg;
	if(name)
	{
		for(reg = 0; reg < JIT_NUM_REGS; reg++)
		{
			if(strcmp(jit_reg_name(reg), name) == 0)
			{
				return reg;
			}
		}
	}
	return -1;
}

/*@
 * @deftypefun void _jit_regs_alloc_global (jit_gencode_t gen, jit_function_t func)
 * Perform global register allocation on the values in @code{func}.
 * This is called during function compilation just after variable
 * liveness has been computed.
 * @end deftypefun
@*/
void _jit_regs_alloc_global(jit_gencode_t gen, jit_function_t func)
{
#if JIT_NUM_GLOBAL_REGS != 0
	jit_value_t candidates[JIT_NUM_GLOBAL_REGS];
	int num_candidates = 0;
	int index, reg, posn, num;
	jit_pool_block_t block;
	jit_value_t value, temp;

	/* If the function has a "try" block, then don't do global allocation
	   as the "longjmp" for exception throws will wipe out global registers */
	if(func->has_try)
	{
		return;
	}

	/* If the current function involves a tail call, then we don't do
	   global register allocation and we also prevent the code generator
	   from using any of the callee-saved registers.  This simplifies
	   tail calls, which don't have to worry about restoring such registers */
	if(func->builder->has_tail_call)
	{
		for(reg = 0; reg < JIT_NUM_REGS; ++reg)
		{
			if((jit_reg_flags(reg) & (JIT_REG_FIXED|JIT_REG_CALL_USED)) == 0)
			{
				jit_reg_set_used(gen->permanent, reg);
			}
		}
		return;
	}

	/* Scan all values within the function, looking for the most used.
	   We will replace this with a better allocation strategy later */
	block = func->builder->value_pool.blocks;
	num = (int)(func->builder->value_pool.elems_per_block);
	while(block != 0)
	{
		if(!(block->next))
		{
			num = (int)(func->builder->value_pool.elems_in_last);
		}
		for(posn = 0; posn < num; ++posn)
		{
			value = (jit_value_t)(block->data + posn * sizeof(struct _jit_value));
			if(value->global_candidate && value->usage_count >= JIT_MIN_USED &&
			   !(value->is_addressable) && !(value->is_volatile))
			{
				/* Insert this candidate into the list, ordered on count */
				index = 0;
				while(index < num_candidates &&
				      value->usage_count <= candidates[index]->usage_count)
				{
					++index;
				}
				while(index < num_candidates)
				{
					temp = candidates[index];
					candidates[index] = value;
					value = temp;
					++index;
				}
				if(index < JIT_NUM_GLOBAL_REGS)
				{
					candidates[num_candidates++] = value;
				}
			}
		}
		block = block->next;
	}

	/* Allocate registers to the candidates.  We allocate from the top-most
	   register in the allocation order, because some architectures like
	   PPC require global registers to be saved top-down for efficiency */
	reg = JIT_NUM_REGS - 1;
	for(index = 0; index < num_candidates; ++index)
	{
		while(reg >= 0 && (jit_reg_flags(reg) & JIT_REG_GLOBAL) == 0)
		{
			--reg;
		}
		candidates[index]->has_global_register = 1;
		candidates[index]->in_global_register = 1;
		candidates[index]->global_reg = (short)reg;
		jit_reg_set_used(gen->touched, reg);
		jit_reg_set_used(gen->permanent, reg);
		--reg;
	}

#endif
}

/*@
 * @deftypefun void _jit_regs_init_for_block (jit_gencode_t gen)
 * Initialize the register allocation state for a new block.
 * @end deftypefun
@*/
void
_jit_regs_init_for_block(jit_gencode_t gen)
{
	int reg;
	gen->current_age = 1;
	for(reg = 0; reg < JIT_NUM_REGS; ++reg)
	{
		/* Clear everything except permanent and fixed registers */
		if(!jit_reg_is_used(gen->permanent, reg)
		   && (jit_reg_flags(reg) & JIT_REG_FIXED) == 0)
		{
			gen->contents[reg].num_values = 0;
			gen->contents[reg].is_long_start = 0;
			gen->contents[reg].is_long_end = 0;
			gen->contents[reg].age = 0;
			gen->contents[reg].used_for_temp = 0;
		}
#ifdef JIT_REG_STACK
		gen->reg_stack_top = JIT_REG_STACK_START;
#endif
	}
	gen->inhibit = jit_regused_init;
}

/*@
 * @deftypefun void _jit_regs_spill_all (jit_gencode_t gen)
 * Spill all of the temporary registers to memory locations.
 * Normally used at the end of a block, but may also be used in
 * situations where a value must be in a certain register and
 * it is too hard to swap things around to put it there.
 * @end deftypefun
@*/
void
_jit_regs_spill_all(jit_gencode_t gen)
{
	int reg;

#ifdef JIT_REG_DEBUG
	printf("enter _jit_regs_spill_all\n");
#endif

	for(reg = 0; reg < JIT_NUM_REGS; reg++)
	{
		/* Skip this register if it is permanent or fixed */
		if(jit_reg_is_used(gen->permanent, reg)
		   || (jit_reg_flags(reg) & JIT_REG_FIXED) != 0)
		{
			continue;
		}

		/* If this is a stack register, then we need to find the
		   register that contains the top-most stack position,
		   because we must spill stack registers from top down.
		   As we spill each one, something else will become the top */
#ifdef JIT_REG_STACK
		if(IS_STACK_REG(reg))
		{
			if(gen->reg_stack_top > JIT_REG_STACK_START)
			{
				spill_register(gen, gen->reg_stack_top - 1);
			}
		}
		else
#endif
		{
			spill_register(gen, reg);
		}
	}

#ifdef JIT_REG_DEBUG
	printf("leave _jit_regs_spill_all\n\n");
#endif
}

/*@
 * @deftypefun void _jit_regs_set_incoming (jit_gencode_t gen, int reg, jit_value_t value)
 * Set pseudo register @code{reg} to record that it currently holds the
 * contents of @code{value}.  The register must not contain any other
 * live value at this point.
 * @end deftypefun
@*/
void
_jit_regs_set_incoming(jit_gencode_t gen, int reg, jit_value_t value)
{
	int other_reg;

	/* Find the other register in a register pair */
	other_reg = jit_reg_get_pair(value->type, reg);

	/* avd: It's too late to spill here, if there was any
	   value it is already cloberred by the incoming value.
	   So for correct code generation the register must be
	   free by now (spilled at some earlier point). */
#if 0
	/* Eject any values that are currently in the register */
	spill_register(gen, reg);
	if(other_reg >= 0)
	{
		spill_register(gen, other_reg);
	}
#endif

	/* Record that the value is in "reg", but not in the frame */
#ifdef JIT_REG_STACK
	if(IS_STACK_REG(reg))
	{
		++(gen->reg_stack_top);
	}
#endif
	bind_value(gen, value, reg, other_reg, 0);
}

/*@
 * @deftypefun void _jit_regs_set_outgoing (jit_gencode_t gen, int reg, jit_value_t value)
 * Load the contents of @code{value} into pseudo register @code{reg},
 * spilling out the current contents.  This is used to set up outgoing
 * parameters for a function call.
 * @end deftypefun
@*/
void
_jit_regs_set_outgoing(jit_gencode_t gen, int reg, jit_value_t value)
{
	int other_reg;

#ifdef JIT_BACKEND_X86
	jit_type_t type;

	other_reg = -1;

	type = jit_type_normalize(value->type);
	if(type)
	{
		/* We might need to put float values in register pairs under x86 */
		if(type->kind == JIT_TYPE_LONG || type->kind == JIT_TYPE_ULONG ||
		   type->kind == JIT_TYPE_FLOAT64 || type->kind == JIT_TYPE_NFLOAT)
		{
			/* Long values in outgoing registers must be in ECX:EDX,
			   not in the ordinary register pairing of ECX:EBX */
			other_reg = 2;

			/* Force the value out of whatever register it is already in */
			_jit_regs_force_out(gen, value, 0);
		}
	}
#else
	other_reg = jit_reg_get_pair(value->type, reg);
#endif

	if(value->in_register && value->reg == reg)
	{
		/* The value is already in the register, but we may need to spill
		   if the frame copy is not up to date with the register */
		if(!(value->in_global_register || value->in_frame))
		{
			save_value(gen, value, reg, other_reg, 0);
		}

		/* The value is no longer "really" in the register.  A copy is
		   left behind, but the value itself reverts to the frame copy
		   as we are about to kill the registers in a function call */
		free_value(gen, value, reg, other_reg, 1);
	}
	else
	{
		/* Reload the value into the specified register */
		spill_register(gen, reg);
		if(other_reg >= 0)
		{
			spill_register(gen, other_reg);
		}

		_jit_gen_load_value(gen, reg, other_reg, value);
	}

	jit_reg_set_used(gen->inhibit, reg);
	if(other_reg >= 0)
	{
		jit_reg_set_used(gen->inhibit, other_reg);
	}
}

/*@
 * @deftypefun void _jit_regs_clear_all_outgoing (jit_gencode_t gen)
 * Free registers used for outgoing parameters.  This is used to
 * clean up after a function call.
 * @end deftypefun
@*/
void
_jit_regs_clear_all_outgoing(jit_gencode_t gen)
{
	gen->inhibit = jit_regused_init;
}

/* TODO: remove this function */
/*@
 * @deftypefun void _jit_regs_force_out (jit_gencode_t gen, jit_value_t value, int is_dest)
 * If @code{value} is currently in a register, then force its value out
 * into the stack frame.  The @code{is_dest} flag indicates that the value
 * will be a destination, so we don't care about the original value.
 *
 * This function is deprecated and going to be removed soon.
 *
 * @end deftypefun
@*/
void _jit_regs_force_out(jit_gencode_t gen, jit_value_t value, int is_dest)
{
	int reg, other_reg;
	if(value->in_register)
	{
		reg = value->reg;
		other_reg = jit_reg_get_pair(value->type, reg);

		if(is_dest)
		{
			free_value(gen, value, reg, other_reg, 0);
		}
		else
		{
			save_value(gen, value, reg, other_reg, 1);
		}
	}
}

/* TODO: remove this function */
/*@
 * @deftypefun int _jit_regs_load_value (jit_gencode_t gen, jit_value_t value, int destroy, int used_again)
 * Load a value into any register that is suitable and return that register.
 * If the value needs a long pair, then this will return the first register
 * in the pair.  Returns -1 if the value will not fit into any register.
 *
 * If @code{destroy} is non-zero, then we are about to destroy the register,
 * so the system must make sure that such destruction will not side-effect
 * @code{value} or any of the other values currently in that register.
 *
 * If @code{used_again} is non-zero, then it indicates that the value is
 * used again further down the block.
 *
 * This function is deprecated and going to be removed soon.
 *
 * @end deftypefun
@*/
int
_jit_regs_load_value(jit_gencode_t gen, jit_value_t value, int destroy, int used_again)
{
	int type;
	int reg, other_reg;
	int spill_cost;
	int suitable_reg, suitable_other_reg;
	int suitable_cost;
	int suitable_age;

	/* If the value is in a global register, and we are not going
	   to destroy the value, then use the global register itself.
	   This will avoid a redundant register copy operation */
	if(value->in_global_register && !destroy)
	{
		return value->global_reg;
	}

	/* If the value is already in a register, then try to use that register */
	if(value->in_register && (!destroy || !used_again))
	{
		reg = value->reg;
		if(!used_again)
		{
			other_reg = jit_reg_get_pair(value->type, reg);
			free_value(gen, value, reg, other_reg, 1);
		}
		return reg;
	}

	switch(jit_type_remove_tags(value->type)->kind)
	{
	case JIT_TYPE_SBYTE:
	case JIT_TYPE_UBYTE:
	case JIT_TYPE_SHORT:
	case JIT_TYPE_USHORT:
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
	case JIT_TYPE_NINT:
	case JIT_TYPE_NUINT:
	case JIT_TYPE_SIGNATURE:
	case JIT_TYPE_PTR:
		type = JIT_REG_WORD;
		break;
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		type = JIT_REG_LONG;
		break;
	case JIT_TYPE_FLOAT32:
		type = JIT_REG_FLOAT32;
		break;
	case JIT_TYPE_FLOAT64:
		type = JIT_REG_FLOAT64;
		break;
	case JIT_TYPE_NFLOAT:
		type = JIT_REG_NFLOAT;
		break;
	default:
		return 0;
	}

	suitable_reg = -1;
	suitable_other_reg = -1;
	suitable_cost = COST_TOO_MUCH;
	suitable_age = -1;
	for(reg = 0; reg < JIT_NUM_REGS; reg++)
	{
		if((jit_reg_flags(reg) & type) == 0)
		{
			continue;
		}
		if(jit_reg_is_used(gen->inhibit, reg))
		{
			continue;
		}
		if(jit_reg_is_used(gen->permanent, reg))
		{
			continue;
		}

		other_reg = jit_reg_get_pair(value->type, reg);
		if(other_reg >= 0)
		{
			if(jit_reg_is_used(gen->inhibit, other_reg))
			{
				continue;
			}
			if(jit_reg_is_used(gen->permanent, other_reg))
			{
				continue;
			}
		}

		spill_cost = compute_spill_cost(gen, 0, reg, other_reg);

		if(spill_cost < suitable_cost
		   || (spill_cost == suitable_cost
		       && spill_cost > 0 && gen->contents[reg].age < suitable_age))
		{
			suitable_reg = reg;
			suitable_other_reg = other_reg;
			suitable_cost = spill_cost;
			suitable_age = gen->contents[reg].age;
		}
	}

	if(suitable_reg >= 0)
	{
		spill_register(gen, suitable_reg);
		if(suitable_other_reg >= 0)
		{
			spill_register(gen, suitable_other_reg);
		}

		_jit_gen_load_value(gen, suitable_reg, suitable_other_reg, value);

		if(!destroy && !used_again)
		{
			bind_value(gen, value, suitable_reg, suitable_other_reg, 1);
		}
		else
		{
			bind_temporary(gen, suitable_reg, suitable_other_reg);
		}
	}

	return suitable_reg;
}

void
_jit_regs_init(jit_gencode_t gen, _jit_regs_t *regs, int flags)
{
	int index;

	jit_memset(regs, 0, sizeof(_jit_regs_t));

	regs->ternary = (flags & _JIT_REGS_TERNARY) != 0;
	regs->branch = (flags & _JIT_REGS_BRANCH) != 0;
	regs->copy = (flags & _JIT_REGS_COPY) != 0;
	regs->commutative = (flags & _JIT_REGS_COMMUTATIVE) != 0;
	regs->free_dest = (flags & _JIT_REGS_FREE_DEST) != 0;
#ifdef JIT_REG_STACK
	regs->on_stack = (flags & _JIT_REGS_STACK) != 0;
	regs->x87_arith = (flags & _JIT_REGS_X87_ARITH) != 0;
	regs->reversible = (flags & _JIT_REGS_REVERSIBLE) != 0;
	regs->no_pop = (regs->on_stack & regs->copy) != 0;
#endif

	for(index = 0; index < _JIT_REGS_VALUE_MAX; index++)
	{
		regs->descs[index].reg = -1;
		regs->descs[index].other_reg = -1;
		regs->descs[index].stack_reg = -1;
	}
	for(index = 0; index < _JIT_REGS_SCRATCH_MAX; index++)
	{
		regs->scratch[index].reg = -1;
	}

	regs->clobber = jit_regused_init;
	regs->assigned = gen->inhibit;
}

void
_jit_regs_init_dest(_jit_regs_t *regs, jit_insn_t insn, int flags, _jit_regclass_t *regclass)
{
	if((insn->flags & JIT_INSN_DEST_OTHER_FLAGS) == 0)
	{
		set_regdesc_value(regs, 0, insn->dest, flags, regclass,
				  (insn->flags & JIT_INSN_DEST_LIVE) != 0,
				  (insn->flags & JIT_INSN_DEST_NEXT_USE) != 0);
	}
}

void
_jit_regs_init_value1(_jit_regs_t *regs, jit_insn_t insn, int flags, _jit_regclass_t *regclass)
{
	if((insn->flags & JIT_INSN_VALUE1_OTHER_FLAGS) == 0)
	{
		set_regdesc_value(regs, 1, insn->value1, flags, regclass,
				  (insn->flags & JIT_INSN_VALUE1_LIVE) != 0,
				  (insn->flags & JIT_INSN_VALUE1_NEXT_USE) != 0);
	}
}

void
_jit_regs_init_value2(_jit_regs_t *regs, jit_insn_t insn, int flags, _jit_regclass_t *regclass)
{
	if((insn->flags & JIT_INSN_VALUE2_OTHER_FLAGS) == 0)
	{
		set_regdesc_value(regs, 2, insn->value2, flags, regclass,
				  (insn->flags & JIT_INSN_VALUE2_LIVE) != 0,
				  (insn->flags & JIT_INSN_VALUE2_NEXT_USE) != 0);
	}
}

void
_jit_regs_add_scratch(_jit_regs_t *regs, _jit_regclass_t *regclass)
{
	if(regs->num_scratch < _JIT_REGS_SCRATCH_MAX)
	{
		regs->scratch[regs->num_scratch].reg = -1;
		regs->scratch[regs->num_scratch].regclass = regclass;
		++regs->num_scratch;
	}
}

void
_jit_regs_set_dest(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg)
{
	if(reg >= 0 && !IS_STACK_REG(reg))
	{
		set_regdesc_register(gen, regs, 0, reg, other_reg);
	}
}

void
_jit_regs_set_value1(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg)
{
	if(reg >= 0 && !IS_STACK_REG(reg))
	{
		set_regdesc_register(gen, regs, 1, reg, other_reg);
	}
}

void
_jit_regs_set_value2(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg)
{
	if(reg >= 0 && !IS_STACK_REG(reg))
	{
		set_regdesc_register(gen, regs, 2, reg, other_reg);
	}
}

void
_jit_regs_set_scratch(jit_gencode_t gen, _jit_regs_t *regs, int index, int reg)
{
	if(index < regs->num_scratch && index >= 0 && reg >= 0 && !IS_STACK_REG(reg))
	{
		set_scratch_register(gen, regs, index, reg);
	}
}

int
_jit_regs_get_dest(_jit_regs_t *regs)
{
	return regs->descs[0].reg;
}

int
_jit_regs_get_value1(_jit_regs_t *regs)
{
	return regs->descs[1].reg;
}

int
_jit_regs_get_value2(_jit_regs_t *regs)
{
	return regs->descs[2].reg;
}

int
_jit_regs_get_dest_other(_jit_regs_t *regs)
{
	return regs->descs[0].other_reg;
}

int
_jit_regs_get_value1_other(_jit_regs_t *regs)
{
	return regs->descs[1].other_reg;
}

int
_jit_regs_get_value2_other(_jit_regs_t *regs)
{
	return regs->descs[2].other_reg;
}

int
_jit_regs_get_scratch(_jit_regs_t *regs, int index)
{
	if(index < regs->num_scratch && index >= 0)
	{
		return regs->scratch[index].reg;
	}
	return -1;
}

void
_jit_regs_clobber(_jit_regs_t *regs, int reg)
{
	if(reg >= 0)
	{
		jit_reg_set_used(regs->clobber, reg);
	}
}

void
_jit_regs_clobber_class(jit_gencode_t gen, _jit_regs_t *regs, _jit_regclass_t *regclass)
{
	int index;

	for(index = 0; index < regclass->num_regs; index++)
	{
		if(jit_reg_is_used(gen->permanent, index))
		{
			continue;
		}
		jit_reg_set_used(regs->clobber, regclass->regs[index]);
	}
}

void
_jit_regs_clobber_all(jit_gencode_t gen, _jit_regs_t *regs)
{
	int index;

	for(index = 0; index < JIT_NUM_REGS; index++)
	{
		if((jit_reg_flags(index) & JIT_REG_FIXED) != 0)
		{
			continue;
		}
		if(jit_reg_is_used(gen->permanent, index))
		{
			continue;
		}
		jit_reg_set_used(regs->clobber, index);
	}
}

void
_jit_regs_assign(jit_gencode_t gen, _jit_regs_t *regs)
{
	int index;

#ifdef JIT_REG_DEBUG
	printf("_jit_regs_assign()\n");
#endif

	/* Check explicitely assigned registers */
	if(regs->descs[2].value && regs->descs[2].reg >= 0)
	{
		check_duplicate_value(regs, &regs->descs[2], &regs->descs[1]);
		if(regs->ternary)
		{
			check_duplicate_value(regs, &regs->descs[2], &regs->descs[0]);
		}
	}
	if(regs->descs[1].value && regs->descs[1].reg >= 0)
	{
		if(regs->ternary)
		{
			check_duplicate_value(regs, &regs->descs[1], &regs->descs[0]);
		}
		else if(!regs->free_dest && regs->descs[0].value && regs->descs[0].reg < 0)
		{
			/* For binary or unary ops with explicitely assigned registers
			   the output always goes to the same register as the first input
			   value unless this is a three-address instruction. */
			set_regdesc_register(gen, regs, 0,
					     regs->descs[1].reg,
					     regs->descs[1].other_reg);
		}
	}

#if JIT_REG_STACK
	/* Choose between x87 pop and no-pop instructions.  */
	select_nopop_or_pop(gen, regs);
#endif

	/* Assign output and input registers. */
	if(regs->descs[0].value)
	{
		if(regs->descs[0].reg < 0)
		{
			if(regs->ternary)
			{
				choose_input_register(gen, regs, 0);
			}
			else
			{
				choose_output_register(gen, regs);
			}
		}
		if(regs->ternary)
		{
			check_duplicate_value(regs, &regs->descs[0], &regs->descs[1]);
			check_duplicate_value(regs, &regs->descs[0], &regs->descs[2]);
		}
		else if(!regs->free_dest)
		{
			choose_input_order(gen, regs);
			if(regs->dest_input_index)
			{
				set_regdesc_register(gen, regs, regs->dest_input_index,
						     regs->descs[0].reg,
						     regs->descs[0].other_reg);
			}
		}
	}
	if(regs->descs[1].value && regs->descs[1].reg < 0)
	{
		choose_input_register(gen, regs, 1);
	}
	check_duplicate_value(regs, &regs->descs[1], &regs->descs[2]);
	if(regs->descs[2].value && regs->descs[2].reg < 0)
	{
		choose_input_register(gen, regs, 2);
	}

	/* Assign scratch registers. */
	for(index = 0; index < regs->num_scratch; index++)
	{
		if(regs->scratch[index].reg < 0)
		{
			choose_scratch_register(gen, regs, index);
		}
	}

	/* Collect information about registers. */
	set_regdesc_flags(gen, regs, 0);
	set_regdesc_flags(gen, regs, 1);
	set_regdesc_flags(gen, regs, 2);
}

void
_jit_regs_gen(jit_gencode_t gen, _jit_regs_t *regs)
{
	int reg;

#ifdef JIT_REG_DEBUG
	dump_regs(gen, "enter _jit_regs_gen");
#endif

	/* Spill clobbered registers. */
	for(reg = 0; reg < JIT_NUM_REGS; reg++)
	{
		if((jit_reg_flags(reg) & JIT_REG_FIXED))
		{
			continue;
		}

		if(!jit_reg_is_used(regs->clobber, reg))
		{
			continue;
		}
		if(jit_reg_is_used(gen->permanent, reg))
		{
#ifdef IS_REGISTER_OCCUPIED
			/* If the op computes the value assigned to the global
			   register then it is not really clobbered. */
			if(!regs->ternary
			   && regs->descs[0].value
			   && regs->descs[0].value->has_global_register
			   && regs->descs[0].value->global_reg == reg)
			{
				continue;
			}
#endif
			/* Oops, the global register is going to be clobbered. Save
			   it on the stack in order to restore after the op. */
#ifdef JIT_REG_DEBUG
			printf("*** Spill global register: %d ***\n", reg);
#endif
			if(regs->branch)
			{
				/* After the branch is taken there is no way
				   to load the global register back. */
				jit_exception_builtin(JIT_RESULT_COMPILE_ERROR);
			}
			_jit_gen_spill_global(gen, reg, 0);
			continue;
		}

#ifdef JIT_REG_STACK
		/* If this is a stack register, then we need to find the
		   register that contains the top-most stack position,
		   because we must spill stack registers from top down.
		   As we spill each one, something else will become the top */
		if(IS_STACK_REG(reg))
		{
			int top = gen->reg_stack_top - 1;
			/* spill top registers if there are any that needs to be */
			for(; top >= reg && jit_reg_is_used(regs->clobber, top); top--)
			{
				spill_clobbered_register(gen, regs, top);
				/* If an input value is on the top then it stays there
				   and the top position does not change. */
				if(gen->contents[top].num_values > 0)
				{
					break;
				}
			}
			if(top > reg)
			{
				spill_clobbered_register(gen, regs, reg);
			}
		}
		else
#endif
		{
			spill_clobbered_register(gen, regs, reg);
		}
	}

	/* Save input values if necessary and free the output value if it is in a register */
	if(regs->ternary)
	{
		save_input_value(gen, regs, 0);
	}
	else
	{
		free_output_value(gen, regs);
	}
	save_input_value(gen, regs, 1);
	save_input_value(gen, regs, 2);

#ifdef JIT_REG_STACK
	if(regs->wanted_stack_count > 0)
	{
		/* Adjust assignment of stack registers. */
		select_stack_order(gen, regs);
		adjust_assignment(gen, regs, 2);
		adjust_assignment(gen, regs, 1);
		adjust_assignment(gen, regs, 0);

		if(regs->ternary)
		{
			/* Ternary ops with only one stack register are supported. */
			if(regs->loaded_stack_count > 0)
			{
				move_input_value(gen, regs, 0);
				move_input_value(gen, regs, 1);
				move_input_value(gen, regs, 2);
			}
			load_input_value(gen, regs, 0);
			load_input_value(gen, regs, 1);
			load_input_value(gen, regs, 2);
		}
		else if(regs->flip_args)
		{
			/* Shuffle the values that are already on the register stack. */
			if(regs->loaded_stack_count > 0)
			{
				move_input_value(gen, regs, 1);
				move_input_value(gen, regs, 2);
			}

			/* Load and shuffle the remaining values. */
			load_input_value(gen, regs, 1);
			move_input_value(gen, regs, 1);
			load_input_value(gen, regs, 2);
		}
		else
		{
			/* Shuffle the values that are already on the register stack. */
			if(regs->loaded_stack_count > 0)
			{
				move_input_value(gen, regs, 2);
				move_input_value(gen, regs, 1);
			}

			/* Load and shuffle the remaining values. */
			load_input_value(gen, regs, 2);
			move_input_value(gen, regs, 2);
			load_input_value(gen, regs, 1);
		}
	}
	else
#endif
	{
		/* Load flat registers. */
		if(regs->ternary)
		{
			load_input_value(gen, regs, 0);
		}
#ifdef JIT_REG_STACK
		else if(regs->descs[0].reg >= 0 && IS_STACK_REG(regs->descs[0].reg))
		{
			adjust_assignment(gen, regs, 0);
		}
#endif
		load_input_value(gen, regs, 1);
		load_input_value(gen, regs, 2);
	}

#ifdef JIT_REG_DEBUG
	dump_regs(gen, "leave _jit_regs_gen");
#endif
}

#ifdef JIT_REG_STACK
int
_jit_regs_select(_jit_regs_t *regs)
{
	int flags;

	flags = 0;
	if(regs->no_pop)
	{
		flags |= _JIT_REGS_NO_POP;
	}
	if(regs->flip_args)
	{
		flags |= _JIT_REGS_FLIP_ARGS;
	}
	if(regs->dest_input_index == 2)
	{
		flags |= _JIT_REGS_REVERSE;
	}
	return flags;
}
#endif

void
_jit_regs_commit(jit_gencode_t gen, _jit_regs_t *regs)
{
	int reg;

#ifdef JIT_REG_DEBUG
	dump_regs(gen, "enter _jit_regs_commit");
#endif

	if(regs->ternary)
	{
#ifdef JIT_REG_STACK
		if(regs->wanted_stack_count > 0)
		{
			pop_input_value(gen, regs, 0);
			pop_input_value(gen, regs, 1);
			pop_input_value(gen, regs, 2);
		}
#endif
		commit_input_value(gen, regs, 0, 1);
		commit_input_value(gen, regs, 1, 1);
		commit_input_value(gen, regs, 2, 1);
	}
	else if(!regs->descs[0].value)
	{
#ifdef JIT_REG_STACK
		if(regs->wanted_stack_count > 0)
		{
			pop_input_value(gen, regs, 1);
			pop_input_value(gen, regs, 2);
		}
#endif
		commit_input_value(gen, regs, 1, 1);
		commit_input_value(gen, regs, 2, 1);
	}
#ifdef JIT_REG_STACK
	else if(regs->wanted_stack_count > 0)
	{
		int pop1, pop2;
		struct _jit_value temp;
		int reg1, reg2;

		pop1 = pop2 = 0;
		if(!regs->no_pop)
		{
			if(regs->x87_arith)
			{
				if(regs->flip_args)
				{
					pop_input_value(gen, regs, 2);
					pop2 = 1;
				}
				else
				{
					pop_input_value(gen, regs, 1);
					pop1 = 1;
				}
			}
			else
			{
				pop_input_value(gen, regs, 1);
				pop_input_value(gen, regs, 2);
				pop1 = pop2 = 1;
			}
		}

		if(IS_STACK_REG(regs->descs[0].reg))
		{
			temp = *regs->descs[0].value;
			if(!regs->x87_arith && !regs->copy)
			{
				++(gen->reg_stack_top);
			}
			bind_value(gen, &temp, regs->descs[0].reg, -1, 0);
		}

		reg1 = ((regs->descs[1].value && regs->descs[1].value->in_register)
			? regs->descs[1].value->reg : -1);
		reg2 = ((regs->descs[2].value && regs->descs[2].value->in_register)
			? regs->descs[2].value->reg : -1);
		if(reg1 > reg2)
		{
			commit_input_value(gen, regs, 1, pop1);
			commit_input_value(gen, regs, 2, pop2);
		}
		else
		{
			commit_input_value(gen, regs, 2, pop2);
			commit_input_value(gen, regs, 1, pop1);
		}

		if(IS_STACK_REG(regs->descs[0].reg))
		{
			reg1 = temp.reg;
			free_value(gen, &temp, reg1, -1, 1);
			regs->descs[0].reg = reg1;
			regs->descs[0].other_reg = -1;
		}
		commit_output_value(gen, regs, 0);
	}
#endif
	else
	{
		commit_input_value(gen, regs, 2, 0);
		commit_input_value(gen, regs, 1, 0);
		commit_output_value(gen, regs, 1);
	}

	/* Load clobbered global registers. */
	for(reg = JIT_NUM_REGS - 1; reg >= 0; reg--)
	{
		if(jit_reg_is_used(regs->clobber, reg) && jit_reg_is_used(gen->permanent, reg))
		{
#ifdef IS_REGISTER_OCCUPIED
			if(!regs->ternary
			   && regs->descs[0].value
			   && regs->descs[0].value->has_global_register
			   && regs->descs[0].value->global_reg == reg)
			{
				continue;
			}
#endif
			_jit_gen_load_global(gen, reg, 0);
		}
	}

#ifdef JIT_REG_DEBUG
	dump_regs(gen, "leave _jit_regs_commit");
#endif
}

void
_jit_regs_begin(jit_gencode_t gen, _jit_regs_t *regs, int space)
{
	_jit_regs_assign(gen, regs);
	_jit_regs_gen(gen, regs);
	_jit_gen_check_space(gen, space);
}
