/*
 * jit-gen-arm.h - Code generation macros for the ARM processor.
 *
 * Copyright (C) 2003, 2004  Southern Storm Software, Pty Ltd.
 * Copyright (C) 2008, 2009  Michele Tartara  <mikyt@users.sourceforge.net>
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

#ifndef	_JIT_GEN_ARM_H
#define	_JIT_GEN_ARM_H

#include <assert.h>
#include <jit-rules-arm.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Register numbers.
 */
typedef enum
{
	ARM_R0   = 0,
	ARM_R1   = 1,
	ARM_R2   = 2,
	ARM_R3   = 3,
	ARM_R4   = 4,
	ARM_R5   = 5,
	ARM_R6   = 6,
	ARM_R7   = 7,
	ARM_R8   = 8,
	ARM_R9   = 9,
	ARM_R10  = 10,
	ARM_R11  = 11,
	ARM_R12  = 12,
	ARM_R13  = 13,
	ARM_R14  = 14,
	ARM_R15  = 15,
	ARM_FP   = ARM_R11,			/* Frame pointer */
	ARM_LINK = ARM_R14,			/* Link register */
	ARM_PC   = ARM_R15,			/* Program counter */
	ARM_WORK = ARM_R12,			/* Work register that we can destroy */
	ARM_SP   = ARM_R13			/* Stack pointer */

} ARM_REG;

#ifdef JIT_ARM_HAS_FPA
/*
 * Floating-point register numbers for the FPA architecture.
 */
typedef enum
{
	ARM_F0	= 0,
	ARM_F1	= 1,
	ARM_F2	= 2,
	ARM_F3	= 3,
	ARM_F4	= 4,
	ARM_F5	= 5,
	ARM_F6	= 6,
	ARM_F7	= 7

} ARM_FREG;
#endif

#ifdef JIT_ARM_HAS_VFP
/*
 * Floating-point register numbers for the Vector Floating Point architecture.
 */
typedef enum
{
	ARM_S0	= 0,
	ARM_S1	= 1,
	ARM_S2	= 2,
	ARM_S3	= 3,
	ARM_S4	= 4,
	ARM_S5	= 5,
	ARM_S6	= 6,
	ARM_S7	= 7,
	ARM_S8	= 8,
	ARM_S9	= 9,
	ARM_S10	= 10,
	ARM_S11	= 11,
	ARM_S12	= 12,
	ARM_S13	= 13,
	ARM_S14	= 14,
	ARM_S15	= 15,
	ARM_D8 = 8,
	ARM_D9 = 9,
	ARM_D10 = 10,
	ARM_D11 = 11,
	ARM_D12 = 12,
	ARM_D13 = 13,
	ARM_D14 = 14,
	ARM_D15 = 15
} ARM_FREG;
#endif

/*
 * Condition codes.
 */
typedef enum
{
	ARM_CC_EQ    = 0,			/* Equal */
	ARM_CC_NE    = 1,			/* Not equal */
	ARM_CC_CS    = 2,			/* Carry set */
	ARM_CC_CC    = 3,			/* Carry clear */
	ARM_CC_MI    = 4,			/* Negative */
	ARM_CC_PL    = 5,			/* Positive */
	ARM_CC_VS    = 6,			/* Overflow set */
	ARM_CC_VC    = 7,			/* Overflow clear */
	ARM_CC_HI    = 8,			/* Higher */
	ARM_CC_LS    = 9,			/* Lower or same */
	ARM_CC_GE    = 10,			/* Signed greater than or equal */
	ARM_CC_LT    = 11,			/* Signed less than */
	ARM_CC_GT    = 12,			/* Signed greater than */
	ARM_CC_LE    = 13,			/* Signed less than or equal */
	ARM_CC_AL    = 14,			/* Always */
	ARM_CC_NV    = 15,			/* Never */
	ARM_CC_GE_UN = ARM_CC_CS,	/* Unsigned greater than or equal */
	ARM_CC_LT_UN = ARM_CC_CC,	/* Unsigned less than */
	ARM_CC_GT_UN = ARM_CC_HI,	/* Unsigned greater than */
	ARM_CC_LE_UN = ARM_CC_LS	/* Unsigned less than or equal */

} ARM_CC;

/*
 * Arithmetic and logical operations.
 */
typedef enum
{
	ARM_AND = 0,				/* Bitwise AND */
	ARM_EOR = 1,				/* Bitwise XOR */
	ARM_SUB = 2,				/* Subtract */
	ARM_RSB = 3,				/* Reverse subtract */
	ARM_ADD = 4,				/* Add */
	ARM_ADC = 5,				/* Add with carry */
	ARM_SBC = 6,				/* Subtract with carry */
	ARM_RSC = 7,				/* Reverse subtract with carry */
	ARM_TST = 8,				/* Test with AND */
	ARM_TEQ = 9,				/* Test with XOR */
	ARM_CMP = 10,				/* Test with SUB (compare) */
	ARM_CMN = 11,				/* Test with ADD */
	ARM_ORR = 12,				/* Bitwise OR */
	ARM_MOV = 13,				/* Move */
	ARM_BIC = 14,				/* Test with Op1 & ~Op2 */
	ARM_MVN = 15				/* Bitwise NOT: Negate the content of a word*/

} ARM_OP;

/*
 * Shift operators.
 */
typedef enum
{
	ARM_SHL = 0,				/* Logical left */
	ARM_SHR = 1,				/* Logical right */
	ARM_SAR = 2,				/* Arithmetic right */
	ARM_ROR = 3					/* Rotate right */

} ARM_SHIFT;

#ifdef JIT_ARM_HAS_FPA
/* Floating point definitions for the FPA architecture */

/*
 * Floating-point unary operators.
 */
typedef enum
{
	ARM_MVF		= 0,			/* Move */
	ARM_MNF		= 1,			/* Move negative */
	ARM_ABS		= 2,			/* Absolute value */
	ARM_RND		= 3,			/* Round */
	ARM_SQT		= 4,			/* Square root */
	ARM_LOG		= 5,			/* log10 */
	ARM_LGN		= 6,			/* ln */
	ARM_EXP		= 7,			/* exp */
	ARM_SIN		= 8,			/* sin */
	ARM_COS		= 9,			/* cos */
	ARM_TAN		= 10,			/* tan */
	ARM_ASN		= 11,			/* asin */
	ARM_ACS		= 12,			/* acos */
	ARM_ATN		= 13			/* atan */

} ARM_FUNARY;

/*
 * Floating-point binary operators.
 */
typedef enum
{
	ARM_ADF		= 0,			/* Add */
	ARM_MUF		= 1,			/* Multiply */
	ARM_SUF		= 2,			/* Subtract */
	ARM_RSF		= 3,			/* Reverse subtract */
	ARM_DVF		= 4,			/* Divide */
	ARM_RDF		= 5,			/* Reverse divide */
	ARM_POW		= 6,			/* pow */
	ARM_RPW		= 7,			/* Reverse pow */
	ARM_RMF		= 8,			/* Remainder */
	ARM_FML		= 9,			/* Fast multiply (32-bit only) */
	ARM_FDV		= 10,			/* Fast divide (32-bit only) */
	ARM_FRD		= 11,			/* Fast reverse divide (32-bit only) */
	ARM_POL		= 12			/* Polar angle */

} ARM_FBINARY;

#endif /* JIT_ARM_HAS_FPA */

#ifdef JIT_ARM_HAS_VFP
/* Floating point definitions for the Vector Floating Point architecture */

/*
 * Floating-point unary operators.
 */
typedef enum
{
	ARM_MVF		= 0,			/* Move - FCPY */
	ARM_MNF		= 1,			/* Move negative - FNEG */
	ARM_ABS		= 2			/* Absolute value - FABS */
} ARM_FUNARY;

/*
 * Floating-point binary operators.
 */
typedef enum
{
	ARM_FADD	= 0,			/* Add */
	ARM_FMUL	= 1,			/* Multiply */
	ARM_FSUB	= 2,			/* Subtract */
	ARM_FDIV	= 4			/* Divide */
} ARM_FBINARY;

#endif /* JIT_ARM_HAS_VFP */

/*
 * Number of registers that are used for parameters (r0-r3).
 */
#define	ARM_NUM_PARAM_REGS	4

/*
 * Type that keeps track of the instruction buffer.
 */
typedef unsigned int arm_inst_word;
typedef struct
{
	arm_inst_word *current;
	arm_inst_word *limit;

} arm_inst_buf;
#define	arm_inst_get_posn(inst)		((inst).current)
#define	arm_inst_get_limit(inst)	((inst).limit)

/*
 * Build an instruction prefix from a condition code and a mask value.
 */
#define	arm_build_prefix(cond,mask)	\
			((((unsigned int)(cond)) << 28) | ((unsigned int)(mask)))

/*
 * Build an "always" instruction prefix for a regular instruction.
 */
