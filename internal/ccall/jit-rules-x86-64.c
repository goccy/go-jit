/*
 * jit-rules-x86-64.c - Rules that define the characteristics of the x86_64.
 *
 * Copyright (C) 2008  Southern Storm Software, Pty Ltd.
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

#if defined(JIT_BACKEND_X86_64)

#include "jit-gen-x86-64.h"
#include "jit-reg-alloc.h"
#include "jit-setjmp.h"
#include <stdio.h>

/*
 * Pseudo register numbers for the x86_64 registers. These are not the
 * same as the CPU instruction register numbers.  The order of these
 * values must match the order in "JIT_REG_INFO".
 */
#define	X86_64_REG_RAX		0
#define	X86_64_REG_RCX		1
#define	X86_64_REG_RDX		2
#define	X86_64_REG_RBX		3
#define	X86_64_REG_RSI		4
#define	X86_64_REG_RDI		5
#define	X86_64_REG_R8		6
#define	X86_64_REG_R9		7
#define	X86_64_REG_R10		8
#define	X86_64_REG_R11		9
#define	X86_64_REG_R12		10
#define	X86_64_REG_R13		11
#define	X86_64_REG_R14		12
#define	X86_64_REG_R15		13
#define	X86_64_REG_RBP		14
#define	X86_64_REG_RSP		15
#define	X86_64_REG_XMM0		16
#define	X86_64_REG_XMM1		17
#define	X86_64_REG_XMM2		18
#define	X86_64_REG_XMM3		19
#define	X86_64_REG_XMM4		20
#define	X86_64_REG_XMM5		21
#define	X86_64_REG_XMM6		22
#define	X86_64_REG_XMM7		23
#define	X86_64_REG_XMM8		24
#define	X86_64_REG_XMM9		25
#define	X86_64_REG_XMM10	26
#define	X86_64_REG_XMM11	27
#define	X86_64_REG_XMM12	28
#define	X86_64_REG_XMM13	29
#define	X86_64_REG_XMM14	30
#define	X86_64_REG_XMM15	31
#define	X86_64_REG_ST0		32
#define	X86_64_REG_ST1		33
#define	X86_64_REG_ST2		34
#define	X86_64_REG_ST3		35
#define	X86_64_REG_ST4		36
#define	X86_64_REG_ST5		37
#define	X86_64_REG_ST6		38
#define	X86_64_REG_ST7		39

/*
 * Determine if a pseudo register number is general, xmm or fpu.
 */
#define	IS_GENERAL_REG(reg)	(((reg) & ~0x0f) == 0)
#define	IS_XMM_REG(reg)		(((reg) & ~0x0f) == 0x10)
#define	IS_FPU_REG(reg)		(((reg) & ~0x0f) == 0x20)

/*
 * Scratch register, that is used for calls via register and
 * for loading the exception pc to the setjmp buffer.
 * This register *MUST* not be used for parameter passing and
 * *MUST* not be a callee saved register.
 * For SysV abi R11 is perfect.
 */
#define X86_64_SCRATCH X86_64_R11

/*
 * Set this definition to 1 if the OS supports the SysV red zone.
 * This is a 128 byte area below the stack pointer that is guaranteed
 * to be not modified by interrupts or signal handlers.
 * This allows us to use a temporary area on the stack without
 * having to modify the stack pointer saving us two instructions.
 * TODO: Make this a configure switch.
 */
#define HAVE_RED_ZONE 1

/*
 * Some declarations that should be replaced by querying the cpuinfo
 * if generating code for the current cpu.
 */
/*
#define HAVE_X86_SSE_4_1 0
#define HAVE_X86_SSE_4 0
#define HAVE_X86_SSE_3 0
#define HAVE_X86_FISTTP 0
*/

#define	TODO() \
do { \
	fprintf(stderr, "TODO at %s, %d\n", __FILE__, (int)__LINE__); \
} while(0)

/*
 * Setup or teardown the x86 code output process.
 */
#define	jit_cache_setup_output(needed)		\
	unsigned char *inst = gen->ptr;		\
	_jit_gen_check_space(gen, (needed))

#define	jit_cache_end_output()	\
	gen->ptr = inst

/*
 * Set this to 1 for debugging fixups
 */
#define DEBUG_FIXUPS 0

/*
 * The maximum block size copied inline
 */
#define _JIT_MAX_MEMCPY_INLINE	0x40

/*
 * The maximum block size set inline
 */
#define _JIT_MAX_MEMSET_INLINE 0x80

/*
 * va_list type as specified in x86_64 sysv abi version 0.99
 * Figure 3.34
 */
typedef struct
{
	unsigned int gp_offset;
	unsigned int fp_offset;
	void *overflow_arg_area;
	void *reg_save_area;
} _jit_va_list;

/* Registers used for INTEGER arguments */
static int _jit_word_arg_regs[] = {X86_64_REG_RDI, X86_64_REG_RSI,
								   X86_64_REG_RDX, X86_64_REG_RCX,
								   X86_64_REG_R8, X86_64_REG_R9};
#define _jit_num_word_regs	6

/* Registers used for float arguments */
static int _jit_float_arg_regs[] = {X86_64_REG_XMM0, X86_64_REG_XMM1,
									X86_64_REG_XMM2, X86_64_REG_XMM3,
									X86_64_REG_XMM4, X86_64_REG_XMM5,
									X86_64_REG_XMM6, X86_64_REG_XMM7};
#define _jit_num_float_regs	8

/* Registers used for returning INTEGER values */
static int _jit_word_return_regs[] = {X86_64_REG_RAX, X86_64_REG_RDX};
#define _jit_num_word_return_regs	2

/* Registers used for returning sse values */
static int _jit_sse_return_regs[] = {X86_64_REG_XMM0, X86_64_REG_XMM1};
#define _jit_num_sse_return_regs	2

/*
 * X86_64 register classes
 */
static _jit_regclass_t *x86_64_reg;		/* X86_64 general purpose registers */
static _jit_regclass_t *x86_64_creg;	/* X86_64 call clobbered general */
										/* purpose registers */
static _jit_regclass_t *x86_64_dreg;	/* general purpose registers that */
										/* can be used as divisor */
										/* (all but %rax and %rdx) */
static _jit_regclass_t *x86_64_rreg;	/* general purpose registers not used*/
										/* for returning values */
static _jit_regclass_t *x86_64_sreg;	/* general purpose registers that can*/
										/* be used for the value to be */
										/* shifted (all but %rcx)*/
										/* for returning values */
static _jit_regclass_t *x86_64_freg;	/* X86_64 fpu registers */
static _jit_regclass_t *x86_64_xreg;	/* X86_64 xmm registers */

void
_jit_init_backend(void)
{
	x86_64_reg = _jit_regclass_create(
		"reg", JIT_REG_WORD | JIT_REG_LONG, 14,
		X86_64_REG_RAX, X86_64_REG_RCX,
		X86_64_REG_RDX, X86_64_REG_RBX,
		X86_64_REG_RSI, X86_64_REG_RDI,
		X86_64_REG_R8, X86_64_REG_R9,
		X86_64_REG_R10, X86_64_REG_R11,
		X86_64_REG_R12, X86_64_REG_R13,
		X86_64_REG_R14, X86_64_REG_R15);

	/* register class with all call clobbered registers */
	x86_64_creg = _jit_regclass_create(
		"creg", JIT_REG_WORD | JIT_REG_LONG, 9,
		X86_64_REG_RAX, X86_64_REG_RCX,
		X86_64_REG_RDX, X86_64_REG_RSI,
		X86_64_REG_RDI, X86_64_REG_R8,
		X86_64_REG_R9, X86_64_REG_R10,
		X86_64_REG_R11);

	/* r egister class for divisors */
	x86_64_dreg = _jit_regclass_create(
		"dreg", JIT_REG_WORD | JIT_REG_LONG, 12,
		X86_64_REG_RCX, X86_64_REG_RBX,
		X86_64_REG_RSI, X86_64_REG_RDI,
		X86_64_REG_R8, X86_64_REG_R9,
		X86_64_REG_R10, X86_64_REG_R11,
		X86_64_REG_R12, X86_64_REG_R13,
		X86_64_REG_R14, X86_64_REG_R15);

	/* register class with all registers not used for returning values */
	x86_64_rreg = _jit_regclass_create(
		"rreg", JIT_REG_WORD | JIT_REG_LONG, 12,
		X86_64_REG_RCX, X86_64_REG_RBX,
		X86_64_REG_RSI, X86_64_REG_RDI,
		X86_64_REG_R8, X86_64_REG_R9,
		X86_64_REG_R10, X86_64_REG_R11,
		X86_64_REG_R12, X86_64_REG_R13,
		X86_64_REG_R14, X86_64_REG_R15);

	/* register class with all registers that can be used for shifted values */
	x86_64_sreg = _jit_regclass_create(
		"sreg", JIT_REG_WORD | JIT_REG_LONG, 13,
		X86_64_REG_RAX, X86_64_REG_RDX,
		X86_64_REG_RBX, X86_64_REG_RSI,
		X86_64_REG_RDI, X86_64_REG_R8,
		X86_64_REG_R9, X86_64_REG_R10,
		X86_64_REG_R11, X86_64_REG_R12,
		X86_64_REG_R13, X86_64_REG_R14,
		X86_64_REG_R15);

	x86_64_freg = _jit_regclass_create(
		"freg", JIT_REG_X86_64_FLOAT | JIT_REG_IN_STACK, 8,
		X86_64_REG_ST0, X86_64_REG_ST1,
		X86_64_REG_ST2, X86_64_REG_ST3,
		X86_64_REG_ST4, X86_64_REG_ST5,
		X86_64_REG_ST6, X86_64_REG_ST7);

	x86_64_xreg = _jit_regclass_create(
		"xreg", JIT_REG_FLOAT32 | JIT_REG_FLOAT64, 16,
		X86_64_REG_XMM0, X86_64_REG_XMM1,
		X86_64_REG_XMM2, X86_64_REG_XMM3,
		X86_64_REG_XMM4, X86_64_REG_XMM5,
		X86_64_REG_XMM6, X86_64_REG_XMM7,
		X86_64_REG_XMM8, X86_64_REG_XMM9,
		X86_64_REG_XMM10, X86_64_REG_XMM11,
		X86_64_REG_XMM12, X86_64_REG_XMM13,
		X86_64_REG_XMM14, X86_64_REG_XMM15);
}

int
_jit_opcode_is_supported(int opcode)
{
	switch(opcode)
	{
	#define JIT_INCLUDE_SUPPORTED
	#include "jit-rules-x86-64.inc"
	#undef JIT_INCLUDE_SUPPORTED
	}
	return 0;
}

int
_jit_setup_indirect_pointer(jit_function_t func, jit_value_t value)
{
	return jit_insn_outgoing_reg(func, value, X86_64_REG_R11);
}

/*
 * Do a xmm operation with a constant float32 value
 */
static int
_jit_xmm1_reg_imm_size_float32(jit_gencode_t gen, unsigned char **inst_ptr,
			       X86_64_XMM1_OP opc, int reg,
			       jit_float32 *float32_value)
{
	void *ptr;
	jit_nint offset;
	unsigned char *inst;

	inst = *inst_ptr;
	ptr = _jit_gen_alloc(gen, sizeof(jit_float32));
	if(!ptr)
	{
		return 0;
	}
	jit_memcpy(ptr, float32_value, sizeof(jit_float32));

	offset = (jit_nint)ptr - ((jit_nint)inst + (reg > 7 ? 9 : 8));
	if((offset >= jit_min_int) && (offset <= jit_max_int))
	{
		/* We can use RIP relative addressing here */
		x86_64_xmm1_reg_membase(inst, opc, reg,
									 X86_64_RIP, offset, 0);
	}
	else if(((jit_nint)ptr >= jit_min_int) &&
			((jit_nint)ptr <= jit_max_int))
	{
		/* We can use absolute addressing */
		x86_64_xmm1_reg_mem(inst, opc, reg, (jit_nint)ptr, 0);
	}
	else
	{
		/* We have to use an extra general register */
		TODO();
		return 0;
	}
	*inst_ptr = inst;
	return 1;
}

/*
 * Do a xmm operation with a constant float64 value
 */
static int
_jit_xmm1_reg_imm_size_float64(jit_gencode_t gen, unsigned char **inst_ptr,
			       X86_64_XMM1_OP opc, int reg,
			       jit_float64 *float64_value)
{
	void *ptr;
	jit_nint offset;
	unsigned char *inst;

	inst = *inst_ptr;
	ptr = _jit_gen_alloc(gen, sizeof(jit_float64));
	if(!ptr)
	{
		return 0;
	}
	jit_memcpy(ptr, float64_value, sizeof(jit_float64));

	offset = (jit_nint)ptr - ((jit_nint)inst + (reg > 7 ? 9 : 8));
	if((offset >= jit_min_int) && (offset <= jit_max_int))
	{
		/* We can use RIP relative addressing here */
		x86_64_xmm1_reg_membase(inst, opc, reg,
									 X86_64_RIP, offset, 1);
	}
	else if(((jit_nint)ptr >= jit_min_int) &&
			((jit_nint)ptr <= jit_max_int))
	{
		/* We can use absolute addressing */
		x86_64_xmm1_reg_mem(inst, opc, reg, (jit_nint)ptr, 1);
	}
	else
	{
		/* We have to use an extra general register */
		TODO();
		return 0;
	}
	*inst_ptr = inst;
	return 1;
}

/*
 * Do a logical xmm operation with packed float32 values
 */
