/*
 * jit-memory.c - Memory management.
 *
 * Copyright (C) 2012  Aleksey Demakov
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

void
_jit_memory_lock(jit_context_t context)
{
	jit_mutex_lock(&context->memory_lock);
}

void
_jit_memory_unlock(jit_context_t context)
{
	jit_mutex_unlock(&context->memory_lock);
}

int
_jit_memory_ensure(jit_context_t context)
{
	if(!context->memory_context)
	{
 		context->memory_context = context->memory_manager->create(context);
	}
	return (context->memory_context != 0);
}

void
_jit_memory_destroy(jit_context_t context)
{
	if(!context->memory_context)
	{
		return;
	}
	context->memory_manager->destroy(context->memory_context);
}

jit_function_info_t
_jit_memory_find_function_info(jit_context_t context, void *pc)
{
	if(!context->memory_context)
	{
		return 0;
	}
	/* TODO: read lock? */
	return context->memory_manager->find_function_info(context->memory_context, pc);
}

jit_function_t
_jit_memory_get_function(jit_context_t context, jit_function_info_t info)
{
	/* TODO: read lock? */
	return context->memory_manager->get_function(context->memory_context, info);
}

void *
_jit_memory_get_function_start(jit_context_t context, jit_function_info_t info)
{
	/* TODO: read lock? */
	return context->memory_manager->get_function_start(context->memory_context, info);
}

void *
_jit_memory_get_function_end(jit_context_t context, jit_function_info_t info)
{
	/* TODO: read lock? */
	return context->memory_manager->get_function_end(context->memory_context, info);
}

jit_function_t
_jit_memory_alloc_function(jit_context_t context)
{
	return context->memory_manager->alloc_function(context->memory_context);
}

void
_jit_memory_free_function(jit_context_t context, jit_function_t func)
{
	context->memory_manager->free_function(context->memory_context, func);
}

int
_jit_memory_start_function(jit_context_t context, jit_function_t func)
{
	return context->memory_manager->start_function(context->memory_context, func);
}

int
_jit_memory_end_function(jit_context_t context, int result)
{
	return context->memory_manager->end_function(context->memory_context, result);
}

int
_jit_memory_extend_limit(jit_context_t context, int count)
{
	return context->memory_manager->extend_limit(context->memory_context, count);
}

void *
_jit_memory_get_limit(jit_context_t context)
{
	return context->memory_manager->get_limit(context->memory_context);
}

void *
_jit_memory_get_break(jit_context_t context)
{
	return context->memory_manager->get_break(context->memory_context);
}

void
_jit_memory_set_break(jit_context_t context, void *brk)
{
	context->memory_manager->set_break(context->memory_context, brk);
}

void *
_jit_memory_alloc_trampoline(jit_context_t context)
{
	return context->memory_manager->alloc_trampoline(context->memory_context);
}

void
_jit_memory_free_trampoline(jit_context_t context, void *ptr)
{
	context->memory_manager->free_trampoline(context->memory_context, ptr);
}

void *
_jit_memory_alloc_closure(jit_context_t context)
{
	return context->memory_manager->alloc_closure(context->memory_context);
}

void
_jit_memory_free_closure(jit_context_t context, void *ptr)
{
	context->memory_manager->free_closure(context->memory_context, ptr);
}

void *
_jit_memory_alloc_data(jit_context_t context, jit_size_t size, jit_size_t align)
{
	return context->memory_manager->alloc_data(context->memory_context, size, align);
}
