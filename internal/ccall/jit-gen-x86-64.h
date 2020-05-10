/*
 * jit-gen-x86-64.h - Macros for generating x86_64 code.
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

#ifndef	_JIT_GEN_X86_64_H
#define	_JIT_GEN_X86_64_H

#include <jit/jit-defs.h>
#include "jit-gen-x86.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * X86_64 64 bit general purpose integer registers.
 */
typedef enum
{
	X86_64_RAX = 0,
	X86_64_RCX = 1,
	X86_64_RDX = 2,
	X86_64_RBX = 3,
	X86_64_RSP = 4,
	X86_64_RBP = 5,
	X86_64_RSI = 6,
	X86_64_RDI = 7,
	X86_64_R8  = 8,
	X86_64_R9  = 9,
	X86_64_R10 = 10,
	X86_64_R11 = 11,
	X86_64_R12 = 12,
	X86_64_R13 = 13,
	X86_64_R14 = 14,
	X86_64_R15 = 15,
	X86_64_RIP = 16		/* This register encoding doesn't exist in the */
						/* instructions. It's used for RIP relative encoding. */
} X86_64_Reg_No;

/*
 * X86-64 xmm registers.
 */
typedef enum
{
	X86_64_XMM0 = 0,
	X86_64_XMM1 = 1,
	X86_64_XMM2 = 2,
	X86_64_XMM3 = 3,
	X86_64_XMM4 = 4,
	X86_64_XMM5 = 5,
	X86_64_XMM6 = 6,
	X86_64_XMM7 = 7,
	X86_64_XMM8 = 8,
	X86_64_XMM9 = 9,
	X86_64_XMM10 = 10,
	X86_64_XMM11 = 11,
	X86_64_XMM12 = 12,
	X86_64_XMM13 = 13,
	X86_64_XMM14 = 14,
	X86_64_XMM15 = 15
} X86_64_XMM_Reg_No;

/*
 * Bits in the REX prefix byte.
 */
typedef enum
{
	X86_64_REX_B = 1,	/* 1-bit (high) extension of the ModRM r/m field */
						/* SIB base field, or opcode reg field, thus */
						/* permitting access to 16 registers. */
	X86_64_REX_X = 2,	/* 1-bit (high) extension of the SIB index field */
						/* thus permitting access to 16 registers. */
	X86_64_REX_R = 4,	/* 1-bit (high) extension of the ModRM reg field, */
						/* thus permitting access to 16 registers. */
	X86_64_REX_W = 8	/* 0 = Default operand size */
						/* 1 = 64 bit operand size */
} X86_64_REX_Bits;

/*
 * Third part of the opcodes for xmm instructions which are encoded
 * Opcode1: 0xF3 (single precision) or 0xF2 (double precision)
 *          This is handled as a prefix.
 * Opcode2: 0x0F
 */
typedef enum
{
	XMM1_MOV		= 0x10,
	XMM1_MOV_REV	= 0x11,
	XMM1_ADD		= 0x58,
	XMM1_MUL		= 0x59,
	XMM1_SUB		= 0x5C,
	XMM1_DIV		= 0x5E
} X86_64_XMM1_OP;

/*
 * Logical opcodes used with packed single and double precision values.
 */
typedef enum
{
	XMM_ANDP		= 0x54,
	XMM_ORP			= 0x56,
	XMM_XORP		= 0x57
} X86_64_XMM_PLOP;

/*
 * Rounding modes for xmm rounding instructions, the mxcsr register and
 * the fpu control word.
 */
typedef enum
{
	X86_ROUND_NEAREST	= 0x00,		/* Round to the nearest integer */
	X86_ROUND_DOWN		= 0x01,		/* Round towards negative infinity */
	X86_ROUND_UP		= 0x02,		/* Round towards positive infinity */
	X86_ROUND_ZERO		= 0x03		/* Round towards zero (truncate) */
} X86_64_ROUNDMODE;

/*
 * Helper union for emmitting 64 bit immediate values.
 */
typedef union
{
	jit_long val;
	unsigned char b[8];
} x86_64_imm_buf;

#define x86_64_imm_emit64(inst, imm) \
	do { \
		x86_64_imm_buf imb; \
		imb.val = (jit_long)(imm); \
		*(inst)++ = imb.b[0]; \
		*(inst)++ = imb.b[1]; \
		*(inst)++ = imb.b[2]; \
		*(inst)++ = imb.b[3]; \
		*(inst)++ = imb.b[4]; \
		*(inst)++ = imb.b[5]; \
		*(inst)++ = imb.b[6]; \
		*(inst)++ = imb.b[7]; \
	} while (0)

#define x86_64_imm_emit_max32(inst, imm, size) \
	do { \
		switch((size)) \
		{ \
			case 1: \
			{ \
				x86_imm_emit8(inst, (imm)); \
			} \
			break; \
			case 2: \
			{ \
				x86_imm_emit16(inst, (imm)); \
			} \
			break; \
			case 4: \
			case 8: \
			{ \
				x86_imm_emit32((inst), (imm)); \
			} \
			break; \
			default: \
			{ \
				jit_assert(0); \
			} \
		} \
	} while(0)

#define x86_64_imm_emit_max64(inst, imm, size) \
	do { \
		switch((size)) \
		{ \
			case 1: \
			{ \
				x86_imm_emit8(inst, (imm)); \
			} \
			break; \
			case 2: \
			{ \
				x86_imm_emit16(inst, (imm)); \
			} \
			break; \
			case 4: \
			{ \
				x86_imm_emit32((inst), (imm)); \
			} \
			break; \
			case 8: \
			{ \
				x86_64_imm_emit64(inst, (imm)); \
			} \
			break; \
			default: \
			{ \
				jit_assert(0); \
			} \
		} \
	} while(0)

/*
 * Emit the Rex prefix.
 * The natural size is a power of 2 (1, 2, 4 or 8).
 * For accessing the low byte registers DIL, SIL, BPL and SPL we have to
 * generate a Rex prefix with the value 0x40 too.
 * To enable this OR the natural size with 1.
 */
#define x86_64_rex(rex_bits)	(0x40 | (rex_bits))
#define x86_64_rex_emit(inst, width, modrm_reg, index_reg, rm_base_opcode_reg) \
	do { \
		unsigned char __rex_bits = \
			(((width) & 8) ? X86_64_REX_W : 0) | \
			(((modrm_reg) & 8) ? X86_64_REX_R : 0) | \
			(((index_reg) & 8) ? X86_64_REX_X : 0) | \
			(((rm_base_opcode_reg) & 8) ? X86_64_REX_B : 0); \
		if((__rex_bits != 0)) \
		{ \
			 *(inst)++ = x86_64_rex(__rex_bits); \
		} \
		else if(((width) & 1) && ((modrm_reg & 4) || (rm_base_opcode_reg & 4))) \
		{ \
			 *(inst)++ = x86_64_rex(0); \
		} \
	} while(0)

/*
 * Helper for emitting the rex prefix for opcodes with 64bit default size.
 */
#define x86_64_rex_emit64(inst, width, modrm_reg, index_reg, rm_base_opcode_reg) \
	do { \
		x86_64_rex_emit((inst), 0, (modrm_reg), (index_reg), (rm_base_opcode_reg)); \
	} while(0)

/* In 64 bit mode, all registers have a low byte subregister */
#undef X86_IS_BYTE_REG
#define X86_IS_BYTE_REG(reg) 1

#define x86_64_reg_emit(inst, r, regno) \
	do { \
		x86_reg_emit((inst), ((r) & 0x7), ((regno) & 0x7)); \
	} while(0)

#define x86_64_mem_emit(inst, r, disp) \
	do { \
		x86_address_byte ((inst), 0, ((r) & 0x7), 4); \
		x86_address_byte ((inst), 0, 4, 5); \
		x86_imm_emit32((inst), (disp)); \
	} while(0)

#define x86_64_mem64_emit(inst, r, disp) \
	do { \
		x86_address_byte ((inst), 0, ((r) & 0x7), 4); \
		x86_address_byte ((inst), 0, 4, 5); \
		x86_64_imm_emit64((inst), (disp)); \
	} while(0)

#define x86_64_membase_emit(inst, reg, basereg, disp) \
	do { \
		if((basereg) == X86_64_RIP) \
		{ \
			x86_address_byte((inst), 0, ((reg) & 0x7), 5); \
			x86_imm_emit32((inst), (disp)); \
		} \
		else \
		{ \
			x86_membase_emit((inst), ((reg) & 0x7), ((basereg) & 0x7), (disp)); \
		} \
	} while(0)

#define x86_64_memindex_emit(inst, r, basereg, disp, indexreg, shift) \
	do { \
		x86_memindex_emit((inst), ((r) & 0x7), ((basereg) & 0x7), (disp), ((indexreg) & 0x7), (shift)); \
	} while(0)

/*
 * RSP, RBP and the corresponding upper registers (R12 and R13) can't be used
 * for relative addressing without displacement because their codes are used
 * for encoding addressing modes with diplacement.
 * So we do a membase addressing in this case with a zero offset.
 */
#define x86_64_regp_emit(inst, r, regno) \
	do { \
		switch(regno) \
		{ \
			case X86_64_RSP: \
			case X86_64_RBP: \
			case X86_64_R12: \
			case X86_64_R13: \
			{ \
				x86_64_membase_emit((inst), (r), (regno), 0); \
			} \
			break; \
			default: \
			{ \
				x86_address_byte((inst), 0, ((r) & 0x7), ((regno) & 0x7)); \
			} \
			break; \
		} \
	} while(0)

/*
 * Helper to encode an opcode where the encoding is different between
 * 8bit and 16 ... 64 bit width in the following way:
 * 8 bit == opcode given
 * 16 ... 64 bit = opcode given | 0x1
 */
#define x86_64_opcode1_emit(inst, opc, size) \
	do { \
		switch ((size)) \
		{ \
			case 1: \
			{ \
				*(inst)++ = (unsigned char)(opc); \
			} \
			break;	\
			case 2: \
			case 4: \
			case 8: \
			{ \
				*(inst)++ = ((unsigned char)(opc) | 0x1); \
			} \
			break;\
			default: \
			{ \
				jit_assert(0); \
			} \
		} \
	} while(0)

/*
 * Macros to implement the simple opcodes.
 */
#define x86_64_alu_reg_reg_size(inst, opc, dreg, sreg, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, (sreg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 2; \
				x86_64_reg_emit((inst), (dreg), (sreg)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, (sreg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 3; \
				x86_64_reg_emit((inst), (dreg), (sreg)); \
			} \
		} \
	} while(0)

#define x86_64_alu_regp_reg_size(inst, opc, dregp, sreg, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), 0, (dregp)); \
				*(inst)++ = (((unsigned char)(opc)) << 3); \
				x86_64_regp_emit((inst), (sreg), (dregp));	\
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), 0, (dregp)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 1; \
				x86_64_regp_emit((inst), (sreg), (dregp));	\
			} \
		} \
	} while(0)

#define x86_64_alu_mem_reg_size(inst, opc, mem, sreg, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), 0, 0); \
				*(inst)++ = (((unsigned char)(opc)) << 3); \
				x86_64_mem_emit((inst), (sreg), (mem));	\
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), 0, 0); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 1; \
				x86_64_mem_emit((inst), (sreg), (mem));	\
			} \
		} \
	} while(0)

#define x86_64_alu_membase_reg_size(inst, opc, basereg, disp, sreg, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), 0, (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3); \
				x86_64_membase_emit((inst), (sreg), (basereg), (disp)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), 0, (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 1; \
				x86_64_membase_emit((inst), (sreg), (basereg), (disp)); \
			} \
		} \
	} while(0)

#define x86_64_alu_memindex_reg_size(inst, opc, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), (indexreg), (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3); \
				x86_64_memindex_emit((inst), (sreg), (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (sreg), (indexreg), (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 1; \
				x86_64_memindex_emit((inst), (sreg), (basereg), (disp), (indexreg), (shift)); \
			} \
		} \
	} while(0)

#define x86_64_alu_reg_regp_size(inst, opc, dreg, sregp, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, (sregp)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 2; \
				x86_64_regp_emit((inst), (dreg), (sregp));	\
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, (sregp)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 3; \
				x86_64_regp_emit((inst), (dreg), (sregp));	\
			} \
		} \
	} while(0)

#define x86_64_alu_reg_mem_size(inst, opc, dreg, mem, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, 0); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 2; \
				x86_64_mem_emit((inst), (dreg), (mem));	\
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, 0); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 3; \
				x86_64_mem_emit((inst), (dreg), (mem));	\
			} \
		} \
	} while(0)

#define x86_64_alu_reg_membase_size(inst, opc, dreg, basereg, disp, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 2; \
				x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), 0, (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 3; \
				x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
			} \
		} \
	} while(0)

