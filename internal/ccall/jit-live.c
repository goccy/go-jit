/*
 * jit-live.c - Liveness analysis for function bodies.
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
#include <jit/jit-dump.h>

#define USE_FORWARD_PROPAGATION 1
#define USE_BACKWARD_PROPAGATION 1

/*
 * Compute liveness information for a basic block.
 */
static void
compute_liveness_for_block(jit_block_t block)
{
	jit_insn_iter_t iter;
	jit_insn_t insn;
	jit_value_t dest;
	jit_value_t value1;
	jit_value_t value2;
	int flags;

	/* Scan backwards to compute the liveness flags */
	jit_insn_iter_init_last(&iter, block);
	while((insn = jit_insn_iter_previous(&iter)) != 0)
	{
		/* Skip NOP instructions, which may have arguments left
		   over from when the instruction was replaced, but which
		   are not relevant to our liveness analysis */
		if(insn->opcode == JIT_OP_NOP)
		{
			continue;
		}

		/* Fetch the value parameters to this instruction */
		flags = insn->flags;
		if((flags & JIT_INSN_DEST_OTHER_FLAGS) == 0)
		{
			dest = insn->dest;
			if(dest && dest->is_constant)
			{
				dest = 0;
			}
		}
		else
		{
			dest = 0;
		}
		if((flags & JIT_INSN_VALUE1_OTHER_FLAGS) == 0)
		{
			value1 = insn->value1;
			if(value1 && value1->is_constant)
			{
				value1 = 0;
			}
		}
		else
		{
			value1 = 0;
		}
		if((flags & JIT_INSN_VALUE2_OTHER_FLAGS) == 0)
		{
			value2 = insn->value2;
			if(value2 && value2->is_constant)
			{
				value2 = 0;
			}
		}
		else
		{
			value2 = 0;
		}

		/* Record the liveness information in the instruction flags */
		flags &= ~JIT_INSN_LIVENESS_FLAGS;
		if(dest)
		{
			if(dest->live)
			{
				flags |= JIT_INSN_DEST_LIVE;
			}
			if(dest->next_use)
			{
				flags |= JIT_INSN_DEST_NEXT_USE;
			}
		}
		if(value1)
		{
			if(value1->live)
			{
				flags |= JIT_INSN_VALUE1_LIVE;
			}
			if(value1->next_use)
			{
				flags |= JIT_INSN_VALUE1_NEXT_USE;
			}
		}
		if(value2)
		{
			if(value2->live)
			{
				flags |= JIT_INSN_VALUE2_LIVE;
			}
			if(value2->next_use)
			{
				flags |= JIT_INSN_VALUE2_NEXT_USE;
			}
		}
		insn->flags = (short)flags;

		/* Set the destination to "not live, no next use" */
		if(dest)
		{
			if((flags & JIT_INSN_DEST_IS_VALUE) == 0)
			{
				if(!(dest->next_use) && !(dest->live))
				{
					/* There is no next use of this value and it is not
					   live on exit from the block.  So we can discard
					   the entire instruction as it will have no effect */
#ifdef _JIT_COMPILE_DEBUG
					printf("liveness analysis: optimize away instruction '");
					jit_dump_insn(stdout, block->func, insn);
					printf("'\n");
#endif
					insn->opcode = (short)JIT_OP_NOP;
					continue;
				}
				dest->live = 0;
				dest->next_use = 0;
			}
			else
			{
				/* The destination is actually a source value for this
				   instruction (e.g. JIT_OP_STORE_RELATIVE_*) */
				dest->live = 1;
				dest->next_use = 1;
			}
		}

		/* Set value1 and value2 to "live, next use" */
		if(value1)
		{
			value1->live = 1;
			value1->next_use = 1;
		}
		if(value2)
		{
			value2->live = 1;
			value2->next_use = 1;
		}
	}
}