#define arm_prefix(mask)	(arm_build_prefix(ARM_CC_AL, (mask)))

/*
 * Build special "always" prefixes.
 */
#define	arm_always			(arm_build_prefix(ARM_CC_AL, 0))
#define	arm_always_cc		(arm_build_prefix(ARM_CC_AL, (1 << 20)))
#define	arm_always_imm		(arm_build_prefix(ARM_CC_AL, (1 << 25)))

/*
 * Wrappers for "arm_always*" that allow higher-level routines
 * to change code generation to be based on a condition.  This is
 * used to perform branch elimination.
 */
#ifndef arm_execute
#define	arm_execute			arm_always
#define	arm_execute_cc		arm_always_cc
#define	arm_execute_imm		arm_always_imm
#endif

/*
 * Initialize an instruction buffer.
 */
#define	arm_inst_buf_init(inst,start,end)	\
			do { \
				(inst).current = (arm_inst_word *)(start); \
				(inst).limit = (arm_inst_word *)(end); \
			} while (0)

/*
 * Add an instruction to an instruction buffer.
 */
#define	arm_inst_add(inst,value)	\
			do { \
				if((inst).current < (inst).limit) \
				{ \
					*((inst).current)++ = (value); \
				} \
			} while (0)

/*
 * Arithmetic or logical operation which doesn't set condition codes.
 */
#define	arm_alu_reg_reg(inst,opc,dreg,sreg1,sreg2)	\
			do { \
				arm_inst_add((inst), arm_execute | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg1)) << 16) | \
							 ((unsigned int)(sreg2))); \
			} while (0)
#define	arm_alu_reg_imm8(inst,opc,dreg,sreg,imm)	\
			do { \
				arm_inst_add((inst), arm_execute_imm | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg)) << 16) | \
							 ((unsigned int)((imm) & 0xFF))); \
			} while (0)
#define	arm_alu_reg_imm8_cond(inst,opc,dreg,sreg,imm,cond)	\
			do { \
				arm_inst_add((inst), arm_build_prefix((cond), (1 << 25)) | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg)) << 16) | \
							 ((unsigned int)((imm) & 0xFF))); \
			} while (0)
#define	arm_alu_reg_imm8_rotate(inst,opc,dreg,sreg,imm,rotate)	\
			do { \
				arm_inst_add((inst), arm_execute_imm | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg)) << 16) | \
							(((unsigned int)(rotate)) << 8) | \
							 ((unsigned int)((imm) & 0xFF))); \
			} while (0)
extern void _arm_alu_reg_imm
		(arm_inst_buf *inst, int opc, int dreg,
		 int sreg, int imm, int saveWork, int execute_prefix);
#define	arm_alu_reg_imm(inst,opc,dreg,sreg,imm)	\
			do { \
				int __alu_imm = (int)(imm); \
				if(__alu_imm >= 0 && __alu_imm < 256) \
				{ \
					arm_alu_reg_imm8 \
						((inst), (opc), (dreg), (sreg), __alu_imm); \
				} \
				else \
				{ \
					_arm_alu_reg_imm \
						(&(inst), (opc), (dreg), (sreg), __alu_imm, 0, \
						 arm_execute); \
				} \
			} while (0)
#define	arm_alu_reg_imm_save_work(inst,opc,dreg,sreg,imm)	\
			do { \
				int __alu_imm_save = (int)(imm); \
				if(__alu_imm_save >= 0 && __alu_imm_save < 256) \
				{ \
					arm_alu_reg_imm8 \
						((inst), (opc), (dreg), (sreg), __alu_imm_save); \
				} \
				else \
				{ \
					_arm_alu_reg_imm \
						(&(inst), (opc), (dreg), (sreg), __alu_imm_save, 1, \
						 arm_execute); \
				} \
			} while (0)
#define arm_alu_reg(inst,opc,dreg,sreg)	\
			do { \
				arm_inst_add((inst), arm_execute | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							 ((unsigned int)(sreg))); \
			} while (0)
#define arm_alu_reg_cond(inst,opc,dreg,sreg,cond)	\
			do { \
				arm_inst_add((inst), arm_build_prefix((cond), 0) | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							 ((unsigned int)(sreg))); \
			} while (0)

/*
 * Arithmetic or logical operation which sets condition codes.
 */
#define	arm_alu_cc_reg_reg(inst,opc,dreg,sreg1,sreg2)	\
			do { \
				arm_inst_add((inst), arm_execute_cc | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg1)) << 16) | \
							 ((unsigned int)(sreg2))); \
			} while (0)
#define	arm_alu_cc_reg_imm8(inst,opc,dreg,sreg,imm)	\
			do { \
				arm_inst_add((inst), arm_execute_imm | arm_execute_cc | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg)) << 16) | \
							 ((unsigned int)((imm) & 0xFF))); \
			} while (0)
