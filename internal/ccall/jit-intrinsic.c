/*
 * jit-intrinsic.c - Support routines for JIT intrinsics.
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
#if defined(HAVE_TGMATH_H) && !defined(JIT_NFLOAT_IS_DOUBLE)
	#include <tgmath.h>
#elif defined(HAVE_MATH_H)
	#include <math.h>
#endif
#ifdef JIT_WIN32_PLATFORM
#include <float.h>
#if !defined(isnan)
#define isnan(value)	_isnan((value))
#endif
#if !defined(isfinite)
#define isfinite(value)	_finite((value))
#endif
#ifndef HAVE_ISNAN
#define HAVE_ISNAN 1
#endif
#undef	HAVE_ISNANF
#undef	HAVE_ISNANL
#else
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
#endif

/*@

@cindex jit-intrinsic.h

Intrinsics are functions that are provided to ease code generation
on platforms that may not be able to perform all operations natively.

For example, on a CPU without a floating-point unit, the back end code
generator will output a call to an intrinsic function when a floating-point
operation is performed.  CPU's with a floating-point unit would use
a native instruction instead.

Some platforms may already have appropriate intrinsics (e.g. the ARM
floating-point emulation routines).  The back end code generator may choose
to use either the system-supplied intrinsics or the ones supplied by
this library.  We supply all of them in our library just in case a
particular platform lacks an appropriate intrinsic.

Some intrinsics have no equivalent in existing system libraries;
particularly those that deal with overflow checking.

Functions that perform overflow checking or which divide integer operands
return a built-in exception code to indicate the type of exception
to be thrown (the caller is responsible for throwing the actual
exception).  @xref{Exceptions}, for a list of built-in exception codes.

The following functions are defined in @code{<jit/jit-intrinsic.h>}:

@*/

/*
 * Special values that indicate "not a number" for floating-point types.
 * Visual C++ won't let us compute NaN's at compile time, so we have to
 * work around it with a run-time hack.
 */
#if defined(_MSC_VER) && defined(_M_IX86)
static unsigned int const float32_nan = 0x7FC00000;
static unsigned __int64 const float64_nan = 0x7FF8000000000000LL;
#define	jit_float32_nan	(*((jit_float32 *)&float32_nan))
#define	jit_float64_nan	(*((jit_float64 *)&float64_nan))
#define	jit_nfloat_nan	((jit_nfloat)(*((jit_float64 *)&float64_nan)))
#else
#define	jit_float32_nan	((jit_float32)(0.0 / 0.0))
#define	jit_float64_nan	((jit_float64)(0.0 / 0.0))
#define	jit_nfloat_nan	((jit_nfloat)(0.0 / 0.0))
#endif

/*@
 * @deftypefun jit_int jit_int_add (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_sub (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_mul (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_neg (jit_int @var{value1})
 * @deftypefunx jit_int jit_int_and (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_or (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_xor (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_not (jit_int @var{value1})
 * @deftypefunx jit_int jit_int_not (jit_int @var{value1})
 * @deftypefunx jit_int jit_int_shl (jit_int @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_int_shr (jit_int @var{value1}, jit_uint @var{value2})
 * Perform an arithmetic operation on signed 32-bit integers.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_int_add_ovf (jit_int *@var{result}, jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_sub_ovf (jit_int *@var{result}, jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_mul_ovf (jit_int *@var{result}, jit_int @var{value1}, jit_int @var{value2})
 * Perform an arithmetic operation on two signed 32-bit integers,
 * with overflow checking.  Returns @code{JIT_RESULT_OK}
 * or @code{JIT_RESULT_OVERFLOW}.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_int_div_ovf (jit_int *@var{result}, jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_rem_ovf (jit_int *@var{result}, jit_int @var{value1}, jit_int @var{value2})
 * Perform a division or remainder operation on two signed 32-bit integers.
 * Returns @code{JIT_RESULT_OK}, @code{JIT_RESULT_DIVISION_BY_ZERO},
 * or @code{JIT_RESULT_ARITHMETIC}.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_int_eq (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_ne (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_lt (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_le (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_gt (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_ge (jit_int @var{value1}, jit_int @var{value2})
 * Compare two signed 32-bit integers, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_int_cmp (jit_int @var{value1}, jit_int @var{value2})
 * Compare two signed 32-bit integers and return -1, 0, or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_int_abs (jit_int @var{value1})
 * @deftypefunx jit_int jit_int_min (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_max (jit_int @var{value1}, jit_int @var{value2})
 * @deftypefunx jit_int jit_int_sign (jit_int @var{value1})
 * Calculate the absolute value, minimum, maximum, or sign for
 * signed 32-bit integer values.
 * @end deftypefun
@*/
jit_int jit_int_add(jit_int value1, jit_int value2)
{
	return value1 + value2;
}

jit_int jit_int_sub(jit_int value1, jit_int value2)
{
	return value1 - value2;
}

jit_int jit_int_mul(jit_int value1, jit_int value2)
{
	return value1 * value2;
}

