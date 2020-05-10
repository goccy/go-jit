/*
 * jit-value.h - Functions for manipulating temporary values.
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

#ifndef	_JIT_VALUE_H
#define	_JIT_VALUE_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Full struction that can hold a constant of any type.
 */
typedef struct
{
	jit_type_t			type;
	union
	{
		void		   *ptr_value;
		jit_int			int_value;
		jit_uint		uint_value;
		jit_nint		nint_value;
		jit_nuint		nuint_value;
		jit_long		long_value;
		jit_ulong		ulong_value;
		jit_float32		float32_value;
		jit_float64		float64_value;
		jit_nfloat		nfloat_value;

	} un;

} jit_constant_t;

/*
 * External function declarations.
 */
jit_value_t jit_value_create(jit_function_t func, jit_type_t type) JIT_NOTHROW;
jit_value_t jit_value_create_nint_constant
	(jit_function_t func, jit_type_t type, jit_nint const_value) JIT_NOTHROW;
jit_value_t jit_value_create_long_constant
	(jit_function_t func, jit_type_t type, jit_long const_value) JIT_NOTHROW;
jit_value_t jit_value_create_float32_constant
	(jit_function_t func, jit_type_t type,
	 jit_float32 const_value) JIT_NOTHROW;
jit_value_t jit_value_create_float64_constant
	(jit_function_t func, jit_type_t type,
	 jit_float64 const_value) JIT_NOTHROW;
jit_value_t jit_value_create_nfloat_constant
	(jit_function_t func, jit_type_t type,
	 jit_nfloat const_value) JIT_NOTHROW;
jit_value_t jit_value_create_constant
	(jit_function_t func, const jit_constant_t *const_value) JIT_NOTHROW;
jit_value_t jit_value_get_param
	(jit_function_t func, unsigned int param) JIT_NOTHROW;
jit_value_t jit_value_get_struct_pointer(jit_function_t func) JIT_NOTHROW;
int jit_value_is_temporary(jit_value_t value) JIT_NOTHROW;
int jit_value_is_local(jit_value_t value) JIT_NOTHROW;
int jit_value_is_constant(jit_value_t value) JIT_NOTHROW;
int jit_value_is_parameter(jit_value_t value) JIT_NOTHROW;
void jit_value_ref(jit_function_t func, jit_value_t value) JIT_NOTHROW;
void jit_value_set_volatile(jit_value_t value) JIT_NOTHROW;
int jit_value_is_volatile(jit_value_t value) JIT_NOTHROW;
void jit_value_set_addressable(jit_value_t value) JIT_NOTHROW;
int jit_value_is_addressable(jit_value_t value) JIT_NOTHROW;
jit_type_t jit_value_get_type(jit_value_t value) JIT_NOTHROW;
jit_function_t jit_value_get_function(jit_value_t value) JIT_NOTHROW;
jit_block_t jit_value_get_block(jit_value_t value) JIT_NOTHROW;
jit_context_t jit_value_get_context(jit_value_t value) JIT_NOTHROW;
jit_constant_t jit_value_get_constant(jit_value_t value) JIT_NOTHROW;
jit_nint jit_value_get_nint_constant(jit_value_t value) JIT_NOTHROW;
jit_long jit_value_get_long_constant(jit_value_t value) JIT_NOTHROW;
jit_float32 jit_value_get_float32_constant(jit_value_t value) JIT_NOTHROW;
jit_float64 jit_value_get_float64_constant(jit_value_t value) JIT_NOTHROW;
jit_nfloat jit_value_get_nfloat_constant(jit_value_t value) JIT_NOTHROW;
int jit_value_is_true(jit_value_t value) JIT_NOTHROW;
int jit_constant_convert
	(jit_constant_t *result, const jit_constant_t *value,
	 jit_type_t type, int overflow_check) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_VALUE_H */
