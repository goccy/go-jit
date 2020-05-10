/*
 * jit-function.c - Functions for manipulating function blocks.
 *
 * Copyright (C) 2004, 2006-2008  Southern Storm Software, Pty Ltd.
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
#include "jit-apply-func.h"
#include "jit-rules.h"
#include "jit-setjmp.h"

/*@
 * @deftypefun jit_function_t jit_function_create (jit_context_t @var{context}, jit_type_t @var{signature})
 * Create a new function block and associate it with a JIT context.
 * Returns NULL if out of memory.
 *
 * A function persists for the lifetime of its containing context.
 * It initially starts life in the "building" state, where the user
 * constructs instructions that represents the function body.
 * Once the build process is complete, the user calls
 * @code{jit_function_compile} to convert it into its executable form.
 *
 * It is recommended that you call @code{jit_context_build_start} before
 * calling @code{jit_function_create}, and then call
 * @code{jit_context_build_end} after you have called
 * @code{jit_function_compile}.  This will protect the JIT's internal
 * data structures within a multi-threaded environment.
 * @end deftypefun
@*/
jit_function_t
jit_function_create(jit_context_t context, jit_type_t signature)
{
	jit_function_t func;
#if !defined(JIT_BACKEND_INTERP) && (defined(jit_redirector_size) || defined(jit_indirector_size))
	unsigned char *trampoline;
#endif

	/* Acquire the memory context */
	_jit_memory_lock(context);
	if(!_jit_memory_ensure(context))
	{
		_jit_memory_unlock(context);
		return 0;
	}

	/* Allocate memory for the function and clear it */
	func = _jit_memory_alloc_function(context);
	if(!func)
	{
		_jit_memory_unlock(context);
		return 0;
	}

#if !defined(JIT_BACKEND_INTERP) && (defined(jit_redirector_size) || defined(jit_indirector_size))
	trampoline = (unsigned char *) _jit_memory_alloc_trampoline(context);
	if(!trampoline)
	{
		_jit_memory_free_function(context, func);
		_jit_memory_unlock(context);
		return 0;
	}
# if defined(jit_redirector_size)
	func->redirector = trampoline;
	trampoline += jit_redirector_size;
# endif
# if defined(jit_indirector_size)
	func->indirector = trampoline;
# endif
#endif /* !defined(JIT_BACKEND_INTERP) && (defined(jit_redirector_size) || defined(jit_indirector_size)) */

	/* Release the memory context */
	_jit_memory_unlock(context);

	/* Initialize the function block */
	func->context = context;
	func->signature = jit_type_copy(signature);
	func->optimization_level = JIT_OPTLEVEL_NORMAL;

#if !defined(JIT_BACKEND_INTERP) && defined(jit_redirector_size)
	/* If we aren't using interpretation, then point the function's
	   initial entry point at the redirector, which in turn will
	   invoke the on-demand compiler */
	func->entry_point = _jit_create_redirector
		(func->redirector, (void *) context->on_demand_driver,
		 func, jit_type_get_abi(signature));
	_jit_flush_exec(func->redirector, jit_redirector_size);
#endif
#if !defined(JIT_BACKEND_INTERP) && defined(jit_indirector_size)
	_jit_create_indirector(func->indirector, (void**) &(func->entry_point));
	_jit_flush_exec(func->indirector, jit_indirector_size);
#endif

	/* Add the function to the context list */
	func->next = 0;
	func->prev = context->last_function;
	if(context->last_function)
	{
		context->last_function->next = func;
	}
	else
	{
		context->functions = func;
	}
	context->last_function = func;

	/* Return the function to the caller */
	return func;
}

/*@
 * @deftypefun jit_function_t jit_function_create_nested (jit_context_t @var{context}, jit_type_t @var{signature}, jit_function_t @var{parent})
 * Create a new function block and associate it with a JIT context.
 * In addition, this function is nested inside the specified
 * @var{parent} function and is able to access its parent's
 * (and grandparent's) local variables.
 *
 * The front end is responsible for ensuring that the nested function
 * is compiled before its parent.
 * @end deftypefun
@*/
jit_function_t jit_function_create_nested
		(jit_context_t context, jit_type_t signature, jit_function_t parent)
{
	jit_function_t func;
	func = jit_function_create(context, signature);
	if(!func)
	{
		return 0;
	}
	func->nested_parent = parent;
	return func;
}