#define x86_64_alu_reg_memindex_size(inst, opc, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), (indexreg), (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 2; \
				x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			case 4: \
			case 8: \
			{ \
				x86_64_rex_emit(inst, size, (dreg), (indexreg), (basereg)); \
				*(inst)++ = (((unsigned char)(opc)) << 3) + 3; \
				x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
			} \
		} \
	} while(0)

/*
 * The immediate value has to be at most 32 bit wide unless it can be sign
 * extended from a 8 bit or 32 bit wide value.
 */
#define x86_64_alu_reg_imm_size(inst, opc, dreg, imm, size) \
	do { \
		if(x86_is_imm8((imm)) && ((size) != 1 || (dreg) != X86_64_RAX)) \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (dreg)); \
					*(inst)++ = (unsigned char)0x80; \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
				} \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (dreg)); \
					*(inst)++ = (unsigned char)0x83; \
				} \
			} \
			x86_64_reg_emit((inst), (opc), (dreg)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else if((dreg) == X86_64_RAX) \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					*(inst)++ = (((unsigned char)(opc)) << 3) + 4; \
					x86_imm_emit8((inst), (imm)); \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
					*(inst)++ = (((unsigned char)(opc)) << 3) + 5; \
					x86_imm_emit16((inst), (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, 0); \
					*(inst)++ = (((unsigned char)(opc)) << 3) + 5; \
					x86_imm_emit32((inst), (imm)); \
				} \
			} \
		} \
		else \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (dreg)); \
					*(inst)++ = (unsigned char)0x80; \
					x86_64_reg_emit((inst), (opc), (dreg)); \
					x86_imm_emit8((inst), (imm)); \
					jit_assert(1); \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
					x86_64_rex_emit(inst, size, 0, 0, (dreg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_reg_emit((inst), (opc), (dreg)); \
					x86_imm_emit16((inst), (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (dreg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_reg_emit((inst), (opc), (dreg)); \
					x86_imm_emit32((inst), (imm)); \
				} \
			} \
		} \
	} while(0)

#define x86_64_alu_regp_imm_size(inst, opc, reg, imm, size) \
	do { \
		if(x86_is_imm8((imm))) \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (reg)); \
					*(inst)++ = (unsigned char)0x80; \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
				} \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (reg)); \
					*(inst)++ = (unsigned char)0x83; \
				} \
			} \
			x86_64_regp_emit((inst), (opc), (reg)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (reg)); \
					*(inst)++ = (unsigned char)0x80; \
					x86_64_regp_emit((inst), (opc), (reg)); \
					x86_imm_emit8((inst), (imm)); \
					jit_assert(1); \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
					x86_64_rex_emit(inst, size, 0, 0, (reg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_regp_emit((inst), (opc), (reg)); \
					x86_imm_emit16((inst), (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit(inst, size, 0, 0, (reg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_regp_emit((inst), (opc), (reg)); \
					x86_imm_emit32((inst), (imm)); \
				} \
			} \
		} \
	} while(0)

#define x86_64_alu_mem_imm_size(inst, opc, mem, imm, size) \
	do { \
		if(x86_is_imm8((imm))) \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, 0); \
					*(inst)++ = (unsigned char)0x80; \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
				} \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, 0); \
					*(inst)++ = (unsigned char)0x83; \
				} \
			} \
			x86_64_mem_emit((inst), (opc), (mem));	\
			x86_imm_emit8((inst), (imm));	\
		} \
		else \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, 0); \
					*(inst)++ = (unsigned char)0x80; \
					x86_64_mem_emit((inst), (opc), (mem));	\
					x86_imm_emit8((inst), (imm)); \
					jit_assert(1); \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
					x86_64_rex_emit((inst), (size), 0, 0, 0); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_mem_emit((inst), (opc), (mem));	\
					x86_imm_emit16((inst), (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, 0); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_mem_emit((inst), (opc), (mem));	\
					x86_imm_emit32((inst), (imm)); \
				} \
			} \
		} \
	} while(0)

#define x86_64_alu_membase_imm_size(inst, opc, basereg, disp, imm, size) \
	do { \
		if(x86_is_imm8((imm))) \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
					*(inst)++ = (unsigned char)0x80; \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
				} \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
					*(inst)++ = (unsigned char)0x83; \
				} \
			} \
			x86_64_membase_emit((inst), (opc), (basereg), (disp));	\
			x86_imm_emit8((inst), (imm));	\
		} \
		else \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
					*(inst)++ = (unsigned char)0x80; \
					x86_64_membase_emit((inst), (opc), (basereg), (disp));	\
					x86_imm_emit8((inst), (imm)); \
					jit_assert(1); \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
					x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_membase_emit((inst), (opc), (basereg), (disp));	\
					x86_imm_emit16((inst), (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_membase_emit((inst), (opc), (basereg), (disp));	\
					x86_imm_emit32((inst), (imm)); \
				} \
			} \
		} \
	} while(0)

#define x86_64_alu_memindex_imm_size(inst, opc, basereg, disp, indexreg, shift, imm, size) \
	do { \
		if(x86_is_imm8((imm))) \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
					*(inst)++ = (unsigned char)0x80; \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
				} \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
					*(inst)++ = (unsigned char)0x83; \
				} \
			} \
			x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			switch(size) \
			{ \
				case 1: \
				{ \
					x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
					*(inst)++ = (unsigned char)0x80; \
					x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
					x86_imm_emit8((inst), (imm)); \
					jit_assert(1); \
				} \
				break; \
				case 2: \
				{ \
					*(inst)++ = (unsigned char)0x66; \
					x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
					x86_imm_emit16((inst), (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
					*(inst)++ = (unsigned char)0x81; \
					x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
					x86_imm_emit32((inst), (imm)); \
				} \
			} \
		} \
	} while(0)

/*
 * Instructions with one opcode (plus optional r/m)
 */

/*
 * Unary opcodes
 */
#define x86_64_alu1_reg(inst, opc1, r, reg) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (reg)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_reg_emit((inst), (r), (reg));	\
	} while(0)

#define x86_64_alu1_regp(inst, opc1, r, regp) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (regp)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_regp_emit((inst), (r), (regp));	\
	} while(0)

#define x86_64_alu1_mem(inst, opc1, r, mem) \
	do { \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_mem_emit((inst), (r), (mem)); \
	} while(0)

#define x86_64_alu1_membase(inst, opc1, r, basereg, disp) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_membase_emit((inst), (r), (basereg), (disp)); \
	} while(0)

#define x86_64_alu1_memindex(inst, opc1, r, basereg, disp, indexreg, shift) \
	do { \
		x86_64_rex_emit((inst), 0, 0, (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_memindex_emit((inst), (r), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

#define x86_64_alu1_reg_size(inst, opc1, r, reg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (reg)); \
		x86_64_opcode1_emit((inst), (opc1), (size)); \
		x86_64_reg_emit((inst), (r), (reg));	\
	} while(0)

#define x86_64_alu1_regp_size(inst, opc1, r, regp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (regp)); \
		x86_64_opcode1_emit((inst), (opc1), (size)); \
		x86_64_regp_emit((inst), (r), (regp));	\
	} while(0)

#define x86_64_alu1_mem_size(inst, opc1, r, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, 0); \
		x86_64_opcode1_emit((inst), (opc1), (size)); \
		x86_64_mem_emit((inst), (r), (mem)); \
	} while(0)

#define x86_64_alu1_membase_size(inst, opc1, r, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
		x86_64_opcode1_emit((inst), (opc1), (size)); \
		x86_64_membase_emit((inst), (r), (basereg), (disp)); \
	} while(0)

#define x86_64_alu1_memindex_size(inst, opc1, r, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
		x86_64_opcode1_emit((inst), (opc1), (size)); \
		x86_64_memindex_emit((inst), (r), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

#define x86_64_alu1_reg_reg_size(inst, opc1, dreg, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sreg)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_reg_emit((inst), (dreg), (sreg));	\
	} while(0)

#define x86_64_alu1_reg_regp_size(inst, opc1, dreg, sregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sregp)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_regp_emit((inst), (dreg), (sregp));	\
	} while(0)

#define x86_64_alu1_reg_mem_size(inst, opc1, dreg, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, 0); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_mem_emit((inst), (dreg), (mem)); \
	} while(0)

#define x86_64_alu1_reg_membase_size(inst, opc1, dreg, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_alu1_reg_memindex_size(inst, opc1, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

#define x86_64_alu2_reg_reg_size(inst, opc1, opc2, dreg, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sreg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_reg_emit((inst), (dreg), (sreg));	\
	} while(0)

#define x86_64_alu2_reg_regp_size(inst, opc1, opc2, dreg, sregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sregp)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_regp_emit((inst), (dreg), (sregp));	\
	} while(0)

#define x86_64_alu2_reg_mem_size(inst, opc1, opc2, dreg, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, 0); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_mem_emit((inst), (dreg), (mem)); \
	} while(0)

#define x86_64_alu2_reg_membase_size(inst, opc1, opc2, dreg, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_alu2_reg_memindex_size(inst, opc1, opc2, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * Group1 general instructions
 */
#define x86_64_alu_reg_reg(inst, opc, dreg, sreg) \
	do { \
		x86_64_alu_reg_reg_size((inst), (opc), (dreg), (sreg), 8); \
	} while(0)

#define x86_64_alu_reg_imm(inst, opc, dreg, imm) \
	do { \
		x86_64_alu_reg_imm_size((inst), (opc), (dreg), (imm), 8); \
	} while(0)

/*
 * ADC: Add with carry
 */
#define x86_64_adc_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 2, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_adc_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 2, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_adc_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 2, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_adc_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 2, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_adc_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 2, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_adc_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 2, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_adc_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 2, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_adc_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 2, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_adc_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 2, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_adc_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 2, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_adc_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 2, (reg), (imm), (size)); \
	} while(0)

#define x86_64_adc_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 2, mem, imm, size); \
	} while(0)

#define x86_64_adc_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 2, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_adc_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 2, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * ADD
 */
#define x86_64_add_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 0, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_add_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 0, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_add_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 0, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_add_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 0, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_add_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 0, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_add_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 0, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_add_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 0, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_add_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 0, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_add_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 0, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_add_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 0, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_add_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 0, (reg), (imm), (size)); \
	} while(0)

#define x86_64_add_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 0, mem, imm, size); \
	} while(0)

#define x86_64_add_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 0, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_add_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 0, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * AND
 */
#define x86_64_and_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 4, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_and_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 4, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_and_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 4, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_and_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 4, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_and_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 4, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_and_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 4, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_and_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 4, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_and_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 4, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_and_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 4, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_and_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 4, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_and_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 4, (reg), (imm), (size)); \
	} while(0)

#define x86_64_and_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 4, mem, imm, size); \
	} while(0)

#define x86_64_and_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 4, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_and_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 4, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * CMP: compare
 */
#define x86_64_cmp_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 7, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_cmp_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 7, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_cmp_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 7, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_cmp_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 7, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_cmp_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 7, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_cmp_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 7, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_cmp_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 7, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_cmp_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 7, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cmp_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 7, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_cmp_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 7, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_cmp_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 7, (reg), (imm), (size)); \
	} while(0)

#define x86_64_cmp_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 7, mem, imm, size); \
	} while(0)

#define x86_64_cmp_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 7, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_cmp_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 7, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * OR
 */
#define x86_64_or_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 1, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_or_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 1, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_or_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 1, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_or_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 1, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_or_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 1, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_or_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 1, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_or_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 1, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_or_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 1, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_or_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 1, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_or_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 1, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_or_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 1, (reg), (imm), (size)); \
	} while(0)

#define x86_64_or_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 1, mem, imm, size); \
	} while(0)

#define x86_64_or_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 1, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_or_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 1, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * SBB: Subtract with borrow from al
 */
#define x86_64_sbb_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 3, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_sbb_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 3, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_sbb_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 3, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_sbb_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 3, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_sbb_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 3, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_sbb_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 3, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_sbb_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 3, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_sbb_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 3, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_sbb_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 3, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_sbb_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 3, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_sbb_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 3, (reg), (imm), (size)); \
	} while(0)

#define x86_64_sbb_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 3, mem, imm, size); \
	} while(0)

#define x86_64_sbb_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 3, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_sbb_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 3, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * SUB: Subtract
 */
#define x86_64_sub_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 5, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_sub_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 5, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_sub_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 5, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_sub_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 5, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_sub_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 5, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_sub_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 5, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_sub_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 5, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_sub_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 5, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_sub_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 5, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_sub_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 5, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_sub_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 5, (reg), (imm), (size)); \
	} while(0)

#define x86_64_sub_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 5, mem, imm, size); \
	} while(0)

#define x86_64_sub_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 5, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_sub_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 5, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * XOR
 */
#define x86_64_xor_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu_reg_reg_size((inst), 6, (dreg), (sreg), (size)); \
	} while(0)

#define x86_64_xor_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		x86_64_alu_regp_reg_size((inst), 6, (dregp), (sreg), (size)); \
	} while(0)

#define x86_64_xor_mem_reg_size(inst, mem, sreg, size) \
	do { \
		x86_64_alu_mem_reg_size((inst), 6, (mem), (sreg), (size)); \
	} while(0)

