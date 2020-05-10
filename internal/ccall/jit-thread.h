/*
 * jit-thread.h - Internal thread management routines for libjit.
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

#ifndef	_JIT_THREAD_H
#define	_JIT_THREAD_H

#include <jit/jit-defs.h>
#include "jit-config.h"

#if defined(JIT_THREADS_PTHREAD)
# include <pthread.h>
#elif defined(JIT_THREADS_WIN32)
# include <windows.h>
#endif

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Type that describes a thread's identifier, and the id comparison function.
 */
#if defined(JIT_THREADS_PTHREAD)
typedef pthread_t jit_thread_id_t;
#define	jit_thread_id_equal(x,y)	(pthread_equal((x), (y)))
#define	jit_thread_self()			(pthread_self())
#define	jit_thread_release_self(t)	do { ; } while (0)
#elif defined(JIT_THREADS_WIN32)
typedef HANDLE jit_thread_id_t;
#define	jit_thread_id_equal(x,y)	((x) == (y))
jit_thread_id_t _jit_thread_self(void);
#define	jit_thread_self()			_jit_thread_self()
#define	jit_thread_release_self(t)	CloseHandle((t))
#else
typedef int jit_thread_id_t;
#define	jit_thread_id_equal(x,y)	((x) == (y))
#define	jit_thread_self()			1
#define	jit_thread_release_self(t)	do { ; } while (0)
#endif

/*
 * Control information that is associated with a thread.
 */
typedef struct jit_thread_control *jit_thread_control_t;

/*
 * Initialize the thread routines.  Ignored if called multiple times.
 */
void _jit_thread_init(void);

/*
 * Get the JIT control object for the current thread.
 */
jit_thread_control_t _jit_thread_get_control(void);

/*
 * Get the identifier for the current thread.
 */
jit_thread_id_t _jit_thread_current_id(void);

/*
 * Define the primitive mutex operations.
 */
#if defined(JIT_THREADS_PTHREAD)

typedef pthread_mutex_t jit_mutex_t;
#define	jit_mutex_create(mutex)		(pthread_mutex_init((mutex), 0))
#define	jit_mutex_destroy(mutex)	(pthread_mutex_destroy((mutex)))
#define	jit_mutex_lock(mutex)		(pthread_mutex_lock((mutex)))
#define	jit_mutex_unlock(mutex)		(pthread_mutex_unlock((mutex)))

#elif defined(JIT_THREADS_WIN32)

typedef CRITICAL_SECTION jit_mutex_t;
#define	jit_mutex_create(mutex)		(InitializeCriticalSection((mutex)))
#define	jit_mutex_destroy(mutex)	(DeleteCriticalSection((mutex)))
#define	jit_mutex_lock(mutex)		(EnterCriticalSection((mutex)))
#define	jit_mutex_unlock(mutex)		(LeaveCriticalSection((mutex)))

#else

typedef int jit_mutex_t;
#define	jit_mutex_create(mutex)		do { ; } while (0)
#define	jit_mutex_destroy(mutex)	do { ; } while (0)
#define	jit_mutex_lock(mutex)		do { ; } while (0)
#define	jit_mutex_unlock(mutex)		do { ; } while (0)

#endif

/*
 * Mutex that synchronizes global data initialization.
 */
extern jit_mutex_t _jit_global_lock;

/*
 * Define the primitive monitor operations.
 */

#if defined(JIT_THREADS_PTHREAD)

typedef struct
{
	pthread_mutex_t	_mutex;
	pthread_cond_t	_cond;

} jit_monitor_t;
#define	jit_monitor_create(mon)	\
		do { \
			pthread_mutex_init(&((mon)->_mutex), 0); \
			pthread_cond_init(&((mon)->_cond), 0); \
		} while (0)
#define	jit_monitor_destroy(mon)	\
		do { \
			pthread_cond_destroy(&((mon)->_cond)); \
			pthread_mutex_destroy(&((mon)->_mutex)); \
		} while (0)
#define	jit_monitor_lock(mon)			\
		do { \
			pthread_mutex_lock(&((mon)->_mutex)); \
		} while (0)
#define	jit_monitor_unlock(mon)			\
		do { \
			pthread_mutex_unlock(&((mon)->_mutex)); \
		} while (0)
#define	jit_monitor_signal(mon)			\
		do { \
			pthread_cond_signal(&((mon)->_cond)); \
		} while (0)
#define	jit_monitor_signal_all(mon)		\
		do { \
			pthread_cond_broadcast(&((mon)->_cond)); \
		} while (0)

#elif defined(JIT_THREADS_WIN32)

typedef struct
{
	HANDLE	_mutex;
	HANDLE	_cond;
	LONG volatile _waiting;
} jit_monitor_t;
#define	jit_monitor_create(mon)			\
		do { \
			(mon)->_mutex = CreateMutex(NULL, FALSE, NULL); \
			(mon)->_cond = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL); \
			(mon)->_waiting = 0; \
		} while (0)
#define	jit_monitor_destroy(mon)		\
		do { \
			CloseHandle((mon)->_cond); \
			CloseHandle((mon)->_mutex); \
		} while (0)
#define	jit_monitor_lock(mon)			\
		do { \
			WaitForSingleObject((mon)->_mutex, INFINITE); \
		} while (0)
#define	jit_monitor_unlock(mon)			\
		do { \
			ReleaseMutex((mon)->_mutex); \
		} while (0)
#define	jit_monitor_signal(mon)			\
		do { \
			if((mon)->_waiting > 0) \
			{ \
				--((mon)->_waiting); \
				ReleaseSemaphore((mon)->_cond, 1, NULL); \
			} \
		} while (0)
#define	jit_monitor_signal_all(mon)		\
		do { \
			LONG _count = (mon)->_waiting; \
			if(_count > 0) \
			{ \
				(mon)->_waiting = 0; \
				ReleaseSemaphore((mon)->_cond, _count, NULL); \
			} \
		} while (0)

#else

typedef int jit_monitor_t;
#define	jit_monitor_create(mon)			do { ; } while (0)
#define	jit_monitor_destroy(mon)		do { ; } while (0)
#define	jit_monitor_lock(mon)			do { ; } while (0)
#define	jit_monitor_unlock(mon)			do { ; } while (0)
#define	jit_monitor_signal(mon)			do { ; } while (0)
#define	jit_monitor_signal_all(mon)		do { ; } while (0)

#endif
int _jit_monitor_wait(jit_monitor_t *mon, jit_int timeout);
#define	jit_monitor_wait(mon,timeout)	_jit_monitor_wait((mon), (timeout))

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_THREAD_H */
