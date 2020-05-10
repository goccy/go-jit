/*
 * jit-context.c - Functions for manipulating JIT contexts.
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

Everything that is done with @code{libjit} is done relative to a context.
It is possible to have more than one context at a time - each acts as an
independent environment for compiling and managing code.

When you want to compile a function, you create it with
@code{jit_function_create}, and then populate its body with
calls to the value and instruction functions.  See @xref{Values}, and
@ref{Instructions} for more information on how to do this.

@section Using libjit in a multi-threaded environment

The library does not handle the creation, management, and destruction
of threads itself.  It is up to the front-end environment to take
care of that.  But the library is thread-aware, as long as you take
some very simple steps.

In a multi-threaded environment, you must ensure that only one
thread can build functions at any one time.  Otherwise the
JIT's context may become corrupted.  To protect the system,
you should call @code{jit_context_build_start} before
creating the function.  And then call @code{jit_context_build_end}
once the function has been fully compiled.

You can compile multiple functions during the one build process
if you wish, which is the normal case when compiling a class.

It is usually a good idea to suspend the finalization of
garbage-collected objects while function building is in progress.
Otherwise you may get a deadlock when the finalizer thread tries
to call the builder to compile a finalization routine.  Suspension
of finalization is the responsibility of the caller.

@section Context functions
@cindex jit-context.h

The following functions are available to create, manage, and
ultimately destroy JIT contexts:

@*/

/*@
 * @deftypefun jit_context_t jit_context_create (void)
 * Create a new context block for the JIT.  Returns NULL
 * if out of memory.
 * @end deftypefun
@*/
jit_context_t
jit_context_create(void)
{
	jit_context_t context;

	/* Make sure that the JIT is initialized */
	jit_init();

	/* Allocate memory for the context */
	context = jit_cnew(struct _jit_context);
	if(!context)
	{
		return 0;
	}

	/* Initialize the context and return it */
	jit_mutex_create(&context->memory_lock);
	jit_mutex_create(&context->builder_lock);
	context->functions = 0;
	context->last_function = 0;
	context->on_demand_driver = _jit_function_compile_on_demand;
	context->memory_manager = jit_default_memory_manager();
	return context;
}

/*@
 * @deftypefun void jit_context_destroy (jit_context_t @var{context})
 * Destroy a JIT context block and everything that is associated with it.
 * It is very important that no threads within the program are currently
 * running compiled code when this function is called.
 * @end deftypefun
@*/
void
jit_context_destroy(jit_context_t context)
{
	int sym;

	if(!context)
	{
		return;
	}

	for(sym = 0; sym < context->num_registered_symbols; ++sym)
	{
		jit_free(context->registered_symbols[sym]);
	}
	jit_free(context->registered_symbols);

	while(context->functions != 0)
	{
		_jit_function_destroy(context->functions);
	}

	_jit_memory_destroy(context);

	jit_mutex_destroy(&context->memory_lock);
	jit_mutex_destroy(&context->builder_lock);

	jit_free(context);
}

/*@
 * @deftypefun void jit_context_build_start (jit_context_t @var{context})
 * This routine should be called before you start building a function
 * to be JIT'ed.  It acquires a lock on the context to prevent other
 * threads from accessing the build process, since only one thread
 * can be performing build operations at any one time.
 * @end deftypefun
@*/
void
jit_context_build_start(jit_context_t context)
{
	jit_mutex_lock(&context->builder_lock);
}

/*@
 * @deftypefun void jit_context_build_end (jit_context_t @var{context})
 * This routine should be called once you have finished building
 * and compiling a function and are ready to resume normal execution.
 * This routine will release the build lock, allowing other threads
 * that are waiting on the builder to proceed.
 * @end deftypefun
@*/
void
jit_context_build_end(jit_context_t context)
{
	jit_mutex_unlock(&context->builder_lock);
}

/*@
 * @deftypefun void jit_context_set_on_demand_driver (jit_context_t @var{context}, jit_on_demand_driver_func @var{driver})
 * Specify the C function to be called to drive on-demand compilation.
 *
 * When on-demand compilation is requested the default driver provided by
 * @code{libjit} takes the following actions:
 *
 * @enumerate
 * @item
 * The context is locked by calling @code{jit_context_build_start}.
 *
 * @item
 * If the function has already been compiled, @code{libjit} unlocks
 * the context and returns immediately.  This can happen because of race
 * conditions between threads: some other thread may have beaten us
 * to the on-demand compiler.
 *
 * @item
 * The user's on-demand compiler is called.  It is responsible for building
 * the instructions in the function's body.  It should return one of the
 * result codes @code{JIT_RESULT_OK}, @code{JIT_RESULT_COMPILE_ERROR},
 * or @code{JIT_RESULT_OUT_OF_MEMORY}.
 *
 * @item
 * If the user's on-demand function hasn't already done so, @code{libjit}
 * will call @code{jit_function_compile} to compile the function.
 *
 * @item
 * The context is unlocked by calling @code{jit_context_build_end} and
 * @code{libjit} jumps to the newly-compiled entry point.  If an error
 * occurs, a built-in exception of type @code{JIT_RESULT_COMPILE_ERROR}
 * or @code{JIT_RESULT_OUT_OF_MEMORY} will be thrown.
 *
 * @item
 * The entry point of the compiled function is returned from the driver.
 * @end enumerate
 *
 * You may need to provide your own driver if some additional actions
 * are required.
 *
 * @end deftypefun
@*/
void
jit_context_set_on_demand_driver(jit_context_t context, jit_on_demand_driver_func driver)
{
	if (driver)
	{
		context->on_demand_driver = driver;
	}
	else
	{
		context->on_demand_driver = _jit_function_compile_on_demand;
	}
}