#if defined(USE_FORWARD_PROPAGATION) || defined(USE_BACKWARD_PROPAGATION)
/*
 * Check if the instruction is eligible for copy propagation.
 */
static int
is_copy_insn(jit_insn_t insn)
{
	jit_type_t dtype;
	jit_type_t vtype;

	if (!insn || !insn->dest || !insn->value1)
	{
		return 0;
	}

	switch(insn->opcode)
	{
	case JIT_OP_COPY_INT:
		/* Currently JIT_INSN_COPY_INT is used not only for int-to-int
		   copying but for byte-to-int and short-to-int copying too
		   (see jit_insn_convert). Propagation of byte and short values
		   to instructions that expect ints might confuse them. */
		dtype = jit_type_normalize(insn->dest->type);
		vtype = jit_type_normalize(insn->value1->type);
		if(dtype != vtype)
		{
			/* signed/unsigned int conversion should be safe */
			if((dtype->kind == JIT_TYPE_INT || dtype->kind == JIT_TYPE_UINT)
			   && (vtype->kind == JIT_TYPE_INT || vtype->kind == JIT_TYPE_UINT))
			{
				return 1;
			}
			return 0;
		}
		return 1;

	case JIT_OP_COPY_LOAD_SBYTE:
	case JIT_OP_COPY_LOAD_UBYTE:
	case JIT_OP_COPY_LOAD_SHORT:
	case JIT_OP_COPY_LOAD_USHORT:
	case JIT_OP_COPY_LONG:
	case JIT_OP_COPY_FLOAT32:
	case JIT_OP_COPY_FLOAT64:
	case JIT_OP_COPY_NFLOAT:
	case JIT_OP_COPY_STRUCT:
	case JIT_OP_COPY_STORE_BYTE:
	case JIT_OP_COPY_STORE_SHORT:
		return 1;
	}

	return 0;
}
#endif

#if USE_FORWARD_PROPAGATION
/*
 * Perform simple copy propagation within basic block. Replaces instructions
 * that look like this:
 *
 * i) t = x
 * ...
 * j) y = op(t)
 *
 * with the folowing:
 *
 * i) t = x
 * ...
 * j) y = op(x)
 *
 * If "t" is not used after the instruction "j" then further liveness analysis
 * may replace the instruction "i" with a noop:
 *
 * i) noop
 * ...
 * j) y = op(x)
 *
 * The propagation stops as soon as either "t" or "x" are changed (used as a
 * dest in a different instruction).
 */
