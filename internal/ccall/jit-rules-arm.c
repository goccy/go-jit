/*
 * jit-rules-arm.c - Rules that define the characteristics of the ARM.
 *
 * Copyright (C) 2004  Southern Storm Software, Pty Ltd.
 * Copyright (C) 2008  Michele Tartara <mikyt@users.sourceforge.net>
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
#include "jit-rules.h"
#include "jit-apply-rules.h"

#if defined(JIT_BACKEND_ARM)

#include "jit-gen-arm.h"
#include "jit-reg-alloc.h"
#include "jit-setjmp.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Pseudo register numbers for the ARM registers.  These are not the
 * same as the CPU instruction register numbers.  The order of these
 * values must match the order in "JIT_REG_INFO".
 */
#define ARM_REG_R0		0
#define ARM_REG_R1		1
#define ARM_REG_R2		2
#define ARM_REG_R3		3
#define ARM_REG_R4		4
#define ARM_REG_R5		5
#define ARM_REG_R6		6
#define ARM_REG_R7		7
#define ARM_REG_R8		8
#define ARM_REG_R9		9
#define ARM_REG_R10		10
#define ARM_REG_FP		11
#define ARM_REG_R12		12
#define ARM_REG_SP		13
#define ARM_REG_LR		14
#define ARM_REG_PC		15

#ifdef JIT_ARM_HAS_FPA
#define ARM_F0			16
#define ARM_F1			17
#define ARM_F2			18
#define ARM_F3			19
#define ARM_F4			20
#define ARM_F5			21
#define ARM_F6			22
#define ARM_F7			23
#endif

#ifdef JIT_ARM_HAS_VFP
#define ARM_REG_S0		16
#define ARM_REG_S1		17
#define ARM_REG_S2		18
#define ARM_REG_S3		19
#define ARM_REG_S4		20
#define ARM_REG_S5		21
#define ARM_REG_S6		22
#define ARM_REG_S7		23
#define ARM_REG_S8		24
#define ARM_REG_S9		25
#define ARM_REG_S10		26
#define ARM_REG_S11		27
#define ARM_REG_S12		28
#define ARM_REG_S13		29
#define ARM_REG_S14		30
#define ARM_REG_S15		31
#define ARM_REG_D8		32
#define ARM_REG_D9		33
#define ARM_REG_D10		34
#define ARM_REG_D11		35
#define ARM_REG_D12		36
#define ARM_REG_D13		37
#define ARM_REG_D14		38
#define ARM_REG_D15		39
#endif

/*
 * Determine if a pseudo register number is word-based or float-based.
 */
#define	IS_WORD_REG(reg)	((reg) <= ARM_REG_PC)
#define	IS_FLOAT_REG(reg)	((reg) > ARM_REG_PC)

/*
 * Round a size up to a multiple of the stack word size.
 */
#define	ROUND_STACK(size) \
	(((size) + (sizeof(void *) - 1)) & ~(sizeof(void *) - 1))

/*
 * Given the first register of a long pair get the other register,
 * only if the two are currently forming a pair
 */
#define jit_reg_current_other_reg(gen,reg) \
	(gen->contents[reg].is_long_start ? jit_reg_other_reg(reg) : -1)

/* 
 * Define the classes of registers
 */
static _jit_regclass_t *arm_reg;
static _jit_regclass_t *arm_freg;
static _jit_regclass_t *arm_freg32;
static _jit_regclass_t *arm_freg64;
static _jit_regclass_t *arm_lreg;

/*
 * -------------------- Helper functions --------------------------
 */

/*
 * Load the instruction pointer from the generation context.
 */
#define	jit_gen_load_inst_ptr(gen,inst)	\
	do { \
		arm_inst_buf_init((inst), (gen)->ptr, (gen)->limit); \
	} while (0)

/*
 * Save the instruction pointer back to the generation context.
 */
#define	jit_gen_save_inst_ptr(gen,inst)	\
	do { \
		(gen)->ptr = (unsigned char *)arm_inst_get_posn(inst); \
	} while (0)

/*
 * Get a temporary register that isn't one of the specified registers.
 * When this function is used EVERY REGISTER COULD BE DESTROYED!!!
 * TODO: this function is only used by JIT_OP_STORE_RELATIVE_STRUCT, through memory_copy: remove
 * the need of using it by substituting it with register allocation with [scratch reg]
 */
static int get_temp_reg(int reg1, int reg2, int reg3)
{
	/* 
	 * R0-R3 are not used because they could be needed for parameter passing.
	 * R4 is not used because it's used by jit_apply to store the base of the frame where
	 * it saves all the data it needs in order to restart execution after calling a compiled function.
	 * R9 is not used because it's the platform register and could have special
	 * uses on some ARM platform
	 * R11, R13-R15 are not used because they have special meaning on the ARM platform.
	 * The other registers are candidates for 
	 */
	if(reg1 != ARM_R5 && reg2 != ARM_R5 && reg3 != ARM_R5)
	{
		return ARM_R5;
	}
	if(reg1 != ARM_R6 && reg2 != ARM_R6 && reg3 != ARM_R6)
	{
		return ARM_R6;
	}
	if(reg1 != ARM_R7 && reg2 != ARM_R7 && reg3 != ARM_R7)
	{
		return ARM_R7;
	}
	if(reg1 != ARM_R8 && reg2 != ARM_R8 && reg3 != ARM_R8)
	{
		return ARM_R8;
	}
	if(reg1 != ARM_R10 && reg2 != ARM_R10 && reg3 != ARM_R10)
	{
		return ARM_R10;
	}
	return ARM_R12;
}