int _jit_function_ensure_builder(jit_function_t func)
{
	/* Handle the easy cases first */
	if(!func)
	{
		return 0;
	}
	if(func->builder)
	{
		return 1;
	}

	/* Allocate memory for the builder and clear it */
	func->builder = jit_cnew(struct _jit_builder);
	if(!(func->builder))
	{
		return 0;
	}

	/* Cache the value of the JIT_OPTION_POSITION_INDEPENDENT option */
	func->builder->position_independent
		= jit_context_get_meta_numeric(
			func->context, JIT_OPTION_POSITION_INDEPENDENT);

	/* Initialize the function builder */
	jit_memory_pool_init(&(func->builder->value_pool), struct _jit_value);
	jit_memory_pool_init(&(func->builder->edge_pool), struct _jit_edge);
	jit_memory_pool_init(&(func->builder->meta_pool), struct _jit_meta);

	/* Create the entry block */
	if(!_jit_block_init(func))
	{
		_jit_function_free_builder(func);
		return 0;
	}

	/* Create instructions to initialize the incoming arguments */
	func->builder->current_block = func->builder->entry_block;
	if(!_jit_create_entry_insns(func))
	{
		_jit_function_free_builder(func);
		return 0;
	}

	/* The current position is where initialization code will be
	   inserted by "jit_insn_move_blocks_to_start" */
	func->builder->init_block = func->builder->current_block;

	/* Start first block for function body */
	if(!jit_insn_new_block(func))
	{
		_jit_function_free_builder(func);
		return 0;
	}

	/* The builder is ready to go */
	return 1;
}

void _jit_function_free_builder(jit_function_t func)
{
	if(func->builder)
	{
		_jit_block_free(func);
		jit_memory_pool_free(&(func->builder->edge_pool), 0);
		jit_memory_pool_free(&(func->builder->value_pool), _jit_value_free);
		jit_memory_pool_free(&(func->builder->meta_pool), _jit_meta_free_one);
		jit_free(func->builder->param_values);
		jit_free(func->builder->label_info);
		jit_free(func->builder);
		func->builder = 0;
		func->is_optimized = 0;
	}
}

void
_jit_function_destroy(jit_function_t func)
{
	jit_context_t context;

	if(!func)
	{
		return;
	}

	context = func->context;
	if(func->next)
	{
		func->next->prev = func->prev;
	}
	else
	{
		context->last_function = func->prev;
	}
	if(func->prev)
	{
		func->prev->next = func->next;
	}
	else
	{
		context->functions = func->next;
	}

	_jit_function_free_builder(func);
	_jit_varint_free_data(func->bytecode_offset);
	jit_meta_destroy(&func->meta);
	jit_type_free(func->signature);

	_jit_memory_lock(context);

#if !defined(JIT_BACKEND_INTERP) && (defined(jit_redirector_size) || defined(jit_indirector_size))
# if defined(jit_redirector_size)
	_jit_memory_free_trampoline(context, func->redirector);
# else
	_jit_memory_free_trampoline(context, func->indirector);
# endif
#endif
	_jit_memory_free_function(context, func);

	_jit_memory_unlock(context);
}

/*@
 * @deftypefun void jit_function_abandon (jit_function_t @var{func})
 * Abandon this function during the build process.  This should be called
 * when you detect a fatal error that prevents the function from being
 * properly built.  The @var{func} object is completely destroyed and
 * detached from its owning context.  The function is left alone if
 * it was already compiled.
 * @end deftypefun
@*/
void jit_function_abandon(jit_function_t func)
{
	if(func && func->builder)
	{
		if(func->is_compiled)
		{
			/* We already compiled this function previously, but we
			   have tried to recompile it with new contents.  Throw
			   away the builder, but keep the original version */
			_jit_function_free_builder(func);
		}
		else
		{
			/* This function was never compiled, so abandon entirely */
			_jit_function_destroy(func);
		}
	}
}

