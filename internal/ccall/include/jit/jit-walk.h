/*
 * jit-walk.h - Functions for walking stack frames.
 *
 * Copyright (C) 2004  Southern Storm Software, Pty Ltd.
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

#ifndef	_JIT_WALK_H
#define	_JIT_WALK_H

#include <jit/jit-arch.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Get the frame address for a frame which is "n" levels up the stack.
 * A level value of zero indicates the current frame.
 */
void *_jit_get_frame_address(void *start, unsigned int n);
#if defined(__GNUC__)
# define jit_get_frame_address(n)	\
	(_jit_get_frame_address(jit_get_current_frame(), (n)))
#else
# define jit_get_frame_address(n)	(_jit_get_frame_address(0, (n)))
#endif

/*
 * Get the frame address for the current frame.  May be more efficient
 * than using "jit_get_frame_address(0)".
 *
 * Note: some gcc vestions have broken __builtin_frame_address() so use
 * _JIT_ARCH_GET_CURRENT_FRAME() if available. 
 */
#if defined(__GNUC__)
# define JIT_FAST_GET_CURRENT_FRAME	1
# if defined(_JIT_ARCH_GET_CURRENT_FRAME)
#  define jit_get_current_frame()			\
	({						\
		void *address;				\
		_JIT_ARCH_GET_CURRENT_FRAME(address);	\
		address;				\
	})
# else
#  define jit_get_current_frame()	(__builtin_frame_address(0))
# endif
#else
# define JIT_FAST_GET_CURRENT_FRAME	0
# define jit_get_current_frame()	(jit_get_frame_address(0))
#endif

/*
 * Get the next frame up the stack from a specified frame.
 * Returns NULL if it isn't possible to retrieve the next frame.
 */
void *_jit_get_next_frame_address(void *frame);
#if defined(__GNUC__) && defined(_JIT_ARCH_GET_NEXT_FRAME)
# define jit_get_next_frame_address(frame)			\
	({							\
		void *address;					\
		_JIT_ARCH_GET_NEXT_FRAME(address, (frame));	\
		address;					\
	})
#else
# define jit_get_next_frame_address(frame)	\
	(_jit_get_next_frame_address(frame))
#endif

/*
 * Get the return address for a specific frame.
 */
void *_jit_get_return_address(void *frame, void *frame0, void *return0);
#if defined(__GNUC__)
# if defined(_JIT_ARCH_GET_RETURN_ADDRESS)
#  define jit_get_return_address(frame)				\
	({							\
		void *address;					\
		_JIT_ARCH_GET_RETURN_ADDRESS(address, (frame));	\
		address;					\
	})
# else
#  define jit_get_return_address(frame)			\
	(_jit_get_return_address			\
		((frame),				\
		 __builtin_frame_address(0),		\
		 __builtin_return_address(0)))
# endif
#else
# define jit_get_return_address(frame)	\
	(_jit_get_return_address((frame), 0, 0))
#endif

/*
 * Get the return address for the current frame.  May be more efficient
 * than using "jit_get_return_address(0)".
 */
#if defined(__GNUC__)
# if defined(_JIT_ARCH_GET_CURRENT_RETURN)
#  define jit_get_current_return()			\
	({						\
		void *address;				\
		_JIT_ARCH_GET_CURRENT_RETURN(address);	\
		address;				\
	})
# else
#  define jit_get_current_return()	(__builtin_return_address(0))
# endif
#else
# define jit_get_current_return()	\
	(jit_get_return_address(jit_get_current_frame()))
#endif

/*
 * Declare a stack crawl mark variable.  The address of this variable
 * can be passed to "jit_frame_contains_crawl_mark" to determine
 * if a frame contains the mark.
 */
typedef struct { void * volatile mark; } jit_crawl_mark_t;
#define	jit_declare_crawl_mark(name)	jit_crawl_mark_t name = {0}

/*
 * Determine if the stack frame just above "frame" contains a
 * particular crawl mark.
 */
int jit_frame_contains_crawl_mark(void *frame, jit_crawl_mark_t *mark);

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_WALK_H */
