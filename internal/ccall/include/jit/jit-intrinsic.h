/*
 * jit-intrinsic.h - Support routines for JIT intrinsics.
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

#ifndef	_JIT_INTRINSIC_H
#define	_JIT_INTRINSIC_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Perform operations on signed 32-bit integers.
 */
jit_int jit_int_add(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_sub(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_mul(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_div
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_rem
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_add_ovf
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_sub_ovf
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_mul_ovf
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_div_ovf
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_rem_ovf
	(jit_int *result, jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_neg(jit_int value1) JIT_NOTHROW;
jit_int jit_int_and(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_or(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_xor(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_not(jit_int value1) JIT_NOTHROW;
jit_int jit_int_shl(jit_int value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_int_shr(jit_int value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_int_eq(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_ne(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_lt(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_le(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_gt(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_ge(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_cmp(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_abs(jit_int value1) JIT_NOTHROW;
jit_int jit_int_min(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_max(jit_int value1, jit_int value2) JIT_NOTHROW;
jit_int jit_int_sign(jit_int value1) JIT_NOTHROW;

/*
 * Perform operations on unsigned 32-bit integers.
 */
jit_uint jit_uint_add(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_sub(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_mul(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_div
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_rem
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_add_ovf
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_sub_ovf
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_mul_ovf
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_div_ovf
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_rem_ovf
	(jit_uint *result, jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_neg(jit_uint value1) JIT_NOTHROW;
jit_uint jit_uint_and(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_or(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_xor(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_not(jit_uint value1) JIT_NOTHROW;
jit_uint jit_uint_shl(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_shr(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_eq(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_ne(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_lt(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_le(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_gt(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_ge(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_uint_cmp(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_min(jit_uint value1, jit_uint value2) JIT_NOTHROW;
jit_uint jit_uint_max(jit_uint value1, jit_uint value2) JIT_NOTHROW;

/*
 * Perform operations on signed 64-bit integers.
 */
jit_long jit_long_add(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_sub(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_mul(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_div
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_rem
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_add_ovf
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_sub_ovf
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_mul_ovf
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_div_ovf
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_rem_ovf
	(jit_long *result, jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_neg(jit_long value1) JIT_NOTHROW;
jit_long jit_long_and(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_or(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_xor(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_not(jit_long value1) JIT_NOTHROW;
jit_long jit_long_shl(jit_long value1, jit_uint value2) JIT_NOTHROW;
jit_long jit_long_shr(jit_long value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_long_eq(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_ne(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_lt(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_le(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_gt(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_ge(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_cmp(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_abs(jit_long value1) JIT_NOTHROW;
jit_long jit_long_min(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_long jit_long_max(jit_long value1, jit_long value2) JIT_NOTHROW;
jit_int jit_long_sign(jit_long value1) JIT_NOTHROW;

/*
 * Perform operations on unsigned 64-bit integers.
 */
jit_ulong jit_ulong_add(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_sub(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_mul(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_div
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_rem
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_add_ovf
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_sub_ovf
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_mul_ovf
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_div_ovf
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_rem_ovf
	(jit_ulong *result, jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_neg(jit_ulong value1) JIT_NOTHROW;
jit_ulong jit_ulong_and(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_or(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_xor(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_not(jit_ulong value1) JIT_NOTHROW;
jit_ulong jit_ulong_shl(jit_ulong value1, jit_uint value2) JIT_NOTHROW;
jit_ulong jit_ulong_shr(jit_ulong value1, jit_uint value2) JIT_NOTHROW;
jit_int jit_ulong_eq(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_ne(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_lt(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_le(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_gt(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_ge(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_int jit_ulong_cmp(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_min(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;
jit_ulong jit_ulong_max(jit_ulong value1, jit_ulong value2) JIT_NOTHROW;

/*
 * Perform operations on 32-bit floating-point values.
 */
jit_float32 jit_float32_add
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_sub
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_mul
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_div
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_rem
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_ieee_rem
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_neg(jit_float32 value1) JIT_NOTHROW;
jit_int jit_float32_eq(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_ne(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_lt(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_le(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_gt(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_ge(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_cmpl(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_cmpg(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_acos(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_asin(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_atan(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_atan2
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_ceil(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_cos(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_cosh(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_exp(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_floor(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_log(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_log10(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_pow
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_rint(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_round(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_sin(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_sinh(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_sqrt(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_tan(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_tanh(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_trunc(jit_float32 value1) JIT_NOTHROW;
jit_int jit_float32_is_finite(jit_float32 value) JIT_NOTHROW;
jit_int jit_float32_is_nan(jit_float32 value) JIT_NOTHROW;
jit_int jit_float32_is_inf(jit_float32 value) JIT_NOTHROW;
jit_float32 jit_float32_abs(jit_float32 value1) JIT_NOTHROW;
jit_float32 jit_float32_min
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_float32 jit_float32_max
	(jit_float32 value1, jit_float32 value2) JIT_NOTHROW;
jit_int jit_float32_sign(jit_float32 value1) JIT_NOTHROW;

/*
 * Perform operations on 64-bit floating-point values.
 */
jit_float64 jit_float64_add
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_sub
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_mul
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_div
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_rem
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_ieee_rem
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_neg(jit_float64 value1) JIT_NOTHROW;
jit_int jit_float64_eq(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_ne(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_lt(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_le(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_gt(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_ge(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_cmpl(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_cmpg(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_acos(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_asin(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_atan(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_atan2
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_ceil(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_cos(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_cosh(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_exp(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_floor(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_log(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_log10(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_pow
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_rint(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_round(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_sin(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_sinh(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_sqrt(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_tan(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_tanh(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_trunc(jit_float64 value1) JIT_NOTHROW;
jit_int jit_float64_is_finite(jit_float64 value) JIT_NOTHROW;
jit_int jit_float64_is_nan(jit_float64 value) JIT_NOTHROW;
jit_int jit_float64_is_inf(jit_float64 value) JIT_NOTHROW;
jit_float64 jit_float64_abs(jit_float64 value1) JIT_NOTHROW;
jit_float64 jit_float64_min
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_float64 jit_float64_max
	(jit_float64 value1, jit_float64 value2) JIT_NOTHROW;
jit_int jit_float64_sign(jit_float64 value1) JIT_NOTHROW;

/*
 * Perform operations on native floating-point values.
 */
jit_nfloat jit_nfloat_add(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_sub(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_mul(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_div(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_rem(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_ieee_rem
	(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_neg(jit_nfloat value1) JIT_NOTHROW;
jit_int jit_nfloat_eq(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_ne(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_lt(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_le(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_gt(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_ge(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_cmpl(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_cmpg(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_acos(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_asin(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_atan(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_atan2(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_ceil(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_cos(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_cosh(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_exp(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_floor(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_log(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_log10(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_pow(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_rint(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_round(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_sin(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_sinh(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_sqrt(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_tan(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_tanh(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_trunc(jit_nfloat value1) JIT_NOTHROW;
jit_int jit_nfloat_is_finite(jit_nfloat value) JIT_NOTHROW;
jit_int jit_nfloat_is_nan(jit_nfloat value) JIT_NOTHROW;
jit_int jit_nfloat_is_inf(jit_nfloat value) JIT_NOTHROW;
jit_nfloat jit_nfloat_abs(jit_nfloat value1) JIT_NOTHROW;
jit_nfloat jit_nfloat_min(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_nfloat jit_nfloat_max(jit_nfloat value1, jit_nfloat value2) JIT_NOTHROW;
jit_int jit_nfloat_sign(jit_nfloat value1) JIT_NOTHROW;

/*
 * Convert between integer types.
 */
jit_int jit_int_to_sbyte(jit_int value) JIT_NOTHROW;
jit_int jit_int_to_ubyte(jit_int value) JIT_NOTHROW;
jit_int jit_int_to_short(jit_int value) JIT_NOTHROW;
jit_int jit_int_to_ushort(jit_int value) JIT_NOTHROW;
jit_int jit_int_to_int(jit_int value) JIT_NOTHROW;
jit_uint jit_int_to_uint(jit_int value) JIT_NOTHROW;
jit_long jit_int_to_long(jit_int value) JIT_NOTHROW;
jit_ulong jit_int_to_ulong(jit_int value) JIT_NOTHROW;
jit_int jit_uint_to_int(jit_uint value) JIT_NOTHROW;
jit_uint jit_uint_to_uint(jit_uint value) JIT_NOTHROW;
jit_long jit_uint_to_long(jit_uint value) JIT_NOTHROW;
jit_ulong jit_uint_to_ulong(jit_uint value) JIT_NOTHROW;
jit_int jit_long_to_int(jit_long value) JIT_NOTHROW;
jit_uint jit_long_to_uint(jit_long value) JIT_NOTHROW;
jit_long jit_long_to_long(jit_long value) JIT_NOTHROW;
jit_ulong jit_long_to_ulong(jit_long value) JIT_NOTHROW;
jit_int jit_ulong_to_int(jit_ulong value) JIT_NOTHROW;
jit_uint jit_ulong_to_uint(jit_ulong value) JIT_NOTHROW;
jit_long jit_ulong_to_long(jit_ulong value) JIT_NOTHROW;
jit_ulong jit_ulong_to_ulong(jit_ulong value) JIT_NOTHROW;

/*
 * Convert between integer types with overflow detection.
 */
jit_int jit_int_to_sbyte_ovf(jit_int *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_ubyte_ovf(jit_int *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_short_ovf(jit_int *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_ushort_ovf(jit_int *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_int_ovf(jit_int *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_uint_ovf(jit_uint *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_long_ovf(jit_long *result, jit_int value) JIT_NOTHROW;
jit_int jit_int_to_ulong_ovf(jit_ulong *result, jit_int value) JIT_NOTHROW;
jit_int jit_uint_to_int_ovf(jit_int *result, jit_uint value) JIT_NOTHROW;
jit_int jit_uint_to_uint_ovf(jit_uint *result, jit_uint value) JIT_NOTHROW;
jit_int jit_uint_to_long_ovf(jit_long *result, jit_uint value) JIT_NOTHROW;
jit_int jit_uint_to_ulong_ovf(jit_ulong *result, jit_uint value) JIT_NOTHROW;
jit_int jit_long_to_int_ovf(jit_int *result, jit_long value) JIT_NOTHROW;
jit_int jit_long_to_uint_ovf(jit_uint *result, jit_long value) JIT_NOTHROW;
jit_int jit_long_to_long_ovf(jit_long *result, jit_long value) JIT_NOTHROW;
jit_int jit_long_to_ulong_ovf(jit_ulong *result, jit_long value) JIT_NOTHROW;
jit_int jit_ulong_to_int_ovf(jit_int *result, jit_ulong value) JIT_NOTHROW;
jit_int jit_ulong_to_uint_ovf(jit_uint *result, jit_ulong value) JIT_NOTHROW;
jit_int jit_ulong_to_long_ovf(jit_long *result, jit_ulong value) JIT_NOTHROW;
jit_int jit_ulong_to_ulong_ovf(jit_ulong *result, jit_ulong value) JIT_NOTHROW;

/*
 * Convert a 32-bit floating-point value into various integer types.
 */
jit_int jit_float32_to_int(jit_float32 value) JIT_NOTHROW;
jit_uint jit_float32_to_uint(jit_float32 value) JIT_NOTHROW;
jit_long jit_float32_to_long(jit_float32 value) JIT_NOTHROW;
jit_ulong jit_float32_to_ulong(jit_float32 value) JIT_NOTHROW;

/*
 * Convert a 32-bit floating-point value into various integer types,
 * with overflow detection.
 */
jit_int jit_float32_to_int_ovf(jit_int *result, jit_float32 value) JIT_NOTHROW;
jit_int jit_float32_to_uint_ovf(jit_uint *result, jit_float32 value) JIT_NOTHROW;
jit_int jit_float32_to_long_ovf(jit_long *result, jit_float32 value) JIT_NOTHROW;
jit_int jit_float32_to_ulong_ovf
	(jit_ulong *result, jit_float32 value) JIT_NOTHROW;

/*
 * Convert a 64-bit floating-point value into various integer types.
 */
jit_int jit_float64_to_int(jit_float64 value) JIT_NOTHROW;
jit_uint jit_float64_to_uint(jit_float64 value) JIT_NOTHROW;
jit_long jit_float64_to_long(jit_float64 value) JIT_NOTHROW;
jit_ulong jit_float64_to_ulong(jit_float64 value) JIT_NOTHROW;

/*
 * Convert a 64-bit floating-point value into various integer types,
 * with overflow detection.
 */
jit_int jit_float64_to_int_ovf(jit_int *result, jit_float64 value) JIT_NOTHROW;
jit_int jit_float64_to_uint_ovf(jit_uint *result, jit_float64 value) JIT_NOTHROW;
jit_int jit_float64_to_long_ovf(jit_long *result, jit_float64 value) JIT_NOTHROW;
jit_int jit_float64_to_ulong_ovf
	(jit_ulong *result, jit_float64 value) JIT_NOTHROW;

/*
 * Convert a native floating-point value into various integer types.
 */
jit_int jit_nfloat_to_int(jit_nfloat value) JIT_NOTHROW;
jit_uint jit_nfloat_to_uint(jit_nfloat value) JIT_NOTHROW;
jit_long jit_nfloat_to_long(jit_nfloat value) JIT_NOTHROW;
jit_ulong jit_nfloat_to_ulong(jit_nfloat value) JIT_NOTHROW;

/*
 * Convert a native floating-point value into various integer types,
 * with overflow detection.
 */
jit_int jit_nfloat_to_int_ovf(jit_int *result, jit_nfloat value) JIT_NOTHROW;
jit_int jit_nfloat_to_uint_ovf(jit_uint *result, jit_nfloat value) JIT_NOTHROW;
jit_int jit_nfloat_to_long_ovf(jit_long *result, jit_nfloat value) JIT_NOTHROW;
jit_int jit_nfloat_to_ulong_ovf
	(jit_ulong *result, jit_nfloat value) JIT_NOTHROW;

/*
 * Convert integer types into floating-point values.
 */
jit_float32 jit_int_to_float32(jit_int value) JIT_NOTHROW;
jit_float64 jit_int_to_float64(jit_int value) JIT_NOTHROW;
jit_nfloat jit_int_to_nfloat(jit_int value) JIT_NOTHROW;
jit_float32 jit_uint_to_float32(jit_uint value) JIT_NOTHROW;
jit_float64 jit_uint_to_float64(jit_uint value) JIT_NOTHROW;
jit_nfloat jit_uint_to_nfloat(jit_uint value) JIT_NOTHROW;
jit_float32 jit_long_to_float32(jit_long value) JIT_NOTHROW;
jit_float64 jit_long_to_float64(jit_long value) JIT_NOTHROW;
jit_nfloat jit_long_to_nfloat(jit_long value) JIT_NOTHROW;
jit_float32 jit_ulong_to_float32(jit_ulong value) JIT_NOTHROW;
jit_float64 jit_ulong_to_float64(jit_ulong value) JIT_NOTHROW;
jit_nfloat jit_ulong_to_nfloat(jit_ulong value) JIT_NOTHROW;

/*
 * Convert between floating-point types.
 */
jit_float64 jit_float32_to_float64(jit_float32 value) JIT_NOTHROW;
jit_nfloat jit_float32_to_nfloat(jit_float32 value) JIT_NOTHROW;
jit_float32 jit_float64_to_float32(jit_float64 value) JIT_NOTHROW;
jit_nfloat jit_float64_to_nfloat(jit_float64 value) JIT_NOTHROW;
jit_float32 jit_nfloat_to_float32(jit_nfloat value) JIT_NOTHROW;
jit_float64 jit_nfloat_to_float64(jit_nfloat value) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_INTRINSIC_H */