#define arm_alu_cc_reg(inst,opc,dreg,sreg)	\
			do { \
				arm_inst_add((inst), arm_execute_cc | \
							(((unsigned int)(opc)) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							 ((unsigned int)(sreg))); \
			} while (0)

/*
 * Test operation, which sets the condition codes but has no other result.
 */
#define arm_test_reg_reg(inst,opc,sreg1,sreg2)	\
			do { \
				arm_alu_cc_reg_reg((inst), (opc), 0, (sreg1), (sreg2)); \
			} while (0)
#define arm_test_reg_imm8(inst,opc,sreg,imm)	\
			do { \
				arm_alu_cc_reg_imm8((inst), (opc), 0, (sreg), (imm)); \
			} while (0)
#define arm_test_reg_imm(inst,opc,sreg,imm)	\
			do { \
				int __test_imm = (int)(imm); \
				if(__test_imm >= 0 && __test_imm < 256) \
				{ \
					arm_alu_cc_reg_imm8((inst), (opc), 0, (sreg), __test_imm); \
				} \
				else \
				{ \
					arm_mov_reg_imm((inst), ARM_WORK, __test_imm); \
					arm_test_reg_reg((inst), (opc), (sreg), ARM_WORK); \
				} \
			} while (0)

#define arm_test_reg_membase(inst,opc,reg,basereg,disp,scratchreg)	\
			do {	\
				assert(reg!=scratchreg);	\
				assert(basereg!=scratchreg);	\
				arm_load_membase((inst), (tmpreg),(basereg),(disp));	\
				arm_test_reg_reg((inst), (opc), (reg), (tmpreg));	\
} while (0)

/*
 * Move a value between registers.
 */
#define	arm_mov_reg_reg(inst,dreg,sreg)	\
			do { \
				arm_alu_reg((inst), ARM_MOV, (dreg), (sreg)); \
			} while (0)
			
/**
 * Move a value between floating point registers.
 * @var inst is the pointer to the location of memory at which the instruction will be put
 * @var dreg is the destination register
 * @var freg is the source register
 */
#define	arm_mov_freg_freg(inst,dreg,sreg)	\
			do { \
				arm_alu_freg((inst), ARM_MVF, (dreg), (sreg)); \
			} while (0)

/*
 * Move an immediate value into a register.  This is hard because
 * ARM lacks an instruction to load a 32-bit immediate value directly.
 * We handle the simple cases and then bail out to a function for the rest.
 */
#define	arm_mov_reg_imm8(inst,reg,imm)	\
			do { \
				arm_alu_reg_imm8((inst), ARM_MOV, (reg), 0, (imm)); \
			} while (0)
#define	arm_mov_reg_imm8_rotate(inst,reg,imm,rotate)	\
			do { \
				arm_alu_reg_imm8_rotate((inst), ARM_MOV, (reg), \
										0, (imm), (rotate)); \
			} while (0)
extern void _arm_mov_reg_imm
	(arm_inst_buf *inst, int reg, int value, int execute_prefix);
extern int arm_is_complex_imm(int value);

/**
 * Moves the immediate value imm into register reg.
 *
 * In case imm is > 255, it builds the value one byte at a time, by calling _arm_mov_reg_imm
 * This is done by using a big number of instruction.
 * In that case, using mov_reg_imm (defined in jit-rules-arm.c is probably a better idea, when possible
 */
#define	arm_mov_reg_imm(inst,reg,imm)	\
			do { \
				int __imm = (int)(imm); \
				if(__imm >= 0 && __imm < 256) \
				{ \
					arm_mov_reg_imm8((inst), (reg), __imm); \
				} \
				else if((reg) == ARM_PC) \
				{ \
					_arm_mov_reg_imm \
						(&(inst), ARM_WORK, __imm, arm_execute); \
					arm_mov_reg_reg((inst), ARM_PC, ARM_WORK); \
				} \
				else if(__imm > -256 && __imm < 0) \
				{ \
					arm_mov_reg_imm8((inst), (reg), ~(__imm)); \
					arm_alu_reg((inst), ARM_MVN, (reg), (reg)); \
				} \
				else \
				{ \
					_arm_mov_reg_imm(&(inst), (reg), __imm, arm_execute); \
				} \
			} while (0)

#define ARM_NOBASEREG (-1)

/**
 * LDR (Load Register), LDRB (Load Register Byte)
 * Load the content of the memory area of size "size" at position basereg+disp+(indexreg<<shift) into the 32-bit "reg", with zero-extension.
 * "scratchreg" is a scratch register that has to be asked to the register allocator; it is 
 * used only when disp!=0; if disp==0, it can have whatever value, since it won't be used
 */
#define arm_mov_reg_memindex(inst,reg,basereg,disp,indexreg,shift,size,scratchreg)	\
do {	\
	if (basereg==ARM_NOBASEREG)	\
	{	\
		fprintf(stderr, "TODO(NOBASEREG) at %s, %d\n", __FILE__, (int)__LINE__);	\
	}	\
	else	\
	{	\
		/* Add the displacement (only if needed)*/\
		int tempreg=(basereg);	\
		if (disp!=0)	\
		{	\
			tempreg=(scratchreg);	\
			assert(tempreg!=basereg);	\
			assert(tempreg!=indexreg);	\
			arm_alu_reg_imm((inst), ARM_ADD, (tempreg), (basereg), (disp)); \
		}	\
		/* Load the content, depending on its size */	\
		switch ((size)) {	\
			case 1: arm_load_memindex_either(inst,reg,(tempreg),indexreg,shift,0x00400000); break;	\
			case 2: arm_load_memindex_either(inst,reg,tempreg,indexreg,shift,0);	\
				arm_shift_reg_imm8((inst), ARM_SHL, (reg), (reg), 16);	\
				arm_shift_reg_imm8((inst), ARM_SHR, (reg), (reg), 16);	\
				break; \
			case 4: arm_load_memindex_either(inst,reg,tempreg,indexreg,shift,0); break;	\
			default: assert (0);	\
		}	\
	}	\
} while (0)

/**
 * Store the content of "reg" into a memory area of size "size" at position basereg+disp+(indexreg<<shift)
 * NB: the scratch register has to be asked to the register allocator.
 *     It can't be ARM_WORK, since it's already used
 */
#define arm_mov_memindex_reg(inst,basereg,disp,indexreg,shift,reg,size,scratchreg)	\
do {	\
	if (basereg==ARM_NOBASEREG)	\
	{	\
		fprintf(stderr, "TODO(NOBASEREG) at %s, %d\n", __FILE__, (int)__LINE__);	\
	}	\
	else	\
	{	\
		arm_shift_reg_imm8((inst), ARM_SHL, (ARM_WORK), (indexreg), (shift));	\
		arm_alu_reg_reg((inst), ARM_ADD, (scratchreg), (basereg), ARM_WORK);	\
		arm_mov_membase_reg((inst),(scratchreg),(disp),(reg),(size))	\
	}	\
} while (0);

/*
 * Stores the content of register "reg" in memory, at position "mem" with size "size"
 * NB: destroys the content of ARM_WORK
 */
#define arm_mov_mem_reg(inst,mem,reg,size)	\
do {	\
	arm_mov_reg_imm((inst), ARM_WORK, (mem));	\
	switch ((size)) {	\
		case 1: arm_store_membase_byte((inst), (reg), ARM_WORK, 0); break;	\
		case 2: arm_store_membase_short((inst), (reg), ARM_WORK, 0); break;	\
		case 4: arm_store_membase((inst), (reg), ARM_WORK, 0); break;	\
		default: jit_assert(0);	\
	}	\
} while (0)

/*
 * Stores the content of "imm" in memory, at position "mem" with size "size". Uses a scratch register (scratchreg),
 * that has to be asked to the register allocator via the [scratch reg] parameter in the definition of the OPCODE.
 * NB: destroys the content of ARM_WORK
 */
#define arm_mov_mem_imm(inst,mem,imm,size,scratchreg)	\
do {	\
	arm_mov_reg_imm((inst), (scratchreg), (imm));	\
	arm_mov_reg_imm((inst), ARM_WORK, (mem));	\
	switch ((size)) {	\
		case 1: arm_store_membase_byte((inst), (scratchreg), ARM_WORK, 0); break;	\
		case 2: arm_store_membase_short((inst), (scratchreg), ARM_WORK, 0); break;	\
		case 4: arm_store_membase((inst), (scratchreg), ARM_WORK, 0); break;	\
		default: assert(0);	\
	}	\
} while (0)

/**
 * Set "size" bytes at position basereg+disp at the value of imm
 * NB: destroys the content of scratchreg. A good choice for scratchreg is ARM_WORK,
 * unless the value of disp is too big to be handled by arm_store_membase_either. In that case,
 * it's better to require the allocation of a scratch reg by adding the parameter [scratch reg] at the end
 * of the parameters of the rule inside jit-rules-arm.ins that's calling this function.
 */
#define arm_mov_membase_imm(inst,basereg,disp,imm,size,scratchreg)	\
do {	\
	arm_mov_reg_imm((inst), (scratchreg), imm);	\
	arm_mov_membase_reg((inst), (basereg), (disp), (scratchreg), (size));	\
} while(0);

/**
 * Set "size" bytes at position basereg+disp at the value of reg
 * NB: might destroy the content of ARM_WORK because of arm_store_membase
 */
#define arm_mov_membase_reg(inst,basereg,disp,reg,size)	\
do {	\
	switch ((size)) {	\
		case 1: arm_store_membase_byte((inst), (reg), (basereg), (disp)); break;	\
		case 2: arm_store_membase_short((inst), (reg), (basereg), (disp)); break;	\
		case 4: arm_store_membase((inst), (reg), (basereg), (disp)); break;	\
		default: jit_assert(0);	\
	}	\
} while(0);

/**
* Set the value of "reg" to the "size"-bytes-long value held in memory at position basereg+disp
* NB: can destroys the content of ARM_WORK because of arm_store_membase_short
*/
#define arm_mov_reg_membase(inst,reg,basereg,disp,size)	\
do {	\
	switch ((size)) {	\
		case 1: arm_load_membase_byte((inst), (reg), (basereg), (disp)); break;	\
		case 2: arm_load_membase_short((inst), (reg), (basereg), (disp)); break;	\
		case 4: arm_load_membase((inst), (reg), (basereg), (disp)); break;	\
		default: jit_assert(0);	\
}	\
} while(0);

/*
 * Clear a register to zero.
 */
#define	arm_clear_reg(inst,reg)	\
			do { \
				arm_mov_reg_imm8((inst), (reg), 0); \
			} while (0)

/*
 * No-operation instruction.
 */
#define	arm_nop(inst)	arm_mov_reg_reg((inst), ARM_R0, ARM_R0)

/*
 * Perform a shift operation.
 */
#define	arm_shift_reg_reg(inst,opc,dreg,sreg1,sreg2) \
			do { \
				arm_inst_add((inst), arm_execute | \
							(((unsigned int)ARM_MOV) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg2)) << 8) | \
							(((unsigned int)(opc)) << 5) | \
							 ((unsigned int)(1 << 4)) | \
							 ((unsigned int)(sreg1))); \
			} while (0)
#define	arm_shift_reg_imm8(inst,opc,dreg,sreg,imm) \
			do { \
				arm_inst_add((inst), arm_execute | \
							(((unsigned int)ARM_MOV) << 21) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(opc)) << 5) | \
							(((unsigned int)(imm)) << 7) | \
							 ((unsigned int)(sreg))); \
			} while (0)

/*
 * Perform a multiplication instruction.  Note: ARM instruction rules
 * say that dreg should not be the same as sreg2, so we swap the order
 * of the arguments if that situation occurs.  We assume that sreg1
 * and sreg2 are distinct registers.
 */
#define arm_mul_reg_reg(inst,dreg,sreg1,sreg2)	\
			do { \
				if((dreg) != (sreg2)) \
				{ \
					arm_inst_add((inst), arm_prefix(0x00000090) | \
								(((unsigned int)(dreg)) << 16) | \
								(((unsigned int)(sreg1)) << 8) | \
								 ((unsigned int)(sreg2))); \
				} \
				else \
				{ \
					arm_inst_add((inst), arm_prefix(0x00000090) | \
								(((unsigned int)(dreg)) << 16) | \
								(((unsigned int)(sreg2)) << 8) | \
								 ((unsigned int)(sreg1))); \
				} \
			} while (0)

#ifdef JIT_ARM_HAS_FPA
/*
 * Perform a binary operation on floating-point arguments.
 */
#define	arm_alu_freg_freg(inst,opc,dreg,sreg1,sreg2)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x0E000180) | \
							(((unsigned int)(opc)) << 20) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg1)) << 16) | \
							 ((unsigned int)(sreg2))); \
			} while (0)