static int
_jit_plops_reg_imm(jit_gencode_t gen, unsigned char **inst_ptr,
				   X86_64_XMM_PLOP opc, int reg, void *packed_value)
{
	void *ptr;
	jit_nint offset;
	unsigned char *inst;

	inst = *inst_ptr;
	ptr = _jit_gen_alloc(gen, 16);
	if(!ptr)
	{
		return 0;
	}
	jit_memcpy(ptr, packed_value, 16);

	/* calculate the offset for membase addressing */
	offset = (jit_nint)ptr - ((jit_nint)inst + (reg > 7 ? 8 : 7));
	if((offset >= jit_min_int) && (offset <= jit_max_int))
	{
		/* We can use RIP relative addressing here */
		x86_64_plops_reg_membase(inst, opc, reg, X86_64_RIP, offset);
		*inst_ptr = inst;
		return 1;
	}
	/* Check if mem addressing can be used */
	if(((jit_nint)ptr >= jit_min_int) &&
		((jit_nint)ptr <= jit_max_int))
	{
		/* We can use absolute addressing */
		x86_64_plops_reg_mem(inst, opc, reg, (jit_nint)ptr);
		*inst_ptr = inst;
		return 1;
	}
	/* We have to use an extra general register */
	TODO();
	return 0;
}

/*
 * Do a logical xmm operation with packed float64 values
 */
static int
_jit_plopd_reg_imm(jit_gencode_t gen, unsigned char **inst_ptr,
				   X86_64_XMM_PLOP opc, int reg, void *packed_value)
{
	void *ptr;
	jit_nint offset;
	unsigned char *inst;

	inst = *inst_ptr;
	ptr = _jit_gen_alloc(gen, 16);
	if(!ptr)
	{
		return 0;
	}
	jit_memcpy(ptr, packed_value, 16);

	/* calculate the offset for membase addressing */
	offset = (jit_nint)ptr - ((jit_nint)inst + (reg > 7 ? 9 : 8));
	if((offset >= jit_min_int) && (offset <= jit_max_int))
	{
		/* We can use RIP relative addressing here */
		x86_64_plopd_reg_membase(inst, opc, reg, X86_64_RIP, offset);
		*inst_ptr = inst;
		return 1;
	}
	/* Check if mem addressing can be used */
	if(((jit_nint)ptr >= jit_min_int) &&
		((jit_nint)ptr <= jit_max_int))
	{
		/* We can use absolute addressing */
		x86_64_plopd_reg_mem(inst, opc, reg, (jit_nint)ptr);
		*inst_ptr = inst;
		return 1;
	}
	/* We have to use an extra general register */
	TODO();
	return 0;
}

/*
 * Helpers for saving and setting roundmode in the fpu control word
 * and restoring it afterwards.
 * The rounding mode bits are bit 10 and 11 in the fpu control word.
 * sp_offset is the start offset of a temporary eight byte block.
 */
static unsigned char *
_x86_64_set_fpu_roundmode(unsigned char *inst, int scratch_reg,
						  int sp_offset, X86_64_ROUNDMODE mode)
{
	int fpcw_save_offset = sp_offset + 4;
	int fpcw_new_offset = sp_offset;
	int round_mode = ((int)mode) << 10;
	int round_mode_mask = ~(((int)X86_ROUND_ZERO) << 10);

	/* store FPU control word */
	x86_64_fnstcw_membase(inst, X86_64_RSP, fpcw_save_offset);
	/* load the value into the scratch register */
	x86_64_mov_reg_membase_size(inst, scratch_reg, X86_64_RSP, fpcw_save_offset, 2);
	/* Set the rounding mode */
	if(mode != X86_ROUND_ZERO)
	{
		/* Not all bits are set in the mask so we have to clear it first */
		x86_64_and_reg_imm_size(inst, scratch_reg, round_mode_mask, 2);
	}
	x86_64_or_reg_imm_size(inst, scratch_reg, round_mode, 2);
	/* Store the new round mode */
	x86_64_mov_membase_reg_size(inst, X86_64_RSP, fpcw_new_offset, scratch_reg, 2);
	/* Now load the new control word */
	x86_64_fldcw_membase(inst, X86_64_RSP, fpcw_new_offset);

	return inst;
}

static unsigned char *
_x86_64_restore_fpcw(unsigned char *inst, int sp_offset)
{
	int fpcw_save_offset = sp_offset + 4;

	/* Now load the saved control word */
	x86_64_fldcw_membase(inst, X86_64_RSP, fpcw_save_offset);

	return inst;
}

/*
 * Helpers for saving and setting roundmode in the mxcsr register and
 * restoring it afterwards.
 * The rounding mode bits are bit 13 and 14 in the mxcsr register.
 * sp_offset is the start offset of a temporary eight byte block.
 */
static unsigned char *
_x86_64_set_xmm_roundmode(unsigned char *inst, int scratch_reg,
						  int sp_offset, X86_64_ROUNDMODE mode)
{
	int mxcsr_save_offset = sp_offset + 4;
	int mxcsr_new_offset = sp_offset;
	int round_mode = ((int)mode) << 13;
	int round_mode_mask = ~(((int)X86_ROUND_ZERO) << 13);

	/* save the mxcsr register */
	x86_64_stmxcsr_membase(inst, X86_64_RSP, mxcsr_save_offset);
	/* Load the contents of the mxcsr register into the scratch register */
	x86_64_mov_reg_membase_size(inst, scratch_reg, X86_64_RSP, mxcsr_save_offset, 4);
	/* Set the rounding mode */
	if(mode != X86_ROUND_ZERO)
	{
		/* Not all bits are set in the mask so we have to clear it first */
		x86_64_and_reg_imm_size(inst, scratch_reg, round_mode_mask, 4);
	}
	x86_64_or_reg_imm_size(inst, scratch_reg, round_mode, 4);
	/* Store the new round mode */
	x86_64_mov_membase_reg_size(inst, X86_64_RSP, mxcsr_new_offset, scratch_reg, 4);
	/* and load it to the mxcsr register */
	x86_64_ldmxcsr_membase(inst, X86_64_RSP, mxcsr_new_offset);

	return inst;
}

static unsigned char *
_x86_64_restore_mxcsr(unsigned char *inst, int sp_offset)
{
	int mxcsr_save_offset = sp_offset + 4;

	/* restore the mxcsr register */
	x86_64_ldmxcsr_membase(inst, X86_64_RSP, mxcsr_save_offset);

	return inst;
}

/*
 * perform rounding of scalar single precision values.
 * We have to use the fpu where see4.1 is not supported.
 */
static unsigned char *
x86_64_rounds_reg_reg(unsigned char *inst, int dreg, int sreg,
					  int scratch_reg, X86_64_ROUNDMODE mode)
{
#ifdef HAVE_RED_ZONE
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundss_reg_reg(inst, dreg, sreg, mode);
#else
	/* Copy the xmm register to the stack */
	x86_64_movss_membase_reg(inst, X86_64_RSP, -16, sreg);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, -8, mode);
	/* Load the value to the fpu */
	x86_64_fld_membase_size(inst, X86_64_RSP, -16, 4);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, -8);
	/* and move st(0) to the destination register */
	x86_64_fstp_membase_size(inst, X86_64_RSP, -16, 4);
	x86_64_movss_reg_membase(inst, dreg, X86_64_RSP, -16);
#endif
#else
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundss_reg_reg(inst, dreg, sreg, mode);
#else
	/* allocate space on the stack for two ints and one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
	/* Copy the xmm register to the stack */
	x86_64_movss_regp_reg(inst, X86_64_RSP, sreg);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, 8, mode);
	/* Load the value to the fpu */
	x86_64_fld_regp_size(inst, X86_64_RSP, 4);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, 8);
	/* and move st(0) to the destination register */
	x86_64_fstp_regp_size(inst, X86_64_RSP, 4);
	x86_64_movss_reg_regp(inst, dreg, X86_64_RSP);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 16, 8);
#endif
#endif
	return inst;
}

static unsigned char *
x86_64_rounds_reg_membase(unsigned char *inst, int dreg, int offset,
						  int scratch_reg, X86_64_ROUNDMODE mode)
{
#ifdef HAVE_RED_ZONE
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundss_reg_membase(inst, dreg, X86_64_RBP, offset, mode);
#else
	/* Load the value to the fpu */
	x86_64_fld_membase_size(inst, X86_64_RBP, offset, 4);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, -8, mode);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, -8);
	/* and move st(0) to the destination register */
	x86_64_fstp_membase_size(inst, X86_64_RSP, -16, 4);
	x86_64_movss_reg_membase(inst, dreg, X86_64_RSP, -16);
#endif
#else
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundss_reg_membase(inst, dreg, X86_64_RBP, offset, mode);
#else
	/* allocate space on the stack for two ints and one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
	/* Load the value to the fpu */
	x86_64_fld_membase_size(inst, X86_64_RBP, offset, 4);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, 8, mode);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, 8);
	/* and move st(0) to the destination register */
	x86_64_fstp_regp_size(inst, X86_64_RSP, 4);
	x86_64_movss_reg_regp(inst, dreg, X86_64_RSP);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 16, 8);
#endif
#endif
	return inst;
}

/*
 * perform rounding of scalar double precision values.
 * We have to use the fpu where see4.1 is not supported.
 */
static unsigned char *
x86_64_roundd_reg_reg(unsigned char *inst, int dreg, int sreg,
					  int scratch_reg, X86_64_ROUNDMODE mode)
{
#ifdef HAVE_RED_ZONE
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundsd_reg_reg(inst, dreg, sreg, mode);
#else
	/* Copy the xmm register to the stack */
	x86_64_movsd_membase_reg(inst, X86_64_RSP, -16, sreg);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, -8, mode);
	/* Load the value to the fpu */
	x86_64_fld_membase_size(inst, X86_64_RSP, -16, 8);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, -8);
	/* and move st(0) to the destination register */
	x86_64_fstp_membase_size(inst, X86_64_RSP, -16, 8);
	x86_64_movsd_reg_membase(inst, dreg, X86_64_RSP, -16);
#endif
#else
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundsd_reg_reg(inst, dreg, sreg, mode);
#else
	/* allocate space on the stack for two ints and one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
	/* Copy the xmm register to the stack */
	x86_64_movsd_regp_reg(inst, X86_64_RSP, sreg);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, 8, mode);
	/* Load the value to the fpu */
	x86_64_fld_regp_size(inst, X86_64_RSP, 8);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, 8);
	/* and move st(0) to the destination register */
	x86_64_fstp_regp_size(inst, X86_64_RSP, 8);
	x86_64_movsd_reg_regp(inst, dreg, X86_64_RSP);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 16, 8);
#endif
#endif
	return inst;
}

static unsigned char *
x86_64_roundd_reg_membase(unsigned char *inst, int dreg, int offset,
						  int scratch_reg, X86_64_ROUNDMODE mode)
{
#ifdef HAVE_RED_ZONE
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundsd_reg_membase(inst, dreg, X86_64_RBP, offset, mode);
#else
	/* Load the value to the fpu */
	x86_64_fld_membase_size(inst, X86_64_RBP, offset, 8);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, -8, mode);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, -8);
	/* and move st(0) to the destination register */
	x86_64_fstp_membase_size(inst, X86_64_RSP, -16, 8);
	x86_64_movsd_reg_membase(inst, dreg, X86_64_RSP, -16);
#endif
#else
#ifdef HAVE_X86_SSE_4_1
	x86_64_roundsd_reg_membase(inst, dreg, X86_64_RBP, offset, mode);
#else
	/* allocate space on the stack for two ints and one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
	/* Load the value to the fpu */
	x86_64_fld_membase_size(inst, X86_64_RBP, offset, 8);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, 8, mode);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, 8);
	/* and move st(0) to the destination register */
	x86_64_fstp_regp_size(inst, X86_64_RSP, 8);
	x86_64_movsd_reg_regp(inst, dreg, X86_64_RSP);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 16, 8);
#endif
#endif
	return inst;
}

/*
 * Round the value in St(0) to integer according to the rounding
 * mode specified.
 */
static unsigned char *
x86_64_roundnf(unsigned char *inst, int scratch_reg, X86_64_ROUNDMODE mode)
{
#ifdef HAVE_RED_ZONE
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, -8, mode);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, -8);
#else
	/* allocate space on the stack for two ints and one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, 0, mode);
	/* And round it to integer */
	x86_64_frndint(inst);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, 0);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
#endif
	return inst;
}

/*
 * Round the value in the fpu register st(0) to integer and
 * store the value in dreg. St(0) is popped from the fpu stack.
 */
static unsigned char *
x86_64_nfloat_to_int(unsigned char *inst, int dreg, int scratch_reg, int size)
{
#ifdef HAVE_RED_ZONE
#ifdef HAVE_X86_FISTTP
	/* convert float to int */
	x86_64_fisttp_membase_size(inst, X86_64_RSP, -8, 4);
	/* move result to the destination */
	x86_64_mov_reg_membase_size(inst, dreg, X86_64_RSP, -8, 4);
#else
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, -8, X86_ROUND_ZERO);
	/* And round the value in st(0) to integer and store it on the stack */
	x86_64_fistp_membase_size(inst, X86_64_RSP, -16, size);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, -8);
	/* and load the integer to the destination register */
	x86_64_mov_reg_membase_size(inst, dreg, X86_64_RSP, -16, size);
#endif
#else
#ifdef HAVE_X86_FISTTP
	/* allocate space on the stack for one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
	/* convert float to int */
	x86_64_fisttp_regp_size(inst, X86_64_RSP, 4);
	/* move result to the destination */
	x86_64_mov_reg_regp_size(inst, dreg, X86_64_RSP, 4);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
#else
	/* allocate space on the stack for 2 ints and one long value */
	x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
	/* Set the fpu round mode */
	inst = _x86_64_set_fpu_roundmode(inst, scratch_reg, 8, X86_ROUND_ZERO);
	/* And round the value in st(0) to integer and store it on the stack */
	x86_64_fistp_regp_size(inst, X86_64_RSP, size);
	/* restore the fpu control word */
	inst = _x86_64_restore_fpcw(inst, 8);
	/* and load the integer to the destination register */
	x86_64_mov_reg_regp_size(inst, dreg, X86_64_RSP, size);
	/* restore the stack pointer */
	x86_64_add_reg_imm_size(inst, X86_64_RSP, 16, 8);
#endif
#endif
	return inst;
}

/*
 * Call a function
 */
