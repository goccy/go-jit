/*
 * jit-block.h - Functions for manipulating blocks.
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

#ifndef	_JIT_BLOCK_H
#define	_JIT_BLOCK_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

jit_function_t jit_block_get_function(jit_block_t block) JIT_NOTHROW;
jit_context_t jit_block_get_context(jit_block_t block) JIT_NOTHROW;
jit_label_t jit_block_get_label(jit_block_t block) JIT_NOTHROW;
jit_label_t jit_block_get_next_label(jit_block_t block,
				     jit_label_t label) JIT_NOTHROW;
jit_block_t jit_block_next(jit_function_t func,
			   jit_block_t previous) JIT_NOTHROW;
jit_block_t jit_block_previous(jit_function_t func,
			       jit_block_t previous) JIT_NOTHROW;
jit_block_t jit_block_from_label(jit_function_t func,
				 jit_label_t label) JIT_NOTHROW;
int jit_block_set_meta(jit_block_t block, int type, void *data,
		       jit_meta_free_func free_data) JIT_NOTHROW;
void *jit_block_get_meta(jit_block_t block, int type) JIT_NOTHROW;
void jit_block_free_meta(jit_block_t block, int type) JIT_NOTHROW;
int jit_block_is_reachable(jit_block_t block) JIT_NOTHROW;
int jit_block_ends_in_dead(jit_block_t block) JIT_NOTHROW;
int jit_block_current_is_dead(jit_function_t func) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_BLOCK_H */