#define x86_64_xor_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		x86_64_alu_membase_reg_size((inst), 6, (basereg), (disp), (sreg), (size)); \
	} while(0)

#define x86_64_xor_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		x86_64_alu_memindex_reg_size((inst), 6, (basereg), (disp), (indexreg), (shift), (sreg), (size)); \
	} while(0)

#define x86_64_xor_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu_reg_regp_size((inst), 6, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_xor_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu_reg_mem_size((inst), 6, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_xor_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu_reg_membase_size((inst), 6, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_xor_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu_reg_memindex_size((inst), 6, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

#define x86_64_xor_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_alu_reg_imm_size((inst), 6, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_xor_regp_imm_size(inst, reg, imm, size) \
	do { \
		x86_64_alu_regp_imm_size((inst), 6, (reg), (imm), (size)); \
	} while(0)

#define x86_64_xor_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_alu_mem_imm_size(inst, 6, mem, imm, size); \
	} while(0)

#define x86_64_xor_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_alu_membase_imm_size((inst), 6, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_xor_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_alu_memindex_imm_size((inst), 6, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

/*
 * dec
 */
#define x86_64_dec_reg_size(inst, reg, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xfe, 1, (reg), (size)); \
	} while(0)

#define x86_64_dec_regp_size(inst, regp, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xfe, 1, (regp), (size)); \
	} while(0)

#define x86_64_dec_mem_size(inst, mem, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xfe, 1, (mem), (size)); \
	} while(0)

#define x86_64_dec_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xfe, 1, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_dec_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xfe, 1, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * div: unsigned division RDX:RAX / operand
 */
#define x86_64_div_reg_size(inst, reg, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xf6, 6, (reg), (size)); \
	} while(0)

#define x86_64_div_regp_size(inst, regp, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xf6, 6, (regp), (size)); \
	} while(0)

#define x86_64_div_mem_size(inst, mem, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xf6, 6, (mem), (size)); \
	} while(0)

#define x86_64_div_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xf6, 6, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_div_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xf6, 6, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * idiv: signed division RDX:RAX / operand
 */
#define x86_64_idiv_reg_size(inst, reg, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xf6, 7, (reg), (size)); \
	} while(0)

#define x86_64_idiv_regp_size(inst, regp, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xf6, 7, (regp), (size)); \
	} while(0)

#define x86_64_idiv_mem_size(inst, mem, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xf6, 7, (mem), (size)); \
	} while(0)

#define x86_64_idiv_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xf6, 7, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_idiv_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xf6, 7, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * inc
 */
#define x86_64_inc_reg_size(inst, reg, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xfe, 0, (reg), (size)); \
	} while(0)

#define x86_64_inc_regp_size(inst, regp, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xfe, 0, (regp), (size)); \
	} while(0)

#define x86_64_inc_mem_size(inst, mem, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xfe, 0, (mem), (size)); \
	} while(0)

#define x86_64_inc_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xfe, 0, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_inc_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xfe, 0, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * mul: multiply RDX:RAX = RAX * operand
 * is_signed == 0 -> unsigned multiplication
 * signed multiplication otherwise.
 */
#define x86_64_mul_reg_issigned_size(inst, reg, is_signed, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xf6, ((is_signed) ? 5 : 4), (reg), (size)); \
	} while(0)

#define x86_64_mul_regp_issigned_size(inst, regp, is_signed, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xf6, ((is_signed) ? 5 : 4), (regp), (size)); \
	} while(0)

#define x86_64_mul_mem_issigned_size(inst, mem, is_signed, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xf6, ((is_signed) ? 5 : 4), (mem), (size)); \
	} while(0)

#define x86_64_mul_membase_issigned_size(inst, basereg, disp, is_signed, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xf6, ((is_signed) ? 5 : 4), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_mul_memindex_issigned_size(inst, basereg, disp, indexreg, shift, is_signed, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xf6, ((is_signed) ? 5 : 4), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * neg
 */
#define x86_64_neg_reg_size(inst, reg, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xf6, 3, (reg), (size)); \
	} while(0)

#define x86_64_neg_regp_size(inst, regp, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xf6, 3, (regp), (size)); \
	} while(0)

#define x86_64_neg_mem_size(inst, mem, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xf6, 3, (mem), (size)); \
	} while(0)

#define x86_64_neg_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xf6, 3, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_neg_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xf6, 3, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * not
 */
#define x86_64_not_reg_size(inst, reg, size) \
	do { \
		x86_64_alu1_reg_size((inst), 0xf6, 2, (reg), (size)); \
	} while(0)

#define x86_64_not_regp_size(inst, regp, size) \
	do { \
		x86_64_alu1_regp_size((inst), 0xf6, 2, (regp), (size)); \
	} while(0)

#define x86_64_not_mem_size(inst, mem, size) \
	do { \
		x86_64_alu1_mem_size((inst), 0xf6, 2, (mem), (size)); \
	} while(0)

#define x86_64_not_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_alu1_membase_size((inst), 0xf6, 2, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_not_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_memindex_size((inst), 0xf6, 2, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * Note: x86_64_clear_reg () changes the condition code!
 */
#define x86_64_clear_reg(inst, reg) \
	x86_64_xor_reg_reg_size((inst), (reg), (reg), 4)

/*
 * shift instructions
 */
#define x86_64_shift_reg_imm_size(inst, opc, dreg, imm, size) \
	do { \
		if((imm) == 1) \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, (dreg)); \
			x86_64_opcode1_emit((inst), 0xd0, (size)); \
			x86_64_reg_emit((inst), (opc), (dreg)); \
		} \
		else \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, (dreg)); \
			x86_64_opcode1_emit((inst), 0xc0, (size)); \
			x86_64_reg_emit((inst), (opc), (dreg)); \
			x86_imm_emit8((inst), (imm)); \
		} \
	} while(0)

#define x86_64_shift_mem_imm_size(inst, opc, mem, imm, size) \
	do { \
		if((imm) == 1) \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, 0); \
			x86_64_opcode1_emit((inst), 0xd0, (size)); \
			x86_64_mem_emit((inst), (opc), (mem)); \
		} \
		else \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, 0); \
			x86_64_opcode1_emit((inst), 0xc0, (size)); \
			x86_64_mem_emit((inst), (opc), (mem)); \
			x86_imm_emit8((inst), (imm)); \
		} \
	} while(0)

#define x86_64_shift_regp_imm_size(inst, opc, dregp, imm, size) \
	do { \
		if((imm) == 1) \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, (dregp)); \
			x86_64_opcode1_emit((inst), 0xd0, (size)); \
			x86_64_regp_emit((inst), (opc), (dregp)); \
		} \
		else \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, (dregp)); \
			x86_64_opcode1_emit((inst), 0xc0, (size)); \
			x86_64_regp_emit((inst), (opc), (dregp)); \
			x86_imm_emit8((inst), (imm)); \
		} \
	} while(0)

#define x86_64_shift_membase_imm_size(inst, opc, basereg, disp, imm, size) \
	do { \
		if((imm) == 1) \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
			x86_64_opcode1_emit((inst), 0xd0, (size)); \
			x86_64_membase_emit((inst), (opc), (basereg), (disp)); \
		} \
		else \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
			x86_64_opcode1_emit((inst), 0xc0, (size)); \
			x86_64_membase_emit((inst), (opc), (basereg), (disp)); \
			x86_imm_emit8((inst), (imm)); \
		} \
	} while(0)

#define x86_64_shift_memindex_imm_size(inst, opc, basereg, disp, indexreg, shift, imm, size) \
	do { \
		if((imm) == 1) \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
			x86_64_opcode1_emit((inst), 0xd0, (size)); \
			x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
		} \
		else \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
			x86_64_opcode1_emit((inst), 0xc0, (size)); \
			x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
			x86_imm_emit8((inst), (imm)); \
		} \
	} while(0)

/*
 * shift by the number of bits in %cl
 */
#define x86_64_shift_reg_size(inst, opc, dreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (dreg)); \
		x86_64_opcode1_emit((inst), 0xd2, (size)); \
		x86_64_reg_emit((inst), (opc), (dreg)); \
	} while(0)

#define x86_64_shift_mem_size(inst, opc, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, 0); \
		x86_64_opcode1_emit((inst), 0xd2, (size)); \
		x86_64_mem_emit((inst), (opc), (mem)); \
	} while(0)

#define x86_64_shift_regp_size(inst, opc, dregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (dregp)); \
		x86_64_opcode1_emit((inst), 0xd2, (size)); \
		x86_64_regp_emit((inst), (opc), (dregp)); \
	} while(0)

#define x86_64_shift_membase_size(inst, opc, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
		x86_64_opcode1_emit((inst), 0xd2, (size)); \
		x86_64_membase_emit((inst), (opc), (basereg), (disp)); \
	} while(0)

#define x86_64_shift_memindex_size(inst, opc, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
		x86_64_opcode1_emit((inst), 0xd2, (size)); \
		x86_64_memindex_emit((inst), (opc), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * shl: Shit left (clear the least significant bit)
 */
#define x86_64_shl_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_shift_reg_imm_size((inst), 4, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_shl_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_shift_mem_imm_size((inst), 4, (mem), (imm), (size)); \
	} while(0)

#define x86_64_shl_regp_imm_size(inst, dregp, imm, size) \
	do { \
		x86_64_shift_regp_imm_size((inst), 4, (dregp), (imm), (size)); \
	} while(0)

#define x86_64_shl_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_shift_membase_imm_size((inst), 4, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_shl_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_shift_memindex_imm_size((inst), 4, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

#define x86_64_shl_reg_size(inst, dreg, size) \
	do { \
		x86_64_shift_reg_size((inst), 4, (dreg), (size)); \
	} while(0)

#define x86_64_shl_mem_size(inst, mem, size) \
	do { \
		x86_64_shift_mem_size((inst), 4, (mem), (size)); \
	} while(0)

#define x86_64_shl_regp_size(inst, dregp, size) \
	do { \
		x86_64_shift_regp_size((inst), 4, (dregp), (size)); \
	} while(0)

#define x86_64_shl_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_shift_membase_size((inst), 4, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_shl_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_shift_memindex_size((inst), 4, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * shr: Unsigned shit right (clear the most significant bit)
 */
#define x86_64_shr_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_shift_reg_imm_size((inst), 5, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_shr_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_shift_mem_imm_size((inst), 5, (mem), (imm), (size)); \
	} while(0)

#define x86_64_shr_regp_imm_size(inst, dregp, imm, size) \
	do { \
		x86_64_shift_regp_imm_size((inst), 5, (dregp), (imm), (size)); \
	} while(0)

#define x86_64_shr_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_shift_membase_imm_size((inst), 5, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_shr_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_shift_memindex_imm_size((inst), 5, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

#define x86_64_shr_reg_size(inst, dreg, size) \
	do { \
		x86_64_shift_reg_size((inst), 5, (dreg), (size)); \
	} while(0)

#define x86_64_shr_mem_size(inst, mem, size) \
	do { \
		x86_64_shift_mem_size((inst), 5, (mem), (size)); \
	} while(0)

#define x86_64_shr_regp_size(inst, dregp, size) \
	do { \
		x86_64_shift_regp_size((inst), 5, (dregp), (size)); \
	} while(0)

#define x86_64_shr_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_shift_membase_size((inst), 5, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_shr_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_shift_memindex_size((inst), 5, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * sar: Signed shit right (keep the most significant bit)
 */
#define x86_64_sar_reg_imm_size(inst, dreg, imm, size) \
	do { \
		x86_64_shift_reg_imm_size((inst), 7, (dreg), (imm), (size)); \
	} while(0)

#define x86_64_sar_mem_imm_size(inst, mem, imm, size) \
	do { \
		x86_64_shift_mem_imm_size((inst), 7, (mem), (imm), (size)); \
	} while(0)

#define x86_64_sar_regp_imm_size(inst, dregp, imm, size) \
	do { \
		x86_64_shift_regp_imm_size((inst), 7, (dregp), (imm), (size)); \
	} while(0)

#define x86_64_sar_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		x86_64_shift_membase_imm_size((inst), 7, (basereg), (disp), (imm), (size)); \
	} while(0)

#define x86_64_sar_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		x86_64_shift_memindex_imm_size((inst), 7, (basereg), (disp), (indexreg), (shift), (imm), (size)); \
	} while(0)

#define x86_64_sar_reg_size(inst, dreg, size) \
	do { \
		x86_64_shift_reg_size((inst), 7, (dreg), (size)); \
	} while(0)

#define x86_64_sar_mem_size(inst, mem, size) \
	do { \
		x86_64_shift_mem_size((inst), 7, (mem), (size)); \
	} while(0)

#define x86_64_sar_regp_size(inst, dregp, size) \
	do { \
		x86_64_shift_regp_size((inst), 7, (dregp), (size)); \
	} while(0)