static unsigned char *
x86_64_call_code(unsigned char *inst, jit_nint func)
{
	jit_nint offset;

	x86_64_mov_reg_imm_size(inst, X86_64_RAX, 8, 4);
	offset = func - ((jit_nint)inst + 5);
	if(offset >= jit_min_int && offset <= jit_max_int)
	{
		/* We can use the immediate call */
		x86_64_call_imm(inst, offset);
	}
	else
	{
		/* We have to do a call via register */
		x86_64_mov_reg_imm_size(inst, X86_64_SCRATCH, func, 8);
		x86_64_call_reg(inst, X86_64_SCRATCH);
	}
	return inst;
}

/*
 * Jump to a function
 */
static unsigned char *
x86_64_jump_to_code(unsigned char *inst, jit_nint func)
{
	jit_nint offset;

	offset = func - ((jit_nint)inst + 5);
	if(offset >= jit_min_int && offset <= jit_max_int)
	{
		/* We can use the immediate call */
		x86_64_jmp_imm(inst, offset);
	}
	else
	{
		/* We have to do a call via register */
		x86_64_mov_reg_imm_size(inst, X86_64_SCRATCH, func, 8);
		x86_64_jmp_reg(inst, X86_64_SCRATCH);
	}
	return inst;
}

/*
 * Throw a builtin exception.
 */
static unsigned char *
throw_builtin(unsigned char *inst, jit_function_t func, int type)
{
	/* We need to update "catch_pc" if we have a "try" block */
	if(func->builder->setjmp_value != 0)
	{
		_jit_gen_fix_value(func->builder->setjmp_value);

		x86_64_lea_membase_size(inst, X86_64_RDI, X86_64_RIP, 0, 8);
		x86_64_mov_membase_reg_size(inst, X86_64_RBP,
					func->builder->setjmp_value->frame_offset
					+ jit_jmp_catch_pc_offset, X86_64_RDI, 8);
	}

	/* Push the exception type onto the stack */
	x86_64_mov_reg_imm_size(inst, X86_64_RDI, type, 4);

	/* Call the "jit_exception_builtin" function, which will never return */
	return x86_64_call_code(inst, (jit_nint)jit_exception_builtin);
}

/*
 * spill a register to it's place in the current stack frame.
 * The argument type must be in it's normalized form.
 */
static void
_spill_reg(unsigned char **inst_ptr, jit_type_t type,
		   jit_int reg, jit_int offset)
{
	unsigned char *inst = *inst_ptr;

	if(IS_GENERAL_REG(reg))
	{
		switch(type->kind)
		{
#if 0
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			{
				x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
											_jit_reg_info[reg].cpu_reg, 1);
			}
			break;

			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
			{
				x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
											_jit_reg_info[reg].cpu_reg, 2);
			}
			break;
#else
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
#endif
			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			case JIT_TYPE_FLOAT32:
			{
				x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
											_jit_reg_info[reg].cpu_reg, 4);
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			case JIT_TYPE_FLOAT64:
			{
				x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
											_jit_reg_info[reg].cpu_reg, 8);
			}
			break;

			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				jit_nuint size = jit_type_get_size(type);

				if(size == 1)
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
												_jit_reg_info[reg].cpu_reg, 1);
				}
				else if(size == 2)
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
												_jit_reg_info[reg].cpu_reg, 2);
				}
				else if(size <= 4)
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
												_jit_reg_info[reg].cpu_reg, 4);
				}
				else
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
												_jit_reg_info[reg].cpu_reg, 8);
				}
			}
		}
	}
	else if(IS_XMM_REG(reg))
	{
		switch(type->kind)
		{
			case JIT_TYPE_FLOAT32:
			{
				x86_64_movss_membase_reg(inst, X86_64_RBP, offset,
										 _jit_reg_info[reg].cpu_reg);
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				x86_64_movsd_membase_reg(inst, X86_64_RBP, offset,
										 _jit_reg_info[reg].cpu_reg);
			}
			break;

			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				jit_nuint size = jit_type_get_size(type);

				if(size <= 4)
				{
					x86_64_movss_membase_reg(inst, X86_64_RBP, offset,
											 _jit_reg_info[reg].cpu_reg);
				}
				else if(size <= 8)
				{
					x86_64_movsd_membase_reg(inst, X86_64_RBP, offset,
											 _jit_reg_info[reg].cpu_reg);
				}
				else
				{
					jit_nint alignment = jit_type_get_alignment(type);

					if((alignment & 0xf) == 0)
					{
						x86_64_movaps_membase_reg(inst, X86_64_RBP, offset,
												  _jit_reg_info[reg].cpu_reg);
					}
					else
					{
						x86_64_movups_membase_reg(inst, X86_64_RBP, offset,
												  _jit_reg_info[reg].cpu_reg);
					}
				}
			}
			break;
		}
	}
	else if(IS_FPU_REG(reg))
	{
		switch(type->kind)
		{
			case JIT_TYPE_FLOAT32:
			{
				x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 4);
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 8);
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				if(sizeof(jit_nfloat) == sizeof(jit_float64))
				{
					x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 8);
				}
				else
				{
					x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 10);
				}
			}
			break;
		}
	}

	/* Write the current instruction pointer back */
	*inst_ptr = inst;
}

void
_jit_gen_fix_value(jit_value_t value)
{
	if(!(value->has_frame_offset) && !(value->is_constant))
	{
		jit_nuint alignment = jit_type_get_alignment(value->type);
		jit_nint size =jit_type_get_size(value->type);
		jit_nint frame_size = value->block->func->builder->frame_size;

		/* Round the size to a multiple of the stack item size */
		size = (jit_nint)(ROUND_STACK(size));

		/* Add the size to the existing local items */
		frame_size += size;

		/* Align the new frame_size for the value */
		frame_size = (frame_size + (alignment - 1)) & ~(alignment - 1);

		value->block->func->builder->frame_size = frame_size;
		value->frame_offset = -frame_size;
		value->has_frame_offset = 1;
	}
}

void
_jit_gen_spill_global(jit_gencode_t gen, int reg, jit_value_t value)
{
	jit_cache_setup_output(16);
	if(value)
	{
		jit_type_t type = jit_type_normalize(value->type);

		_jit_gen_fix_value(value);

		_spill_reg(&inst, type, value->global_reg, value->frame_offset);
	}
	else
	{
		x86_64_push_reg_size(inst, _jit_reg_info[reg].cpu_reg, 8);
	}
	jit_cache_end_output();
}

void
_jit_gen_load_global(jit_gencode_t gen, int reg, jit_value_t value)
{
	jit_cache_setup_output(16);
	if(value)
	{
		x86_64_mov_reg_membase_size(inst,
			_jit_reg_info[value->global_reg].cpu_reg,
			X86_64_RBP, value->frame_offset, 8);
	}
	else
	{
		x86_64_pop_reg_size(inst, _jit_reg_info[reg].cpu_reg, 8);
	}
	jit_cache_end_output();
}

void
_jit_gen_spill_reg(jit_gencode_t gen, int reg,
				   int other_reg, jit_value_t value)
{
	jit_type_t type;

	/* Make sure that we have sufficient space */
	jit_cache_setup_output(16);

	/* If the value is associated with a global register, then copy to that */
	if(value->has_global_register)
	{
		reg = _jit_reg_info[reg].cpu_reg;
		other_reg = _jit_reg_info[value->global_reg].cpu_reg;
		x86_64_mov_reg_reg_size(inst, other_reg, reg, sizeof(void *));
		jit_cache_end_output();
		return;
	}

	/* Fix the value in place within the local variable frame */
	_jit_gen_fix_value(value);

	/* Get the normalized type */
	type = jit_type_normalize(value->type);

	/* and spill the register */
	_spill_reg(&inst, type, reg, value->frame_offset);

	/* End the code output process */
	jit_cache_end_output();
}

void
_jit_gen_free_reg(jit_gencode_t gen, int reg,
		  int other_reg, int value_used)
{
	/* We only need to take explicit action if we are freeing a
	   floating-point register whose value hasn't been used yet */
	if(!value_used && IS_FPU_REG(reg))
	{
		_jit_gen_check_space(gen, 2);
		x86_fstp(gen->ptr, reg - X86_64_REG_ST0);
	}
}

/*
 * Set a register value based on a condition code.
 */
static unsigned char *
setcc_reg(unsigned char *inst, int reg, int cond, int is_signed)
{
	/* Use a SETcc instruction if we have a basic register */
	x86_64_set_reg(inst, cond, reg, is_signed);
	x86_64_movzx8_reg_reg_size(inst, reg, reg, 4);
	return inst;
}

/*
 * Helper macros for fixup handling.
 *
 * We have only 4 bytes for the jump offsets.
 * Therefore we have do something tricky here.
 * The fixup pointer in the block/gen points to the last fixup.
 * The fixup itself contains the offset to the previous fixup or
 * null if it's the last fixup in the list.
 */

/*
 * Calculate the fixup value
 * This is the value stored as placeholder in the instruction.
 */
#define _JIT_CALC_FIXUP(fixup_list, inst) \
	((jit_int)((jit_nint)(inst) - (jit_nint)(fixup_list)))

/*
 * Calculate the pointer to the fixup value.
 */
#define _JIT_CALC_NEXT_FIXUP(fixup_list, fixup) \
	((fixup) ? ((jit_nint)(fixup_list) - (jit_nint)(fixup)) : (jit_nint)0)

/*
 * Get the long form of a branch opcode.
 */
static int
long_form_branch(int opcode)
{
	if(opcode == 0xEB)
	{
		return 0xE9;
	}
	else
	{
		return opcode + 0x0F10;
	}
}

/*
 * Output a branch instruction.
 */
static unsigned char *
output_branch(jit_function_t func, unsigned char *inst, int opcode,
	      jit_insn_t insn)
{
	jit_block_t block;

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
		return inst;
	}
	if(block->address)
	{
		jit_nint offset;

		/* We already know the address of the block */
		offset = ((unsigned char *)(block->address)) - (inst + 2);
		if(x86_is_imm8(offset))
		{
			/* We can output a short-form backwards branch */
			*inst++ = (unsigned char)opcode;
			*inst++ = (unsigned char)offset;
		}
		else
		{
			/* We need to output a long-form backwards branch */
			offset -= 3;
			opcode = long_form_branch(opcode);
			if(opcode < 256)
			{
				*inst++ = (unsigned char)opcode;
			}
			else
			{
				*inst++ = (unsigned char)(opcode >> 8);
				*inst++ = (unsigned char)opcode;
				--offset;
			}
			x86_imm_emit32(inst, offset);
		}
	}
	else
	{
		jit_int fixup;

		/* Output a placeholder and record on the block's fixup list */
		opcode = long_form_branch(opcode);
		if(opcode < 256)
		{
			*inst++ = (unsigned char)opcode;
		}
		else
		{
			*inst++ = (unsigned char)(opcode >> 8);
			*inst++ = (unsigned char)opcode;
		}
		if(block->fixup_list)
		{
			fixup = _JIT_CALC_FIXUP(block->fixup_list, inst);
		}
		else
		{
			fixup = 0;
		}
		block->fixup_list = (void *)inst;
		x86_imm_emit32(inst, fixup);

		if(DEBUG_FIXUPS)
		{
			fprintf(stderr,
					"Block: %lx, Current Fixup: %lx, Next fixup: %lx\n",
					(jit_nint)block, (jit_nint)(block->fixup_list),
					(jit_nint)fixup);
		}
	}
	return inst;
}

/*
 * Jump to the current function's epilog.
 */
static unsigned char *
jump_to_epilog(jit_gencode_t gen, unsigned char *inst, jit_block_t block)
{
	jit_int fixup;

	/* If the epilog is the next thing that we will output,
	   then fall through to the epilog directly */
	if(_jit_block_is_final(block))
	{
		return inst;
	}

	/* Output a placeholder for the jump and add it to the fixup list */
	*inst++ = (unsigned char)0xE9;
	if(gen->epilog_fixup)
	{
		fixup = _JIT_CALC_FIXUP(gen->epilog_fixup, inst);
	}
	else
	{
		fixup = 0;
	}
	gen->epilog_fixup = (void *)inst;
	x86_imm_emit32(inst, fixup);
	return inst;
}

/*
 * fixup a register being alloca'd to by accounting for the param area
 */
static unsigned char *
fixup_alloca(jit_gencode_t gen, unsigned char *inst, int reg)
{
#ifdef JIT_USE_PARAM_AREA
	jit_int fixup;
	jit_int temp;

	/*
	 * emit the instruction and then replace the imm section of op with
	 * the fixup.
	 * NOTE: We are using the temp variable here to avoid a compiler
	 * warning and the temp value to make sure that an instruction with
	 * a 32 bit immediate is emitted. The temp value in the instruction
	 * will be replaced by the fixup
	 */
	temp = 1234567;
	x86_64_add_reg_imm_size(inst, reg, temp, 8);

	/* Make inst pointing to the 32bit immediate in the instruction */
	inst -= 4;

	/* calculalte the fixup */
	if (gen->alloca_fixup)
	{
		fixup = _JIT_CALC_FIXUP(gen->alloca_fixup, inst);
	}
	else
	{
		fixup = 0;
	}
	gen->alloca_fixup = (void *)inst;
	x86_imm_emit32(inst, fixup);
#else /* !JIT_USE_PARAM_AREA */
	/* alloca fixup is not needed if the param area is not used */
#endif /* JIT_USE_PARAM_AREA */
	return inst;
}

/*
 * Compare a xmm register with an immediate value.
 */