#define	arm_alu_freg_freg_32(inst,opc,dreg,sreg1,sreg2)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x0E000100) | \
							(((unsigned int)(opc)) << 20) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg1)) << 16) | \
							 ((unsigned int)(sreg2))); \
			} while (0)

/*
 * Perform a unary operation on floating-point arguments.
 */
#define	arm_alu_freg(inst,opc,dreg,sreg)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x0E008180) | \
							(((unsigned int)(opc)) << 20) | \
							(((unsigned int)(dreg)) << 12) | \
							 ((unsigned int)(sreg))); \
			} while (0)
#define	arm_alu_freg_32(inst,opc,dreg,sreg)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x0E008100) | \
							(((unsigned int)(opc)) << 20) | \
							(((unsigned int)(dreg)) << 12) | \
							 ((unsigned int)(sreg))); \
			} while (0)


#endif /* JIT_ARM_HAS_FPA */

#ifdef JIT_ARM_HAS_VFP
/**
 * Perform a binary operation on double-precision floating-point arguments.
 * OPC is the number indicating the operation to execute (taken from enum ARM_FBINARY)
 * sreg1 and sreg2 are the registers containing the first and second operand
 * dreg is the destination register
 */
#define	arm_alu_freg_freg(inst,opc,dreg,sreg1,sreg2)	\
			do { \
				unsigned int mask;	\
				switch(opc)	\
				{	\
					case ARM_FADD:	\
						mask=0x0E300B00;	\
						break;	\
					case ARM_FMUL:	\
						mask=0x0E200B00;	\
						break;	\
					case ARM_FSUB:	\
						mask=0x0E300B40;	\
						break;	\
					case ARM_FDIV:	\
						mask=0x0E800B00;	\
						break;	\
					default:	\
						printf("Unimplemented binary operation %d in %s\n", opc,  __FILE__);	\
						abort();	\
				}	\
				arm_inst_add((inst), arm_prefix(mask) | \
							(((unsigned int)(dreg)) << 12) | \
							(((unsigned int)(sreg1)) << 16) | \
							((unsigned int)(sreg2))); \
			} while (0)
/**
 * Perform a binary operation on single-precision floating-point arguments.
 * OPC is the number indicating the operation to execute (taken from enum ARM_FBINARY)
 * sreg1 and sreg2 are the registers containing the first and second operand
 * dreg is the destination register
 */
#define	arm_alu_freg_freg_32(inst,opc,dreg,sreg1,sreg2)	\
do { \
	unsigned int mask;	\
	switch(opc)	\
	{	\
		case ARM_FADD:	\
			mask=0x0E300A00;	\
			break;	\
		case ARM_FMUL:	\
			mask=0x0E200A00;	\
			break;	\
		case ARM_FSUB:	\
			mask=0x0E300A40;	\
			break;	\
		case ARM_FDIV:	\
			mask=0x0E800A00;	\
			break;	\
		default:	\
			printf("Unimplemented binary operation %d in %s\n", opc,  __FILE__);	\
			abort();	\
	}	\
	unsigned int dreg_top_4_bits = (dreg & 0x1E) >> 1;	\
	unsigned int dreg_bottom_bit = (dreg & 0x01);	\
	unsigned int sreg1_top_4_bits = (sreg1 & 0x1E) >> 1;	\
	unsigned int sreg1_bottom_bit = (sreg1 & 0x01);	\
	unsigned int sreg2_top_4_bits = (sreg2 & 0x1E) >> 1;	\
	unsigned int sreg2_bottom_bit = (sreg2 & 0x01);	\
	arm_inst_add((inst), arm_prefix(mask) | \
				(((unsigned int)(dreg_top_4_bits)) << 12) |	\
				(((unsigned int)(dreg_bottom_bit)) << 22) |	\
				(((unsigned int)(sreg1_top_4_bits)) << 16) |	\
				(((unsigned int)(sreg1_bottom_bit)) << 7) |	\
				(((unsigned int)(sreg2_bottom_bit)) << 5) |	\
				((unsigned int)(sreg2_top_4_bits))); \
} while (0)

/**
* Perform a unary operation on a double-precision floating-point argument.
* OPC is the number indicating the operation to execute (taken from enum ARM_FUNARY)
* sreg is the register containing the operand
* dreg is the destination register
*/
#define	arm_alu_freg(inst,opc,dreg,sreg)	\
			do { \
				unsigned int mask;	\
				switch(opc)	\
				{	\
					case ARM_MVF:	\
						mask=0xEB00B40;	\
						break;	\
					case ARM_MNF:	\
						mask=0xEB10B40;	\
						break;	\
					case ARM_ABS:	\
						mask=0xEB00BC0;	\
						break;	\
					default:	\
						printf("Unimplemented unary operation %d in %s\n", opc,  __FILE__);	\
						abort();	\
				}	\
				arm_inst_add((inst), arm_prefix(mask) | \
							(((unsigned int)(dreg)) << 12) |	\
							 ((unsigned int)(sreg))); \
			} while (0)
			
/**
 * Perform a unary operation on a single-precision floating-point argument.
 * OPC is the number indicating the operation to execute (taken from enum ARM_FUNARY)
 * sreg is the register containing the operand
 * dreg is the destination register
 */			
#define	arm_alu_freg_32(inst,opc,dreg,sreg)	\
			do { \
				unsigned int mask;	\
				switch(opc)	\
				{	\
					case ARM_MVF:	\
						mask=0xEB00A40;	\
						break;	\
					case ARM_MNF:	\
						mask=0xEB10A40;	\
						break;	\
					case ARM_ABS:	\
						mask=0xEB00AC0;	\
						break;	\
					default:	\
						printf("Unimplemented OPCODE in %s\n", __FILE__);	\
						abort();	\
				}	\
				unsigned int dreg_top_4_bits = (dreg & 0x1E) >> 1;	\
				unsigned int dreg_bottom_bit = (dreg & 0x01);	\
				unsigned int sreg_top_4_bits = (sreg & 0x1E) >> 1;	\
				unsigned int sreg_bottom_bit = (sreg & 0x01);	\
				arm_inst_add((inst), arm_prefix(mask) | \
							(((unsigned int)(dreg_top_4_bits)) << 12) |	\
							(((unsigned int)(dreg_bottom_bit)) << 22) |	\
							(((unsigned int)(sreg_bottom_bit)) << 5)  |	\
							 ((unsigned int)(sreg_top_4_bits))); \
			} while (0)

#endif /* JIT_ARM_HAS_VFP */
/*
 * Branch or jump immediate by a byte offset.  The offset is
 * assumed to be +/- 32 Mbytes.
 */
#define	arm_branch_imm(inst,cond,imm)	\
			do { \
				arm_inst_add((inst), arm_build_prefix((cond), 0x0A000000) | \
							(((unsigned int)(((int)(imm)) >> 2)) & \
								0x00FFFFFF)); \
			} while (0)
#define	arm_jump_imm(inst,imm)	arm_branch_imm((inst), ARM_CC_AL, (imm))

/*
 * Branch or jump to a specific target location.  The offset is
 * assumed to be +/- 32 Mbytes.
 */
#define	arm_branch(inst,cond,target)	\
			do { \
				int __br_offset = (int)(((unsigned char *)(target)) - \
					           (((unsigned char *)((inst).current)) + 8)); \
				arm_branch_imm((inst), (cond), __br_offset); \
			} while (0)
#define	arm_jump(inst,target)	arm_branch((inst), ARM_CC_AL, (target))

/*
 * Jump to a specific target location that may be greater than
 * 32 Mbytes away from the current location.
 */
#define	arm_jump_long(inst,target)	\
			do { \
				int __jmp_offset = (int)(((unsigned char *)(target)) - \
					            (((unsigned char *)((inst).current)) + 8)); \
				if(__jmp_offset >= -0x04000000 && __jmp_offset < 0x04000000) \
				{ \
					arm_jump_imm((inst), __jmp_offset); \
				} \
				else \
				{ \
					arm_mov_reg_imm((inst), ARM_PC, (int)(target)); \
				} \
			} while (0)

/*
 * Back-patch a branch instruction.
 */
#define	arm_patch(inst,posn,target)	\
			do { \
				int __p_offset = (int)(((unsigned char *)(target)) - \
							          (((unsigned char *)(posn)) + 8)); \
				__p_offset = (__p_offset >> 2) & 0x00FFFFFF; \
				if(((arm_inst_word *)(posn)) < (inst).limit) \
				{ \
					*((int *)(posn)) = (*((int *)(posn)) & 0xFF000000) | \
						__p_offset; \
				} \
			} while (0)

/*
 * Call a subroutine immediate by a byte offset.
 */
#define	arm_call_imm(inst,imm)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x0B000000) | \
							(((unsigned int)(((int)(imm)) >> 2)) & \
								0x00FFFFFF)); \
			} while (0)

/*
 * Call a subroutine at a specific target location.
 * (Equivalent to x86_call_code)
 */
