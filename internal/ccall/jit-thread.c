/*
 * jit-thread.c - Internal thread management routines for libjit.
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

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# elif !defined(__palmos__)
#  include <time.h>
# endif
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <errno.h>

/*
 * Mutex that synchronizes global data initialization.
 */
jit_mutex_t _jit_global_lock;

#if defined(JIT_THREADS_PTHREAD)

/*
 * The thread-specific key to use to fetch the control object.
 */
static pthread_key_t control_key;

/*
 * Initialize the pthread support routines.  Only called once.
 */
static void init_pthread(void)
{
	jit_mutex_create(&_jit_global_lock);

	/* Allocate a thread-specific variable for the JIT's thread
	   control object, and arrange for it to be freed when the
	   thread exits or is otherwise terminated */
	pthread_key_create(&control_key, jit_free);
}

#elif defined(JIT_THREADS_WIN32)

/*
 * The thread-specific key to use to fetch the control object.
 */
static DWORD control_key;

/*
 * Initialize the Win32 thread support routines.  Only called once.
 */
static void init_win32_thread(void)
{
	jit_mutex_create(&_jit_global_lock);

	control_key = TlsAlloc();
}

jit_thread_id_t _jit_thread_self(void)
{
	HANDLE new_handle;
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
					GetCurrentProcess(), &new_handle,
					0, 0, DUPLICATE_SAME_ACCESS);
	return new_handle;
}

#else /* No thread package */

/*
 * The control object for the only thread in the system.
 */
static void *control_object = 0;

#endif /* No thread package */

void _jit_thread_init(void)
{
#if defined(JIT_THREADS_PTHREAD)
	static pthread_once_t once_control = PTHREAD_ONCE_INIT;
	pthread_once(&once_control, init_pthread);
#elif defined(JIT_THREADS_WIN32)
	static LONG volatile once_control = 0;
	switch(InterlockedCompareExchange(&once_control, 1, 0))
	{
	case 0:
		init_win32_thread();
		InterlockedExchange(&once_control, 2);
		break;
	case 1:
		while(InterlockedCompareExchange(&once_control, 2, 2) != 2)
		{
		}
		break;
	}
#endif
}

static void *get_raw_control(void)
{
	_jit_thread_init();
#if defined(JIT_THREADS_PTHREAD)
	return pthread_getspecific(control_key);
#elif defined(JIT_THREADS_WIN32)
	return (void *)(TlsGetValue(control_key));
#else
	return control_object;
#endif
}

static void set_raw_control(void *obj)
{
	_jit_thread_init();
#if defined(JIT_THREADS_PTHREAD)
	pthread_setspecific(control_key, obj);
#elif defined(JIT_THREADS_WIN32)
	TlsSetValue(control_key, obj);
#else
	control_object = obj;
#endif
}

jit_thread_control_t _jit_thread_get_control(void)
{
	jit_thread_control_t control;
	control = (jit_thread_control_t)get_raw_control();
	if(!control)
	{
		control = jit_cnew(struct jit_thread_control);
		if(control)
		{
			set_raw_control(control);
		}
	}
	return control;
}

jit_thread_id_t _jit_thread_current_id(void)
{
#if defined(JIT_THREADS_PTHREAD)
	return pthread_self();
#elif defined(JIT_THREADS_WIN32)
	return GetCurrentThread();
#else
	/* There is only one thread, so lets give it an identifier of 1 */
	return 1;
#endif
}

int _jit_monitor_wait(jit_monitor_t *mon, jit_int timeout)
{
#if defined(JIT_THREADS_PTHREAD)
	if(timeout < 0)
	{
		pthread_cond_wait(&(mon->_cond), &(mon->_mutex));
		return 1;
	}
	else
	{
		struct timeval tv;
		struct timespec ts;
		int result;

		gettimeofday(&tv, 0);
		ts.tv_sec = tv.tv_sec + (long)(timeout / 1000);
		ts.tv_nsec = (tv.tv_usec + (long)((timeout % 1000) * 1000)) * 1000L;
		if(ts.tv_nsec >= 1000000000L)
		{
			++(ts.tv_sec);
			ts.tv_nsec -= 1000000000L;
		}

		/* Wait until we are signalled or the timeout expires */
		do
		{
			result = pthread_cond_timedwait(&(mon->_cond), &(mon->_mutex), &ts);
		}
		while(result == EINTR);
		return ((result == 0) ? 1 : 0);
	}
#elif defined(JIT_THREADS_WIN32)
	DWORD result;
	++(mon->_waiting);
	if(timeout >= 0)
	{
		result = SignalObjectAndWait(mon->_mutex, mon->_cond, (DWORD)timeout, FALSE);
	}
	else
	{
		result = SignalObjectAndWait(mon->_mutex, mon->_cond, INFINITE, FALSE);
	}
	WaitForSingleObject(mon->_mutex, INFINITE);
	return (result == WAIT_OBJECT_0);
#else
	return 0;
#endif
}
