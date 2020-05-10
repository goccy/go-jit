/*
 * jit-memory.h - Memory management.
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

#ifndef _JIT_MEMORY_H
#define	_JIT_MEMORY_H

#include <jit/jit-defs.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Result values for "_jit_cache_start_function" and "_jit_cache_end_function".
 */
#define	JIT_MEMORY_OK		0	/* Function is OK */
#define	JIT_MEMORY_RESTART	1	/* Restart is required */
#define	JIT_MEMORY_TOO_BIG	2	/* Function is too big for the cache */
#define JIT_MEMORY_ERROR	3	/* Other error */

typedef void *jit_memory_context_t;
typedef void *jit_function_info_t;

typedef struct jit_memory_manager const* jit_memory_manager_t;

struct jit_memory_manager
{
	jit_memory_context_t (*create)(jit_context_t context);
	void (*destroy)(jit_memory_context_t memctx);

	jit_function_info_t (*find_function_info)(jit_memory_context_t memctx, void *pc);
	jit_function_t (*get_function)(jit_memory_context_t memctx, jit_function_info_t info);
	void * (*get_function_start)(jit_memory_context_t memctx, jit_function_info_t info);
	void * (*get_function_end)(jit_memory_context_t memctx, jit_function_info_t info);

	jit_function_t (*alloc_function)(jit_memory_context_t memctx);
	void (*free_function)(jit_memory_context_t memctx, jit_function_t func);

	int (*start_function)(jit_memory_context_t memctx, jit_function_t func);
	int (*end_function)(jit_memory_context_t memctx, int result);
	int (*extend_limit)(jit_memory_context_t memctx, int count);

	void * (*get_limit)(jit_memory_context_t memctx);
	void * (*get_break)(jit_memory_context_t memctx);
	void (*set_break)(jit_memory_context_t memctx, void *brk);

	void * (*alloc_trampoline)(jit_memory_context_t memctx);
	void (*free_trampoline)(jit_memory_context_t memctx, void *ptr);

	void * (*alloc_closure)(jit_memory_context_t memctx);
	void (*free_closure)(jit_memory_context_t memctx, void *ptr);

	void * (*alloc_data)(jit_memory_context_t memctx, jit_size_t size, jit_size_t align);
};

jit_memory_manager_t jit_default_memory_manager(void) JIT_NOTHROW;

#ifdef	__cplusplus
}
#endif

#endif	/* _JIT_MEMORY_H */
