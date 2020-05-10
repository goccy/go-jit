/*
 * jit-signal.c - Internal management routines to use Operating System
 *		  signals for libjit exceptions handling.
 *
 * Copyright (C) 2006  Southern Storm Software, Pty Ltd.
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

#ifdef JIT_USE_SIGNALS

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Use SIGSEGV for builtin libjit exception.
 */
static void sigsegv_handler(int signum, siginfo_t *info, void *uap)
{
	jit_exception_builtin(JIT_RESULT_NULL_REFERENCE);
}

/*
 * Use SIGFPE for builtin libjit exception.
 */
static void sigfpe_handler(int signum, siginfo_t *info, void *uap)
{
	switch(info->si_code)
	{
	case FPE_INTDIV:
		jit_exception_builtin(JIT_RESULT_DIVISION_BY_ZERO);
		break;
	case FPE_INTOVF:
		jit_exception_builtin(JIT_RESULT_OVERFLOW);
		break;
	case FPE_FLTDIV:
		jit_exception_builtin(JIT_RESULT_DIVISION_BY_ZERO);
		break;
	case FPE_FLTOVF:
		jit_exception_builtin(JIT_RESULT_OVERFLOW);
		break;
	case FPE_FLTUND:
		jit_exception_builtin(JIT_RESULT_ARITHMETIC);
		break;
	case FPE_FLTSUB:
		jit_exception_builtin(JIT_RESULT_ARITHMETIC);
		break;
	default:
		jit_exception_builtin(JIT_RESULT_ARITHMETIC);
		break;
	}
}

/*
 * Initialize signal handlers.
 */
void _jit_signal_init(void)
{
	struct sigaction sa_fpe, sa_segv;

	sa_fpe.sa_sigaction = sigfpe_handler;
	sigemptyset(&sa_fpe.sa_mask);
	sa_fpe.sa_flags = SA_SIGINFO;
	if (sigaction(SIGFPE, &sa_fpe, 0)) {
		perror("Sigaction SIGFPE");
		exit(1);
	}

	sa_segv.sa_sigaction = sigsegv_handler;
	sigemptyset(&sa_segv.sa_mask);
	sa_segv.sa_flags = SA_SIGINFO;
	if (sigaction(SIGSEGV, &sa_segv, 0)) {
		perror("Sigaction SIGSEGV");
		exit(1);
	}
}

#endif /* JIT_USE_SIGNALS */