static unsigned char *
xmm_cmp_reg_imm(jit_gencode_t gen, unsigned char *inst, int xreg, void *imm,
		int is_double)
{
	int inst_len = 7 + (is_double ? 1 : 0) + (xreg > 7 ? 1 : 0);
	void *ptr;
	jit_nint offset;

	if(is_double)
	{
		ptr = _jit_gen_alloc(gen, sizeof(jit_float64));
		if(!ptr)
		{
			return 0;
		}
		jit_memcpy(ptr, imm, sizeof(jit_float64));
	}
	else
	{
		ptr = _jit_gen_alloc(gen, sizeof(jit_float32));
		if(!ptr)
		{
			return 0;
		}
		jit_memcpy(ptr, imm, sizeof(jit_float32));
	}
	offset = (jit_nint)ptr - ((jit_nint)inst + inst_len);
	if((offset >= jit_min_int) && (offset <= jit_max_int))
	{
		/* We can use RIP relative addressing here */
		if(is_double)
		{
			x86_64_ucomisd_reg_membase(inst, xreg, X86_64_RIP, offset);
		}
		else
		{
			x86_64_ucomiss_reg_membase(inst, xreg, X86_64_RIP, offset);
		}
	}
	else if(((jit_nint)ptr >= jit_min_int) &&
		((jit_nint)ptr <= jit_max_int))
	{
		/* We can use absolute addressing */
		if(is_double)
		{
			x86_64_ucomisd_reg_mem(inst, xreg, (jit_nint)ptr);
		}
		else
		{
			x86_64_ucomiss_reg_mem(inst, xreg, (jit_nint)ptr);
		}
	}
	else
	{
		/* We have to use an extra general register */
		TODO();
		return 0;
	}
	return inst;
}

/*
 * Compare two scalar float or double values and set dreg depending on the
 * flags set.
 * The result for nan values depends on nan_result.
 * If nan_result is == 0 then the result is 0 if any nan value is involved,
 * otherwise the result is true.
 */
static unsigned char *
xmm_setcc(unsigned char *inst, int dreg, int cond, int sreg, int nan_result)
{
	x86_64_set_reg(inst, cond, dreg, 0);
	if(nan_result)
	{
		/*
		 * Check pf only for comparisions where a flag is checked
		 * for 0 because an unordered result sets all flags.
		 * The cases where the additional check is not needed is
		 * eq, lt and le.
		 */
		if((cond != 0) && (cond != 2) && (cond != 3))
		{
			x86_64_set_reg(inst, 8 /* p */ , sreg, 0);
			x86_64_or_reg_reg_size(inst, dreg, sreg, 4);
		}
	}
	else
	{
		/*
		 * Check pf only for comparisions where a flag is checked
		 * for 1 because an unordered result sets all flags.
		 * The cases where the additional check is not needed is
		 * ne, gt and ge.
		 */
		if((cond != 1) && (cond != 4) && (cond != 5))
		{
			x86_64_set_reg(inst, 9 /* np */ , sreg, 0);
			x86_64_and_reg_reg_size(inst, dreg, sreg, 4);
		}
	}
	x86_64_movzx8_reg_reg_size(inst, dreg, dreg, 4);
	return inst;
}

static unsigned char *
xmm_cmp_setcc_reg_imm(jit_gencode_t gen, unsigned char *inst, int dreg,
		      int cond, int xreg, void *imm, int sreg, int is_double,
		      int nan_result)
{
	inst = xmm_cmp_reg_imm(gen, inst, xreg, imm, is_double);
	return xmm_setcc(inst, dreg, cond, sreg, nan_result);
}

static unsigned char *
xmm_cmp_setcc_reg_reg(unsigned char *inst, int dreg, int cond, int xreg1,
		      int xreg2, int sreg, int is_double, int nan_result)
{
	if(is_double)
	{
		x86_64_ucomisd_reg_reg(inst, xreg1, xreg2);
	}
	else
	{
		x86_64_ucomiss_reg_reg(inst, xreg1, xreg2);
	}
	return xmm_setcc(inst, dreg, cond, sreg, nan_result);
}

/*
 * Compare two float values and branch depending on the flags.
 */
static unsigned char *
xmm_brcc(jit_function_t func, unsigned char *inst, int cond, int nan_result,
	 jit_insn_t insn)
{
	if(nan_result)
	{
		/*
		 * Check pf only for comparisions where a flag is checked
		 * for 0 because an unordered result sets all flags.
		 * The cases where the additional check is not needed is
		 * eq, lt and le.
		 */
		if((cond != 0) && (cond != 2) && (cond != 3))
		{
			/* Branch if the parity flag is set */
			inst = output_branch(func, inst,
					     x86_cc_unsigned_map[8], insn);
		}
		inst = output_branch(func, inst, x86_cc_unsigned_map[cond], insn);
	}
	else
	{
		/*
		 * Check pf only for comparisions where a flag is checked
		 * for 1 because an unordered result sets all flags.
		 * The cases where the additional check is not needed is
		 * ne, gt and ge.
		 */
		if((cond != 1) && (cond != 4) && (cond != 5))
		{
			unsigned char *patch;
			patch = inst;
			x86_branch8(inst, X86_CC_P, 0, 0);
			inst = output_branch(func, inst,
					     x86_cc_unsigned_map[cond], insn);
			x86_patch(patch, inst);
		}
		else
		{
			inst = output_branch(func, inst,
					     x86_cc_unsigned_map[cond], insn);
		}
	}
	return inst;
}

static unsigned char *
xmm_cmp_brcc_reg_imm(jit_gencode_t gen, jit_function_t func,
		     unsigned char *inst, int cond, int xreg, void *imm,
		     int is_double, int nan_result, jit_insn_t insn)
{
	inst = xmm_cmp_reg_imm(gen, inst, xreg, imm, is_double);
	return xmm_brcc(func, inst, cond, nan_result, insn);
}

static unsigned char *
xmm_cmp_brcc_reg_reg(jit_function_t func, unsigned char *inst, int cond,
		     int xreg1, int xreg2, int is_double, int nan_result,
		     jit_insn_t insn)
{
	if(is_double)
	{
		x86_64_ucomisd_reg_reg(inst, xreg1, xreg2);
	}
	else
	{
		x86_64_ucomiss_reg_reg(inst, xreg1, xreg2);
	}
	return xmm_brcc(func, inst, cond, nan_result, insn);
}

static unsigned char *
xmm_cmp_brcc_reg_membase(jit_function_t func, unsigned char *inst, int cond,
			 int xreg1, int basereg, int offset, int is_double,
			 int nan_result, jit_insn_t insn)
{
	if(is_double)
	{
		x86_64_ucomisd_reg_membase(inst, xreg1, basereg, offset);
	}
	else
	{
		x86_64_ucomiss_reg_membase(inst, xreg1, basereg, offset);
	}
	return xmm_brcc(func, inst, cond, nan_result, insn);
}

/*
 * Support functiond for the FPU stack
 */

static int
fp_stack_index(jit_gencode_t gen, int reg)
{
	return gen->reg_stack_top - reg - 1;
}

void
_jit_gen_exch_top(jit_gencode_t gen, int reg)
{
	if(IS_FPU_REG(reg))
	{
		jit_cache_setup_output(2);
		x86_fxch(inst, fp_stack_index(gen, reg));
		jit_cache_end_output();
	}
}

void
 _jit_gen_move_top(jit_gencode_t gen, int reg)
{
	if(IS_FPU_REG(reg))
	{
		jit_cache_setup_output(2);
		x86_fstp(inst, fp_stack_index(gen, reg));
		jit_cache_end_output();
	}
}

void
_jit_gen_spill_top(jit_gencode_t gen, int reg, jit_value_t value, int pop)
{
	if(IS_FPU_REG(reg))
	{
		int offset;

		/* Make sure that we have sufficient space */
		jit_cache_setup_output(16);

		/* Fix the value in place within the local variable frame */
		_jit_gen_fix_value(value);

		/* Output an appropriate instruction to spill the value */
		offset = (int)(value->frame_offset);

		/* Spill the top of the floating-point register stack */
		switch(jit_type_normalize(value->type)->kind)
		{
			case JIT_TYPE_FLOAT32:
			{
				if(pop)
				{
					x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 4);
				}
				else
				{
					x86_64_fst_membase_size(inst, X86_64_RBP, offset, 4);
				}
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				if(pop)
				{
					x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 8);
				}
				else
				{
					x86_64_fst_membase_size(inst, X86_64_RBP, offset, 8);
				}
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				if(sizeof(jit_nfloat) == sizeof(jit_float64))
				{
					if(pop)
					{
						x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 8);
					}
					else
					{
						x86_64_fst_membase_size(inst, X86_64_RBP, offset, 8);
					}
				}
				else
				{
					x86_64_fstp_membase_size(inst, X86_64_RBP, offset, 10);
					if(!pop)
					{
						x86_64_fld_membase_size(inst, X86_64_RBP, offset, 10);
					}
				}
			}
			break;
		}

		/* End the code output process */
		jit_cache_end_output();
	}
}

