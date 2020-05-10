/*
 * jit-apply.h - Dynamic invocation and closure support functions.
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

#ifndef	_JIT_APPLY_H
#define	_JIT_APPLY_H

#include <jit/jit-type.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Prototype for closure functions.
 */
typedef void (*jit_closure_func)(jit_type_t signature, void *result,
                                 void **args, void *user_data);

/*
 * Opaque type for accessing vararg parameters on closures.
 */
typedef struct jit_closure_va_list *jit_closure_va_list_t;

/*
 * External function declarations.
 */
void jit_apply(jit_type_t signature, void *func,
               void **args, unsigned int num_fixed_args,
               void *return_value);
void jit_apply_raw(jit_type_t signature, void *func,
                   void *args, void *return_value);
int jit_raw_supported(jit_type_t signature);

void *jit_closure_create(jit_context_t context, jit_type_t signature,
			 jit_closure_func func, void *user_data);

jit_nint jit_closure_va_get_nint(jit_closure_va_list_t va);
jit_nuint jit_closure_va_get_nuint(jit_closure_va_list_t va);
jit_long jit_closure_va_get_long(jit_closure_va_list_t va);
jit_ulong jit_closure_va_get_ulong(jit_closure_va_list_t va);
jit_float32 jit_closure_va_get_float32(jit_closure_va_list_t va);
jit_float64 jit_closure_va_get_float64(jit_closure_va_list_t va);
jit_nfloat jit_closure_va_get_nfloat(jit_closure_va_list_t va);
void *jit_closure_va_get_ptr(jit_closure_va_list_t va);
void jit_closure_va_get_struct(jit_closure_va_list_t va, void *buf, jit_type_t type);

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_APPLY_H */