#define x86_64_sar_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_shift_membase_size((inst), 7, (basereg), (disp), (size)); \
	} while(0)

#define x86_64_sar_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_shift_memindex_size((inst), 7, (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * test: and tha values and set sf, zf and pf according to the result
 */
#define x86_64_test_reg_imm_size(inst, reg, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (reg)); \
		if((reg) == X86_64_RAX) { \
			x86_64_opcode1_emit((inst), 0xa8, (size)); \
		} \
		else \
		{ \
			x86_64_opcode1_emit((inst), 0xf6, (size)); \
			x86_64_reg_emit((inst), 0, (reg)); \
		} \
		x86_64_imm_emit_max32((inst), (imm), (size)); \
	} while (0)

#define x86_64_test_regp_imm_size(inst, regp, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (regp)); \
		x86_64_opcode1_emit((inst), 0xf6, (size)); \
		x86_64_regp_emit((inst), 0, (regp)); \
		x86_64_imm_emit_max32((inst), (imm), (size)); \
	} while (0)

#define x86_64_test_mem_imm_size(inst, mem, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, 0); \
		x86_64_opcode1_emit((inst), 0xf6, (size)); \
		x86_64_mem_emit((inst), 0, (mem)); \
		x86_64_imm_emit_max32((inst), (imm), (size)); \
	} while (0)

#define x86_64_test_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (basereg)); \
		x86_64_opcode1_emit((inst), 0xf6, (size)); \
		x86_64_membase_emit((inst), 0, (basereg), (disp)); \
		x86_64_imm_emit_max32((inst), (imm), (size)); \
	} while (0)

#define x86_64_test_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
		x86_64_opcode1_emit((inst), 0xf6, (size)); \
		x86_64_memindex_emit((inst), 0, (basereg), (disp), (indexreg), (shift)); \
		x86_64_imm_emit_max32((inst), (imm), (size)); \
	} while (0)

#define x86_64_test_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (sreg), 0, (dreg)); \
		x86_64_opcode1_emit((inst), 0x84, (size)); \
		x86_64_reg_emit((inst), (sreg), (dreg)); \
	} while (0)

#define x86_64_test_regp_reg_size(inst, dregp, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (sreg), 0, (dregp)); \
		x86_64_opcode1_emit((inst), 0x84, (size)); \
		x86_64_regp_emit((inst), (sreg), (dregp)); \
	} while (0)

#define x86_64_test_mem_reg_size(inst, mem, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (sreg), 0, 0); \
		x86_64_opcode1_emit((inst), 0x84, (size)); \
		x86_64_mem_emit((inst), (sreg), (mem)); \
	} while (0)

#define x86_64_test_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (sreg), 0, (basereg)); \
		x86_64_opcode1_emit((inst), 0x84, (size)); \
		x86_64_membase_emit((inst), (sreg), (basereg), (disp)); \
	} while (0)

#define x86_64_test_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (sreg), (indexreg), (basereg)); \
		x86_64_opcode1_emit((inst), 0x84, (size)); \
		x86_64_memindex_emit((inst), (sreg), (basereg), (disp), (indexreg), (shift)); \
	} while (0)

/*
 * imul: signed multiply
 */
#define x86_64_imul_reg_reg_imm_size(inst, dreg, sreg, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sreg)); \
		if(x86_is_imm8((imm))) \
		{ \
			*(inst)++ = (unsigned char)0x6b; \
			x86_64_reg_emit((inst), (dreg), (sreg)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			*(inst)++ = (unsigned char)0x69; \
			x86_64_reg_emit((inst), (dreg), (sreg)); \
			switch((size)) \
			{ \
				case 2: \
				{ \
					x86_imm_emit16(inst, (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_imm_emit32(inst, (imm)); \
				} \
				break; \
			} \
		} \
	} while(0)

#define x86_64_imul_reg_regp_imm_size(inst, dreg, sregp, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sregp)); \
		if(x86_is_imm8((imm))) \
		{ \
			*(inst)++ = (unsigned char)0x6b; \
			x86_64_regp_emit((inst), (dreg), (sregp)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			*(inst)++ = (unsigned char)0x69; \
			x86_64_regp_emit((inst), (dreg), (sregp)); \
			switch((size)) \
			{ \
				case 2: \
				{ \
					x86_imm_emit16(inst, (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_imm_emit32(inst, (imm)); \
				} \
				break; \
			} \
		} \
	} while(0)

#define x86_64_imul_reg_mem_imm_size(inst, dreg, mem, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, 0); \
		if(x86_is_imm8((imm))) \
		{ \
			*(inst)++ = (unsigned char)0x6b; \
			x86_64_mem_emit((inst), (dreg), (mem)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			*(inst)++ = (unsigned char)0x69; \
			x86_64_mem_emit((inst), (dreg), (mem)); \
			switch((size)) \
			{ \
				case 2: \
				{ \
					x86_imm_emit16(inst, (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_imm_emit32(inst, (imm)); \
				} \
				break; \
			} \
		} \
	} while(0)

#define x86_64_imul_reg_membase_imm_size(inst, dreg, basereg, disp, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (basereg)); \
		if(x86_is_imm8((imm))) \
		{ \
			*(inst)++ = (unsigned char)0x6b; \
			x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			*(inst)++ = (unsigned char)0x69; \
			x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
			switch((size)) \
			{ \
				case 2: \
				{ \
					x86_imm_emit16(inst, (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_imm_emit32(inst, (imm)); \
				} \
				break; \
			} \
		} \
	} while(0)

#define x86_64_imul_reg_memindex_imm_size(inst, dreg, basereg, disp, indexreg, shift, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		if(x86_is_imm8((imm))) \
		{ \
			*(inst)++ = (unsigned char)0x6b; \
			x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
			x86_imm_emit8((inst), (imm)); \
		} \
		else \
		{ \
			*(inst)++ = (unsigned char)0x69; \
			x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
			switch((size)) \
			{ \
				case 2: \
				{ \
					x86_imm_emit16(inst, (imm)); \
				} \
				break; \
				case 4: \
				case 8: \
				{ \
					x86_imm_emit32(inst, (imm)); \
				} \
				break; \
			} \
		} \
	} while(0)

#define x86_64_imul_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sreg)); \
		*(inst)++ = (unsigned char)0x0F; \
		*(inst)++ = (unsigned char)0xAF; \
		x86_64_reg_emit((inst), (dreg), (sreg)); \
	} while(0)

#define x86_64_imul_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sregp)); \
		*(inst)++ = (unsigned char)0x0F; \
		*(inst)++ = (unsigned char)0xAF; \
		x86_64_regp_emit((inst), (dreg), (sregp)); \
	} while(0)

#define x86_64_imul_reg_mem_size(inst, dreg, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, 0); \
		*(inst)++ = (unsigned char)0x0F; \
		*(inst)++ = (unsigned char)0xAF; \
		x86_64_mem_emit((inst), (dreg), (mem)); \
	} while(0)

#define x86_64_imul_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (basereg)); \
		*(inst)++ = (unsigned char)0x0F; \
		*(inst)++ = (unsigned char)0xAF; \
		x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_imul_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)0x0F; \
		*(inst)++ = (unsigned char)0xAF; \
		x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * cwd, cdq, cqo: sign extend ax to dx (used for div and idiv)
 */
#define x86_64_cwd(inst) \
	do { \
		*(inst)++ = (unsigned char)0x66; \
		*(inst)++ = (unsigned char)0x99; \
	} while(0)

#define x86_64_cdq(inst) \
	do { \
		*(inst)++ = (unsigned char)0x99; \
	} while(0)

#define x86_64_cqo(inst) \
	do { \
		*(inst)++ = (unsigned char)0x48; \
		*(inst)++ = (unsigned char)0x99; \
	} while(0)

/*
 * Lea instructions
 */
#define x86_64_lea_mem_size(inst, dreg, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, (dreg)); \
		x86_lea_mem((inst), ((dreg) & 0x7), (mem)); \
	} while(0)

#define x86_64_lea_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (dreg), 0, (basereg)); \
		*(inst)++ = (unsigned char)0x8d;	\
		x86_64_membase_emit((inst), (dreg), (basereg), (disp));	\
	} while (0)

#define x86_64_lea_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)0x8d; \
		x86_64_memindex_emit ((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * Move instructions.
 */
#define x86_64_mov_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (dreg), 0, (sreg)); \
		x86_64_opcode1_emit(inst, 0x8a, (size)); \
		x86_64_reg_emit((inst), ((dreg) & 0x7), ((sreg) & 0x7)); \
	} while(0)

#define x86_64_mov_regp_reg_size(inst, regp, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (sreg), 0, (regp)); \
		x86_64_opcode1_emit(inst, 0x88, (size)); \
		x86_64_regp_emit((inst), (sreg), (regp)); \
	} while (0)

#define x86_64_mov_membase_reg_size(inst, basereg, disp, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (sreg), 0, (basereg)); \
		x86_64_opcode1_emit(inst, 0x88, (size)); \
		x86_64_membase_emit((inst), (sreg), (basereg), (disp));	\
	} while(0)

#define x86_64_mov_memindex_reg_size(inst, basereg, disp, indexreg, shift, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (sreg), (indexreg), (basereg)); \
		x86_64_opcode1_emit(inst, 0x88, (size)); \
		x86_64_memindex_emit((inst), (sreg), (basereg), (disp), (indexreg), (shift)); \
	} while (0)

/*
 * Using the AX register is the only possibility to address 64bit.
 * All other registers are bound to 32bit values.
 */
#define x86_64_mov_mem_reg_size(inst, mem, sreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (sreg), 0, 0); \
		if((sreg) == X86_64_RAX) \
		{ \
			x86_64_opcode1_emit(inst, 0xa2, (size)); \
			x86_64_imm_emit64(inst, (mem)); \
		} \
		else \
		{ \
			x86_64_opcode1_emit(inst, 0x88, (size)); \
			x86_address_byte((inst), 0, ((sreg) & 0x7), 4); \
			x86_address_byte((inst), 0, 4, 5); \
	        x86_imm_emit32((inst), (mem)); \
		} \
	} while (0)

#define x86_64_mov_reg_imm_size(inst, dreg, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), 0, 0, (dreg)); \
		switch((size)) \
		{ \
			case 1: \
			{ \
				*(inst)++ = (unsigned char)0xb0 + ((dreg) & 0x7); \
				x86_imm_emit8(inst, (imm)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xb8 + ((dreg) & 0x7); \
				x86_imm_emit16(inst, (imm)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xb8 + ((dreg) & 0x7); \
				x86_imm_emit32(inst, (imm)); \
			} \
			break; \
			case 8: \
			{ \
				jit_nint __x86_64_imm = (imm); \
				if(__x86_64_imm >= (jit_nint)jit_min_int && __x86_64_imm <= (jit_nint)jit_max_int) \
				{ \
					*(inst)++ = (unsigned char)0xc7; \
					x86_64_reg_emit((inst), 0, (dreg)); \
					x86_imm_emit32(inst, (__x86_64_imm)); \
				} \
				else \
				{ \
					*(inst)++ = (unsigned char)0xb8 + ((dreg) & 0x7); \
					x86_64_imm_emit64(inst, (__x86_64_imm)); \
				} \
			} \
			break; \
		} \
	} while(0)

/*
 * Using the AX register is the only possibility to address 64bit.
 * All other registers are bound to 32bit values.
 */
#define x86_64_mov_reg_mem_size(inst, dreg, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (dreg), 0, 0); \
		if((dreg) == X86_64_RAX) \
		{ \
			x86_64_opcode1_emit(inst, 0xa0, (size)); \
			x86_64_imm_emit64(inst, (mem)); \
		} \
		else \
		{ \
			x86_64_opcode1_emit(inst, 0x8a, (size)); \
			x86_address_byte ((inst), 0, (dreg), 4); \
			x86_address_byte ((inst), 0, 4, 5); \
			x86_imm_emit32 ((inst), (mem)); \
		} \
	} while (0)

#define x86_64_mov_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (dreg), 0, (sregp)); \
		x86_64_opcode1_emit(inst, 0x8a, (size)); \
		x86_64_regp_emit((inst), (dreg), (sregp)); \
	} while(0)

#define x86_64_mov_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), (dreg), 0, (basereg)); \
		x86_64_opcode1_emit(inst, 0x8a, (size)); \
		x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
	} while(0)


#define x86_64_mov_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		x86_64_opcode1_emit(inst, 0x8a, (size)); \
		x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * Only 32bit mem and imm values are allowed here.
 * mem is be RIP relative.
 * 32 bit imm will be sign extended to 64 bits for 64 bit size.
 */
#define x86_64_mov_mem_imm_size(inst, mem, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, 0, 0); \
		x86_64_opcode1_emit(inst, 0xc6, (size)); \
		x86_64_mem_emit((inst), 0, (mem)); \
		x86_64_imm_emit_max32(inst, (imm), (size)); \
	} while(0)

