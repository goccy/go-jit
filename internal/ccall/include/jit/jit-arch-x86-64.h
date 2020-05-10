/*
 * jit-arch-x86.h - Architecture-specific definitions.
 *
 * Copyright (C) 2008  Southern Storm Software, Pty Ltd.
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

#ifndef	_JIT_ARCH_X86_64_H
#define	_JIT_ARCH_X86_64_H

/*
 * The frame header structure for X86_64
 */
typedef struct _jit_arch_frame _jit_arch_frame_t;
struct _jit_arch_frame
{
	_jit_arch_frame_t *next_frame;
	void *return_address;
};

/*
 * If defined _JIT_ARCH_GET_CURRENT_FRAME() macro assigns the current frame
 * pointer to the supplied argument that has to be a void pointer.
 */
#if defined(__GNUC__)
#define _JIT_ARCH_GET_CURRENT_FRAME(f)		\
	do {					\
		register void *__f asm("rbp");	\
		f = __f;			\
	} while(0)
#elif defined(_MSC_VER) && defined(_M_IX86)
#define	_JIT_ARCH_GET_CURRENT_FRAME(f)		\
	do {					\
		void *__ptr;			\
		__asm				\
		{				\
			__asm mov qword ptr __ptr, rbp	\
		}				\
		(f) = __ptr;			\
	} while(0)
#else
#undef _JIT_ARCH_GET_CURRENT_FRAME
#endif

/*
 * If defined _JIT_ARCH_GET_NEXT_FRAME() assigns the frame address following
 * the frame supplied as second arg to the value supplied as first argument.
 */
#define _JIT_ARCH_GET_NEXT_FRAME(n, f)							\
	do {										\
		(n) = (void *)((f) ? ((_jit_arch_frame_t *)(f))->next_frame : 0);	\
	} while(0)

/*
 * If defined _JIT_ARCH_GET_RETURN_ADDRESS() assigns the return address of
 * the frame supplied as second arg to the value supplied as first argument.
 */
#define _JIT_ARCH_GET_RETURN_ADDRESS(r, f)						\
	do {										\
		(r) = (void *)((f) ? ((_jit_arch_frame_t *)(f))->return_address : 0);	\
	} while(0)

/*
 * If defined _JIT_ARCH_GET_CURRENT_RETURN() assigns the return address of
 * the current to the supplied argument.
 */
#define _JIT_ARCH_GET_CURRENT_RETURN(r)				\
	do {							\
		void *__frame;					\
		_JIT_ARCH_GET_CURRENT_FRAME(__frame);		\
		_JIT_ARCH_GET_RETURN_ADDRESS((r), __frame);	\
	} while(0)

#endif /* _JIT_ARCH_X86_64_H */
