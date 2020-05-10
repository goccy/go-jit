/*
 * jit-config.h - Configuration macros for the JIT.
 *
 * Copyright (C) 2011  Southern Storm Software, Pty Ltd.
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

#ifndef _JIT_CONFIG_H
#define	_JIT_CONFIG_H

#include <config.h>

/*
 * Determine what kind of system we are running on.
 */
#if defined(_WIN32) || defined(WIN32)
# define JIT_WIN32_PLATFORM	1
#elif defined(__APPLE__) && defined(__MACH__)
# define JIT_DARWIN_PLATFORM	1
#elif defined(__linux__)
# define JIT_LINUX_PLATFORM	1
#endif

/*
 * Determine the type of threading library that we are using.
 */
#if defined(HAVE_PTHREAD_H) && defined(HAVE_LIBPTHREAD)
# define JIT_THREADS_SUPPORTED	1
# define JIT_THREADS_PTHREAD	1
#elif defined(JIT_WIN32_PLATFORM)
# define JIT_THREADS_SUPPORTED	1
# define JIT_THREADS_WIN32	1
#else
# define JIT_THREADS_SUPPORTED	0
#endif

/*
 * Determine the type of virtual memory API that we are using.
 */
#if defined(JIT_WIN32_PLATFORM)
# define JIT_VMEM_SUPPORTED	1
# define JIT_VMEM_WIN32		1
#elif defined(HAVE_SYS_MMAN_H)
# define JIT_VMEM_SUPPORTED	1
# define JIT_VMEM_MMAP		1
#else
# define JIT_VMEM_SUPPORTED	0
#endif

/*
 * Determine which backend to use.
 */
#if defined(USE_LIBJIT_INTERPRETER)
# define JIT_BACKEND_INTERP	1
# define JIT_HAVE_BACKEND	1
#elif defined(__alpha) || defined(__alpha__)
# define JIT_BACKEND_ALPHA	1
# define JIT_HAVE_BACKEND	1
#elif defined(__arm) || defined(__arm__)
# define JIT_BACKEND_ARM	1
# define JIT_HAVE_BACKEND	1
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
# define JIT_BACKEND_X86	1
# define JIT_HAVE_BACKEND	1
#elif defined(__amd64) || defined(__amd64__) || defined(_x86_64) || defined(_x86_64__)
# define JIT_BACKEND_X86_64	1
# define JIT_HAVE_BACKEND	1
#endif

/*
 * Fallback  to interpreter if there is no appropriate native backend.
 */
#if !defined(JIT_HAVE_BACKEND)
# define JIT_BACKEND_INTERP	1
#endif

/*
#define _JIT_COMPILE_DEBUG	1
#define _JIT_BLOCK_DEBUG	1
 */

#endif	/* _JIT_CONFIG_H */