void
_jit_gen_load_value(jit_gencode_t gen, int reg, int other_reg, jit_value_t value)
{
	jit_type_t type;
	int src_reg;
	void *ptr;
	int offset;

	/* Make sure that we have sufficient space */
	jit_cache_setup_output(16);

	type = jit_type_normalize(value->type);

	/* Load zero */
	if(value->is_constant)
	{
		switch(type->kind)
		{
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				if((jit_nint)(value->address) == 0)
				{
					x86_64_clear_reg(inst, _jit_reg_info[reg].cpu_reg);
				}
				else
				{
					x86_64_mov_reg_imm_size(inst, _jit_reg_info[reg].cpu_reg,
							(jit_nint)(value->address), 4);
				}
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			{
				if((jit_nint)(value->address) == 0)
				{
					x86_64_clear_reg(inst, _jit_reg_info[reg].cpu_reg);
				}
				else
				{
					if((jit_nint)(value->address) > 0 && (jit_nint)(value->address) <= (jit_nint)jit_max_uint)
					{
						x86_64_mov_reg_imm_size(inst, _jit_reg_info[reg].cpu_reg,
								(jit_nint)(value->address), 4);

					}
					else
					{
						x86_64_mov_reg_imm_size(inst, _jit_reg_info[reg].cpu_reg,
								(jit_nint)(value->address), 8);
					}
				}
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				jit_float32 float32_value;

				float32_value = jit_value_get_float32_constant(value);

				if(IS_GENERAL_REG(reg))
				{
					union
					{
						jit_float32 float32_value;
						jit_int int_value;
					} un;

					un.float32_value = float32_value;
					x86_64_mov_reg_imm_size(inst, _jit_reg_info[reg].cpu_reg,
											un.int_value, 4);
				}
				else if(IS_XMM_REG(reg))
				{
					int xmm_reg = _jit_reg_info[reg].cpu_reg;

					if(float32_value == (jit_float32) 0.0)
					{
						x86_64_clear_xreg(inst, xmm_reg);
					}
					else
					{
						_jit_xmm1_reg_imm_size_float32(gen, &inst, XMM1_MOV,
													   xmm_reg, &float32_value);
					}
				}
				else
				{
					if(float32_value == (jit_float32) 0.0)
					{
						x86_fldz(inst);
					}
					else if(float32_value == (jit_float32) 1.0)
					{
						x86_fld1(inst);
					}
					else
					{
						jit_nint offset;

						ptr = _jit_gen_alloc(gen, sizeof(jit_float32));
						jit_memcpy(ptr, &float32_value, sizeof(float32_value));

						offset = (jit_nint)ptr - ((jit_nint)inst + 6);
						if((offset >= jit_min_int) && (offset <= jit_max_int))
						{
							/* We can use RIP relative addressing here */
							x86_64_fld_membase_size(inst, X86_64_RIP, offset, 4);
						}
						else if(((jit_nint)ptr >= jit_min_int) &&
								((jit_nint)ptr <= jit_max_int))
						{
							/* We can use absolute addressing */
							x86_64_fld_mem_size(inst, (jit_nint)ptr, 4);
						}
						else
						{
							/* We have to use an extra general register */
							TODO();
						}
					}
				}
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				jit_float64 float64_value;
				float64_value = jit_value_get_float64_constant(value);
				if(IS_GENERAL_REG(reg))
				{
					union
					{
						jit_float64 float64_value;
						jit_long long_value;
					} un;

					un.float64_value = float64_value;
					x86_64_mov_reg_imm_size(inst, _jit_reg_info[reg].cpu_reg,
											un.long_value, 8);
				}
				else if(IS_XMM_REG(reg))
				{
					int xmm_reg = _jit_reg_info[reg].cpu_reg;

					if(float64_value == (jit_float64) 0.0)
					{
						x86_64_clear_xreg(inst, xmm_reg);
					}
					else
					{
						_jit_xmm1_reg_imm_size_float64(gen, &inst, XMM1_MOV,
													   xmm_reg, &float64_value);
					}
				}
				else
				{
					if(float64_value == (jit_float64) 0.0)
					{
						x86_fldz(inst);
					}
					else if(float64_value == (jit_float64) 1.0)
					{
						x86_fld1(inst);
					}
					else
					{
						jit_nint offset;

						ptr = _jit_gen_alloc(gen, sizeof(jit_float64));
						jit_memcpy(ptr, &float64_value, sizeof(float64_value));

						offset = (jit_nint)ptr - ((jit_nint)inst + 6);
						if((offset >= jit_min_int) && (offset <= jit_max_int))
						{
							/* We can use RIP relative addressing here */
							x86_64_fld_membase_size(inst, X86_64_RIP, offset, 8);
						}
						else if(((jit_nint)ptr >= jit_min_int) &&
								((jit_nint)ptr <= jit_max_int))
						{
							/* We can use absolute addressing */
							x86_64_fld_mem_size(inst, (jit_nint)ptr, 8);
						}
						else
						{
							/* We have to use an extra general register */
							TODO();
						}
					}
				}
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				jit_nfloat nfloat_value;
				nfloat_value = jit_value_get_nfloat_constant(value);
				if(IS_GENERAL_REG(reg) && sizeof(jit_nfloat) == sizeof(jit_float64))
				{
					union
					{
						jit_nfloat nfloat_value;
						jit_long long_value;
					} un;

					un.nfloat_value = nfloat_value;
					x86_64_mov_reg_imm_size(inst, _jit_reg_info[reg].cpu_reg,
											un.long_value, 8);
				}
				else if(IS_XMM_REG(reg) && sizeof(jit_nfloat) == sizeof(jit_float64))
				{
					jit_nint offset;
					int xmm_reg = _jit_reg_info[reg].cpu_reg;

					ptr = _jit_gen_alloc(gen, sizeof(jit_nfloat));
					jit_memcpy(ptr, &nfloat_value, sizeof(nfloat_value));
					offset = (jit_nint)ptr -
								((jit_nint)inst + (xmm_reg > 7 ? 9 : 8));
					if((offset >= jit_min_int) && (offset <= jit_max_int))
					{
						/* We can use RIP relative addressing here */
						x86_64_movsd_reg_membase(inst, xmm_reg, X86_64_RIP, offset);
					}
					else if(((jit_nint)ptr >= jit_min_int) &&
							((jit_nint)ptr <= jit_max_int))
					{
						/* We can use absolute addressing */
						x86_64_movsd_reg_mem(inst, xmm_reg, (jit_nint)ptr);
					}
					else
					{
						/* We have to use an extra general register */
						TODO();
					}
				}
				else
				{
					if(nfloat_value == (jit_nfloat) 0.0)
					{
						x86_fldz(inst);
					}
					else if(nfloat_value == (jit_nfloat) 1.0)
					{
						x86_fld1(inst);
					}
					else
					{
						jit_nint offset;

						ptr = _jit_gen_alloc(gen, sizeof(jit_nfloat));
						jit_memcpy(ptr, &nfloat_value, sizeof(nfloat_value));

						offset = (jit_nint)ptr - ((jit_nint)inst + 6);
						if((offset >= jit_min_int) && (offset <= jit_max_int))
						{
							/* We can use RIP relative addressing here */
							if(sizeof(jit_nfloat) == sizeof(jit_float64))
							{
								x86_64_fld_membase_size(inst, X86_64_RIP, offset, 8);
							}
							else
							{
								x86_64_fld_membase_size(inst, X86_64_RIP, offset, 10);
							}
						}
						else if(((jit_nint)ptr >= jit_min_int) &&
								((jit_nint)ptr <= jit_max_int))
						{
							/* We can use absolute addressing */
							if(sizeof(jit_nfloat) == sizeof(jit_float64))
							{
								x86_64_fld_mem_size(inst, (jit_nint)ptr, 8);
							}
							else
							{
								x86_64_fld_mem_size(inst, (jit_nint)ptr, 10);
							}
						}
						else
						{
							/* We have to use an extra general register */
							TODO();
						}
					}
				}
			}
			break;
		}
	}
	else if(value->in_register || value->in_global_register)
	{
		if(value->in_register)
		{
			src_reg = value->reg;
		}
		else
		{
			src_reg = value->global_reg;
		}

		switch(type->kind)
		{
#if 0
			case JIT_TYPE_SBYTE:
			{
				x86_widen_reg(inst, _jit_reg_info[reg].cpu_reg,
					      _jit_reg_info[src_reg].cpu_reg, 1, 0);
			}
			break;

			case JIT_TYPE_UBYTE:
			{
				x86_widen_reg(inst, _jit_reg_info[reg].cpu_reg,
					      _jit_reg_info[src_reg].cpu_reg, 0, 0);
			}
			break;

			case JIT_TYPE_SHORT:
			{
				x86_widen_reg(inst, _jit_reg_info[reg].cpu_reg,
					      _jit_reg_info[src_reg].cpu_reg, 1, 1);
			}
			break;

			case JIT_TYPE_USHORT:
			{
				x86_widen_reg(inst, _jit_reg_info[reg].cpu_reg,
					      _jit_reg_info[src_reg].cpu_reg, 0, 1);
			}
			break;
#else
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
#endif
			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				x86_64_mov_reg_reg_size(inst, _jit_reg_info[reg].cpu_reg,
										_jit_reg_info[src_reg].cpu_reg, 4);
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			{
				x86_64_mov_reg_reg_size(inst, _jit_reg_info[reg].cpu_reg,
										_jit_reg_info[src_reg].cpu_reg, 8);
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				if(IS_FPU_REG(reg))
				{
					if(IS_FPU_REG(src_reg))
					{
						x86_fld_reg(inst, fp_stack_index(gen, src_reg));
					}
					else if(IS_XMM_REG(src_reg))
					{
						/* Fix the position of the value in the stack frame */
						_jit_gen_fix_value(value);
						offset = (int)(value->frame_offset);

						x86_64_movss_membase_reg(inst, X86_64_RBP, offset,
												 _jit_reg_info[src_reg].cpu_reg);
						x86_64_fld_membase_size(inst, X86_64_RBP, offset, 4);
					}
				}
				else if(IS_XMM_REG(reg))
				{
					if(IS_FPU_REG(src_reg))
					{
						/* Fix the position of the value in the stack frame */
						_jit_gen_fix_value(value);
						offset = (int)(value->frame_offset);

						x86_64_fst_membase_size(inst, X86_64_RBP, offset, 4);
						x86_64_movss_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
												 X86_64_RBP, offset);
					}
					else if(IS_XMM_REG(src_reg))
					{
						x86_64_movss_reg_reg(inst, _jit_reg_info[reg].cpu_reg,
											 _jit_reg_info[src_reg].cpu_reg);
					}
				}
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				if(IS_FPU_REG(reg))
				{
					if(IS_FPU_REG(src_reg))
					{
						x86_fld_reg(inst, fp_stack_index(gen, src_reg));
					}
					else if(IS_XMM_REG(src_reg))
					{
						/* Fix the position of the value in the stack frame */
						_jit_gen_fix_value(value);
						offset = (int)(value->frame_offset);

						x86_64_movsd_membase_reg(inst, X86_64_RBP, offset,
												 _jit_reg_info[src_reg].cpu_reg);
						x86_64_fld_membase_size(inst, X86_64_RBP, offset, 8);
					}
				}
				else if(IS_XMM_REG(reg))
				{
					if(IS_FPU_REG(src_reg))
					{
						/* Fix the position of the value in the stack frame */
						_jit_gen_fix_value(value);
						offset = (int)(value->frame_offset);

						x86_64_fst_membase_size(inst, X86_64_RBP, offset, 8);
						x86_64_movsd_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
												 X86_64_RBP, offset);
					}
					else if(IS_XMM_REG(src_reg))
					{
						x86_64_movsd_reg_reg(inst, _jit_reg_info[reg].cpu_reg,
											 _jit_reg_info[src_reg].cpu_reg);
					}
				}
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				if(IS_FPU_REG(reg))
				{
					if(IS_FPU_REG(src_reg))
					{
						x86_fld_reg(inst, fp_stack_index(gen, src_reg));
					}
					else
					{
						fputs("Unsupported native float reg - reg move\n", stderr);
					}
				}
			}
			break;

			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				if(IS_GENERAL_REG(reg))
				{
					if(IS_GENERAL_REG(src_reg))
					{
						x86_64_mov_reg_reg_size(inst, _jit_reg_info[reg].cpu_reg,
												_jit_reg_info[src_reg].cpu_reg, 8);
					}
					else if(IS_XMM_REG(src_reg))
					{
						x86_64_movq_reg_xreg(inst, _jit_reg_info[reg].cpu_reg,
											 _jit_reg_info[src_reg].cpu_reg);
					}
					else
					{
						fputs("Unsupported struct/union reg - reg move\n", stderr);
					}
				}
				else if(IS_XMM_REG(reg))
				{
					if(IS_GENERAL_REG(src_reg))
					{
						x86_64_movq_xreg_reg(inst, _jit_reg_info[reg].cpu_reg,
											 _jit_reg_info[src_reg].cpu_reg);
					}
					else if(IS_XMM_REG(src_reg))
					{
						x86_64_movaps_reg_reg(inst, _jit_reg_info[reg].cpu_reg,
											  _jit_reg_info[src_reg].cpu_reg);
					}
					else
					{
						fputs("Unsupported struct/union reg - reg move\n", stderr);
					}
				}
				else
				{
					fputs("Unsupported struct/union reg - reg move\n", stderr);
				}
			}
		}
	}
	else
	{
		/* Fix the position of the value in the stack frame */
		_jit_gen_fix_value(value);
		offset = (int)(value->frame_offset);

		/* Load the value into the specified register */
		switch(type->kind)
		{
			case JIT_TYPE_SBYTE:
			{
				x86_64_movsx8_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
											   X86_64_RBP, offset, 4);
			}
			break;

			case JIT_TYPE_UBYTE:
			{
				x86_64_movzx8_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
											   X86_64_RBP, offset, 4);
			}
			break;

			case JIT_TYPE_SHORT:
			{
				x86_64_movsx16_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
												X86_64_RBP, offset, 4);
			}
			break;

			case JIT_TYPE_USHORT:
			{
				x86_64_movzx16_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
												X86_64_RBP, offset, 4);
			}
			break;

			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
											X86_64_RBP, offset, 4);
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
			{
				x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
											X86_64_RBP, offset, 8);
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				if(IS_GENERAL_REG(reg))
				{
					x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
												X86_64_RBP, offset, 4);
				}
				if(IS_XMM_REG(reg))
				{
					x86_64_movss_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
											 X86_64_RBP, offset);
				}
				else
				{
					x86_64_fld_membase_size(inst, X86_64_RBP, offset, 4);
				}
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				if(IS_GENERAL_REG(reg))
				{
					x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
												X86_64_RBP, offset, 8);
				}
				else if(IS_XMM_REG(reg))
				{
					x86_64_movsd_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
											 X86_64_RBP, offset);
				}
				else
				{
					x86_64_fld_membase_size(inst, X86_64_RBP, offset, 8);
				}
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				if(sizeof(jit_nfloat) == sizeof(jit_float64))
				{
					if(IS_GENERAL_REG(reg))
					{
						x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
													X86_64_RBP, offset, 8);
					}
					else if(IS_XMM_REG(reg))
					{
						x86_64_movsd_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
												 X86_64_RBP, offset);
					}
					else
					{
						x86_64_fld_membase_size(inst, X86_64_RBP, offset, 8);
					}
				}
				else
				{
					x86_64_fld_membase_size(inst, X86_64_RBP, offset, 10);
				}
			}
			break;

			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				jit_nuint size = jit_type_get_size(type);

				if(IS_GENERAL_REG(reg))
				{
					if(size == 1)
					{
						x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
													X86_64_RBP, offset, 1);
					}
					else if(size == 2)
					{
						x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
													X86_64_RBP, offset, 2);
					}
					else if(size <= 4)
					{
						x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
													X86_64_RBP, offset, 4);
					}
					else if(size <= 8)
					{
						x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
													X86_64_RBP, offset, 8);
					}
				}
				else if(IS_XMM_REG(reg))
				{
					if(size <= 4)
					{
						x86_64_movss_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
												 X86_64_RBP, offset);
					}
					else if(size <= 8)
					{
						x86_64_movsd_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
												 X86_64_RBP, offset);
					}
					else
					{
						int alignment = jit_type_get_alignment(type);

						if((alignment & 0xf) == 0)
						{
							x86_64_movaps_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
													  X86_64_RBP, offset);
						}
						else
						{
							x86_64_movups_reg_membase(inst, _jit_reg_info[reg].cpu_reg,
													  X86_64_RBP, offset);
						}
					}
				}
			}
		}
	}

	/* End the code output process */
	jit_cache_end_output();
}

void
_jit_gen_get_elf_info(jit_elf_info_t *info)
{
	info->machine = 62;	/* EM_X86_64 */
	info->abi = 0;		/* ELFOSABI_SYSV */
	info->abi_version = 0;
}