#define x86_64_mov_regp_imm_size(inst, dregp, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), 0, 0, (dregp)); \
		x86_64_opcode1_emit(inst, 0xc6, (size)); \
		x86_64_regp_emit((inst), 0, (dregp)); \
		x86_64_imm_emit_max32(inst, (imm), (size)); \
	} while(0)

#define x86_64_mov_membase_imm_size(inst, basereg, disp, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit(inst, (size), 0, 0, (basereg)); \
		x86_64_opcode1_emit(inst, 0xc6, (size)); \
		x86_64_membase_emit((inst), 0, (basereg), (disp)); \
		x86_64_imm_emit_max32(inst, (imm), (size)); \
	} while(0)

#define x86_64_mov_memindex_imm_size(inst, basereg, disp, indexreg, shift, imm, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), 0, (indexreg), (basereg)); \
		x86_64_opcode1_emit(inst, 0xc6, (size)); \
		x86_64_memindex_emit((inst), 0, (basereg), (disp), (indexreg), (shift)); \
		x86_64_imm_emit_max32(inst, (imm), (size)); \
	} while(0)

/*
 * Move with sign extension to the given size (signed)
 */
#define x86_64_movsx8_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu2_reg_reg_size((inst), 0x0f, 0xbe, (dreg), (sreg), (size) | 1); \
	}while(0)

#define x86_64_movsx8_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu2_reg_regp_size((inst), 0x0f, 0xbe, (dreg), (sregp), (size)); \
	}while(0)

#define x86_64_movsx8_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu2_reg_mem_size((inst), 0x0f, 0xbe, (dreg), (mem), (size)); \
	}while(0)

#define x86_64_movsx8_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu2_reg_membase_size((inst), 0x0f, 0xbe, (dreg), (basereg), (disp), (size)); \
	}while(0)

#define x86_64_movsx8_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu2_reg_memindex_size((inst), 0x0f, 0xbe, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	}while(0)

#define x86_64_movsx16_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu2_reg_reg_size((inst), 0x0f, 0xbf, (dreg), (sreg), (size)); \
	}while(0)

#define x86_64_movsx16_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu2_reg_regp_size((inst), 0x0f, 0xbf, (dreg), (sregp), (size)); \
	}while(0)

#define x86_64_movsx16_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu2_reg_mem_size((inst), 0x0f, 0xbf, (dreg), (mem), (size)); \
	}while(0)

#define x86_64_movsx16_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu2_reg_membase_size((inst), 0x0f, 0xbf, (dreg), (basereg), (disp), (size)); \
	}while(0)

#define x86_64_movsx16_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu2_reg_memindex_size((inst), 0x0f, 0xbf, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	}while(0)

#define x86_64_movsx32_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu1_reg_reg_size((inst), 0x63, (dreg), (sreg), (size)); \
	}while(0)

#define x86_64_movsx32_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu1_reg_regp_size((inst), 0x63, (dreg), (sregp), (size)); \
	}while(0)

#define x86_64_movsx32_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu1_reg_mem_size((inst), 0x63, (dreg), (mem), (size)); \
	}while(0)

#define x86_64_movsx32_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu1_reg_membase_size((inst), 0x63, (dreg), (basereg), (disp), (size)); \
	}while(0)

#define x86_64_movsx32_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu1_reg_memindex_size((inst), 0x63, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	}while(0)

/*
 * Move with zero extension to the given size (unsigned)
 */
#define x86_64_movzx8_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu2_reg_reg_size((inst), 0x0f, 0xb6, (dreg), (sreg), (size) | 1); \
	}while(0)

#define x86_64_movzx8_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu2_reg_regp_size((inst), 0x0f, 0xb6, (dreg), (sregp), (size)); \
	}while(0)

#define x86_64_movzx8_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu2_reg_mem_size((inst), 0x0f, 0xb6, (dreg), (mem), (size)); \
	}while(0)

#define x86_64_movzx8_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu2_reg_membase_size((inst), 0x0f, 0xb6, (dreg), (basereg), (disp), (size)); \
	}while(0)

#define x86_64_movzx8_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu2_reg_memindex_size((inst), 0x0f, 0xb6, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	}while(0)

#define x86_64_movzx16_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		x86_64_alu2_reg_reg_size((inst), 0x0f, 0xb7, (dreg), (sreg), (size)); \
	}while(0)

#define x86_64_movzx16_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_alu2_reg_regp_size((inst), 0x0f, 0xb7, (dreg), (sregp), (size)); \
	}while(0)

#define x86_64_movzx16_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_alu2_reg_mem_size((inst), 0x0f, 0xb7, (dreg), (mem), (size)); \
	}while(0)

#define x86_64_movzx16_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_alu2_reg_membase_size((inst), 0x0f, 0xb7, (dreg), (basereg), (disp), (size)); \
	}while(0)

#define x86_64_movzx16_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_alu2_reg_memindex_size((inst), 0x0f, 0xb7, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	}while(0)

/*
 * cmov: conditional move
 */
#define x86_64_cmov_reg_reg_size(inst, cond, dreg, sreg, is_signed, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sreg)); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] - 0x30; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] - 0x30; \
		} \
		x86_64_reg_emit((inst), (dreg), (sreg)); \
	} while (0)

#define x86_64_cmov_reg_regp_size(inst, cond, dreg, sregp, is_signed, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (sregp)); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] - 0x30; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] - 0x30; \
		} \
		x86_64_regp_emit((inst), (dreg), (sregp)); \
	} while (0)

#define x86_64_cmov_reg_mem_size(inst, cond, dreg, mem, is_signed, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, 0); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] - 0x30; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] - 0x30; \
		} \
		x86_64_mem_emit((inst), (dreg), (mem)); \
	} while (0)

#define x86_64_cmov_reg_membase_size(inst, cond, dreg, basereg, disp, is_signed, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), 0, (basereg)); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] - 0x30; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] - 0x30; \
		} \
		x86_64_membase_emit((inst), (dreg), (basereg), (disp)); \
	} while (0)

#define x86_64_cmov_reg_memindex_size(inst, cond, dreg, basereg, disp, indexreg, shift, is_signed, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit((inst), (size), (dreg), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] - 0x30; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] - 0x30; \
		} \
		x86_64_memindex_emit((inst), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while (0)

/*
 * Stack manupulation instructions (push and pop)
 */

/*
 * Push instructions have a default size of 64 bit. mode.
 * There is no way to encode a 32 bit push.
 * So only the sizes 8 and 2 are allowed in 64 bit mode.
 */
#define x86_64_push_reg_size(inst, reg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, 0, (reg)); \
		*(inst)++ = (unsigned char)0x50 + ((reg) & 0x7); \
	} while(0)

#define x86_64_push_regp_size(inst, sregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, 0, (sregp)); \
		*(inst)++ = (unsigned char)0xff; \
		x86_64_regp_emit((inst), 6, (sregp)); \
	} while(0)

#define x86_64_push_mem_size(inst, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, 0, 0); \
		*(inst)++ = (unsigned char)0xff; \
		x86_64_mem_emit((inst), 6, (mem)); \
	} while(0)

#define x86_64_push_membase_size(inst, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, 0, (basereg)); \
		*(inst)++ = (unsigned char)0xff; \
		x86_64_membase_emit((inst), 6, (basereg), (disp)); \
	} while(0)

#define x86_64_push_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)0xff; \
		x86_64_memindex_emit((inst), 6, (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * We can push only 32 bit immediate values.
 * The value is sign extended to 64 bit on the stack.
 */
#define x86_64_push_imm(inst, imm) \
	do { \
		int _imm = (int) (imm); \
		if(x86_is_imm8(_imm)) \
		{ \
			*(inst)++ = (unsigned char)0x6A; \
			x86_imm_emit8 ((inst), (_imm)); \
		} \
		else \
		{ \
			*(inst)++ = (unsigned char)0x68; \
			x86_imm_emit32((inst), (_imm)); \
		} \
	} while(0)

/*
 * Use this version if you need a specific width of the value
 * pushed. The Value on the stack will allways be 64bit wide.
 */
#define x86_64_push_imm_size(inst, imm, size) \
	do { \
		switch(size) \
		{ \
			case 1: \
			{ \
				*(inst)++ = (unsigned char)0x6A; \
				x86_imm_emit8((inst), (imm)); \
			} \
			break; \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0x66; \
				*(inst)++ = (unsigned char)0x68; \
				x86_imm_emit16((inst), (imm)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0x68; \
				x86_imm_emit32((inst), (imm)); \
			}\
		} \
	} while (0)


/*
 * Pop instructions have a default size of 64 bit in 64 bit mode.
 * There is no way to encode a 32 bit pop.
 * So only the sizes 2 and 8 are allowed.
 */
#define x86_64_pop_reg_size(inst, dreg, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), 0, 0, 0, (dreg)); \
		*(inst)++ = (unsigned char)0x58 + ((dreg) & 0x7); \
	} while(0)

#define x86_64_pop_regp_size(inst, dregp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, 0, (dregp)); \
		*(inst)++ = (unsigned char)0x8f; \
		x86_64_regp_emit((inst), 0, (dregp)); \
	} while(0)

#define x86_64_pop_mem_size(inst, mem, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		*(inst)++ = (unsigned char)0x8f; \
		x86_64_mem_emit((inst), 0, (mem)); \
	} while(0)

#define x86_64_pop_membase_size(inst, basereg, disp, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, 0,(basereg)); \
		*(inst)++ = (unsigned char)0x8f; \
		x86_64_membase_emit((inst), 0, (basereg), (disp)); \
	} while(0)