static void mov_reg_imm (jit_gencode_t gen, arm_inst_buf *inst, int reg, int value);

/*
 * Copy a block of memory that has a specific size.  Other than
 * the parameter pointers, all registers must be unused at this point.
 */
static arm_inst_buf memory_copy (jit_gencode_t gen, arm_inst_buf inst, int dreg, jit_nint doffset, int sreg, jit_nint soffset, jit_nuint size, int temp_reg)
{
	//int temp_reg = get_temp_reg(dreg, sreg, -1);
	if(temp_reg == -1)
	{
		temp_reg = get_temp_reg(dreg, sreg, -1);
	}
	if(size <= 4 * sizeof(void *))
	{
		/* Use direct copies to copy the memory */
		int offset = 0;
		while(size >= sizeof(void *))
		{
			arm_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, sizeof(void *));
			arm_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, sizeof(void *));
			size -= sizeof(void *);
			offset += sizeof(void *);
		}
		if(size >= 2)
		{
			arm_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 2);
			arm_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 2);
			size -= 2;
			offset += 2;
		}
		if(size >= 1)
		{
			arm_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 1);
			arm_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 1);
		}
	}
	else
	{
		/* Call out to "jit_memcpy" to effect the copy */
		//Load the parameters in the right registers
		//R2 <- size
		mov_reg_imm(gen, &inst, ARM_R2, size);
		//R1 <- source pointer
		if(soffset == 0)
		{
			arm_mov_reg_reg(inst, ARM_R1, sreg);
		}
		else
		{
			arm_alu_reg_imm(inst, ARM_ADD, temp_reg, sreg, soffset);
			arm_mov_reg_reg(inst, ARM_R1, temp_reg);
		}
		//R0 <- destination pointer
		/* On ARM, the stack doesn't need special treatment, since parameters
		   are passed using registers, not using the stack as it's done on x86 */
		if(doffset == 0)
		{
			arm_mov_reg_reg(inst, ARM_R0, dreg);
		}
		else
		{
			arm_alu_reg_imm(inst, ARM_ADD, temp_reg, dreg, doffset);
			arm_mov_reg_reg(inst, ARM_R0, temp_reg);
		}
		
		arm_call(inst, jit_memcpy);
	}
	return inst;
}

/*
 * Flush the contents of the constant pool.
 */
static void flush_constants(jit_gencode_t gen, int after_epilog)
{
	arm_inst_buf inst;
	arm_inst_word *patch;
	arm_inst_word *current;
	arm_inst_word *fixup;
	int index, value, offset;

	/* Bail out if there are no constants to flush */
	if(!(gen->num_constants))
	{
		return;
	}

	/* Initialize the cache output pointer */
	jit_gen_load_inst_ptr(gen, inst);

	/* Jump over the constant pool if it is being output inline */
	if(!after_epilog)
	{
		patch = arm_inst_get_posn(inst);
		arm_jump_imm(inst, 0);
	}
	else
	{
		patch = 0;
	}

	/* Align the constant pool, if requested */
	if(gen->align_constants && (((int)arm_inst_get_posn(inst)) & 7) != 0)
	{
		arm_inst_add(inst, 0);
	}

	/* Output the constant values and apply the necessary fixups */
	for(index = 0; index < gen->num_constants; ++index)
	{
		current = arm_inst_get_posn(inst);
		arm_inst_add(inst, gen->constants[index]);
		fixup = gen->fixup_constants[index];
		while(fixup != 0)
		{
			if((*fixup & 0x0F000000) == 0x05000000)
			{
				/* Word constant fixup */
				value = *fixup & 0x0FFF;
				offset = ((arm_inst_get_posn(inst) - 1 - fixup) * 4) - 8;
				*fixup = ((*fixup & ~0x0FFF) | offset);
			}
			else
			{
				/* Floating-point constant fixup */
				value = (*fixup & 0x00FF) * 4;
				offset = ((arm_inst_get_posn(inst) - 1 - fixup) * 4) - 8;
				*fixup = ((*fixup & ~0x00FF) | (offset / 4));
			}
			if(value)
			{
				fixup -= value / sizeof(void *);
			}
			else
			{
				fixup = 0;
			}
		}
	}

	/* Backpatch the jump if necessary */
	if(!after_epilog)
	{
		arm_patch(inst, patch, arm_inst_get_posn(inst));
	}

	/* Flush the pool state and restart */
	gen->num_constants = 0;
	gen->align_constants = 0;
	gen->first_constant_use = 0;
	jit_gen_save_inst_ptr(gen, inst);
}

/*
 * Perform a constant pool flush if we are too far from the starting point.
 */