static int
forward_propagation(jit_block_t block)
{
	int optimized;
	jit_insn_iter_t iter, iter2;
	jit_insn_t insn, insn2;
	jit_value_t dest, value;
	int flags2;

	optimized = 0;

	jit_insn_iter_init(&iter, block);
	while((insn = jit_insn_iter_next(&iter)) != 0)
	{
		if(!is_copy_insn(insn))
		{
			continue;
		}

		dest = insn->dest;
		value = insn->value1;

		/* Discard copy to itself */
		if(dest == value)
		{
#ifdef _JIT_COMPILE_DEBUG
			printf("forward copy propagation: optimize away copy to itself in '");
			jit_dump_insn(stdout, block->func, insn);
			printf("'\n");
#endif
			insn->opcode = (short)JIT_OP_NOP;
			optimized = 1;
			continue;
		}

		/* Not smart enough to tell when it is safe to optimize copying
		   to a value that is used in other basic blocks or may be
		   aliased. */
		if(!dest->is_temporary)
		{
			continue;
		}
		if(dest->is_addressable || dest->is_volatile)
		{
			continue;
		}
		if(value->is_addressable || value->is_volatile)
		{
			continue;
		}

		iter2 = iter;
		while((insn2 = jit_insn_iter_next(&iter2)) != 0)
		{
			/* Skip NOP instructions, which may have arguments left
			   over from when the instruction was replaced, but which
			   are not relevant to our analysis */
			if(insn->opcode == JIT_OP_NOP)
			{
				continue;
			}

			flags2 = insn2->flags;
			if((flags2 & JIT_INSN_DEST_OTHER_FLAGS) == 0)
			{
				if((flags2 & JIT_INSN_DEST_IS_VALUE) == 0)
				{
					if(insn2->dest == dest || insn2->dest == value)
					{
						break;
					}
				}
				else if(insn2->dest == dest)
				{
#ifdef _JIT_COMPILE_DEBUG
					printf("forward copy propagation: in '");
					jit_dump_insn(stdout, block->func, insn2);
					printf("' replace ");
					jit_dump_value(stdout, block->func, insn2->dest, 0);
					printf(" with ");
					jit_dump_value(stdout, block->func, value, 0);
					printf("'\n");
#endif
					insn2->dest = value;
					optimized = 1;
				}
			}
			if((flags2 & JIT_INSN_VALUE1_OTHER_FLAGS) == 0)
			{
				if(insn2->value1 == dest)
				{
#ifdef _JIT_COMPILE_DEBUG
					printf("forward copy propagation: in '");
					jit_dump_insn(stdout, block->func, insn2);
					printf("' replace ");
					jit_dump_value(stdout, block->func, insn2->value1, 0);
					printf(" with ");
					jit_dump_value(stdout, block->func, value, 0);
					printf("'\n");
#endif
					insn2->value1 = value;
					optimized = 1;
				}
			}
			if((flags2 & JIT_INSN_VALUE2_OTHER_FLAGS) == 0)
			{
				if(insn2->value2 == dest)
				{
#ifdef _JIT_COMPILE_DEBUG
					printf("forward copy propagation: in '");
					jit_dump_insn(stdout, block->func, insn2);
					printf("' replace ");
					jit_dump_value(stdout, block->func, insn2->value2, 0);
					printf(" with ");
					jit_dump_value(stdout, block->func, value, 0);
					printf("'\n");
#endif
					insn2->value2 = value;
					optimized = 1;
				}
			}
		}
	}

	return optimized;
}
#endif

#ifdef USE_BACKWARD_PROPAGATION
/*
 * Perform simple copy propagation within basic block for the case when a
 * temporary value is stored to another value. This replaces instructions
 * that look like this:
 *
 * i) t = op(x)
 * ...
 * j) y = t
 *
 * with the following
 *
 * i) y = op(x)
 * ...
 * j) noop
 *
 * This is only allowed if "t" is used only in the instructions "i" and "j"
 * and "y" is not used between "i" and "j" (but can be used after "j").
 */