#define	arm_call(inst,target)	\
			do { \
				int __call_offset = (int)(((unsigned char *)(target)) - \
					             (((unsigned char *)((inst).current)) + 8)); \
				if(__call_offset >= -0x04000000 && __call_offset < 0x04000000) \
				{ \
					arm_call_imm((inst), __call_offset); \
				} \
				else \
				{ \
					arm_load_membase((inst), ARM_WORK, ARM_PC, 4); \
					arm_alu_reg_imm8((inst), ARM_ADD, ARM_LINK, ARM_PC, 4); \
					arm_mov_reg_reg((inst), ARM_PC, ARM_WORK); \
					arm_inst_add((inst), (int)(target)); \
				} \
			} while (0)

/*
 * Return from a subroutine, where the return address is in the link register.
 */
#define	arm_return(inst)	\
			do { \
				arm_mov_reg_reg((inst), ARM_PC, ARM_LINK); \
			} while (0)

/*
 * Push a register onto the system stack.
 */
#define	arm_push_reg(inst,reg)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x05200004) | \
							(((unsigned int)ARM_SP) << 16) | \
							(((unsigned int)(reg)) << 12)); \
			} while (0)

/*
 * Pop a register from the system stack.
 */
#define	arm_pop_reg(inst,reg)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x04900004) | \
							(((unsigned int)ARM_SP) << 16) | \
							(((unsigned int)(reg)) << 12)); \
			} while (0)

/*
 * Pop the top of the system stack and put it at a given offset from the position specified by basereg (that is, usually, the frame pointer). NB: This macro thrashes the content of ARM_WORK
 */
#define arm_pop_membase(inst,basereg,offset)	\
			do {	\
				arm_pop_reg((inst), ARM_WORK);	\
				arm_store_membase((inst),ARM_WORK,basereg,offset);	\
			} while (0)
			
/*
 * Set up a local variable frame, and save the registers in "regset".
 */
#define	arm_setup_frame(inst,regset)	\
			do { \
				arm_mov_reg_reg((inst), ARM_WORK, ARM_SP); \
				arm_inst_add((inst), arm_prefix(0x0920D800) | \
							(((unsigned int)ARM_SP) << 16) | \
							(((unsigned int)(regset)))); \
				arm_alu_reg_imm8((inst), ARM_SUB, ARM_FP, ARM_WORK, 4); \
			} while (0)

/*
 * Pop a local variable frame, restore the registers in "regset",
 * and return to the caller.
 */
#define	arm_pop_frame(inst,regset)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x0910A800) | \
							(((unsigned int)ARM_FP) << 16) | \
							(((unsigned int)(regset)))); \
			} while (0)

/*
 * Pop a local variable frame, in preparation for a tail call.
 * This restores "lr" to its original value, but does not set "pc".
 */
#define	arm_pop_frame_tail(inst,regset)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x09106800) | \
							(((unsigned int)ARM_FP) << 16) | \
							(((unsigned int)(regset)))); \
			} while (0)

/*
 * Load a word value from a pointer and then advance the pointer.
 */
#define	arm_load_advance(inst,dreg,sreg)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x04900004) | \
							(((unsigned int)(sreg)) << 16) | \
							(((unsigned int)(dreg)) << 12)); \
			} while (0)

/*
 * Load a value from an address into a register.
 */
#define arm_load_membase_either(inst,reg,basereg,imm,mask)	\
			do { \
				int __mb_offset = (int)(imm); \
				if(__mb_offset >= 0 && __mb_offset < (1 << 12)) \
				{ \
					arm_inst_add((inst), arm_prefix(0x05900000 | (mask)) | \
								(((unsigned int)(basereg)) << 16) | \
								(((unsigned int)(reg)) << 12) | \
								 ((unsigned int)__mb_offset)); \
				} \
				else if(__mb_offset > -(1 << 12) && __mb_offset < 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x05100000 | (mask)) | \
								(((unsigned int)(basereg)) << 16) | \
								(((unsigned int)(reg)) << 12) | \
								 ((unsigned int)(-__mb_offset))); \
				} \
				else \
				{ \
					assert(basereg!=ARM_WORK);	\
					arm_mov_reg_imm((inst), ARM_WORK, __mb_offset); \
					arm_inst_add((inst), arm_prefix(0x07900000 | (mask)) | \
								(((unsigned int)(basereg)) << 16) | \
								(((unsigned int)(reg)) << 12) | \
								 ((unsigned int)ARM_WORK)); \
				} \
			} while (0)
#define	arm_load_membase(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_either((inst), (reg), (basereg), (imm), 0); \
			} while (0)

/**
 * Moves the content of 1 byte (is_half==0) or 2 bytes (is_half==1) from memory address basereg+disp+(indexreg<<shift) into dreg, with sign extension (is_signed==1) or zero extension (is_signed==0)
 */
#define arm_widen_memindex(inst,dreg,basereg,disp,indexreg,shift,is_signed,is_half)	\
do {	\
	int scratchreg=ARM_WORK;	\
	if(is_half)	\
	{	\
		arm_mov_reg_memindex((inst),(dreg),(basereg),(disp),(indexreg),(shift),2, scratchreg);	\
	}	\
	else	\
	{	\
		arm_mov_reg_memindex((inst),(dreg),(basereg),(disp),(indexreg),(shift),1, scratchreg);	\
	}	\
	if(is_signed)	\
	{	\
		int shiftSize;	\
		if (is_half)	\
		{	\
			shiftSize=16;	\
		}	\
		else	\
		{	\
			shiftSize=24;	\
		}	\
		arm_shift_reg_imm8((inst), ARM_SHL, (dreg), (dreg), shiftSize); \
		arm_shift_reg_imm8((inst), ARM_SAR, (dreg), (dreg), shiftSize);	\
	}	\
} while (0)

#define	arm_load_membase_byte(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_either((inst), (reg), (basereg), (imm), \
										0x00400000); \
			} while (0)
#define	arm_load_membase_sbyte(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_either((inst), (reg), (basereg), (imm), \
										0x00400000); \
				arm_shift_reg_imm8((inst), ARM_SHL, (reg), (reg), 24); \
				arm_shift_reg_imm8((inst), ARM_SAR, (reg), (reg), 24); \
			} while (0)
#define	arm_load_membase_ushort(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_byte((inst), ARM_WORK, (basereg), (imm)); \
				arm_load_membase_byte((inst), (reg), (basereg), (imm) + 1); \
				arm_shift_reg_imm8((inst), ARM_SHL, (reg), (reg), 8); \
				arm_alu_reg_reg((inst), ARM_ORR, (reg), (reg), ARM_WORK); \
			} while (0)
#define	arm_load_membase_short(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_byte((inst), ARM_WORK, (basereg), (imm)); \
				arm_load_membase_byte((inst), (reg), (basereg), (imm) + 1); \
				arm_shift_reg_imm8((inst), ARM_SHL, (reg), (reg), 24); \
				arm_shift_reg_imm8((inst), ARM_SAR, (reg), (reg), 16); \
				arm_alu_reg_reg((inst), ARM_ORR, (reg), (reg), ARM_WORK); \
			} while (0)


#ifdef JIT_ARM_HAS_FPA
/*
 * Load a floating-point value from an address into a register.
 */
#define	arm_load_membase_float(inst,reg,basereg,imm,mask)	\
			do { \
				int __mb_offset = (int)(imm); \
				if(__mb_offset >= 0 && __mb_offset < (1 << 10) && \
				   (__mb_offset & 3) == 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x0D900100 | (mask)) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							 ((unsigned int)((__mb_offset / 4) & 0xFF))); \
				} \
				else if(__mb_offset > -(1 << 10) && __mb_offset < 0 && \
				        (__mb_offset & 3) == 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x0D180100 | (mask)) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							 ((unsigned int)(((-__mb_offset) / 4) & 0xFF)));\
				} \
				else \
				{ \
					arm_mov_reg_imm((inst), ARM_WORK, __mb_offset); \
					arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, \
								    (basereg), ARM_WORK); \
					arm_inst_add((inst), arm_prefix(0x0D900100 | (mask)) | \
							(((unsigned int)ARM_WORK) << 16) | \
							(((unsigned int)(reg)) << 12)); \
				} \
			} while (0)
#define	arm_load_membase_float32(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_float((inst), (reg), (basereg), (imm), 0); \
			} while (0)
#define	arm_load_membase_float64(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_float((inst), (reg), (basereg), \
									   (imm), 0x00008000); \
			} while (0)

#endif /* JIT_ARM_HAS_FPA */

#ifdef JIT_ARM_HAS_VFP
/**
 * FLDS (Floating-point Load, Single-precision)
 * Loads a word from memory address basereg+imm to 
 * the single precision floating point register reg.
 * "mask" is usually set to 0
 */
