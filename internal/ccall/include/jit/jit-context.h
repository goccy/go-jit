/*
 * jit-context.h - Functions for manipulating JIT contexts.
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

#ifndef	_JIT_CONTEXT_H
#define	_JIT_CONTEXT_H

#include <jit/jit-common.h>
#include <jit/jit-memory.h>

#ifdef	__cplusplus
extern	"C" {
#endif

jit_context_t jit_context_create(void) JIT_NOTHROW;
void jit_context_destroy(jit_context_t context) JIT_NOTHROW;

void jit_context_build_start(jit_context_t context) JIT_NOTHROW;
void jit_context_build_end(jit_context_t context) JIT_NOTHROW;

void jit_context_set_on_demand_driver(
	jit_context_t context,
	jit_on_demand_driver_func driver) JIT_NOTHROW;

void jit_context_set_memory_manager(
	jit_context_t context,
	jit_memory_manager_t manager) JIT_NOTHROW;

int jit_context_set_meta
	(jit_context_t context, int type, void *data,
	 jit_meta_free_func free_data) JIT_NOTHROW;
int jit_context_set_meta_numeric
	(jit_context_t context, int type, jit_nuint data) JIT_NOTHROW;
void *jit_context_get_meta(jit_context_t context, int type) JIT_NOTHROW;
jit_nuint jit_context_get_meta_numeric
	(jit_context_t context, int type) JIT_NOTHROW;
void jit_context_free_meta(jit_context_t context, int type) JIT_NOTHROW;

/*
 * Standard meta values for builtin configurable options.
 */
#define	JIT_OPTION_CACHE_LIMIT		10000
#define	JIT_OPTION_CACHE_PAGE_SIZE	10001
#define	JIT_OPTION_PRE_COMPILE		10002
#define	JIT_OPTION_DONT_FOLD		10003
#define JIT_OPTION_POSITION_INDEPENDENT	10004
#define JIT_OPTION_CACHE_MAX_PAGE_FACTOR	10005

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_CONTEXT_H */
