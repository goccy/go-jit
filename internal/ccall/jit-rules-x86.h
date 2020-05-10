/*
 * jit-rules-x86.h - Rules that define the characteristics of the x86.
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

#ifndef	_JIT_RULES_X86_H
#define	_JIT_RULES_X86_H

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Information about all of the registers, in allocation order.
 */
#define	JIT_REG_X86_FLOAT	\
	(JIT_REG_FLOAT32 | JIT_REG_FLOAT64 | JIT_REG_NFLOAT)
#define	JIT_REG_INFO	\
	{"eax", 0, 2, JIT_REG_WORD | JIT_REG_LONG | JIT_REG_CALL_USED}, \
	{"ecx", 1, 3, JIT_REG_WORD | JIT_REG_LONG | JIT_REG_CALL_USED}, \
	{"edx", 2, -1, JIT_REG_WORD | JIT_REG_CALL_USED}, \
	{"ebx", 3, -1, JIT_REG_WORD | JIT_REG_GLOBAL}, \
	{"esi", 6, -1, JIT_REG_WORD | JIT_REG_GLOBAL}, \
	{"edi", 7, -1, JIT_REG_WORD | JIT_REG_GLOBAL}, \
	{"ebp", 4, -1, JIT_REG_FRAME | JIT_REG_FIXED}, \
	{"esp", 5, -1, JIT_REG_STACK_PTR | JIT_REG_FIXED | JIT_REG_CALL_USED}, \
	{"st",  0, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st1", 1, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st2", 2, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st3", 3, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st4", 4, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st5", 5, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st6", 6, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st7", 7, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK},
#define	JIT_NUM_REGS		16
#define	JIT_NUM_GLOBAL_REGS	3

#define JIT_REG_STACK		1
#define JIT_REG_STACK_START	8
#define JIT_REG_STACK_END	15

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
#define	JIT_PROLOG_SIZE			32

/*
 * Preferred alignment for the start of functions.
 */
#define	JIT_FUNCTION_ALIGNMENT	32

/*
 * Define this to 1 if the platform allows reads and writes on
 * any byte boundary.  Define to 0 if only properly-aligned
 * memory accesses are allowed.
 */
#define	JIT_ALIGN_OVERRIDES		1

/*
 * Parameter passing rules.
 */
#define	JIT_CDECL_WORD_REG_PARAMS		{-1}
#define	JIT_FASTCALL_WORD_REG_PARAMS	{1, 2, -1}	/* ecx, edx */
#define	JIT_MAX_WORD_REG_PARAMS			2
#define	JIT_INITIAL_STACK_OFFSET		(2 * sizeof(void *))
#define	JIT_INITIAL_FRAME_SIZE			0

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_RULES_X86_H */
