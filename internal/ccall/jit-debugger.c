/*
 * jit-debugger.c - Helper routines for single-step debugging of programs.
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

/*@

@cindex jit-debugger.h

The @code{libjit} library provides support routines for breakpoint-based
single-step debugging.  It isn't a full debugger, but provides the
infrastructure necessary to support one.

The front end virtual machine is responsible for inserting "potential
breakpoints" into the code when functions are built and compiled.  This
is performed using @code{jit_insn_mark_breakpoint}:

@deftypefun int jit_insn_mark_breakpoint (jit_function_t @var{func}, jit_nint @var{data1}, jit_nint @var{data2})
Mark the current position in @var{func} as corresponding to a breakpoint
location.  When a break occurs, the debugging routines are passed
@var{func}, @var{data1}, and @var{data2} as arguments.  By convention,
@var{data1} is the type of breakpoint (source line, function entry,
function exit, etc).
@end deftypefun

There are two ways for a front end to receive notification about breakpoints.
The bulk of this chapter describes the @code{jit_debugger_t} interface,
which handles most of the ugly details.  In addition, a low-level "debug hook
mechanism" is provided for front ends that wish more control over the
process.  The debug hook mechanism is described below, under the
@code{jit_debugger_set_hook} function.

This debugger implementation requires a threading system to work
successfully.  At least two threads are required, in addition to those of
the program being debugged:

@enumerate
@item
Event thread which calls @code{jit_debugger_wait_event} to receive
notifications of breakpoints and other interesting events.

@item
User interface thread which calls functions like @code{jit_debugger_run},
@code{jit_debugger_step}, etc, to control the debug process.
@end enumerate

These two threads should be set to "unbreakable" with a call to
@code{jit_debugger_set_breakable}.  This prevents them from accidentally
stopping at a breakpoint, which would cause a system deadlock.
Other housekeeping threads, such as a finalization thread, should
also be set to "unbreakable" for the same reason.

@noindent
Events have the following members:

@table @code
@item type
The type of event (see the next table for details).

@item thread
The thread that the event occurred on.

@item function
The function that the breakpoint occurred within.

@item data1
@itemx data2
The data values at the breakpoint.  These values are inserted into
the function's code with @code{jit_insn_mark_breakpoint}.

@item id
The identifier for the breakpoint.

@item trace
The stack trace corresponding to the location where the breakpoint
occurred.  This value is automatically freed upon the next call
to @code{jit_debugger_wait_event}.  If you wish to preserve the
value, then you must call @code{jit_stack_trace_copy}.
@end table

@noindent
The following event types are currently supported:

@table @code
@item JIT_DEBUGGER_TYPE_QUIT
A thread called @code{jit_debugger_quit}, indicating that it wanted the
event thread to terminate.

@item JIT_DEBUGGER_TYPE_HARD_BREAKPOINT
A thread stopped at a hard breakpoint.  That is, a breakpoint defined
by a call to @code{jit_debugger_add_breakpoint}.

@item JIT_DEBUGGER_TYPE_SOFT_BREAKPOINT
A thread stopped at a breakpoint that wasn't explicitly defined by
a call to @code{jit_debugger_add_breakpoint}.  This typicaly results
from a call to a "step" function like @code{jit_debugger_step}, where
execution stopped at the next line but there isn't an explicit breakpoint
on that line.

@item JIT_DEBUGGER_TYPE_USER_BREAKPOINT
A thread stopped because of a call to @code{jit_debugger_break}.

@item JIT_DEBUGGER_TYPE_ATTACH_THREAD
A thread called @code{jit_debugger_attach_self}.  The @code{data1} field
of the event is set to the value of @code{stop_immediately} for the call.

@item JIT_DEBUGGER_TYPE_DETACH_THREAD
A thread called @code{jit_debugger_detach_self}.
@end table

@deftypefun int jit_insn_mark_breakpoint_variable (jit_function_t @var{func}, jit_value_t @var{data1}, jit_value_t @var{data2})
This function is similar to @code{jit_insn_mark_breakpoint} except that values
in @var{data1} and @var{data2} can be computed at runtime. You can use this
function for example to get address of local variable.
@end deftypefun

@*/

