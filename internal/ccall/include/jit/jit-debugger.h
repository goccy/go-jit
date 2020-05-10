/*
 * jit-debugger.h - Helper routines for single-step debugging of programs.
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

#ifndef	_JIT_DEBUGGER_H
#define	_JIT_DEBUGGER_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

typedef struct jit_debugger *jit_debugger_t;
typedef jit_nint jit_debugger_thread_id_t;
typedef jit_nint jit_debugger_breakpoint_id_t;

typedef struct jit_debugger_event
{
	int								type;
	jit_debugger_thread_id_t		thread;
	jit_function_t					function;
	jit_nint						data1;
	jit_nint						data2;
	jit_debugger_breakpoint_id_t	id;
	jit_stack_trace_t				trace;

} jit_debugger_event_t;

#define	JIT_DEBUGGER_TYPE_QUIT				0
#define	JIT_DEBUGGER_TYPE_HARD_BREAKPOINT	1
#define	JIT_DEBUGGER_TYPE_SOFT_BREAKPOINT	2
#define	JIT_DEBUGGER_TYPE_USER_BREAKPOINT	3
#define	JIT_DEBUGGER_TYPE_ATTACH_THREAD		4
#define	JIT_DEBUGGER_TYPE_DETACH_THREAD		5

typedef struct jit_debugger_breakpoint_info
{
	int								flags;
	jit_debugger_thread_id_t		thread;
	jit_function_t					function;
	jit_nint						data1;
	jit_nint						data2;

} *jit_debugger_breakpoint_info_t;

#define	JIT_DEBUGGER_FLAG_THREAD		(1 << 0)
#define	JIT_DEBUGGER_FLAG_FUNCTION		(1 << 1)
#define	JIT_DEBUGGER_FLAG_DATA1			(1 << 2)
#define	JIT_DEBUGGER_FLAG_DATA2			(1 << 3)

#define	JIT_DEBUGGER_DATA1_FIRST		10000
#define	JIT_DEBUGGER_DATA1_LINE			10000
#define	JIT_DEBUGGER_DATA1_ENTER		10001
#define	JIT_DEBUGGER_DATA1_LEAVE		10002
#define	JIT_DEBUGGER_DATA1_THROW		10003

typedef void (*jit_debugger_hook_func)
	(jit_function_t func, jit_nint data1, jit_nint data2);

int jit_debugging_possible(void) JIT_NOTHROW;

jit_debugger_t jit_debugger_create(jit_context_t context) JIT_NOTHROW;
void jit_debugger_destroy(jit_debugger_t dbg) JIT_NOTHROW;

jit_context_t jit_debugger_get_context(jit_debugger_t dbg) JIT_NOTHROW;
jit_debugger_t jit_debugger_from_context(jit_context_t context) JIT_NOTHROW;

jit_debugger_thread_id_t jit_debugger_get_self(jit_debugger_t dbg) JIT_NOTHROW;
jit_debugger_thread_id_t jit_debugger_get_thread
		(jit_debugger_t dbg, const void *native_thread) JIT_NOTHROW;
int jit_debugger_get_native_thread
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread,
		 void *native_thread) JIT_NOTHROW;
void jit_debugger_set_breakable
		(jit_debugger_t dbg, const void *native_thread, int flag) JIT_NOTHROW;

void jit_debugger_attach_self
		(jit_debugger_t dbg, int stop_immediately) JIT_NOTHROW;
void jit_debugger_detach_self(jit_debugger_t dbg) JIT_NOTHROW;

int jit_debugger_wait_event
		(jit_debugger_t dbg, jit_debugger_event_t *event,
		 jit_int timeout) JIT_NOTHROW;

jit_debugger_breakpoint_id_t jit_debugger_add_breakpoint
		(jit_debugger_t dbg, jit_debugger_breakpoint_info_t info) JIT_NOTHROW;
void jit_debugger_remove_breakpoint
		(jit_debugger_t dbg, jit_debugger_breakpoint_id_t id) JIT_NOTHROW;
void jit_debugger_remove_all_breakpoints(jit_debugger_t dbg) JIT_NOTHROW;

int jit_debugger_is_alive
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread) JIT_NOTHROW;
int jit_debugger_is_running
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread) JIT_NOTHROW;
void jit_debugger_run
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread) JIT_NOTHROW;
void jit_debugger_step
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread) JIT_NOTHROW;
void jit_debugger_next
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread) JIT_NOTHROW;
void jit_debugger_finish
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread) JIT_NOTHROW;

void jit_debugger_break(jit_debugger_t dbg) JIT_NOTHROW;

void jit_debugger_quit(jit_debugger_t dbg) JIT_NOTHROW;

jit_debugger_hook_func jit_debugger_set_hook
		(jit_context_t context, jit_debugger_hook_func hook);

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_DEBUGGER_H */