#define	arm_load_membase_float(inst,reg,basereg,imm,mask)	\
do { \
	unsigned int reg_top_4_bits = (reg & 0x1E) >> 1;	\
	unsigned int reg_bottom_bit = (reg & 0x01);	\
	int __mb_offset = (int)(imm); \
	if(__mb_offset >= 0 && __mb_offset < (1 << 10) && \
		(__mb_offset & 3) == 0) \
	{ \
		arm_inst_add((inst), arm_prefix(0x0D900A00 | (mask)) | \
			(((unsigned int)(basereg)) << 16) | \
			(((unsigned int)(reg_top_4_bits)) << 12) | \
			(((unsigned int)(reg_bottom_bit)) << 22) | \
			((unsigned int)((__mb_offset / 4) & 0xFF))); \
	} \
	else if(__mb_offset > -(1 << 10) && __mb_offset < 0 && \
		(__mb_offset & 3) == 0) \
	{ \
		arm_inst_add((inst), arm_prefix(0x0D100A00 | (mask)) | \
			(((unsigned int)(basereg)) << 16) | \
			(((unsigned int)(reg_top_4_bits)) << 12) | \
			(((unsigned int)(reg_bottom_bit)) << 22) | \
			((unsigned int)(((-__mb_offset) / 4) & 0xFF)));\
	} \
	else \
	{ \
		assert(reg != ARM_WORK);	\
		assert(basereg!=ARM_WORK);	\
		if(__mb_offset > 0)	\
		{	\
			arm_mov_reg_imm((inst), ARM_WORK, __mb_offset);	\
			arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, (basereg), ARM_WORK);	\
		}	\
		else	\
		{	\
			arm_mov_reg_imm((inst), ARM_WORK, -__mb_offset);	\
			arm_alu_reg_reg((inst), ARM_SUB, ARM_WORK, (basereg), ARM_WORK);	\
		}	\
		arm_inst_add((inst), arm_prefix(0x0D900A00 | (mask)) | \
			(((unsigned int)ARM_WORK) << 16) | \
			(((unsigned int)(reg_top_4_bits)) << 12) | \
			(((unsigned int)(reg_bottom_bit)) << 22)); \
	} \
} while (0)

/**
 * FLDD
 */
#define	arm_load_membase_float64(inst,reg,basereg,imm)	\
			do { \
				int __mb_offset = (int)(imm); \
				if(__mb_offset >= 0 && __mb_offset < (1 << 10) && \
				   (__mb_offset & 3) == 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x0D900B00) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							 ((unsigned int)((__mb_offset / 4) & 0xFF))); \
				} \
				else if(__mb_offset > -(1 << 10) && __mb_offset < 0 && \
				        (__mb_offset & 3) == 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x0D100B00) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							 ((unsigned int)(((-__mb_offset) / 4) & 0xFF)));\
				} \
				else \
				{ \
					assert(reg != ARM_WORK);	\
					assert(basereg!=ARM_WORK);	\
					if(__mb_offset > 0)	\
					{	\
						arm_mov_reg_imm((inst), ARM_WORK, __mb_offset);	\
						arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, (basereg), ARM_WORK);	\
					}	\
					else	\
					{	\
						arm_mov_reg_imm((inst), ARM_WORK, -__mb_offset);	\
						arm_alu_reg_reg((inst), ARM_SUB, ARM_WORK, (basereg), ARM_WORK);	\
					}	\
					arm_inst_add((inst), arm_prefix(0x0D900B00) | \
							(((unsigned int)ARM_WORK) << 16) | \
							(((unsigned int)(reg)) << 12)); \
				} \
			} while (0)

#define	arm_load_membase_float32(inst,reg,basereg,imm)	\
			do { \
				arm_load_membase_float((inst), (reg), (basereg), (imm), 0); \
			} while (0)

/**
* Load the content of the memory area at position basereg+disp into the float register "dfreg",
* using the appropriate instruction depending whether the value to be loaded is_double (1 => 64 bits) or not (0 => 32 bits)
* (it's similar to x86_fld_membase)
*/
#define arm_fld_membase(inst,dfreg,basereg,disp,is_double)	\
do {	\
	if (is_double)	\
	{	\
		arm_load_membase_float64((inst), (dfreg), (basereg), (disp));	\
	}	\
	else	\
	{	\
		arm_load_membase_float32((inst), (dfreg), (basereg), (disp));	\
	}\
} while(0)

/**
 * Load the content of the memory area at position basereg+disp+(indexreg<<shift) into the float register "dfreg",
 * using the appropriate instruction depending whether the value to be loaded is_double (1 => 64 bits) or not (0 => 32 bits)
 * (it's similar to x86_fld_memindex)
 */
#define arm_fld_memindex(inst,dfreg,basereg,disp,indexreg,shift,is_double,scratchreg) \
	do {	\
		if (is_double)	\
		{	\
			arm_load_memindex_float64((inst), (dfreg), (basereg), (disp), (indexreg), (shift), (scratchreg));	\
		}	\
		else	\
		{	\
			arm_load_memindex_float32((inst), (dfreg), (basereg), (disp), (indexreg), (shift), (scratchreg));	\
		}\
	} while(0)
/**
 * Load the content of the 64-bits memory area at position basereg+disp+(indexreg<<shift) into the double register "dfreg"
 * NB: the scratch register has to be asked to the register allocator.
 *     It can't be ARM_WORK, since it's already used
 */
#define arm_load_memindex_float64(inst,dfreg,basereg,disp,indexreg,shift,scratchreg)	\
	do {	\
		arm_shift_reg_imm8((inst), ARM_SHL, ARM_WORK, (indexreg), (shift));	\
		arm_alu_reg_reg((inst), ARM_ADD, (scratchreg), (basereg), ARM_WORK);	\
		arm_load_membase_float64((inst), (dfreg), (scratchreg), (disp));	\
	} while (0)
	
/**
 * Load the content of the 32-bits memory area at position basereg+disp+(indexreg<<shift) into the single float register "dfreg"
 * NB: the scratch register has to be asked to the register allocator.
 *     It can't be ARM_WORK, since it's already used
 */
#define arm_load_memindex_float32(inst,dfreg,basereg,disp,indexreg,shift,scratchreg)	\
do {	\
	arm_shift_reg_imm8((inst), ARM_SHL, ARM_WORK, (indexreg), (shift));	\
	arm_alu_reg_reg((inst), ARM_ADD, (scratchreg), (basereg), ARM_WORK);	\
	arm_load_membase_float32((inst), (dfreg), (scratchreg), (disp));	\
	} while (0)

/**
 * Store the content of the float register "sfreg" into the memory area at position basereg+disp+(indexreg<<shift)
 * using the appropriate instruction depending whether the value to be loaded is_double (1 => 64 bits) or not (0 => 32 bits)
 * (it's similar to x86_fst_memindex)
 */
#define arm_fst_memindex(inst,sfreg,basereg,disp,indexreg,shift,is_double,scratchreg) \
do {	\
	if (is_double)	\
	{	\
		arm_store_memindex_float64((inst), (sfreg), (basereg), (disp), (indexreg), (shift), (scratchreg));	\
	}	\
	else	\
	{	\
		arm_store_memindex_float32((inst), (sfreg), (basereg), (disp), (indexreg), (shift), (scratchreg));	\
	}\
} while(0)

/**
 * Store the content of the double float register "dfreg" into the 64-bits memory area at position basereg+disp+(indexreg<<shift)
 * NB: the scratch register has to be asked to the register allocator.
 *     It can't be ARM_WORK, since it's already used
 */
#define arm_store_memindex_float64(inst,dfreg,basereg,disp,indexreg,shift,scratchreg)	\
do {	\
	arm_shift_reg_imm8((inst), ARM_SHL, ARM_WORK, (indexreg), (shift));	\
	arm_alu_reg_reg((inst), ARM_ADD, (scratchreg), (basereg), ARM_WORK);	\
	arm_store_membase_float64((inst), (dfreg), (scratchreg), (disp));	\
} while (0)

/**
 * Store the content of the single float register "dfreg" into the 32-bits memory area at position basereg+disp+(indexreg<<shift)
 * NB: the scratch register has to be asked to the register allocator.
 *     It can't be ARM_WORK, since it's already used
 */
#define arm_store_memindex_float32(inst,dfreg,basereg,disp,indexreg,shift,scratchreg)	\
do {	\
	arm_shift_reg_imm8((inst), ARM_SHL, ARM_WORK, (indexreg), (shift));	\
	arm_alu_reg_reg((inst), ARM_ADD, (scratchreg), (basereg), ARM_WORK);	\
	arm_store_membase_float32((inst), (dfreg), (scratchreg), (disp));	\
} while (0)

#endif /* JIT_ARM_HAS_VFP */

/**
 * Store a value from a register (reg) into an address (basereg+imm).
 *
 */
