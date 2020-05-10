/*
 * jit-init.c - Initialization routines for the JIT.
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
#include "jit-rules.h"

/*@
 * @deftypefun void jit_init (void)
 * This is normally the first function that you call when using
 * @code{libjit}.  It initializes the library and prepares for
 * JIT operations.
 *
 * The @code{jit_context_create} function also calls this, so you can
 * avoid using @code{jit_init} if @code{jit_context_create} is the first
 * JIT function that you use.
 *
 * It is safe to initialize the JIT multiple times.  Subsequent
 * initializations are quietly ignored.
 * @end deftypefun
@*/
void
jit_init(void)
{
	static int init_done = 0;

	/* Make sure that the thread subsystem is initialized */
	_jit_thread_init();

	/* Make sure that the initialization is done only once
	   (requires the global lock initialized above) */
	jit_mutex_lock(&_jit_global_lock);
	if(init_done)
	{
		goto done;
	}
	init_done = 1;

#ifdef JIT_USE_SIGNALS
	/* Initialize the signal handlers */
	_jit_signal_init();
#endif

	/* Initialize the virtual memory system */
	jit_vmem_init();

	/* Initialize the backend */
	_jit_init_backend();

done:
	jit_mutex_unlock(&_jit_global_lock);
}

/*@
 * @deftypefun int jit_uses_interpreter (void)
 * Determine if the JIT uses a fall-back interpreter to execute code
 * rather than generating and executing native code.  This can be
 * called prior to @code{jit_init}.
 * @end deftypefun
@*/
int
jit_uses_interpreter(void)
{
#if defined(JIT_BACKEND_INTERP)
	return 1;
#else
	return 0;
#endif
}

/*@
 * @deftypefun int jit_supports_threads (void)
 * Determine if the JIT supports threads.
 * @end deftypefun
@*/
int
jit_supports_threads(void)
{
	return JIT_THREADS_SUPPORTED;
}

/*@
 * @deftypefun int jit_supports_virtual_memory (void)
 * Determine if the JIT supports virtual memory.
 * @end deftypefun
@*/
int
jit_supports_virtual_memory(void)
{
	return JIT_VMEM_SUPPORTED;
}
