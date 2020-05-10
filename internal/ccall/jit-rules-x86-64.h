/*
 * jit-rules-x86-64.h - Rules that define the characteristics of the x86_64.
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

#ifndef	_JIT_RULES_X86_64_H
#define	_JIT_RULES_X86_64_H

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Information about all of the registers, in allocation order.
 */
#define	JIT_REG_X86_64_FLOAT	\
	(JIT_REG_FLOAT32 | JIT_REG_FLOAT64 | JIT_REG_NFLOAT)
#define	JIT_REG_X86_64_XMM	\
	(JIT_REG_FLOAT32 | JIT_REG_FLOAT64)
#define JIT_REG_X86_64_GENERAL	\
	(JIT_REG_WORD | JIT_REG_LONG)
#define	JIT_REG_INFO	\
	{"rax", 0, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"rcx", 1, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"rdx", 2, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"rbx", 3, -1, JIT_REG_X86_64_GENERAL | JIT_REG_GLOBAL}, \
	{"rsi", 6, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"rdi", 7, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"r8",  8, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"r9",  9, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"r10", 10, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"r11", 11, -1, JIT_REG_X86_64_GENERAL | JIT_REG_CALL_USED}, \
	{"r12", 12, -1, JIT_REG_X86_64_GENERAL | JIT_REG_GLOBAL}, \
	{"r13", 13, -1, JIT_REG_X86_64_GENERAL | JIT_REG_GLOBAL}, \
	{"r14", 14, -1, JIT_REG_X86_64_GENERAL | JIT_REG_GLOBAL}, \
	{"r15", 15, -1, JIT_REG_X86_64_GENERAL | JIT_REG_GLOBAL}, \
	{"rbp", 5, -1, JIT_REG_FRAME | JIT_REG_FIXED | JIT_REG_CALL_USED}, \
	{"rsp", 4, -1, JIT_REG_STACK_PTR | JIT_REG_FIXED | JIT_REG_CALL_USED}, \
	{"xmm0", 0, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm1", 1, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm2", 2, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm3", 3, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm4", 4, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm5", 5, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm6", 6, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm7", 7, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm8", 8, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm9", 9, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm10", 10, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm11", 11, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm12", 12, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm13", 13, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm14", 14, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"xmm15", 15, -1, JIT_REG_X86_64_XMM | JIT_REG_CALL_USED}, \
	{"st0", 0, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st1", 1, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st2", 2, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st3", 3, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st4", 4, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st5", 5, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st6", 6, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st7", 7, -1, JIT_REG_X86_64_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK},
#define	JIT_NUM_REGS		40
#define	JIT_NUM_GLOBAL_REGS	5

#define JIT_REG_STACK		1
#define JIT_REG_STACK_START	32
#define JIT_REG_STACK_END	39

/*
 * Define to 1 if we should always load values into registers
 * before operating on them.  i.e. the CPU does not have reg-mem
 * and mem-reg addressing modes.
 */
#define	JIT_ALWAYS_REG_REG		0

/*
 * The maximum number of bytes to allocate for the prolog.
 * This may be shortened once we know the true prolog size.
 */
#define	JIT_PROLOG_SIZE			64

/*
 * Preferred alignment for the start of functions.
 */
#define	JIT_FUNCTION_ALIGNMENT		32

/*
 * Define this to 1 if the platform allows reads and writes on
 * any byte boundary.  Define to 0 if only properly-aligned
 * memory accesses are allowed.
 */
#define	JIT_ALIGN_OVERRIDES		1

/*
 * Extra state information that is added to the "jit_gencode" structure.
 */

#define jit_extra_gen_state	\
	void *alloca_fixup

#define jit_extra_gen_init(gen)	\
	do {	\
		(gen)->alloca_fixup = 0;	\
	} while (0)

#define jit_extra_gen_cleanup(gen)	do { ; } while (0)

/*
 * Parameter passing rules.
 */
#define	JIT_INITIAL_STACK_OFFSET	(2 * sizeof(void *))
#define	JIT_INITIAL_FRAME_SIZE		0

/*
 * We are using the param area on x86_64
 */
#define JIT_USE_PARAM_AREA

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_RULES_X86_64_H */