#define x86_64_pop_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		if((size) == 2) \
		{ \
			*(inst)++ = (unsigned char)0x66; \
		} \
		x86_64_rex_emit64((inst), (size), 0, (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)0x8f; \
		x86_64_memindex_emit((inst), 0, (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * control flow change instructions
 */

/*
 * call
 */

/*
 * call_imm is a relative call.
 * imm has to be a 32bit offset from the instruction following the
 * call instruction (absolute - (inst + 5)).
 * For offsets greater that 32bit an indirect call (via register)
 * has to be used.
 */
#define x86_64_call_imm(inst, imm) \
	do { \
		x86_call_imm((inst), (imm)); \
	} while(0)

#define x86_64_call_reg(inst, reg) \
	do { \
		x86_64_alu1_reg((inst), 0xff, 2, (reg)); \
	} while(0)

#define x86_64_call_regp(inst, regp) \
	do { \
		x86_64_alu1_regp((inst), 0xff, 2, (regp)); \
	} while(0)

/*
 * call_mem is a absolute indirect call.
 * To be able to use this instruction the address must be either
 * in the lowest 2GB or in the highest 2GB addressrange.
 * This is because mem is sign extended to 64bit.
 */
#define x86_64_call_mem(inst, mem) \
	do { \
		x86_64_alu1_mem((inst), 0xff, 2, (mem)); \
	} while(0)

#define x86_64_call_membase(inst, basereg, disp) \
	do { \
		x86_64_alu1_membase((inst), 0xff, 2, (basereg), (disp)); \
	} while(0)

#define x86_64_call_memindex(inst, basereg, disp, indexreg, shift) \
	do { \
		x86_64_alu1_memindex((inst), 0xff, 2, (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * jmp
 */

/*
 * unconditional relative jumps
 */
#define x86_64_jmp_imm8(inst, disp) \
	do { \
		*(inst)++ = (unsigned char)0xEB; \
		x86_imm_emit8((inst), (disp)); \
	} while(0)

#define x86_64_jmp_imm(inst, disp) \
	do { \
		*(inst)++ = (unsigned char)0xE9; \
		x86_imm_emit32((inst), (disp)); \
	} while(0)

/*
 * unconditional indirect jumps
 */
#define x86_64_jmp_reg(inst, reg) \
	do { \
		x86_64_alu1_reg((inst), 0xff, 4, (reg)); \
	} while(0)

#define x86_64_jmp_regp(inst, regp) \
	do { \
		x86_64_alu1_regp((inst), 0xff, 4, (regp)); \
	} while(0)

#define x86_64_jmp_mem(inst, mem) \
	do { \
		x86_64_alu1_mem((inst), 0xff, 4, (mem)); \
	} while(0)

#define x86_64_jmp_membase(inst, basereg, disp) \
	do { \
		x86_64_alu1_membase((inst), 0xff, 4, (basereg), (disp)); \
	} while(0)

#define x86_64_jmp_memindex(inst, basereg, disp, indexreg, shift) \
	do { \
		x86_64_alu1_memindex((inst), 0xff, 4, (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * Set the low byte in a register to 0x01 if a condition is met
 * or 0x00 otherwise.
 */
#define x86_64_set_reg(inst, cond, dreg, is_signed) \
	do { \
		x86_64_rex_emit((inst), 1, 0, 0, (dreg)); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] + 0x20; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] + 0x20; \
		} \
		x86_64_reg_emit((inst), 0, (dreg)); \
	} while(0)

#define x86_64_set_mem(inst, cond, mem, is_signed) \
	do { \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] + 0x20; \
		} \
		else \
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] + 0x20; \
		} \
		x86_64_mem_emit((inst), 0, (mem)); \
	} while(0)

#define x86_64_set_membase(inst, cond, basereg, disp, is_signed) \
	do { \
		x86_64_rex_emit((inst), 4, 0, 0, (basereg)); \
		*(inst)++ = (unsigned char)0x0f; \
		if((is_signed)) \
		{ \
			*(inst)++ = x86_cc_signed_map[(cond)] + 0x20;	\
		} \
		else	\
		{ \
			*(inst)++ = x86_cc_unsigned_map[(cond)] + 0x20;	\
		} \
		x86_64_membase_emit((inst), 0, (basereg), (disp));	\
	} while(0)

/*
 * ret
 */
#define x86_64_ret(inst) \
	do { \
		x86_ret((inst)); \
	} while(0)

/*
 * xchg: Exchange values
 */
#define x86_64_xchg_reg_reg_size(inst, dreg, sreg, size) \
	do { \
		if(((size) > 1) && ((dreg) == X86_64_RAX || (sreg) == X86_64_RAX)) \
		{ \
			if((size) == 2) \
			{ \
				*(inst)++ = (unsigned char)0x66; \
			} \
			if((dreg) == X86_64_RAX) \
			{ \
				x86_64_rex_emit((inst), (size), 0, 0, (sreg)); \
				*(inst)++ = (unsigned char)(0x90 + (unsigned char)(sreg & 0x7)); \
			} \
			else \
			{ \
				x86_64_rex_emit((inst), (size), 0, 0, (dreg)); \
				*(inst)++ = (unsigned char)(0x90 + (unsigned char)(dreg & 0x7)); \
			} \
		} \
		else \
		{ \
			if((size) == 1) \
			{ \
				x86_64_alu1_reg_reg_size((inst), 0x86, (dreg), (sreg), (size)); \
			} \
			else \
			{ \
				x86_64_alu1_reg_reg_size((inst), 0x87, (dreg), (sreg), (size)); \
			} \
		} \
	} while(0)

/*
 * XMM instructions
 */

/*
 * xmm instructions with two opcodes
 */
#define x86_64_xmm2_reg_reg(inst, opc1, opc2, r, reg) \
	do { \
		x86_64_rex_emit(inst, 0, (r), 0, (reg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_reg_emit(inst, (r), (reg)); \
	} while(0)

#define x86_64_xmm2_reg_regp(inst, opc1, opc2, r, regp) \
	do { \
		x86_64_rex_emit(inst, 0, (r), 0, (regp)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_regp_emit(inst, (r), (regp)); \
	} while(0)

#define x86_64_xmm2_reg_mem(inst, opc1, opc2, r, mem) \
	do { \
		x86_64_rex_emit(inst, 0, (r), 0, 0); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_mem_emit(inst, (r), (mem)); \
	} while(0)

#define x86_64_xmm2_reg_membase(inst, opc1, opc2, r, basereg, disp) \
	do { \
		x86_64_rex_emit(inst, 0, (r), 0, (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_membase_emit(inst, (r), (basereg), (disp)); \
	} while(0)

#define x86_64_xmm2_reg_memindex(inst, opc1, opc2, r, basereg, disp, indexreg, shift) \
	do { \
		x86_64_rex_emit(inst, 0, (r), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_memindex_emit((inst), (r), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * xmm instructions with a prefix and two opcodes
 */
#define x86_64_p1_xmm2_reg_reg_size(inst, p1, opc1, opc2, r, reg, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, (reg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_reg_emit(inst, (r), (reg)); \
	} while(0)

#define x86_64_p1_xmm2_reg_regp_size(inst, p1, opc1, opc2, r, regp, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, (regp)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_regp_emit(inst, (r), (regp)); \
	} while(0)

#define x86_64_p1_xmm2_reg_mem_size(inst, p1, opc1, opc2, r, mem, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, 0); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_mem_emit(inst, (r), (mem)); \
	} while(0)

#define x86_64_p1_xmm2_reg_membase_size(inst, p1, opc1, opc2, r, basereg, disp, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_membase_emit(inst, (r), (basereg), (disp)); \
	} while(0)

#define x86_64_p1_xmm2_reg_memindex_size(inst, p1, opc1, opc2, r, basereg, disp, indexreg, shift, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		x86_64_memindex_emit((inst), (r), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * xmm instructions with a prefix and three opcodes
 */
#define x86_64_p1_xmm3_reg_reg_size(inst, p1, opc1, opc2, opc3, r, reg, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, (reg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		*(inst)++ = (unsigned char)(opc3); \
		x86_64_reg_emit(inst, (r), (reg)); \
	} while(0)

#define x86_64_p1_xmm3_reg_regp_size(inst, p1, opc1, opc2, opc3, r, regp, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, (regp)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		*(inst)++ = (unsigned char)(opc3); \
		x86_64_regp_emit(inst, (r), (regp)); \
	} while(0)

#define x86_64_p1_xmm3_reg_mem_size(inst, p1, opc1, opc2, opc3, r, mem, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, 0); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		*(inst)++ = (unsigned char)(opc3); \
		x86_64_mem_emit(inst, (r), (mem)); \
	} while(0)

#define x86_64_p1_xmm3_reg_membase_size(inst, p1, opc1, opc2, opc3, r, basereg, disp, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), 0, (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		*(inst)++ = (unsigned char)(opc3); \
		x86_64_membase_emit(inst, (r), (basereg), (disp)); \
	} while(0)

#define x86_64_p1_xmm3_reg_memindex_size(inst, p1, opc1, opc2, opc3, r, basereg, disp, indexreg, shift, size) \
	do { \
		*(inst)++ = (unsigned char)(p1); \
		x86_64_rex_emit(inst, (size), (r), (indexreg), (basereg)); \
		*(inst)++ = (unsigned char)(opc1); \
		*(inst)++ = (unsigned char)(opc2); \
		*(inst)++ = (unsigned char)(opc3); \
		x86_64_memindex_emit((inst), (r), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * xmm1: Macro for use of the X86_64_XMM1 enum
 */
#define x86_64_xmm1_reg_reg(inst, opc, dreg, sreg, is_double) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), ((is_double) ? 0xf2 : 0xf3), 0x0f, (opc), (dreg), (sreg), 0); \
	} while(0)

#define x86_64_xmm1_reg_regp(inst, opc, dreg, sregp, is_double) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), ((is_double) ? 0xf2 : 0xf3), 0x0f, (opc), (dreg), (sregp), 0); \
	} while(0)

#define x86_64_xmm1_reg_mem(inst, opc, dreg, mem, is_double) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), ((is_double) ? 0xf2 : 0xf3), 0x0f, (opc), (dreg), (mem), 0); \
	} while(0)

#define x86_64_xmm1_reg_membase(inst, opc, dreg, basereg, disp, is_double) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), ((is_double) ? 0xf2 : 0xf3), 0x0f, (opc), (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_xmm1_reg_memindex(inst, opc, dreg, basereg, disp, indexreg, shift, is_double) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), ((is_double) ? 0xf2 : 0xf3), 0x0f, (opc), (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * Load and store MXCSR register state
 */

/*
 * ldmxcsr: Load MXCSR register
 */
#define x86_64_ldmxcsr_regp(inst, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0xae, 2, (sregp)); \
	} while(0)

#define x86_64_ldmxcsr_mem(inst, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0xae, 2, (mem)); \
	} while(0)

#define x86_64_ldmxcsr_membase(inst, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0xae, 2, (basereg), (disp)); \
	} while(0)

#define x86_64_ldmxcsr_memindex(inst, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0xae, 2, (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * stmxcsr: Store MXCSR register
 */
#define x86_64_stmxcsr_regp(inst, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0xae, 3, (sregp)); \
	} while(0)

#define x86_64_stmxcsr_mem(inst, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0xae, 3, (mem)); \
	} while(0)

#define x86_64_stmxcsr_membase(inst, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0xae, 3, (basereg), (disp)); \
	} while(0)

#define x86_64_stmxcsr_memindex(inst, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0xae, 3, (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * Move instructions
 */

/*
 * movd: Move doubleword from/to xmm register
 */
#define x86_64_movd_xreg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x6e, (dreg), (sreg), 4); \
	} while(0)

#define x86_64_movd_xreg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x6e, (dreg), (mem), 4); \
	} while(0)

#define x86_64_movd_xreg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x6e, (dreg), (sregp), 4); \
	} while(0)

#define x86_64_movd_xreg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x6e, (dreg), (basereg), (disp), 4); \
	} while(0)

#define x86_64_movd_xreg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x6e, (dreg), (basereg), (disp), (indexreg), (shift), 4); \
	} while(0)

#define x86_64_movd_reg_xreg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x7e, (sreg), (dreg), 4); \
	} while(0)

#define x86_64_movd_mem_xreg(inst, mem, sreg) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x7e, (sreg), (mem), 4); \
	} while(0)

#define x86_64_movd_regp_xreg(inst, dregp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x7e, (sreg), (dregp), 4); \
	} while(0)

#define x86_64_movd_membase_xreg(inst, basereg, disp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x7e, (sreg), (basereg), (disp), 4); \
	} while(0)

#define x86_64_movd_memindex_xreg(inst, basereg, disp, indexreg, shift, sreg) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x7e, (sreg), (basereg), (disp), (indexreg), (shift), 4); \
	} while(0)

/*
 * movq: Move quadword from/to xmm register
 */
#define x86_64_movq_xreg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x6e, (dreg), (sreg), 8); \
	} while(0)

#define x86_64_movq_xreg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x6e, (dreg), (mem), 8); \
	} while(0)

#define x86_64_movq_xreg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x6e, (dreg), (sregp), 8); \
	} while(0)

#define x86_64_movq_xreg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x6e, (dreg), (basereg), (disp), 8); \
	} while(0)

#define x86_64_movq_xreg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x6e, (dreg), (basereg), (disp), (indexreg), (shift), 8); \
	} while(0)

#define x86_64_movq_reg_xreg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x7e, (sreg), (dreg), 8); \
	} while(0)

#define x86_64_movq_mem_xreg(inst, mem, sreg) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x7e, (sreg), (mem), 8); \
	} while(0)

#define x86_64_movq_regp_xreg(inst, dregp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x7e, (sreg), (dregp), 8); \
	} while(0)

#define x86_64_movq_membase_xreg(inst, basereg, disp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x7e, (sreg), (basereg), (disp), 8); \
	} while(0)

#define x86_64_movq_memindex_xreg(inst, basereg, disp, indexreg, shift, sreg) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x7e, (sreg), (basereg), (disp), (indexreg), (shift), 8); \
	} while(0)

/*
 * movaps: Move aligned quadword (16 bytes)
 */
#define x86_64_movaps_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x28, (dreg), (sreg)); \
	} while(0)

#define x86_64_movaps_regp_reg(inst, dregp, sreg) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x29, (sreg), (dregp)); \
	} while(0)

#define x86_64_movaps_mem_reg(inst, mem, sreg) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x29, (sreg), (mem)); \
	} while(0)

#define x86_64_movaps_membase_reg(inst, basereg, disp, sreg) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x29, (sreg), (basereg), (disp)); \
	} while(0)

#define x86_64_movaps_memindex_reg(inst, basereg, disp, indexreg, shift, sreg) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x29, (sreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

#define x86_64_movaps_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x28, (dreg), (sregp)); \
	} while(0)

#define x86_64_movaps_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x28, (dreg), (mem)); \
	} while(0)

#define x86_64_movaps_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x28, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_movaps_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x28, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * movups: Move unaligned quadword (16 bytes)
 */
#define x86_64_movups_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x10, (dreg), (sreg)); \
	} while(0)

#define x86_64_movups_regp_reg(inst, dregp, sreg) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x11, (sreg), (dregp)); \
	} while(0)

#define x86_64_movups_mem_reg(inst, mem, sreg) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x11, (sreg), (mem)); \
	} while(0)

#define x86_64_movups_membase_reg(inst, basereg, disp, sreg) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x11, (sreg), (basereg), (disp)); \
	} while(0)

#define x86_64_movups_memindex_reg(inst, basereg, disp, indexreg, shift, sreg) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x11, (sreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

#define x86_64_movups_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x10, (dreg), (sregp)); \
	} while(0)

#define x86_64_movups_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x10, (dreg), (mem)); \
	} while(0)

#define x86_64_movups_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x10, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_movups_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x10, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * movlhps: Move lower 64bit of sreg to higher 64bit of dreg
 * movhlps: Move higher 64bit of sreg to lower 64bit of dreg
 */
#define x86_64_movlhps(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x16, (dreg), (sreg)); \
	} while(0)
#define x86_64_movhlps(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x12, (dreg), (sreg)); \
	} while(0)

/*
 * movsd: Move scalar double (64bit float)
 */
#define x86_64_movsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x10, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_movsd_regp_reg(inst, dregp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x11, (sreg), (dregp), 0); \
	} while(0)