void *
_jit_gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf)
{
	unsigned char prolog[JIT_PROLOG_SIZE];
	unsigned char *inst = prolog;
	int reg;
	int frame_size = 0;
	int regs_to_save = 0;

	/* Push ebp onto the stack */
	x86_64_push_reg_size(inst, X86_64_RBP, 8);

	/* Initialize EBP for the current frame */
	x86_64_mov_reg_reg_size(inst, X86_64_RBP, X86_64_RSP, 8);

	/* Allocate space for the local variable frame */
	if(func->builder->frame_size > 0)
	{
		/* Make sure that the framesize is a multiple of 8 bytes */
		frame_size = (func->builder->frame_size + 0x7) & ~0x7;
	}

	/* Get the number of registers we need to preserve */
	for(reg = 0; reg < 14; ++reg)
	{
		if(jit_reg_is_used(gen->touched, reg) &&
		   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
		{
			++regs_to_save;
		}
	}

	/* add the register save area to the initial frame size */
	frame_size += (regs_to_save << 3);

#ifdef JIT_USE_PARAM_AREA
	/* Add the param area to the frame_size if the additional offset
	   doesnt cause the offsets in the register saves become 4 bytes */
	if(func->builder->param_area_size > 0 &&
	   (func->builder->param_area_size <= 0x50 || regs_to_save == 0))
	{
		frame_size += func->builder->param_area_size;
	}
#endif /* JIT_USE_PARAM_AREA */

	/* Make sure that the framesize is a multiple of 16 bytes */
	/* so that the final RSP will be alligned on a 16byte boundary. */
	frame_size = (frame_size + 0xf) & ~0xf;

	if(frame_size > 0)
	{
		x86_64_sub_reg_imm_size(inst, X86_64_RSP, frame_size, 8);
	}

	if(regs_to_save > 0)
	{
		int current_offset;
#ifdef JIT_USE_PARAM_AREA
		if(func->builder->param_area_size > 0 &&
		   func->builder->param_area_size <= 0x50)
		{
			current_offset = func->builder->param_area_size;
		}
		else
#endif /* JIT_USE_PARAM_AREA */
		{
			current_offset = 0;
		}

		/* Save registers that we need to preserve */
		for(reg = 0; reg <= 14; ++reg)
		{
			if(jit_reg_is_used(gen->touched, reg) &&
			   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
			{
				x86_64_mov_membase_reg_size(inst, X86_64_RSP, current_offset,
											_jit_reg_info[reg].cpu_reg, 8);
				current_offset += 8;
			}
		}
	}
#ifdef JIT_USE_PARAM_AREA
	if(func->builder->param_area_size > 0x50 && regs_to_save > 0)
	{
		x86_64_sub_reg_imm_size(inst, X86_64_RSP, func->builder->param_area_size, 8);
	}
#endif /* JIT_USE_PARAM_AREA */

	/* Copy the prolog into place and return the adjusted entry position */
	reg = (int)(inst - prolog);
	jit_memcpy(((unsigned char *)buf) + JIT_PROLOG_SIZE - reg, prolog, reg);
	return (void *)(((unsigned char *)buf) + JIT_PROLOG_SIZE - reg);
}

void
_jit_gen_epilog(jit_gencode_t gen, jit_function_t func)
{
	unsigned char *inst;
	int reg;
	int current_offset;
	jit_int *fixup;
	jit_int *next;

	/* Bail out if there is insufficient space for the epilog */
	_jit_gen_check_space(gen, 48);

	inst = gen->ptr;

	/* Perform fixups on any blocks that jump to the epilog */
	fixup = (jit_int *)(gen->epilog_fixup);
	while(fixup != 0)
	{
		if(DEBUG_FIXUPS)
		{
			fprintf(stderr, "Fixup Address: %lx, Value: %x\n",
					(jit_nint)fixup, fixup[0]);
		}
		next = (jit_int *)_JIT_CALC_NEXT_FIXUP(fixup, fixup[0]);
		fixup[0] = (jit_int)(((jit_nint)inst) - ((jit_nint)fixup) - 4);
		fixup = next;
	}
	gen->epilog_fixup = 0;

	/* Perform fixups on any alloca calls */
	fixup = (jit_int *)(gen->alloca_fixup);
	while (fixup != 0)
	{
		next = (jit_int *)_JIT_CALC_NEXT_FIXUP(fixup, fixup[0]);
		fixup[0] = func->builder->param_area_size;
		if(DEBUG_FIXUPS)
		{
			fprintf(stderr, "Fixup Param Area Size: %lx, Value: %x\n",
					(jit_nint)fixup, fixup[0]);
		}
		fixup = next;
	}
	gen->alloca_fixup = 0;

	/* Restore the used callee saved registers */
	if(gen->stack_changed)
	{
		int frame_size = func->builder->frame_size;
		int regs_saved = 0;

		/* Get the number of registers we preserves */
		for(reg = 0; reg < 14; ++reg)
		{
			if(jit_reg_is_used(gen->touched, reg) &&
			   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
			{
				++regs_saved;
			}
		}

		/* add the register save area to the initial frame size */
		frame_size += (regs_saved << 3);

		/* Make sure that the framesize is a multiple of 16 bytes */
		/* so that the final RSP will be alligned on a 16byte boundary. */
		frame_size = (frame_size + 0xf) & ~0xf;

		current_offset = -frame_size;

		for(reg = 0; reg <= 14; ++reg)
		{
			if(jit_reg_is_used(gen->touched, reg) &&
			   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
			{
				x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
							    X86_64_RBP, current_offset, 8);
				current_offset += 8;
			}
		}
	}
	else
	{
#ifdef JIT_USE_PARAM_AREA
		if(func->builder->param_area_size > 0)
		{
			current_offset = func->builder->param_area_size;
		}
		else
		{
			current_offset = 0;
		}
#else /* !JIT_USE_PARAM_AREA */
		current_offset = 0;
#endif /* !JIT_USE_PARAM_AREA */
		for(reg = 0; reg <= 14; ++reg)
		{
			if(jit_reg_is_used(gen->touched, reg) &&
			   (_jit_reg_info[reg].flags & JIT_REG_CALL_USED) == 0)
			{
				x86_64_mov_reg_membase_size(inst, _jit_reg_info[reg].cpu_reg,
							    X86_64_RSP, current_offset, 8);
				current_offset += 8;
			}
		}
	}

	/* Restore stackpointer and frame register */
	x86_64_mov_reg_reg_size(inst, X86_64_RSP, X86_64_RBP, 8);
	x86_64_pop_reg_size(inst, X86_64_RBP, 8);

	/* and return */
	x86_64_ret(inst);

	gen->ptr = inst;
}

/*
 * Copy a small block. This generates inlined code.
 *
 * Set is_aligned to zero if the source or target locations might be not
 * aligned on a 16-byte boundary and to non-zero if both blocks are always
 * aligned.
 *
 * We assume that offset + size is in the range -2GB ... +2GB.
 */
static unsigned char *
small_block_copy(jit_gencode_t gen, unsigned char *inst,
				 int dreg, jit_nint doffset,
				 int sreg, jit_nint soffset, jit_int size,
				 int scratch_reg, int scratch_xreg, int is_aligned)
{
	jit_nint offset = 0;
	int i;

	/* Copy all 16 byte blocks of the struct */
	while(size >= 16)
	{
		if(is_aligned)
		{
			x86_64_movaps_reg_membase(inst, scratch_xreg,
									  sreg, soffset + offset);
			x86_64_movaps_membase_reg(inst, dreg, doffset + offset,
									  scratch_xreg);
		}
		else
		{
			x86_64_movups_reg_membase(inst, scratch_xreg,
									  sreg, soffset + offset);
			x86_64_movups_membase_reg(inst, dreg, doffset + offset,
									  scratch_xreg);
		}
		size -= 16;
		offset += 16;
	}

	/* Now copy the rest of the struct */
	for(i = 8; i > 0; i /= 2)
	{
		if(size >= i)
		{
			x86_64_mov_reg_membase_size(inst, scratch_reg, sreg,
										soffset + offset, i);
			x86_64_mov_membase_reg_size(inst, dreg, doffset + offset,
										scratch_reg, i);
			size -= i;
			offset += i;
		}
	}
	return inst;
}

/*
 * Copy a struct.
 * The size of the type must be <= 4 * 16bytes
 */
static unsigned char *
small_struct_copy(jit_gencode_t gen, unsigned char *inst,
				  int dreg, jit_nint doffset,
				  int sreg, jit_nint soffset, jit_type_t type,
				  int scratch_reg, int scratch_xreg)
{
	int size = jit_type_get_size(type);
	int alignment = jit_type_get_alignment(type);

	return small_block_copy(gen, inst, dreg, doffset,
							sreg, soffset, size, scratch_reg,
							scratch_xreg, ((alignment & 0xf) == 0));
}

/*
 * Copy a block of memory that has a specific size. All call clobbered
 * registers must be unused at this point.
 */
static unsigned char *
memory_copy(jit_gencode_t gen, unsigned char *inst,
			int dreg, jit_nint doffset,
			int sreg, jit_nint soffset, jit_nint size)
{
	if(dreg == X86_64_RDI)
	{
		if(sreg != X86_64_RSI)
		{
			x86_64_mov_reg_reg_size(inst, X86_64_RSI, sreg, 8);
		}
	}
	else if(dreg == X86_64_RSI)
	{
		if(sreg == X86_64_RDI)
		{
			/* The registers are swapped so we need a temporary register */
			x86_64_mov_reg_reg_size(inst, X86_64_RCX, X86_64_RSI, 8);
			x86_64_mov_reg_reg_size(inst, X86_64_RSI, X86_64_RDI, 8);
			x86_64_mov_reg_reg_size(inst, X86_64_RDI, X86_64_RCX, 8);
		}
		else
		{
			x86_64_mov_reg_reg_size(inst, X86_64_RDI, X86_64_RSI, 8);
			if(sreg != X86_64_RSI)
			{
				x86_64_mov_reg_reg_size(inst, X86_64_RSI, sreg, 8);
			}
		}
	}
	else
	{
		x86_64_mov_reg_reg_size(inst, X86_64_RSI, sreg, 8);
		x86_64_mov_reg_reg_size(inst, X86_64_RDI, dreg, 8);
	}
	/* Move the size to argument register 3 now */
	if((size > 0) && (size <= jit_max_uint))
	{
		x86_64_mov_reg_imm_size(inst, X86_64_RDX, size, 4);
	}
	else
	{
		x86_64_mov_reg_imm_size(inst, X86_64_RDX, size, 8);
	}
	if(soffset != 0)
	{
		x86_64_add_reg_imm_size(inst, X86_64_RSI, soffset, 8);
	}
	if(doffset != 0)
	{
		x86_64_add_reg_imm_size(inst, X86_64_RDI, doffset, 8);
	}
	inst = x86_64_call_code(inst, (jit_nint)jit_memcpy);
	return inst;
}

/*
 * Fill a small block. This generates inlined code.
 *
 * Set is_aligned to zero if the target location might be not aligned on a
 * 16-byte boundary and to non-zero if the block is always aligned.
 *
 * Set use_sse to zero to disable SSE instructions use (it will make this
 * function ignore scratch_xreg). Set it to non-zero otherwise.
 *
 * We assume that offset + size is in the range -2GB ... +2GB.
 */
static unsigned char *
small_block_set(jit_gencode_t gen, unsigned char *inst,
				int dreg, jit_nint doffset,
				jit_nuint val, jit_nint size,
				int scratch_reg, int scratch_xreg,
				int is_aligned, int use_sse)
{
	jit_nint offset = 0;
	int i;

	/* Make sure only the least significant byte serves as the filler. */
	val &= 0xff;

	/* Load the filler into a register. */
	if(val == 0)
	{
		if(!use_sse || (size % 16) != 0)
		{
			x86_64_clear_reg(inst, scratch_reg);
		}
	}
	else
	{
		val |= val << 8;
		val |= val << 16;
		val |= val << 32;
		x86_64_mov_reg_imm_size(inst, scratch_reg, val, 8);
	}

	/* Fill all 16 byte blocks */
	if(use_sse)
	{
		if(val == 0)
		{
			x86_64_clear_xreg(inst, scratch_xreg);
		}
		else
		{
			x86_64_movq_xreg_reg(inst, scratch_xreg, scratch_reg);
			x86_64_movlhps(inst, scratch_xreg, scratch_xreg);
		}

		while(size >= 16)
		{
			if(is_aligned)
			{
				x86_64_movaps_membase_reg(inst, dreg, doffset + offset,
										  scratch_xreg);
			}
			else
			{
				x86_64_movups_membase_reg(inst, dreg, doffset + offset,
										  scratch_xreg);
			}
			size -= 16;
			offset += 16;
		}
	}

	/* Now fill the rest */
	for(i = 8; i > 0; i /= 2)
	{
		while(size >= i)
		{
			x86_64_mov_membase_reg_size(inst, dreg, doffset + offset,
										scratch_reg, i);
			size -= i;
			offset += i;
		}
	}
	return inst;
}

void
_jit_gen_start_block(jit_gencode_t gen, jit_block_t block)
{
	jit_int *fixup;
	jit_int *next;
	void **absolute_fixup;
	void **absolute_next;

	/* Set the address of this block */
	block->address = (void *)(gen->ptr);

	/* If this block has pending fixups, then apply them now */
	fixup = (jit_int *)(block->fixup_list);
	if(DEBUG_FIXUPS && fixup)
	{
		fprintf(stderr, "Block: %lx\n", (jit_nint)block);
	}
	while(fixup != 0)
	{
		if(DEBUG_FIXUPS)
		{
			fprintf(stderr, "Fixup Address: %lx, Value: %x\n",
					(jit_nint)fixup, fixup[0]);
		}
		next = (jit_int *)_JIT_CALC_NEXT_FIXUP(fixup, fixup[0]);
		fixup[0] = (jit_int)
			(((jit_nint)(block->address)) - ((jit_nint)fixup) - 4);
		fixup = next;
	}
	block->fixup_list = 0;

	/* Absolute fixups contain complete pointers */
	absolute_fixup = (void**)(block->fixup_absolute_list);
	while(absolute_fixup != 0)
	{
		absolute_next = (void **)(absolute_fixup[0]);
		absolute_fixup[0] = (void *)((jit_nint)(block->address));
		absolute_fixup = absolute_next;
	}
	block->fixup_absolute_list = 0;
}

void
_jit_gen_end_block(jit_gencode_t gen, jit_block_t block)
{
	/* Nothing to do here for x86 */
}

int
_jit_gen_is_global_candidate(jit_type_t type)
{
	switch(jit_type_remove_tags(type)->kind)
	{
		case JIT_TYPE_SBYTE:
		case JIT_TYPE_UBYTE:
		case JIT_TYPE_SHORT:
		case JIT_TYPE_USHORT:
		case JIT_TYPE_INT:
		case JIT_TYPE_UINT:
		case JIT_TYPE_LONG:
		case JIT_TYPE_ULONG:
		case JIT_TYPE_NINT:
		case JIT_TYPE_NUINT:
		case JIT_TYPE_PTR:
		case JIT_TYPE_SIGNATURE:
		{
			return 1;
		}
	}
	return 0;
}

/*
 * Do the stuff usually handled in jit-rules.c for native implementations
 * here too because the common implementation is not enough for x86_64.
 */

/*
 * Determine if a type corresponds to a structure or union.
 */
static int
is_struct_or_union(jit_type_t type)
{
	type = jit_type_normalize(type);
	if(type)
	{
		if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION)
		{
			return 1;
		}
	}
	return 0;
}

static int
_jit_classify_struct_return(jit_param_passing_t *passing,
					_jit_param_t *param, jit_type_t return_type)
{
	/* Initialize the param passing structure */
	jit_memset(passing, 0, sizeof(jit_param_passing_t));
	jit_memset(param, 0, sizeof(_jit_param_t));

	passing->word_regs = _jit_word_return_regs;
	passing->max_word_regs = _jit_num_word_return_regs;
	passing->float_regs = _jit_sse_return_regs;
	passing->max_float_regs = _jit_num_sse_return_regs;

	if(!(_jit_classify_struct(passing, param, return_type)))
	{
		return 0;
	}

	return 1;
}

/*
 * Load a struct to the register(s) in which it will be returned.
 */
static unsigned char *
return_struct(unsigned char *inst, jit_function_t func, int ptr_reg)
{
	jit_type_t return_type;
	jit_type_t signature = jit_function_get_signature(func);

	return_type = jit_type_get_return(signature);
	if(is_struct_or_union(return_type))
	{
		jit_nuint size;
		jit_param_passing_t passing;
		_jit_param_t return_param;

		if(!_jit_classify_struct_return(&passing, &return_param,
										return_type))
		{
			/* It's an error so simply return insn */
			return inst;
		}

		size = jit_type_get_size(return_type);
		if(size <= 8)
		{
			/* one register is used for returning the value */
			if(IS_GENERAL_REG(return_param.un.reg_info[0].reg))
			{
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;

				if(size <= 4)
				{
					x86_64_mov_reg_regp_size(inst, reg, ptr_reg, 4);
				}
				else
				{
					x86_64_mov_reg_regp_size(inst, reg, ptr_reg, 8);
				}
			}
			else
			{
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;

				if(size <= 4)
				{
					x86_64_movss_reg_regp(inst, reg, ptr_reg);
				}
				else
				{
					x86_64_movsd_reg_regp(inst, reg, ptr_reg);
				}
			}
		}
		else
		{
			/* In this case we might need up to two registers */
			if(return_param.arg_class == 1)
			{
				/* This must be one xmm register */
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;
				int alignment = jit_type_get_alignment(return_type);

				if((alignment & 0xf) == 0)
				{
					/* The type is aligned on a 16 byte boundary */
					x86_64_movaps_reg_regp(inst, reg, ptr_reg);
				}
				else
				{
					x86_64_movups_reg_regp(inst, reg, ptr_reg);
				}
			}
			else
			{
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;

				if(IS_GENERAL_REG(return_param.un.reg_info[0].reg))
				{
					x86_64_mov_reg_regp_size(inst, reg,
											 ptr_reg, 8);
				}
				else
				{
					x86_64_movsd_reg_regp(inst, reg, ptr_reg);
				}
				size -= 8;
				reg = _jit_reg_info[return_param.un.reg_info[1].reg].cpu_reg;
				if(IS_GENERAL_REG(return_param.un.reg_info[1].reg))
				{
					if(size <= 4)
					{
						x86_64_mov_reg_membase_size(inst, reg, ptr_reg,
													8, 4);
					}
					else
					{
						x86_64_mov_reg_membase_size(inst, reg, ptr_reg,
													8, 8);
					}
				}
				else
				{
					if(size <= 4)
					{
						x86_64_movss_reg_membase(inst, reg,
												 ptr_reg, 8);
					}
					else
					{
						x86_64_movsd_reg_membase(inst, reg,
												 ptr_reg, 8);
					}
				}
			}
		}
	}
	return inst;
}

/*
 * Flush a struct return value from the registers to the value
 * on the stack.
 */
static unsigned char *
flush_return_struct(unsigned char *inst, jit_value_t value)
{
	jit_type_t return_type;

	return_type = jit_value_get_type(value);
	if(is_struct_or_union(return_type))
	{
		jit_nuint size;
		jit_nint offset;
		jit_param_passing_t passing;
		_jit_param_t return_param;

		if(!_jit_classify_struct_return(&passing, &return_param, return_type))
		{
			/* It's an error so simply return insn */
			return inst;
		}

		return_param.value = value;

		_jit_gen_fix_value(value);
		size = jit_type_get_size(return_type);
		offset = value->frame_offset;
		if(size <= 8)
		{
			/* one register is used for returning the value */
			if(IS_GENERAL_REG(return_param.un.reg_info[0].reg))
			{
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;

				if(size <= 4)
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset, reg, 4);
				}
				else
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset, reg, 8);
				}
			}
			else
			{
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;

				if(size <= 4)
				{
					x86_64_movss_membase_reg(inst, X86_64_RBP, offset, reg);
				}
				else
				{
					x86_64_movsd_membase_reg(inst, X86_64_RBP, offset, reg);
				}
			}
		}
		else
		{
			/* In this case we might need up to two registers */
			if(return_param.arg_class == 1)
			{
				/* This must be one xmm register */
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;
				int alignment = jit_type_get_alignment(return_type);

				if((alignment & 0xf) == 0)
				{
					/* The type is aligned on a 16 byte boundary */
					x86_64_movaps_membase_reg(inst, X86_64_RBP, offset, reg);
				}
				else
				{
					x86_64_movups_membase_reg(inst, X86_64_RBP, offset, reg);
				}
			}
			else
			{
				int reg = _jit_reg_info[return_param.un.reg_info[0].reg].cpu_reg;

				if(IS_GENERAL_REG(return_param.un.reg_info[0].reg))
				{
					x86_64_mov_membase_reg_size(inst, X86_64_RBP, offset,
												reg, 8);
				}
				else
				{
					x86_64_movsd_membase_reg(inst, X86_64_RBP, offset, reg);
				}
				size -= 8;
				reg = _jit_reg_info[return_param.un.reg_info[1].reg].cpu_reg;
				if(IS_GENERAL_REG(return_param.un.reg_info[1].reg))
				{
					if(size <= 4)
					{
						x86_64_mov_membase_reg_size(inst, X86_64_RBP,
													offset + 8, reg, 4);
					}
					else
					{
						x86_64_mov_membase_reg_size(inst, X86_64_RBP,
													offset + 8, reg, 8);
					}
				}
				else
				{
					if(size <= 4)
					{
						x86_64_movss_membase_reg(inst, X86_64_RBP,
												 offset + 8, reg);
					}
					else
					{
						x86_64_movsd_membase_reg(inst, X86_64_RBP,
												 offset + 8, reg);
					}
				}
			}
		}
	}
	return inst;
}

