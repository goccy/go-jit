/*
 * jit-walk.c - Routines for performing native stack walking.
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

#include "jit-internal.h"
#include "jit-apply-rules.h"

/*
 * The routines here are system specific to a large extent,
 * but we can avoid a lot of the nastiness using gcc builtins.
 * It is highly recommended that you use gcc to build libjit.
 *
 * The following macros may need to be tweaked on some platforms.
 */

/*
 * Some platforms store the return address in an altered form
 * (e.g. an offset rather than a pointer).  We use this macro to
 * fix such address values.
 */
#if defined(__GNUC__)
#define	jit_fix_return_address(x)	(__builtin_extract_return_addr((x)))
#else
#define	jit_fix_return_address(x)	(x)
#endif

#if JIT_APPLY_BROKEN_FRAME_BUILTINS == 0

/*
 * Extract the next frame pointer in the chain.
 */
#define	jit_next_frame_pointer(x)	\
	(*((void **)(((unsigned char *)(x)) + JIT_APPLY_PARENT_FRAME_OFFSET)))

/*
 * Extract the return address from a particular frame.
 */
#define	jit_extract_return_address(x)	\
	(*((void **)(((unsigned char *)(x)) + JIT_APPLY_RETURN_ADDRESS_OFFSET)))

#else	/* JIT_APPLY_BROKEN_FRAME_BUILTINS */

/*
 * Extract the next frame pointer in the chain.
 */
#define	jit_next_frame_pointer(x)		0

/*
 * Extract the return address from a particular frame.
 */
#define	jit_extract_return_address(x)	0

#endif	/* JIT_APPLY_BROKEN_FRAME_BUILTINS */

/*
 * Fetch the starting frame address if the caller did not supply it
 * (probably because the caller wasn't compiled with gcc).  The address
 * that we want is actually one frame out from where we are at the moment.
 *
 * Note: some gcc vestions have broken __builtin_frame_address() so use
 * _JIT_ARCH_GET_CURRENT_FRAME() if available. 
 */
#if defined(__GNUC__)
#if defined(_JIT_ARCH_GET_CURRENT_FRAME)
#define	jit_get_starting_frame()	\
	do { \
		_JIT_ARCH_GET_CURRENT_FRAME(start); \
		if(start) \
		{ \
			start = jit_next_frame_pointer(start); \
		} \
	} while (0)
#else
#define	jit_get_starting_frame()	\
	do { \
		start = __builtin_frame_address(0); \
		if(start) \
		{ \
			start = jit_next_frame_pointer(start); \
		} \
	} while (0)
#endif
#elif defined(_MSC_VER) && defined(_M_IX86)
#define	jit_get_starting_frame()	\
	__asm \
	{ \
		__asm mov eax, [ebp] \
		__asm mov dword ptr start, eax \
	}
#else
#define	jit_get_starting_frame()	do { ; } while (0)
#endif

/*@

@section Stack walking
@cindex Stack walking
@cindex jit-walk.h

The functions in @code{<jit/jit-walk.h>} allow the caller to walk
up the native execution stack, inspecting frames and return addresses.

@*/

/*@
 * @deftypefun {void *} jit_get_frame_address (unsigned int @var{n})
 * Get the frame address for the call frame @var{n} levels up
 * the stack.  Setting @var{n} to zero will retrieve the frame
 * address for the current function.  Returns NULL if it isn't
 * possible to retrieve the address of the specified frame.
 * @end deftypefun
 *
 * @deftypefun {void *} jit_get_current_frame (void)
 * Get the frame address for the current function.  This may be more
 * efficient on some platforms than using @code{jit_get_frame_address(0)}.
 * Returns NULL if it isn't possible to retrieve the address of
 * the current frame.
 * @end deftypefun
@*/
void *_jit_get_frame_address(void *start, unsigned int n)
{
	/* Fetch the starting frame address if the caller did not supply it */
	if(!start)
	{
		jit_get_starting_frame();
	}

	/* Scan up the stack until we find the frame we want */
	while(start != 0 && n > 0)
	{
		start = jit_next_frame_pointer(start);
		--n;
	}
	return start;
}

/*@
 * @deftypefun {void *} jit_get_next_frame_address (void *@var{frame})
 * Get the address of the next frame up the stack from @var{frame}.
 * Returns NULL if it isn't possible to retrieve the address of
 * the next frame up the stack.
 * @end deftypefun
@*/
void *_jit_get_next_frame_address(void *frame)
{
	if(frame)
	{
		return jit_next_frame_pointer(frame);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun {void *} jit_get_return_address (void *@var{frame})
 * Get the return address from a specified frame.  The address
 * represents the place where execution returns to when the
 * specified frame exits.  Returns NULL if it isn't possible
 * to retrieve the return address of the specified frame.
 * @end deftypefun
 *
 * @deftypefun {void *} jit_get_current_return (void)
 * Get the return address for the current function.  This may be more
 * efficient on some platforms than using @code{jit_get_return_address(0)}.
 * Returns NULL if it isn't possible to retrieve the return address of
 * the current frame.
 * @end deftypefun
@*/
void *_jit_get_return_address(void *frame, void *frame0, void *return0)
{
	/* If the caller was compiled with gcc, it may have already figured
	   out the return address for us using builtin gcc facilities */
	if(frame && frame == frame0)
	{
		return jit_fix_return_address(return0);
	}
	else if(frame)
	{
		return jit_fix_return_address(jit_extract_return_address(frame));
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_frame_contains_crawl_mark (void *@var{frame}, jit_crawl_mark_t *@var{mark})
 * Determine if the stack frame that resides just above @var{frame}
 * contains a local variable whose address is @var{mark}.  The @var{mark}
 * parameter should be the address of a local variable that is declared with
 * @code{jit_declare_crawl_mark(@var{name})}.
 *
 * Crawl marks are used internally by libjit to determine where control
 * passes between JIT'ed and ordinary code during an exception throw.
 * They can also be used to mark frames that have special security
 * conditions associated with them.
 * @end deftypefun
@*/
int jit_frame_contains_crawl_mark(void *frame, jit_crawl_mark_t *mark)
{
	void *markptr = (void *)mark;
	void *next;
	if(!frame)
	{
		/* We don't have a frame to check against */
		return 0;
	}
	next = jit_next_frame_pointer(frame);
	if(!next)
	{
		/* We are at the top of the stack crawl */
		return 0;
	}
	if(frame <= next)
	{
		/* The stack grows downwards in memory */
		return (markptr >= frame && markptr < next);
	}
	else
	{
		/* The stack grows upwards in memory */
		return (markptr >= next && markptr < frame);
	}
}