#define x86_64_movsd_mem_reg(inst, mem, sreg) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x11, (sreg), (mem), 0); \
	} while(0)

#define x86_64_movsd_membase_reg(inst, basereg, disp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x11, (sreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_movsd_memindex_reg(inst, basereg, disp, indexreg, shift, sreg) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x11, (sreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

#define x86_64_movsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x10, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_movsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x10, (dreg), (mem), 0); \
	} while(0)

#define x86_64_movsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x10, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_movsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x10, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * movss: Move scalar single (32bit float)
 */
#define x86_64_movss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x10, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_movss_regp_reg(inst, dregp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x11, (sreg), (dregp), 0); \
	} while(0)

#define x86_64_movss_mem_reg(inst, mem, sreg) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x11, (sreg), (mem), 0); \
	} while(0)

#define x86_64_movss_membase_reg(inst, basereg, disp, sreg) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x11, (sreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_movss_memindex_reg(inst, basereg, disp, indexreg, shift, sreg) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x11, (sreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

#define x86_64_movss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x10, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_movss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x10, (dreg), (mem), 0); \
	} while(0)

#define x86_64_movss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x10, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_movss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x10, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * Conversion opcodes
 */

/*
 * cvtsi2ss: Convert signed integer to float32
 * The size is the size of the integer value (4 or 8)
 */
#define x86_64_cvtsi2ss_reg_reg_size(inst, dxreg, sreg, size) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x2a, (dxreg), (sreg), (size)); \
	} while(0)

#define x86_64_cvtsi2ss_reg_regp_size(inst, dxreg, sregp, size) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x2a, (dxreg), (sregp), (size)); \
	} while(0)

#define x86_64_cvtsi2ss_reg_mem_size(inst, dxreg, mem, size) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x2a, (dxreg), (mem), (size)); \
	} while(0)

#define x86_64_cvtsi2ss_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x2a, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cvtsi2ss_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x2a, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * cvtsi2sd: Convert signed integer to float64
 * The size is the size of the integer value (4 or 8)
 */
#define x86_64_cvtsi2sd_reg_reg_size(inst, dxreg, sreg, size) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x2a, (dxreg), (sreg), (size)); \
	} while(0)

#define x86_64_cvtsi2sd_reg_regp_size(inst, dxreg, sregp, size) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x2a, (dxreg), (sregp), (size)); \
	} while(0)

#define x86_64_cvtsi2sd_reg_mem_size(inst, dxreg, mem, size) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x2a, (dxreg), (mem), (size)); \
	} while(0)

#define x86_64_cvtsi2sd_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x2a, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cvtsi2sd_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x2a, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * cvtss2si: Convert float32.to a signed integer using the rounding mode
 * in the mxcsr register
 * The size is the size of the integer value (4 or 8)
 */
#define x86_64_cvtss2si_reg_reg_size(inst, dreg, sxreg, size) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x2d, (dreg), (sxreg), (size)); \
	} while(0)

#define x86_64_cvtss2si_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x2d, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_cvtss2si_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x2d, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_cvtss2si_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x2d, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cvtss2si_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x2d, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * cvttss2si: Convert float32.to a signed integer using the truncate rounding mode.
 * The size is the size of the integer value (4 or 8)
 */
#define x86_64_cvttss2si_reg_reg_size(inst, dreg, sxreg, size) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x2c, (dreg), (sxreg), (size)); \
	} while(0)

#define x86_64_cvttss2si_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x2c, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_cvttss2si_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x2c, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_cvttss2si_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x2c, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cvttss2si_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x2c, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * cvtsd2si: Convert float64 to a signed integer using the rounding mode
 * in the mxcsr register
 * The size is the size of the integer value (4 or 8)
 */
#define x86_64_cvtsd2si_reg_reg_size(inst, dreg, sxreg, size) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x2d, (dreg), (sxreg), (size)); \
	} while(0)

#define x86_64_cvtsd2si_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x2d, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_cvtsd2si_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x2d, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_cvtsd2si_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x2d, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cvtsd2si_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x2d, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * cvttsd2si: Convert float64 to a signed integer using the truncate rounding mode.
 * The size is the size of the integer value (4 or 8)
 */
#define x86_64_cvttsd2si_reg_reg_size(inst, dreg, sxreg, size) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x2c, (dreg), (sxreg), (size)); \
	} while(0)

#define x86_64_cvttsd2si_reg_regp_size(inst, dreg, sregp, size) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x2c, (dreg), (sregp), (size)); \
	} while(0)

#define x86_64_cvttsd2si_reg_mem_size(inst, dreg, mem, size) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x2c, (dreg), (mem), (size)); \
	} while(0)

#define x86_64_cvttsd2si_reg_membase_size(inst, dreg, basereg, disp, size) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x2c, (dreg), (basereg), (disp), (size)); \
	} while(0)

#define x86_64_cvttsd2si_reg_memindex_size(inst, dreg, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x2c, (dreg), (basereg), (disp), (indexreg), (shift), (size)); \
	} while(0)

/*
 * cvtss2sd: Convert float32 to float64
 */
#define x86_64_cvtss2sd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x5a, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_cvtss2sd_reg_regp(inst, dxreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x5a, (dxreg), (sregp), 0); \
	} while(0)

#define x86_64_cvtss2sd_reg_mem(inst, dxreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x5a, (dxreg), (mem), 0); \
	} while(0)

#define x86_64_cvtss2sd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x5a, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_cvtss2sd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x5a, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * cvtsd2ss: Convert float64 to float32
 */
#define x86_64_cvtsd2ss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x5a, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_cvtsd2ss_reg_regp(inst, dxreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x5a, (dxreg), (sregp), 0); \
	} while(0)

#define x86_64_cvtsd2ss_reg_mem(inst, dxreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x5a, (dxreg), (mem), 0); \
	} while(0)

#define x86_64_cvtsd2ss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x5a, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_cvtsd2ss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x5a, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * Compare opcodes
 */

/*
 * comiss: Compare ordered scalar single precision values
 */
#define x86_64_comiss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x2f, (dreg), (sreg)); \
	} while(0)

#define x86_64_comiss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x2f, (dreg), (sregp)); \
	} while(0)

#define x86_64_comiss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x2f, (dreg), (mem)); \
	} while(0)

#define x86_64_comiss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x2f, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_comiss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x2f, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * comisd: Compare ordered scalar double precision values
 */
#define x86_64_comisd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x2f, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_comisd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x2f, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_comisd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x2f, (dreg), (mem), 0); \
	} while(0)

#define x86_64_comisd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x2f, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_comisd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x2f, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * ucomiss: Compare unordered scalar single precision values
 */
#define x86_64_ucomiss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x2e, (dreg), (sreg)); \
	} while(0)

#define x86_64_ucomiss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x2e, (dreg), (sregp)); \
	} while(0)

#define x86_64_ucomiss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x2e, (dreg), (mem)); \
	} while(0)

#define x86_64_ucomiss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x2e, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_ucomiss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x2e, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * ucomisd: Compare unordered scalar double precision values
 */
#define x86_64_ucomisd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x2e, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_ucomisd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x2e, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_ucomisd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x2e, (dreg), (mem), 0); \
	} while(0)

#define x86_64_ucomisd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x2e, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_ucomisd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x2e, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * Arithmetic opcodes
 */

/*
 * addss: Add scalar single precision float values
 */
#define x86_64_addss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x58, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_addss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x58, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_addss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x58, (dreg), (mem), 0); \
	} while(0)

#define x86_64_addss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x58, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_addss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x58, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * subss: Substract scalar single precision float values
 */
#define x86_64_subss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x5c, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_subss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x5c, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_subss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x5c, (dreg), (mem), 0); \
	} while(0)

#define x86_64_subss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x5c, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_subss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x5c, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * mulss: Multiply scalar single precision float values
 */
#define x86_64_mulss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x59, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_mulss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x59, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_mulss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x59, (dreg), (mem), 0); \
	} while(0)

#define x86_64_mulss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x59, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_mulss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x59, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * divss: Divide scalar single precision float values
 */
#define x86_64_divss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x5e, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_divss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x5e, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_divss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x5e, (dreg), (mem), 0); \
	} while(0)

#define x86_64_divss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x5e, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_divss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x5e, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * Macros for the logical operations with packed single precision values.
 */
#define x86_64_plops_reg_reg(inst, op, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, (op), (dreg), (sreg)); \
	} while(0)

#define x86_64_plops_reg_regp(inst, op, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, (op), (dreg), (sregp)); \
	} while(0)

#define x86_64_plops_reg_mem(inst, op, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, (op), (dreg), (mem)); \
	} while(0)

#define x86_64_plops_reg_membase(inst, op, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, (op), (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_plops_reg_memindex(inst, op, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, (op), (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * andps: And
 */
#define x86_64_andps_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x54, (dreg), (sreg)); \
	} while(0)

#define x86_64_andps_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x54, (dreg), (sregp)); \
	} while(0)

#define x86_64_andps_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x54, (dreg), (mem)); \
	} while(0)

#define x86_64_andps_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x54, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_andps_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x54, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * orps: Or
 */
#define x86_64_orps_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x56, (dreg), (sreg)); \
	} while(0)

#define x86_64_orps_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x56, (dreg), (sregp)); \
	} while(0)

#define x86_64_orps_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x56, (dreg), (mem)); \
	} while(0)

#define x86_64_orps_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x56, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_orps_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x56, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * xorps: Xor
 */
#define x86_64_xorps_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_xmm2_reg_reg((inst), 0x0f, 0x57, (dreg), (sreg)); \
	} while(0)

#define x86_64_xorps_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_xmm2_reg_regp((inst), 0x0f, 0x57, (dreg), (sregp)); \
	} while(0)

#define x86_64_xorps_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_xmm2_reg_mem((inst), 0x0f, 0x57, (dreg), (mem)); \
	} while(0)

#define x86_64_xorps_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_xmm2_reg_membase((inst), 0x0f, 0x57, (dreg), (basereg), (disp)); \
	} while(0)

#define x86_64_xorps_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex((inst), 0x0f, 0x57, (dreg), (basereg), (disp), (indexreg), (shift)); \
	} while(0)

/*
 * maxss: Maximum value
 */
#define x86_64_maxss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x5f, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_maxss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x5f, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_maxss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x5f, (dreg), (mem), 0); \
	} while(0)

#define x86_64_maxss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x5f, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_maxss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x5f, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * minss: Minimum value
 */
#define x86_64_minss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x5d, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_minss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x5d, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_minss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x5d, (dreg), (mem), 0); \
	} while(0)

#define x86_64_minss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x5d, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_minss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x5d, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * sqrtss: Square root
 */
#define x86_64_sqrtss_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf3, 0x0f, 0x51, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_sqrtss_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf3, 0x0f, 0x51, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_sqrtss_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf3, 0x0f, 0x51, (dreg), (mem), 0); \
	} while(0)

#define x86_64_sqrtss_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf3, 0x0f, 0x51, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_sqrtss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf3, 0x0f, 0x51, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)


/*
 * Macros for the logical operations with packed double precision values.
 */
#define x86_64_plopd_reg_reg(inst, op, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, (op), (dreg), (sreg), 0); \
	} while(0)

#define x86_64_plopd_reg_regp(inst, op, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, (op), (dreg), (sregp), 0); \
	} while(0)

#define x86_64_plopd_reg_mem(inst, op, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, (op), (dreg), (mem), 0); \
	} while(0)

#define x86_64_plopd_reg_membase(inst, op, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, (op), (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_plopd_reg_memindex(inst, op, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_xmm2_reg_memindex_size((inst), 0x66, 0x0f, (op), (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * addsd: Add scalar double precision float values
 */
#define x86_64_addsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x58, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_addsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x58, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_addsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x58, (dreg), (mem), 0); \
	} while(0)

#define x86_64_addsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x58, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_addsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x58, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * subsd: Substract scalar double precision float values
 */
#define x86_64_subsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x5c, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_subsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x5c, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_subsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x5c, (dreg), (mem), 0); \
	} while(0)

#define x86_64_subsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x5c, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_subsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x5c, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * mulsd: Multiply scalar double precision float values
 */
#define x86_64_mulsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x59, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_mulsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x59, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_mulsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x59, (dreg), (mem), 0); \
	} while(0)

#define x86_64_mulsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x59, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_mulsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x59, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * divsd: Divide scalar double precision float values
 */
#define x86_64_divsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x5e, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_divsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x5e, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_divsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x5e, (dreg), (mem), 0); \
	} while(0)

#define x86_64_divsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x5e, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_divsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x5e, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * andpd: And
 */
#define x86_64_andpd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x54, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_andpd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x54, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_andpd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x54, (dreg), (mem), 0); \
	} while(0)

#define x86_64_andpd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x54, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_andpd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x54, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * orpd: Or
 */
#define x86_64_orpd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x56, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_orpd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x56, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_orpd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x56, (dreg), (mem), 0); \
	} while(0)

#define x86_64_orpd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x56, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_orpd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x56, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * xorpd: Xor
 */
