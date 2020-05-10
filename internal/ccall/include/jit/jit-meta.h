/*
 * jit-meta.h - Manipulate lists of metadata values.
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

#ifndef	_JIT_META_H
#define	_JIT_META_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

typedef struct _jit_meta *jit_meta_t;

int jit_meta_set
	(jit_meta_t *list, int type, void *data,
	 jit_meta_free_func free_data, jit_function_t pool_owner) JIT_NOTHROW;
void *jit_meta_get(jit_meta_t list, int type) JIT_NOTHROW;
void jit_meta_free(jit_meta_t *list, int type) JIT_NOTHROW;
void jit_meta_destroy(jit_meta_t *list) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_META_H */