/*
 * Linked event, for the debugger event queue.
 */
typedef struct jit_debugger_linked_event
{
	jit_debugger_event_t				event;
	struct jit_debugger_linked_event   *next;

} jit_debugger_linked_event_t;

/*
 * Run types.
 */
#define	JIT_RUN_TYPE_STOPPED		0
#define	JIT_RUN_TYPE_CONTINUE		1
#define	JIT_RUN_TYPE_STEP			2
#define	JIT_RUN_TYPE_NEXT			3
#define	JIT_RUN_TYPE_FINISH			4
#define	JIT_RUN_TYPE_DETACHED		5

/*
 * Information about a thread that is under the control of the debugger.
 */
typedef struct jit_debugger_thread
{
	struct jit_debugger_thread *next;
	jit_debugger_thread_id_t	id;
	jit_thread_id_t				native_id;
	int				   volatile	run_type;
	jit_function_t				find_func;
	jit_nint					last_data1;
	jit_nint					last_func_data1;
	int							breakable;

} *jit_debugger_thread_t;

/*
 * Structure of a debugger instance.
 */
struct jit_debugger
{
	jit_monitor_t				  queue_lock;
	jit_monitor_t				  run_lock;
	jit_context_t				  context;
	jit_debugger_linked_event_t * volatile events;
	jit_debugger_linked_event_t * volatile last_event;
};

/*
 * Lock the debugger object.
 */
#define	lock_debugger(dbg)		jit_monitor_lock(&((dbg)->run_lock))

/*
 * Unlock the debugger object.
 */
#define	unlock_debugger(dbg)	jit_monitor_unlock(&((dbg)->run_lock))

/*
 * Suspend the current thread until it is marked as running again.
 * It is assumed that the debugger's monitor lock has been acquired.
 */
static void suspend_thread(jit_debugger_t dbg, jit_debugger_thread_t thread)
{
	while(thread->run_type == JIT_RUN_TYPE_STOPPED)
	{
		jit_monitor_wait(&(dbg->run_lock), -1);
	}
}

/*
 * Wake all threads that are waiting on the debugger's monitor.
 */
#define	wakeup_all(dbg)		jit_monitor_signal_all(&((dbg)->run_lock))

/*
 * Get the information block for the current thread.
 */
static jit_debugger_thread_t get_current_thread(jit_debugger_t dbg)
{
	/* TODO */
	return 0;
}

/*
 * Get the information block for a specific thread.
 */
static jit_debugger_thread_t get_specific_thread
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	/* TODO */
	return 0;
}

/*
 * Allocate space for a new event on the event queue.
 */
#define	alloc_event()		\
		((jit_debugger_event_t *)jit_cnew(jit_debugger_linked_event_t))

/*
 * Add an event that was previously allocated with "alloc_event"
 * to a debugger's event queue.
 */
static void add_event(jit_debugger_t dbg, jit_debugger_event_t *_event)
{
	jit_debugger_linked_event_t *event = (jit_debugger_linked_event_t *)_event;
	event->next = 0;
	jit_monitor_lock(&(dbg->queue_lock));
	if(dbg->last_event)
	{
		dbg->last_event->next = event;
	}
	else
	{
		dbg->events = event;
	}
	dbg->last_event = event;
	jit_monitor_signal(&(dbg->queue_lock));
	jit_monitor_unlock(&(dbg->queue_lock));
}

/*@
 * @deftypefun int jit_debugging_possible (void)
 * Determine if debugging is possible.  i.e. that threading is available
 * and compatible with the debugger's requirements.
 * @end deftypefun
@*/
int jit_debugging_possible(void)
{
	return JIT_THREADS_SUPPORTED;
}

