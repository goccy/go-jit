/*
 * jit-apply-func.h - Definition of "__builtin_apply" and friends.
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

#ifndef	_JIT_APPLY_FUNC_H
#define	_JIT_APPLY_FUNC_H

#if defined(__i386) || defined(__i386__) || defined(_M_IX86)

#include "jit-apply-x86.h"

#elif defined(__arm) || defined(__arm__)

#include "jit-apply-arm.h"

#elif defined(__alpha) || defined(__alpha__)

#include "jit-apply-alpha.h"

#elif defined(__x86_64) || defined(__x86_64__)

#include "jit-apply-x86-64.h"

#endif

#if !defined(jit_builtin_apply)
#define	jit_builtin_apply(func,args,size,return_float,return_buf)	\
		do { \
			(return_buf) = __builtin_apply \
					((void (*)())(func), (args), (size)); \
		} while (0)
#endif

#if !defined(jit_builtin_apply_args)
#define	jit_builtin_apply_args(type,args)	\
		do { \
			(args) = (type)__builtin_apply_args(); \
		} while (0)
#endif

#if !defined(jit_builtin_return_int)
#define	jit_builtin_return_int(return_buf)	\
		do { \
			__builtin_return((return_buf)); \
		} while (0)
#endif

#if !defined(jit_builtin_return_long)
#define	jit_builtin_return_long(return_buf)	\
		do { \
			__builtin_return((return_buf)); \
		} while (0)
#endif

#if !defined(jit_builtin_return_float)
#define	jit_builtin_return_float(return_buf)	\
		do { \
			__builtin_return((return_buf)); \
		} while (0)
#endif

#if !defined(jit_builtin_return_double)
#define	jit_builtin_return_double(return_buf)	\
		do { \
			__builtin_return((return_buf)); \
		} while (0)
#endif

#if !defined(jit_builtin_return_nfloat)
#define	jit_builtin_return_nfloat(return_buf)	\
		do { \
			__builtin_return((return_buf)); \
		} while (0)
#endif

/*
 * Create a closure for the underlying platform in the given buffer.
 * The closure must arrange to call "func" with two arguments:
 * "closure" and a pointer to an apply structure.
 */
void _jit_create_closure(unsigned char *buf, void *func,
                         void *closure, void *type);

/*
 * Create a redirector stub for the underlying platform in the given buffer.
 * The redirector arranges to call "func" with the "user_data" argument.
 * It is assumed that "func" returns a pointer to the actual function.
 * Returns a pointer to the position in "buf" where the redirector starts,
 * which may be different than "buf" if alignment occurred.
 */
void *_jit_create_redirector(unsigned char *buf, void *func,
							 void *user_data, int abi);


/*
 * Create the indirector for the function.
 */
void *_jit_create_indirector(unsigned char *buf, void **entry);

/*
 * Pad a buffer with NOP instructions.  Used to align code.
 * This will only be called if "jit_should_pad" is defined.
 */
void _jit_pad_buffer(unsigned char *buf, int len);

#endif	/* _JIT_APPLY_FUNC_H */