jit_int jit_int_div(jit_int *result, jit_int value1, jit_int value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else if(value2 == (jit_int)(-1) && value1 == jit_min_int)
	{
		*result = 0;
		return JIT_RESULT_ARITHMETIC;
	}
	else
	{
		*result = value1 / value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_int_rem(jit_int *result, jit_int value1, jit_int value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else if(value2 == (jit_int)(-1) && value1 == jit_min_int)
	{
		*result = 0;
		return JIT_RESULT_ARITHMETIC;
	}
	else
	{
		*result = value1 % value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_int_add_ovf(jit_int *result, jit_int value1, jit_int value2)
{
	if(value1 >= 0 && value2 >= 0)
	{
		return ((*result = value1 + value2) >= value1);
	}
	else if(value1 < 0 && value2 < 0)
	{
		return ((*result = value1 + value2) < value1);
	}
	else
	{
		*result = value1 + value2;
		return 1;
	}
}

jit_int jit_int_sub_ovf(jit_int *result, jit_int value1, jit_int value2)
{
	if(value1 >= 0 && value2 >= 0)
	{
		*result = value1 - value2;
		return 1;
	}
	else if(value1 < 0 && value2 < 0)
	{
		*result = value1 - value2;
		return 1;
	}
	else if(value1 < 0)
	{
		return ((*result = value1 - value2) <= value1);
	}
	else
	{
		return ((*result = value1 - value2) >= value1);
	}
}

jit_int jit_int_mul_ovf(jit_int *result, jit_int value1, jit_int value2)
{
	jit_long temp = ((jit_long)value1) * ((jit_long)value2);
	*result = (jit_int)temp;
	return (temp >= (jit_long)jit_min_int && temp <= (jit_long)jit_max_int);
}

jit_int jit_int_neg(jit_int value1)
{
	return -value1;
}

jit_int jit_int_and(jit_int value1, jit_int value2)
{
	return value1 & value2;
}

jit_int jit_int_or(jit_int value1, jit_int value2)
{
	return value1 | value2;
}

jit_int jit_int_xor(jit_int value1, jit_int value2)
{
	return value1 ^ value2;
}

jit_int jit_int_not(jit_int value1)
{
	return ~value1;
}

jit_int jit_int_shl(jit_int value1, jit_uint value2)
{
	return value1 << (value2 & 0x1F);
}

jit_int jit_int_shr(jit_int value1, jit_uint value2)
{
	return value1 >> (value2 & 0x1F);
}

jit_int jit_int_eq(jit_int value1, jit_int value2)
{
	return (value1 == value2);
}

jit_int jit_int_ne(jit_int value1, jit_int value2)
{
	return (value1 != value2);
}

jit_int jit_int_lt(jit_int value1, jit_int value2)
{
	return (value1 < value2);
}

jit_int jit_int_le(jit_int value1, jit_int value2)
{
	return (value1 <= value2);
}

jit_int jit_int_gt(jit_int value1, jit_int value2)
{
	return (value1 > value2);
}

jit_int jit_int_ge(jit_int value1, jit_int value2)
{
	return (value1 >= value2);
}

jit_int jit_int_cmp(jit_int value1, jit_int value2)
{
	if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_int jit_int_abs(jit_int value1)
{
	return ((value1 >= 0) ? value1 : -value1);
}

jit_int jit_int_min(jit_int value1, jit_int value2)
{
	return ((value1 <= value2) ? value1 : value2);
}

jit_int jit_int_max(jit_int value1, jit_int value2)
{
	return ((value1 >= value2) ? value1 : value2);
}

jit_int jit_int_sign(jit_int value1)
{
	if(value1 < 0)
	{
		return -1;
	}
	else if(value1 > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_uint jit_uint_add (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_sub (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_mul (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_neg (jit_uint @var{value1})
 * @deftypefunx jit_uint jit_uint_and (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_or (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_xor (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_not (jit_uint @var{value1})
 * @deftypefunx jit_uint jit_uint_not (jit_uint @var{value1})
 * @deftypefunx jit_uint jit_uint_shl (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_shr (jit_uint @var{value1}, jit_uint @var{value2})
 * Perform an arithmetic operation on unsigned 32-bit integers.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_uint_add_ovf (jit_uint *@var{result}, jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_sub_ovf (jit_uint *@var{result}, jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_mul_ovf (jit_uint *@var{result}, jit_uint @var{value1}, jit_uint @var{value2})
 * Perform an arithmetic operation on two unsigned 32-bit integers,
 * with overflow checking.  Returns @code{JIT_RESULT_OK}
 * or @code{JIT_RESULT_OVERFLOW}.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_uint_div_ovf (jit_uint *@var{result}, jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_rem_ovf (jit_uint *@var{result}, jit_uint @var{value1}, jit_uint @var{value2})
 * Perform a division or remainder operation on two unsigned 32-bit integers.
 * Returns @code{JIT_RESULT_OK} or @code{JIT_RESULT_DIVISION_BY_ZERO}
 * (@code{JIT_RESULT_ARITHMETIC} is not possible with unsigned integers).
 * @end deftypefun
 *
 * @deftypefun jit_int jit_uint_eq (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_ne (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_lt (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_le (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_gt (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_int jit_uint_ge (jit_uint @var{value1}, jit_uint @var{value2})
 * Compare two unsigned 32-bit integers, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_uint_cmp (jit_uint @var{value1}, jit_uint @var{value2})
 * Compare two unsigned 32-bit integers and return -1, 0, or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_uint jit_uint_min (jit_uint @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_uint jit_uint_max (jit_uint @var{value1}, jit_uint @var{value2})
 * Calculate the minimum or maximum for unsigned 32-bit integer values.
 * @end deftypefun
@*/
jit_uint jit_uint_add(jit_uint value1, jit_uint value2)
{
	return value1 + value2;
}

jit_uint jit_uint_sub(jit_uint value1, jit_uint value2)
{
	return value1 - value2;
}

jit_uint jit_uint_mul(jit_uint value1, jit_uint value2)
{
	return value1 * value2;
}

jit_int jit_uint_div(jit_uint *result, jit_uint value1, jit_uint value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else
	{
		*result = value1 / value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_uint_rem(jit_uint *result, jit_uint value1, jit_uint value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else
	{
		*result = value1 % value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_uint_add_ovf(jit_uint *result, jit_uint value1, jit_uint value2)
{
	return ((*result = value1 + value2) >= value1);
}

jit_int jit_uint_sub_ovf(jit_uint *result, jit_uint value1, jit_uint value2)
{
	return ((*result = value1 - value2) <= value1);
}

jit_int jit_uint_mul_ovf(jit_uint *result, jit_uint value1, jit_uint value2)
{
	jit_ulong temp = ((jit_ulong)value1) * ((jit_ulong)value2);
	*result = (jit_uint)temp;
	return (temp <= (jit_ulong)jit_max_uint);
}

jit_uint jit_uint_neg(jit_uint value1)
{
	return (jit_uint)(-((jit_int)value1));
}

jit_uint jit_uint_and(jit_uint value1, jit_uint value2)
{
	return value1 & value2;
}

jit_uint jit_uint_or(jit_uint value1, jit_uint value2)
{
	return value1 | value2;
}

jit_uint jit_uint_xor(jit_uint value1, jit_uint value2)
{
	return value1 ^ value2;
}

jit_uint jit_uint_not(jit_uint value1)
{
	return ~value1;
}

jit_uint jit_uint_shl(jit_uint value1, jit_uint value2)
{
	return value1 << (value2 & 0x1F);
}

jit_uint jit_uint_shr(jit_uint value1, jit_uint value2)
{
	return value1 >> (value2 & 0x1F);
}

jit_int jit_uint_eq(jit_uint value1, jit_uint value2)
{
	return (value1 == value2);
}

jit_int jit_uint_ne(jit_uint value1, jit_uint value2)
{
	return (value1 != value2);
}

jit_int jit_uint_lt(jit_uint value1, jit_uint value2)
{
	return (value1 < value2);
}

jit_int jit_uint_le(jit_uint value1, jit_uint value2)
{
	return (value1 <= value2);
}

jit_int jit_uint_gt(jit_uint value1, jit_uint value2)
{
	return (value1 > value2);
}

jit_int jit_uint_ge(jit_uint value1, jit_uint value2)
{
	return (value1 >= value2);
}

jit_int jit_uint_cmp(jit_uint value1, jit_uint value2)
{
	if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_uint jit_uint_min(jit_uint value1, jit_uint value2)
{
	return ((value1 <= value2) ? value1 : value2);
}

jit_uint jit_uint_max(jit_uint value1, jit_uint value2)
{
	return ((value1 >= value2) ? value1 : value2);
}

/*@
 * @deftypefun jit_long jit_long_add (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_sub (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_mul (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_neg (jit_long @var{value1})
 * @deftypefunx jit_long jit_long_and (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_or (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_xor (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_not (jit_long @var{value1})
 * @deftypefunx jit_long jit_long_not (jit_long @var{value1})
 * @deftypefunx jit_long jit_long_shl (jit_long @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_long jit_long_shr (jit_long @var{value1}, jit_uint @var{value2})
 * Perform an arithmetic operation on signed 64-bit integers.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_long_add_ovf (jit_long *@var{result}, jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_sub_ovf (jit_long *@var{result}, jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_mul_ovf (jit_long *@var{result}, jit_long @var{value1}, jit_long @var{value2})
 * Perform an arithmetic operation on two signed 64-bit integers,
 * with overflow checking.  Returns @code{JIT_RESULT_OK}
 * or @code{JIT_RESULT_OVERFLOW}.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_long_div_ovf (jit_long *@var{result}, jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_rem_ovf (jit_long *@var{result}, jit_long @var{value1}, jit_long @var{value2})
 * Perform a division or remainder operation on two signed 64-bit integers.
 * Returns @code{JIT_RESULT_OK}, @code{JIT_RESULT_DIVISION_BY_ZERO},
 * or @code{JIT_RESULT_ARITHMETIC}.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_long_eq (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_ne (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_lt (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_le (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_gt (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_ge (jit_long @var{value1}, jit_long @var{value2})
 * Compare two signed 64-bit integers, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_long_cmp (jit_long @var{value1}, jit_long @var{value2})
 * Compare two signed 64-bit integers and return -1, 0, or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_long jit_long_abs (jit_long @var{value1})
 * @deftypefunx jit_long jit_long_min (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_long jit_long_max (jit_long @var{value1}, jit_long @var{value2})
 * @deftypefunx jit_int jit_long_sign (jit_long @var{value1})
 * Calculate the absolute value, minimum, maximum, or sign for
 * signed 64-bit integer values.
 * @end deftypefun
@*/
jit_long jit_long_add(jit_long value1, jit_long value2)
{
	return value1 + value2;
}

jit_long jit_long_sub(jit_long value1, jit_long value2)
{
	return value1 - value2;
}

jit_long jit_long_mul(jit_long value1, jit_long value2)
{
	return value1 * value2;
}

jit_int jit_long_div(jit_long *result, jit_long value1, jit_long value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else if(value2 == (jit_long)(-1) && value1 == jit_min_long)
	{
		*result = 0;
		return JIT_RESULT_ARITHMETIC;
	}
	else
	{
		*result = value1 / value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_long_rem(jit_long *result, jit_long value1, jit_long value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else if(value2 == (jit_long)(-1) && value1 == jit_min_long)
	{
		*result = 0;
		return JIT_RESULT_ARITHMETIC;
	}
	else
	{
		*result = value1 % value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_long_add_ovf(jit_long *result, jit_long value1, jit_long value2)
{
	if(value1 >= 0 && value2 >= 0)
	{
		return ((*result = value1 + value2) >= value1);
	}
	else if(value1 < 0 && value2 < 0)
	{
		return ((*result = value1 + value2) < value1);
	}
	else
	{
		*result = value1 + value2;
		return 1;
	}
}

jit_int jit_long_sub_ovf(jit_long *result, jit_long value1, jit_long value2)
{
	if(value1 >= 0 && value2 >= 0)
	{
		*result = value1 - value2;
		return 1;
	}
	else if(value1 < 0 && value2 < 0)
	{
		*result = value1 - value2;
		return 1;
	}
	else if(value1 < 0)
	{
		return ((*result = value1 - value2) <= value1);
	}
	else
	{
		return ((*result = value1 - value2) >= value1);
	}
}

jit_int jit_long_mul_ovf(jit_long *result, jit_long value1, jit_long value2)
{
	jit_ulong temp;
	if(value1 >= 0 && value2 >= 0)
	{
		/* Both values are positive */
		if(!jit_ulong_mul_ovf(&temp, (jit_ulong)value1, (jit_ulong)value2))
		{
			*result = jit_max_long;
			return 0;
		}
		if(temp > ((jit_ulong)jit_max_long))
		{
			*result = jit_max_long;
			return 0;
		}
		*result = (jit_long)temp;
		return 1;
	}
	else if(value1 >= 0)
	{
		/* The first value is positive */
		if(!jit_ulong_mul_ovf(&temp, (jit_ulong)value1, (jit_ulong)-value2))
		{
			*result = jit_min_long;
			return 0;
		}
		if(temp > (((jit_ulong)jit_max_long) + 1))
		{
			*result = jit_min_long;
			return 0;
		}
		*result = -((jit_long)temp);
		return 1;
	}
	else if(value2 >= 0)
	{
		/* The second value is positive */
		if(!jit_ulong_mul_ovf(&temp, (jit_ulong)-value1, (jit_ulong)value2))
		{
			*result = jit_min_long;
			return 0;
		}
		if(temp > (((jit_ulong)jit_max_long) + 1))
		{
			*result = jit_min_long;
			return 0;
		}
		*result = -((jit_long)temp);
		return 1;
	}
	else
	{
		/* Both values are negative */
		if(!jit_ulong_mul_ovf(&temp, (jit_ulong)-value1, (jit_ulong)-value2))
		{
			*result = jit_max_long;
			return 0;
		}
		if(temp > ((jit_ulong)jit_max_long))
		{
			*result = jit_max_long;
			return 0;
		}
		*result = (jit_long)temp;
		return 1;
	}
}

jit_long jit_long_neg(jit_long value1)
{
	return -value1;
}

jit_long jit_long_and(jit_long value1, jit_long value2)
{
	return value1 & value2;
}

jit_long jit_long_or(jit_long value1, jit_long value2)
{
	return value1 | value2;
}

jit_long jit_long_xor(jit_long value1, jit_long value2)
{
	return value1 ^ value2;
}

jit_long jit_long_not(jit_long value1)
{
	return ~value1;
}

jit_long jit_long_shl(jit_long value1, jit_uint value2)
{
	return value1 << (value2 & 0x3F);
}

jit_long jit_long_shr(jit_long value1, jit_uint value2)
{
	return value1 >> (value2 & 0x3F);
}

jit_int jit_long_eq(jit_long value1, jit_long value2)
{
	return (value1 == value2);
}

jit_int jit_long_ne(jit_long value1, jit_long value2)
{
	return (value1 != value2);
}

jit_int jit_long_lt(jit_long value1, jit_long value2)
{
	return (value1 < value2);
}

jit_int jit_long_le(jit_long value1, jit_long value2)
{
	return (value1 <= value2);
}

jit_int jit_long_gt(jit_long value1, jit_long value2)
{
	return (value1 > value2);
}

jit_int jit_long_ge(jit_long value1, jit_long value2)
{
	return (value1 >= value2);
}

jit_int jit_long_cmp(jit_long value1, jit_long value2)
{
	if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_long jit_long_abs(jit_long value1)
{
	return ((value1 >= 0) ? value1 : -value1);
}

jit_long jit_long_min(jit_long value1, jit_long value2)
{
	return ((value1 <= value2) ? value1 : value2);
}

jit_long jit_long_max(jit_long value1, jit_long value2)
{
	return ((value1 >= value2) ? value1 : value2);
}

jit_int jit_long_sign(jit_long value1)
{
	if(value1 < 0)
	{
		return -1;
	}
	else if(value1 > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_ulong jit_ulong_add (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_sub (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_mul (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_neg (jit_ulong @var{value1})
 * @deftypefunx jit_ulong jit_ulong_and (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_or (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_xor (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_not (jit_ulong @var{value1})
 * @deftypefunx jit_ulong jit_ulong_not (jit_ulong @var{value1})
 * @deftypefunx jit_ulong jit_ulong_shl (jit_ulong @var{value1}, jit_uint @var{value2})
 * @deftypefunx jit_ulong jit_ulong_shr (jit_ulong @var{value1}, jit_uint @var{value2})
 * Perform an arithmetic operation on unsigned 64-bit integers.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_ulong_add_ovf (jit_ulong *@var{result}, jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_sub_ovf (jit_ulong *@var{result}, jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_mul_ovf (jit_ulong *@var{result}, jit_ulong @var{value1}, jit_ulong @var{value2})
 * Perform an arithmetic operation on two unsigned 64-bit integers,
 * with overflow checking.  Returns @code{JIT_RESULT_OK}
 * or @code{JIT_RESULT_OVERFLOW}.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_ulong_div_ovf (jit_ulong *@var{result}, jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_rem_ovf (jit_ulong *@var{result}, jit_ulong @var{value1}, jit_ulong @var{value2})
 * Perform a division or remainder operation on two unsigned 64-bit integers.
 * Returns @code{JIT_RESULT_OK} or @code{JIT_RESULT_DIVISION_BY_ZERO}
 * (@code{JIT_RESULT_ARITHMETIC} is not possible with unsigned integers).
 * @end deftypefun
 *
 * @deftypefun jit_int jit_ulong_eq (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_ne (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_lt (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_le (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_gt (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_int jit_ulong_ge (jit_ulong @var{value1}, jit_ulong @var{value2})
 * Compare two unsigned 64-bit integers, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_ulong_cmp (jit_ulong @var{value1}, jit_ulong @var{value2})
 * Compare two unsigned 64-bit integers and return -1, 0, or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_ulong jit_ulong_min (jit_ulong @var{value1}, jit_ulong @var{value2})
 * @deftypefunx jit_ulong jit_ulong_max (jit_ulong @var{value1}, jit_ulong @var{value2})
 * Calculate the minimum or maximum for unsigned 64-bit integer values.
 * @end deftypefun
@*/
jit_ulong jit_ulong_add(jit_ulong value1, jit_ulong value2)
{
	return value1 + value2;
}

jit_ulong jit_ulong_sub(jit_ulong value1, jit_ulong value2)
{
	return value1 - value2;
}

jit_ulong jit_ulong_mul(jit_ulong value1, jit_ulong value2)
{
	return value1 * value2;
}

jit_int jit_ulong_div(jit_ulong *result, jit_ulong value1, jit_ulong value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else
	{
		*result = value1 / value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_ulong_rem(jit_ulong *result, jit_ulong value1, jit_ulong value2)
{
	if(value2 == 0)
	{
		*result = 0;
		return JIT_RESULT_DIVISION_BY_ZERO;
	}
	else
	{
		*result = value1 % value2;
		return JIT_RESULT_OK;
	}
}

jit_int jit_ulong_add_ovf(jit_ulong *result, jit_ulong value1, jit_ulong value2)
{
	return ((*result = value1 + value2) >= value1);
}

jit_int jit_ulong_sub_ovf(jit_ulong *result, jit_ulong value1, jit_ulong value2)
{
	return ((*result = value1 - value2) <= value1);
}

jit_int jit_ulong_mul_ovf(jit_ulong *result, jit_ulong value1, jit_ulong value2)
{
	jit_uint high1, low1, high2, low2, orig;
	jit_ulong temp;
	jit_uint result1, result2, result3, result4;
	high1 = (jit_uint)(value1 >> 32);
	low1  = (jit_uint)value1;
	high2 = (jit_uint)(value2 >> 32);
	low2  = (jit_uint)value2;
	temp = ((jit_ulong)low1) * ((jit_ulong)low2);
	result1 = (jit_uint)temp;
	result2 = (jit_uint)(temp >> 32);
	temp = ((jit_ulong)low1) * ((jit_ulong)high2);
	orig = result2;
	result2 += (jit_uint)temp;
	if(result2 < orig)
		result3 = (((jit_uint)(temp >> 32)) + 1);
	else
		result3 = ((jit_uint)(temp >> 32));
	temp = ((jit_ulong)high1) * ((jit_ulong)low2);
	orig = result2;
	result2 += (jit_uint)temp;
	if(result2 < orig)
	{
		orig = result3;
		result3 += (((jit_uint)(temp >> 32)) + 1);
		if(result3 < orig)
			result4 = 1;
		else
			result4 = 0;
	}
	else
	{
		orig = result3;
		result3 += ((jit_uint)(temp >> 32));
		if(result3 < orig)
			result4 = 1;
		else
			result4 = 0;
	}
	temp = ((jit_ulong)high1) * ((jit_ulong)high2);
	orig = result3;
	result3 += (jit_uint)temp;
	if(result3 < orig)
		result4 += ((jit_uint)(temp >> 32)) + 1;
	else
		result4 += ((jit_uint)(temp >> 32));
	if(result3 != 0 || result4 != 0)
	{
		*result = jit_max_ulong;
		return 0;
	}
	*result = (((jit_ulong)result2) << 32) | ((jit_ulong)result1);
	return 1;
}

jit_ulong jit_ulong_neg(jit_ulong value1)
{
	return (jit_ulong)(-((jit_long)value1));
}

jit_ulong jit_ulong_and(jit_ulong value1, jit_ulong value2)
{
	return value1 & value2;
}

jit_ulong jit_ulong_or(jit_ulong value1, jit_ulong value2)
{
	return value1 | value2;
}

jit_ulong jit_ulong_xor(jit_ulong value1, jit_ulong value2)
{
	return value1 ^ value2;
}

jit_ulong jit_ulong_not(jit_ulong value1)
{
	return ~value1;
}

jit_ulong jit_ulong_shl(jit_ulong value1, jit_uint value2)
{
	return value1 << (value2 & 0x3F);
}

jit_ulong jit_ulong_shr(jit_ulong value1, jit_uint value2)
{
	return value1 >> (value2 & 0x3F);
}

jit_int jit_ulong_eq(jit_ulong value1, jit_ulong value2)
{
	return (value1 == value2);
}

jit_int jit_ulong_ne(jit_ulong value1, jit_ulong value2)
{
	return (value1 != value2);
}

jit_int jit_ulong_lt(jit_ulong value1, jit_ulong value2)
{
	return (value1 < value2);
}

jit_int jit_ulong_le(jit_ulong value1, jit_ulong value2)
{
	return (value1 <= value2);
}

jit_int jit_ulong_gt(jit_ulong value1, jit_ulong value2)
{
	return (value1 > value2);
}

jit_int jit_ulong_ge(jit_ulong value1, jit_ulong value2)
{
	return (value1 >= value2);
}

jit_int jit_ulong_cmp(jit_ulong value1, jit_ulong value2)
{
	if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_ulong jit_ulong_min(jit_ulong value1, jit_ulong value2)
{
	return ((value1 <= value2) ? value1 : value2);
}

jit_ulong jit_ulong_max(jit_ulong value1, jit_ulong value2)
{
	return ((value1 >= value2) ? value1 : value2);
}

/*@
 * @deftypefun jit_float32 jit_float32_add (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_sub (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_mul (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_div (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_rem (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_ieee_rem (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_neg (jit_float32 @var{value1})
 * Perform an arithmetic operation on 32-bit floating-point values.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_float32_eq (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_int jit_float32_ne (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_int jit_float32_lt (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_int jit_float32_le (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_int jit_float32_gt (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_int jit_float32_ge (jit_float32 @var{value1}, jit_float32 @var{value2})
 * Compare two 32-bit floating-point values, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_float32_cmpl (jit_float32 @var{value1}, jit_float32 @var{value2})
 * Compare two 32-bit floating-point values and return -1, 0, or 1 based
 * on their relationship.  If either value is "not a number",
 * then -1 is returned.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_float32_cmpg (jit_float32 @var{value1}, jit_float32 @var{value2})
 * Compare two 32-bit floating-point values and return -1, 0, or 1 based
 * on their relationship.  If either value is "not a number",
 * then 1 is returned.
 * @end deftypefun
 *
 * @deftypefun jit_float32 jit_float32_abs (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_min (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_max (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_int jit_float32_sign (jit_float32 @var{value1})
 * Calculate the absolute value, minimum, maximum, or sign for
 * 32-bit floating point values.
 * @end deftypefun
@*/
jit_float32 jit_float32_add(jit_float32 value1, jit_float32 value2)
{
	return value1 + value2;
}

jit_float32 jit_float32_sub(jit_float32 value1, jit_float32 value2)
{
	return value1 - value2;
}

jit_float32 jit_float32_mul(jit_float32 value1, jit_float32 value2)
{
	return value1 * value2;
}

jit_float32 jit_float32_div(jit_float32 value1, jit_float32 value2)
{
	return value1 / value2;
}

jit_float32 jit_float32_rem(jit_float32 value1, jit_float32 value2)
{
#if defined(HAVE_FMODF)
	return fmod(value1, value2);
#elif defined(HAVE_FMOD)
	return fmod(value1, value2);
#elif defined(HAVE_CEILF) && defined(HAVE_FLOORF)
	jit_float32 temp = value1 / value2;
	if(jit_float32_is_nan(temp))
	{
		return temp;
	}
	if(temp < (jit_float32)0.0)
	{
		temp = ceilf(temp);
	}
	else
	{
		temp = floorf(temp);
	}
	return value1 - temp * value2;
#elif defined(HAVE_CEIL) && defined(HAVE_FLOOR)
	jit_float32 temp = value1 / value2;
	if(jit_float32_is_nan(temp))
	{
		return temp;
	}
	if(temp < (jit_float32)0.0)
	{
		temp = ceil(temp);
	}
	else
	{
		temp = floor(temp);
	}
	return value1 - temp * value2;
#else
	/* Don't know how to compute remainders on this platform */
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_ieee_rem(jit_float32 value1, jit_float32 value2)
{
#if defined(HAVE_REMAINDERF)
	return remainderf(value1, value2);
#elif defined(HAVE_REMAINDER)
	return remainder(value1, value2);
#elif defined(HAVE_DREMF)
	return dremf(value1, value2);
#elif defined(HAVE_DREM)
	return drem(value1, value2);
#elif defined(HAVE_CEILF) && defined(HAVE_FLOORF)
	jit_float32 temp = value1 / value2;
	jit_float32 ceil_value, floor_value;
	if(jit_float32_is_nan(temp))
	{
		return temp;
	}
	ceil_value = ceilf(temp);
	floor_value = floorf(temp);
	if((temp - floor_value) < (jit_float32)0.5)
	{
		temp = floor_value;
	}
	else if((temp - floor_value) > (jit_float32)0.5)
	{
		temp = ceil_value;
	}
	else if((floor(ceil_value / (jit_float32)2.0) * (jit_float32)2.0)
				== ceil_value)
	{
		temp = ceil_value;
	}
	else
	{
		temp = floor_value;
	}
	return value1 - temp * value2;
#elif defined(HAVE_CEIL) && defined(HAVE_FLOOR)
	jit_float32 temp = value1 / value2;
	jit_float32 ceil_value, floor_value;
	if(jit_float32_is_nan(temp))
	{
		return temp;
	}
	ceil_value = ceil(temp);
	floor_value = floor(temp);
	if((temp - floor_value) < (jit_float32)0.5)
	{
		temp = floor_value;
	}
	else if((temp - floor_value) > (jit_float32)0.5)
	{
		temp = ceil_value;
	}
	else if((floor(ceil_value / (jit_float32)2.0) * (jit_float32)2.0)
				== ceil_value)
	{
		temp = ceil_value;
	}
	else
	{
		temp = floor_value;
	}
	return value1 - temp * value2;
#else
	/* Don't know how to compute remainders on this platform */
	return (jit_float32)(0.0 / 0.0);
#endif
}

jit_float32 jit_float32_neg(jit_float32 value1)
{
	return -value1;
}

jit_int jit_float32_eq(jit_float32 value1, jit_float32 value2)
{
	return (value1 == value2);
}

jit_int jit_float32_ne(jit_float32 value1, jit_float32 value2)
{
	return (value1 != value2);
}

jit_int jit_float32_lt(jit_float32 value1, jit_float32 value2)
{
	return (value1 < value2);
}

jit_int jit_float32_le(jit_float32 value1, jit_float32 value2)
{
	return (value1 <= value2);
}

jit_int jit_float32_gt(jit_float32 value1, jit_float32 value2)
{
	return (value1 > value2);
}

jit_int jit_float32_ge(jit_float32 value1, jit_float32 value2)
{
	return (value1 >= value2);
}

jit_int jit_float32_cmpl(jit_float32 value1, jit_float32 value2)
{
	if(jit_float32_is_nan(value1) || jit_float32_is_nan(value2))
	{
		return -1;
	}
	else if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_int jit_float32_cmpg(jit_float32 value1, jit_float32 value2)
{
	if(jit_float32_is_nan(value1) || jit_float32_is_nan(value2))
	{
		return 1;
	}
	else if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_float32 jit_float32_abs(jit_float32 value1)
{
	if(jit_float32_is_nan(value1))
	{
		return jit_float32_nan;
	}
	return ((value1 >= 0) ? value1 : -value1);
}

jit_float32 jit_float32_min(jit_float32 value1, jit_float32 value2)
{
	if(jit_float32_is_nan(value1) || jit_float32_is_nan(value2))
	{
		return jit_float32_nan;
	}
	return ((value1 <= value2) ? value1 : value2);
}

jit_float32 jit_float32_max(jit_float32 value1, jit_float32 value2)
{
	if(jit_float32_is_nan(value1) || jit_float32_is_nan(value2))
	{
		return jit_float32_nan;
	}
	return ((value1 >= value2) ? value1 : value2);
}

jit_int jit_float32_sign(jit_float32 value1)
{
	if(jit_float32_is_nan(value1))
	{
		return 0;
	}
	else if(value1 < 0)
	{
		return -1;
	}
	else if(value1 > 0)
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_float32 jit_float32_acos (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_asin (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_atan (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_atan2 (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_cos (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_cosh (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_exp (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_log (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_log10 (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_pow (jit_float32 @var{value1}, jit_float32 @var{value2})
 * @deftypefunx jit_float32 jit_float32_sin (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_sinh (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_sqrt (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_tan (jit_float32 @var{value1})
 * @deftypefunx jit_float32 jit_float32_tanh (jit_float32 @var{value1})
 * Apply a mathematical function to one or two 32-bit floating-point values.
 * @end deftypefun
@*/
jit_float32 jit_float32_acos(jit_float32 value1)
{
#if defined(HAVE_ACOSF)
	return (jit_float32)(acosf(value1));
#elif defined(HAVE_ACOS)
	return (jit_float32)(acos(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_asin(jit_float32 value1)
{
#if defined(HAVE_ASINF)
	return (jit_float32)(asinf(value1));
#elif defined(HAVE_ASIN)
	return (jit_float32)(asin(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_atan(jit_float32 value1)
{
#if defined(HAVE_ATANF)
	return (jit_float32)(atanf(value1));
#elif defined(HAVE_ATAN)
	return (jit_float32)(atan(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_atan2(jit_float32 value1, jit_float32 value2)
{
#if defined(HAVE_ATAN2F)
	return (jit_float32)(atan2f(value1, value2));
#elif defined(HAVE_ATAN2)
	return (jit_float32)(atan2(value1, value2));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_cos(jit_float32 value1)
{
#if defined(HAVE_COSF)
	return (jit_float32)(cosf(value1));
#elif defined(HAVE_COS)
	return (jit_float32)(cos(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_cosh(jit_float32 value1)
{
#if defined(HAVE_COSHF)
	return (jit_float32)(coshf(value1));
#elif defined(HAVE_COSH)
	return (jit_float32)(cosh(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_exp(jit_float32 value1)
{
#if defined(HAVE_EXPF)
	return (jit_float32)(expf(value1));
#elif defined(HAVE_EXP)
	return (jit_float32)(exp(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_log(jit_float32 value1)
{
#if defined(HAVE_LOGF)
	return (jit_float32)(logf(value1));
#elif defined(HAVE_LOG)
	return (jit_float32)(log(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_log10(jit_float32 value1)
{
#if defined(HAVE_LOG10F)
	return (jit_float32)(log10f(value1));
#elif defined(HAVE_LOG10)
	return (jit_float32)(log10(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_pow(jit_float32 value1, jit_float32 value2)
{
#if defined(HAVE_POWF)
	return (jit_float32)(powf(value1, value2));
#elif defined(HAVE_POW)
	return (jit_float32)(pow(value1, value2));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_sin(jit_float32 value1)
{
#if defined(HAVE_SINF)
	return (jit_float32)(sinf(value1));
#elif defined(HAVE_SIN)
	return (jit_float32)(sin(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_sinh(jit_float32 value1)
{
#if defined(HAVE_SINHF)
	return (jit_float32)(sinhf(value1));
#elif defined(HAVE_SINH)
	return (jit_float32)(sinh(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_sqrt(jit_float32 value1)
{
	/* Some platforms give a SIGFPE for negative arguments (e.g. Alpha) */
	if(value1 < (jit_float32)0.0)
	{
		return jit_float32_nan;
	}
#if defined(HAVE_SQRTF)
	return (jit_float32)(sqrt(value1));
#elif defined(HAVE_SQRT)
	return (jit_float32)(sqrt(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_tan(jit_float32 value1)
{
#if defined(HAVE_TANF)
	return (jit_float32)(tanf(value1));
#elif defined(HAVE_TAN)
	return (jit_float32)(tan(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float32 jit_float32_tanh(jit_float32 value1)
{
#if defined(HAVE_TANHF)
	return (jit_float32)(tanhf(value1));
#elif defined(HAVE_TANH)
	return (jit_float32)(tanh(value1));
#else
	return jit_float32_nan;
#endif
}

/*@
 * @deftypefun jit_int jit_float32_is_finite (jit_float32 @var{value})
 * Determine if a 32-bit floating point value is finite, returning
 * non-zero if it is, or zero if it is not.  If the value is
 * "not a number", this function returns zero.
 * @end deftypefun
@*/
jit_int jit_float32_is_finite(jit_float32 value)
{
#if defined(hpux) || defined(JIT_WIN32_PLATFORM)
	return isfinite(value);
#else /* !hpux */
#if defined(HAVE_FINITEF)
	return finitef(value);
#elif defined(HAVE_FINITE)
	return isfinite(value);
#else /* !HAVE_FINITE */
#if defined(HAVE_ISNANF) && defined(HAVE_ISINFF)
	return (!isnanf(value) && isinff(value) == 0);
#elif defined(HAVE_ISNAN) && defined(HAVE_ISINF)
	return (!isnan(value) && isinf(value) == 0);
#else
	#error "Don't know how to determine if floating point numbers are finite"
	return 1;
#endif
#endif /* !HAVE_FINITE */
#endif /* !hpux */
}

/*@
 * @deftypefun jit_int jit_float32_is_nan (jit_float32 @var{value})
 * Determine if a 32-bit floating point value is "not a number", returning
 * non-zero if it is, or zero if it is not.
 * @end deftypefun
@*/
jit_int jit_float32_is_nan(jit_float32 value)
{
#if defined(HAVE_ISNANF)
	return isnanf(value);
#elif defined(HAVE_ISNAN)
	return isnan(value);
#else
	return (value != value);
#endif
}

/*@
 * @deftypefun jit_int jit_float32_is_inf (jit_float32 @var{value})
 * Determine if a 32-bit floating point value is infinite or not.
 * Returns -1 for negative infinity, 1 for positive infinity,
 * and 0 for everything else.
 *
 * Note: this function is preferable to the system @code{isinf} intrinsic
 * because some systems have a broken @code{isinf} function that returns
 * 1 for both positive and negative infinity.
 * @end deftypefun
@*/
jit_int jit_float32_is_inf(jit_float32 value)
{
	/* The code below works around broken "isinf" implementations */
#if defined(HAVE_ISINFF)
	if(isinff(value) == 0)
	{
		return 0;
	}
#elif defined(HAVE_ISINF)
	if(isinf(value) == 0)
	{
		return 0;
	}
#else
	if(jit_float32_is_nan(value) || jit_float32_is_finite(value))
	{
		return 0;
	}
#endif
	if(value < (jit_float32)0.0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/*@
 * @deftypefun jit_float64 jit_float64_add (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_sub (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_mul (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_div (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_rem (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_ieee_rem (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_neg (jit_float64 @var{value1})
 * Perform an arithmetic operation on 64-bit floating-point values.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_float64_eq (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_int jit_float64_ne (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_int jit_float64_lt (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_int jit_float64_le (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_int jit_float64_gt (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_int jit_float64_ge (jit_float64 @var{value1}, jit_float64 @var{value2})
 * Compare two 64-bit floating-point values, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_float64_cmpl (jit_float64 @var{value1}, jit_float64 @var{value2})
 * Compare two 64-bit floating-point values and return -1, 0, or 1 based
 * on their relationship.  If either value is "not a number",
 * then -1 is returned.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_float64_cmpg (jit_float64 @var{value1}, jit_float64 @var{value2})
 * Compare two 64-bit floating-point values and return -1, 0, or 1 based
 * on their relationship.  If either value is "not a number",
 * then 1 is returned.
 * @end deftypefun
 *
 * @deftypefun jit_float64 jit_float64_abs (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_min (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_max (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_int jit_float64_sign (jit_float64 @var{value1})
 * Calculate the absolute value, minimum, maximum, or sign for
 * 64-bit floating point values.
 * @end deftypefun
@*/
jit_float64 jit_float64_add(jit_float64 value1, jit_float64 value2)
{
	return value1 + value2;
}

jit_float64 jit_float64_sub(jit_float64 value1, jit_float64 value2)
{
	return value1 - value2;
}

jit_float64 jit_float64_mul(jit_float64 value1, jit_float64 value2)
{
	return value1 * value2;
}

jit_float64 jit_float64_div(jit_float64 value1, jit_float64 value2)
{
	return value1 / value2;
}

jit_float64 jit_float64_rem(jit_float64 value1, jit_float64 value2)
{
#if defined(HAVE_FMOD)
	return fmod(value1, value2);
#elif defined(HAVE_CEIL) && defined(HAVE_FLOOR)
	jit_float64 temp = value1 / value2;
	if(jit_float64_is_nan(temp))
	{
		return temp;
	}
	if(temp < (jit_float64)0.0)
	{
		temp = ceil(temp);
	}
	else
	{
		temp = floor(temp);
	}
	return value1 - temp * value2;
#else
	/* Don't know how to compute remainders on this platform */
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_ieee_rem(jit_float64 value1, jit_float64 value2)
{
#if defined(HAVE_REMAINDER)
	return remainder(value1, value2);
#elif defined(HAVE_DREM)
	return drem(value1, value2);
#elif defined(HAVE_CEIL) && defined(HAVE_FLOOR)
	jit_float64 temp = value1 / value2;
	jit_float64 ceil_value, floor_value;
	if(jit_float64_is_nan(temp))
	{
		return temp;
	}
	ceil_value = ceil(temp);
	floor_value = floor(temp);
	if((temp - floor_value) < (jit_float64)0.5)
	{
		temp = floor_value;
	}
	else if((temp - floor_value) > (jit_float64)0.5)
	{
		temp = ceil_value;
	}
	else if((floor(ceil_value / (jit_float64)2.0) * (jit_float64)2.0)
				== ceil_value)
	{
		temp = ceil_value;
	}
	else
	{
		temp = floor_value;
	}
	return value1 - temp * value2;
#else
	/* Don't know how to compute remainders on this platform */
	return (jit_float64)(0.0 / 0.0);
#endif
}

jit_float64 jit_float64_neg(jit_float64 value1)
{
	return -value1;
}

jit_int jit_float64_eq(jit_float64 value1, jit_float64 value2)
{
	return (value1 == value2);
}

jit_int jit_float64_ne(jit_float64 value1, jit_float64 value2)
{
	return (value1 != value2);
}

jit_int jit_float64_lt(jit_float64 value1, jit_float64 value2)
{
	return (value1 < value2);
}

jit_int jit_float64_le(jit_float64 value1, jit_float64 value2)
{
	return (value1 <= value2);
}

jit_int jit_float64_gt(jit_float64 value1, jit_float64 value2)
{
	return (value1 > value2);
}

jit_int jit_float64_ge(jit_float64 value1, jit_float64 value2)
{
	return (value1 >= value2);
}

jit_int jit_float64_cmpl(jit_float64 value1, jit_float64 value2)
{
	if(jit_float64_is_nan(value1) || jit_float64_is_nan(value2))
	{
		return -1;
	}
	else if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_int jit_float64_cmpg(jit_float64 value1, jit_float64 value2)
{
	if(jit_float64_is_nan(value1) || jit_float64_is_nan(value2))
	{
		return 1;
	}
	else if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_float64 jit_float64_abs(jit_float64 value1)
{
	if(jit_float64_is_nan(value1))
	{
		return jit_float64_nan;
	}
	return ((value1 >= 0) ? value1 : -value1);
}

jit_float64 jit_float64_min(jit_float64 value1, jit_float64 value2)
{
	if(jit_float64_is_nan(value1) || jit_float64_is_nan(value2))
	{
		return jit_float64_nan;
	}
	return ((value1 <= value2) ? value1 : value2);
}

jit_float64 jit_float64_max(jit_float64 value1, jit_float64 value2)
{
	if(jit_float64_is_nan(value1) || jit_float64_is_nan(value2))
	{
		return jit_float64_nan;
	}
	return ((value1 >= value2) ? value1 : value2);
}

jit_int jit_float64_sign(jit_float64 value1)
{
	if(jit_float64_is_nan(value1))
	{
		return 0;
	}
	else if(value1 < 0)
	{
		return -1;
	}
	else if(value1 > 0)
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_float64 jit_float64_acos (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_asin (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_atan (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_atan2 (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_cos (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_cosh (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_exp (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_log (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_log10 (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_pow (jit_float64 @var{value1}, jit_float64 @var{value2})
 * @deftypefunx jit_float64 jit_float64_sin (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_sinh (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_sqrt (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_tan (jit_float64 @var{value1})
 * @deftypefunx jit_float64 jit_float64_tanh (jit_float64 @var{value1})
 * Apply a mathematical function to one or two 64-bit floating-point values.
 * @end deftypefun
@*/
jit_float64 jit_float64_acos(jit_float64 value1)
{
#if defined(HAVE_ACOS)
	return (jit_float64)(acos(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_asin(jit_float64 value1)
{
#if defined(HAVE_ASIN)
	return (jit_float64)(asin(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_atan(jit_float64 value1)
{
#if defined(HAVE_ATAN)
	return (jit_float64)(atan(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_atan2(jit_float64 value1, jit_float64 value2)
{
#if defined(HAVE_ATAN2)
	return (jit_float64)(atan2(value1, value2));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_cos(jit_float64 value1)
{
#if defined(HAVE_COS)
	return (jit_float64)(cos(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_cosh(jit_float64 value1)
{
#if defined(HAVE_COSH)
	return (jit_float64)(cosh(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_exp(jit_float64 value1)
{
#if defined(HAVE_EXP)
	return (jit_float64)(exp(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_log(jit_float64 value1)
{
#if defined(HAVE_LOG)
	return (jit_float64)(log(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_log10(jit_float64 value1)
{
#if defined(HAVE_LOG10)
	return (jit_float64)(log10(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_pow(jit_float64 value1, jit_float64 value2)
{
#if defined(HAVE_POW)
	return (jit_float64)(pow(value1, value2));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_sin(jit_float64 value1)
{
#if defined(HAVE_SIN)
	return (jit_float64)(sin(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_sinh(jit_float64 value1)
{
#if defined(HAVE_SINH)
	return (jit_float64)(sinh(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_sqrt(jit_float64 value1)
{
	/* Some platforms give a SIGFPE for negative arguments (e.g. Alpha) */
	if(value1 < (jit_float64)0.0)
	{
		return jit_float64_nan;
	}
#if defined(HAVE_SQRT)
	return (jit_float64)(sqrt(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_tan(jit_float64 value1)
{
#if defined(HAVE_TAN)
	return (jit_float64)(tan(value1));
#else
	return jit_float64_nan;
#endif
}

jit_float64 jit_float64_tanh(jit_float64 value1)
{
#if defined(HAVE_TANH)
	return (jit_float64)(tanh(value1));
#else
	return jit_float64_nan;
#endif
}

/*@
 * @deftypefun jit_int jit_float64_is_finite (jit_float64 @var{value})
 * Determine if a 64-bit floating point value is finite, returning
 * non-zero if it is, or zero if it is not.  If the value is
 * "not a number", this function returns zero.
 * @end deftypefun
@*/
jit_int jit_float64_is_finite(jit_float64 value)
{
#if defined(hpux) || defined(JIT_WIN32_PLATFORM)
	return isfinite(value);
#else /* !hpux */
#if defined(HAVE_FINITE)
	return isfinite(value);
#else /* !HAVE_FINITE */
#if defined(HAVE_ISNAN) && defined(HAVE_ISINF)
	return (!isnan(value) && isinf(value) == 0);
#else
	#error "Don't know how to determine if floating point numbers are finite"
	return 1;
#endif
#endif /* !HAVE_FINITE */
#endif /* !hpux */
}

/*@
 * @deftypefun jit_int jit_float64_is_nan (jit_float64 @var{value})
 * Determine if a 64-bit floating point value is "not a number", returning
 * non-zero if it is, or zero if it is not.
 * @end deftypefun
@*/
jit_int jit_float64_is_nan(jit_float64 value)
{
#if defined(HAVE_ISNAN)
	return isnan(value);
#else
	return (value != value);
#endif
}

/*@
 * @deftypefun jit_int jit_float64_is_inf (jit_float64 @var{value})
 * Determine if a 64-bit floating point value is infinite or not.
 * Returns -1 for negative infinity, 1 for positive infinity,
 * and 0 for everything else.
 *
 * Note: this function is preferable to the system @code{isinf} intrinsic
 * because some systems have a broken @code{isinf} function that returns
 * 1 for both positive and negative infinity.
 * @end deftypefun
@*/
jit_int jit_float64_is_inf(jit_float64 value)
{
	/* The code below works around broken "isinf" implementations */
#if defined(HAVE_ISINF)
	if(isinf(value) == 0)
	{
		return 0;
	}
#else
	if(jit_float64_is_nan(value) || jit_float64_is_finite(value))
	{
		return 0;
	}
#endif
	if(value < (jit_float64)0.0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/*@
 * @deftypefun jit_nfloat jit_nfloat_add (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_sub (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_mul (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_div (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_rem (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_ieee_rem (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_neg (jit_nfloat @var{value1})
 * Perform an arithmetic operation on native floating-point values.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_nfloat_eq (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_int jit_nfloat_ne (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_int jit_nfloat_lt (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_int jit_nfloat_le (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_int jit_nfloat_gt (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_int jit_nfloat_ge (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * Compare two native floating-point values, returning 0 or 1 based
 * on their relationship.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_nfloat_cmpl (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * Compare two native floating-point values and return -1, 0, or 1 based
 * on their relationship.  If either value is "not a number",
 * then -1 is returned.
 * @end deftypefun
 *
 * @deftypefun jit_int jit_nfloat_cmpg (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * Compare two native floating-point values and return -1, 0, or 1 based
 * on their relationship.  If either value is "not a number",
 * then 1 is returned.
 * @end deftypefun
 *
 * @deftypefun jit_nfloat jit_nfloat_abs (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_min (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_max (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_int jit_nfloat_sign (jit_nfloat @var{value1})
 * Calculate the absolute value, minimum, maximum, or sign for
 * native floating point values.
 * @end deftypefun
@*/
jit_nfloat jit_nfloat_add(jit_nfloat value1, jit_nfloat value2)
{
	return value1 + value2;
}

jit_nfloat jit_nfloat_sub(jit_nfloat value1, jit_nfloat value2)
{
	return value1 - value2;
}

jit_nfloat jit_nfloat_mul(jit_nfloat value1, jit_nfloat value2)
{
	return value1 * value2;
}

jit_nfloat jit_nfloat_div(jit_nfloat value1, jit_nfloat value2)
{
	return value1 / value2;
}

jit_nfloat jit_nfloat_rem(jit_nfloat value1, jit_nfloat value2)
{
#if defined(HAVE_FMODL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return fmodl(value1, value2);
#elif defined(HAVE_FMOD)
	return fmod(value1, value2);
#elif defined(HAVE_CEILL) && defined(HAVE_FLOORL) && \
		!defined(JIT_NFLOAT_IS_DOUBLE)
	jit_nfloat temp = value1 / value2;
	if(jit_nfloat_is_nan(temp))
	{
		return temp;
	}
	if(temp < (jit_nfloat)0.0)
	{
		temp = ceill(temp);
	}
	else
	{
		temp = floorl(temp);
	}
	return value1 - temp * value2;
#elif defined(HAVE_CEIL) && defined(HAVE_FLOOR)
	jit_nfloat temp = value1 / value2;
	if(jit_nfloat_is_nan(temp))
	{
		return temp;
	}
	if(temp < (jit_nfloat)0.0)
	{
		temp = ceil(temp);
	}
	else
	{
		temp = floor(temp);
	}
	return value1 - temp * value2;
#else
	/* Don't know how to compute remainders on this platform */
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_ieee_rem(jit_nfloat value1, jit_nfloat value2)
{
#if defined(HAVE_REMAINDERL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return remainderl(value1, value2);
#elif defined(HAVE_REMAINDER)
	return remainder(value1, value2);
#elif defined(HAVE_DREML) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return dreml(value1, value2);
#elif defined(HAVE_DREM)
	return drem(value1, value2);
#elif defined(HAVE_CEILL) && defined(HAVE_FLOORL) && \
		!defined(JIT_NFLOAT_IS_DOUBLE)
	jit_nfloat temp = value1 / value2;
	jit_nfloat ceil_value, floor_value;
	if(jit_nfloat_is_nan(temp))
	{
		return temp;
	}
	ceil_value = ceill(temp);
	floor_value = floorl(temp);
	if((temp - floor_value) < (jit_nfloat)0.5)
	{
		temp = floor_value;
	}
	else if((temp - floor_value) > (jit_nfloat)0.5)
	{
		temp = ceil_value;
	}
	else if((floor(ceil_value / (jit_nfloat)2.0) * (jit_nfloat)2.0)
				== ceil_value)
	{
		temp = ceil_value;
	}
	else
	{
		temp = floor_value;
	}
	return value1 - temp * value2;
#elif defined(HAVE_CEIL) && defined(HAVE_FLOOR)
	jit_nfloat temp = value1 / value2;
	jit_nfloat ceil_value, floor_value;
	if(jit_nfloat_is_nan(temp))
	{
		return temp;
	}
	ceil_value = ceil(temp);
	floor_value = floor(temp);
	if((temp - floor_value) < (jit_nfloat)0.5)
	{
		temp = floor_value;
	}
	else if((temp - floor_value) > (jit_nfloat)0.5)
	{
		temp = ceil_value;
	}
	else if((floor(ceil_value / (jit_nfloat)2.0) * (jit_nfloat)2.0)
				== ceil_value)
	{
		temp = ceil_value;
	}
	else
	{
		temp = floor_value;
	}
	return value1 - temp * value2;
#else
	/* Don't know how to compute remainders on this platform */
	return (jit_nfloat)(0.0 / 0.0);
#endif
}

jit_nfloat jit_nfloat_neg(jit_nfloat value1)
{
	return -value1;
}

jit_int jit_nfloat_eq(jit_nfloat value1, jit_nfloat value2)
{
	return (value1 == value2);
}

jit_int jit_nfloat_ne(jit_nfloat value1, jit_nfloat value2)
{
	return (value1 != value2);
}

jit_int jit_nfloat_lt(jit_nfloat value1, jit_nfloat value2)
{
	return (value1 < value2);
}

jit_int jit_nfloat_le(jit_nfloat value1, jit_nfloat value2)
{
	return (value1 <= value2);
}

jit_int jit_nfloat_gt(jit_nfloat value1, jit_nfloat value2)
{
	return (value1 > value2);
}

jit_int jit_nfloat_ge(jit_nfloat value1, jit_nfloat value2)
{
	return (value1 >= value2);
}

jit_int jit_nfloat_cmpl(jit_nfloat value1, jit_nfloat value2)
{
	if(jit_nfloat_is_nan(value1) || jit_nfloat_is_nan(value2))
	{
		return -1;
	}
	else if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_int jit_nfloat_cmpg(jit_nfloat value1, jit_nfloat value2)
{
	if(jit_nfloat_is_nan(value1) || jit_nfloat_is_nan(value2))
	{
		return 1;
	}
	else if(value1 < value2)
	{
		return -1;
	}
	else if(value1 > value2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

jit_nfloat jit_nfloat_abs(jit_nfloat value1)
{
	if(jit_nfloat_is_nan(value1))
	{
		return jit_nfloat_nan;
	}
	return ((value1 >= 0) ? value1 : -value1);
}

jit_nfloat jit_nfloat_min(jit_nfloat value1, jit_nfloat value2)
{
	if(jit_nfloat_is_nan(value1) || jit_nfloat_is_nan(value2))
	{
		return jit_nfloat_nan;
	}
	return ((value1 <= value2) ? value1 : value2);
}

jit_nfloat jit_nfloat_max(jit_nfloat value1, jit_nfloat value2)
{
	if(jit_nfloat_is_nan(value1) || jit_nfloat_is_nan(value2))
	{
		return jit_nfloat_nan;
	}
	return ((value1 >= value2) ? value1 : value2);
}

jit_int jit_nfloat_sign(jit_nfloat value1)
{
	if(jit_nfloat_is_nan(value1))
	{
		return 0;
	}
	else if(value1 < 0)
	{
		return -1;
	}
	else if(value1 > 0)
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_nfloat jit_nfloat_acos (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_asin (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_atan (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_atan2 (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_cos (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_cosh (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_exp (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_log (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_log10 (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_pow (jit_nfloat @var{value1}, jit_nfloat @var{value2})
 * @deftypefunx jit_nfloat jit_nfloat_sin (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_sinh (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_sqrt (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_tan (jit_nfloat @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_tanh (jit_nfloat @var{value1})
 * Apply a mathematical function to one or two native floating-point values.
 * @end deftypefun
@*/
jit_nfloat jit_nfloat_acos(jit_nfloat value1)
{
#if defined(HAVE_ACOSL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(acosl(value1));
#elif defined(HAVE_ACOS)
	return (jit_nfloat)(acos(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_asin(jit_nfloat value1)
{
#if defined(HAVE_ASINL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(asinl(value1));
#elif defined(HAVE_ASIN)
	return (jit_nfloat)(asin(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_atan(jit_nfloat value1)
{
#if defined(HAVE_ATANL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(atanl(value1));
#elif defined(HAVE_ATAN)
	return (jit_nfloat)(atan(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_atan2(jit_nfloat value1, jit_nfloat value2)
{
#if defined(HAVE_ATAN2L) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(atan2l(value1, value2));
#elif defined(HAVE_ATAN2)
	return (jit_nfloat)(atan2(value1, value2));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_cos(jit_nfloat value1)
{
#if defined(HAVE_COSL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(cosl(value1));
#elif defined(HAVE_COS)
	return (jit_nfloat)(cos(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_cosh(jit_nfloat value1)
{
#if defined(HAVE_COSHL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(coshl(value1));
#elif defined(HAVE_COSH)
	return (jit_nfloat)(cosh(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_exp(jit_nfloat value1)
{
#if defined(HAVE_EXPL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(expl(value1));
#elif defined(HAVE_EXP)
	return (jit_nfloat)(exp(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_log(jit_nfloat value1)
{
#if defined(HAVE_LOGL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(logl(value1));
#elif defined(HAVE_LOG)
	return (jit_nfloat)(log(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_log10(jit_nfloat value1)
{
#if defined(HAVE_LOG10L) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(log10l(value1));
#elif defined(HAVE_LOG10)
	return (jit_nfloat)(log10(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_pow(jit_nfloat value1, jit_nfloat value2)
{
#if defined(HAVE_POWL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(powl(value1, value2));
#elif defined(HAVE_POW)
	return (jit_nfloat)(pow(value1, value2));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_sin(jit_nfloat value1)
{
#if defined(HAVE_SINL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(sinl(value1));
#elif defined(HAVE_SIN)
	return (jit_nfloat)(sin(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_sinh(jit_nfloat value1)
{
#if defined(HAVE_SINHL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(sinhl(value1));
#elif defined(HAVE_SINH)
	return (jit_nfloat)(sinh(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_sqrt(jit_nfloat value1)
{
	/* Some platforms give a SIGFPE for negative arguments (e.g. Alpha) */
	if(value1 < (jit_nfloat)0.0)
	{
		return jit_nfloat_nan;
	}
#if defined(HAVE_SQRTL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(sqrtl(value1));
#elif defined(HAVE_SQRT)
	return (jit_nfloat)(sqrt(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_tan(jit_nfloat value1)
{
#if defined(HAVE_TANL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(tanl(value1));
#elif defined(HAVE_TAN)
	return (jit_nfloat)(tan(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_nfloat jit_nfloat_tanh(jit_nfloat value1)
{
#if defined(HAVE_TANHL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(tanhl(value1));
#elif defined(HAVE_TANH)
	return (jit_nfloat)(tanh(value1));
#else
	return jit_nfloat_nan;
#endif
}

/*@
 * @deftypefun jit_int jit_nfloat_is_finite (jit_nfloat @var{value})
 * Determine if a native floating point value is finite, returning
 * non-zero if it is, or zero if it is not.  If the value is
 * "not a number", this function returns zero.
 * @end deftypefun
@*/
jit_int jit_nfloat_is_finite(jit_nfloat value)
{
#if defined(hpux) || defined(JIT_WIN32_PLATFORM)
	return isfinite(value);
#else /* !hpux */
#if defined(HAVE_FINITEL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return finitel(value);
#elif defined(HAVE_FINITE)
	return isfinite(value);
#else /* !HAVE_FINITE */
#if defined(HAVE_ISNANL) && defined(HAVE_ISINFL) && \
		!defined(JIT_NFLOAT_IS_DOUBLE)
	return (!isnanl(value) && isinfl(value) == 0);
#elif defined(HAVE_ISNAN) && defined(HAVE_ISINF)
	return (!isnan(value) && isinf(value) == 0);
#else
	#error "Don't know how to determine if floating point numbers are finite"
	return 1;
#endif
#endif /* !HAVE_FINITE */
#endif /* !hpux */
}

/*@
 * @deftypefun jit_int jit_nfloat_is_nan (jit_nfloat @var{value})
 * Determine if a native floating point value is "not a number", returning
 * non-zero if it is, or zero if it is not.
 * @end deftypefun
@*/
jit_int jit_nfloat_is_nan(jit_nfloat value)
{
#if defined(HAVE_ISNANL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return isnanl(value);
#elif defined(HAVE_ISNAN)
	return isnan(value);
#else
	return (value != value);
#endif
}

/*@
 * @deftypefun jit_int jit_nfloat_is_inf (jit_nfloat @var{value})
 * Determine if a native floating point value is infinite or not.
 * Returns -1 for negative infinity, 1 for positive infinity,
 * and 0 for everything else.
 *
 * Note: this function is preferable to the system @code{isinf} intrinsic
 * because some systems have a broken @code{isinf} function that returns
 * 1 for both positive and negative infinity.
 * @end deftypefun
@*/
jit_int jit_nfloat_is_inf(jit_nfloat value)
{
	/* The code below works around broken "isinf" implementations */
#if defined(HAVE_ISINFL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	if(isinfl(value) == 0)
	{
		return 0;
	}
#elif defined(HAVE_ISINF)
	if(isinf(value) == 0)
	{
		return 0;
	}
#else
	if(jit_nfloat_is_nan(value) || jit_nfloat_is_finite(value))
	{
		return 0;
	}
#endif
	if(value < (jit_nfloat)0.0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

/*@
 * Floatingpoint rounding operations.defined by ieee754
 * @deftypefun jit_float32 jit_float32_rint (jit_float32 @var{value1})
 * @deftypefunx jit_float64 jit_float64_rint (jit_float64 @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_rint (jit_nfloat @var{value1})
 * Round @var{value1} to the nearest integer.  Half-way cases
 * are rounded to an even number.
 * @end deftypefun
 * @deftypefun jit_float32 jit_float32_ceil (jit_float32 @var{value1})
 * @deftypefunx jit_float64 jit_float64_ceil (jit_float64 @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_ceil (jit_nfloat @var{value1})
 * Round @var{value1} up towards positive infinity.
 * @end deftypefun
 * @deftypefun jit_float32 jit_float32_floor (jit_float32 @var{value1})
 * @deftypefunx jit_float64 jit_float64_floor (jit_float64 @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_floor (jit_nfloat @var{value1})
 * Round @var{value1} down towards negative infinity.
 * @end deftypefun
 * @deftypefun jit_float32 jit_float32_trunc (jit_float32 @var{value1})
 * @deftypefunx jit_float64 jit_float64_trunc (jit_float64 @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_trunc (jit_nfloat @var{value1})
 * Round @var{value1} towards zero.
 * @end deftypefun
@*/

/*
 * NOTE: rint rounds the value according to the current rounding mode.
 * The default rounding mode is round to nearest with half way cases
 * rounded to the even number. So there is no need to set the rounding
 * mode here.
 */
jit_float32 jit_float32_rint(jit_float32 value1)
{
#ifdef HAVE_RINTF
	return (jit_float32)rintf(value1);
#elif defined(HAVE_RINT)
	return (jit_float32)(rint(value1));
#else
	jit_float32 above, below;
	if(!jit_float32_is_finite(value1))
	{
		return value1;
	}
	above = jit_float32_ceil(value1);
	below = jit_float32_floor(value1);
	if((above - value1) < (jit_float32)0.5)
	{
		return above;
	}
	else if((value1 - below) < (jit_float32)0.5)
	{
		return below;
	}
	else if(jit_float32_ieee_rem(above, (jit_float32)2.0) == (jit_float32)0.0)
	{
		return above;
	}
	else
	{
		return below;
	}
#endif
}

jit_float64 jit_float64_rint(jit_float64 value1)
{
#ifdef HAVE_RINT
	return (jit_float64)rint(value1);
#else
	jit_float64 above, below;
	if(!jit_float64_is_finite(value1))
	{
		return value1;
	}
	above = jit_float64_ceil(value1);
	below = jit_float64_floor(value1);
	if((above - value1) < (jit_float64)0.5)
	{
		return above;
	}
	else if((value1 - below) < (jit_float64)0.5)
	{
		return below;
	}
	else if(jit_float64_ieee_rem(above, (jit_float64)2.0) == (jit_float64)0.0)
	{
		return above;
	}
	else
	{
		return below;
	}
#endif
}

jit_nfloat jit_nfloat_rint(jit_nfloat value1)
{
#if defined(HAVE_RINTL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(rintl(value1));
#elif defined(HAVE_RINT) && defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(rint(value1));
#else
	jit_nfloat above, below;
	if(!jit_nfloat_is_finite(value1))
	{
		return value1;
	}
	above = jit_nfloat_ceil(value1);
	below = jit_nfloat_floor(value1);
	if((above - value1) < (jit_nfloat)0.5)
	{
		return above;
	}
	else if((value1 - below) < (jit_nfloat)0.5)
	{
		return below;
	}
	else if(jit_nfloat_ieee_rem(above, (jit_nfloat)2.0) == (jit_nfloat)0.0)
	{
		return above;
	}
	else
	{
		return below;
	}
#endif
}

jit_float32 jit_float32_ceil(jit_float32 value1)
{
#if defined(HAVE_CEILF)
	return (jit_float32)(ceilf(value1));
#elif defined(HAVE_CEIL)
	return (jit_float32)(ceil(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float64 jit_float64_ceil(jit_float64 value1)
{
#if defined(HAVE_CEIL)
	return (jit_float64)(ceil(value1));
#else
	return jit_float64_nan;
#endif
}

jit_nfloat jit_nfloat_ceil(jit_nfloat value1)
{
#if defined(HAVE_CEILL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(ceill(value1));
#elif defined(HAVE_CEIL) && defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(ceil(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_float32 jit_float32_floor(jit_float32 value1)
{
#if defined(HAVE_FLOORF)
	return (jit_float32)(floorf(value1));
#elif defined(HAVE_FLOOR)
	return (jit_float32)(floor(value1));
#else
	return jit_float32_nan;
#endif
}

jit_float64 jit_float64_floor(jit_float64 value1)
{
#if defined(HAVE_FLOOR)
	return (jit_float64)(floor(value1));
#else
	return jit_float64_nan;
#endif
}

jit_nfloat jit_nfloat_floor(jit_nfloat value1)
{
#if defined(HAVE_FLOORL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(floorl(value1));
#elif defined(HAVE_FLOOR) && defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(floor(value1));
#else
	return jit_nfloat_nan;
#endif
}

jit_float32 jit_float32_trunc(jit_float32 value1)
{
#if defined(HAVE_TRUNCF)
	return (jit_float32)(truncf(value1));
#elif defined(HAVE_TRUNC)
	return (jit_float32)(trunc(value1));
#else
	if(value1 > 0)
	{
		return jit_float32_floor(value1);
	}
	else
	{
		return jit_float32_ceil(value1);
	}
#endif
}

jit_float64 jit_float64_trunc(jit_float64 value1)
{
#if defined(HAVE_TRUNC)
	return (jit_float64)(trunc(value1));
#else
	if(value1 > 0)
	{
		return jit_float64_floor(value1);
	}
	else
	{
		return jit_float64_ceil(value1);
	}
#endif
}

jit_nfloat jit_nfloat_trunc(jit_nfloat value1)
{
#if defined(HAVE_TRUNCL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(truncl(value1));
#elif defined(HAVE_TRUNC) && defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(trunc(value1));
#else
	if(value1 > 0)
	{
		return jit_nfloat_floor(value1);
	}
	else
	{
		return jit_nfloat_ceil(value1);
	}
#endif
}

/*@
 * Floatingpoint rounding operations.not covered by ieee754
 * @deftypefun jit_float32 jit_float32_round (jit_float32 @var{value1})
 * @deftypefunx jit_float64 jit_float64_round (jit_float64 @var{value1})
 * @deftypefunx jit_nfloat jit_nfloat_round (jit_nfloat @var{value1})
 * Round @var{value1} to the nearest integer.  Half-way cases
 * are rounded away from zero.
 * @end deftypefun
@*/
jit_float32 jit_float32_round(jit_float32 value1)
{
#ifdef HAVE_ROUNDF
	return (jit_float32)roundf(value1);
#else
	jit_float32 above, below;
	if(!jit_float32_is_finite(value1))
	{
		return value1;
	}
	above = jit_float32_ceil(value1);
	below = jit_float32_floor(value1);
	if((above - value1) < (jit_float32)0.5)
	{
		return above;
	}
	else if((value1 - below) < (jit_float32)0.5)
	{
		return below;
	}
	else if(above >= (jit_float32)0.0)
	{
		return above;
	}
	else
	{
		return below;
	}
#endif
}

jit_float64 jit_float64_round(jit_float64 value1)
{
#ifdef HAVE_ROUND
	return (jit_float64)round(value1);
#else
	jit_float64 above, below;
	if(!jit_float64_is_finite(value1))
	{
		return value1;
	}
	above = jit_float64_ceil(value1);
	below = jit_float64_floor(value1);
	if((above - value1) < (jit_float64)0.5)
	{
		return above;
	}
	else if((value1 - below) < (jit_float64)0.5)
	{
		return below;
	}
	else if(above >= (jit_float64)0.0)
	{
		return above;
	}
	else
	{
		return below;
	}
#endif
}

jit_nfloat jit_nfloat_round(jit_nfloat value1)
{
#if defined(HAVE_ROUNDL) && !defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(roundl(value1));
#elif defined(HAVE_ROUND) && defined(JIT_NFLOAT_IS_DOUBLE)
	return (jit_nfloat)(round(value1));
#else
	jit_nfloat above, below;
	if(!jit_nfloat_is_finite(value1))
	{
		return value1;
	}
	above = jit_nfloat_ceil(value1);
	below = jit_nfloat_floor(value1);
	if((above - value1) < (jit_nfloat)0.5)
	{
		return above;
	}
	else if((value1 - below) < (jit_nfloat)0.5)
	{
		return below;
	}
	else if(above >= (jit_nfloat)0.0)
	{
		return above;
	}
	else
	{
		return below;
	}
#endif
}

/*@
 * @deftypefun jit_int jit_int_to_sbyte (jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_ubyte (jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_short (jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_ushort (jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_int (jit_int @var{value})
 * @deftypefunx jit_uint jit_int_to_uint (jit_int @var{value})
 * @deftypefunx jit_long jit_int_to_long (jit_int @var{value})
 * @deftypefunx jit_ulong jit_int_to_ulong (jit_int @var{value})
 * @deftypefunx jit_int jit_uint_to_int (jit_uint @var{value})
 * @deftypefunx jit_uint jit_uint_to_uint (jit_uint @var{value})
 * @deftypefunx jit_long jit_uint_to_long (jit_uint @var{value})
 * @deftypefunx jit_ulong jit_uint_to_ulong (jit_uint @var{value})
 * @deftypefunx jit_int jit_long_to_int (jit_long @var{value})
 * @deftypefunx jit_uint jit_long_to_uint (jit_long @var{value})
 * @deftypefunx jit_long jit_long_to_long (jit_long @var{value})
 * @deftypefunx jit_ulong jit_long_to_ulong (jit_long @var{value})
 * @deftypefunx jit_int jit_ulong_to_int (jit_ulong @var{value})
 * @deftypefunx jit_uint jit_ulong_to_uint (jit_ulong @var{value})
 * @deftypefunx jit_long jit_ulong_to_long (jit_ulong @var{value})
 * @deftypefunx jit_ulong jit_ulong_to_ulong (jit_ulong @var{value})
 * Convert between integer types.
 * @end deftypefun
@*/
jit_int jit_int_to_sbyte(jit_int value)
{
	return (jit_int)(jit_sbyte)value;
}

jit_int jit_int_to_ubyte(jit_int value)
{
	return (jit_int)(jit_ubyte)value;
}

jit_int jit_int_to_short(jit_int value)
{
	return (jit_int)(jit_short)value;
}

jit_int jit_int_to_ushort(jit_int value)
{
	return (jit_int)(jit_ushort)value;
}

jit_int jit_int_to_int(jit_int value)
{
	return value;
}

jit_uint jit_int_to_uint(jit_int value)
{
	return (jit_uint)value;
}

jit_long jit_int_to_long(jit_int value)
{
	return (jit_long)value;
}

jit_ulong jit_int_to_ulong(jit_int value)
{
	return (jit_ulong)(jit_long)value;
}

jit_int jit_uint_to_int(jit_uint value)
{
	return (jit_int)value;
}

jit_uint jit_uint_to_uint(jit_uint value)
{
	return value;
}

jit_long jit_uint_to_long(jit_uint value)
{
	return (jit_long)value;
}

jit_ulong jit_uint_to_ulong(jit_uint value)
{
	return (jit_long)value;
}

jit_int jit_long_to_int(jit_long value)
{
	return (jit_int)value;
}

jit_uint jit_long_to_uint(jit_long value)
{
	return (jit_uint)value;
}

jit_long jit_long_to_long(jit_long value)
{
	return value;
}

jit_ulong jit_long_to_ulong(jit_long value)
{
	return (jit_ulong)value;
}

jit_int jit_ulong_to_int(jit_ulong value)
{
	return (jit_int)value;
}

jit_uint jit_ulong_to_uint(jit_ulong value)
{
	return (jit_uint)value;
}

jit_long jit_ulong_to_long(jit_ulong value)
{
	return (jit_long)value;
}

jit_ulong jit_ulong_to_ulong(jit_ulong value)
{
	return value;
}

/*@
 * @deftypefun jit_int jit_int_to_sbyte_ovf (jit_int *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_ubyte_ovf (jit_int *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_short_ovf (jit_int *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_ushort_ovf (jit_int *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_int_ovf (jit_int *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_uint_ovf (jit_uint *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_long_ovf (jit_long *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_int_to_ulong_ovf (jit_ulong *@var{result}, jit_int @var{value})
 * @deftypefunx jit_int jit_uint_to_int_ovf (jit_int *@var{result}, jit_uint @var{value})
 * @deftypefunx jit_int jit_uint_to_uint_ovf (jit_uint *@var{result}, jit_uint @var{value})
 * @deftypefunx jit_int jit_uint_to_long_ovf (jit_long *@var{result}, jit_uint @var{value})
 * @deftypefunx jit_int jit_uint_to_ulong_ovf (jit_ulong *@var{result}, jit_uint @var{value})
 * @deftypefunx jit_int jit_long_to_int_ovf (jit_int *@var{result}, jit_long @var{value})
 * @deftypefunx jit_int jit_long_to_uint_ovf (jit_uint *@var{result}, jit_long @var{value})
 * @deftypefunx jit_int jit_long_to_long_ovf (jit_long *@var{result}, jit_long @var{value})
 * @deftypefunx jit_int jit_long_to_ulong_ovf (jit_ulong *@var{result}, jit_long @var{value})
 * @deftypefunx jit_int jit_ulong_to_int_ovf (jit_int *@var{result}, jit_ulong @var{value})
 * @deftypefunx jit_int jit_ulong_to_uint_ovf (jit_uint *@var{result}, jit_ulong @var{value})
 * @deftypefunx jit_int jit_ulong_to_long_ovf (jit_long *@var{result}, jit_ulong @var{value})
 * @deftypefunx jit_int jit_ulong_to_ulong_ovf (jit_ulong *@var{result}, jit_ulong @var{value})
 * Convert between integer types with overflow detection.
 * @end deftypefun
@*/
jit_int jit_int_to_sbyte_ovf(jit_int *result, jit_int value)
{
	return ((*result = (jit_int)(jit_sbyte)value) == value);
}

jit_int jit_int_to_ubyte_ovf(jit_int *result, jit_int value)
{
	return ((*result = (jit_int)(jit_ubyte)value) == value);
}

jit_int jit_int_to_short_ovf(jit_int *result, jit_int value)
{
	return ((*result = (jit_int)(jit_short)value) == value);
}

jit_int jit_int_to_ushort_ovf(jit_int *result, jit_int value)
{
	return ((*result = (jit_int)(jit_ushort)value) == value);
}

jit_int jit_int_to_int_ovf(jit_int *result, jit_int value)
{
	*result = value;
	return 1;
}

jit_int jit_int_to_uint_ovf(jit_uint *result, jit_int value)
{
	*result = (jit_uint)value;
	return (value >= 0);
}

jit_int jit_int_to_long_ovf(jit_long *result, jit_int value)
{
	*result = (jit_long)value;
	return 1;
}

jit_int jit_int_to_ulong_ovf(jit_ulong *result, jit_int value)
{
	*result = (jit_ulong)(jit_long)value;
	return (value >= 0);
}

jit_int jit_uint_to_int_ovf(jit_int *result, jit_uint value)
{
	*result = (jit_int)value;
	return (*result >= 0);
}

jit_int jit_uint_to_uint_ovf(jit_uint *result, jit_uint value)
{
	*result = value;
	return 1;
}

jit_int jit_uint_to_long_ovf(jit_long *result, jit_uint value)
{
	*result = (jit_long)value;
	return 1;
}

jit_int jit_uint_to_ulong_ovf(jit_ulong *result, jit_uint value)
{
	*result = (jit_ulong)value;
	return 1;
}

jit_int jit_long_to_int_ovf(jit_int *result, jit_long value)
{
	*result = (jit_int)value;
	return (((jit_long)(*result)) == value);
}

jit_int jit_long_to_uint_ovf(jit_uint *result, jit_long value)
{
	*result = (jit_uint)value;
	return (((jit_long)(*result)) == value);
}

jit_int jit_long_to_long_ovf(jit_long *result, jit_long value)
{
	*result = value;
	return 1;
}

jit_int jit_long_to_ulong_ovf(jit_ulong *result, jit_long value)
{
	*result = (jit_ulong)value;
	return (value >= 0);
}

jit_int jit_ulong_to_int_ovf(jit_int *result, jit_ulong value)
{
	*result = (jit_int)value;
	return (value <= (jit_ulong)(jit_long)jit_max_int);
}

jit_int jit_ulong_to_uint_ovf(jit_uint *result, jit_ulong value)
{
	*result = (jit_uint)value;
	return (value <= (jit_ulong)jit_max_uint);
}

jit_int jit_ulong_to_long_ovf(jit_long *result, jit_ulong value)
{
	*result = (jit_long)value;
	return (*result >= 0);
}

jit_int jit_ulong_to_ulong_ovf(jit_ulong *result, jit_ulong value)
{
	*result = value;
	return 1;
}

/*@
 * @deftypefun jit_int jit_float32_to_int (jit_float32 @var{value})
 * @deftypefunx jit_uint jit_float32_to_uint (jit_float32 @var{value})
 * @deftypefunx jit_long jit_float32_to_long (jit_float32 @var{value})
 * @deftypefunx jit_ulong jit_float32_to_ulong (jit_float32 @var{value})
 * Convert a 32-bit floating-point value into an integer.
 * @end deftypefun
@*/
jit_int jit_float32_to_int(jit_float32 value)
{
	return (jit_int)value;
}

jit_uint jit_float32_to_uint(jit_float32 value)
{
	return (jit_uint)value;
}

jit_long jit_float32_to_long(jit_float32 value)
{
	return (jit_long)value;
}

jit_ulong jit_float32_to_ulong(jit_float32 value)
{
	/* Some platforms cannot perform the conversion directly,
	   so we need to do it in stages */
	if(jit_float32_is_finite(value))
	{
		if(value >= (jit_float32)0.0)
		{
			if(value < (jit_float32)9223372036854775808.0)
			{
				return (jit_ulong)(jit_long)value;
			}
			else if(value < (jit_float32)18446744073709551616.0)
			{
				jit_long temp = (jit_long)(value - 9223372036854775808.0);
				return (jit_ulong)(temp - jit_min_long);
			}
			else
			{
				return jit_max_ulong;
			}
		}
		else
		{
			return 0;
		}
	}
	else if(jit_float32_is_nan(value))
	{
		return 0;
	}
	else if(value < (jit_float32)0.0)
	{
		return 0;
	}
	else
	{
		return jit_max_ulong;
	}
}

/*@
 * @deftypefun jit_int jit_float32_to_int_ovf (jit_int *@var{result}, jit_float32 @var{value})
 * @deftypefunx jit_uint jit_float32_to_uint_ovf (jit_uint *@var{result}, jit_float32 @var{value})
 * @deftypefunx jit_long jit_float32_to_long_ovf (jit_long *@var{result}, jit_float32 @var{value})
 * @deftypefunx jit_ulong jit_float32_to_ulong_ovf (jit_ulong *@var{result}, jit_float32 @var{value})
 * Convert a 32-bit floating-point value into an integer,
 * with overflow detection.  Returns @code{JIT_RESULT_OK} if the conversion
 * was successful or @code{JIT_RESULT_OVERFLOW} if an overflow occurred.
 * @end deftypefun
@*/
jit_int jit_float32_to_int_ovf(jit_int *result, jit_float32 value)
{
	if(jit_float32_is_finite(value))
	{
		if(value > (jit_float32)(-2147483649.0) &&
		   value < (jit_float32)2147483648.0)
		{
			*result = jit_float32_to_int(value);
			return 1;
		}
	}
	return 0;
}

jit_int jit_float32_to_uint_ovf(jit_uint *result, jit_float32 value)
{
	if(jit_float32_is_finite(value))
	{
		if(value >= (jit_float32)0.0 &&
		   value < (jit_float32)4294967296.0)
		{
			*result = jit_float32_to_uint(value);
			return 1;
		}
	}
	return 0;
}

jit_int jit_float32_to_long_ovf(jit_long *result, jit_float32 value)
{
	if(jit_float32_is_finite(value))
	{
		if(value >= (jit_float32)-9223372036854775808.0 &&
		   value < (jit_float32)9223372036854775808.0)
		{
			*result = jit_float32_to_long(value);
			return 1;
		}
		else if(value < (jit_float32)0.0)
		{
			/* Account for the range -9223372036854775809.0 to
			   -9223372036854775808.0, which may get rounded
			   off if we aren't careful */
			value += (jit_float32)9223372036854775808.0;
			if(value > (jit_float32)(-1.0))
			{
				*result = jit_min_long;
				return 1;
			}
		}
	}
	return 0;
}

jit_int jit_float32_to_ulong_ovf(jit_ulong *result, jit_float32 value)
{
	if(jit_float32_is_finite(value))
	{
		if(value >= (jit_float32)0.0)
		{
			if(value < (jit_float32)18446744073709551616.0)
			{
				*result = jit_float32_to_ulong(value);
				return 1;
			}
		}
	}
	return 0;
}

/*@
 * @deftypefun jit_int jit_float64_to_int (jit_float64 @var{value})
 * @deftypefunx jit_uint jit_float64_to_uint (jit_float64 @var{value})
 * @deftypefunx jit_long jit_float64_to_long (jit_float64 @var{value})
 * @deftypefunx jit_ulong jit_float64_to_ulong (jit_float64 @var{value})
 * Convert a 64-bit floating-point value into an integer.
 * @end deftypefun
@*/
jit_int jit_float64_to_int(jit_float64 value)
{
	return (jit_int)value;
}

jit_uint jit_float64_to_uint(jit_float64 value)
{
	return (jit_uint)value;
}

jit_long jit_float64_to_long(jit_float64 value)
{
	return (jit_long)value;
}

jit_ulong jit_float64_to_ulong(jit_float64 value)
{
	/* Some platforms cannot perform the conversion directly,
	   so we need to do it in stages */
	if(jit_float64_is_finite(value))
	{
		if(value >= (jit_float64)0.0)
		{
			if(value < (jit_float64)9223372036854775808.0)
			{
				return (jit_ulong)(jit_long)value;
			}
			else if(value < (jit_float64)18446744073709551616.0)
			{
				jit_long temp = (jit_long)(value - 9223372036854775808.0);
				return (jit_ulong)(temp - jit_min_long);
			}
			else
			{
				return jit_max_ulong;
			}
		}
		else
		{
			return 0;
		}
	}
	else if(jit_float64_is_nan(value))
	{
		return 0;
	}
	else if(value < (jit_float64)0.0)
	{
		return 0;
	}
	else
	{
		return jit_max_ulong;
	}
}

/*@
 * @deftypefun jit_int jit_float64_to_int_ovf (jit_int *@var{result}, jit_float64 @var{value})
 * @deftypefunx jit_uint jit_float64_to_uint_ovf (jit_uint *@var{result}, jit_float64 @var{value})
 * @deftypefunx jit_long jit_float64_to_long_ovf (jit_long *@var{result}, jit_float64 @var{value})
 * @deftypefunx jit_ulong jit_float64_to_ulong_ovf (jit_ulong *@var{result}, jit_float64 @var{value})
 * Convert a 64-bit floating-point value into an integer,
 * with overflow detection.  Returns @code{JIT_RESULT_OK} if the conversion
 * was successful or @code{JIT_RESULT_OVERFLOW} if an overflow occurred.
 * @end deftypefun
@*/
jit_int jit_float64_to_int_ovf(jit_int *result, jit_float64 value)
{
	if(jit_float64_is_finite(value))
	{
		if(value > (jit_float64)(-2147483649.0) &&
		   value < (jit_float64)2147483648.0)
		{
			*result = jit_float64_to_int(value);
			return 1;
		}
	}
	return 0;
}

jit_int jit_float64_to_uint_ovf(jit_uint *result, jit_float64 value)
{
	if(jit_float64_is_finite(value))
	{
		if(value >= (jit_float64)0.0 &&
		   value < (jit_float64)4294967296.0)
		{
			*result = jit_float64_to_uint(value);
			return 1;
		}
	}
	return 0;
}

jit_int jit_float64_to_long_ovf(jit_long *result, jit_float64 value)
{
	if(jit_float64_is_finite(value))
	{
		if(value >= (jit_float64)-9223372036854775808.0 &&
		   value < (jit_float64)9223372036854775808.0)
		{
			*result = jit_float64_to_long(value);
			return 1;
		}
		else if(value < (jit_float64)0.0)
		{
			/* Account for the range -9223372036854775809.0 to
			   -9223372036854775808.0, which may get rounded
			   off if we aren't careful */
			value += (jit_float64)9223372036854775808.0;
			if(value > (jit_float64)(-1.0))
			{
				*result = jit_min_long;
				return 1;
			}
		}
	}
	return 0;
}

jit_int jit_float64_to_ulong_ovf(jit_ulong *result, jit_float64 value)
{
	if(jit_float64_is_finite(value))
	{
		if(value >= (jit_float64)0.0)
		{
			if(value < (jit_float64)18446744073709551616.0)
			{
				*result = jit_float64_to_ulong(value);
				return 1;
			}
		}
	}
	return 0;
}

/*@
 * @deftypefun jit_int jit_nfloat_to_int (jit_nfloat @var{value})
 * @deftypefunx jit_uint jit_nfloat_to_uint (jit_nfloat @var{value})
 * @deftypefunx jit_long jit_nfloat_to_long (jit_nfloat @var{value})
 * @deftypefunx jit_ulong jit_nfloat_to_ulong (jit_nfloat @var{value})
 * Convert a native floating-point value into an integer.
 * @end deftypefun
@*/
jit_int jit_nfloat_to_int(jit_nfloat value)
{
	return (jit_int)value;
}

jit_uint jit_nfloat_to_uint(jit_nfloat value)
{
	return (jit_uint)value;
}

jit_long jit_nfloat_to_long(jit_nfloat value)
{
	return (jit_long)value;
}

jit_ulong jit_nfloat_to_ulong(jit_nfloat value)
{
	/* Some platforms cannot perform the conversion directly,
	   so we need to do it in stages */
	if(jit_nfloat_is_finite(value))
	{
		if(value >= (jit_nfloat)0.0)
		{
			if(value < (jit_nfloat)9223372036854775808.0)
			{
				return (jit_ulong)(jit_long)value;
			}
			else if(value < (jit_nfloat)18446744073709551616.0)
			{
				jit_long temp = (jit_long)(value - 9223372036854775808.0);
				return (jit_ulong)(temp - jit_min_long);
			}
			else
			{
				return jit_max_ulong;
			}
		}
		else
		{
			return 0;
		}
	}
	else if(jit_nfloat_is_nan(value))
	{
		return 0;
	}
	else if(value < (jit_nfloat)0.0)
	{
		return 0;
	}
	else
	{
		return jit_max_ulong;
	}
}

/*@
 * @deftypefun jit_int jit_nfloat_to_int_ovf (jit_int *@var{result}, jit_nfloat @var{value})
 * @deftypefunx jit_uint jit_nfloat_to_uint_ovf (jit_uint *@var{result}, jit_nfloat @var{value})
 * @deftypefunx jit_long jit_nfloat_to_long_ovf (jit_long *@var{result}, jit_nfloat @var{value})
 * @deftypefunx jit_ulong jit_nfloat_to_ulong_ovf (jit_ulong *@var{result}, jit_nfloat @var{value})
 * Convert a native floating-point value into an integer,
 * with overflow detection.  Returns @code{JIT_RESULT_OK} if the conversion
 * was successful or @code{JIT_RESULT_OVERFLOW} if an overflow occurred.
 * @end deftypefun
@*/
jit_int jit_nfloat_to_int_ovf(jit_int *result, jit_nfloat value)
{
	if(jit_nfloat_is_finite(value))
	{
		if(value > (jit_nfloat)(-2147483649.0) &&
		   value < (jit_nfloat)2147483648.0)
		{
			*result = jit_nfloat_to_int(value);
			return 1;
		}
	}
	return 0;
}

jit_int jit_nfloat_to_uint_ovf(jit_uint *result, jit_nfloat value)
{
	if(jit_nfloat_is_finite(value))
	{
		if(value >= (jit_nfloat)0.0 &&
		   value < (jit_nfloat)4294967296.0)
		{
			*result = jit_nfloat_to_uint(value);
			return 1;
		}
	}
	return 0;
}

jit_int jit_nfloat_to_long_ovf(jit_long *result, jit_nfloat value)
{
	if(jit_nfloat_is_finite(value))
	{
		if(value >= (jit_nfloat)-9223372036854775808.0 &&
		   value < (jit_nfloat)9223372036854775808.0)
		{
			*result = jit_nfloat_to_long(value);
			return 1;
		}
		else if(value < (jit_nfloat)0.0)
		{
			/* Account for the range -9223372036854775809.0 to
			   -9223372036854775808.0, which may get rounded
			   off if we aren't careful */
			value += (jit_nfloat)9223372036854775808.0;
			if(value > (jit_nfloat)(-1.0))
			{
				*result = jit_min_long;
				return 1;
			}
		}
	}
	return 0;
}

jit_int jit_nfloat_to_ulong_ovf(jit_ulong *result, jit_nfloat value)
{
	if(jit_nfloat_is_finite(value))
	{
		if(value >= (jit_nfloat)0.0)
		{
			if(value < (jit_nfloat)18446744073709551616.0)
			{
				*result = jit_nfloat_to_ulong(value);
				return 1;
			}
		}
	}
	return 0;
}

/*@
 * @deftypefun jit_float32 jit_int_to_float32 (jit_int @var{value})
 * @deftypefunx jit_float32 jit_uint_to_float32 (jit_uint @var{value})
 * @deftypefunx jit_float32 jit_long_to_float32 (jit_long @var{value})
 * @deftypefunx jit_float32 jit_ulong_to_float32 (jit_ulong @var{value})
 * Convert an integer into 32-bit floating-point value.
 * @end deftypefun
@*/
jit_float32 jit_int_to_float32(jit_int value)
{
	return (jit_float32)value;
}

jit_float32 jit_uint_to_float32(jit_uint value)
{
	return (jit_float32)value;
}

jit_float32 jit_long_to_float32(jit_long value)
{
	return (jit_float32)value;
}

jit_float32 jit_ulong_to_float32(jit_ulong value)
{
	/* Some platforms cannot perform the conversion directly,
	   so we need to do it in stages */
	if(value < (((jit_ulong)1) << 63))
	{
		return (jit_float32)(jit_long)value;
	}
	else
	{
		return (jit_float32)((jit_long)value) +
					(jit_float32)18446744073709551616.0;
	}
}

/*@
 * @deftypefun jit_float64 jit_int_to_float64 (jit_int @var{value})
 * @deftypefunx jit_float64 jit_uint_to_float64 (jit_uint @var{value})
 * @deftypefunx jit_float64 jit_long_to_float64 (jit_long @var{value})
 * @deftypefunx jit_float64 jit_ulong_to_float64 (jit_ulong @var{value})
 * Convert an integer into 64-bit floating-point value.
 * @end deftypefun
@*/
jit_float64 jit_int_to_float64(jit_int value)
{
	return (jit_float64)value;
}

jit_float64 jit_uint_to_float64(jit_uint value)
{
	return (jit_float64)value;
}

jit_float64 jit_long_to_float64(jit_long value)
{
	return (jit_float64)value;
}

jit_float64 jit_ulong_to_float64(jit_ulong value)
{
	/* Some platforms cannot perform the conversion directly,
	   so we need to do it in stages */
	if(value < (((jit_ulong)1) << 63))
	{
		return (jit_float64)(jit_long)value;
	}
	else
	{
		return (jit_float64)((jit_long)value) +
					(jit_float64)18446744073709551616.0;
	}
}

/*@
 * @deftypefun jit_nfloat jit_int_to_nfloat (jit_int @var{value})
 * @deftypefunx jit_nfloat jit_uint_to_nfloat (jit_uint @var{value})
 * @deftypefunx jit_nfloat jit_long_to_nfloat (jit_long @var{value})
 * @deftypefunx jit_nfloat jit_ulong_to_nfloat (jit_ulong @var{value})
 * Convert an integer into native floating-point value.
 * @end deftypefun
@*/
jit_nfloat jit_int_to_nfloat(jit_int value)
{
	return (jit_nfloat)value;
}

jit_nfloat jit_uint_to_nfloat(jit_uint value)
{
	return (jit_nfloat)value;
}

jit_nfloat jit_long_to_nfloat(jit_long value)
{
	return (jit_nfloat)value;
}

jit_nfloat jit_ulong_to_nfloat(jit_ulong value)
{
	/* Some platforms cannot perform the conversion directly,
	   so we need to do it in stages */
	if(value < (((jit_ulong)1) << 63))
	{
		return (jit_nfloat)(jit_long)value;
	}
	else
	{
		return (jit_nfloat)((jit_long)value) +
					(jit_nfloat)18446744073709551616.0;
	}
}

/*@
 * @deftypefun jit_float64 jit_float32_to_float64 (jit_float32 @var{value})
 * @deftypefunx jit_nfloat jit_float32_to_nfloat (jit_float32 @var{value})
 * @deftypefunx jit_float32 jit_float64_to_float32 (jit_float64 @var{value})
 * @deftypefunx jit_nfloat jit_float64_to_nfloat (jit_float64 @var{value})
 * @deftypefunx jit_float32 jit_nfloat_to_float32 (jit_nfloat @var{value})
 * @deftypefunx jit_float64 jit_nfloat_to_float64 (jit_nfloat @var{value})
 * Convert between floating-point types.
 * @end deftypefun
@*/
jit_float64 jit_float32_to_float64(jit_float32 value)
{
	return (jit_float64)value;
}

jit_nfloat jit_float32_to_nfloat(jit_float32 value)
{
	return (jit_nfloat)value;
}

jit_float32 jit_float64_to_float32(jit_float64 value)
{
	return (jit_float32)value;
}

jit_nfloat jit_float64_to_nfloat(jit_float64 value)
{
	return (jit_nfloat)value;
}

jit_float32 jit_nfloat_to_float32(jit_nfloat value)
{
	return (jit_float32)value;
}

jit_float64 jit_nfloat_to_float64(jit_nfloat value)
{
	return (jit_float64)value;
}