/*@
 * @deftypefun jit_debugger_t jit_debugger_create (jit_context_t @var{context})
 * Create a new debugger instance and attach it to a JIT @var{context}.
 * If the context already has a debugger associated with it, then this
 * function will return the previous debugger.
 * @end deftypefun
@*/
jit_debugger_t jit_debugger_create(jit_context_t context)
{
	jit_debugger_t dbg;
	if(context)
	{
		if(context->debugger)
		{
			return context->debugger;
		}
		dbg = jit_cnew(struct jit_debugger);
		if(!dbg)
		{
			return 0;
		}
		dbg->context = context;
		context->debugger = dbg;
		jit_monitor_create(&(dbg->queue_lock));
		jit_monitor_create(&(dbg->run_lock));
		return dbg;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun void jit_debugger_destroy (jit_debugger_t @var{dbg})
 * Destroy a debugger instance.
 * @end deftypefun
@*/
void jit_debugger_destroy(jit_debugger_t dbg)
{
	/* TODO */
}

/*@
 * @deftypefun jit_context_t jit_debugger_get_context (jit_debugger_t @var{dbg})
 * Get the JIT context that is associated with a debugger instance.
 * @end deftypefun
@*/
jit_context_t jit_debugger_get_context(jit_debugger_t dbg)
{
	if(dbg)
	{
		return dbg->context;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_debugger_t jit_debugger_from_context (jit_context_t @var{context})
 * Get the debugger that is currently associated with a JIT @var{context},
 * or NULL if there is no debugger associated with the context.
 * @end deftypefun
@*/
jit_debugger_t jit_debugger_from_context(jit_context_t context)
{
	if(context)
	{
		return context->debugger;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_debugger_thread_id_t jit_debugger_get_self (jit_debugger_t @var{dbg})
 * Get the thread identifier associated with the current thread.
 * The return values are normally values like 1, 2, 3, etc, allowing
 * the user interface to report messages like "thread 3 has stopped
 * at a breakpoint".
 * @end deftypefun
@*/
jit_debugger_thread_id_t jit_debugger_get_self(jit_debugger_t dbg)
{
	jit_thread_id_t id = jit_thread_self();
	jit_debugger_thread_id_t thread;
	thread = jit_debugger_get_thread(dbg, &id);
	jit_thread_release_self(id);
	return thread;
}

/*@
 * @deftypefun jit_debugger_thread_id_t jit_debugger_get_thread (jit_debugger_t @var{dbg}, const void *@var{native_thread})
 * Get the thread identifier for a specific native thread.  The
 * @var{native_thread} pointer is assumed to point at a block
 * of memory containing a native thread handle.  This would be a
 * @code{pthread_t} on Pthreads platforms or a @code{HANDLE}
 * on Win32 platforms.  If the native thread has not been seen
 * previously, then a new thread identifier is allocated.
 * @end deftypefun
@*/
jit_debugger_thread_id_t jit_debugger_get_thread
		(jit_debugger_t dbg, const void *native_thread)
{
	/* TODO */
	return 0;
}

/*@
 * @deftypefun int jit_debugger_get_native_thread (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread}, void *@var{native_thread})
 * Get the native thread handle associated with a debugger thread identifier.
 * Returns non-zero if OK, or zero if the debugger thread identifier is not
 * yet associated with a native thread handle.
 * @end deftypefun
@*/
int jit_debugger_get_native_thread
		(jit_debugger_t dbg, jit_debugger_thread_id_t thread,
		 void *native_thread)
{
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_specific_thread(dbg, thread);
	if(th)
	{
		jit_memcpy(native_thread, &(th->native_id), sizeof(th->native_id));
		unlock_debugger(dbg);
		return 1;
	}
	else
	{
		unlock_debugger(dbg);
		return 0;
	}
}

/*@
 * @deftypefun void jit_debugger_set_breakable (jit_debugger_t @var{dbg}, const void *@var{native_thread}, int @var{flag})
 * Set a flag that indicates if a native thread can stop at breakpoints.
 * If set to 1 (the default), breakpoints will be active on the thread.
 * If set to 0, breakpoints will be ignored on the thread.  Typically
 * this is used to mark threads associated with the debugger's user
 * interface, or the virtual machine's finalization thread, so that they
 * aren't accidentally suspended by the debugger (which might cause a
 * deadlock).
 * @end deftypefun
@*/
void jit_debugger_set_breakable
		(jit_debugger_t dbg, const void *native_thread, int flag)
{
	jit_debugger_thread_t th;
	jit_debugger_thread_id_t id;
	id = jit_debugger_get_thread(dbg, native_thread);
	lock_debugger(dbg);
	th = get_specific_thread(dbg, id);
	if(th)
	{
		th->breakable = flag;
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_attach_self (jit_debugger_t @var{dbg}, int @var{stop_immediately})
 * Attach the current thread to a debugger.  If @var{stop_immediately}
 * is non-zero, then the current thread immediately suspends, waiting for
 * the user to start it with @code{jit_debugger_run}.  This function is
 * typically called in a thread's startup code just before any "real work"
 * is performed.
 * @end deftypefun
@*/
void jit_debugger_attach_self(jit_debugger_t dbg, int stop_immediately)
{
	jit_debugger_event_t *event;
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_current_thread(dbg);
	if(th)
	{
		event = alloc_event();
		if(event)
		{
			event->type = JIT_DEBUGGER_TYPE_ATTACH_THREAD;
			event->thread = th->id;
			event->data1 = (jit_nint)stop_immediately;
			add_event(dbg, event);
			th->find_func = 0;
			th->last_data1 = 0;
			th->last_func_data1 = 0;
			if(stop_immediately)
			{
				th->run_type = JIT_RUN_TYPE_STOPPED;
				suspend_thread(dbg, th);
			}
			else
			{
				th->run_type = JIT_RUN_TYPE_CONTINUE;
			}
		}
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_detach_self (jit_debugger_t @var{dbg})
 * Detach the current thread from the debugger.  This is typically
 * called just before the thread exits.
 * @end deftypefun
@*/
void jit_debugger_detach_self(jit_debugger_t dbg)
{
	jit_debugger_event_t *event;
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_current_thread(dbg);
	if(th)
	{
		event = alloc_event();
		if(event)
		{
			event->type = JIT_DEBUGGER_TYPE_DETACH_THREAD;
			event->thread = th->id;
			add_event(dbg, event);
			th->run_type = JIT_RUN_TYPE_DETACHED;
		}
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun int jit_debugger_wait_event (jit_debugger_t @var{dbg}, jit_debugger_event_t *@var{event}, jit_nint @var{timeout})
 * Wait for the next debugger event to arrive.  Debugger events typically
 * indicate breakpoints that have occurred.  The @var{timeout} is in
 * milliseconds, or -1 for an infinite timeout period.  Returns non-zero
 * if an event has arrived, or zero on timeout.
 * @end deftypefun
@*/
int jit_debugger_wait_event
		(jit_debugger_t dbg, jit_debugger_event_t *event, jit_int timeout)
{
	jit_debugger_linked_event_t *levent;
	jit_monitor_lock(&(dbg->queue_lock));
	if((levent = dbg->events) == 0)
	{
		if(!jit_monitor_wait(&(dbg->queue_lock), timeout))
		{
			jit_monitor_unlock(&(dbg->queue_lock));
			return 0;
		}
		levent = dbg->events;
	}
	*event = levent->event;
	dbg->events = levent->next;
	if(!(levent->next))
	{
		dbg->last_event = 0;
	}
	jit_free(levent);
	jit_monitor_unlock(&(dbg->queue_lock));
	return 1;
}

/*@
 * @deftypefun jit_debugger_breakpoint_id_t jit_debugger_add_breakpoint (jit_debugger_t @var{dbg}, jit_debugger_breakpoint_info_t @var{info})
 * Add a hard breakpoint to a debugger instance.  The @var{info} structure
 * defines the conditions under which the breakpoint should fire.
 * The fields of @var{info} are as follows:
 *
 * @table @code
 * @item flags
 * Flags that indicate which of the following fields should be matched.
 * If a flag is not present, then all possible values of the field will match.
 * Valid flags are @code{JIT_DEBUGGER_FLAG_THREAD},
 * @code{JIT_DEBUGGER_FLAG_FUNCTION}, @code{JIT_DEBUGGER_FLAG_DATA1},
 * and @code{JIT_DEBUGGER_FLAG_DATA2}.
 *
 * @item thread
 * The thread to match against, if @code{JIT_DEBUGGER_FLAG_THREAD} is set.
 *
 * @item function
 * The function to match against, if @code{JIT_DEBUGGER_FLAG_FUNCTION} is set.
 *
 * @item data1
 * The @code{data1} value to match against, if @code{JIT_DEBUGGER_FLAG_DATA1}
 * is set.
 *
 * @item data2
 * The @code{data2} value to match against, if @code{JIT_DEBUGGER_FLAG_DATA2}
 * is set.
 * @end table
 *
 * The following special values for @code{data1} are recommended for marking
 * breakpoint locations with @code{jit_insn_mark_breakpoint}:
 *
 * @table @code
 * @item JIT_DEBUGGER_DATA1_LINE
 * Breakpoint location that corresponds to a source line.  This is used
 * to determine where to continue to upon a "step".
 *
 * @item JIT_DEBUGGER_DATA1_ENTER
 * Breakpoint location that corresponds to the start of a function.
 *
 * @item JIT_DEBUGGER_DATA1_LEAVE
 * Breakpoint location that corresponds to the end of a function, just
 * prior to a @code{return} statement.  This is used to determine where
 * to continue to upon a "finish".
 *
 * @item JIT_DEBUGGER_DATA1_THROW
 * Breakpoint location that corresponds to an exception throw.
 * @end table
 * @end deftypefun
@*/
jit_debugger_breakpoint_id_t jit_debugger_add_breakpoint
		(jit_debugger_t dbg, jit_debugger_breakpoint_info_t info)
{
	/* TODO */
	return 0;
}

/*@
 * @deftypefun void jit_debugger_remove_breakpoint (jit_debugger_t @var{dbg}, jit_debugger_breakpoint_id_t @var{id})
 * Remove a previously defined breakpoint from a debugger instance.
 * @end deftypefun
@*/
void jit_debugger_remove_breakpoint
		(jit_debugger_t dbg, jit_debugger_breakpoint_id_t id)
{
	/* TODO */
}

/*@
 * @deftypefun void jit_debugger_remove_all_breakpoints (jit_debugger_t @var{dbg})
 * Remove all breakpoints from a debugger instance.
 * @end deftypefun
@*/
void jit_debugger_remove_all_breakpoints(jit_debugger_t dbg)
{
	/* TODO */
}

/*@
 * @deftypefun int jit_debugger_is_alive (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread})
 * Determine if a particular thread is still alive.
 * @end deftypefun
@*/
int jit_debugger_is_alive(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	/* TODO */
	return 1;
}

/*@
 * @deftypefun int jit_debugger_is_running (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread})
 * Determine if a particular thread is currently running (non-zero) or
 * stopped (zero).
 * @end deftypefun
@*/
int jit_debugger_is_running(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	jit_debugger_thread_t th;
	int flag = 0;
	lock_debugger(dbg);
	th = get_specific_thread(dbg, thread);
	if(th)
	{
		flag = (th->run_type != JIT_RUN_TYPE_STOPPED);
	}
	unlock_debugger(dbg);
	return flag;
}

/*@
 * @deftypefun void jit_debugger_run (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread})
 * Start the specified thread running, or continue from the last breakpoint.
 *
 * This function, and the others that follow, sends a request to the specified
 * thread and then returns to the caller immediately.
 * @end deftypefun
@*/
void jit_debugger_run(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_specific_thread(dbg, thread);
	if(th && th->run_type == JIT_RUN_TYPE_STOPPED)
	{
		th->run_type = JIT_RUN_TYPE_CONTINUE;
		wakeup_all(dbg);
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_step (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread})
 * Step over a single line of code.  If the line performs a method call,
 * then this will step into the call.  The request will be ignored if
 * the thread is currently running.
 * @end deftypefun
@*/
void jit_debugger_step(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_specific_thread(dbg, thread);
	if(th && th->run_type == JIT_RUN_TYPE_STOPPED)
	{
		th->run_type = JIT_RUN_TYPE_STEP;
		wakeup_all(dbg);
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_next (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread})
 * Step over a single line of code but do not step into method calls.
 * The request will be ignored if the thread is currently running.
 * @end deftypefun
@*/
void jit_debugger_next(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_specific_thread(dbg, thread);
	if(th && th->run_type == JIT_RUN_TYPE_STOPPED)
	{
		th->run_type = JIT_RUN_TYPE_NEXT;
		wakeup_all(dbg);
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_finish (jit_debugger_t @var{dbg}, jit_debugger_thread_id_t @var{thread})
 * Keep running until the end of the current function.  The request will
 * be ignored if the thread is currently running.
 * @end deftypefun
@*/
void jit_debugger_finish(jit_debugger_t dbg, jit_debugger_thread_id_t thread)
{
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_specific_thread(dbg, thread);
	if(th && th->run_type == JIT_RUN_TYPE_STOPPED)
	{
		th->run_type = JIT_RUN_TYPE_FINISH;
		wakeup_all(dbg);
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_break (jit_debugger_t @var{dbg})
 * Force an explicit user breakpoint at the current location within the
 * current thread.  Control returns to the caller when the debugger
 * calls one of the above "run" or "step" functions in another thread.
 * @end deftypefun
@*/
void jit_debugger_break(jit_debugger_t dbg)
{
	jit_debugger_event_t *event;
	jit_debugger_thread_t th;
	lock_debugger(dbg);
	th = get_current_thread(dbg);
	if(th && th->breakable)
	{
		event = alloc_event();
		if(event)
		{
			th->run_type = JIT_RUN_TYPE_STOPPED;
			th->find_func = 0;
			th->last_data1 = 0;
			th->last_func_data1 = 0;
			event->type = JIT_DEBUGGER_TYPE_USER_BREAKPOINT;
			event->thread = th->id;
			event->trace = jit_exception_get_stack_trace();
			add_event(dbg, event);
			suspend_thread(dbg, th);
		}
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun void jit_debugger_quit (jit_debugger_t @var{dbg})
 * Sends a request to the thread that called @code{jit_debugger_wait_event}
 * indicating that the debugger should quit.
 * @end deftypefun
@*/
void jit_debugger_quit(jit_debugger_t dbg)
{
	jit_debugger_event_t *event;
	lock_debugger(dbg);
	event = alloc_event();
	if(event)
	{
		event->type = JIT_DEBUGGER_TYPE_QUIT;
		add_event(dbg, event);
	}
	unlock_debugger(dbg);
}

/*@
 * @deftypefun jit_debugger_hook_func jit_debugger_set_hook (jit_context_t @var{context}, jit_debugger_hook_func @var{hook})
 * Set a debugger hook on a JIT context.  Returns the previous hook.
 *
 * Debug hooks are a very low-level breakpoint mechanism.  Upon reaching each
 * breakpoint in a function, a user-supplied hook function is called.
 * It is up to the hook function to decide whether to stop execution
 * or to ignore the breakpoint.  The hook function has the following
 * prototype:
 *
 * @example
 * void hook(jit_function_t func, jit_nint data1, jit_nint data2);
 * @end example
 *
 * The @code{func} argument indicates the function that the breakpoint
 * occurred within.  The @code{data1} and @code{data2} arguments are
 * those supplied to @code{jit_insn_mark_breakpoint}.  The debugger can use
 * these values to indicate information about the breakpoint's type
 * and location.
 *
 * Hook functions can be used for other purposes besides breakpoint
 * debugging.  For example, a program could be instrumented with hooks
 * that tally up the number of times that each function is called,
 * or which profile the amount of time spent in each function.
 *
 * By convention, @code{data1} values less than 10000 are intended for
 * use by user-defined hook functions.  Values of 10000 and greater are
 * reserved for the full-blown debugger system described earlier.
 * @end deftypefun
@*/
jit_debugger_hook_func jit_debugger_set_hook
		(jit_context_t context, jit_debugger_hook_func hook)
{
	jit_debugger_hook_func prev;
	if(context)
	{
		prev = context->debug_hook;
		context->debug_hook = hook;
		return prev;
	}
	else
	{
		return 0;
	}
}

void _jit_debugger_hook(jit_function_t func, jit_nint data1, jit_nint data2)
{
	jit_context_t context;
	jit_debugger_t dbg;
	jit_debugger_thread_t th;
	jit_debugger_event_t *event;
	int stop;

	/* Find the context and look for a user-supplied debug hook */
	context = func->context;
	if(context->debug_hook)
	{
		(*(context->debug_hook))(func, data1, data2);
	}

	/* Ignore breakpoints with data1 values less than 10000.  These are
	   presumed to be handled by a user-supplied debug hook instead */
	if(data1 < JIT_DEBUGGER_DATA1_FIRST)
	{
		return;
	}

	/* Determine if there is a debugger attached to the context */
	dbg = context->debugger;
	if(!dbg)
	{
		return;
	}

	/* Lock down the debugger while we do this */
	lock_debugger(dbg);

	/* Get the current thread's information block */
	th = get_current_thread(dbg);
	if(!th || !(th->breakable))
	{
		unlock_debugger(dbg);
		return;
	}

	/* Determine if there is a hard breakpoint at this location */
	/* TODO */

	/* Determine if we are looking for a soft breakpoint */
	stop = 0;
	switch(th->run_type)
	{
		case JIT_RUN_TYPE_STEP:
		{
			/* Stop at all breakpoints */
			stop = 1;
		}
		break;

		case JIT_RUN_TYPE_NEXT:
		{
			/* Stop only if we are in the same function as the last stopping
			   point, or if we might have already left the function */
			if(func == th->find_func || th->find_func == 0 ||
			   th->last_func_data1 == JIT_DEBUGGER_DATA1_LEAVE ||
			   th->last_data1 == JIT_DEBUGGER_DATA1_THROW)
			{
				stop = 1;
			}
			if(func == th->find_func)
			{
				th->last_func_data1 = data1;
			}
		}
		break;

		case JIT_RUN_TYPE_FINISH:
		{
			/* Stop if we are at a leave point, or we saw an exception */
			if((func == th->find_func && data1 == JIT_DEBUGGER_DATA1_LEAVE) ||
			   th->last_data1 == JIT_DEBUGGER_DATA1_THROW ||
			   th->find_func == 0)
			{
				stop = 1;
			}
		}
		break;
	}
	th->last_data1 = data1;

	/* Do we need to stop the thread at this breakpoint? */
	if(stop)
	{
		event = alloc_event();
		if(event)
		{
			th->run_type = JIT_RUN_TYPE_STOPPED;
			th->find_func = func;
			th->last_func_data1 = data1;
			event->type = JIT_DEBUGGER_TYPE_SOFT_BREAKPOINT;
			event->thread = th->id;
			event->function = func;
			event->data1 = data1;
			event->data2 = data2;
			event->trace = jit_exception_get_stack_trace();
			add_event(dbg, event);
			suspend_thread(dbg, th);
		}
	}

	/* Unlock and exit */
	unlock_debugger(dbg);
}