#define x86_64_xorpd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0x66, 0x0f, 0x57, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_xorpd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0x66, 0x0f, 0x57, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_xorpd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0x66, 0x0f, 0x57, (dreg), (mem), 0); \
	} while(0)

#define x86_64_xorpd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0x66, 0x0f, 0x57, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_xorpd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0x66, 0x0f, 0x57, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * maxsd: Maximum value
 */
#define x86_64_maxsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x5f, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_maxsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x5f, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_maxsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x5f, (dreg), (mem), 0); \
	} while(0)

#define x86_64_maxsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x5f, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_maxsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x5f, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * minsd: Minimum value
 */
#define x86_64_minsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x5d, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_minsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x5d, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_minsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x5d, (dreg), (mem), 0); \
	} while(0)

#define x86_64_minsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x5d, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_minsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2 0x0f, 0x5d, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * sqrtsd: Square root
 */
#define x86_64_sqrtsd_reg_reg(inst, dreg, sreg) \
	do { \
		x86_64_p1_xmm2_reg_reg_size((inst), 0xf2, 0x0f, 0x51, (dreg), (sreg), 0); \
	} while(0)

#define x86_64_sqrtsd_reg_regp(inst, dreg, sregp) \
	do { \
		x86_64_p1_xmm2_reg_regp_size((inst), 0xf2, 0x0f, 0x51, (dreg), (sregp), 0); \
	} while(0)

#define x86_64_sqrtsd_reg_mem(inst, dreg, mem) \
	do { \
		x86_64_p1_xmm2_reg_mem_size((inst), 0xf2, 0x0f, 0x51, (dreg), (mem), 0); \
	} while(0)

#define x86_64_sqrtsd_reg_membase(inst, dreg, basereg, disp) \
	do { \
		x86_64_p1_xmm2_reg_membase_size((inst), 0xf2, 0x0f, 0x51, (dreg), (basereg), (disp), 0); \
	} while(0)

#define x86_64_sqrtsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift) \
	do { \
		x86_64_p1_xmm2_reg_memindex_size((inst), 0xf2, 0x0f, 0x51, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
	} while(0)

/*
 * Rounding: Available in SSE 4.1 only
 */

/*
 * roundss: Round scalar single precision value
 */
#define x86_64_roundss_reg_reg(inst, dreg, sreg, mode) \
	do { \
		x86_64_p1_xmm3_reg_reg_size((inst), 0x66, 0x0f, 0x3a, 0x0a, (dreg), (sreg), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundss_reg_regp(inst, dreg, sregp, mode) \
	do { \
		x86_64_p1_xmm3_reg_regp_size((inst), 0x66, 0x0f, 0x3a, 0x0a, (dreg), (sregp), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundss_reg_mem(inst, dreg, mem, mode) \
	do { \
		x86_64_p1_xmm3_reg_mem_size((inst), 0x66, 0x0f, 0x3a, 0x0a, (dreg), (mem), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundss_reg_membase(inst, dreg, basereg, disp, mode) \
	do { \
		x86_64_p1_xmm3_reg_membase_size((inst), 0x66, 0x0f, 0x3a, 0x0a, (dreg), (basereg), (disp), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundss_reg_memindex(inst, dreg, basereg, disp, indexreg, shift, mode) \
	do { \
		x86_64_p1_xmm3_reg_memindex_size((inst), 0x66, 0x0f, 0x3a, 0x0a, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

/*
 * roundsd: Round scalar double precision value
 */
#define x86_64_roundsd_reg_reg(inst, dreg, sreg, mode) \
	do { \
		x86_64_p1_xmm3_reg_reg_size((inst), 0x66, 0x0f, 0x3a, 0x0b, (dreg), (sreg), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundsd_reg_regp(inst, dreg, sregp, mode) \
	do { \
		x86_64_p1_xmm3_reg_regp_size((inst), 0x66, 0x0f, 0x3a, 0x0b, (dreg), (sregp), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundsd_reg_mem(inst, dreg, mem, mode) \
	do { \
		x86_64_p1_xmm3_reg_mem_size((inst), 0x66, 0x0f, 0x3a, 0x0b, (dreg), (mem), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundsd_reg_membase(inst, dreg, basereg, disp, mode) \
	do { \
		x86_64_p1_xmm3_reg_membase_size((inst), 0x66, 0x0f, 0x3a, 0x0b, (dreg), (basereg), (disp), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

#define x86_64_roundsd_reg_memindex(inst, dreg, basereg, disp, indexreg, shift, mode) \
	do { \
		x86_64_p1_xmm3_reg_memindex_size((inst), 0x66, 0x0f, 0x3a, 0x0b, (dreg), (basereg), (disp), (indexreg), (shift), 0); \
		x86_imm_emit8((inst), (mode)); \
	} while(0)

/*
 * Clear xmm register
 */
#define x86_64_clear_xreg(inst, reg) \
	do { \
		x86_64_xorps_reg_reg((inst), (reg), (reg)); \
	} while(0)

/*
 * fpu instructions
 */

/*
 * fld
 */

#define x86_64_fld_regp_size(inst, sregp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (sregp)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_regp_emit((inst), 0, (sregp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_regp_emit((inst), 0, (sregp)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_regp_emit((inst), 5, (sregp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fld_mem_size(inst, mem, size) \
	do { \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_mem_emit((inst), 0, (mem)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_mem_emit((inst), 0, (mem)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_mem_emit((inst), 5, (mem)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fld_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_membase_emit((inst), 0, (basereg), (disp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_membase_emit((inst), 0, (basereg), (disp)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_membase_emit((inst), 5, (basereg), (disp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fld_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, (indexreg), (basereg)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_memindex_emit((inst), 0, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_memindex_emit((inst), 0, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_memindex_emit((inst), 5, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
		} \
	} while(0)

/*
 * fild: Load an integer and convert it to long double
 */
#define x86_64_fild_mem_size(inst, mem, size) \
	do { \
		switch(size) \
		{ \
			case 2: \
			{ \
			    *(inst)++ = (unsigned char)0xdf; \
				x86_64_mem_emit((inst), 0, (mem));	\
			} \
			break; \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_mem_emit((inst), 0, (mem));	\
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdf; \
				x86_64_mem_emit((inst), 5, (mem));	\
			} \
			break; \
		} \
	} while (0)

#define x86_64_fild_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		switch(size) \
		{ \
			case 2: \
			{ \
			    *(inst)++ = (unsigned char)0xdf; \
				x86_64_membase_emit((inst), 0, (basereg), (disp)); \
			} \
			break; \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_membase_emit((inst), 0, (basereg), (disp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdf; \
				x86_64_membase_emit((inst), 5, (basereg), (disp)); \
			} \
			break; \
		} \
	} while (0)

/*
 * fst: Store fpu register to memory (only float32 and float64 allowed)
 */

#define x86_64_fst_regp_size(inst, sregp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (sregp)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_regp_emit((inst), 2, (sregp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_regp_emit((inst), 2, (sregp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fst_mem_size(inst, mem, size) \
	do { \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_mem_emit((inst), 2, (mem)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_mem_emit((inst), 2, (mem)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fst_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_membase_emit((inst), 2, (basereg), (disp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_membase_emit((inst), 2, (basereg), (disp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fst_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, (indexreg), (basereg)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_memindex_emit((inst), 2, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_memindex_emit((inst), 2, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
		} \
	} while(0)

/*
 * fstp: store top fpu register to memory and pop it from the fpu stack
 */
#define x86_64_fstp_regp_size(inst, sregp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (sregp)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_regp_emit((inst), 3, (sregp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_regp_emit((inst), 3, (sregp)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_regp_emit((inst), 7, (sregp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fstp_mem_size(inst, mem, size) \
	do { \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_mem_emit((inst), 3, (mem)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_mem_emit((inst), 3, (mem)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_mem_emit((inst), 7, (mem)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fstp_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_membase_emit((inst), 3, (basereg), (disp)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_membase_emit((inst), 3, (basereg), (disp)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_membase_emit((inst), 7, (basereg), (disp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fstp_memindex_size(inst, basereg, disp, indexreg, shift, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, (indexreg), (basereg)); \
		switch(size) \
		{ \
			case 4: \
			{ \
			    *(inst)++ = (unsigned char)0xd9; \
				x86_64_memindex_emit((inst), 3, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 8: \
			{ \
			    *(inst)++ = (unsigned char)0xdd; \
				x86_64_memindex_emit((inst), 3, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
			case 10: \
			{ \
			    *(inst)++ = (unsigned char)0xdb; \
				x86_64_memindex_emit((inst), 7, (basereg), (disp), (indexreg), (shift)); \
			} \
			break; \
		} \
	} while(0)

/*
 * fistp: Convert long double to integer
 */
#define x86_64_fistp_mem_size(inst, mem, size) \
	do { \
		switch((size)) \
		{ \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_mem_emit((inst), 3, (mem)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xdb; \
				x86_64_mem_emit((inst), 3, (mem)); \
			} \
			break; \
			case 8: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_mem_emit((inst), 7, (mem)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fistp_regp_size(inst, dregp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (dregp)); \
		switch((size)) \
		{ \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_regp_emit((inst), 3, (dregp)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xdb; \
				x86_64_regp_emit((inst), 3, (dregp)); \
			} \
			break; \
			case 8: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_regp_emit((inst), 7, (dregp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fistp_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		switch((size)) \
		{ \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_membase_emit((inst), 3, (basereg), (disp)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xdb; \
				x86_64_membase_emit((inst), 3, (basereg), (disp)); \
			} \
			break; \
			case 8: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_membase_emit((inst), 7, (basereg), (disp)); \
			} \
			break; \
		} \
	} while(0)

/*
 * frndint: Round st(0) to integer according to the rounding mode set in the fpu control word.
 */
#define x86_64_frndint(inst) \
	do { \
		*(inst)++ = (unsigned char)0xd9; \
		*(inst)++ = (unsigned char)0xfc; \
	} while(0)

/*
 * fisttp: Convert long double to integer using truncation as rounding mode Available in SSE 3 only
 */
#define x86_64_fisttp_regp_size(inst, dregp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (dregp)); \
		switch((size)) \
		{ \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_regp_emit((inst), 1, (dregp)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xdb; \
				x86_64_regp_emit((inst), 1, (dregp)); \
			} \
			break; \
			case 8: \
			{ \
				*(inst)++ = (unsigned char)0xdd; \
				x86_64_regp_emit((inst), 1, (dregp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fisttp_mem_size(inst, mem, size) \
	do { \
		switch((size)) \
		{ \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_mem_emit((inst), 1, (mem)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xdb; \
				x86_64_mem_emit((inst), 1, (mem)); \
			} \
			break; \
			case 8: \
			{ \
				*(inst)++ = (unsigned char)0xdd; \
				x86_64_mem_emit((inst), 1, (mem)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fisttp_membase_size(inst, basereg, disp, size) \
	do { \
		x86_64_rex_emit((inst), 0, 0, 0, (basereg)); \
		switch((size)) \
		{ \
			case 2: \
			{ \
				*(inst)++ = (unsigned char)0xdf; \
				x86_64_membase_emit((inst), 1, (basereg), (disp)); \
			} \
			break; \
			case 4: \
			{ \
				*(inst)++ = (unsigned char)0xdb; \
				x86_64_membase_emit((inst), 1, (basereg), (disp)); \
			} \
			break; \
			case 8: \
			{ \
				*(inst)++ = (unsigned char)0xdd; \
				x86_64_membase_emit((inst), 1, (basereg), (disp)); \
			} \
			break; \
		} \
	} while(0)

#define x86_64_fabs(inst) \
	do { \
		*(inst)++ = (unsigned char)0xd9; \
		*(inst)++ = (unsigned char)0xe1; \
	} while(0)

#define x86_64_fchs(inst)	\
	do {	\
		*(inst)++ = (unsigned char)0xd9;	\
		*(inst)++ = (unsigned char)0xe0;	\
	} while(0)

/*
 * Store fpu control word after checking for pending unmasked fpu exceptions
 */
#define x86_64_fnstcw(inst, mem) \
	do { \
		*(inst)++ = (unsigned char)0xd9; \
		x86_64_mem_emit((inst), 7, (mem)); \
	} while(0)

#define x86_64_fnstcw_membase(inst, basereg, disp) \
	do { \
		*(inst)++ = (unsigned char)0xd9; \
		x86_64_membase_emit((inst), 7, (basereg), (disp)); \
	} while(0)

/*
 * Load fpu control word
 */
#define x86_64_fldcw(inst, mem) \
	do { \
		*(inst)++ = (unsigned char)0xd9; \
		x86_64_mem_emit((inst), 5, (mem)); \
	} while(0)

#define x86_64_fldcw_membase(inst, basereg, disp) \
	do { \
		*(inst)++ = (unsigned char)0xd9; \
		x86_64_membase_emit ((inst), 5, (basereg), (disp)); \
	} while(0)

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_GEN_X86_64_H */
