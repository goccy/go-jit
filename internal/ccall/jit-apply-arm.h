/*
 * jit-apply-arm.h - Special definitions for ARM function application.
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

#ifndef	_JIT_APPLY_ARM_H
#define	_JIT_APPLY_ARM_H

/*
 * The maximum number of bytes that are needed to represent a closure,
 * and the alignment to use for the closure.
 */
#define	jit_closure_size		128
#define	jit_closure_align		16

/*
 * The number of bytes that are needed for a redirector stub.
 * This includes any extra bytes that are needed for alignment.
 */
#define	jit_redirector_size		128

/*
 * The number of bytes that are needed for a indirector stub.
 * This includes any extra bytes that are needed for alignment.
 */
#define	jit_indirector_size		24

/*
 * We should pad unused code space with NOP's.
 */
#define	jit_should_pad			1

/*
 * Defines the alignment for the stack pointer at a public interface.
 * As of the "Procedure Call Standard for the ARM Architecture" (AAPCS release 2.07)
 *    SP mod 8 = 0
 * must always be true at every public interface (function calls, etc)
 */
#define JIT_SP_ALIGN_PUBLIC 8

/*
 * Redefine jit_builtin_apply in order to correctly align the stack pointer
 * to JIT_SP_ALING_PUBLIC bytes before calling __builtin_apply to execute the
 * jit-compiled function
 */
#define	jit_builtin_apply(func,args,size,return_float,return_buf)	\
do {									\
	register void *sp asm("sp");					\
	sp = (void *) (((unsigned) sp) & ~(JIT_SP_ALIGN_PUBLIC - 1));	\
	(return_buf) = __builtin_apply					\
	((void (*)())(func), (args), (size));				\
} while (0)

#endif	/* _JIT_APPLY_ARM_H */