void
_jit_gen_insn(jit_gencode_t gen, jit_function_t func,
			  jit_block_t block, jit_insn_t insn)
{
	switch(insn->opcode)
	{
	#define JIT_INCLUDE_RULES
	#include "jit-rules-x86-64.inc"
	#undef JIT_INCLUDE_RULES

	default:
		{
			fprintf(stderr, "TODO(%x) at %s, %d\n",
				(int)(insn->opcode), __FILE__, (int)__LINE__);
		}
		break;
	}
}

/*
 * Fixup the passing area after all parameters have been allocated either
 * in registers or on the stack.
 * This is typically used for adding pad words for keeping the stack aligned.
 */
void
_jit_fix_call_stack(jit_param_passing_t *passing)
{
	if((passing->stack_size & 0x0f) != 0)
	{
		passing->stack_size = (passing->stack_size + 0x0f) & ~((jit_nint)0x0f);
		passing->stack_pad = 1;
	}
}

#ifndef JIT_USE_PARAM_AREA
/*
 * Setup the call stack before pushing any parameters.
 * This is used usually for pushing pad words for alignment.
 * The function is needed only if the backend doesn't work with the
 * parameter area.
 */
int
_jit_setup_call_stack(jit_function_t func, jit_param_passing_t *passing)
{
	if(passing->stack_pad)
	{
		int current;
		jit_value_t pad_value;

		pad_value = jit_value_create_nint_constant(func, jit_type_nint, 0);
		if(!pad_value)
		{
			return 0;
		}
		for(current = 0; current < passing->stack_pad; ++current)
		{
			if(!jit_insn_push(func, pad_value))
			{
				return 0;
			}
		}
	}
	return 1;
}
#endif /* !JIT_USE_PARAM_AREA */

/*
 * Push a parameter onto the stack.
 */
static int
push_param(jit_function_t func, _jit_param_t *param, jit_type_t type)
{
	if(is_struct_or_union(type) && !is_struct_or_union(param->value->type))
	{
		jit_value_t value;

		if(!(value = jit_insn_address_of(func, param->value)))
		{
			return 0;
		}
	#ifdef JIT_USE_PARAM_AREA
		/* Copy the value into the outgoing parameter area, by pointer */
		if(!jit_insn_set_param_ptr(func, value, type, param->un.offset))
		{
			return 0;
		}
	#else
		/* Push the parameter value onto the stack, by pointer */
		if(!jit_insn_push_ptr(func, value, type))
		{
			return 0;
		}
		if(param->stack_pad)
		{
			int current;
			jit_value_t pad_value;

			pad_value = jit_value_create_nint_constant(func, jit_type_nint, 0);
			if(!pad_value)
			{
				return 0;
			}
			for(current = 0; current < param->stack_pad; ++current)
			{
				if(!jit_insn_push(func, pad_value))
				{
					return 0;
				}
			}
		}
	#endif
	}
	else
	{
	#ifdef JIT_USE_PARAM_AREA
		/* Copy the value into the outgoing parameter area */
		if(!jit_insn_set_param(func, param->value, param->un.offset))
		{
			return 0;
		}
	#else
		/* Push the parameter value onto the stack */
		if(!jit_insn_push(func, param->value))
		{
			return 0;
		}
		if(param->stack_pad)
		{
			int current;
			jit_value_t pad_value;

			pad_value = jit_value_create_nint_constant(func, jit_type_nint, 0);
			if(!pad_value)
			{
				return 0;
			}
			for(current = 0; current < param->stack_pad; ++current)
			{
				if(!jit_insn_push(func, pad_value))
				{
					return 0;
				}
			}
		}
	#endif
	}
	return 1;
}

