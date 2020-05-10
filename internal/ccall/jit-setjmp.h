/*
 * jit-setjmp.h - Support definitions that use "setjmp" for exceptions.
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

#ifndef	_JIT_SETJMP_H
#define	_JIT_SETJMP_H

#include <setjmp.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Jump buffer structure, with link.
 */
typedef struct jit_jmp_buf
{
	jmp_buf				buf;
	jit_backtrace_t		trace;
	void			   *catch_pc;
	struct jit_jmp_buf *parent;

} jit_jmp_buf;
#define	jit_jmp_catch_pc_offset	\
			((jit_nint)&(((jit_jmp_buf *)0)->catch_pc))

/*
 * Push a "setjmp" buffer onto the current thread's unwind stck.
 */
void _jit_unwind_push_setjmp(jit_jmp_buf *jbuf);

/*
 * Pop the top-most "setjmp" buffer from the current thread's unwind stack.
 */
void _jit_unwind_pop_setjmp(void);

/*
 * Pop the top-most "setjmp" buffer and rethrow the current exception.
 */
void _jit_unwind_pop_and_rethrow(void);

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_SETJMP_H */