/*@
 * @deftypefun jit_context_t jit_function_get_context (jit_function_t @var{func})
 * Get the context associated with a function.
 * @end deftypefun
@*/
jit_context_t jit_function_get_context(jit_function_t func)
{
	if(func)
	{
		return func->context;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_type_t jit_function_get_signature (jit_function_t @var{func})
 * Get the signature associated with a function.
 * @end deftypefun
@*/
jit_type_t jit_function_get_signature(jit_function_t func)
{
	if(func)
	{
		return func->signature;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_function_set_meta (jit_function_t @var{func}, int @var{type}, void *@var{data}, jit_meta_free_func @var{free_data}, int @var{build_only})
 * Tag a function with some metadata.  Returns zero if out of memory.
 *
 * Metadata may be used to store dependency graphs, branch prediction
 * information, or any other information that is useful to optimizers
 * or code generators.  It can also be used by higher level user code
 * to store information about the function that is specific to the
 * virtual machine or language.
 *
 * If the @var{type} already has some metadata associated with it, then
 * the previous value will be freed.
 *
 * If @var{build_only} is non-zero, then the metadata will be freed
 * when the function is compiled with @code{jit_function_compile}.
 * Otherwise the metadata will persist until the JIT context is destroyed,
 * or @code{jit_function_free_meta} is called for the specified @var{type}.
 *
 * Metadata type values of 10000 or greater are reserved for internal use.
 * @end deftypefun
@*/
int jit_function_set_meta(jit_function_t func, int type, void *data,
					      jit_meta_free_func free_data, int build_only)
{
	if(build_only)
	{
		if(!_jit_function_ensure_builder(func))
		{
			return 0;
		}
		return jit_meta_set(&(func->builder->meta), type, data,
							free_data, func);
	}
	else
	{
		return jit_meta_set(&(func->meta), type, data, free_data, 0);
	}
}

/*@
 * @deftypefun {void *} jit_function_get_meta (jit_function_t @var{func}, int @var{type})
 * Get the metadata associated with a particular tag.  Returns NULL
 * if @var{type} does not have any metadata associated with it.
 * @end deftypefun
@*/
void *jit_function_get_meta(jit_function_t func, int type)
{
	void *data = jit_meta_get(func->meta, type);
	if(!data && func->builder)
	{
		data = jit_meta_get(func->builder->meta, type);
	}
	return data;
}

/*@
 * @deftypefun void jit_function_free_meta (jit_function_t @var{func}, int @var{type})
 * Free metadata of a specific type on a function.  Does nothing if
 * the @var{type} does not have any metadata associated with it.
 * @end deftypefun
@*/
void jit_function_free_meta(jit_function_t func, int type)
{
	jit_meta_free(&(func->meta), type);
	if(func->builder)
	{
		jit_meta_free(&(func->builder->meta), type);
	}
}

/*@
 * @deftypefun jit_function_t jit_function_next (jit_context_t @var{context}, jit_function_t @var{prev})
 * Iterate over the defined functions in creation order.  The @var{prev}
 * argument should be NULL on the first call.  Returns NULL at the end.
 * @end deftypefun
@*/
jit_function_t jit_function_next(jit_context_t context, jit_function_t prev)
{
	if(prev)
	{
		return prev->next;
	}
	else if(context)
	{
		return context->functions;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_function_t jit_function_previous (jit_context_t @var{context}, jit_function_t @var{prev})
 * Iterate over the defined functions in reverse creation order.
 * @end deftypefun
@*/
jit_function_t jit_function_previous(jit_context_t context, jit_function_t prev)
{
	if(prev)
	{
		return prev->prev;
	}
	else if(context)
	{
		return context->last_function;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_block_t jit_function_get_entry (jit_function_t @var{func})
 * Get the entry block for a function.  This is always the first block
 * created by @code{jit_function_create}.
 * @end deftypefun
@*/
jit_block_t jit_function_get_entry(jit_function_t func)
{
	if(func && func->builder)
	{
		return func->builder->entry_block;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_block_t jit_function_get_current (jit_function_t @var{func})
 * Get the current block for a function.  New blocks are created by
 * certain @code{jit_insn_xxx} calls.
 * @end deftypefun
@*/
jit_block_t jit_function_get_current(jit_function_t func)
{
	if(func && func->builder)
	{
		return func->builder->current_block;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_function_t jit_function_get_nested_parent (jit_function_t @var{func})
 * Get the nested parent for a function, or NULL if @var{func}
 * does not have a nested parent.
 * @end deftypefun
@*/
jit_function_t jit_function_get_nested_parent(jit_function_t func)
{
	if(func)
	{
		return func->nested_parent;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_function_t jit_function_get_nested_parent (jit_function_t @var{func}, jit_value_t @var{parent_frame})
 * Set the frame pointer of the parent of a nested function
 * @end deftypefun
@*/
void jit_function_set_parent_frame(jit_function_t func,
	jit_value_t parent_frame)
{
	func->parent_frame = parent_frame;
	func->cached_parent = NULL;
	func->cached_parent_frame = NULL;
}

/*
 * Information that is stored for an exception region in the cache.
 */
typedef struct jit_cache_eh *jit_cache_eh_t;
struct jit_cache_eh
{
	jit_label_t		handler_label;
	unsigned char  *handler;
	jit_cache_eh_t	previous;
};

/*@
 * @deftypefun int jit_function_is_compiled (jit_function_t @var{func})
 * Determine if a function has already been compiled.
 * @end deftypefun
@*/
int jit_function_is_compiled(jit_function_t func)
{
	if(func)
	{
		return func->is_compiled;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_function_set_recompilable (jit_function_t @var{func})
 * Mark this function as a candidate for recompilation.  That is,
 * it is possible that we may call @code{jit_function_compile}
 * more than once, to re-optimize an existing function.
 *
 * It is very important that this be called before the first time that
 * you call @code{jit_function_compile}.  Functions that are recompilable
 * are invoked in a slightly different way to non-recompilable functions.
 * If you don't set this flag, then existing invocations of the function
 * may continue to be sent to the original compiled version, not the new
 * version.
 * @end deftypefun
@*/
void jit_function_set_recompilable(jit_function_t func)
{
	if(func)
	{
		func->is_recompilable = 1;
	}
}

/*@
 * @deftypefun void jit_function_clear_recompilable (jit_function_t @var{func})
 * Clear the recompilable flag on this function.  Normally you would use
 * this once you have decided that the function has been optimized enough,
 * and that you no longer intend to call @code{jit_function_compile} again.
 *
 * Future uses of the function with @code{jit_insn_call} will output a
 * direct call to the function, which is more efficient than calling
 * its recompilable version.  Pre-existing calls to the function may still
 * use redirection stubs, and will remain so until the pre-existing
 * functions are themselves recompiled.
 * @end deftypefun
@*/
void jit_function_clear_recompilable(jit_function_t func)
{
	if(func)
	{
		func->is_recompilable = 0;
	}
}

/*@
 * @deftypefun int jit_function_is_recompilable (jit_function_t @var{func})
 * Determine if this function is recompilable.
 * @end deftypefun
@*/
int jit_function_is_recompilable(jit_function_t func)
{
	if(func)
	{
		return func->is_recompilable;
	}
	else
	{
		return 0;
	}
}

#ifdef JIT_BACKEND_INTERP

/*
 * Closure handling function for "jit_function_to_closure".
 */
static void function_closure(jit_type_t signature, void *result,
							 void **args, void *user_data)
{
	if(!jit_function_apply((jit_function_t)user_data, args, result))
	{
		/* We cannot report the exception through the closure,
		   so we have no choice but to rethrow it up the stack */
		jit_exception_throw(jit_exception_get_last());
	}
}

#endif /* JIT_BACKEND_INTERP */

/*@
 * @deftypefun {void *} jit_function_to_closure (jit_function_t @var{func})
 * Convert a compiled function into a closure that can called directly
 * from C.  Returns NULL if out of memory, or if closures are not
 * supported on this platform.
 *
 * If the function has not been compiled yet, then this will return
 * a pointer to a redirector that will arrange for the function to be
 * compiled on-demand when it is called.
 *
 * Creating a closure for a nested function is not recommended as
 * C does not have any way to call such closures directly.
 * @end deftypefun
@*/
void *jit_function_to_closure(jit_function_t func)
{
	if(!func)
	{
		return 0;
	}
#ifdef JIT_BACKEND_INTERP
	return jit_closure_create(func->context, func->signature,
							  function_closure, (void *)func);
#else
	/* On native platforms, use the closure entry point */
	if(func->indirector && (!func->is_compiled || func->is_recompilable))
	{
		return func->indirector;
	}
	return func->entry_point;
#endif
}

/*@
 * @deftypefun jit_function_t jit_function_from_closure (jit_context_t @var{context}, void *@var{closure})
 * Convert a closure back into a function.  Returns NULL if the
 * closure does not correspond to a function in the specified context.
 * @end deftypefun
@*/
jit_function_t
jit_function_from_closure(jit_context_t context, void *closure)
{
	void *func_info;

	if(!context)
	{
		return 0;
	}

	func_info = _jit_memory_find_function_info(context, closure);
	if(!func_info)
	{
		return 0;
	}

	return _jit_memory_get_function(context, func_info);
}

/*@
 * @deftypefun jit_function_t jit_function_from_pc (jit_context_t @var{context}, void *@var{pc}, void **@var{handler})
 * Get the function that contains the specified program counter location.
 * Also return the address of the @code{catch} handler for the same location.
 * Returns NULL if the program counter does not correspond to a function
 * under the control of @var{context}.
 * @end deftypefun
@*/
jit_function_t
jit_function_from_pc(jit_context_t context, void *pc, void **handler)
{
	void *func_info;
	jit_function_t func;

	if(!context)
	{
		return 0;
	}

	/* Get the function and the exception handler cookie */
	func_info = _jit_memory_find_function_info(context, pc);
	if(!func_info)
	{
		return 0;
	}
	func = _jit_memory_get_function(context, func_info);
	if(!func)
	{
		return 0;
	}

	/* Convert the cookie into a handler address */
	if(handler)
	{
#if 0
		if(func->cookie)
		{
			*handler = ((jit_cache_eh_t) func->cookie)->handler;
		}
		else
		{
			*handler = 0;
		}
#else
		*handler = func->cookie;
#endif
	}
	return func;
}

/*@
 * @deftypefun {void *} jit_function_to_vtable_pointer (jit_function_t @var{func})
 * Return a pointer that is suitable for referring to this function
 * from a vtable.  Such pointers should only be used with the
 * @code{jit_insn_call_vtable} instruction.
 *
 * Using @code{jit_insn_call_vtable} is generally more efficient than
 * @code{jit_insn_call_indirect} for calling virtual methods.
 *
 * The vtable pointer might be the same as the closure, but this isn't
 * guaranteed.  Closures can be used with @code{jit_insn_call_indirect}.
 * @end deftypefun
@*/
void *
jit_function_to_vtable_pointer(jit_function_t func)
{
#ifdef JIT_BACKEND_INTERP
	/* In the interpreted version, the function pointer is used in vtables */
	return func;
#else
	/* On native platforms, the closure entry point is the vtable pointer */
	if(!func)
	{
		return 0;
	}
	if(func->indirector && (!func->is_compiled || func->is_recompilable))
	{
		return func->indirector;
	}
	return func->entry_point;
#endif
}

/*@
 * @deftypefun jit_function_t jit_function_from_vtable_pointer (jit_context_t @var{context}, void *@var{vtable_pointer})
 * Convert a vtable_pointer back into a function.  Returns NULL if the
 * vtable_pointer does not correspond to a function in the specified context.
 * @end deftypefun
@*/
jit_function_t
jit_function_from_vtable_pointer(jit_context_t context, void *vtable_pointer)
{
#ifdef JIT_BACKEND_INTERP
	/* In the interpreted version, the function pointer is used in vtables */
	jit_function_t func = (jit_function_t)vtable_pointer;

	if(func && func->context == context)
	{
		return func;
	}
	return 0;
#else
	void *func_info;

	if(!context)
	{
		return 0;
	}

	func_info = _jit_memory_find_function_info(context, vtable_pointer);
	if(!func_info)
	{
		return 0;
	}

	return _jit_memory_get_function(context, func_info);
#endif
}

/*@
 * @deftypefun void jit_function_set_on_demand_compiler (jit_function_t @var{func}, jit_on_demand_func @var{on_demand})
 * Specify the C function to be called when @var{func} needs to be
 * compiled on-demand.  This should be set just after the function
 * is created, before any build or compile processes begin.
 *
 * You won't need an on-demand compiler if you always build and compile
 * your functions before you call them.  But if you can call a function
 * before it is built, then you must supply an on-demand compiler.
 *
 * When on-demand compilation is requested, @code{libjit} takes the following
 * actions:
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
 * @end enumerate
 *
 * Normally you will need some kind of context information to tell you
 * which higher-level construct is being compiled.  You can use the
 * metadata facility to add this context information to the function
 * just after you create it with @code{jit_function_create}.
 * @end deftypefun
@*/
void
jit_function_set_on_demand_compiler(jit_function_t func, jit_on_demand_func on_demand)
{
	if(func)
	{
		func->on_demand = on_demand;
	}
}

/*@
 * @deftypefun jit_on_demand_func jit_function_get_on_demand_compiler (jit_function_t @var{func})
 * Returns function's on-demand compiler.
 * @end deftypefun
@*/
jit_on_demand_func
jit_function_get_on_demand_compiler(jit_function_t func)
{
	if(func)
	{
		return func->on_demand;
	}
	return 0;
}

/*@
 * @deftypefun int jit_function_apply (jit_function_t @var{func}, void **@var{args}, void *@var{return_area})
 * Call the function @var{func} with the supplied arguments.  Each element
 * in @var{args} is a pointer to one of the arguments, and @var{return_area}
 * points to a buffer to receive the return value.  Returns zero if an
 * exception occurred.
 *
 * This is the primary means for executing a function from ordinary
 * C code without creating a closure first with @code{jit_function_to_closure}.
 * Closures may not be supported on all platforms, but function application
 * is guaranteed to be supported everywhere.
 *
 * Function applications acts as an exception blocker.  If any exceptions
 * occur during the execution of @var{func}, they won't travel up the
 * stack any further than this point.  This prevents ordinary C code
 * from being accidentally presented with a situation that it cannot handle.
 * This blocking protection is not present when a function is invoked
 * via its closure.
 * @end deftypefun
 *
 * @deftypefun int jit_function_apply_vararg (jit_function_t @var{func}, jit_type_t @var{signature}, void **@var{args}, void *@var{return_area})
 * Call the function @var{func} with the supplied arguments.  There may
 * be more arguments than are specified in the function's original signature,
 * in which case the additional values are passed as variable arguments.
 * This function is otherwise identical to @code{jit_function_apply}.
 * @end deftypefun
@*/
#if !defined(JIT_BACKEND_INTERP)
/* The interpreter version is in "jit-interp.cpp" */

int jit_function_apply(jit_function_t func, void **args, void *return_area)
{
	if(func)
	{
		return jit_function_apply_vararg
			(func, func->signature, args, return_area);
	}
	else
	{
		return jit_function_apply_vararg(func, 0, args, return_area);
	}
}

int jit_function_apply_vararg
	(jit_function_t func, jit_type_t signature, void **args, void *return_area)
{
	struct jit_backtrace call_trace;
	void *entry;
	jit_jmp_buf jbuf;

	/* Establish a "setjmp" point here so that we can unwind the
	   stack to this point when an exception occurs and then prevent
	   the exception from propagating further up the stack */
	_jit_unwind_push_setjmp(&jbuf);
	if(setjmp(jbuf.buf))
	{
		_jit_unwind_pop_setjmp();
		return 0;
	}

	/* Create a backtrace entry that blocks exceptions from
	   flowing further than this up the stack */
	_jit_backtrace_push(&call_trace, 0);

	/* Get the function's entry point */
	if(!func)
	{
		jit_exception_builtin(JIT_RESULT_NULL_FUNCTION);
		return 0;
	}
	if(func->nested_parent)
	{
		jit_exception_builtin(JIT_RESULT_CALLED_NESTED);
		return 0;
	}
	if(func->is_compiled)
	{
		entry = func->entry_point;
	}
	else
	{
		entry = (*func->context->on_demand_driver)(func);
	}

	/* Get the default signature if necessary */
	if(!signature)
	{
		signature = func->signature;
	}

	/* Clear the exception state */
	jit_exception_clear_last();

	/* Apply the function.  If it returns, then there is no exception */
	jit_apply(signature, entry, args, jit_type_num_params(func->signature), return_area);

	/* Restore the backtrace and "setjmp" contexts and exit */
	_jit_unwind_pop_setjmp();
	return 1;
}

#endif /* !JIT_BACKEND_INTERP */

/*@
 * @deftypefun void jit_function_set_optimization_level (jit_function_t @var{func}, unsigned int @var{level})
 * Set the optimization level for @var{func}.  Increasing values indicate
 * that the @code{libjit} dynamic compiler should expend more effort to
 * generate better code for this function.  Usually you would increase
 * this value just before forcing @var{func} to recompile.
 *
 * When the optimization level reaches the value returned by
 * @code{jit_function_get_max_optimization_level()}, there is usually
 * little point in continuing to recompile the function because
 * @code{libjit} may not be able to do any better.
 *
 * The front end is usually responsible for choosing candidates for
 * function inlining.  If it has identified more such candidates, then
 * it may still want to recompile @var{func} again even once it has
 * reached the maximum optimization level.
 * @end deftypefun
@*/
void
jit_function_set_optimization_level(jit_function_t func, unsigned int level)
{
	unsigned int max_level = jit_function_get_max_optimization_level();
	if(level > max_level)
	{
		level = max_level;
	}
	if(func)
	{
		func->optimization_level = level;
	}
}

/*@
 * @deftypefun {unsigned int} jit_function_get_optimization_level (jit_function_t @var{func})
 * Get the current optimization level for @var{func}.
 * @end deftypefun
@*/
unsigned int
jit_function_get_optimization_level(jit_function_t func)
{
	if(func)
	{
		return func->optimization_level;
	}
	else
	{
		return JIT_OPTLEVEL_NONE;
	}
}

/*@
 * @deftypefun {unsigned int} jit_function_get_max_optimization_level (void)
 * Get the maximum optimization level that is supported by @code{libjit}.
 * @end deftypefun
@*/
unsigned int
jit_function_get_max_optimization_level(void)
{
	return JIT_OPTLEVEL_NORMAL;
}

/*@
 * @deftypefun {jit_label_t} jit_function_reserve_label (jit_function_t @var{func})
 * Allocate a new label for later use within the function @var{func}.  Most
 * instructions that require a label could perform label allocation themselves.
 * A separate label allocation could be useful to fill a jump table with
 * identical entries.
 * @end deftypefun
@*/
jit_label_t
jit_function_reserve_label(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return jit_label_undefined;
	}

	return (func->builder->next_label)++;
}

/*@
 * @deftypefun {int} jit_function_labels_equal (jit_function_t @var{func}, jit_label_t @var{label}, jit_label_t @var{label2})
 * Check if labels @var{label} and @var{label2} defined within the function
 * @var{func} are equal that is belong to the same basic block.  Labels that
 * are not associated with any block are never considered equal.
 * @end deftypefun
@*/
int
jit_function_labels_equal(jit_function_t func, jit_label_t label, jit_label_t label2)
{
	jit_block_t block, block2;

	if(func && func->builder
	   && label != jit_label_undefined
	   && label2 != jit_label_undefined
	   && label < func->builder->max_label_info
	   && label2 < func->builder->max_label_info)
	{
		block = func->builder->label_info[label].block;
		if(block)
		{
			block2 = func->builder->label_info[label2].block;
			return block == block2;
		}
	}

	return 0;
}