/*@
 * @deftypefun void jit_context_set_memory_manager (jit_context_t @var{context}, jit_memory_manager_t @var{manager})
 * Specify the memory manager plug-in.
 * @end deftypefun
@*/
void
jit_context_set_memory_manager(jit_context_t context, jit_memory_manager_t manager)
{
	/* Bail out if there is already an established memory context */
	if (context->memory_context)
	{
		return;
	}

	/* Set the context memory manager */
	if (manager)
	{
		context->memory_manager = manager;
	}
	else
	{
		context->memory_manager = jit_default_memory_manager();
	}
}

/*@
 * @deftypefun int jit_context_set_meta (jit_context_t @var{context}, int @var{type}, void *@var{data}, jit_meta_free_func @var{free_data})
 * Tag a context with some metadata.  Returns zero if out of memory.
 *
 * Metadata may be used to store dependency graphs, branch prediction
 * information, or any other information that is useful to optimizers
 * or code generators.  It can also be used by higher level user code
 * to store information about the context that is specific to the
 * virtual machine or language.
 *
 * If the @var{type} already has some metadata associated with it, then
 * the previous value will be freed.
 * @end deftypefun
@*/
int
jit_context_set_meta(jit_context_t context, int type, void *data, jit_meta_free_func free_data)
{
	return jit_meta_set(&(context->meta), type, data, free_data, 0);
}

/*@
 * @deftypefun int jit_context_set_meta_numeric (jit_context_t @var{context}, int @var{type}, jit_nuint @var{data})
 * Tag a context with numeric metadata.  Returns zero if out of memory.
 * This function is more convenient for accessing the context's
 * special option values:
 *
 * @table @code
 * @vindex JIT_OPTION_CACHE_LIMIT
 * @item JIT_OPTION_CACHE_LIMIT
 * A numeric option that indicates the maximum size in bytes of the function
 * cache.  If set to zero (the default), the function cache is unlimited
 * in size.
 *
 * @vindex JIT_OPTION_CACHE_PAGE_SIZE
 * @item JIT_OPTION_CACHE_PAGE_SIZE
 * A numeric option that indicates the size in bytes of a single page in the
 * function cache.  Memory is allocated for the cache in chunks of
 * this size.  If set to zero, the cache page size is set to an
 * internally-determined default (usually 128k).  The cache page size
 * also determines the maximum size of a single compiled function.
 *
 * @vindex JIT_OPTION_PRE_COMPILE
 * @item JIT_OPTION_PRE_COMPILE
 * A numeric option that indicates that this context is being used
 * for pre-compilation if it is set to a non-zero value.  Code within
 * pre-compiled contexts cannot be executed directly.  Instead, they
 * can be written out to disk in ELF format to be reloaded at
 * some future time.
 *
 * @vindex JIT_OPTION_DONT_FOLD
 * @item JIT_OPTION_DONT_FOLD
 * A numeric option that disables constant folding when it is set to a
 * non-zero value.  This is useful for debugging, as it forces @code{libjit} to
 * always execute constant expressions at run time, instead of at compile time.
 *
 * @vindex JIT_OPTION_POSITION_INDEPENDENT
 * @item JIT_OPTION_POSITION_INDEPENDENT
 * A numeric option that forces generation of position-independent code (PIC)
 * if it is set to a non-zero value. This may be mainly useful for pre-compiled
 * contexts.
 * @end table
 *
 * Metadata type values of 10000 or greater are reserved for internal use.
 * @end deftypefun
@*/
int
jit_context_set_meta_numeric(jit_context_t context, int type, jit_nuint data)
{
	return jit_meta_set(&(context->meta), type, (void *)data, 0, 0);
}

/*@
 * @deftypefun {void *} jit_context_get_meta (jit_context_t @var{context}, int @var{type})
 * Get the metadata associated with a particular tag.  Returns NULL
 * if @var{type} does not have any metadata associated with it.
 * @end deftypefun
@*/
void *
jit_context_get_meta(jit_context_t context, int type)
{
	return jit_meta_get(context->meta, type);
}

/*@
 * @deftypefun jit_nuint jit_context_get_meta_numeric (jit_context_t @var{context}, int @var{type})
 * Get the metadata associated with a particular tag.  Returns zero
 * if @var{type} does not have any metadata associated with it.
 * This version is more convenient for the pre-defined numeric option values.
 * @end deftypefun
@*/
jit_nuint
jit_context_get_meta_numeric(jit_context_t context, int type)
{
	return (jit_nuint)jit_meta_get(context->meta, type);
}

/*@
 * @deftypefun void jit_context_free_meta (jit_context_t @var{context}, int @var{type})
 * Free metadata of a specific type on a context.  Does nothing if
 * the @var{type} does not have any metadata associated with it.
 * @end deftypefun
@*/
void
jit_context_free_meta(jit_context_t context, int type)
{
	jit_meta_free(&(context->meta), type);
}