#define arm_store_membase_either(inst,reg,basereg,imm,mask)	\
			do { \
				int __sm_offset = (int)(imm); \
				if(__sm_offset >= 0 && __sm_offset < (1 << 12)) \
				{ \
					arm_inst_add((inst), arm_prefix(0x05800000 | (mask)) | \
								(((unsigned int)(basereg)) << 16) | \
								(((unsigned int)(reg)) << 12) | \
								 ((unsigned int)__sm_offset)); \
				} \
				else if(__sm_offset > -(1 << 12) && __sm_offset < 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x05000000 | (mask)) | \
								(((unsigned int)(basereg)) << 16) | \
								(((unsigned int)(reg)) << 12) | \
								 ((unsigned int)(-__sm_offset))); \
				} \
				else \
				{ \
					assert(reg != ARM_WORK);	\
					assert(basereg!=ARM_WORK);	\
					arm_mov_reg_imm((inst), ARM_WORK, __sm_offset); \
					arm_inst_add((inst), arm_prefix(0x07800000 | (mask)) | \
								(((unsigned int)(basereg)) << 16) | \
								(((unsigned int)(reg)) << 12) | \
								 ((unsigned int)ARM_WORK)); \
				} \
			} while (0)

/*
 * The ARM STR instruction. The content of "reg" will be put in memory at the address given by the content of basereg + imm
 */
#define	arm_store_membase(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_either((inst), (reg), (basereg), (imm), 0); \
			} while (0)
#define	arm_store_membase_byte(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_either((inst), (reg), (basereg), (imm), \
										 0x00400000); \
			} while (0)
#define	arm_store_membase_sbyte(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_byte((inst), (reg), (basereg), (imm)); \
			} while (0)
#define	arm_store_membase_short(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_either((inst), (reg), (basereg), (imm), \
										 0x00400000); \
				arm_shift_reg_imm8((inst), ARM_SHR, (reg), (reg), 8); \
				arm_store_membase_either((inst), (reg), (basereg), \
										 (imm) + 1, 0x00400000); \
			} while (0)
#define	arm_store_membase_ushort(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_short((inst), (reg), (basereg), (imm)); \
			} while (0)

#ifdef JIT_ARM_HAS_FPA
/*
 * Store a floating-point value to a memory address.
 */
#define	arm_store_membase_float(inst,reg,basereg,imm,mask)	\
			do { \
				int __mb_offset = (int)(imm); \
				if(__mb_offset >= 0 && __mb_offset < (1 << 10) && \
				   (__mb_offset & 3) == 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x0D800100 | (mask)) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							 ((unsigned int)((__mb_offset / 4) & 0xFF))); \
				} \
				else if(__mb_offset > -(1 << 10) && __mb_offset < 0 && \
				        (__mb_offset & 3) == 0) \
				{ \
					arm_inst_add((inst), arm_prefix(0x0D080100 | (mask)) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							 ((unsigned int)(((-__mb_offset) / 4) & 0xFF)));\
				} \
				else \
				{ \
					arm_mov_reg_imm((inst), ARM_WORK, __mb_offset); \
					arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, \
								    (basereg), ARM_WORK); \
					arm_inst_add((inst), arm_prefix(0x0D800100 | (mask)) | \
							(((unsigned int)ARM_WORK) << 16) | \
							(((unsigned int)(reg)) << 12)); \
				} \
			} while (0)
#define	arm_store_membase_float32(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_float((inst), (reg), (basereg), (imm), 0); \
			} while (0)
#define	arm_store_membase_float64(inst,reg,basereg,imm)	\
			do { \
				arm_store_membase_float((inst), (reg), (basereg), \
									    (imm), 0x00008000); \
			} while (0)
#define	arm_push_reg_float32(inst,reg)	\
			do { \
				arm_store_membase_float((inst), (reg), ARM_SP, \
									    -4, 0x00200000); \
			} while (0)
#define	arm_push_reg_float64(inst,reg)	\
			do { \
				arm_store_membase_float((inst), (reg), ARM_SP, \
									    -4, 0x00208000); \
			} while (0)

#endif /* JIT_ARM_HAS_FPA */

#ifdef JIT_ARM_HAS_VFP
/**
 * FSTS
 * Store a floating-point value to a memory address.
 */
#define arm_store_membase_float32(inst,reg,basereg,imm)	\
do { \
	unsigned int reg_top_4_bits = (reg & 0x1E) >> 1;	\
	unsigned int reg_bottom_bit = (reg & 0x01);	\
	int __mb_offset = (int)(imm);	\
	if(__mb_offset >= 0 && __mb_offset < (1 << 10) && (__mb_offset & 3) == 0)	\
	{	\
		arm_inst_add((inst), arm_prefix(0x0D800A00) |	\
			(((unsigned int)(basereg)) << 16) | 	\
			(((unsigned int)(reg_top_4_bits)) << 12) |	\
			(((unsigned int)(reg_bottom_bit)) << 22) |	\
			((unsigned int)((__mb_offset / 4) & 0xFF)));	\
	}	\
	else if(__mb_offset > -(1 << 10) && __mb_offset < 0 && (__mb_offset & 3) == 0)	\
	{	\
		arm_inst_add((inst), arm_prefix(0x0D000A00) |	\
			(((unsigned int)(basereg)) << 16) |	\
			(((unsigned int)(reg_top_4_bits)) << 12) |	\
			(((unsigned int)(reg_bottom_bit)) << 22) |	\
			((unsigned int)(((-__mb_offset) / 4) & 0xFF)));	\
	}	\
	else	\
	{ \
		assert(reg != ARM_WORK);	\
		assert(basereg!=ARM_WORK);	\
		if(__mb_offset > 0)	\
		{	\
			arm_mov_reg_imm((inst), ARM_WORK, __mb_offset); \
			arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, (basereg), ARM_WORK); \
		}	\
		else	\
		{	\
			arm_mov_reg_imm((inst), ARM_WORK, -__mb_offset); \
			arm_alu_reg_reg((inst), ARM_SUB, ARM_WORK, (basereg), ARM_WORK); \
		}	\
		arm_inst_add((inst), arm_prefix(0x0D800A00) |	\
			(((unsigned int)ARM_WORK) << 16) | 	\
			(((unsigned int)(reg_top_4_bits)) << 12) |	\
			(((unsigned int)(reg_bottom_bit)) << 22));	\
	} \
} while (0)

/**
* FSTD
*/
#define	arm_store_membase_float64(inst,reg,basereg,imm)	\
do { \
	int __mb_offset = (int)(imm); \
	if(__mb_offset >= 0 && __mb_offset < (1 << 10) && \
		(__mb_offset & 3) == 0) \
	{ \
		arm_inst_add((inst), arm_prefix(0x0D800B00 |	\
			(((unsigned int)(basereg)) << 16) | \
			(((unsigned int)(reg)) << 12) | 	\
			((unsigned int)((__mb_offset / 4) & 0xFF)))); \
	} \
	else if(__mb_offset > -(1 << 10) && __mb_offset < 0 && \
		(__mb_offset & 3) == 0) \
	{ \
		arm_inst_add((inst), arm_prefix(0x0D000B00 |	\
			(((unsigned int)(basereg)) << 16) | \
			(((unsigned int)(reg)) << 12) | 	\
			((unsigned int)(((-__mb_offset) / 4) & 0xFF))));\
	} \
	else \
	{ \
		assert(reg != ARM_WORK);	\
		assert(basereg!=ARM_WORK);	\
		if(__mb_offset > 0)	\
		{	\
			arm_mov_reg_imm((inst), ARM_WORK, __mb_offset);	\
			arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, (basereg), ARM_WORK);	\
		}	\
		else	\
		{	\
			arm_mov_reg_imm((inst), ARM_WORK, -__mb_offset);	\
			arm_alu_reg_reg((inst), ARM_SUB, ARM_WORK, (basereg), ARM_WORK);\
		}	\
		arm_inst_add((inst), arm_prefix(0x0D800B00 |	\
			(((unsigned int)ARM_WORK) << 16) | \
			(((unsigned int)(reg)) << 12)));\
	} \
} while (0)

/*
 * Floating point push/pop operations
 */
#define	arm_push_reg_float64(inst,reg)	\
			do { \
				arm_store_membase_float64((inst), (reg), ARM_SP, -8); \
				arm_alu_reg_imm(inst, ARM_SUB, ARM_SP, ARM_SP, 8);	\
			} while (0)
			
#define	arm_push_reg_float32(inst,reg)	\
			do { \
				arm_store_membase_float32((inst), (reg), ARM_SP, -4); \
				arm_alu_reg_imm(inst, ARM_SUB, ARM_SP, ARM_SP, 4);	\
			} while (0)

/**
 * FMDRR (Floating-point Move to Double-precision Register from two Registers)
 * Move a value from two ARM registers (lowsreg, highsreg) to a double-precision floating point register (dreg)
 */