static int flush_if_too_far(jit_gencode_t gen)
{
	if(gen->first_constant_use &&
		  (((arm_inst_word *)(gen->ptr)) -
		  ((arm_inst_word *)(gen->first_constant_use))) >= 100)
	{
		flush_constants(gen, 0);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Add a fixup for a particular constant pool entry.
 */
static void add_constant_fixup
		(jit_gencode_t gen, int index, arm_inst_word *fixup)
{
	arm_inst_word *prev;
	int value;
	if(((unsigned char *)fixup) >= gen->limit)
	{
		/* The instruction buffer is full, so don't record this fixup */
		return;
	}
	prev = gen->fixup_constants[index];
	if(prev)
	{
		value = (fixup - prev) * sizeof(void *);
	}
	else
	{
		value = 0;
	}
	if((*fixup & 0x0F000000) == 0x05000000)
	{
		//Word fixup
		*fixup = ((*fixup & ~0x0FFF) | value);
	}
	else
	{
		//Float fixup
		*fixup = ((*fixup & ~0x00FF) | (value / 4));
	}
	gen->fixup_constants[index] = fixup;
	if(!(gen->first_constant_use))
	{
		gen->first_constant_use = fixup;
	}
}

/*
 * Add an immediate value to the constant pool.  The constant
 * is loaded from the instruction at "fixup".
 */
static void add_constant(jit_gencode_t gen, int value, arm_inst_word *fixup)
{
	int index;

	/* Search the constant pool for an existing copy of the value */
	for(index = 0; index < gen->num_constants; ++index)
	{
		if(gen->constants[index] == value)
		{
			add_constant_fixup(gen, index, fixup);
			return;
		}
	}

	/* Flush the constant pool if there is insufficient space */
	if(gen->num_constants >= JIT_ARM_MAX_CONSTANTS)
	{
		flush_constants(gen, 0);
	}

	/* Add the constant value to the pool */
	gen->constants[gen->num_constants] = value;
	gen->fixup_constants[gen->num_constants] = 0;
	++(gen->num_constants);
	add_constant_fixup(gen, gen->num_constants - 1, fixup);
}

/*
 * Add a double-word immediate value to the constant pool.
 */
static void add_constant_dword
		(jit_gencode_t gen, int value1, int value2, arm_inst_word *fixup, int align)
{
	int index;

	/* Make sure that the constant pool is properly aligned when output */
	if(align)
	{
		gen->align_constants = 1;
	}

	/* Search the constant pool for an existing copy of the value */
	for(index = 0; index < (gen->num_constants - 1); ++index)
	{
		if(gen->constants[index] == value1 &&
				 gen->constants[index + 1] == value2)
		{
			if(!align || (index % 2) == 0)
			{
				add_constant_fixup(gen, index, fixup);
				return;
			}
		}
	}

	/* Flush the constant pool if there is insufficient space */
	if(gen->num_constants >= (JIT_ARM_MAX_CONSTANTS - 1))
	{
		flush_constants(gen, 0);
	}

	/* Align the constant pool on a 64-bit boundary if necessary */
	if(align && (gen->num_constants % 2) != 0)
	{
		gen->constants[gen->num_constants] = 0;
		gen->fixup_constants[gen->num_constants] = 0;
		++(gen->num_constants);
	}

	/* Add the double word constant value to the pool */
	gen->constants[gen->num_constants] = value1;
	gen->fixup_constants[gen->num_constants] = 0;
	gen->constants[gen->num_constants+1] = value2;
	gen->fixup_constants[gen->num_constants+1] = 0;
	gen->num_constants += 2;
	add_constant_fixup(gen, gen->num_constants - 2, fixup);
}

/*
 * Load an immediate value into a word register.  If the value is
 * complicated, then add an entry to the constant pool.
 */
static void mov_reg_imm
		(jit_gencode_t gen, arm_inst_buf *inst, int reg, int value)
{
	arm_inst_word *fixup;

	/* Bail out if the value is not complex enough to need a pool entry */
	if(!arm_is_complex_imm(value))
	{
		arm_mov_reg_imm(*inst, reg, value);
		return;
	}

	/* Output a placeholder to load the value later */
	fixup = arm_inst_get_posn(*inst);
	arm_load_membase(*inst, reg, ARM_PC, 0);

	/* Add the constant to the pool, which may cause a flush */
	jit_gen_save_inst_ptr(gen, *inst);
	add_constant(gen, value, fixup);
	jit_gen_load_inst_ptr(gen, *inst);
}

/*
 * Load a float32 immediate value into a float register.  If the value is
 * complicated, then add an entry to the constant pool.
 */
static void mov_freg_imm_32
		(jit_gencode_t gen, arm_inst_buf *inst, int reg, int value)
{
	arm_inst_word *fixup;

	/* Output a placeholder to load the value later */
	fixup = arm_inst_get_posn(*inst);
	arm_load_membase_float32(*inst, reg, ARM_PC, 0);

	/* Add the constant to the pool, which may cause a flush */
	jit_gen_save_inst_ptr(gen, *inst);
	add_constant(gen, value, fixup);
	jit_gen_load_inst_ptr(gen, *inst);
}

/*
 * Load a float64 immediate value into a float register.  If the value is
 * complicated, then add an entry to the constant pool.
 */
static void mov_freg_imm_64
		(jit_gencode_t gen, arm_inst_buf *inst, int reg, int value1, int value2)
{
	arm_inst_word *fixup;

	/* Output a placeholder to load the value later */
	fixup = arm_inst_get_posn(*inst);
	arm_load_membase_float64(*inst, reg, ARM_PC, 0);

	/* Add the constant to the pool, which may cause a flush */
	jit_gen_save_inst_ptr(gen, *inst);
	add_constant_dword(gen, value1, value2, fixup, 1);
	jit_gen_load_inst_ptr(gen, *inst);
}

/*
 * Output a branch instruction.
 */
static void output_branch
		(jit_function_t func, arm_inst_buf *inst, int cond, jit_insn_t insn)
{
	jit_block_t block;
	int offset;
	//block = jit_block_from_label(func, (jit_label_t)(insn->dest));
	if((insn->flags & JIT_INSN_VALUE1_IS_LABEL) != 0)
	{
		/* "address_of_label" instruction */
		block = jit_block_from_label(func, (jit_label_t)(insn->value1));
	}
	else
	{
		block = jit_block_from_label(func, (jit_label_t)(insn->dest));
	}
	if(!block)
	{
		return;
	}
	if(arm_inst_get_posn(*inst) >= arm_inst_get_limit(*inst))
	{
		/* The buffer has overflowed, so don't worry about fixups */
		return;
	}
	if(block->address)
	{
		/* We already know the address of the block */
		arm_branch(*inst, cond, block->address);
	}
	else
	{
		/* Output a placeholder and record on the block's fixup list */
		if(block->fixup_list)
		{
			offset = (int)(((unsigned char *)arm_inst_get_posn(*inst)) -
					((unsigned char *)(block->fixup_list)));
		}
		else
		{
			offset = 0;
		}
		arm_branch_imm(*inst, cond, offset);
		block->fixup_list = (void *)(arm_inst_get_posn(*inst) - 1);
	}
}

/*
 * Throw a builtin exception.
 */
static void throw_builtin
		(arm_inst_buf *inst, jit_function_t func, int cond, int type)
{
	arm_inst_word *patch;

	/* Branch past the following code if "cond" is not true */
	patch = arm_inst_get_posn(*inst);
	arm_branch_imm(*inst, cond ^ 0x01, 0);

	/* We need to update "catch_pc" if we have a "try" block */
	if(func->builder->setjmp_value != 0)
	{
		_jit_gen_fix_value(func->builder->setjmp_value);
		arm_mov_reg_reg(*inst, ARM_WORK, ARM_PC);
		arm_store_membase(*inst, ARM_WORK, ARM_FP,
				   func->builder->setjmp_value->frame_offset +
						   jit_jmp_catch_pc_offset);
	}

	/* Push the exception type onto the stack */
	arm_mov_reg_imm(*inst, ARM_WORK, type);
	arm_push_reg(*inst, ARM_WORK);

	/* Call the "jit_exception_builtin" function, which will never return */
	arm_call(*inst, jit_exception_builtin);

	/* Back-patch the previous branch instruction */
	arm_patch(*inst, patch, arm_inst_get_posn(*inst));
}

/*
 * Jump to the current function's epilog.
 */
static void jump_to_epilog
		(jit_gencode_t gen, arm_inst_buf *inst, jit_block_t block)
{
	int offset;

	/* If the epilog is the next thing that we will output,
	   then fall through to the epilog directly */
	if(_jit_block_is_final(block))
	{
		return;
	}

	/* Bail out if the instruction buffer has overflowed */
	if(arm_inst_get_posn(*inst) >= arm_inst_get_limit(*inst))
	{
		return;
	}

	/* Output a placeholder for the jump and add it to the fixup list */
	if(gen->epilog_fixup)
	{
		offset = (int)(((unsigned char *)arm_inst_get_posn(*inst)) -
				((unsigned char *)(gen->epilog_fixup)));
	}
	else
	{
		offset = 0;
	}
	arm_branch_imm(*inst, ARM_CC_AL, offset);
	gen->epilog_fixup = (void *)(arm_inst_get_posn(*inst) - 1);
}

#define	TODO()		\
	do { \
		fprintf(stderr, "TODO at %s, %d\n", __FILE__, (int)__LINE__); \
} while (0)

/*
 * -------------------- End of helper functions ------------------------
 */


/*
 * Initialize the backend.
 */
void _jit_init_backend(void)
{
	/*
	 * Init the various classes of registers
	 */

	/* WORD registers */
	arm_reg = _jit_regclass_create(
		"reg", JIT_REG_WORD, 9,
		ARM_REG_R0, ARM_REG_R1,
		ARM_REG_R2, ARM_REG_R3,
		ARM_REG_R4, ARM_REG_R5,
		ARM_REG_R6, ARM_REG_R7,
		ARM_REG_R8);

#ifdef JIT_ARM_HAS_FPA
	/* float registers */
	arm_freg = _jit_regclass_create(
		"freg64", JIT_REG_ARM_FLOAT, 4,
		ARM_REG_F0, ARM_REG_F1,
		ARM_REG_F2, ARM_REG_F3,
		/*ARM_REG_F4, ARM_REG_F5,
		  ARM_REG_F6, ARM_REG_F7*/);
#endif

#ifdef JIT_ARM_HAS_VFP
	/* 32-bits float registers */
	arm_freg32 = _jit_regclass_create(
		"freg32", JIT_REG_ARM_FLOAT32, 16,
		ARM_REG_S0, ARM_REG_S1,
		ARM_REG_S2, ARM_REG_S3,
		ARM_REG_S4, ARM_REG_S5,
		ARM_REG_S6, ARM_REG_S7,
		ARM_REG_S8, ARM_REG_S9,
		ARM_REG_S10, ARM_REG_S11,
		ARM_REG_S12, ARM_REG_S13,
		ARM_REG_S14, ARM_REG_S15);

	/* 64-bits float registers */
	arm_freg64 = _jit_regclass_create(
		"freg64", JIT_REG_ARM_FLOAT64, 8,
		ARM_REG_D8, ARM_REG_D9,
		ARM_REG_D10, ARM_REG_D11,
		ARM_REG_D12, ARM_REG_D13,
		ARM_REG_D14, ARM_REG_D15);
#endif

	/* Long registers */
	arm_lreg = _jit_regclass_create(
		"lreg", JIT_REG_LONG, 2,
		ARM_REG_R0, ARM_REG_R2);
}

void _jit_gen_get_elf_info(jit_elf_info_t *info)
{
	info->machine = 40;		/* EM_ARM */
	info->abi = 0;			/* ELFOSABI_SYSV */
	info->abi_version = 0;
}

int _jit_setup_indirect_pointer(jit_function_t func, jit_value_t value)
{
	return jit_insn_outgoing_reg(func, value, ARM_WORK);
}

int _jit_create_call_return_insns
	(jit_function_t func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args,
	 jit_value_t return_value, int is_nested)
{
	jit_type_t return_type;
	int ptr_return;

	/* Bail out now if we don't need to worry about return values */
	return_type = jit_type_normalize(jit_type_get_return(signature));
	ptr_return = jit_type_return_via_pointer(return_type);
	if(!return_value || ptr_return)
	{
		return 1;
	}

	/* Structure values must be flushed into the frame, and
	   everything else ends up in a register */
	if(jit_type_is_struct(return_type) || jit_type_is_union(return_type))
	{
		if(!jit_insn_flush_struct(func, return_value))
		{
			return 0;
		}
	}
	else if(return_type->kind != JIT_TYPE_VOID)
	{
		if(!jit_insn_return_reg(func, return_value, ARM_REG_R0))
		{
			return 0;
		}
	}

	/* Everything is back where it needs to be */
	return 1;
}

int _jit_opcode_is_supported(int opcode)
{
	switch(opcode)
	{
		#define JIT_INCLUDE_SUPPORTED
		#include "jit-rules-arm.inc"
		#undef JIT_INCLUDE_SUPPORTED
	}
	return 0;
}

void *_jit_gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf)
{
	unsigned int prolog[JIT_PROLOG_SIZE / sizeof(int)];
	arm_inst_buf inst;
	int reg, regset;
	unsigned int saved;
	unsigned int frame_size;
	unsigned int stack_growth;
	
	/* Initialize the instruction buffer */
	arm_inst_buf_init(inst, prolog, prolog + JIT_PROLOG_SIZE / sizeof(int));

	/* Determine which registers need to be preserved */
	regset = 0;
	saved = 0;
	for(reg = 0; reg <= 15; ++reg)
	{
		if(jit_reg_is_used(gen->touched, reg) &&
		   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
		{
			regset |= (1 << reg);
			saved += sizeof(void *);
		}
	}

	/* Setup the frame, pushing all the callee-save registers */
	arm_setup_frame(inst, regset);

	/* Allocate space for the local variable frame.  Subtract off
	   the space for the registers that we just saved.  The pc, lr,
	   and fp registers are always saved, so account for them too */
	stack_growth=(saved + 4 * sizeof(void *));
	frame_size = func->builder->frame_size - stack_growth;
	frame_size = func->builder->frame_size - (saved + 3 * sizeof(void *));
	frame_size += (unsigned int)(func->builder->param_area_size);
	while(frame_size % JIT_SP_ALIGN_PUBLIC != 0)
	{
		//Pad to reach the required stacl pointer alignment
		frame_size++;
	}
	
	/* If the registers that get saved on the stack make it grow of a odd number
	 * of words, the preceding while isn't able anymore to compute the correct
	 * allignment. The following adds a correction when needed.
	 */
	if (stack_growth % JIT_SP_ALIGN_PUBLIC != 0)
	{
		//Pad to reach the required stack pointer alignment
		frame_size += (stack_growth % JIT_SP_ALIGN_PUBLIC);
	}
	
	if(frame_size > 0)
	{
		arm_alu_reg_imm(inst, ARM_SUB, ARM_SP, ARM_SP, frame_size);
	}

	/* Copy the prolog into place and return the adjusted entry position */
	reg = (int)((arm_inst_get_posn(inst) - prolog) * sizeof(unsigned int));
	jit_memcpy(((unsigned char *)buf) + JIT_PROLOG_SIZE - reg, prolog, reg);
	return (void *)(((unsigned char *)buf) + JIT_PROLOG_SIZE - reg);
}

void _jit_gen_epilog(jit_gencode_t gen, jit_function_t func)
{
	int reg, regset;
	arm_inst_buf inst;
	void **fixup;
	void **next;
	jit_nint offset;

	/* Initialize the instruction buffer */
	jit_gen_load_inst_ptr(gen, inst);

	/* Determine which registers need to be restored when we return */
	regset = 0;
	for(reg = 0; reg <= 15; ++reg)
	{
		if(jit_reg_is_used(gen->touched, reg) &&
		   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
		{
			regset |= (1 << reg);
		}
	}

	/* Apply fixups for blocks that jump to the epilog */
	fixup = (void **)(gen->epilog_fixup);
	while(fixup != 0)
	{
		offset = (((jit_nint)(fixup[0])) & 0x00FFFFFF) << 2;
		if(!offset)
		{
			next = 0;
		}
		else
		{
			next = (void **)(((unsigned char *)fixup) - offset);
		}
		arm_patch(inst, fixup, arm_inst_get_posn(inst));
		fixup = next;
	}
	gen->epilog_fixup = 0;

	/* Pop the local stack frame and return */
	arm_pop_frame(inst, regset);
	jit_gen_save_inst_ptr(gen, inst);

	/* Flush the remainder of the constant pool */
	flush_constants(gen, 1);
}

#if 0
/*
 * The ARM backend does not need this function because it uses
 * _jit_create_indirector() instead.
 */
void *_jit_gen_redirector(jit_gencode_t gen, jit_function_t func)
{
	void *ptr, *entry;
	arm_inst_buf inst;
	jit_gen_load_inst_ptr(gen, inst);
	ptr = (void *)&(func->entry_point);
	entry = gen->ptr;
	arm_load_membase(inst, ARM_WORK, ARM_PC, 0);
	arm_load_membase(inst, ARM_PC, ARM_WORK, 0);
	arm_inst_add(inst, (unsigned int)ptr);
	jit_gen_save_inst_ptr(gen, inst);
	return entry;
}
#endif 
/*
 * Setup or teardown the ARM code output process.
 */
#define	jit_cache_setup_output(needed)	\
	arm_inst_buf inst; \
	jit_gen_load_inst_ptr(gen, inst)
#define	jit_cache_end_output()	\
	jit_gen_save_inst_ptr(gen, inst)

/** 
 * Spill the content of register "reg" (and "other_reg", if it's different from -1) 
 * into the global register or the memory area associated with "value"
 * NB: it doesn't set value->in_global_register or value->in_frame. The caller has to
 * take care of that.
 */
void _jit_gen_spill_reg(jit_gencode_t gen, int reg,
						int other_reg, jit_value_t value)
{
	int offset;

	/* Make sure that we have sufficient space */
	jit_cache_setup_output(32);
	if(flush_if_too_far(gen))
	{
		jit_gen_load_inst_ptr(gen, inst);
	}

	/* Output an appropriate instruction to spill the value */
	if(value->has_global_register)
	{
		if (IS_FLOAT_REG(reg))
		{
			printf("TODO:Copy from float reg to global reg is not handled properly in %s\n", __FILE__);
			abort();
		}
		else
		{
			arm_mov_reg_reg(inst, _jit_reg_info[value->global_reg].cpu_reg, _jit_reg_info[reg].cpu_reg);
		}
	}
	else
	{
		_jit_gen_fix_value(value);
		offset = (int)(value->frame_offset);
		if(IS_WORD_REG(reg))
		{
			arm_store_membase(inst, reg, ARM_FP, offset);
			if(other_reg != -1)
			{
				/* Spill the other word register in a pair */
				offset += sizeof(void *);
				arm_store_membase(inst, other_reg, ARM_FP, offset);
			}
		}
		else if(jit_type_normalize(value->type)->kind == JIT_TYPE_FLOAT32)
		{
			arm_store_membase_float32(inst, _jit_reg_info[value->reg].cpu_reg, ARM_FP, offset);
		}
		else
		{
			arm_store_membase_float64(inst, _jit_reg_info[value->reg].cpu_reg, ARM_FP, offset);
		}
	}

	/* End the code output process */
	jit_cache_end_output();
}

void _jit_gen_free_reg(jit_gencode_t gen, int reg,
					   int other_reg, int value_used)
{
	/* We don't have to do anything to free ARM registers */
}

/*
 * Loads the content of the value @var{value} into register @var{reg} and (if needed) @var{other_reg}
 */
void _jit_gen_load_value
	(jit_gencode_t gen, int reg, int other_reg, jit_value_t value)
{
	int offset;

	/* Make sure that we have sufficient space */
	jit_cache_setup_output(32);
	if(flush_if_too_far(gen))
	{
		jit_gen_load_inst_ptr(gen, inst);
	}

	if(value->is_constant)
	{
		/* Determine the type of constant to be loaded */
		switch(jit_type_normalize(value->type)->kind)
		{
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				mov_reg_imm(gen, &inst, _jit_reg_info[reg].cpu_reg,
							(jit_nint)(value->address));
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			{
				jit_long long_value;
				long_value = jit_value_get_long_constant(value);
				mov_reg_imm(gen, &inst, _jit_reg_info[reg].cpu_reg,
						    (jit_int)long_value);
				mov_reg_imm(gen, &inst, _jit_reg_info[reg].cpu_reg + 1,
						    (jit_int)(long_value >> 32));
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				jit_float32 float32_value;
				float32_value = jit_value_get_float32_constant(value);
				if(IS_WORD_REG(reg))
				{
					mov_reg_imm(gen, &inst, _jit_reg_info[reg].cpu_reg,
							    *((int *)&float32_value));
				}
				else
				{
					mov_freg_imm_32
						(gen, &inst, _jit_reg_info[reg].cpu_reg,
					     *((int *)&float32_value));
				}
			}
			break;

			case JIT_TYPE_FLOAT64:
			case JIT_TYPE_NFLOAT:
			{
				jit_float64 float64_value;
				float64_value = jit_value_get_float64_constant(value);
				if(IS_WORD_REG(reg))
				{
					mov_reg_imm
						(gen, &inst, _jit_reg_info[reg].cpu_reg,
						 ((int *)&float64_value)[0]);
					mov_reg_imm
						(gen, &inst, _jit_reg_info[reg].cpu_reg + 1,
						 ((int *)&float64_value)[1]);
				}
				else
				{
					mov_freg_imm_64
						(gen, &inst, _jit_reg_info[reg].cpu_reg,
					     ((int *)&float64_value)[0],
					     ((int *)&float64_value)[1]);
				}
			}
			break;
		}
	}
	else if(value->in_global_register)
	{
		/* Load the value out of a global register */
		if(IS_FLOAT_REG(reg))
		{
			/* Load into a floating point register */
#ifdef JIT_ARM_HAS_VFP
			/* Vector Floating Point instructions */
			if(jit_type_normalize(value->type)->kind == JIT_TYPE_FLOAT32)
			{
				arm_mov_float_reg(inst,
						  _jit_reg_info[reg].cpu_reg,
						  _jit_reg_info[value->global_reg].cpu_reg);
			}
			else
			{
				//JIT_TYPE_FLOAT64 or JIT_TYPE_NFLOAT
				arm_mov_double_reg_reg(inst,
						       _jit_reg_info[reg].cpu_reg,
						       _jit_reg_info[value->global_reg].cpu_reg,
						       _jit_reg_info[value->global_reg].cpu_reg + 1);
			}
#endif
#ifdef JIT_ARM_HAS_FPA
			/* Floating Point Architecture instructions */
			TODO();
			abort();
#endif
		}
		else
		{
			/* Load into a general-purpose register */
			arm_mov_reg_reg(inst,
					_jit_reg_info[reg].cpu_reg,
					_jit_reg_info[value->global_reg].cpu_reg);
		}
	}
	else if(value->in_register)
	{
		/* The value is already in another register. Move it */
		switch(jit_type_normalize(value->type)->kind)
		{
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				arm_mov_reg_reg(inst, jit_reg_code(reg), jit_reg_code(value->reg));
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			{
				assert(jit_reg_code(other_reg) !=-1);
				assert(jit_reg_other_reg(value->reg) != -1);
				
				arm_mov_reg_reg(inst, jit_reg_code(reg), jit_reg_code(value->reg));
				arm_mov_reg_reg(inst, jit_reg_code(other_reg), jit_reg_other_reg(value->reg));
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
#ifdef JIT_ARM_HAS_VFP
				/* Vector Floating Point instructions */
				if(IS_FLOAT_REG(reg))
				{
					if(IS_WORD_REG(value->reg))
					{
						arm_mov_float_reg(inst,
								   jit_reg_code(reg),
								   jit_reg_code(value->reg));
					}
					else
					{
						arm_alu_freg_32(inst, ARM_MVF, 
								 jit_reg_code(reg),
								 jit_reg_code(value->reg));
					}
				}
				else
				{
					if(IS_WORD_REG(value->reg))
					{
						arm_mov_reg_reg(inst,
								 jit_reg_code(reg),
								 jit_reg_code(value->reg));
					}
					else
					{
						arm_mov_reg_float(inst,
								   jit_reg_code(reg),
								   jit_reg_code(value->reg));
					}
				}
#endif
#ifdef JIT_ARM_HAS_FPA
				/* Floating Point Architecture instructions */
				TODO();
				abort();
#endif
			}
			break;
			
			case JIT_TYPE_FLOAT64:
			case JIT_TYPE_NFLOAT:
			{
#ifdef JIT_ARM_HAS_VFP
				/* Vector Floating Point instruction */
				if(IS_FLOAT_REG(reg))
				{
					if(IS_WORD_REG(value->reg))
					{
						assert(jit_reg_other_reg(value->reg) != -1);
						
						arm_mov_double_reg_reg(inst,
									jit_reg_code(reg),
									jit_reg_code(value->reg),
									jit_reg_other_reg(value->reg));
					}
					else
					{
						arm_alu_freg(inst, ARM_MVF,
							      jit_reg_code(reg),
							      jit_reg_code(value->reg));
					}
				}
				else
				{
					if(IS_WORD_REG(value->reg))
					{
						arm_mov_reg_reg(inst,
								 jit_reg_code(reg),
								 jit_reg_code(value->reg));
					}
					else
					{
						assert(jit_reg_other_reg(reg));
						arm_mov_reg_reg_double(inst,
									jit_reg_code(reg),
									jit_reg_other_reg(reg),
									jit_reg_code(value->reg));
					}
				}
#endif
#ifdef JIT_ARM_HAS_FPA
				/* Floating Point Architecture instructions */
				TODO();
				abort();
#endif
			}
			break;
		}
	}
	else
	{
		/* Load from the stack */
		assert(!value->in_global_register && !value->is_constant && !value->in_register);
		
		/* Fix the position of the value in the stack frame */
		_jit_gen_fix_value(value);
		offset = (int)(value->frame_offset);

		switch(jit_type_normalize(value->type)->kind)
		{
			case JIT_TYPE_SBYTE:
			{
				arm_load_membase_sbyte(inst, _jit_reg_info[reg].cpu_reg,
								ARM_FP, offset);
			}
			break;

			case JIT_TYPE_UBYTE:
			{
				arm_load_membase_byte(inst, _jit_reg_info[reg].cpu_reg,
								ARM_FP, offset);
			}
			break;

			case JIT_TYPE_SHORT:
			{
				arm_load_membase_short(inst, _jit_reg_info[reg].cpu_reg,
								ARM_FP, offset);
			}
			break;

			case JIT_TYPE_USHORT:
			{
				arm_load_membase_ushort(inst, _jit_reg_info[reg].cpu_reg,
									ARM_FP, offset);
			}
			break;

			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				arm_load_membase(inst, _jit_reg_info[reg].cpu_reg,
								ARM_FP, offset);
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			{
				arm_load_membase(inst, _jit_reg_info[reg].cpu_reg,
								ARM_FP, offset);
				arm_load_membase(inst, _jit_reg_info[reg].cpu_reg + 1,
								ARM_FP, offset + 4);
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				if(IS_WORD_REG(reg))
				{
					arm_load_membase(inst, _jit_reg_info[reg].cpu_reg,
									ARM_FP, offset);
				}
				else
				{
					arm_load_membase_float32
						(inst, _jit_reg_info[reg].cpu_reg, ARM_FP, offset);
				}
			}
			break;

			case JIT_TYPE_FLOAT64:
			case JIT_TYPE_NFLOAT:
			{
				if(IS_WORD_REG(reg))
				{
					arm_load_membase(inst, _jit_reg_info[reg].cpu_reg,
									ARM_FP, offset);
					arm_load_membase(inst, _jit_reg_info[reg].cpu_reg + 1,
									ARM_FP, offset + 4);
				}
				else
				{
					arm_load_membase_float64
						(inst, _jit_reg_info[reg].cpu_reg, ARM_FP, offset);
				}
			}
			break;
		}
	}

	/* End the code output process */
	jit_cache_end_output();
}

/**
 * Loads a struct indicated by "value" into the given register reg
 */
void _jit_gen_load_value_struct (jit_gencode_t gen, int reg, jit_value_t value)
{
	int offset;

	/* Make sure that we have sufficient space */
	jit_cache_setup_output(32);
	if(flush_if_too_far(gen))
	{
		jit_gen_load_inst_ptr(gen, inst);
	}

	if(value->is_constant)
	{
		TODO();
		abort();
	}
	else if(value->has_global_register)
	{
		/*
		 * This value has been assinged a global register. This means
		 * that it can use that register, but not necessarily that it's
		 * already in it!!
		 */
		
		/* Ensure that the value is already in the global_register */
		if (!value->in_global_register)
		{	
			/* Find the other register in a long pair */
			int reg = value->reg;
			int other_reg = jit_reg_current_other_reg(gen,reg);
			
			//Spill to the global register
			_jit_gen_spill_reg(gen, reg, other_reg, value);
			value->in_global_register=1;

			/* A new instruction has probably been generated by _jit_gen_spill_reg: reload the inst pointer */
			jit_gen_load_inst_ptr(gen, inst);
		}
		/* Load the value out of a global register */
		arm_mov_reg_reg(inst, _jit_reg_info[reg].cpu_reg,
						_jit_reg_info[value->global_reg].cpu_reg);
	}
	else
	{
		/* Fix the position of the value in the stack frame */
		_jit_gen_fix_value(value);
		offset = (int)(value->frame_offset);

		/* Ensure that the value is already in the stack frame */
		if(value->in_register)
		{
			/* Find the other register in a long pair */
			int reg = value->reg;
			int other_reg = jit_reg_current_other_reg(gen,reg);
			
			_jit_gen_spill_reg(gen, reg, other_reg, value);
			value->in_frame=1;

			/* A new instruction has probably been generated by _jit_gen_spill_reg: reload the inst pointer */
			jit_gen_load_inst_ptr(gen, inst);
		}
		
		assert(jit_type_normalize(value->type)->kind==JIT_TYPE_STRUCT);
		
		arm_load_membase(inst, _jit_reg_info[reg].cpu_reg, ARM_FP, offset);
		if (jit_type_get_size(jit_value_get_type(value)) > 4)
		{
			TODO();
			abort();
		}
		
	}

	/* End the code output process */
	jit_cache_end_output();
}

void _jit_gen_spill_global(jit_gencode_t gen, int reg, jit_value_t value)
{
	/* TODO: Implement if ARM needs it. */
}

void _jit_gen_load_global(jit_gencode_t gen, int reg, jit_value_t value)
{
	jit_cache_setup_output(32);
	arm_load_membase(inst, _jit_reg_info[value->global_reg].cpu_reg,
					 ARM_FP, value->frame_offset);
	jit_cache_end_output();
}

void _jit_gen_fix_value(jit_value_t value)
{
	if(!(value->has_frame_offset) && !(value->is_constant))
	{
		jit_nint size = (jit_nint)(ROUND_STACK(jit_type_get_size(value->type)));
		value->block->func->builder->frame_size += size;
		value->frame_offset = -(value->block->func->builder->frame_size);
		value->has_frame_offset = 1;
	}
}

void _jit_gen_insn(jit_gencode_t gen, jit_function_t func,
				   jit_block_t block, jit_insn_t insn)
{
	flush_if_too_far(gen);
	switch(insn->opcode)
	{
		#define JIT_INCLUDE_RULES
		#include "jit-rules-arm.inc"
		#undef JIT_INCLUDE_RULES

		default:
		{
			fprintf(stderr, "TODO(%x) at %s, %d\n",
					(int)(insn->opcode), __FILE__, (int)__LINE__);
		}
		break;
	}
}

void _jit_gen_start_block(jit_gencode_t gen, jit_block_t block)
{
	void **fixup;
	void **next;
	jit_nint offset;
	arm_inst_buf inst;

	/* Set the address of this block */
	block->address = (void *)(gen->ptr);

	/* If this block has pending fixups, then apply them now */
	fixup = (void **)(block->fixup_list);
	while(fixup != 0)
	{
		offset = (((jit_nint)(fixup[0])) & 0x00FFFFFF) << 2;
		if(!offset)
		{
			next = 0;
		}
		else
		{
			next = (void **)(((unsigned char *)fixup) - offset);
		}
		jit_gen_load_inst_ptr(gen, inst);
		arm_patch(inst, fixup, block->address);
		fixup = next;
	}
	block->fixup_list = 0;
}

void _jit_gen_end_block(jit_gencode_t gen, jit_block_t block)
{
	/* Nothing to do here for ARM */
}

int _jit_gen_is_global_candidate(jit_type_t type)
{
	switch(jit_type_remove_tags(type)->kind)
	{
		case JIT_TYPE_INT:
		case JIT_TYPE_UINT:
		case JIT_TYPE_NINT:
		case JIT_TYPE_NUINT:
		case JIT_TYPE_PTR:
		case JIT_TYPE_SIGNATURE:	return 1;
	}
	return 0;
}

int
_jit_reg_get_pair(jit_type_t type, int reg)
{
	type = jit_type_normalize(type);
	if(type)
	{
		if(type->kind == JIT_TYPE_LONG || type->kind == JIT_TYPE_ULONG)
		{
			return jit_reg_other_reg(reg);
		}
		else if(type->kind == JIT_TYPE_FLOAT64 || type->kind == JIT_TYPE_NFLOAT)
		{
			if(reg == ARM_REG_R0)
			{
				return jit_reg_other_reg(reg);
			}
		}
	}
	return -1;
}

#endif /* JIT_BACKEND_ARM */