static int
backward_propagation(jit_block_t block)
{
	int optimized;
	jit_insn_iter_t iter, iter2;
	jit_insn_t insn, insn2;
	jit_value_t dest, value;
	int flags2;

	optimized = 0;

	jit_insn_iter_init_last(&iter, block);
	while((insn = jit_insn_iter_previous(&iter)) != 0)
	{
		if(!is_copy_insn(insn))
		{
			continue;
		}

		dest = insn->dest;
		value = insn->value1;

		/* Discard copy to itself */
		if(dest == value)
		{
#ifdef _JIT_COMPILE_DEBUG
			printf("backward copy propagation: optimize away copy to itself in '");
			jit_dump_insn(stdout, block->func, insn);
			printf("'\n");
#endif
			insn->opcode = (short)JIT_OP_NOP;
			optimized = 1;
			continue;
		}

		/* "value" is used afterwards so we cannot eliminate it here */
		if((insn->flags & (JIT_INSN_VALUE1_LIVE | JIT_INSN_VALUE1_NEXT_USE)) != 0)
		{
			continue;
		}

		if(dest->is_addressable || dest->is_volatile)
		{
			continue;
		}
		if(value->is_addressable || value->is_volatile)
		{
			continue;
		}

		iter2 = iter;
		while((insn2 = jit_insn_iter_previous(&iter2)) != 0)
		{
			/* Skip NOP instructions, which may have arguments left
			   over from when the instruction was replaced, but which
			   are not relevant to our analysis */
			if(insn->opcode == JIT_OP_NOP)
			{
				continue;
			}

			flags2 = insn2->flags;
			if((flags2 & JIT_INSN_DEST_OTHER_FLAGS) == 0)
			{
				if(insn2->dest == dest)
				{
					break;
				}
				if(insn2->dest == value)
				{
					if((flags2 & JIT_INSN_DEST_IS_VALUE) == 0)
					{
#ifdef _JIT_COMPILE_DEBUG
						printf("backward copy propagation: in '");
						jit_dump_insn(stdout, block->func, insn2);
						printf("' replace ");
						jit_dump_value(stdout, block->func, insn2->dest, 0);
						printf(" with ");
						jit_dump_value(stdout, block->func, dest, 0);
						printf(" and optimize away '");
						jit_dump_insn(stdout, block->func, insn);
						printf("'\n");
#endif
						insn->opcode = (short)JIT_OP_NOP;
						insn2->dest = dest;
						optimized = 1;
					}
					break;
				}
			}
			if((flags2 & JIT_INSN_VALUE1_OTHER_FLAGS) == 0)
			{
				if(insn2->value1 == dest || insn2->value1 == value)
				{
					break;
				}
			}
			if((flags2 & JIT_INSN_VALUE2_OTHER_FLAGS) == 0)
			{
				if(insn2->value2 == dest || insn2->value1 == value)
				{
					break;
				}
			}
		}
	}

	return optimized;
}
#endif

/* Reset value liveness flags. */
static void
reset_value_liveness(jit_value_t value)
{
	if(value)
	{
		if (!value->is_constant && !value->is_temporary)
		{
			value->live = 1;
		}
		else
		{
			value->live = 0;
		}
		value->next_use = 0;
	}
}

/*
 * Re-scan the block to reset the liveness flags on all non-temporaries
 * because we need them in the original state for the next block.
 */
static void
reset_liveness_flags(jit_block_t block, int reset_all)
{
	jit_insn_iter_t iter;
	jit_insn_t insn;
	int flags;

	jit_insn_iter_init(&iter, block);
	while((insn = jit_insn_iter_next(&iter)) != 0)
	{
		flags = insn->flags;
		if((flags & JIT_INSN_DEST_OTHER_FLAGS) == 0)
		{
			reset_value_liveness(insn->dest);
		}
		if((flags & JIT_INSN_VALUE1_OTHER_FLAGS) == 0)
		{
			reset_value_liveness(insn->value1);
		}
		if((flags & JIT_INSN_VALUE2_OTHER_FLAGS) == 0)
		{
			reset_value_liveness(insn->value2);
		}
		if(reset_all)
		{
			flags &= ~(JIT_INSN_DEST_LIVE | JIT_INSN_DEST_NEXT_USE
				   |JIT_INSN_VALUE1_LIVE | JIT_INSN_VALUE1_NEXT_USE
				   |JIT_INSN_VALUE2_LIVE | JIT_INSN_VALUE2_NEXT_USE);
		}
	}
}

void _jit_function_compute_liveness(jit_function_t func)
{
	jit_block_t block = func->builder->entry_block;
	while(block != 0)
	{
#ifdef USE_FORWARD_PROPAGATION
		/* Perform forward copy propagation for the block */
		forward_propagation(block);
#endif

		/* Reset the liveness flags for the next block */
		reset_liveness_flags(block, 0);

		/* Compute the liveness flags for the block */
		compute_liveness_for_block(block);

#ifdef USE_BACKWARD_PROPAGATION
		/* Perform backward copy propagation for the block */
		if(backward_propagation(block))
		{
			/* Reset the liveness flags and compute them again */
			reset_liveness_flags(block, 1);
			compute_liveness_for_block(block);
		}
#endif

		/* Move on to the next block in the function */
		block = block->next;
	}
}