int
_jit_setup_reg_param(jit_function_t func, _jit_param_t *param,
					 jit_type_t param_type)
{
	if(param->arg_class == 1)
	{
		param->un.reg_info[0].value = param->value;
	}
	else if(param->arg_class == 2)
	{
		jit_nint size = jit_type_get_size(param_type);
		jit_value_t value_ptr;

		if(!(value_ptr = jit_insn_address_of(func, param->value)))
		{
			return 0;
		}
		if(IS_GENERAL_REG(param->un.reg_info[0].reg))
		{
			param->un.reg_info[0].value =
				jit_insn_load_relative(func, value_ptr, 0, jit_type_long);
			if(!(param->un.reg_info[0].value))
			{
				return 0;
			}
		}
		else
		{
			param->un.reg_info[0].value =
				jit_insn_load_relative(func, value_ptr, 0, jit_type_float64);
			if(!(param->un.reg_info[0].value))
			{
				return 0;
			}
		}
		size -= 8;
		if(IS_GENERAL_REG(param->un.reg_info[1].reg))
		{
			if(size <= 4)
			{
				param->un.reg_info[1].value =
					jit_insn_load_relative(func, value_ptr, 8, jit_type_int);
				if(!(param->un.reg_info[1].value))
				{
					return 0;
				}
			}
			else
			{
				param->un.reg_info[1].value =
					jit_insn_load_relative(func, value_ptr, 8, jit_type_long);
				if(!(param->un.reg_info[1].value))
				{
					return 0;
				}
			}
		}
		else
		{
			if(size <= 4)
			{
				param->un.reg_info[1].value =
					jit_insn_load_relative(func, value_ptr, 8, jit_type_float32);
				if(!(param->un.reg_info[1].value))
				{
					return 0;
				}
			}
			else
			{
				param->un.reg_info[1].value =
					jit_insn_load_relative(func, value_ptr, 8, jit_type_float64);
				if(!(param->un.reg_info[1].value))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

int
_jit_flush_incoming_struct(jit_function_t func, _jit_param_t *param,
						  jit_type_t param_type)
{
	if(param->arg_class == 2)
	{
		jit_value_t address;

		/* Now store the two values in place */
		if(!(address = jit_insn_address_of(func, param->value)))
		{
			return 0;
		}
		if(!jit_insn_store_relative(func, address, 0, param->un.reg_info[0].value))
		{
			return 0;
		}
		if(!jit_insn_store_relative(func, address, 8, param->un.reg_info[1].value))
		{
			return 0;
		}
	}
	return 1;
}

int
_jit_setup_incoming_param(jit_function_t func, _jit_param_t *param,
						  jit_type_t param_type)
{
	if(param->arg_class == JIT_ARG_CLASS_STACK)
	{
		/* The parameter is passed on the stack */
		if(!jit_insn_incoming_frame_posn
				(func, param->value, param->un.offset))
		{
			return 0;
		}
	}
	else
	{
		param_type = jit_type_remove_tags(param_type);

		switch(param_type->kind)
		{
			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				if(param->arg_class == 1)
				{
					if(!jit_insn_incoming_reg(func, param->value, param->un.reg_info[0].reg))
					{
						return 0;
					}
				}
				else
				{
					/* These cases have to be handled specially */
					/* The struct is passed in two registers */
					jit_nuint size = jit_type_get_size(param_type);

					/* The first part is allways a full eightbyte */
					if(IS_GENERAL_REG(param->un.reg_info[0].reg))
					{
						if(!(param->un.reg_info[0].value = jit_value_create(func, jit_type_long)))
						{
							return 0;
						}
					}
					else
					{
						if(!(param->un.reg_info[0].value = jit_value_create(func, jit_type_float64)))
						{
							return 0;
						}
					}
					size -= 8;
					/* The second part might be of any size <= 8 */
					if(IS_GENERAL_REG(param->un.reg_info[1].reg))
					{
						if(size <= 4)
						{
							if(!(param->un.reg_info[1].value =
									jit_value_create(func, jit_type_int)))
							{
								return 0;
							}
						}
						else
						{
							if(!(param->un.reg_info[1].value =
									jit_value_create(func, jit_type_long)))
							{
								return 0;
							}
						}
					}
					else
					{
						if(size <= 4)
						{
							if(!(param->un.reg_info[1].value =
									jit_value_create(func, jit_type_float32)))
							{
								return 0;
							}
						}
						else
						{
							if(!(param->un.reg_info[1].value =
									jit_value_create(func, jit_type_float64)))
							{
								return 0;
							}
						}
					}
					if(!jit_insn_incoming_reg(func,
											  param->un.reg_info[0].value,
											  param->un.reg_info[0].reg))
					{
						return 0;
					}
					if(!jit_insn_incoming_reg(func,
											  param->un.reg_info[1].value,
											  param->un.reg_info[1].reg))
					{
						return 0;
					}
				}
			}
			break;

			default:
			{
				if(!jit_insn_incoming_reg(func, param->value, param->un.reg_info[0].reg))
				{
					return 0;
				}
			}
			break;
		}
	}
	return 1;
}

int
_jit_setup_outgoing_param(jit_function_t func, _jit_param_t *param,
						  jit_type_t param_type)
{
	if(param->arg_class == JIT_ARG_CLASS_STACK)
	{
		/* The parameter is passed on the stack */
		if(!push_param(func, param, param_type))
		{
			return 0;
		}
	}
	else
	{
		if(!jit_insn_outgoing_reg(func, param->un.reg_info[0].value,
										param->un.reg_info[0].reg))
		{
			return 0;
		}
		if(param->arg_class == 2)
		{
			if(!jit_insn_outgoing_reg(func, param->un.reg_info[1].value,
											param->un.reg_info[1].reg))
			{
				return 0;
			}
		}
	}
	return 1;
}

int
_jit_setup_return_value(jit_function_t func, jit_value_t return_value,
						jit_type_t return_type)

{
	/* Structure values must be flushed into the frame, and
	   everything else ends up in a register */
	if(is_struct_or_union(return_type))
	{
		jit_param_passing_t passing;
		_jit_param_t return_param;

		if(!_jit_classify_struct_return(&passing, &return_param, return_type))
		{
			/* It's an error so simply return insn */
			return 0;
		}

		if(return_param.arg_class == 1)
		{
			if(!jit_insn_return_reg(func, return_value,
									return_param.un.reg_info[0].reg))
			{
				return 0;
			}
		}
		else
		{
			if(!jit_insn_flush_struct(func, return_value))
			{
				return 0;
			}
		}
	}
	else if(return_type == jit_type_float32 ||
			return_type == jit_type_float64)
	{
		if(!jit_insn_return_reg(func, return_value, X86_64_REG_XMM0))
		{
			return 0;
		}
	}
	else if(return_type == jit_type_nfloat)
	{
		if(!jit_insn_return_reg(func, return_value, X86_64_REG_ST0))
		{
			return 0;
		}
	}
	else if(return_type->kind != JIT_TYPE_VOID)
	{
		if(!jit_insn_return_reg(func, return_value, X86_64_REG_RAX))
		{
			return 0;
		}
	}
	return 1;
}

void
_jit_init_args(int abi, jit_param_passing_t *passing)
{
	passing->max_word_regs = _jit_num_word_regs;
	passing->word_regs = _jit_word_arg_regs;
	passing->max_float_regs = _jit_num_float_regs;
	passing->float_regs = _jit_float_arg_regs;
}

int
_jit_create_entry_insns(jit_function_t func)
{
	jit_value_t value;
	int has_struct_return = 0;
	jit_type_t signature = func->signature;
	int abi = jit_type_get_abi(signature);
	unsigned int num_args = jit_type_num_params(signature);
	jit_param_passing_t passing;
	_jit_param_t param[num_args];
	_jit_param_t nested_param;
	_jit_param_t struct_return_param;
	int current_param;

	/* Reset the local variable frame size for this function */
	func->builder->frame_size = JIT_INITIAL_FRAME_SIZE;

	/* Initialize the param passing structure */
	jit_memset(&passing, 0, sizeof(jit_param_passing_t));
	jit_memset(param, 0, sizeof(_jit_param_t) * num_args);

	passing.params = param;
	passing.stack_size = JIT_INITIAL_STACK_OFFSET;

	/* Let the specific backend initialize it's part of the params */
	_jit_init_args(abi, &passing);

	/* Allocate the structure return pointer */
	if((value = jit_value_get_struct_pointer(func)))
	{
		jit_memset(&struct_return_param, 0, sizeof(_jit_param_t));
		if(!(_jit_classify_param(&passing, &struct_return_param,
								 jit_type_void_ptr)))
		{
			return 0;
		}
		struct_return_param.value = value;
		has_struct_return = 1;
	}

	/* If the function is nested, then we need an extra parameter
	   to pass the pointer to the parent's local variable frame */
	if(func->nested_parent)
	{
		jit_memset(&nested_param, 0, sizeof(_jit_param_t));
		if(!(_jit_classify_param(&passing, &nested_param,
								 jit_type_void_ptr)))
		{
			return 0;
		}

		nested_param.value = jit_value_create(func, jit_type_void_ptr);
		jit_function_set_parent_frame(func, nested_param.value);
	}

	/* Let the backend classify the parameters */
	for(current_param = 0; current_param < num_args; current_param++)
	{
		jit_type_t param_type;

		param_type = jit_type_get_param(signature, current_param);
		param_type = jit_type_normalize(param_type);

		if(!(_jit_classify_param(&passing, &(passing.params[current_param]),
								 param_type)))
		{
			return 0;
		}
	}

	/* Now we can setup the incoming parameters */
	for(current_param = 0; current_param < num_args; current_param++)
	{
		jit_type_t param_type;

		param_type = jit_type_get_param(signature, current_param);
		if(!(param[current_param].value))
		{
			if(!(param[current_param].value = jit_value_get_param(func, current_param)))
			{
				return 0;
			}
		}
		if(!_jit_setup_incoming_param(func, &(param[current_param]), param_type))
		{
			return 0;
		}
	}

	if(func->nested_parent)
	{
		if(!_jit_setup_incoming_param(func, &nested_param, jit_type_void_ptr))
		{
			return 0;
		}
	}

	if(has_struct_return)
	{
		if(!_jit_setup_incoming_param(func, &struct_return_param, jit_type_void_ptr))
		{
			return 0;
		}
	}

	/* Now we flush the incoming structs passed in registers */
	for(current_param = 0; current_param < num_args; current_param++)
	{
		if(param[current_param].arg_class != JIT_ARG_CLASS_STACK)
		{
			jit_type_t param_type;

			param_type = jit_type_get_param(signature, current_param);
			if(!_jit_flush_incoming_struct(func, &(param[current_param]),
										   param_type))
			{
				return 0;
			}
		}
	}

	return 1;
}

int _jit_create_call_setup_insns
	(jit_function_t func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args,
	 int is_nested, jit_value_t parent_frame,
	 jit_value_t *struct_return, int flags)
{
	int abi = jit_type_get_abi(signature);
	jit_type_t return_type;
	jit_value_t value;
	jit_value_t return_ptr;
	int current_param;
	jit_param_passing_t passing;
	_jit_param_t param[num_args];
	_jit_param_t nested_param;
	_jit_param_t struct_return_param;

	/* Initialize the param passing structure */
	jit_memset(&passing, 0, sizeof(jit_param_passing_t));
	jit_memset(param, 0, sizeof(_jit_param_t) * num_args);

	passing.params = param;
	passing.stack_size = 0;

	/* Let the specific backend initialize it's part of the params */
	_jit_init_args(abi, &passing);

	/* Determine if we need an extra hidden parameter for returning a
	   structure */
	return_type = jit_type_get_return(signature);
	if(jit_type_return_via_pointer(return_type))
	{
		value = jit_value_create(func, return_type);
		if(!value)
		{
			return 0;
		}
		*struct_return = value;
		return_ptr = jit_insn_address_of(func, value);
		if(!return_ptr)
		{
			return 0;
		}
		jit_memset(&struct_return_param, 0, sizeof(_jit_param_t));
		struct_return_param.value = return_ptr;
		if(!(_jit_classify_param(&passing, &struct_return_param,
								 jit_type_void_ptr)))
		{
			return 0;
		}
	}
	else
	{
		*struct_return = 0;
		return_ptr = 0;
	}

	/* Determine how many parameters are going to end up in word registers,
	   and compute the largest stack size needed to pass stack parameters */
	if(is_nested)
	{
		jit_memset(&nested_param, 0, sizeof(_jit_param_t));
		if(!(_jit_classify_param(&passing, &nested_param,
								 jit_type_void_ptr)))
		{
			return 0;
		}

		nested_param.value = parent_frame;
	}

	/* Let the backend classify the parameters */
	for(current_param = 0; current_param < num_args; current_param++)
	{
		jit_type_t param_type;

		param_type = jit_type_get_param(signature, current_param);
		param_type = jit_type_normalize(param_type);

		if(!(_jit_classify_param(&passing, &(passing.params[current_param]),
								 param_type)))
		{
			return 0;
		}
		/* Set the argument value */
		passing.params[current_param].value = args[current_param];
	}

	/* Let the backend do final adjustments to the passing area */
	_jit_fix_call_stack(&passing);

#ifdef JIT_USE_PARAM_AREA
	if(passing.stack_size > func->builder->param_area_size)
	{
		func->builder->param_area_size = passing.stack_size;
	}
#else
	/* Flush deferred stack pops from previous calls if too many
	   parameters have collected up on the stack since last time */
	if(!jit_insn_flush_defer_pop(func, 32 - passing.stack_size))
	{
		return 0;
	}

	if(!_jit_setup_call_stack(func, &passing))
	{
		return 0;
	}
#endif

	/* Now setup the arguments on the stack or in the registers in reverse order */
	/* First process the params passed on the stack */
	current_param = num_args;
	while(current_param > 0)
	{
		--current_param;
		if(param[current_param].arg_class == JIT_ARG_CLASS_STACK)
		{
			jit_type_t param_type;

			param_type = jit_type_get_param(signature, current_param);
			if(!_jit_setup_outgoing_param(func, &(param[current_param]), param_type))
			{
				return 0;
			}
		}
	}

	/* Handle the structure return pointer if it's passed on the stack */
	if(return_ptr)
	{
		if(struct_return_param.arg_class == JIT_ARG_CLASS_STACK)
		{
			if(!_jit_setup_outgoing_param(func, &struct_return_param,
										  jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* Handle the parent's frame pointer if it's passed on the stack */
	if(is_nested)
	{
		if(nested_param.arg_class == JIT_ARG_CLASS_STACK)
		{
			if(!_jit_setup_outgoing_param(func, &nested_param,
										  jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* Now setup the values passed in registers */
	current_param = num_args;
	while(current_param > 0)
	{
		--current_param;

		if(param[current_param].arg_class != JIT_ARG_CLASS_STACK)
		{
			jit_type_t param_type;

			param_type = jit_type_get_param(signature, current_param);
			if(!_jit_setup_reg_param(func, &(param[current_param]), param_type))
			{
				return 0;
			}
		}
	}

	/* Handle the parent's frame pointer if required */
	if(is_nested)
	{
		if(nested_param.arg_class != JIT_ARG_CLASS_STACK)
		{
			if(!_jit_setup_reg_param(func, &nested_param,
									 jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* Handle the structure return pointer if required */
	if(return_ptr)
	{
		if(struct_return_param.arg_class != JIT_ARG_CLASS_STACK)
		{
			if(!_jit_setup_reg_param(func, &struct_return_param,
									 jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* And finally assign the registers */
	current_param = num_args;
	while(current_param > 0)
	{
		--current_param;
		if(param[current_param].arg_class != JIT_ARG_CLASS_STACK)
		{
			jit_type_t param_type;

			param_type = jit_type_get_param(signature, current_param);
			if(!_jit_setup_outgoing_param(func, &(param[current_param]),
										  param_type))
			{
				return 0;
			}
		}
	}

	/* Handle the parent's frame pointer if required */
	if(is_nested)
	{
		if(nested_param.arg_class != JIT_ARG_CLASS_STACK)
		{
			if(!_jit_setup_outgoing_param(func, &nested_param,
									 jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* Add the structure return pointer if required */
	if(return_ptr)
	{
		if(struct_return_param.arg_class != JIT_ARG_CLASS_STACK)
		{
			if(!_jit_setup_outgoing_param(func, &struct_return_param,
										  jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	return 1;
}

int
_jit_create_call_return_insns(jit_function_t func, jit_type_t signature,
							  jit_value_t *args, unsigned int num_args,
							  jit_value_t return_value, int is_nested)
{
	jit_type_t return_type;
	int ptr_return;
#ifndef JIT_USE_PARAM_AREA
	int abi = jit_type_get_abi(signature);
	int current_param;
	jit_param_passing_t passing;
	_jit_param_t param[num_args];
	_jit_param_t nested_param;
	_jit_param_t struct_return_param;
#endif /* !JIT_USE_PARAM_AREA */

	return_type = jit_type_normalize(jit_type_get_return(signature));
	ptr_return = jit_type_return_via_pointer(return_type);
#ifndef JIT_USE_PARAM_AREA
	/* Initialize the param passing structure */
	jit_memset(&passing, 0, sizeof(jit_param_passing_t));
	jit_memset(param, 0, sizeof(_jit_param_t) * num_args);

	passing.params = param;
	passing.stack_size = 0;

	/* Let the specific backend initialize it's part of the params */
	_jit_init_args(abi, &passing);

	/* Determine how many parameters are going to end up in word registers,
	   and compute the largest stack size needed to pass stack parameters */
	if(is_nested)
	{
		jit_memset(&nested_param, 0, sizeof(_jit_param_t));
		if(!(_jit_classify_param(&passing, &nested_param,
								 jit_type_void_ptr)))
		{
			return 0;
		}
	}

	/* Determine if we need an extra hidden parameter for returning a
	   structure */
	if(ptr_return)
	{
		jit_memset(&struct_return_param, 0, sizeof(_jit_param_t));
		if(!(_jit_classify_param(&passing, &struct_return_param,
								 jit_type_void_ptr)))
		{
			return 0;
		}
	}

	/* Let the backend classify the parameters */
	for(current_param = 0; current_param < num_args; current_param++)
	{
		jit_type_t param_type;

		param_type = jit_type_get_param(signature, current_param);
		param_type = jit_type_normalize(param_type);

		if(!(_jit_classify_param(&passing, &(passing.params[current_param]),
								 param_type)))
		{
			return 0;
		}
	}

	/* Let the backend do final adjustments to the passing area */
	_jit_fix_call_stack(&passing);

	/* Pop the bytes from the system stack */
	if(passing.stack_size > 0)
	{
		if(!jit_insn_defer_pop_stack(func, passing.stack_size))
		{
			return 0;
		}
	}
#endif /* !JIT_USE_PARAM_AREA */

	/* Bail out now if we don't need to worry about return values */
	if(!return_value || ptr_return)
	{
		return 1;
	}

	if(!_jit_setup_return_value(func, return_value, return_type))
	{
		return 0;
	}

	/* Everything is back where it needs to be */
	return 1;
}

#endif /* JIT_BACKEND_X86_64 */