#define arm_mov_double_reg_reg(inst,dreg,lowsreg,highsreg)	\
do { \
	arm_inst_add((inst), arm_prefix(0x0C400B10) | \
	(((unsigned int)(lowsreg)) << 12) | \
	(((unsigned int)(highsreg)) << 16) | \
	((unsigned int)(dreg))); \
} while(0)

/**
* FMRRD (Floating-point Move to two registers from Double-precision Register)
* Move a value from a double-precision floating point register (sreg) to two ARM registers (lowsreg, highsreg) 
*/
#define arm_mov_reg_reg_double(inst,lowsreg,highsreg,sreg)	\
do { \
	arm_inst_add((inst), arm_prefix(0x0C500B10) | \
	(((unsigned int)(lowsreg)) << 12) | \
	(((unsigned int)(highsreg)) << 16) | \
	((unsigned int)(sreg))); \
} while(0)

/**
 * FMSR (Floating-point Move to Single-precision from Register)
 * Move a value from one ARM registers (sreg) to a single-precision floating point register (dreg)
 */
#define arm_mov_float_reg(inst,dreg,sreg)	\
do { \
	char dreg_top_4_bits = (dreg & 0x1E) >> 1;	\
	char dreg_bottom_bit = (dreg & 0x01);	\
	arm_inst_add((inst), arm_prefix(0x0E000A10) | 	\
	(((unsigned int)(sreg)) << 12) | 	\
	(((unsigned int)(dreg_top_4_bits)) << 16) |	\
	(((unsigned int)(dreg_bottom_bit)) << 7)); \
} while(0)

/**
* FMRS (Floating-point Move to Register from Single-precision)
* Move a value from a single-precision floating point register (sreg) to an ARM registers (dreg) 
*/
#define arm_mov_reg_float(inst,dreg,sreg)	\
do { \
	char sreg_top_4_bits = (sreg & 0x1E) >> 1;	\
	char sreg_bottom_bit = (sreg & 0x01);	\
	arm_inst_add((inst), arm_prefix(0x0E100A10) | 	\
	(((unsigned int)(dreg)) << 12) | 	\
	(((unsigned int)(sreg_top_4_bits)) << 16) |	\
	(((unsigned int)(sreg_bottom_bit)) << 7)); \
} while(0)

/**
* FCVTDS (Floating-point Convert to Double-precision from Single-precision)
* dreg is the double precision destination register
* sreg is the single precision source register
*/
#define arm_convert_float_double_single(inst,dreg,sreg)	\
{	\
	unsigned char sreg_top_4_bits = (sreg & 0x1E) >> 1;	\
	unsigned char sreg_bottom_bit = (sreg & 0x01);	\
	arm_inst_add((inst), arm_prefix(0x0EB70AC0) |	\
		(((unsigned int)(sreg_top_4_bits))) |	\
		(((unsigned int)(sreg_bottom_bit)) << 5) |	\
		(((unsigned int)(dreg)) << 12));	\
}

/**
 * FCVTSD (Floating-point Convert to Single-precision from Double-precision)
 * dreg is the single precision destination register
 * sreg is the double precision source register
 */
#define arm_convert_float_single_double(inst,dreg,sreg)	\
{	\
	unsigned char dreg_top_4_bits = (dreg & 0x1E) >> 1;	\
	unsigned char dreg_bottom_bit = (dreg & 0x01);	\
	arm_inst_add((inst), arm_prefix(0x0EB70BC0) |	\
		(((unsigned int)(dreg_top_4_bits)) << 12) |	\
		(((unsigned int)(dreg_bottom_bit)) << 22) |	\
		((unsigned int)(sreg)));	\
}

/**
 * FSITOD (Floating-point Convert Signed Integer to Double-precision)
 * sreg is the single precision register containing the integer value to be converted
 * dreg is the double precision destination register
 */
#define arm_convert_float_signed_integer_double(inst,dreg,sreg)	\
	unsigned char sreg_top_4_bits = (sreg & 0x1E) >> 1;	\
	unsigned char sreg_bottom_bit = (sreg & 0x01);	\
	arm_inst_add((inst), arm_prefix(0x0EB80BC0) |	\
		(((unsigned int)(dreg)) << 12) |	\
		(((unsigned int)(sreg_bottom_bit)) << 5) |	\
		((unsigned int)(sreg_top_4_bits)));

#endif /* JIT_ARM_HAS_VFP */

/*
 * Load a value from an indexed address into a register.
 */
#define arm_load_memindex_either(inst,reg,basereg,indexreg,shift,mask)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x07900000 | (mask)) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							(((unsigned int)(shift)) << 7) | \
							 ((unsigned int)(indexreg))); \
			} while (0)
#define	arm_load_memindex(inst,reg,basereg,indexreg)	\
			do { \
				arm_load_memindex_either((inst), (reg), (basereg), \
										 (indexreg), 2, 0); \
			} while (0)
#define	arm_load_memindex_byte(inst,reg,basereg,indexreg)	\
			do { \
				arm_load_memindex_either((inst), (reg), (basereg), \
									     (indexreg), 0, 0x00400000); \
			} while (0)
#define	arm_load_memindex_sbyte(inst,reg,basereg,indexreg)	\
			do { \
				arm_load_memindex_either((inst), (reg), (basereg), \
									     (indexreg), 0, 0x00400000); \
				arm_shift_reg_imm8((inst), ARM_SHL, (reg), (reg), 24); \
				arm_shift_reg_imm8((inst), ARM_SAR, (reg), (reg), 24); \
			} while (0)
#define	arm_load_memindex_ushort(inst,reg,basereg,indexreg)	\
			do { \
				arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, (basereg), \
								(indexreg)); \
				arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, ARM_WORK, \
								(indexreg)); \
				arm_load_membase_byte((inst), (reg), ARM_WORK, 0); \
				arm_load_membase_byte((inst), ARM_WORK, ARM_WORK, 1); \
				arm_shift_reg_imm8((inst), ARM_SHL, ARM_WORK, ARM_WORK, 8); \
				arm_alu_reg_reg((inst), ARM_ORR, (reg), (reg), ARM_WORK); \
			} while (0)
#define	arm_load_memindex_short(inst,reg,basereg,indexreg)	\
			do { \
				arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, (basereg), \
								(indexreg)); \
				arm_alu_reg_reg((inst), ARM_ADD, ARM_WORK, ARM_WORK, \
								(indexreg)); \
				arm_load_membase_byte((inst), (reg), ARM_WORK, 0); \
				arm_load_membase_byte((inst), ARM_WORK, ARM_WORK, 1); \
				arm_shift_reg_imm8((inst), ARM_SHL, ARM_WORK, ARM_WORK, 24); \
				arm_shift_reg_imm8((inst), ARM_SAR, ARM_WORK, ARM_WORK, 16); \
				arm_alu_reg_reg((inst), ARM_ORR, (reg), (reg), ARM_WORK); \
			} while (0)

/*
 * Store a value from a register into an indexed address.
 *
 * Note: storing a 16-bit value destroys the values in the base
 * register and the source register.
 */
#define arm_store_memindex_either(inst,reg,basereg,indexreg,shift,mask)	\
			do { \
				arm_inst_add((inst), arm_prefix(0x07800000 | (mask)) | \
							(((unsigned int)(basereg)) << 16) | \
							(((unsigned int)(reg)) << 12) | \
							(((unsigned int)(shift)) << 7) | \
							 ((unsigned int)(indexreg))); \
			} while (0)
#define	arm_store_memindex(inst,reg,basereg,indexreg)	\
			do { \
				arm_store_memindex_either((inst), (reg), (basereg), \
										  (indexreg), 2, 0); \
			} while (0)
#define	arm_store_memindex_byte(inst,reg,basereg,indexreg)	\
			do { \
				arm_store_memindex_either((inst), (reg), (basereg), \
										  (indexreg), 0, 0x00400000); \
			} while (0)
#define	arm_store_memindex_sbyte(inst,reg,basereg,indexreg)	\
			do { \
				arm_store_memindex_byte((inst), (reg), (basereg), \
										(indexreg)); \
			} while (0)
#define	arm_store_memindex_short(inst,reg,basereg,indexreg)	\
			do { \
				arm_store_memindex_either((inst), (reg), (basereg), \
										  (indexreg), 1, 0x00400000); \
				arm_alu_reg_imm8((inst), ARM_ADD, (basereg), (basereg), 1); \
				arm_shift_reg_imm8((inst), ARM_SHR, (reg), (reg), 8); \
				arm_store_memindex_either((inst), (reg), (basereg), \
										  (indexreg), 1, 0x00400000); \
			} while (0)
#define	arm_store_memindex_ushort(inst,reg,basereg,indexreg)	\
			do { \
				arm_store_memindex_short((inst), (reg), \
										 (basereg), (indexreg)); \
			} while (0)

#ifdef __cplusplus
};
#endif

#endif /* _ARM_CODEGEN_H */
