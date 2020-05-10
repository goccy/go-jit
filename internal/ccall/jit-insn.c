/*
 * jit-insn.c - Functions for manipulating instructions.
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
#include "jit-rules.h"
#include "jit-setjmp.h"
#if HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_ALLOCA_H
# include <alloca.h>
#endif
#ifdef JIT_WIN32_PLATFORM
# include <malloc.h>
# ifndef alloca
#  define alloca _alloca
# endif
#endif

/*@

@cindex jit-insn.h

@*/

/*
 * Opcode description blocks.  These describe the alternative opcodes
 * and intrinsic functions to use for various kinds of arguments.
 */
typedef struct
{
	unsigned short		ioper;		/* Primary operator for "int" */
	unsigned short		iuoper;		/* Primary operator for "uint" */
	unsigned short		loper;		/* Primary operator for "long" */
	unsigned short		luoper;		/* Primary operator for "ulong" */
	unsigned short		foper;		/* Primary operator for "float32" */
	unsigned short		doper;		/* Primary operator for "float64" */
	unsigned short		nfoper;		/* Primary operator for "nfloat" */

	void			*ifunc;		/* Function for "int" */
	const char		*iname;		/* Intrinsic name for "int" */
	const jit_intrinsic_descr_t *idesc;	/* Descriptor for "int" */

	void			*iufunc;       	/* Function for "uint" */
	const char		*iuname;	/* Intrinsic name for "uint" */
	const jit_intrinsic_descr_t *iudesc;	/* Descriptor for "uint" */

	void			*lfunc;		/* Function for "long" */
	const char		*lname;		/* Intrinsic name for "long" */
	const jit_intrinsic_descr_t *ldesc;	/* Descriptor for "long" */

	void			*lufunc;	/* Function for "ulong" */
	const char		*luname;	/* Intrinsic name for "ulong" */
	const jit_intrinsic_descr_t *ludesc;	/* Descriptor for "ulong" */

	void			*ffunc;		/* Function for "float32" */
	const char		*fname;		/* Intrinsic name for "float32" */
	const jit_intrinsic_descr_t *fdesc;	/* Descriptor for "float32" */

	void			*dfunc;		/* Function for "float64" */
	const char		*dname;		/* Intrinsic name for "float64" */
	const jit_intrinsic_descr_t *ddesc;	/* Descriptor for "float64" */

	void			*nffunc;	/* Function for "nfloat" */
	const char		*nfname;	/* Intrinsic name for "nfloat" */
	const jit_intrinsic_descr_t *nfdesc;	/* Descriptor for "nfloat" */

} jit_opcode_descr;

#define	jit_intrinsic(name, descr)	(void *)name, #name, &descr
#define	jit_no_intrinsic		0, 0, 0

/*
 * Some common intrinsic descriptors that are used in this file.
 */
static jit_intrinsic_descr_t const descr_i_ii = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_int_def
};
static jit_intrinsic_descr_t const descr_e_pi_ii = {
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_int_def
};
static jit_intrinsic_descr_t const descr_i_iI = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_uint_def
};
static jit_intrinsic_descr_t const descr_i_i = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_int_def,
	0
};
static jit_intrinsic_descr_t const descr_I_II = {
	(jit_type_t) &_jit_type_uint_def,
	0,
	(jit_type_t) &_jit_type_uint_def,
	(jit_type_t) &_jit_type_uint_def
};
static jit_intrinsic_descr_t const descr_e_pI_II = {
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_uint_def,
	(jit_type_t) &_jit_type_uint_def,
	(jit_type_t) &_jit_type_uint_def
};
static jit_intrinsic_descr_t const descr_I_I = {
	(jit_type_t) &_jit_type_uint_def,
	0,
	(jit_type_t) &_jit_type_uint_def,
	0
};
static jit_intrinsic_descr_t const descr_i_II = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_uint_def,
	(jit_type_t) &_jit_type_uint_def
};
static jit_intrinsic_descr_t const descr_l_ll = {
	(jit_type_t) &_jit_type_long_def,
	0,
	(jit_type_t) &_jit_type_long_def,
	(jit_type_t) &_jit_type_long_def
};
static jit_intrinsic_descr_t const descr_e_pl_ll = {
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_long_def,
	(jit_type_t) &_jit_type_long_def,
	(jit_type_t) &_jit_type_long_def
};
static jit_intrinsic_descr_t const descr_l_lI = {
	(jit_type_t) &_jit_type_long_def,
	0,
	(jit_type_t) &_jit_type_long_def,
	(jit_type_t) &_jit_type_uint_def
};
static jit_intrinsic_descr_t const descr_l_l = {
	(jit_type_t) &_jit_type_long_def,
	0,
	(jit_type_t) &_jit_type_long_def,
	0
};
static jit_intrinsic_descr_t const descr_i_ll = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_long_def,
	(jit_type_t) &_jit_type_long_def
};
static jit_intrinsic_descr_t const descr_i_l = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_long_def,
	0
};
static jit_intrinsic_descr_t const descr_L_LL = {
	(jit_type_t) &_jit_type_ulong_def,
	0,
	(jit_type_t) &_jit_type_ulong_def,
	(jit_type_t) &_jit_type_ulong_def
};
static jit_intrinsic_descr_t const descr_e_pL_LL = {
	(jit_type_t) &_jit_type_int_def,
	(jit_type_t) &_jit_type_ulong_def,
	(jit_type_t) &_jit_type_ulong_def,
	(jit_type_t) &_jit_type_ulong_def
};
static jit_intrinsic_descr_t const descr_L_LI = {
	(jit_type_t) &_jit_type_ulong_def,
	0,
	(jit_type_t) &_jit_type_ulong_def,
	(jit_type_t) &_jit_type_uint_def
};
static jit_intrinsic_descr_t const descr_L_L = {
	(jit_type_t) &_jit_type_ulong_def,
	0,
	(jit_type_t) &_jit_type_ulong_def,
	0
};
static jit_intrinsic_descr_t const descr_i_LL = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_ulong_def,
	(jit_type_t) &_jit_type_ulong_def
};
static jit_intrinsic_descr_t const descr_f_ff = {
	(jit_type_t) &_jit_type_float32_def,
	0,
	(jit_type_t) &_jit_type_float32_def,
	(jit_type_t) &_jit_type_float32_def
};
static jit_intrinsic_descr_t const descr_f_f = {
	(jit_type_t) &_jit_type_float32_def,
	0,
	(jit_type_t) &_jit_type_float32_def,
	0
};
static jit_intrinsic_descr_t const descr_i_ff = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_float32_def,
	(jit_type_t) &_jit_type_float32_def
};
static jit_intrinsic_descr_t const descr_i_f = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_float32_def,
	0
};
static jit_intrinsic_descr_t const descr_d_dd = {
	(jit_type_t) &_jit_type_float64_def,
	0,
	(jit_type_t) &_jit_type_float64_def,
	(jit_type_t) &_jit_type_float64_def
};
static jit_intrinsic_descr_t const descr_d_d = {
	(jit_type_t) &_jit_type_float64_def,
	0,
	(jit_type_t) &_jit_type_float64_def,
	0
};
static jit_intrinsic_descr_t const descr_i_dd = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_float64_def,
	(jit_type_t) &_jit_type_float64_def
};
static jit_intrinsic_descr_t const descr_i_d = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_float64_def,
	0
};
static jit_intrinsic_descr_t const descr_D_DD = {
	(jit_type_t) &_jit_type_nfloat_def,
	0,
	(jit_type_t) &_jit_type_nfloat_def,
	(jit_type_t) &_jit_type_nfloat_def
};
static jit_intrinsic_descr_t const descr_D_D = {
	(jit_type_t) &_jit_type_nfloat_def,
	0,
	(jit_type_t) &_jit_type_nfloat_def,
	0
};
static jit_intrinsic_descr_t const descr_i_DD = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_nfloat_def,
	(jit_type_t) &_jit_type_nfloat_def
};
static jit_intrinsic_descr_t const descr_i_D = {
	(jit_type_t) &_jit_type_int_def,
	0,
	(jit_type_t) &_jit_type_nfloat_def,
	0
};

/*
 * Apply a unary operator.
 */
static jit_value_t
apply_unary(jit_function_t func, int oper, jit_value_t value, jit_type_t type)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_value_t dest = jit_value_create(func, type);
	if(!dest)
	{
		return 0;
	}
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;
	insn->dest = dest;
	insn->value1 = value;
	jit_value_ref(func, value);

	return dest;
}

/*
 * Apply a binary operator.
 */
static jit_value_t
apply_binary(jit_function_t func, int oper, jit_value_t value1, jit_value_t value2,
	     jit_type_t type)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_value_t dest = jit_value_create(func, type);
	if(!dest)
	{
		return 0;
	}
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;
	insn->dest = dest;
	insn->value1 = value1;
	jit_value_ref(func, value1);
	insn->value2 = value2;
	jit_value_ref(func, value2);

	return dest;
}

/*
 * Apply a ternary operator.
 */
static int
apply_ternary(jit_function_t func, int oper, jit_value_t value1, jit_value_t value2,
	      jit_value_t value3)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;
	insn->flags = JIT_INSN_DEST_IS_VALUE;
	insn->dest = value1;
	jit_value_ref(func, value1);
	insn->value1 = value2;
	jit_value_ref(func, value2);
	insn->value2 = value3;
	jit_value_ref(func, value3);

	return 1;
}

/*
 * Create a note instruction, which doesn't have a result.
 */
static int
create_note(jit_function_t func, int oper, jit_value_t value1, jit_value_t value2)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;
	insn->value1 = value1;
	jit_value_ref(func, value1);
	insn->value2 = value2;
	jit_value_ref(func, value2);

	return 1;
}

/*
 * Create a unary note instruction, which doesn't have a result.
 */
static int
create_unary_note(jit_function_t func, int oper, jit_value_t value)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;
	insn->value1 = value;
	jit_value_ref(func, value);

	return 1;
}

/*
 * Create a note instruction with no arguments, which doesn't have a result.
 */
static int
create_noarg_note(jit_function_t func, int oper)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;

	return 1;
}

/*
 * Create a note instruction with only a destination.
 */
static jit_value_t
create_dest_note(jit_function_t func, int oper, jit_type_t type)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_value_t dest = jit_value_create(func, type);
	if(!dest)
	{
		return 0;
	}
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) oper;
	insn->dest = dest;

	return dest;
}

/*
 * Get the common type to use for a binary operator.
 */
static jit_type_t
common_binary(jit_type_t type1, jit_type_t type2, int int_only, int float_only)
{
	type1 = jit_type_promote_int(jit_type_normalize(type1));
	type2 = jit_type_promote_int(jit_type_normalize(type2));
	if(!float_only)
	{
		if(type1 == jit_type_int)
		{
			if(type2 == jit_type_int || type2 == jit_type_uint)
			{
				return jit_type_int;
			}
			else if(type2 == jit_type_long || type2 == jit_type_ulong)
			{
				return jit_type_long;
			}
		}
		else if(type1 == jit_type_uint)
		{
			if(type2 == jit_type_int || type2 == jit_type_uint ||
			   type2 == jit_type_long || type2 == jit_type_ulong)
			{
				return type2;
			}
		}
		else if(type1 == jit_type_long)
		{
			if(type2 == jit_type_int || type2 == jit_type_uint ||
			   type2 == jit_type_long || type2 == jit_type_ulong)
			{
				return jit_type_long;
			}
		}
		else if(type1 == jit_type_ulong)
		{
			if(type2 == jit_type_int || type2 == jit_type_long)
			{
				return jit_type_long;
			}
			else if(type2 == jit_type_uint || type2 == jit_type_ulong)
			{
				return jit_type_ulong;
			}
		}
		if(int_only)
		{
			return jit_type_long;
		}
	}
	if(type1 == jit_type_nfloat || type2 == jit_type_nfloat)
	{
		return jit_type_nfloat;
	}
	else if(type1 == jit_type_float64 || type2 == jit_type_float64)
	{
		return jit_type_float64;
	}
	else if(type1 == jit_type_float32 || type2 == jit_type_float32)
	{
		return jit_type_float32;
	}
	else
	{
		/* Probably integer arguments when "float_only" is set */
		return jit_type_nfloat;
	}
}

/*
 * Apply an intrinsic.
 */
static jit_value_t
apply_intrinsic(jit_function_t func, const jit_opcode_descr *descr,
		jit_value_t value1, jit_value_t value2, jit_type_t type)
{
	switch (type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
		return jit_insn_call_intrinsic(func, descr->iname,
					       descr->ifunc, descr->idesc,
					       value1, value2);
	case JIT_TYPE_UINT:
		return jit_insn_call_intrinsic(func, descr->iuname,
					       descr->iufunc, descr->iudesc,
					       value1, value2);
	case JIT_TYPE_LONG:
		return jit_insn_call_intrinsic(func, descr->lname,
					       descr->lfunc, descr->ldesc,
					       value1, value2);
	case JIT_TYPE_ULONG:
		return jit_insn_call_intrinsic(func, descr->luname,
					       descr->lufunc, descr->ludesc,
					       value1, value2);
	case JIT_TYPE_FLOAT32:
		return jit_insn_call_intrinsic(func, descr->fname,
					       descr->ffunc, descr->fdesc,
					       value1, value2);
	case JIT_TYPE_FLOAT64:
		return jit_insn_call_intrinsic(func, descr->dname,
					       descr->dfunc, descr->ddesc,
					       value1, value2);
	case JIT_TYPE_NFLOAT:
		return jit_insn_call_intrinsic(func, descr->nfname,
					       descr->nffunc, descr->nfdesc,
					       value1, value2);
	}
}

/*
 * Apply a unary arithmetic operator, after coercing the
 * argument to a suitable numeric type.
 */
static jit_value_t
apply_unary_arith(jit_function_t func, const jit_opcode_descr *descr,
		  jit_value_t value, int int_only, int float_only,
		  int overflow_check)
{
	jit_type_t type = common_binary(value->type, value->type, int_only, float_only);

	int oper;
	const jit_intrinsic_descr_t *desc;
	switch (type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
		oper = descr->ioper;
		desc = descr->idesc;
		break;
	case JIT_TYPE_UINT:
		oper = descr->iuoper;
		desc = descr->iudesc;
		break;
	case JIT_TYPE_LONG:
		oper = descr->loper;
		desc = descr->ldesc;
		break;
	case JIT_TYPE_ULONG:
		oper = descr->luoper;
		desc = descr->ludesc;
		break;
	case JIT_TYPE_FLOAT32:
		oper = descr->foper;
		desc = descr->fdesc;
		break;
	case JIT_TYPE_FLOAT64:
		oper = descr->doper;
		desc = descr->ddesc;
		break;
	case JIT_TYPE_NFLOAT:
		oper = descr->nfoper;
		desc = descr->nfdesc;
		break;
	}

	value = jit_insn_convert(func, value, type, overflow_check);
	if(!value)
	{
		return 0;
	}
	if(jit_value_is_constant(value))
	{
		jit_value_t result = _jit_opcode_apply_unary(func, oper, value, type);
		if(result)
		{
			return result;
		}
	}

	if(desc && desc->ptr_result_type)
	{
		func->builder->may_throw = 1;
	}
	if(!_jit_opcode_is_supported(oper))
	{
		return apply_intrinsic(func, descr, value, 0, type);
	}
	return apply_unary(func, oper, value, type);
}

/*
 * Apply a binary arithmetic operator, after coercing both
 * arguments to a common type.
 */
static jit_value_t
apply_arith(jit_function_t func, const jit_opcode_descr *descr,
	    jit_value_t value1, jit_value_t value2,
	    int int_only, int float_only, int overflow_check)
{
	jit_type_t type = common_binary(value1->type, value2->type, int_only, float_only);

	int oper;
	const jit_intrinsic_descr_t *desc;
	switch (type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
		oper = descr->ioper;
		desc = descr->idesc;
		break;
	case JIT_TYPE_UINT:
		oper = descr->iuoper;
		desc = descr->iudesc;
		break;
	case JIT_TYPE_LONG:
		oper = descr->loper;
		desc = descr->ldesc;
		break;
	case JIT_TYPE_ULONG:
		oper = descr->luoper;
		desc = descr->ludesc;
		break;
	case JIT_TYPE_FLOAT32:
		oper = descr->foper;
		desc = descr->fdesc;
		break;
	case JIT_TYPE_FLOAT64:
		oper = descr->doper;
		desc = descr->ddesc;
		break;
	case JIT_TYPE_NFLOAT:
		oper = descr->nfoper;
		desc = descr->nfdesc;
		break;
	}

	value1 = jit_insn_convert(func, value1, type, overflow_check);
	value2 = jit_insn_convert(func, value2, type, overflow_check);
	if(!value1 || !value2)
	{
		return 0;
	}
	if(jit_value_is_constant(value1) && jit_value_is_constant(value2))
	{
		jit_value_t result = _jit_opcode_apply(func, oper, value1, value2, type);
		if(result)
		{
			return result;
		}
	}

	if(desc && desc->ptr_result_type)
	{
		func->builder->may_throw = 1;
	}
	if(!_jit_opcode_is_supported(oper))
	{
		return apply_intrinsic(func, descr, value1, value2, type);
	}
	return apply_binary(func, oper, value1, value2, type);
}

/*
 * Apply a binary shift operator, after coercing both
 * arguments to suitable types.
 */
static jit_value_t
apply_shift(jit_function_t func, const jit_opcode_descr *descr,
	    jit_value_t value1, jit_value_t value2)
{
	jit_type_t type = common_binary(value1->type, value1->type, 1, 0);

	int oper;
	switch (type->kind)
	{
	case JIT_TYPE_INT:
		oper = descr->ioper;
		break;
	case JIT_TYPE_UINT:
		oper = descr->iuoper;
		break;
	case JIT_TYPE_LONG:
		oper = descr->loper;
		break;
	default: /* Shouldn't happen */
	case JIT_TYPE_ULONG:
		oper = descr->luoper;
		break;
	}

	jit_type_t count_type = jit_type_promote_int(jit_type_normalize(value2->type));
	if(count_type != jit_type_int)
	{
		count_type = jit_type_uint;
	}

	value1 = jit_insn_convert(func, value1, type, 0);
	value2 = jit_insn_convert(func, value2, count_type, 0);
	if(!value1 || !value2)
	{
		return 0;
	}
	if(jit_value_is_constant(value1) && jit_value_is_constant(value2))
	{
		jit_value_t result = _jit_opcode_apply(func, oper, value1, value2, type);
		if(result)
		{
			return result;
		}
	}

	if(!_jit_opcode_is_supported(oper))
	{
		return apply_intrinsic(func, descr, value1, value2, type);
	}
	return apply_binary(func, oper, value1, value2, type);
}

/*
 * Apply a binary comparison operator, after coercing both
 * arguments to a common type.
 */
static jit_value_t
apply_compare(jit_function_t func, const jit_opcode_descr *descr,
	      jit_value_t value1, jit_value_t value2, int float_only)
{
	jit_type_t type = common_binary(value1->type, value2->type, 0, float_only);

	int oper;
	switch (type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
		oper = descr->ioper;
		break;
	case JIT_TYPE_UINT:
		oper = descr->iuoper;
		break;
	case JIT_TYPE_LONG:
		oper = descr->loper;
		break;
	case JIT_TYPE_ULONG:
		oper = descr->luoper;
		break;
	case JIT_TYPE_FLOAT32:
		oper = descr->foper;
		break;
	case JIT_TYPE_FLOAT64:
		oper = descr->doper;
		break;
	case JIT_TYPE_NFLOAT:
		oper = descr->nfoper;
		break;
	}

	value1 = jit_insn_convert(func, value1, type, 0);
	value2 = jit_insn_convert(func, value2, type, 0);
	if(!value1 || !value2)
	{
		return 0;
	}
	if(jit_value_is_constant(value1) && jit_value_is_constant(value2))
	{
		jit_value_t result = _jit_opcode_apply(func, oper, value1, value2, jit_type_int);
		if(result)
		{
			return result;
		}
	}

	if(!_jit_opcode_is_supported(oper))
	{
		return apply_intrinsic(func, descr, value1, value2, type);
	}
	return apply_binary(func, oper, value1, value2, jit_type_int);
}

/*
 * Apply a unary test to a floating point value.
 */
static jit_value_t
test_float_value(jit_function_t func, const jit_opcode_descr *descr, jit_value_t value)
{
	jit_type_t type = jit_type_normalize(value->type);

	int oper;
	if(type == jit_type_float32)
	{
		oper = descr->foper;
	}
	else if(type == jit_type_float64)
	{
		oper = descr->doper;
	}
	else if(type == jit_type_nfloat)
	{
		oper = descr->nfoper;
	}
	else
	{
		/* if the value is not a float then the result is false */
		return jit_value_create_nint_constant(func, jit_type_int, 0);
	}

	if(!_jit_opcode_is_supported(oper))
	{
		return apply_intrinsic(func, descr, value, 0, type);
	}
	return apply_unary(func, oper, value, jit_type_int);
}

/*@
 * @deftypefun int jit_insn_get_opcode (jit_insn_t @var{insn})
 * Get the opcode that is associated with an instruction.
 * @end deftypefun
@*/
int
jit_insn_get_opcode(jit_insn_t insn)
{
	return insn->opcode;
}

/*@
 * @deftypefun jit_value_t jit_insn_get_dest (jit_insn_t @var{insn})
 * Get the destination value that is associated with an instruction.
 * Returns NULL if the instruction does not have a destination.
 * @end deftypefun
@*/
jit_value_t
jit_insn_get_dest(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_DEST_OTHER_FLAGS) != 0)
	{
		return 0;
	}
	return insn->dest;
}

/*@
 * @deftypefun jit_value_t jit_insn_get_value1 (jit_insn_t @var{insn})
 * Get the first argument value that is associated with an instruction.
 * Returns NULL if the instruction does not have a first argument value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_get_value1(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_VALUE1_OTHER_FLAGS) != 0)
	{
		return 0;
	}
	return insn->value1;
}

/*@
 * @deftypefun jit_value_t jit_insn_get_value2 (jit_insn_t @var{insn})
 * Get the second argument value that is associated with an instruction.
 * Returns NULL if the instruction does not have a second argument value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_get_value2(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_VALUE2_OTHER_FLAGS) != 0)
	{
		return 0;
	}
	return insn->value2;
}

/*@
 * @deftypefun jit_label_t jit_insn_get_label (jit_insn_t @var{insn})
 * Get the label for a branch target from an instruction.
 * Returns NULL if the instruction does not have a branch target.
 * @end deftypefun
@*/
jit_label_t
jit_insn_get_label(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_DEST_IS_LABEL) != 0)
	{
		return (jit_label_t) insn->dest;
	}
	if((insn->flags & JIT_INSN_VALUE1_IS_LABEL) != 0)
	{
		/* "address_of_label" instruction */
		return (jit_label_t) insn->value1;
	}
	return 0;
}

/*@
 * @deftypefun jit_function_t jit_insn_get_function (jit_insn_t @var{insn})
 * Get the function for a call instruction.  Returns NULL if the
 * instruction does not refer to a called function.
 * @end deftypefun
@*/
jit_function_t
jit_insn_get_function(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_DEST_IS_FUNCTION) == 0)
	{
		return 0;
	}
	return (jit_function_t) insn->dest;
}

/*@
 * @deftypefun {void *} jit_insn_get_native (jit_insn_t @var{insn})
 * Get the function pointer for a native call instruction.
 * Returns NULL if the instruction does not refer to a native
 * function call.
 * @end deftypefun
@*/
void *
jit_insn_get_native(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_DEST_IS_NATIVE) == 0)
	{
		return 0;
	}
	return (void *) insn->dest;
}

/*@
 * @deftypefun {const char *} jit_insn_get_name (jit_insn_t @var{insn})
 * Get the diagnostic name for a function call.  Returns NULL
 * if the instruction does not have a diagnostic name.
 * @end deftypefun
@*/
const char *
jit_insn_get_name(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_VALUE1_IS_NAME) == 0)
	{
		return 0;
	}
	return (const char *) insn->value1;
}

/*@
 * @deftypefun jit_type_t jit_insn_get_signature (jit_insn_t @var{insn})
 * Get the signature for a function call instruction.  Returns NULL
 * if the instruction is not a function call.
 * @end deftypefun
@*/
jit_type_t
jit_insn_get_signature(jit_insn_t insn)
{
	if((insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE) == 0)
	{
		return 0;
	}
	return (jit_type_t) insn->value2;
}

/*@
 * @deftypefun int jit_insn_dest_is_value (jit_insn_t @var{insn})
 * Returns a non-zero value if the destination for @var{insn} is
 * actually a source value.  This can happen with instructions
 * such as @code{jit_insn_store_relative} where the instruction
 * needs three source operands, and the real destination is a
 * side-effect on one of the sources.
 * @end deftypefun
@*/
int
jit_insn_dest_is_value(jit_insn_t insn)
{
	return (insn->flags & JIT_INSN_DEST_IS_VALUE) != 0;
}

static int
new_block_with_label(jit_function_t func, jit_label_t *label, int force_new_block)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Create a new block if required */
	jit_block_t block;
	jit_block_t current_block = func->builder->current_block;
	if(force_new_block || _jit_block_get_last(current_block))
	{
		block = _jit_block_create(func);
		if(!block)
		{
			return 0;
		}
	}
	else
	{
		/* Reuse the last empty block */
		block = current_block;
		if(block->label != jit_label_undefined && *label == jit_label_undefined)
		{
			/* Reuse its label if any */
			*label = block->label;
			return 1;
		}
	}

	/* Create a new label if required */
	if(*label == jit_label_undefined)
	{
		*label = func->builder->next_label++;
	}

	/* Register the label use */
	if(!_jit_block_record_label(block, *label))
	{
		_jit_block_destroy(block);
		return 0;
	}

	/* If the block is newly created then insert it to the end of
	   the function's block list */
	if(block != current_block)
	{
		_jit_block_attach_before(func->builder->exit_block, block, block);
		func->builder->current_block = block;
	}

	return 1;
}

/*@
 * @deftypefun void jit_insn_label (jit_function_t @var{func}, jit_label_t *@var{label})
 * Start a new basic block within the function @var{func} and give it the
 * specified @var{label}. Returns zero if out of memory.
 *
 * If the contents of @var{label} are @code{jit_label_undefined}, then this
 * function will allocate a new label for this block.  Otherwise it will
 * reuse the specified label from a previous branch instruction.
 * @end deftypefun
@*/
int
jit_insn_label(jit_function_t func, jit_label_t *label)
{
	return new_block_with_label(func, label, 1);
}

/*@
 * @deftypefun void jit_insn_label_tight (jit_function_t @var{func}, jit_label_t *@var{label})
 * Start a new basic block within the function @var{func} and give it the
 * specified @var{label} but attempt to reuse the last block if it is empty.
 * Returns zero if out of memory.
 *
 * If the contents of @var{label} are @code{jit_label_undefined}, then this
 * function will allocate a new label for this block.  Otherwise it will
 * reuse the specified label from a previous branch instruction.
 * @end deftypefun
@*/
int
jit_insn_label_tight(jit_function_t func, jit_label_t *label)
{
	return new_block_with_label(func, label, 0);
}

/*@
 * @deftypefun int jit_insn_new_block (jit_function_t @var{func})
 * Start a new basic block, without giving it an explicit label. Returns a
 * non-zero value on success.
 * @end deftypefun
@*/
int
jit_insn_new_block(jit_function_t func)
{
	/* Create a new block */
	jit_block_t block = _jit_block_create(func);
	if(!block)
	{
		return 0;
	}

#ifdef _JIT_BLOCK_DEBUG
	jit_label_t label = func->builder->next_label++;
	if(!_jit_block_record_label(block, label))
	{
		_jit_block_destroy(block);
		return 0;
	}
#endif

	/* Insert the block to the end of the function's block list */
	_jit_block_attach_before(func->builder->exit_block, block, block);
	func->builder->current_block = block;

	return 1;
}

int
_jit_load_opcode(int base_opcode, jit_type_t type)
{
	type = jit_type_normalize(type);
	if(!type)
	{
		return 0;
	}

	switch(type->kind)
	{
	case JIT_TYPE_SBYTE:
		return base_opcode;

	case JIT_TYPE_UBYTE:
		return base_opcode + 1;

	case JIT_TYPE_SHORT:
		return base_opcode + 2;

	case JIT_TYPE_USHORT:
		return base_opcode + 3;

	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		return base_opcode + 4;

	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		return base_opcode + 5;

	case JIT_TYPE_FLOAT32:
		return base_opcode + 6;

	case JIT_TYPE_FLOAT64:
		return base_opcode + 7;

	case JIT_TYPE_NFLOAT:
		return base_opcode + 8;

	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		return base_opcode + 9;
	}

	return 0;
}

int
_jit_store_opcode(int base_opcode, int small_base, jit_type_t type)
{
	/* Copy instructions are in two ranges: adjust for them */
	if(small_base)
	{
		base_opcode -= 2;
	}
	else
	{
		small_base = base_opcode;
	}

	/* Determine which opcode to use */
	type = jit_type_normalize(type);
	switch(type->kind)
	{
	case JIT_TYPE_SBYTE:
	case JIT_TYPE_UBYTE:
		return small_base;

	case JIT_TYPE_SHORT:
	case JIT_TYPE_USHORT:
		return small_base + 1;

	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		return base_opcode + 2;

	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		return base_opcode + 3;

	case JIT_TYPE_FLOAT32:
		return base_opcode + 4;

	case JIT_TYPE_FLOAT64:
		return base_opcode + 5;

	case JIT_TYPE_NFLOAT:
		return base_opcode + 6;

	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		return base_opcode + 7;

	default:
		/* Shouldn't happen, but do something sane anyway */
		return base_opcode + 2;
	}
}

/*@
 * @deftypefun int jit_insn_nop (jit_function_t @var{func})
 * Emits "no operation" instruction. You may want to do that if you need
 * an empty block to move it with @code{jit_insn_move_blocks_XXX} later.
 * If you will not put empty instruction between two labels, both labels
 * will point to the same block, and block moving will fail.
 * @end deftypefun
@*/
int
jit_insn_nop(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Add a new no-op instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = JIT_OP_NOP;

	return 1;
}

/*@
 * @deftypefun jit_value_t jit_insn_load (jit_function_t @var{func}, jit_value_t @var{value})
 * Load the contents of @var{value} into a new temporary, essentially
 * duplicating the value.  Constants are not duplicated.
 * @end deftypefun
@*/
jit_value_t
jit_insn_load(jit_function_t func, jit_value_t value)
{
	if(value->is_constant)
	{
		return value;
	}

	int opcode = _jit_load_opcode(JIT_OP_COPY_LOAD_SBYTE, value->type);
	if(opcode == 0)
	{
		return 0;
	}
	return apply_unary(func, opcode, value, value->type);
}

/*@
 * @deftypefun jit_value_t jit_insn_dup (jit_function_t @var{func}, jit_value_t @var{value})
 * This is the same as @code{jit_insn_load}, but the name may better
 * reflect how it is used in some front ends.
 * @end deftypefun
@*/
jit_value_t
jit_insn_dup(jit_function_t func, jit_value_t value)
{
	return jit_insn_load(func, value);
}

/*@
 * @deftypefun void jit_insn_store (jit_function_t @var{func}, jit_value_t @var{dest}, jit_value_t @var{value})
 * Store the contents of @var{value} at the location referred to by
 * @var{dest}.  The @var{dest} should be a @code{jit_value_t} representing a
 * local variable or temporary.  Use @code{jit_insn_store_relative} to store
 * to a location referred to by a pointer.
 * @end deftypefun
@*/
int
jit_insn_store(jit_function_t func, jit_value_t dest, jit_value_t value)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	value = jit_insn_convert(func, value, dest->type, 0);
	if(!value)
	{
		return 0;
	}

	int opcode = _jit_store_opcode(JIT_OP_COPY_INT, JIT_OP_COPY_STORE_BYTE, dest->type);
	if(opcode == 0)
	{
		return 0;
	}

	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) opcode;
	insn->dest = dest;
	jit_value_ref(func, dest);
	insn->value1 = value;
	jit_value_ref(func, value);

	return 1;
}

/*
 * Scan back through the current block, looking for an address instruction that
 * involves "value" as its destination. Returns NULL if no such instruction was
 * found, or it is blocked by a later use of "value".
 *
 * The instruction found may then be combined into a new single instruction with
 * the following "load_relative", "store_relative", or another "relative_add".
 *
 * For instance, consider the code like this:
 *
 * i) y = address_of(x)
 * ...
 * j) z = add_relative(y, a)
 *
 * Let's suppose that we need to add a "store_realtive(z, b, v)" instruction.
 * The "find_base_insn()" call will return the instruction "j" and we will be
 * able to emit the instruction "store_relative(y, a + b, v)" instead. If "z"
 * is not used elsewhere then "j" will be optimized away by the dead code
 * elimination pass.
 *
 * Repetitive use of this procedure for a chain of "add_relative" instructions
 * converts it into a series of indpenedent instructions each using the very
 * first address in the chain as its base. Therefore regardless of the initial
 * chain length it is always enough to make single "find_base_insn()" call to
 * get the base address of the entire chain (think induction).
 *
 * Note that in this situation the second "find_base_insn()" call will return
 * the instruction "i" that obtains the base address as the address of a local
 * frame variable. This instruction is a candidate for being moved down to
 * where the "load_relative" or "store_relative" occurs. This might make it
 * easier for the code generator to handle field accesses whitin local
 * variables.
 *
 * The "plast" argument indicates if the found instruction is already the last
 * one, so there is no need to move it down.
 */
static jit_insn_t
find_base_insn(jit_function_t func, jit_insn_iter_t iter, jit_value_t value, int *plast)
{
	/* The "value" could be vulnerable to aliasing effects so we cannot
	   optimize it */
	if(value->is_addressable || value->is_volatile)
	{
		return 0;
	}

	/* We are about to check the last instruction before the current one */
	int last = 1;

	/* Iterate back through the block looking for a suitable instruction */
	jit_insn_t insn;
	while((insn = jit_insn_iter_previous(&iter)) != 0)
	{
		/* This instruction uses "value" in some way */
		if(insn->dest == value)
		{
			/* This is the instruction we were looking for */
			if(insn->opcode == JIT_OP_ADDRESS_OF)
			{
				*plast = last;
				return insn;
			}
			if(insn->opcode == JIT_OP_ADD_RELATIVE)
			{
				value = insn->value1;
				if(value->is_addressable || value->is_volatile)
				{
					return 0;
				}

				/* Scan forwards to ensure that "insn->value1"
				   is not modified anywhere in the instructions
				   that follow */
				jit_insn_iter_t iter2 = iter;
				jit_insn_iter_next(&iter2);
				jit_insn_t insn2;
				while((insn2 = jit_insn_iter_next(&iter2)) != 0)
				{
					if(insn2->dest == value
					   && (insn2->flags & JIT_INSN_DEST_IS_VALUE) == 0)
					{
						return 0;
					}
				}

				*plast = last;
				return insn;
			}

			/* Oops. This instruction modifies "value" and blocks
			   any previous address_of or add_relative instructions */
			if((insn->flags & JIT_INSN_DEST_IS_VALUE) == 0)
			{
				break;
			}
		}

		/* We are to check instructions that preceed the last one */
		last = 0;
	}
	return 0;
}

/*@
 * @deftypefun jit_value_t jit_insn_load_relative (jit_function_t @var{func}, jit_value_t @var{value}, jit_nint @var{offset}, jit_type_t @var{type})
 * Load a value of the specified @var{type} from the effective address
 * @code{(@var{value} + @var{offset})}, where @var{value} is a pointer.
 * @end deftypefun
@*/
jit_value_t
jit_insn_load_relative(jit_function_t func, jit_value_t value, jit_nint offset, jit_type_t type)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_iter_t iter;
	jit_insn_iter_init_last(&iter, func->builder->current_block);

	int last;
	jit_insn_t insn = find_base_insn(func, iter, value, &last);
	if(insn && insn->opcode == JIT_OP_ADD_RELATIVE)
	{
		/* We have a previous "add_relative" instruction for this
		   pointer. Adjust the current offset accordingly */
		offset += jit_value_get_nint_constant(insn->value2);
		value = insn->value1;
		insn = find_base_insn(func, iter, value, &last);
		last = 0;
	}
	if(insn && insn->opcode == JIT_OP_ADDRESS_OF && !last)
	{
		/* Shift the "address_of" instruction down, to make
		   it easier for the code generator to handle field
		   accesses within local and global variables */
		value = jit_insn_address_of(func, insn->value1);
		if(!value)
		{
			return 0;
		}
	}

	int opcode = _jit_load_opcode(JIT_OP_LOAD_RELATIVE_SBYTE, type);
	if(opcode == 0)
	{
		return 0;
	}
	jit_value_t offset_value = jit_value_create_nint_constant(func, jit_type_nint, offset);
	if(!offset_value)
	{
		return 0;
	}
	return apply_binary(func, opcode, value, offset_value, type);
}

/*@
 * @deftypefun int jit_insn_store_relative (jit_function_t @var{func}, jit_value_t @var{dest}, jit_nint @var{offset}, jit_value_t @var{value})
 * Store @var{value} at the effective address @code{(@var{dest} + @var{offset})},
 * where @var{dest} is a pointer. Returns a non-zero value on success.
 * @end deftypefun
@*/
int
jit_insn_store_relative(jit_function_t func, jit_value_t dest, jit_nint offset, jit_value_t value)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_iter_t iter;
	jit_insn_iter_init_last(&iter, func->builder->current_block);

	int last;
	jit_insn_t insn = find_base_insn(func, iter, dest, &last);
	if(insn && insn->opcode == JIT_OP_ADD_RELATIVE)
	{
		/* We have a previous "add_relative" instruction for this
		   pointer. Adjust the current offset accordingly */
		offset += jit_value_get_nint_constant(insn->value2);
		dest = insn->value1;
		insn = find_base_insn(func, iter, value, &last);
		last = 0;
	}
	if(insn && insn->opcode == JIT_OP_ADDRESS_OF && !last)
	{
		/* Shift the "address_of" instruction down, to make
		   it easier for the code generator to handle field
		   accesses within local and global variables */
		dest = jit_insn_address_of(func, insn->value1);
		if(!dest)
		{
			return 0;
		}
	}

	int opcode = _jit_store_opcode(JIT_OP_STORE_RELATIVE_BYTE, 0, value->type);
	if(opcode == 0)
	{
		return 0;
	}
	jit_value_t offset_value = jit_value_create_nint_constant(func, jit_type_nint, offset);
	if(!offset_value)
	{
		return 0;
	}

	insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) opcode;
	insn->flags = JIT_INSN_DEST_IS_VALUE;
	insn->dest = dest;
	jit_value_ref(func, dest);
	insn->value1 = value;
	jit_value_ref(func, value);
	insn->value2 = offset_value;

	return 1;
}

/*@
 * @deftypefun jit_value_t jit_insn_add_relative (jit_function_t @var{func}, jit_value_t @var{value}, jit_nint @var{offset})
 * Add the constant @var{offset} to the specified pointer @var{value}.
 * This is functionally identical to calling @code{jit_insn_add}, but
 * the JIT can optimize the code better if it knows that the addition
 * is being used to perform a relative adjustment on a pointer.
 * In particular, multiple relative adjustments on the same pointer
 * can be collapsed into a single adjustment.
 * @end deftypefun
@*/
jit_value_t
jit_insn_add_relative(jit_function_t func, jit_value_t value, jit_nint offset)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_insn_iter_t iter;
	jit_insn_iter_init_last(&iter, func->builder->current_block);

	int last;
	jit_insn_t insn = find_base_insn(func, iter, value, &last);
	if(insn && insn->opcode == JIT_OP_ADD_RELATIVE)
	{
		/* We have a previous "add_relative" instruction for this
		   pointer. Adjust the current offset accordingly */
		offset += jit_value_get_nint_constant(insn->value2);
		value = insn->value1;
	}

	jit_value_t offset_value = jit_value_create_nint_constant(func, jit_type_nint, offset);
	if(!offset_value)
	{
		return 0;
	}
	return apply_binary(func, JIT_OP_ADD_RELATIVE, value, offset_value, jit_type_void_ptr);
}

static jit_value_t
element_address(jit_function_t func, jit_value_t base, jit_value_t index, jit_nint size)
{
	jit_value_t offset_value = jit_value_create_nint_constant(func, jit_type_nint, size);
	if(!offset_value)
	{
		return 0;
	}
	offset_value = jit_insn_mul(func, index, offset_value);
	if(!offset_value)
	{
		return 0;
	}
	return jit_insn_add(func, base, offset_value);
}

/*@
 * @deftypefun jit_value_t jit_insn_load_elem (jit_function_t @var{func}, jit_value_t @var{base_addr}, jit_value_t @var{index}, jit_type_t @var{elem_type})
 * Load an element of type @var{elem_type} from position @var{index} within
 * the array starting at @var{base_addr}.  The effective address of the
 * array element is @code{@var{base_addr} + @var{index} * sizeof(@var{elem_type})}.
 * @end deftypefun
@*/
jit_value_t
jit_insn_load_elem(jit_function_t func, jit_value_t base_addr, jit_value_t index,
		   jit_type_t elem_type)
{
	/* Get the size of the element that we are fetching */
	jit_nint size = (jit_nint) jit_type_get_size(elem_type);

	/* Convert the index into a native integer */
	index = jit_insn_convert(func, index, jit_type_nint, 0);
	if(!index)
	{
		return 0;
	}

	/* If the index is constant, then turn this into a relative load */
	if(jit_value_is_constant(index))
	{
		size *= jit_value_get_nint_constant(index);
		return jit_insn_load_relative(func, base_addr, size, elem_type);
	}

	/* See if we can use a special-case instruction */
	int opcode = _jit_load_opcode(JIT_OP_LOAD_ELEMENT_SBYTE, elem_type);
	if(opcode != 0 && opcode != (JIT_OP_LOAD_ELEMENT_SBYTE + 9))
	{
		return apply_binary(func, opcode, base_addr, index, elem_type);
	}

	/* Calculate the effective address and then use a relative load */
	jit_value_t addr = element_address(func, base_addr, index, size);
	if(!addr)
	{
		return 0;
	}
	return jit_insn_load_relative(func, addr, 0, elem_type);
}

/*@
 * @deftypefun jit_value_t jit_insn_load_elem_address (jit_function_t @var{func}, jit_value_t @var{base_addr}, jit_value_t @var{index}, jit_type_t @var{elem_type})
 * Load the effective address of an element of type @var{elem_type} at
 * position @var{index} within the array starting at @var{base_addr}.
 * Essentially, this computes the expression
 * @code{@var{base_addr} + @var{index} * sizeof(@var{elem_type})}, but
 * may be more efficient than performing the steps with @code{jit_insn_mul}
 * and @code{jit_insn_add}.
 * @end deftypefun
@*/
jit_value_t
jit_insn_load_elem_address(jit_function_t func, jit_value_t base_addr, jit_value_t index,
			   jit_type_t elem_type)
{
	jit_nint size = (jit_nint) jit_type_get_size(elem_type);

	/* Convert the index into a native integer */
	index = jit_insn_convert(func, index, jit_type_nint, 0);
	if(!index)
	{
		return 0;
	}

	return element_address(func, base_addr, index, size);
}

/*@
 * @deftypefun int jit_insn_store_elem (jit_function_t @var{func}, jit_value_t @var{base_addr}, jit_value_t @var{index}, jit_value_t @var{value})
 * Store @var{value} at position @var{index} of the array starting at
 * @var{base_addr}.  The effective address of the storage location is
 * @code{@var{base_addr} + @var{index} * sizeof(jit_value_get_type(@var{value}))}.
 * @end deftypefun
@*/
int
jit_insn_store_elem(jit_function_t func, jit_value_t base_addr, jit_value_t index,
		    jit_value_t value)
{
	/* Get the size of the element that we are fetching */
	jit_type_t elem_type = jit_value_get_type(value);
	jit_nint size = (jit_nint) jit_type_get_size(elem_type);

	/* Convert the index into a native integer */
	index = jit_insn_convert(func, index, jit_type_nint, 0);
	if(!index)
	{
		return 0;
	}

	/* If the index is constant, then turn this into a relative store */
	if(jit_value_is_constant(index))
	{
		return jit_insn_store_relative(func, base_addr,
					       jit_value_get_nint_constant(index) * size,
					       value);
	}

	/* See if we can use a special-case instruction */
	int opcode = _jit_store_opcode(JIT_OP_STORE_ELEMENT_BYTE, 0, elem_type);
	if(opcode != 0 && opcode != (JIT_OP_STORE_ELEMENT_BYTE + 7))
	{
		return apply_ternary(func, opcode, base_addr, index, value);
	}

	/* Calculate the effective address and then use a relative store */
	jit_value_t addr = element_address(func, base_addr, index, size);
	if(!addr)
	{
		return 0;
	}
	return jit_insn_store_relative(func, addr, 0, value);
}

/*@
 * @deftypefun int jit_insn_check_null (jit_function_t @var{func}, jit_value_t @var{value})
 * Check @var{value} to see if it is NULL.  If it is, then throw the
 * built-in @code{JIT_RESULT_NULL_REFERENCE} exception.
 * @end deftypefun
@*/
int
jit_insn_check_null(jit_function_t func, jit_value_t value)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Do the check only if the value is no not Null constant */
	if(value->is_nint_constant && value->address != 0)
	{
		return 1;
	}
	func->builder->may_throw = 1;
	return create_unary_note(func, JIT_OP_CHECK_NULL, value);
}

int
_jit_insn_check_is_redundant(const jit_insn_iter_t *iter)
{
	jit_insn_iter_t new_iter = *iter;
	/* Back up to find the "check_null" instruction of interest */
	jit_insn_t insn = jit_insn_iter_previous(&new_iter);
	jit_value_t value = insn->value1;

	/* The value must be temporary or local, and not volatile or addressable.
	   Otherwise the value could be vulnerable to aliasing side-effects that
	   could make it NULL again even after we have checked it */
	if(!value->is_temporary || !value->is_local)
	{
		return 0;
	}
	if(value->is_volatile || value->is_addressable)
	{
		return 0;
	}

	/* Search back for a previous "check_null" instruction */
	while((insn = jit_insn_iter_previous(&new_iter)) != 0)
	{
		if(insn->opcode == JIT_OP_CHECK_NULL && insn->value1 == value)
		{
			/* This is the previous "check_null" that we were looking for */
			return 1;
		}
		if(insn->opcode >= JIT_OP_STORE_RELATIVE_BYTE &&
		   insn->opcode <= JIT_OP_STORE_RELATIVE_STRUCT)
		{
			/* This stores to the memory referenced by the destination,
			   not to the destination itself, so it cannot affect "value" */
			continue;
		}
		if(insn->dest == value)
		{
			/* The value was used as a destination, so we must check */
			return 0;
		}
	}

	/* There was no previous "check_null" instruction for this value */
	return 0;
}

/*@
 * @deftypefun jit_value_t jit_insn_add (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Add two values together and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_add(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const add_descr = {
		JIT_OP_IADD,
		JIT_OP_IADD,
		JIT_OP_LADD,
		JIT_OP_LADD,
		JIT_OP_FADD,
		JIT_OP_DADD,
		JIT_OP_NFADD,
		jit_intrinsic(jit_int_add, descr_i_ii),
		jit_intrinsic(jit_uint_add, descr_I_II),
		jit_intrinsic(jit_long_add, descr_l_ll),
		jit_intrinsic(jit_ulong_add, descr_L_LL),
		jit_intrinsic(jit_float32_add, descr_f_ff),
		jit_intrinsic(jit_float64_add, descr_d_dd),
		jit_intrinsic(jit_nfloat_add, descr_D_DD)
	};
	return apply_arith(func, &add_descr, value1, value2, 0, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_add_ovf (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Add two values together and return the result in a new temporary value.
 * Throw an exception if overflow occurs.
 * @end deftypefun
@*/
jit_value_t
jit_insn_add_ovf(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const add_ovf_descr = {
		JIT_OP_IADD_OVF,
		JIT_OP_IADD_OVF_UN,
		JIT_OP_LADD_OVF,
		JIT_OP_LADD_OVF_UN,
		JIT_OP_FADD,
		JIT_OP_DADD,
		JIT_OP_NFADD,
		jit_intrinsic(jit_int_add_ovf, descr_e_pi_ii),
		jit_intrinsic(jit_uint_add_ovf, descr_e_pI_II),
		jit_intrinsic(jit_long_add_ovf, descr_e_pl_ll),
		jit_intrinsic(jit_ulong_add_ovf, descr_e_pL_LL),
		jit_intrinsic(jit_float32_add, descr_f_ff),
		jit_intrinsic(jit_float64_add, descr_d_dd),
		jit_intrinsic(jit_nfloat_add, descr_D_DD)
	};
	return apply_arith(func, &add_ovf_descr, value1, value2, 0, 0, 1);
}

/*@
 * @deftypefun jit_value_t jit_insn_sub (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Subtract two values and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_sub(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const sub_descr = {
		JIT_OP_ISUB,
		JIT_OP_ISUB,
		JIT_OP_LSUB,
		JIT_OP_LSUB,
		JIT_OP_FSUB,
		JIT_OP_DSUB,
		JIT_OP_NFSUB,
		jit_intrinsic(jit_int_sub, descr_i_ii),
		jit_intrinsic(jit_uint_sub, descr_I_II),
		jit_intrinsic(jit_long_sub, descr_l_ll),
		jit_intrinsic(jit_ulong_sub, descr_L_LL),
		jit_intrinsic(jit_float32_sub, descr_f_ff),
		jit_intrinsic(jit_float64_sub, descr_d_dd),
		jit_intrinsic(jit_nfloat_sub, descr_D_DD)
	};
	return apply_arith(func, &sub_descr, value1, value2, 0, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_sub_ovf (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Subtract two values and return the result in a new temporary value.
 * Throw an exception if overflow occurs.
 * @end deftypefun
@*/
jit_value_t
jit_insn_sub_ovf(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const sub_ovf_descr = {
		JIT_OP_ISUB_OVF,
		JIT_OP_ISUB_OVF_UN,
		JIT_OP_LSUB_OVF,
		JIT_OP_LSUB_OVF_UN,
		JIT_OP_FSUB,
		JIT_OP_DSUB,
		JIT_OP_NFSUB,
		jit_intrinsic(jit_int_sub_ovf, descr_e_pi_ii),
		jit_intrinsic(jit_uint_sub_ovf, descr_e_pI_II),
		jit_intrinsic(jit_long_sub_ovf, descr_e_pl_ll),
		jit_intrinsic(jit_ulong_sub_ovf, descr_e_pL_LL),
		jit_intrinsic(jit_float32_sub, descr_f_ff),
		jit_intrinsic(jit_float64_sub, descr_d_dd),
		jit_intrinsic(jit_nfloat_sub, descr_D_DD)
	};
	return apply_arith(func, &sub_ovf_descr, value1, value2, 0, 0, 1);
}

/*@
 * @deftypefun jit_value_t jit_insn_mul (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Multiply two values and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_mul(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const mul_descr = {
		JIT_OP_IMUL,
		JIT_OP_IMUL,
		JIT_OP_LMUL,
		JIT_OP_LMUL,
		JIT_OP_FMUL,
		JIT_OP_DMUL,
		JIT_OP_NFMUL,
		jit_intrinsic(jit_int_mul, descr_i_ii),
		jit_intrinsic(jit_uint_mul, descr_I_II),
		jit_intrinsic(jit_long_mul, descr_l_ll),
		jit_intrinsic(jit_ulong_mul, descr_L_LL),
		jit_intrinsic(jit_float32_mul, descr_f_ff),
		jit_intrinsic(jit_float64_mul, descr_d_dd),
		jit_intrinsic(jit_nfloat_mul, descr_D_DD)
	};
	return apply_arith(func, &mul_descr, value1, value2, 0, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_mul_ovf (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Multiply two values and return the result in a new temporary value.
 * Throw an exception if overflow occurs.
 * @end deftypefun
@*/
jit_value_t
jit_insn_mul_ovf(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const mul_ovf_descr = {
		JIT_OP_IMUL_OVF,
		JIT_OP_IMUL_OVF_UN,
		JIT_OP_LMUL_OVF,
		JIT_OP_LMUL_OVF_UN,
		JIT_OP_FMUL,
		JIT_OP_DMUL,
		JIT_OP_NFMUL,
		jit_intrinsic(jit_int_mul_ovf, descr_e_pi_ii),
		jit_intrinsic(jit_uint_mul_ovf, descr_e_pI_II),
		jit_intrinsic(jit_long_mul_ovf, descr_e_pl_ll),
		jit_intrinsic(jit_ulong_mul_ovf, descr_e_pL_LL),
		jit_intrinsic(jit_float32_mul, descr_f_ff),
		jit_intrinsic(jit_float64_mul, descr_d_dd),
		jit_intrinsic(jit_nfloat_mul, descr_D_DD)
	};
	return apply_arith(func, &mul_ovf_descr, value1, value2, 0, 0, 1);
}

/*@
 * @deftypefun jit_value_t jit_insn_div (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Divide two values and return the quotient in a new temporary value.
 * Throws an exception on division by zero or arithmetic error
 * (an arithmetic error is one where the minimum possible signed
 * integer value is divided by -1).
 * @end deftypefun
@*/
jit_value_t
jit_insn_div(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const div_descr = {
		JIT_OP_IDIV,
		JIT_OP_IDIV_UN,
		JIT_OP_LDIV,
		JIT_OP_LDIV_UN,
		JIT_OP_FDIV,
		JIT_OP_DDIV,
		JIT_OP_NFDIV,
		jit_intrinsic(jit_int_div, descr_e_pi_ii),
		jit_intrinsic(jit_uint_div, descr_e_pI_II),
		jit_intrinsic(jit_long_div, descr_e_pl_ll),
		jit_intrinsic(jit_ulong_div, descr_e_pL_LL),
		jit_intrinsic(jit_float32_div, descr_f_ff),
		jit_intrinsic(jit_float64_div, descr_d_dd),
		jit_intrinsic(jit_nfloat_div, descr_D_DD)
	};
	return apply_arith(func, &div_descr, value1, value2, 0, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_rem (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Divide two values and return the remainder in a new temporary value.
 * Throws an exception on division by zero or arithmetic error
 * (an arithmetic error is one where the minimum possible signed
 * integer value is divided by -1).
 * @end deftypefun
@*/
jit_value_t
jit_insn_rem(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const rem_descr = {
		JIT_OP_IREM,
		JIT_OP_IREM_UN,
		JIT_OP_LREM,
		JIT_OP_LREM_UN,
		JIT_OP_FREM,
		JIT_OP_DREM,
		JIT_OP_NFREM,
		jit_intrinsic(jit_int_rem, descr_e_pi_ii),
		jit_intrinsic(jit_uint_rem, descr_e_pI_II),
		jit_intrinsic(jit_long_rem, descr_e_pl_ll),
		jit_intrinsic(jit_ulong_rem, descr_e_pL_LL),
		jit_intrinsic(jit_float32_rem, descr_f_ff),
		jit_intrinsic(jit_float64_rem, descr_d_dd),
		jit_intrinsic(jit_nfloat_rem, descr_D_DD)
	};
	return apply_arith(func, &rem_descr, value1, value2, 0, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_rem_ieee (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Divide two values and return the remainder in a new temporary value.
 * Throws an exception on division by zero or arithmetic error
 * (an arithmetic error is one where the minimum possible signed
 * integer value is divided by -1).  This function is identical to
 * @code{jit_insn_rem}, except that it uses IEEE rules for computing
 * the remainder of floating-point values.
 * @end deftypefun
@*/
jit_value_t
jit_insn_rem_ieee(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const rem_ieee_descr = {
		JIT_OP_IREM,
		JIT_OP_IREM_UN,
		JIT_OP_LREM,
		JIT_OP_LREM_UN,
		JIT_OP_FREM_IEEE,
		JIT_OP_DREM_IEEE,
		JIT_OP_NFREM_IEEE,
		jit_intrinsic(jit_int_rem, descr_e_pi_ii),
		jit_intrinsic(jit_uint_rem, descr_e_pI_II),
		jit_intrinsic(jit_long_rem, descr_e_pl_ll),
		jit_intrinsic(jit_ulong_rem, descr_e_pL_LL),
		jit_intrinsic(jit_float32_ieee_rem, descr_f_ff),
		jit_intrinsic(jit_float64_ieee_rem, descr_d_dd),
		jit_intrinsic(jit_nfloat_ieee_rem, descr_D_DD)
	};
	return apply_arith(func, &rem_ieee_descr, value1, value2, 0, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_neg (jit_function_t @var{func}, jit_value_t @var{value1})
 * Negate a value and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_neg(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const neg_descr = {
		JIT_OP_INEG,
		JIT_OP_INEG,
		JIT_OP_LNEG,
		JIT_OP_LNEG,
		JIT_OP_FNEG,
		JIT_OP_DNEG,
		JIT_OP_NFNEG,
		jit_intrinsic(jit_int_neg, descr_i_i),
		jit_intrinsic(jit_uint_neg, descr_I_I),
		jit_intrinsic(jit_long_neg, descr_l_l),
		jit_intrinsic(jit_ulong_neg, descr_L_L),
		jit_intrinsic(jit_float32_neg, descr_f_f),
		jit_intrinsic(jit_float64_neg, descr_d_d),
		jit_intrinsic(jit_nfloat_neg, descr_D_D)
	};

	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));

	int oper;
	switch (type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
		oper = neg_descr.ioper;
		break;
	case JIT_TYPE_UINT:
		type = jit_type_int;
		oper = neg_descr.ioper;
		break;
	case JIT_TYPE_LONG:
		oper = neg_descr.loper;
		break;
	case JIT_TYPE_ULONG:
		type = jit_type_long;
		oper = neg_descr.loper;
		break;
	case JIT_TYPE_FLOAT32:
		oper = neg_descr.foper;
		break;
	case JIT_TYPE_FLOAT64:
		oper = neg_descr.doper;
		break;
	case JIT_TYPE_NFLOAT:
		oper = neg_descr.nfoper;
		break;
	}

	value = jit_insn_convert(func, value, type, 0);
	if(!value)
	{
		return 0;
	}
	if(jit_value_is_constant(value))
	{
		jit_value_t result = _jit_opcode_apply_unary(func, oper, value, type);
		if(result)
		{
			return result;
		}
	}

	if(!_jit_opcode_is_supported(oper))
	{
		return apply_intrinsic(func, &neg_descr, value, 0, type);
	}
	return apply_unary(func, oper, value, type);
}

/*@
 * @deftypefun jit_value_t jit_insn_and (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Bitwise AND two values and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_and(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const and_descr = {
		JIT_OP_IAND,
		JIT_OP_IAND,
		JIT_OP_LAND,
		JIT_OP_LAND,
		0, 0, 0,
		jit_intrinsic(jit_int_and, descr_i_ii),
		jit_intrinsic(jit_uint_and, descr_I_II),
		jit_intrinsic(jit_long_and, descr_l_ll),
		jit_intrinsic(jit_ulong_and, descr_L_LL),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_arith(func, &and_descr, value1, value2, 1, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_or (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Bitwise OR two values and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_or(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const or_descr = {
		JIT_OP_IOR,
		JIT_OP_IOR,
		JIT_OP_LOR,
		JIT_OP_LOR,
		0, 0, 0,
		jit_intrinsic(jit_int_or, descr_i_ii),
		jit_intrinsic(jit_uint_or, descr_I_II),
		jit_intrinsic(jit_long_or, descr_l_ll),
		jit_intrinsic(jit_ulong_or, descr_L_LL),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_arith(func, &or_descr, value1, value2, 1, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_xor (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Bitwise XOR two values and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_xor(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const xor_descr = {
		JIT_OP_IXOR,
		JIT_OP_IXOR,
		JIT_OP_LXOR,
		JIT_OP_LXOR,
		0, 0, 0,
		jit_intrinsic(jit_int_xor, descr_i_ii),
		jit_intrinsic(jit_uint_xor, descr_I_II),
		jit_intrinsic(jit_long_xor, descr_l_ll),
		jit_intrinsic(jit_ulong_xor, descr_L_LL),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_arith(func, &xor_descr, value1, value2, 1, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_not (jit_function_t @var{func}, jit_value_t @var{value1})
 * Bitwise NOT a value and return the result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_not(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const not_descr = {
		JIT_OP_INOT,
		JIT_OP_INOT,
		JIT_OP_LNOT,
		JIT_OP_LNOT,
		0, 0, 0,
		jit_intrinsic(jit_int_not, descr_i_i),
		jit_intrinsic(jit_uint_not, descr_I_I),
		jit_intrinsic(jit_long_not, descr_l_l),
		jit_intrinsic(jit_ulong_not, descr_L_L),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_unary_arith(func, &not_descr, value, 1, 0, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_shl (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Perform a bitwise left shift on two values and return the
 * result in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_shl(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const shl_descr = {
		JIT_OP_ISHL,
		JIT_OP_ISHL,
		JIT_OP_LSHL,
		JIT_OP_LSHL,
		0, 0, 0,
		jit_intrinsic(jit_int_shl, descr_i_iI),
		jit_intrinsic(jit_uint_shl, descr_I_II),
		jit_intrinsic(jit_long_shl, descr_l_lI),
		jit_intrinsic(jit_ulong_shl, descr_L_LI),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_shift(func, &shl_descr, value1, value2);
}

/*@
 * @deftypefun jit_value_t jit_insn_shr (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Perform a bitwise right shift on two values and return the
 * result in a new temporary value.  This performs a signed shift
 * on signed operators, and an unsigned shift on unsigned operands.
 * @end deftypefun
@*/
jit_value_t
jit_insn_shr(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const shr_descr = {
		JIT_OP_ISHR,
		JIT_OP_ISHR_UN,
		JIT_OP_LSHR,
		JIT_OP_LSHR_UN,
		0, 0, 0,
		jit_intrinsic(jit_int_shr, descr_i_iI),
		jit_intrinsic(jit_uint_shr, descr_I_II),
		jit_intrinsic(jit_long_shr, descr_l_lI),
		jit_intrinsic(jit_ulong_shr, descr_L_LI),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_shift(func, &shr_descr, value1, value2);
}

/*@
 * @deftypefun jit_value_t jit_insn_ushr (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Perform a bitwise right shift on two values and return the
 * result in a new temporary value.  This performs an unsigned
 * shift on both signed and unsigned operands.
 * @end deftypefun
@*/
jit_value_t
jit_insn_ushr(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const ushr_descr = {
		JIT_OP_ISHR_UN,
		JIT_OP_ISHR_UN,
		JIT_OP_LSHR_UN,
		JIT_OP_LSHR_UN,
		0, 0, 0,
		jit_intrinsic(jit_uint_shr, descr_I_II),
		jit_intrinsic(jit_uint_shr, descr_I_II),
		jit_intrinsic(jit_ulong_shr, descr_L_LI),
		jit_intrinsic(jit_ulong_shr, descr_L_LI),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_shift(func, &ushr_descr, value1, value2);
}

/*@
 * @deftypefun jit_value_t jit_insn_sshr (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Perform a bitwise right shift on two values and return the
 * result in a new temporary value.  This performs an signed
 * shift on both signed and unsigned operands.
 * @end deftypefun
@*/
jit_value_t
jit_insn_sshr(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const sshr_descr = {
		JIT_OP_ISHR,
		JIT_OP_ISHR,
		JIT_OP_LSHR,
		JIT_OP_LSHR,
		0, 0, 0,
		jit_intrinsic(jit_int_shr, descr_i_iI),
		jit_intrinsic(jit_int_shr, descr_i_iI),
		jit_intrinsic(jit_long_shr, descr_l_lI),
		jit_intrinsic(jit_long_shr, descr_l_lI),
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic
	};
	return apply_shift(func, &sshr_descr, value1, value2);
}

/*@
 * @deftypefun jit_value_t jit_insn_eq (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values for equality and return the result
 * in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_eq(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const eq_descr = {
		JIT_OP_IEQ,
		JIT_OP_IEQ,
		JIT_OP_LEQ,
		JIT_OP_LEQ,
		JIT_OP_FEQ,
		JIT_OP_DEQ,
		JIT_OP_NFEQ,
		jit_intrinsic(jit_int_eq, descr_i_ii),
		jit_intrinsic(jit_uint_eq, descr_i_II),
		jit_intrinsic(jit_long_eq, descr_i_ll),
		jit_intrinsic(jit_ulong_eq, descr_i_LL),
		jit_intrinsic(jit_float32_eq, descr_i_ff),
		jit_intrinsic(jit_float64_eq, descr_i_dd),
		jit_intrinsic(jit_nfloat_eq, descr_i_DD)
	};
	return apply_compare(func, &eq_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_ne (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values for inequality and return the result
 * in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_ne(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const ne_descr = {
		JIT_OP_INE,
		JIT_OP_INE,
		JIT_OP_LNE,
		JIT_OP_LNE,
		JIT_OP_FNE,
		JIT_OP_DNE,
		JIT_OP_NFNE,
		jit_intrinsic(jit_int_ne, descr_i_ii),
		jit_intrinsic(jit_uint_ne, descr_i_II),
		jit_intrinsic(jit_long_ne, descr_i_ll),
		jit_intrinsic(jit_ulong_ne, descr_i_LL),
		jit_intrinsic(jit_float32_ne, descr_i_ff),
		jit_intrinsic(jit_float64_ne, descr_i_dd),
		jit_intrinsic(jit_nfloat_ne, descr_i_DD)
	};
	return apply_compare(func, &ne_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_lt (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values for less than and return the result
 * in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_lt(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const lt_descr = {
		JIT_OP_ILT,
		JIT_OP_ILT_UN,
		JIT_OP_LLT,
		JIT_OP_LLT_UN,
		JIT_OP_FLT,
		JIT_OP_DLT,
		JIT_OP_NFLT,
		jit_intrinsic(jit_int_lt, descr_i_ii),
		jit_intrinsic(jit_uint_lt, descr_i_II),
		jit_intrinsic(jit_long_lt, descr_i_ll),
		jit_intrinsic(jit_ulong_lt, descr_i_LL),
		jit_intrinsic(jit_float32_lt, descr_i_ff),
		jit_intrinsic(jit_float64_lt, descr_i_dd),
		jit_intrinsic(jit_nfloat_lt, descr_i_DD)
	};
	return apply_compare(func, &lt_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_le (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values for less than or equal and return the result
 * in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_le(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const le_descr = {
		JIT_OP_ILE,
		JIT_OP_ILE_UN,
		JIT_OP_LLE,
		JIT_OP_LLE_UN,
		JIT_OP_FLE,
		JIT_OP_DLE,
		JIT_OP_NFLE,
		jit_intrinsic(jit_int_le, descr_i_ii),
		jit_intrinsic(jit_uint_le, descr_i_II),
		jit_intrinsic(jit_long_le, descr_i_ll),
		jit_intrinsic(jit_ulong_le, descr_i_LL),
		jit_intrinsic(jit_float32_le, descr_i_ff),
		jit_intrinsic(jit_float64_le, descr_i_dd),
		jit_intrinsic(jit_nfloat_le, descr_i_DD)
	};
	return apply_compare(func, &le_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_gt (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values for greater than and return the result
 * in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_gt(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const gt_descr = {
		JIT_OP_IGT,
		JIT_OP_IGT_UN,
		JIT_OP_LGT,
		JIT_OP_LGT_UN,
		JIT_OP_FGT,
		JIT_OP_DGT,
		JIT_OP_NFGT,
		jit_intrinsic(jit_int_gt, descr_i_ii),
		jit_intrinsic(jit_uint_gt, descr_i_II),
		jit_intrinsic(jit_long_gt, descr_i_ll),
		jit_intrinsic(jit_ulong_gt, descr_i_LL),
		jit_intrinsic(jit_float32_gt, descr_i_ff),
		jit_intrinsic(jit_float64_gt, descr_i_dd),
		jit_intrinsic(jit_nfloat_gt, descr_i_DD)
	};
	return apply_compare(func, &gt_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_ge (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values for greater than or equal and return the result
 * in a new temporary value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_ge(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const ge_descr = {
		JIT_OP_IGE,
		JIT_OP_IGE_UN,
		JIT_OP_LGE,
		JIT_OP_LGE_UN,
		JIT_OP_FGE,
		JIT_OP_DGE,
		JIT_OP_NFGE,
		jit_intrinsic(jit_int_ge, descr_i_ii),
		jit_intrinsic(jit_uint_ge, descr_i_II),
		jit_intrinsic(jit_long_ge, descr_i_ll),
		jit_intrinsic(jit_ulong_ge, descr_i_LL),
		jit_intrinsic(jit_float32_ge, descr_i_ff),
		jit_intrinsic(jit_float64_ge, descr_i_dd),
		jit_intrinsic(jit_nfloat_ge, descr_i_DD)
	};
	return apply_compare(func, &ge_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_cmpl (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values, and return a -1, 0, or 1 result.  If either
 * value is "not a number", then -1 is returned.
 * @end deftypefun
@*/
jit_value_t
jit_insn_cmpl(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const cmpl_descr = {
		JIT_OP_ICMP,
		JIT_OP_ICMP_UN,
		JIT_OP_LCMP,
		JIT_OP_LCMP_UN,
		JIT_OP_FCMPL,
		JIT_OP_DCMPL,
		JIT_OP_NFCMPL,
		jit_intrinsic(jit_int_cmp, descr_i_ii),
		jit_intrinsic(jit_uint_cmp, descr_i_II),
		jit_intrinsic(jit_long_cmp, descr_i_ll),
		jit_intrinsic(jit_ulong_cmp, descr_i_LL),
		jit_intrinsic(jit_float32_cmpl, descr_i_ff),
		jit_intrinsic(jit_float64_cmpl, descr_i_dd),
		jit_intrinsic(jit_nfloat_cmpl, descr_i_DD)
	};
	return apply_compare(func, &cmpl_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_cmpg (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * Compare two values, and return a -1, 0, or 1 result.  If either
 * value is "not a number", then 1 is returned.
 * @end deftypefun
@*/
jit_value_t
jit_insn_cmpg(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const cmpg_descr = {
		JIT_OP_ICMP,
		JIT_OP_ICMP_UN,
		JIT_OP_LCMP,
		JIT_OP_LCMP_UN,
		JIT_OP_FCMPG,
		JIT_OP_DCMPG,
		JIT_OP_NFCMPG,
		jit_intrinsic(jit_int_cmp, descr_i_ii),
		jit_intrinsic(jit_uint_cmp, descr_i_II),
		jit_intrinsic(jit_long_cmp, descr_i_ll),
		jit_intrinsic(jit_ulong_cmp, descr_i_LL),
		jit_intrinsic(jit_float32_cmpg, descr_i_ff),
		jit_intrinsic(jit_float64_cmpg, descr_i_dd),
		jit_intrinsic(jit_nfloat_cmpg, descr_i_DD)
	};
	return apply_compare(func, &cmpg_descr, value1, value2, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_to_bool (jit_function_t @var{func}, jit_value_t @var{value1})
 * Convert a value into a boolean 0 or 1 result of type @code{jit_type_int}.
 * @end deftypefun
@*/
jit_value_t
jit_insn_to_bool(jit_function_t func, jit_value_t value)
{
	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* If the previous instruction was a comparison, then there is
	   nothing that we need to do to make the value boolean */
	int opcode;
	jit_block_t block = func->builder->current_block;
	jit_insn_t last = _jit_block_get_last(block);
	if(value->is_temporary && last && last->dest == value)
	{
		opcode = last->opcode;
		if(opcode >= JIT_OP_IEQ && opcode <= JIT_OP_NFGE_INV)
		{
			return value;
		}
	}

	/* Perform a comparison to determine if the value is non-zero */
	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));
	jit_value_t zero;
	switch(type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		zero = jit_value_create_nint_constant(func, jit_type_int, 0);
		break;
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		zero = jit_value_create_long_constant(func, jit_type_long, 0);
		break;
	case JIT_TYPE_FLOAT32:
		zero = jit_value_create_float32_constant(func, jit_type_float32,
							 (jit_float32) 0.0);
		break;
	case JIT_TYPE_FLOAT64:
		zero = jit_value_create_float64_constant(func, jit_type_float64,
							 (jit_float64) 0.0);
		break;
	case JIT_TYPE_NFLOAT:
		zero = jit_value_create_nfloat_constant(func, jit_type_nfloat,
							(jit_nfloat) 0.0);
		break;
	}
	if(!zero)
	{
		return 0;
	}
	return jit_insn_ne(func, value, zero);
}

/*@
 * @deftypefun jit_value_t jit_insn_to_not_bool (jit_function_t @var{func}, jit_value_t @var{value1})
 * Convert a value into a boolean 1 or 0 result of type @code{jit_type_int}
 * (i.e. the inverse of @code{jit_insn_to_bool}).
 * @end deftypefun
@*/
jit_value_t
jit_insn_to_not_bool(jit_function_t func, jit_value_t value)
{
	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* If the previous instruction was a comparison, then all
	   we have to do is invert the comparison opcode */
	int opcode;
	jit_block_t block = func->builder->current_block;
	jit_insn_t last = _jit_block_get_last(block);
	if(value->is_temporary && last && last->dest == value)
	{
		opcode = last->opcode;
		if(opcode >= JIT_OP_IEQ && opcode <= JIT_OP_NFGE_INV)
		{
			switch(opcode)
			{
			case JIT_OP_IEQ:	opcode = JIT_OP_INE;      break;
			case JIT_OP_INE:	opcode = JIT_OP_IEQ;      break;
			case JIT_OP_ILT:	opcode = JIT_OP_IGE;      break;
			case JIT_OP_ILT_UN:	opcode = JIT_OP_IGE_UN;   break;
			case JIT_OP_ILE:	opcode = JIT_OP_IGT;      break;
			case JIT_OP_ILE_UN:	opcode = JIT_OP_IGT_UN;   break;
			case JIT_OP_IGT:	opcode = JIT_OP_ILE;      break;
			case JIT_OP_IGT_UN:	opcode = JIT_OP_ILE_UN;   break;
			case JIT_OP_IGE:	opcode = JIT_OP_ILT;      break;
			case JIT_OP_IGE_UN:	opcode = JIT_OP_ILT_UN;   break;
			case JIT_OP_LEQ:	opcode = JIT_OP_LNE;      break;
			case JIT_OP_LNE:	opcode = JIT_OP_LEQ;      break;
			case JIT_OP_LLT:	opcode = JIT_OP_LGE;      break;
			case JIT_OP_LLT_UN:	opcode = JIT_OP_LGE_UN;   break;
			case JIT_OP_LLE:	opcode = JIT_OP_LGT;      break;
			case JIT_OP_LLE_UN:	opcode = JIT_OP_LGT_UN;   break;
			case JIT_OP_LGT:	opcode = JIT_OP_LLE;      break;
			case JIT_OP_LGT_UN:	opcode = JIT_OP_LLE_UN;   break;
			case JIT_OP_LGE:	opcode = JIT_OP_LLT;      break;
			case JIT_OP_LGE_UN:	opcode = JIT_OP_LLT_UN;   break;
			case JIT_OP_FEQ:	opcode = JIT_OP_FNE;      break;
			case JIT_OP_FNE:	opcode = JIT_OP_FEQ;      break;
			case JIT_OP_FLT:	opcode = JIT_OP_FGE_INV;  break;
			case JIT_OP_FLE:	opcode = JIT_OP_FGT_INV;  break;
			case JIT_OP_FGT:	opcode = JIT_OP_FLE_INV;  break;
			case JIT_OP_FGE:	opcode = JIT_OP_FLT_INV;  break;
			case JIT_OP_FLT_INV:	opcode = JIT_OP_FGE;      break;
			case JIT_OP_FLE_INV:	opcode = JIT_OP_FGT;      break;
			case JIT_OP_FGT_INV:	opcode = JIT_OP_FLE;      break;
			case JIT_OP_FGE_INV:	opcode = JIT_OP_FLT;      break;
			case JIT_OP_DEQ:	opcode = JIT_OP_DNE;      break;
			case JIT_OP_DNE:	opcode = JIT_OP_DEQ;      break;
			case JIT_OP_DLT:	opcode = JIT_OP_DGE_INV;  break;
			case JIT_OP_DLE:	opcode = JIT_OP_DGT_INV;  break;
			case JIT_OP_DGT:	opcode = JIT_OP_DLE_INV;  break;
			case JIT_OP_DGE:	opcode = JIT_OP_DLT_INV;  break;
			case JIT_OP_DLT_INV:	opcode = JIT_OP_DGE;      break;
			case JIT_OP_DLE_INV:	opcode = JIT_OP_DGT;      break;
			case JIT_OP_DGT_INV:	opcode = JIT_OP_DLE;      break;
			case JIT_OP_DGE_INV:	opcode = JIT_OP_DLT;      break;
			case JIT_OP_NFEQ:	opcode = JIT_OP_NFNE;     break;
			case JIT_OP_NFNE:	opcode = JIT_OP_NFEQ;     break;
			case JIT_OP_NFLT:	opcode = JIT_OP_NFGE_INV; break;
			case JIT_OP_NFLE:	opcode = JIT_OP_NFGT_INV; break;
			case JIT_OP_NFGT:	opcode = JIT_OP_NFLE_INV; break;
			case JIT_OP_NFGE:	opcode = JIT_OP_NFLT_INV; break;
			case JIT_OP_NFLT_INV:	opcode = JIT_OP_NFGE;     break;
			case JIT_OP_NFLE_INV:	opcode = JIT_OP_NFGT;     break;
			case JIT_OP_NFGT_INV:	opcode = JIT_OP_NFLE;     break;
			case JIT_OP_NFGE_INV:	opcode = JIT_OP_NFLT;     break;
			}
			last->opcode = (short) opcode;
			return value;
		}
	}

	/* Perform a comparison to determine if the value is zero */
	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));
	jit_value_t zero;
	switch(type->kind)
	{
	default: /* Shouldn't happen */
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		zero = jit_value_create_nint_constant(func, jit_type_int, 0);
		break;
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		zero = jit_value_create_long_constant(func, jit_type_long, 0);
		break;
	case JIT_TYPE_FLOAT32:
		zero = jit_value_create_float32_constant(func, jit_type_float32,
							 (jit_float32) 0.0);
		break;
	case JIT_TYPE_FLOAT64:
		zero = jit_value_create_float64_constant(func, jit_type_float64,
							 (jit_float64) 0.0);
		break;
	case JIT_TYPE_NFLOAT:
		zero = jit_value_create_nfloat_constant(func, jit_type_nfloat,
							(jit_nfloat) 0.0);
		break;
	}
	if(!zero)
	{
		return 0;
	}
	return jit_insn_eq(func, value, zero);
}

/*@
 * @deftypefun jit_value_t jit_insn_acos (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_asin (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_atan (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_atan2 (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * @deftypefunx jit_value_t jit_insn_cos (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_cosh (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_exp (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_log (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_log10 (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_pow (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * @deftypefunx jit_value_t jit_insn_sin (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_sinh (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_sqrt (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_tan (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_tanh (jit_function_t @var{func}, jit_value_t @var{value1})
 * Apply a mathematical function to floating-point arguments.
 * @end deftypefun
@*/
jit_value_t
jit_insn_acos(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const acos_descr = {
		0, 0, 0, 0,
		JIT_OP_FACOS,
		JIT_OP_DACOS,
		JIT_OP_NFACOS,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_acos, descr_f_f),
		jit_intrinsic(jit_float64_acos, descr_d_d),
		jit_intrinsic(jit_nfloat_acos, descr_D_D)
	};
	return apply_unary_arith(func, &acos_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_asin(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const asin_descr = {
		0, 0, 0, 0,
		JIT_OP_FASIN,
		JIT_OP_DASIN,
		JIT_OP_NFASIN,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_asin, descr_f_f),
		jit_intrinsic(jit_float64_asin, descr_d_d),
		jit_intrinsic(jit_nfloat_asin, descr_D_D)
	};
	return apply_unary_arith(func, &asin_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_atan(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const atan_descr = {
		0, 0, 0, 0,
		JIT_OP_FATAN,
		JIT_OP_DATAN,
		JIT_OP_NFATAN,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_atan, descr_f_f),
		jit_intrinsic(jit_float64_atan, descr_d_d),
		jit_intrinsic(jit_nfloat_atan, descr_D_D)
	};
	return apply_unary_arith(func, &atan_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_atan2(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const atan2_descr = {
		0, 0, 0, 0,
		JIT_OP_FATAN2,
		JIT_OP_DATAN2,
		JIT_OP_NFATAN2,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_atan2, descr_f_ff),
		jit_intrinsic(jit_float64_atan2, descr_d_dd),
		jit_intrinsic(jit_nfloat_atan2, descr_D_DD)
	};
	return apply_arith(func, &atan2_descr, value1, value2, 0, 1, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_ceil (jit_function_t @var{func}, jit_value_t @var{value1})
 * Round @var{value1} up towads positive infinity.
 * @end deftypefun
@*/
jit_value_t
jit_insn_ceil(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const ceil_descr = {
		0, 0, 0, 0,
		JIT_OP_FCEIL,
		JIT_OP_DCEIL,
		JIT_OP_NFCEIL,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_ceil, descr_f_f),
		jit_intrinsic(jit_float64_ceil, descr_d_d),
		jit_intrinsic(jit_nfloat_ceil, descr_D_D)
	};
	return apply_unary_arith(func, &ceil_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_cos(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const cos_descr = {
		0, 0, 0, 0,
		JIT_OP_FCOS,
		JIT_OP_DCOS,
		JIT_OP_NFCOS,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_cos, descr_f_f),
		jit_intrinsic(jit_float64_cos, descr_d_d),
		jit_intrinsic(jit_nfloat_cos, descr_D_D)
	};
	return apply_unary_arith(func, &cos_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_cosh(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const cosh_descr = {
		0, 0, 0, 0,
		JIT_OP_FCOSH,
		JIT_OP_DCOSH,
		JIT_OP_NFCOSH,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_cosh, descr_f_f),
		jit_intrinsic(jit_float64_cosh, descr_d_d),
		jit_intrinsic(jit_nfloat_cosh, descr_D_D)
	};
	return apply_unary_arith(func, &cosh_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_exp(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const exp_descr = {
		0, 0, 0, 0,
		JIT_OP_FEXP,
		JIT_OP_DEXP,
		JIT_OP_NFEXP,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_exp, descr_f_f),
		jit_intrinsic(jit_float64_exp, descr_d_d),
		jit_intrinsic(jit_nfloat_exp, descr_D_D)
	};
	return apply_unary_arith(func, &exp_descr, value, 0, 1, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_floor (jit_function_t @var{func}, jit_value_t @var{value1})
 * Round @var{value1} down towards negative infinity.
 * @end deftypefun
@*/
jit_value_t
jit_insn_floor(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const floor_descr = {
		0, 0, 0, 0,
		JIT_OP_FFLOOR,
		JIT_OP_DFLOOR,
		JIT_OP_NFFLOOR,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_floor, descr_f_f),
		jit_intrinsic(jit_float64_floor, descr_d_d),
		jit_intrinsic(jit_nfloat_floor, descr_D_D)
	};
	return apply_unary_arith(func, &floor_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_log(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const log_descr = {
		0, 0, 0, 0,
		JIT_OP_FLOG,
		JIT_OP_DLOG,
		JIT_OP_NFLOG,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_log, descr_f_f),
		jit_intrinsic(jit_float64_log, descr_d_d),
		jit_intrinsic(jit_nfloat_log, descr_D_D)
	};
	return apply_unary_arith(func, &log_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_log10(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const log10_descr = {
		0, 0, 0, 0,
		JIT_OP_FLOG10,
		JIT_OP_DLOG10,
		JIT_OP_NFLOG10,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_log10, descr_f_f),
		jit_intrinsic(jit_float64_log10, descr_d_d),
		jit_intrinsic(jit_nfloat_log10, descr_D_D)
	};
	return apply_unary_arith(func, &log10_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_pow(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const pow_descr = {
		0, 0, 0, 0,
		JIT_OP_FPOW,
		JIT_OP_DPOW,
		JIT_OP_NFPOW,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_pow, descr_f_ff),
		jit_intrinsic(jit_float64_pow, descr_d_dd),
		jit_intrinsic(jit_nfloat_pow, descr_D_DD)
	};
	return apply_arith(func, &pow_descr, value1, value2, 0, 1, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_rint (jit_function_t @var{func}, jit_value_t @var{value1})
 * Round @var{value1} to the nearest integer. Half-way cases are rounded to the even number.
 * @end deftypefun
@*/
jit_value_t
jit_insn_rint(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const rint_descr = {
		0, 0, 0, 0,
		JIT_OP_FRINT,
		JIT_OP_DRINT,
		JIT_OP_NFRINT,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_rint, descr_f_f),
		jit_intrinsic(jit_float64_rint, descr_d_d),
		jit_intrinsic(jit_nfloat_rint, descr_D_D)
	};
	return apply_unary_arith(func, &rint_descr, value, 0, 1, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_round (jit_function_t @var{func}, jit_value_t @var{value1})
 * Round @var{value1} to the nearest integer. Half-way cases are rounded away from zero.
 * @end deftypefun
@*/
jit_value_t
jit_insn_round(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const round_descr = {
		0, 0, 0, 0,
		JIT_OP_FROUND,
		JIT_OP_DROUND,
		JIT_OP_NFROUND,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_round, descr_f_f),
		jit_intrinsic(jit_float64_round, descr_d_d),
		jit_intrinsic(jit_nfloat_round, descr_D_D)
	};
	return apply_unary_arith(func, &round_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_sin(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const sin_descr = {
		0, 0, 0, 0,
		JIT_OP_FSIN,
		JIT_OP_DSIN,
		JIT_OP_NFSIN,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_sin, descr_f_f),
		jit_intrinsic(jit_float64_sin, descr_d_d),
		jit_intrinsic(jit_nfloat_sin, descr_D_D)
	};
	return apply_unary_arith(func, &sin_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_sinh(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const sinh_descr = {
		0, 0, 0, 0,
		JIT_OP_FSINH,
		JIT_OP_DSINH,
		JIT_OP_NFSINH,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_sinh, descr_f_f),
		jit_intrinsic(jit_float64_sinh, descr_d_d),
		jit_intrinsic(jit_nfloat_sinh, descr_D_D)
	};
	return apply_unary_arith(func, &sinh_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_sqrt(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const sqrt_descr = {
		0, 0, 0, 0,
		JIT_OP_FSQRT,
		JIT_OP_DSQRT,
		JIT_OP_NFSQRT,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_sqrt, descr_f_f),
		jit_intrinsic(jit_float64_sqrt, descr_d_d),
		jit_intrinsic(jit_nfloat_sqrt, descr_D_D)
	};
	return apply_unary_arith(func, &sqrt_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_tan(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const tan_descr = {
		0, 0, 0, 0,
		JIT_OP_FTAN,
		JIT_OP_DTAN,
		JIT_OP_NFTAN,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_tan, descr_f_f),
		jit_intrinsic(jit_float64_tan, descr_d_d),
		jit_intrinsic(jit_nfloat_tan, descr_D_D)
	};
	return apply_unary_arith(func, &tan_descr, value, 0, 1, 0);
}

jit_value_t
jit_insn_tanh(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const tanh_descr = {
		0, 0, 0, 0,
		JIT_OP_FTANH,
		JIT_OP_DTANH,
		JIT_OP_NFTANH,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_tanh, descr_f_f),
		jit_intrinsic(jit_float64_tanh, descr_d_d),
		jit_intrinsic(jit_nfloat_tanh, descr_D_D)
	};
	return apply_unary_arith(func, &tanh_descr, value, 0, 1, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_trunc (jit_function_t @var{func}, jit_value_t @var{value1})
 * Round @var{value1} towards zero.
 * @end deftypefun
@*/
jit_value_t
jit_insn_trunc(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const trunc_descr = {
		0, 0, 0, 0,
		JIT_OP_FTRUNC,
		JIT_OP_DTRUNC,
		JIT_OP_NFTRUNC,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_trunc, descr_f_f),
		jit_intrinsic(jit_float64_trunc, descr_d_d),
		jit_intrinsic(jit_nfloat_trunc, descr_D_D)
	};
	return apply_unary_arith(func, &trunc_descr, value, 0, 1, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_is_nan (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_is_finite (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_is_inf (jit_function_t @var{func}, jit_value_t @var{value1})
 * Test a floating point value for not a number, finite, or infinity.
 * @end deftypefun
@*/
jit_value_t
jit_insn_is_nan(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const is_nan_descr = {
		0, 0, 0, 0,
		JIT_OP_IS_FNAN,
		JIT_OP_IS_DNAN,
		JIT_OP_IS_NFNAN,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_is_nan, descr_i_f),
		jit_intrinsic(jit_float64_is_nan, descr_i_d),
		jit_intrinsic(jit_nfloat_is_nan, descr_i_D)
	};
	return test_float_value(func, &is_nan_descr, value);
}

jit_value_t
jit_insn_is_finite(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const is_finite_descr = {
		0, 0, 0, 0,
		JIT_OP_IS_FFINITE,
		JIT_OP_IS_DFINITE,
		JIT_OP_IS_NFFINITE,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_is_finite, descr_i_f),
		jit_intrinsic(jit_float64_is_finite, descr_i_d),
		jit_intrinsic(jit_nfloat_is_finite, descr_i_D)
	};
	return test_float_value(func, &is_finite_descr, value);
}

jit_value_t
jit_insn_is_inf(jit_function_t func, jit_value_t value)
{
	static jit_opcode_descr const is_inf_descr = {
		0, 0, 0, 0,
		JIT_OP_IS_FINF,
		JIT_OP_IS_DINF,
		JIT_OP_IS_NFINF,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_no_intrinsic,
		jit_intrinsic(jit_float32_is_inf, descr_i_f),
		jit_intrinsic(jit_float64_is_inf, descr_i_d),
		jit_intrinsic(jit_nfloat_is_inf, descr_i_D)
	};
	return test_float_value(func, &is_inf_descr, value);
}

/*@
 * @deftypefun jit_value_t jit_insn_abs (jit_function_t @var{func}, jit_value_t @var{value1})
 * @deftypefunx jit_value_t jit_insn_min (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * @deftypefunx jit_value_t jit_insn_max (jit_function_t @var{func}, jit_value_t @var{value1}, jit_value_t @var{value2})
 * @deftypefunx jit_value_t jit_insn_sign (jit_function_t @var{func}, jit_value_t @var{value1})
 * Calculate the absolute value, minimum, maximum, or sign of the
 * specified values.
 * @end deftypefun
@*/
jit_value_t
jit_insn_abs(jit_function_t func, jit_value_t value)
{
	int oper;
	void *intrinsic;
	const char *name;
	const jit_intrinsic_descr_t *descr;

	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));
	switch (type->kind)
	{
	case JIT_TYPE_INT:
		oper = JIT_OP_IABS;
		intrinsic = (void *) jit_int_abs;
		name = "jit_int_abs";
		descr = &descr_i_i;
		break;
	case JIT_TYPE_UINT:
		oper = 0;
		intrinsic = (void *) 0;
		name = 0;
		descr = 0;
		break;
	case JIT_TYPE_LONG:
		oper = JIT_OP_LABS;
		intrinsic = (void *) jit_long_abs;
		name = "jit_long_abs";
		descr = &descr_l_l;
		break;
	case JIT_TYPE_ULONG:
		oper = 0;
		intrinsic = (void *) 0;
		name = 0;
		descr = 0;
		break;
	case JIT_TYPE_FLOAT32:
		oper = JIT_OP_FABS;
		intrinsic = (void *) jit_float32_abs;
		name = "jit_float32_abs";
		descr = &descr_f_f;
		break;
	case JIT_TYPE_FLOAT64:
		oper = JIT_OP_DABS;
		intrinsic = (void *) jit_float64_abs;
		name = "jit_float64_abs";
		descr = &descr_d_d;
		break;
	case JIT_TYPE_NFLOAT:
		oper = JIT_OP_NFABS;
		intrinsic = (void *) jit_nfloat_abs;
		name = "jit_nfloat_abs";
		descr = &descr_D_D;
		break;
	default:
		return 0;
	}

	value = jit_insn_convert(func, value, type, 0);
	if(!value)
	{
		return 0;
	}
	if(!oper)
	{
		/* Absolute value of an unsigned value */
		return value;
	}
	if(jit_value_is_constant(value))
	{
		jit_value_t result = _jit_opcode_apply_unary(func, oper, value, type);
		if(result)
		{
			return result;
		}
	}

	if(!_jit_opcode_is_supported(oper))
	{
		return jit_insn_call_intrinsic(func, name, intrinsic, descr, value, 0);
	}
	return apply_unary(func, oper, value, type);
}

jit_value_t
jit_insn_min(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const min_descr = {
		JIT_OP_IMIN,
		JIT_OP_IMIN_UN,
		JIT_OP_LMIN,
		JIT_OP_LMIN_UN,
		JIT_OP_FMIN,
		JIT_OP_DMIN,
		JIT_OP_NFMIN,
		jit_intrinsic(jit_int_min, descr_i_ii),
		jit_intrinsic(jit_uint_min, descr_I_II),
		jit_intrinsic(jit_long_min, descr_l_ll),
		jit_intrinsic(jit_ulong_min, descr_L_LL),
		jit_intrinsic(jit_float32_min, descr_f_ff),
		jit_intrinsic(jit_float64_min, descr_d_dd),
		jit_intrinsic(jit_nfloat_min, descr_D_DD)
	};
	return apply_arith(func, &min_descr, value1, value2, 0, 0, 0);
}

jit_value_t
jit_insn_max(jit_function_t func, jit_value_t value1, jit_value_t value2)
{
	static jit_opcode_descr const max_descr = {
		JIT_OP_IMAX,
		JIT_OP_IMAX_UN,
		JIT_OP_LMAX,
		JIT_OP_LMAX_UN,
		JIT_OP_FMAX,
		JIT_OP_DMAX,
		JIT_OP_NFMAX,
		jit_intrinsic(jit_int_max, descr_i_ii),
		jit_intrinsic(jit_uint_max, descr_I_II),
		jit_intrinsic(jit_long_max, descr_l_ll),
		jit_intrinsic(jit_ulong_max, descr_L_LL),
		jit_intrinsic(jit_float32_max, descr_f_ff),
		jit_intrinsic(jit_float64_max, descr_d_dd),
		jit_intrinsic(jit_nfloat_max, descr_D_DD)
	};
	return apply_arith(func, &max_descr, value1, value2, 0, 0, 0);
}

jit_value_t
jit_insn_sign(jit_function_t func, jit_value_t value)
{
	int oper;
	void *intrinsic;
	const char *name;
	const jit_intrinsic_descr_t *descr;
	jit_value_t zero;

	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));
	switch (type->kind)
	{
	case JIT_TYPE_INT:
		oper = JIT_OP_ISIGN;
		intrinsic = (void *) jit_int_sign;
		name = "jit_int_sign";
		descr = &descr_i_i;
		break;
	case JIT_TYPE_UINT:
		zero = jit_value_create_nint_constant(func, jit_type_uint, 0);
		if(!zero)
		{
			return 0;
		}
		return jit_insn_ne(func, value, zero);
	case JIT_TYPE_LONG:
		oper = JIT_OP_LSIGN;
		intrinsic = (void *) jit_long_sign;
		name = "jit_long_sign";
		descr = &descr_i_l;
		break;
	case JIT_TYPE_ULONG:
		zero = jit_value_create_long_constant(func, jit_type_ulong, 0);
		if(!zero)
		{
			return 0;
		}
		return jit_insn_ne(func, value, zero);
	case JIT_TYPE_FLOAT32:
		oper = JIT_OP_FSIGN;
		intrinsic = (void *) jit_float32_sign;
		name = "jit_float32_sign";
		descr = &descr_i_f;
		break;
	case JIT_TYPE_FLOAT64:
		oper = JIT_OP_DSIGN;
		intrinsic = (void *) jit_float64_sign;
		name = "jit_float64_sign";
		descr = &descr_i_d;
		break;
	case JIT_TYPE_NFLOAT:
		oper = JIT_OP_NFSIGN;
		intrinsic = (void *) jit_nfloat_sign;
		name = "jit_nfloat_sign";
		descr = &descr_i_D;
		break;
	default:
		return 0;
	}

	value = jit_insn_convert(func, value, type, 0);
	if(!value)
	{
		return 0;
	}
	if(jit_value_is_constant(value))
	{
		jit_value_t result = _jit_opcode_apply_unary(func, oper, value, type);
		if(result)
		{
			return result;
		}
	}

	if(!_jit_opcode_is_supported(oper))
	{
		return jit_insn_call_intrinsic(func, name, intrinsic, descr, value, 0);
	}
	return apply_unary(func, oper, value, jit_type_int);
}

/*@
 * @deftypefun int jit_insn_branch (jit_function_t @var{func}, jit_label_t *@var{label})
 * Terminate the current block by branching unconditionally
 * to a specific label.  Returns zero if out of memory.
 * @end deftypefun
@*/
int
jit_insn_branch(jit_function_t func, jit_label_t *label)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Allocate a new label identifier, if necessary */
	if(*label == jit_label_undefined)
	{
		*label = func->builder->next_label++;
	}

	/* Add a new branch instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) JIT_OP_BR;
	insn->flags = JIT_INSN_DEST_IS_LABEL;
	insn->dest = (jit_value_t) *label;
	func->builder->current_block->ends_in_dead = 1;

	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_branch_if (jit_function_t @var{func}, jit_value_t @var{value}, jit_label_t *@var{label})
 * Terminate the current block by branching to a specific label if
 * the specified value is non-zero.  Returns zero if out of memory.
 *
 * If @var{value} refers to a conditional expression that was created
 * by @code{jit_insn_eq}, @code{jit_insn_ne}, etc, then the conditional
 * expression will be replaced by an appropriate conditional branch
 * instruction.
 * @end deftypefun
@*/
int
jit_insn_branch_if(jit_function_t func, jit_value_t value, jit_label_t *label)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Allocate a new label identifier, if necessary */
	if(*label == jit_label_undefined)
	{
		*label = func->builder->next_label++;
	}

	/* If the condition is constant, then convert it into either
	   an unconditional branch or a fall-through, as appropriate */
	if(jit_value_is_constant(value))
	{
		if(jit_value_is_true(value))
		{
			return jit_insn_branch(func, label);
		}
		else
		{
			return 1;
		}
	}

	/* Determine if we can replace a previous comparison instruction */
	int opcode;
	jit_value_t value1;
	jit_value_t value2;
	jit_block_t block = func->builder->current_block;
	jit_insn_t prev = _jit_block_get_last(block);
	if(value->is_temporary && prev && prev->dest == value)
	{
		opcode = prev->opcode;
		if(opcode >= JIT_OP_IEQ && opcode <= JIT_OP_NFGE_INV)
		{
			switch(opcode)
			{
			case JIT_OP_IEQ:	opcode = JIT_OP_BR_IEQ;      break;
			case JIT_OP_INE:	opcode = JIT_OP_BR_INE;      break;
			case JIT_OP_ILT:	opcode = JIT_OP_BR_ILT;      break;
			case JIT_OP_ILT_UN:	opcode = JIT_OP_BR_ILT_UN;   break;
			case JIT_OP_ILE:	opcode = JIT_OP_BR_ILE;      break;
			case JIT_OP_ILE_UN:	opcode = JIT_OP_BR_ILE_UN;   break;
			case JIT_OP_IGT:	opcode = JIT_OP_BR_IGT;      break;
			case JIT_OP_IGT_UN:	opcode = JIT_OP_BR_IGT_UN;   break;
			case JIT_OP_IGE:	opcode = JIT_OP_BR_IGE;      break;
			case JIT_OP_IGE_UN:	opcode = JIT_OP_BR_IGE_UN;   break;
			case JIT_OP_LEQ:	opcode = JIT_OP_BR_LEQ;      break;
			case JIT_OP_LNE:	opcode = JIT_OP_BR_LNE;      break;
			case JIT_OP_LLT:	opcode = JIT_OP_BR_LLT;      break;
			case JIT_OP_LLT_UN:	opcode = JIT_OP_BR_LLT_UN;   break;
			case JIT_OP_LLE:	opcode = JIT_OP_BR_LLE;      break;
			case JIT_OP_LLE_UN:	opcode = JIT_OP_BR_LLE_UN;   break;
			case JIT_OP_LGT:	opcode = JIT_OP_BR_LGT;      break;
			case JIT_OP_LGT_UN:	opcode = JIT_OP_BR_LGT_UN;   break;
			case JIT_OP_LGE:	opcode = JIT_OP_BR_LGE;      break;
			case JIT_OP_LGE_UN:	opcode = JIT_OP_BR_LGE_UN;   break;
			case JIT_OP_FEQ:	opcode = JIT_OP_BR_FEQ;      break;
			case JIT_OP_FNE:	opcode = JIT_OP_BR_FNE;      break;
			case JIT_OP_FLT:	opcode = JIT_OP_BR_FLT;      break;
			case JIT_OP_FLE:	opcode = JIT_OP_BR_FLE;      break;
			case JIT_OP_FGT:	opcode = JIT_OP_BR_FGT;      break;
			case JIT_OP_FGE:	opcode = JIT_OP_BR_FGE;      break;
			case JIT_OP_FLT_INV:	opcode = JIT_OP_BR_FLT_INV;  break;
			case JIT_OP_FLE_INV:	opcode = JIT_OP_BR_FLE_INV;  break;
			case JIT_OP_FGT_INV:	opcode = JIT_OP_BR_FGT_INV;  break;
			case JIT_OP_FGE_INV:	opcode = JIT_OP_BR_FGE_INV;  break;
			case JIT_OP_DEQ:	opcode = JIT_OP_BR_DEQ;      break;
			case JIT_OP_DNE:	opcode = JIT_OP_BR_DNE;      break;
			case JIT_OP_DLT:	opcode = JIT_OP_BR_DLT;      break;
			case JIT_OP_DLE:	opcode = JIT_OP_BR_DLE;      break;
			case JIT_OP_DGT:	opcode = JIT_OP_BR_DGT;      break;
			case JIT_OP_DGE:	opcode = JIT_OP_BR_DGE;      break;
			case JIT_OP_DLT_INV:	opcode = JIT_OP_BR_DLT_INV;  break;
			case JIT_OP_DLE_INV:	opcode = JIT_OP_BR_DLE_INV;  break;
			case JIT_OP_DGT_INV:	opcode = JIT_OP_BR_DGT_INV;  break;
			case JIT_OP_DGE_INV:	opcode = JIT_OP_BR_DGE_INV;  break;
			case JIT_OP_NFEQ:	opcode = JIT_OP_BR_NFEQ;     break;
			case JIT_OP_NFNE:	opcode = JIT_OP_BR_NFNE;     break;
			case JIT_OP_NFLT:	opcode = JIT_OP_BR_NFLT;     break;
			case JIT_OP_NFLE:	opcode = JIT_OP_BR_NFLE;     break;
			case JIT_OP_NFGT:	opcode = JIT_OP_BR_NFGT;     break;
			case JIT_OP_NFGE:	opcode = JIT_OP_BR_NFGE;     break;
			case JIT_OP_NFLT_INV:	opcode = JIT_OP_BR_NFLT_INV; break;
			case JIT_OP_NFLE_INV:	opcode = JIT_OP_BR_NFLE_INV; break;
			case JIT_OP_NFGT_INV:	opcode = JIT_OP_BR_NFGT_INV; break;
			case JIT_OP_NFGE_INV:	opcode = JIT_OP_BR_NFGE_INV; break;
			}

			/* Save the values from the previous insn because *prev might
			   become invalid if the call to _jit_block_add_insn triggers
			   a reallocation of the insns array. */
			value1 = prev->value1;
			value2 = prev->value2;

			/* Add a new branch instruction */
			jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
			if(!insn)
			{
				return 0;
			}
			insn->opcode = (short) opcode;
			insn->flags = JIT_INSN_DEST_IS_LABEL;
			insn->dest = (jit_value_t) *label;
			insn->value1 = value1;
			jit_value_ref(func, value1);
			insn->value2 = value2;
			jit_value_ref(func, value2);

			goto add_block;
		}
	}

	/* Coerce the result to something comparable */
	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));
	value = jit_insn_convert(func, value, type, 0);
	if(!value)
	{
		return 0;
	}

	/* Determine the opcode */
	switch(type->kind)
	{
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		opcode = JIT_OP_BR_ITRUE;
		value2 = 0;
		break;
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		opcode = JIT_OP_BR_LTRUE;
		value2 = 0;
		break;
	case JIT_TYPE_FLOAT32:
		opcode = JIT_OP_BR_FNE;
		value2 = jit_value_create_float32_constant(func, jit_type_float32,
							   (jit_float32) 0.0);
		if(!value2)
		{
			return 0;
		}
		break;
	case JIT_TYPE_FLOAT64:
		opcode = JIT_OP_BR_DNE;
		value2 = jit_value_create_float64_constant(func, jit_type_float64,
							   (jit_float64) 0.0);
		if(!value2)
		{
			return 0;
		}
		break;
	case JIT_TYPE_NFLOAT:
		type = jit_type_nfloat;
		opcode = JIT_OP_BR_NFNE;
		value2 = jit_value_create_nfloat_constant(func, jit_type_nfloat,
							  (jit_nfloat) 0.0);
		if(!value2)
		{
			return 0;
		}
		break;
	default:
		return 0;
	}

	/* Add a new branch instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) opcode;
	insn->flags = JIT_INSN_DEST_IS_LABEL;
	insn->dest = (jit_value_t) *label;
	insn->value1 = value;
	jit_value_ref(func, value);
	if(value2)
	{
		insn->value2 = value2;
		jit_value_ref(func, value2);
	}

add_block:
	/* Add a new block for the fall-through case */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_branch_if_not (jit_function_t @var{func}, jit_value_t @var{value}, jit_label_t *@var{label})
 * Terminate the current block by branching to a specific label if
 * the specified value is zero.  Returns zero if out of memory.
 *
 * If @var{value} refers to a conditional expression that was created
 * by @code{jit_insn_eq}, @code{jit_insn_ne}, etc, then the conditional
 * expression will be followed by an appropriate conditional branch
 * instruction, instead of a value load.
 * @end deftypefun
@*/
int
jit_insn_branch_if_not(jit_function_t func, jit_value_t value, jit_label_t *label)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Allocate a new label identifier, if necessary */
	if(*label == jit_label_undefined)
	{
		*label = func->builder->next_label++;
	}

	/* If the condition is constant, then convert it into either
	   an unconditional branch or a fall-through, as appropriate */
	if(jit_value_is_constant(value))
	{
		if(!jit_value_is_true(value))
		{
			return jit_insn_branch(func, label);
		}
		else
		{
			return 1;
		}
	}

	/* Determine if we can duplicate a previous comparison instruction */
	int opcode;
	jit_value_t value1;
	jit_value_t value2;
	jit_block_t block = func->builder->current_block;
	jit_insn_t prev = _jit_block_get_last(block);
	if(value->is_temporary && prev && prev->dest == value)
	{
		opcode = prev->opcode;
		if(opcode >= JIT_OP_IEQ && opcode <= JIT_OP_NFGE_INV)
		{
			switch(opcode)
			{
			case JIT_OP_IEQ:	opcode = JIT_OP_BR_INE;      break;
			case JIT_OP_INE:	opcode = JIT_OP_BR_IEQ;      break;
			case JIT_OP_ILT:	opcode = JIT_OP_BR_IGE;      break;
			case JIT_OP_ILT_UN:	opcode = JIT_OP_BR_IGE_UN;   break;
			case JIT_OP_ILE:	opcode = JIT_OP_BR_IGT;      break;
			case JIT_OP_ILE_UN:	opcode = JIT_OP_BR_IGT_UN;   break;
			case JIT_OP_IGT:	opcode = JIT_OP_BR_ILE;      break;
			case JIT_OP_IGT_UN:	opcode = JIT_OP_BR_ILE_UN;   break;
			case JIT_OP_IGE:	opcode = JIT_OP_BR_ILT;      break;
			case JIT_OP_IGE_UN:	opcode = JIT_OP_BR_ILT_UN;   break;
			case JIT_OP_LEQ:	opcode = JIT_OP_BR_LNE;      break;
			case JIT_OP_LNE:	opcode = JIT_OP_BR_LEQ;      break;
			case JIT_OP_LLT:	opcode = JIT_OP_BR_LGE;      break;
			case JIT_OP_LLT_UN:	opcode = JIT_OP_BR_LGE_UN;   break;
			case JIT_OP_LLE:	opcode = JIT_OP_BR_LGT;      break;
			case JIT_OP_LLE_UN:	opcode = JIT_OP_BR_LGT_UN;   break;
			case JIT_OP_LGT:	opcode = JIT_OP_BR_LLE;      break;
			case JIT_OP_LGT_UN:	opcode = JIT_OP_BR_LLE_UN;   break;
			case JIT_OP_LGE:	opcode = JIT_OP_BR_LLT;      break;
			case JIT_OP_LGE_UN:	opcode = JIT_OP_BR_LLT_UN;   break;
			case JIT_OP_FEQ:	opcode = JIT_OP_BR_FNE;      break;
			case JIT_OP_FNE:	opcode = JIT_OP_BR_FEQ;      break;
			case JIT_OP_FLT:	opcode = JIT_OP_BR_FGE_INV;  break;
			case JIT_OP_FLE:	opcode = JIT_OP_BR_FGT_INV;  break;
			case JIT_OP_FGT:	opcode = JIT_OP_BR_FLE_INV;  break;
			case JIT_OP_FGE:	opcode = JIT_OP_BR_FLT_INV;  break;
			case JIT_OP_FLT_INV:	opcode = JIT_OP_BR_FGE;      break;
			case JIT_OP_FLE_INV:	opcode = JIT_OP_BR_FGT;      break;
			case JIT_OP_FGT_INV:	opcode = JIT_OP_BR_FLE;      break;
			case JIT_OP_FGE_INV:	opcode = JIT_OP_BR_FLT;      break;
			case JIT_OP_DEQ:	opcode = JIT_OP_BR_DNE;      break;
			case JIT_OP_DNE:	opcode = JIT_OP_BR_DEQ;      break;
			case JIT_OP_DLT:	opcode = JIT_OP_BR_DGE_INV;  break;
			case JIT_OP_DLE:	opcode = JIT_OP_BR_DGT_INV;  break;
			case JIT_OP_DGT:	opcode = JIT_OP_BR_DLE_INV;  break;
			case JIT_OP_DGE:	opcode = JIT_OP_BR_DLT_INV;  break;
			case JIT_OP_DLT_INV:	opcode = JIT_OP_BR_DGE;      break;
			case JIT_OP_DLE_INV:	opcode = JIT_OP_BR_DGT;      break;
			case JIT_OP_DGT_INV:	opcode = JIT_OP_BR_DLE;      break;
			case JIT_OP_DGE_INV:	opcode = JIT_OP_BR_DLT;      break;
			case JIT_OP_NFEQ:	opcode = JIT_OP_BR_NFNE;     break;
			case JIT_OP_NFNE:	opcode = JIT_OP_BR_NFEQ;     break;
			case JIT_OP_NFLT:	opcode = JIT_OP_BR_NFGE_INV; break;
			case JIT_OP_NFLE:	opcode = JIT_OP_BR_NFGT_INV; break;
			case JIT_OP_NFGT:	opcode = JIT_OP_BR_NFLE_INV; break;
			case JIT_OP_NFGE:	opcode = JIT_OP_BR_NFLT_INV; break;
			case JIT_OP_NFLT_INV:	opcode = JIT_OP_BR_NFGE;     break;
			case JIT_OP_NFLE_INV:	opcode = JIT_OP_BR_NFGT;     break;
			case JIT_OP_NFGT_INV:	opcode = JIT_OP_BR_NFLE;     break;
			case JIT_OP_NFGE_INV:	opcode = JIT_OP_BR_NFLT;     break;
			}

			/* Save the values from the previous insn because *prev might
			   become invalid if the call to _jit_block_add_insn triggers
			   a reallocation of the insns array. */
			value1 = prev->value1;
			value2 = prev->value2;

			/* Add a new branch instruction */
			jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
			if(!insn)
			{
				return 0;
			}
			insn->opcode = (short) opcode;
			insn->flags = JIT_INSN_DEST_IS_LABEL;
			insn->dest = (jit_value_t) *label;
			insn->value1 = value1;
			jit_value_ref(func, value1);
			insn->value2 = value2;
			jit_value_ref(func, value2);

			goto add_block;
		}
	}

	/* Coerce the result to something comparable */
	jit_type_t type = jit_type_promote_int(jit_type_normalize(value->type));
	value = jit_insn_convert(func, value, type, 0);
	if(!value)
	{
		return 0;
	}

	/* Determine the opcode */
	switch(type->kind)
	{
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		opcode = JIT_OP_BR_IFALSE;
		value2 = 0;
		break;
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		opcode = JIT_OP_BR_LFALSE;
		value2 = 0;
		break;
	case JIT_TYPE_FLOAT32:
		opcode = JIT_OP_BR_FEQ;
		value2 = jit_value_create_float32_constant(func, jit_type_float32,
							   (jit_float32) 0.0);
		if(!value2)
		{
			return 0;
		}
		break;
	case JIT_TYPE_FLOAT64:
		opcode = JIT_OP_BR_DEQ;
		value2 = jit_value_create_float64_constant(func, jit_type_float64,
							   (jit_float64) 0.0);
		if(!value2)
		{
			return 0;
		}
		break;
	case JIT_TYPE_NFLOAT:
		opcode = JIT_OP_BR_NFEQ;
		value2 = jit_value_create_nfloat_constant(func, jit_type_nfloat,
							  (jit_nfloat) 0.0);
		if(!value2)
		{
			return 0;
		}
		break;
	default:
		return 0;
	}

	/* Add a new branch instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) opcode;
	insn->flags = JIT_INSN_DEST_IS_LABEL;
	insn->dest = (jit_value_t) *label;
	insn->value1 = value;
	jit_value_ref(func, value);
	if(value2)
	{
		insn->value2 = value2;
		jit_value_ref(func, value2);
	}

add_block:
	/* Add a new block for the fall-through case */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_jump_table (jit_function_t @var{func}, jit_value_t @var{value}, jit_label_t *@var{labels}, unsigned int @var{num_labels})
 * Branch to a label from the @var{labels} table. The @var{value} is the
 * index of the label. It is allowed to have identical labels in the table.
 * If an entry in the table has @code{jit_label_undefined} value then it is
 * replaced with a newly allocated label.
 * @end deftypefun
@*/
int
jit_insn_jump_table(jit_function_t func, jit_value_t value,
		    jit_label_t *labels, unsigned int num_labels)
{
	/* Bail out if the parameters are invalid */
	if(!num_labels)
	{
		return 0;
	}

	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Allocate new label identifiers, if necessary */
	unsigned int index;
	for(index = 0; index < num_labels; index++)
	{
		if(labels[index] == jit_label_undefined)
		{
			labels[index] = func->builder->next_label++;
		}
	}

	/* If the condition is constant, then convert it into either
	   an unconditional branch or a fall-through, as appropriate */
	if(jit_value_is_constant(value))
	{
		index = jit_value_get_nint_constant(value);
		if(index < num_labels && index >= 0)
		{
			return jit_insn_branch(func, &labels[index]);
		}
		else
		{
			return 1;
		}
	}

	jit_label_t *new_labels = jit_malloc(num_labels * sizeof(jit_label_t));
	if(!new_labels)
	{
		return 0;
	}
	for(index = 0; index < num_labels; index++)
	{
		new_labels[index] = labels[index];
	}

	jit_value_t value_labels =
		jit_value_create_nint_constant(func, jit_type_void_ptr, (jit_nint) new_labels);
	if(!value_labels)
	{
		jit_free(new_labels);
		return 0;
	}
	value_labels->free_address = 1;

	jit_value_t value_num_labels =
		jit_value_create_nint_constant(func, jit_type_uint, num_labels);
	if(!value_num_labels)
	{
		_jit_value_free(value_labels);
		return 0;
	}

	/* Add a new branch instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = JIT_OP_JUMP_TABLE;
	insn->flags = JIT_INSN_DEST_IS_VALUE;
	insn->dest = value;
	jit_value_ref(func, value);
	insn->value1 = value_labels;
	insn->value2 = value_num_labels;

	/* Add a new block for the fall-through case */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun jit_value_t jit_insn_address_of (jit_function_t @var{func}, jit_value_t @var{value1})
 * Get the address of a value into a new temporary.
 * @end deftypefun
@*/
jit_value_t
jit_insn_address_of(jit_function_t func, jit_value_t value)
{
	if(jit_value_is_constant(value))
	{
		return 0;
	}
	jit_type_t type = jit_type_create_pointer(jit_value_get_type(value), 1);
	if(!type)
	{
		return 0;
	}
	jit_value_set_addressable(value);
	jit_value_t result = apply_unary(func, JIT_OP_ADDRESS_OF, value, type);
	jit_type_free(type);
	return result;
}

/*@
 * @deftypefun jit_value_t jit_insn_address_of_label (jit_function_t @var{func}, jit_label_t *@var{label})
 * Get the address of @var{label} into a new temporary.  This is typically
 * used for exception handling, to track where in a function an exception
 * was actually thrown.
 * @end deftypefun
@*/
jit_value_t
jit_insn_address_of_label(jit_function_t func, jit_label_t *label)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Allocate a new label identifier, if necessary */
	if(*label == jit_label_undefined)
	{
		*label = func->builder->next_label++;
	}
	if(!_jit_block_record_label_flags(func, *label, JIT_LABEL_ADDRESS_OF))
	{
		return 0;
	}

	/* Add a new address-of-label instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	jit_value_t dest = jit_value_create(func, jit_type_void_ptr);
	if(!dest)
	{
		return 0;
	}
	insn->opcode = (short) JIT_OP_ADDRESS_OF_LABEL;
	insn->flags = JIT_INSN_VALUE1_IS_LABEL;
	insn->dest = dest;
	insn->value1 = (jit_value_t) *label;

	return dest;
}

/*
 * Information about the opcodes for a particular conversion.
 */
typedef struct jit_convert_info
{
	int		cvt1;
	jit_type_t	type1;
	int		cvt2;
	jit_type_t	type2;
	int		cvt3;
	jit_type_t	type3;

} jit_convert_info_t;

#define	CVT(opcode,name)	opcode, (jit_type_t) &_jit_type_##name##_def
#define	CVT_NONE		0, 0

/*
 * Intrinsic equivalents for the conversion opcodes.
 */
typedef struct jit_convert_intrinsic
{
	const char *name;
	void	   *func;
	jit_intrinsic_descr_t descr;

} jit_convert_intrinsic_t;

#define	CVT_INTRINSIC_NULL				\
	{0, 0, {0, 0, 0, 0}}

#define	CVT_INTRINSIC(name, intype, outtype)		\
	{ #name, (void *) name, 			\
	  { (jit_type_t) &_jit_type_##outtype##_def, 0, \
	    (jit_type_t) &_jit_type_##intype##_def, 0 } }

#define	CVT_INTRINSIC_CHECK(name, intype, outtype)	\
	{ #name, (void *) name,				\
	  { (jit_type_t) &_jit_type_int_def,		\
	    (jit_type_t) &_jit_type_##outtype##_def, 	\
	    (jit_type_t) &_jit_type_##intype##_def, 0 } }

static jit_convert_intrinsic_t const convert_intrinsics[] = {
	CVT_INTRINSIC(jit_int_to_sbyte, int, int),
	CVT_INTRINSIC(jit_int_to_ubyte, int, int),
	CVT_INTRINSIC(jit_int_to_short, int, int),
	CVT_INTRINSIC(jit_int_to_ushort, int, int),
#ifdef JIT_NATIVE_INT32
	CVT_INTRINSIC(jit_int_to_int, int, int),
	CVT_INTRINSIC(jit_uint_to_uint, uint, uint),
#else
	CVT_INTRINSIC(jit_long_to_int, long, int),
	CVT_INTRINSIC(jit_long_to_uint, long, uint),
#endif
	CVT_INTRINSIC_CHECK(jit_int_to_sbyte_ovf, int, int),
	CVT_INTRINSIC_CHECK(jit_int_to_ubyte_ovf, int, int),
	CVT_INTRINSIC_CHECK(jit_int_to_short_ovf, int, int),
	CVT_INTRINSIC_CHECK(jit_int_to_ushort_ovf, int, int),
#ifdef JIT_NATIVE_INT32
	CVT_INTRINSIC_CHECK(jit_int_to_int_ovf, int, int),
	CVT_INTRINSIC_CHECK(jit_uint_to_uint_ovf, uint, uint),
#else
	CVT_INTRINSIC_CHECK(jit_long_to_int_ovf, long, int),
	CVT_INTRINSIC_CHECK(jit_long_to_uint_ovf, long, uint),
#endif
	CVT_INTRINSIC(jit_long_to_uint, long, uint),
	CVT_INTRINSIC(jit_int_to_long, int, long),
	CVT_INTRINSIC(jit_uint_to_long, uint, long),
	CVT_INTRINSIC_CHECK(jit_long_to_uint_ovf, long, uint),
	CVT_INTRINSIC_CHECK(jit_long_to_int_ovf, long, int),
	CVT_INTRINSIC_CHECK(jit_ulong_to_long_ovf, ulong, long),
	CVT_INTRINSIC_CHECK(jit_long_to_ulong_ovf, long, ulong),
	CVT_INTRINSIC(jit_float32_to_int, float32, int),
	CVT_INTRINSIC(jit_float32_to_uint, float32, uint),
	CVT_INTRINSIC(jit_float32_to_long, float32, long),
	CVT_INTRINSIC(jit_float32_to_ulong, float32, ulong),
	CVT_INTRINSIC_CHECK(jit_float32_to_int_ovf, float32, int),
	CVT_INTRINSIC_CHECK(jit_float32_to_uint_ovf, float32, uint),
	CVT_INTRINSIC_CHECK(jit_float32_to_long_ovf, float32, long),
	CVT_INTRINSIC_CHECK(jit_float32_to_ulong_ovf, float32, ulong),
	CVT_INTRINSIC(jit_int_to_float32, int, float32),
	CVT_INTRINSIC(jit_uint_to_float32, uint, float32),
	CVT_INTRINSIC(jit_long_to_float32, long, float32),
	CVT_INTRINSIC(jit_ulong_to_float32, ulong, float32),
	CVT_INTRINSIC(jit_float32_to_float64, float32, float64),
	CVT_INTRINSIC(jit_float64_to_int, float64, int),
	CVT_INTRINSIC(jit_float64_to_uint, float64, uint),
	CVT_INTRINSIC(jit_float64_to_long, float64, long),
	CVT_INTRINSIC(jit_float64_to_ulong, float64, ulong),
	CVT_INTRINSIC_CHECK(jit_float64_to_int_ovf, float64, int),
	CVT_INTRINSIC_CHECK(jit_float64_to_uint_ovf, float64, uint),
	CVT_INTRINSIC_CHECK(jit_float64_to_long_ovf, float64, long),
	CVT_INTRINSIC_CHECK(jit_float64_to_ulong_ovf, float64, ulong),
	CVT_INTRINSIC(jit_int_to_float64, int, float64),
	CVT_INTRINSIC(jit_uint_to_float64, uint, float64),
	CVT_INTRINSIC(jit_long_to_float64, long, float64),
	CVT_INTRINSIC(jit_ulong_to_float64, ulong, float64),
	CVT_INTRINSIC(jit_float64_to_float32, float64, float32),
	CVT_INTRINSIC(jit_nfloat_to_int, nfloat, int),
	CVT_INTRINSIC(jit_nfloat_to_uint, nfloat, uint),
	CVT_INTRINSIC(jit_nfloat_to_long, nfloat, long),
	CVT_INTRINSIC(jit_nfloat_to_ulong, nfloat, ulong),
	CVT_INTRINSIC_CHECK(jit_nfloat_to_int_ovf, nfloat, int),
	CVT_INTRINSIC_CHECK(jit_nfloat_to_uint_ovf, nfloat, uint),
	CVT_INTRINSIC_CHECK(jit_nfloat_to_long_ovf, nfloat, long),
	CVT_INTRINSIC_CHECK(jit_nfloat_to_ulong_ovf, nfloat, ulong),
	CVT_INTRINSIC(jit_int_to_nfloat, int, nfloat),
	CVT_INTRINSIC(jit_uint_to_nfloat, uint, nfloat),
	CVT_INTRINSIC(jit_long_to_nfloat, long, nfloat),
	CVT_INTRINSIC(jit_ulong_to_nfloat, ulong, nfloat),
	CVT_INTRINSIC(jit_nfloat_to_float32, nfloat, float32),
	CVT_INTRINSIC(jit_nfloat_to_float64, nfloat, float64),
	CVT_INTRINSIC(jit_float32_to_nfloat, float32, nfloat),
	CVT_INTRINSIC(jit_float64_to_nfloat, float64, nfloat)
};

/*
 * Apply a unary conversion operator.
 */
static jit_value_t
apply_conversion(jit_function_t func, int oper, jit_value_t value,
		       jit_type_t result_type)
{
	/* Set the "may_throw" flag if the conversion may throw an exception */
	if(oper < sizeof(convert_intrinsics) / sizeof(jit_convert_intrinsic_t)
		&& convert_intrinsics[oper - 1].descr.ptr_result_type)
	{
		func->builder->may_throw = 1;
	}

	/* Bail out early if the conversion opcode is supported by the back end */
	if(_jit_opcode_is_supported(oper))
	{
		return apply_unary(func, oper, value, result_type);
	}

	/* Call the appropriate intrinsic method */
	return jit_insn_call_intrinsic(func, convert_intrinsics[oper - 1].name,
				       convert_intrinsics[oper - 1].func,
				       &convert_intrinsics[oper - 1].descr,
				       value, 0);
}

/*@
 * @deftypefun jit_value_t jit_insn_convert (jit_function_t @var{func}, jit_value_t @var{value}, jit_type_t @var{type}, int @var{overflow_check})
 * Convert the contents of a value into a new type, with optional
 * overflow checking.
 * @end deftypefun
@*/
jit_value_t
jit_insn_convert(jit_function_t func, jit_value_t value, jit_type_t type,
		 int overflow_check)
{
	/* Normalize the source and destination types */
	jit_type_t vtype = jit_type_normalize(value->type);
	type = jit_type_normalize(type);

	/* If the types are identical, then return the source value as-is */
	if(type == vtype)
	{
		return value;
	}

	/* If the source is a constant, then perform a constant conversion.
	   If an overflow might result, we perform the computation at runtime */
	if(jit_value_is_constant(value))
	{
		jit_constant_t const_value;
		const_value = jit_value_get_constant(value);
		if(jit_constant_convert(&const_value, &const_value, type, overflow_check))
		{
			return jit_value_create_constant(func, &const_value);
		}
	}

	/* Promote the source type, to reduce the number of cases in
	   the switch statement below */
	vtype = jit_type_promote_int(vtype);

	/* Determine how to perform the conversion */
	const jit_convert_info_t *opcode_map = 0;
	switch(type->kind)
	{
	case JIT_TYPE_SBYTE:
	{
		/* Convert the value into a signed byte */
		static jit_convert_info_t const to_sbyte[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_SBYTE, sbyte),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_INT, int),
			  CVT(JIT_OP_CHECK_SBYTE, sbyte),
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_SIGNED_LOW_WORD, int),
			  CVT(JIT_OP_CHECK_SBYTE, sbyte),
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT(JIT_OP_CHECK_INT, int),
			  CVT(JIT_OP_CHECK_SBYTE, sbyte) },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_CHECK_SBYTE, sbyte),
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_CHECK_SBYTE, sbyte),
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_TRUNC_SBYTE, sbyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_CHECK_SBYTE, sbyte),
			  CVT_NONE }
		};
		opcode_map = to_sbyte;
		break;
	}

	case JIT_TYPE_UBYTE:
	{
		/* Convert the value into an unsigned byte */
		static jit_convert_info_t const to_ubyte[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_SIGNED_LOW_WORD, int),
			  CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_TRUNC_UBYTE, ubyte),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_CHECK_UBYTE, ubyte),
			  CVT_NONE }
		};
		opcode_map = to_ubyte;
		break;
	}

	case JIT_TYPE_SHORT:
	{
		/* Convert the value into a signed short */
		static jit_convert_info_t const to_short[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_SHORT, short),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_INT, int),
			  CVT(JIT_OP_CHECK_SHORT, short),
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_SIGNED_LOW_WORD, int),
			  CVT(JIT_OP_CHECK_SHORT, short),
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT(JIT_OP_CHECK_INT, int),
			  CVT(JIT_OP_CHECK_SHORT, short)},
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_CHECK_SHORT, short),
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_CHECK_SHORT, short),
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_TRUNC_SHORT, short),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_CHECK_SHORT, short),
			  CVT_NONE }
		};
		opcode_map = to_short;
		break;
	}

	case JIT_TYPE_USHORT:
	{
		/* Convert the value into an unsigned short */
		static jit_convert_info_t const to_ushort[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_SIGNED_LOW_WORD, int),
			  CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_LOW_WORD, int),
			  CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_INT, int),
			  CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_INT, int),
			  CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_TRUNC_USHORT, ushort),
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_INT, int),
			  CVT(JIT_OP_CHECK_USHORT, ushort),
			  CVT_NONE }
		};
		opcode_map = to_ushort;
		break;
	}

	case JIT_TYPE_INT:
	{
		/* Convert the value into a signed int */
		static jit_convert_info_t const to_int[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_COPY_INT, int),
			  CVT_NONE,
			  CVT_NONE},
			{ CVT(JIT_OP_COPY_INT, int),
			  CVT_NONE,
			  CVT_NONE},
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
#ifndef JIT_NATIVE_INT32
			{ CVT(JIT_OP_TRUNC_INT, int),
			  CVT_NONE,
			  CVT_NONE},
#else
			{ CVT(JIT_OP_COPY_INT, int),
			  CVT_NONE,
			  CVT_NONE},
#endif
			{ CVT(JIT_OP_CHECK_INT, int),
			  CVT_NONE,
			  CVT_NONE},
			/* from signed long */
			{ CVT(JIT_OP_LOW_WORD, int),
#ifndef JIT_NATIVE_INT32
			  CVT(JIT_OP_TRUNC_INT, int),
#else
			  CVT_NONE,
#endif
			  CVT_NONE},
			{ CVT(JIT_OP_CHECK_SIGNED_LOW_WORD, int),
			  CVT_NONE,
			  CVT_NONE},
			/* from unsigned long */
			{ CVT(JIT_OP_LOW_WORD, int),
#ifndef JIT_NATIVE_INT32
			  CVT(JIT_OP_TRUNC_INT, int),
#else
			  CVT_NONE,
#endif
			  CVT_NONE},
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT(JIT_OP_CHECK_INT, int),
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_INT, int),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_INT, int),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_INT, int),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_INT, int),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_INT, int),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_INT, int),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_int;
		break;
	}

	case JIT_TYPE_UINT:
	{
		/* Convert the value into an unsigned int */
		static jit_convert_info_t const to_uint[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
#ifndef JIT_NATIVE_INT32
			{ CVT(JIT_OP_TRUNC_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
#else
			{ CVT(JIT_OP_COPY_INT, uint),
			  CVT_NONE,
			  CVT_NONE },
#endif
			{ CVT(JIT_OP_CHECK_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_COPY_INT, uint),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_COPY_INT, uint),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LOW_WORD, uint),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_LOW_WORD, uint),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LOW_WORD, uint),
			  CVT_NONE,
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_UINT, uint),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_UINT, uint),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_uint;
		break;
	}

	case JIT_TYPE_LONG:
	{
		/* Convert the value into a signed long */
		static jit_convert_info_t const to_long[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_EXPAND_INT, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_EXPAND_INT, long),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_EXPAND_UINT, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_EXPAND_UINT, long),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_COPY_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_COPY_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_COPY_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_LONG, long),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_LONG, long),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_long;
		break;
	}

	case JIT_TYPE_ULONG:
	{
		/* Convert the value into an unsigned long */
		static jit_convert_info_t const to_ulong[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_EXPAND_INT, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_UINT, uint),
			  CVT(JIT_OP_EXPAND_UINT, ulong),
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_EXPAND_UINT, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_EXPAND_UINT, ulong),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_COPY_LONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_COPY_LONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_COPY_LONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT32_TO_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_FLOAT64_TO_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_CHECK_NFLOAT_TO_ULONG, ulong),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_ulong;
		break;
	}

	case JIT_TYPE_FLOAT32:
	{
		/* Convert the value into a 32-bit float */
		static jit_convert_info_t const to_float32[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_INT_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_INT_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_UINT_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_UINT_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LONG_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_LONG_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_ULONG_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_ULONG_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_COPY_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_COPY_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_FLOAT64_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_NFLOAT_TO_FLOAT32, float32),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_float32;
		break;
	}

	case JIT_TYPE_FLOAT64:
	{
		/* Convert the value into a 64-bit float */
		static jit_convert_info_t const to_float64[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_INT_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_INT_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_UINT_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_UINT_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LONG_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_LONG_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_ULONG_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_ULONG_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_FLOAT32_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_COPY_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_COPY_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_NFLOAT_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_NFLOAT_TO_FLOAT64, float64),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_float64;
		break;
	}

	case JIT_TYPE_NFLOAT:
	{
		/* Convert the value into a native floating-point value */
		static jit_convert_info_t const to_nfloat[] = {
			/* from signed byte */
			/* from signed short */
			/* from signed int */
			{ CVT(JIT_OP_INT_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_INT_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned byte */
			/* from unsigned short */
			/* from unsigned int */
			{ CVT(JIT_OP_UINT_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_UINT_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			/* from signed long */
			{ CVT(JIT_OP_LONG_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_LONG_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			/* from unsigned long */
			{ CVT(JIT_OP_ULONG_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_ULONG_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			/* from 32-bit float */
			{ CVT(JIT_OP_FLOAT32_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_FLOAT32_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			/* from 64-bit float */
			{ CVT(JIT_OP_FLOAT64_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_FLOAT64_TO_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			/* from native float */
			{ CVT(JIT_OP_COPY_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE },
			{ CVT(JIT_OP_COPY_NFLOAT, nfloat),
			  CVT_NONE,
			  CVT_NONE }
		};
		opcode_map = to_nfloat;
		break;
	}
	}

	if(opcode_map)
	{
		switch(vtype->kind)
		{
		case JIT_TYPE_UINT:	opcode_map += 2; break;
		case JIT_TYPE_LONG:	opcode_map += 4; break;
		case JIT_TYPE_ULONG:	opcode_map += 6; break;
		case JIT_TYPE_FLOAT32:	opcode_map += 8; break;
		case JIT_TYPE_FLOAT64:	opcode_map += 10; break;
		case JIT_TYPE_NFLOAT:	opcode_map += 12; break;
		}
		if(overflow_check)
		{
			opcode_map += 1;
		}
		if(opcode_map->cvt1)
		{
			value = apply_conversion(func, opcode_map->cvt1, value,
						 opcode_map->type1);
		}
		if(opcode_map->cvt2 && value)
		{
			value = apply_conversion(func, opcode_map->cvt2, value,
						 opcode_map->type2);
		}
		if(opcode_map->cvt3 && value)
		{
			value = apply_conversion(func, opcode_map->cvt3, value,
						 opcode_map->type3);
		}
	}
	return value;
}

/*
 * Convert the parameters for a function call into their final types.
 */
static int
convert_call_parameters(jit_function_t func, jit_type_t signature,
			jit_value_t *args, unsigned int num_args,
			jit_value_t *new_args)
{
	unsigned int param;
	for(param = 0; param < num_args; ++param)
	{
		new_args[param] = jit_insn_convert(func, args[param],
						   jit_type_get_param(signature, param),
						   0);
		if (!new_args[param])
			return 0;
	}
	return 1;
}

/*
 * Set up the exception frame information before a function call out.
 */
static int
setup_eh_frame_for_call(jit_function_t func, int flags)
{
#if !defined(JIT_BACKEND_INTERP)
	jit_type_t type;
	jit_value_t args[2];
	jit_insn_t insn;

	/* If "tail" is set, then we need to pop the "setjmp" context */
	if((flags & JIT_CALL_TAIL) != 0 && func->has_try)
	{
		type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, 0, 0, 1);
		if(!type)
		{
			return 0;
		}
		jit_insn_call_native(func, "_jit_unwind_pop_setjmp",
				     (void *) _jit_unwind_pop_setjmp, type,
				     0, 0, JIT_CALL_NOTHROW);
		jit_type_free(type);
	}

	/* If "nothrow" or "tail" is set, then there is no more to do */
	if((flags & (JIT_CALL_NOTHROW | JIT_CALL_TAIL)) != 0)
	{
		return 1;
	}

	/* This function may throw an exception */
	func->builder->may_throw = 1;

#if JIT_APPLY_BROKEN_FRAME_BUILTINS != 0
	{
		jit_value_t eh_frame_info;
		jit_type_t params[2];

		/* Get the value that holds the exception frame information */
		if((eh_frame_info = func->builder->eh_frame_info) == 0)
		{
			type = jit_type_create_struct(0, 0, 0);
			if(!type)
			{
				return 0;
			}
			jit_type_set_size_and_alignment(type,
							sizeof(struct jit_backtrace),
							sizeof(void *));
			eh_frame_info = jit_value_create(func, type);
			jit_type_free(type);
			if(!eh_frame_info)
			{
				return 0;
			}
			func->builder->eh_frame_info = eh_frame_info;
		}

		/* Output an instruction to load the "pc" into a value */
		args[1] = jit_value_create(func, jit_type_void_ptr);
		if(!args[1])
		{
			return 0;
		}
		insn = _jit_block_add_insn(func->builder->current_block);
		if(!insn)
		{
			return 0;
		}
		insn->opcode = JIT_OP_LOAD_PC;
		insn->dest = args[1];

		/* Load the address of "eh_frame_info" into another value */
		args[0] = jit_insn_address_of(func, eh_frame_info);
		if(!args[0])
		{
			return 0;
		}

		/* Create a signature for the prototype "void (void *, void *)" */
		params[0] = jit_type_void_ptr;
		params[1] = jit_type_void_ptr;
		type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, params, 2, 1);
		if(!type)
		{
			return 0;
		}

		/* Call the "_jit_backtrace_push" function */
		jit_insn_call_native(func, "_jit_backtrace_push",
				     (void *) _jit_backtrace_push, type,
				     args, 2, JIT_CALL_NOTHROW);
		jit_type_free(type);
	}
#endif

	/* Update the "catch_pc" value to reflect the current context */
	if(func->builder->setjmp_value != 0)
	{
		args[0] = jit_value_create(func, jit_type_void_ptr);
		if(!args[0])
		{
			return 0;
		}

		insn = _jit_block_add_insn(func->builder->current_block);
		if(!insn)
		{
			return 0;
		}
		insn->opcode = JIT_OP_LOAD_PC;
		insn->dest = args[0];

		jit_value_t addr = jit_insn_address_of(func, func->builder->setjmp_value);
		if(!addr)
		{
			return 0;
		}
		if(!jit_insn_store_relative(func, addr, jit_jmp_catch_pc_offset, args[0]))
		{
			return 0;
		}
	}

	/* We are now ready to make the actual function call */
	return 1;
#else /* JIT_BACKEND_INTERP */
	/* The interpreter handles exception frames for us */
	if((flags & (JIT_CALL_NOTHROW | JIT_CALL_TAIL)) == 0)
	{
		func->builder->may_throw = 1;
	}
	return 1;
#endif
}

/*
 * Restore the exception handling frame after a function call.
 */
static int
restore_eh_frame_after_call(jit_function_t func, int flags)
{
#if !defined(JIT_BACKEND_INTERP)
	/* If the "nothrow", "noreturn", or "tail" flags are set, then we
	   don't need to worry about this */
	if((flags & (JIT_CALL_NOTHROW | JIT_CALL_NORETURN | JIT_CALL_TAIL)) != 0)
	{
		return 1;
	}

#if JIT_APPLY_BROKEN_FRAME_BUILTINS != 0
	/* Create the signature prototype "void (void)" */
	jit_type_t type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, 0, 0, 0);
	if(!type)
	{
		return 0;
	}

	/* Call the "_jit_backtrace_pop" function */
	jit_insn_call_native(func, "_jit_backtrace_pop",
			     (void *) _jit_backtrace_pop, type,
			     0, 0, JIT_CALL_NOTHROW);
	jit_type_free(type);
#endif

	/* Clear the "catch_pc" value for the current context */
	if(func->builder->setjmp_value != 0)
	{
		jit_value_t null = jit_value_create_nint_constant(func, jit_type_void_ptr, 0);
		jit_value_t addr = jit_insn_address_of(func, func->builder->setjmp_value);
		if(!null || !addr)
		{
			return 0;
		}
		if(!jit_insn_store_relative(func, addr, jit_jmp_catch_pc_offset, null))
		{
			return 0;
		}
	}

	/* Everything is back to where it should be */
	return 1;
#else /* JIT_BACKEND_INTERP */
	/* The interpreter handles exception frames for us */
	return 1;
#endif
}

/*
 * Determine if two signatures are identical for the purpose of tail calls.
 */
static int
signature_identical(jit_type_t type1, jit_type_t type2)
{
	/* Handle the easy case first */
	if(type1 == type2)
	{
		return 1;
	}

	/* Remove the tags and then bail out if either type is invalid */
	type1 = jit_type_remove_tags(type1);
	type2 = jit_type_remove_tags(type2);
	if(!type1 || !type2)
	{
		return 0;
	}

	/* Normalize pointer types, but leave signature types as-is */
	if(type1->kind == JIT_TYPE_PTR)
	{
		type1 = jit_type_normalize(type1);
	}
	if(type2->kind == JIT_TYPE_PTR)
	{
		type2 = jit_type_normalize(type2);
	}

#ifdef JIT_NFLOAT_IS_DOUBLE
	/* "double" and "nfloat" are identical on this platform */
	if((type1->kind == JIT_TYPE_FLOAT64 || type1->kind == JIT_TYPE_NFLOAT) &&
	   (type2->kind == JIT_TYPE_FLOAT64 || type2->kind == JIT_TYPE_NFLOAT))
	{
		return 1;
	}
#endif

	/* If the kinds are not the same now, then we don't have a match */
	if(type1->kind != type2->kind)
	{
		return 0;
	}

	/* Structure and union types must have the same size and alignment */
	if(type1->kind == JIT_TYPE_STRUCT || type1->kind == JIT_TYPE_UNION)
	{
		return (jit_type_get_size(type1) == jit_type_get_size(type2) &&
			jit_type_get_alignment(type1) == jit_type_get_alignment(type2));
	}

	/* Signature types must be compared component-by-component */
	if(type1->kind == JIT_TYPE_SIGNATURE)
	{
		if(type1->abi != type2->abi)
		{
			return 0;
		}
		if(!signature_identical(type1->sub_type, type2->sub_type))
		{
			return 0;
		}
		if(type1->num_components != type2->num_components)
		{
			return 0;
		}

		unsigned int param;
		for(param = 0; param < type1->num_components; ++param)
		{
			if(!signature_identical(type1->components[param].type,
						type2->components[param].type))
			{
				return 0;
			}
		}
	}

	/* If we get here, then the types are compatible */
	return 1;
}

/*
 * Create call setup instructions, taking tail calls into effect.
 */
static int
create_call_setup_insns(jit_function_t func, jit_function_t callee,
			jit_type_t signature,
			jit_value_t *args, unsigned int num_args,
			int is_nested, jit_value_t parent_frame,
			jit_value_t *struct_return, int flags)
{
	jit_value_t *new_args;
	unsigned int arg_num;

	/* If we are performing a tail call, then duplicate the argument
	   values so that we don't accidentally destroy parameters in
	   situations like func(x, y) -> func(y, x) */
	if((flags & JIT_CALL_TAIL) != 0 && num_args > 0)
	{
		new_args = (jit_value_t *) alloca(sizeof(jit_value_t) * num_args);
		for(arg_num = 0; arg_num < num_args; ++arg_num)
		{
			jit_value_t value = args[arg_num];
			if(value && value->is_parameter)
			{
				value = jit_insn_dup(func, value);
				if(!value)
				{
					return 0;
				}
			}
			new_args[arg_num] = value;
		}
		args = new_args;
	}

	/* If we are performing a tail call, then store back to our own parameters */
	if((flags & JIT_CALL_TAIL) != 0)
	{
		for(arg_num = 0; arg_num < num_args; ++arg_num)
		{
			if(!jit_insn_store(func, jit_value_get_param(func, arg_num),
					   args[arg_num]))
			{
				return 0;
			}
		}
		*struct_return = 0;
		return 1;
	}

	/* Let the back end do the work */
	return _jit_create_call_setup_insns(func, signature, args, num_args,
					    is_nested, parent_frame, struct_return,
					    flags);
}

static jit_value_t
handle_return(jit_function_t func,
	      jit_type_t signature,
	      int flags, int is_nested,
	      jit_value_t *args, unsigned int num_args,
	      jit_value_t return_value)
{
	/* If the function does not return, then end the current block.
	   The next block does not have "entered_via_top" set so that
	   it will be eliminated during later code generation */
	if((flags & (JIT_CALL_NORETURN | JIT_CALL_TAIL)) != 0)
	{
		func->builder->current_block->ends_in_dead = 1;
	}

	/* If the function may throw an exceptions then end the current
	   basic block to account for exceptional control flow */
	if((flags & JIT_CALL_NOTHROW) == 0)
	{
		if(!jit_insn_new_block(func))
		{
			return 0;
		}
	}

	/* Create space for the return value, if we don't already have one */
	if(!return_value)
	{
		return_value = jit_value_create(func, jit_type_get_return(signature));
		if(!return_value)
		{
			return 0;
		}
	}

	/* Create the instructions necessary to move the return value into place */
	if((flags & JIT_CALL_TAIL) == 0)
	{
		if(!_jit_create_call_return_insns(func, signature, args, num_args,
						  return_value, is_nested))
		{
			return 0;
		}
	}

	/* Restore exception frame information after the call */
	if(!restore_eh_frame_after_call(func, flags))
	{
		return 0;
	}

	/* Return the value containing the result to the caller */
	return return_value;
}

/*@
 * @deftypefun jit_value_t jit_insn_call (jit_function_t @var{func}, const char *@var{name}, jit_function_t @var{jit_func}, jit_type_t @var{signature}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Call the function @var{jit_func}, which may or may not be translated yet.
 * The @var{name} is for diagnostic purposes only, and can be NULL.
 *
 * If @var{signature} is NULL, then the actual signature of @var{jit_func}
 * is used in its place.  This is the usual case.  However, if the function
 * takes a variable number of arguments, then you may need to construct
 * an explicit signature for the non-fixed argument values.
 *
 * The @var{flags} parameter specifies additional information about the
 * type of call to perform:
 *
 * @table @code
 * @vindex JIT_CALL_NOTHROW
 * @item JIT_CALL_NOTHROW
 * The function never throws exceptions.
 *
 * @vindex JIT_CALL_NORETURN
 * @item JIT_CALL_NORETURN
 * The function will never return directly to its caller.  It may however
 * return to the caller indirectly by throwing an exception that the
 * caller catches.
 *
 * @vindex JIT_CALL_TAIL
 * @item JIT_CALL_TAIL
 * Apply tail call optimizations, as the result of this function
 * call will be immediately returned from the containing function.
 * Tail calls are only appropriate when the signature of the called
 * function matches the callee, and none of the parameters point
 * to local variables.
 * @end table
 *
 * If @var{jit_func} has already been compiled, then @code{jit_insn_call}
 * may be able to intuit some of the above flags for itself.  Otherwise
 * it is up to the caller to determine when the flags may be appropriate.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call(jit_function_t func, const char *name, jit_function_t jit_func,
	      jit_type_t signature, jit_value_t *args, unsigned int num_args,
	      int flags)
{
	int is_nested;
	jit_value_t parent_frame;
	jit_value_t return_value;
	jit_label_t entry_point;
	jit_label_t label_end;

	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Get the default signature from "jit_func" */
	if(!signature)
	{
		signature = jit_func->signature;
	}

	/* Verify that tail calls are possible to the destination */
	if((flags & JIT_CALL_TAIL) != 0)
	{
		if(func->nested_parent || jit_func->nested_parent)
		{
			/* Cannot use tail calls with nested function calls */
			flags &= ~JIT_CALL_TAIL;
		}
		else if(!signature_identical(signature, func->signature))
		{
			/* The signatures are not the same, so tail calls not allowed */
			flags &= ~JIT_CALL_TAIL;
		}
	}

	/* Determine the nesting relationship with the current function */
	if(jit_func->nested_parent)
	{
		is_nested = 1;
		parent_frame = jit_insn_get_parent_frame_pointer_of(func, jit_func);

		if(!parent_frame)
		{
			return 0;
		}
	}
	else
	{
		is_nested = 0;
		parent_frame = 0;
	}

	/* Convert the arguments to the actual parameter types */
	jit_value_t *new_args;
	if(num_args > 0)
	{
		new_args = (jit_value_t *) alloca(sizeof(jit_value_t) * num_args);
		if(!convert_call_parameters(func, signature, args, num_args, new_args))
		{
			return 0;
		}
	}
	else
	{
		new_args = args;
	}

	/* Intuit additional flags from "jit_func" if it was already compiled */
	if(jit_func->no_throw)
	{
		flags |= JIT_CALL_NOTHROW;
	}
	if(jit_func->no_return)
	{
		flags |= JIT_CALL_NORETURN;
	}

	/* Set up exception frame information for the call */
	if(!setup_eh_frame_for_call(func, flags))
	{
		return 0;
	}

	/* Create the instructions to push the parameters onto the stack */
	if(!create_call_setup_insns(func, jit_func, signature, new_args, num_args,
				    is_nested, parent_frame, &return_value, flags))
	{
		return 0;
	}

	/* Output the "call" instruction */
	if((flags & JIT_CALL_TAIL) != 0 && func == jit_func)
	{
		/* We are performing a tail call to ourselves, which we can
		   turn into an unconditional branch back to our entry point */
		entry_point = jit_label_undefined;
		label_end = jit_label_undefined;
		if(!jit_insn_branch(func, &entry_point))
		{
			return 0;
		}
		if(!jit_insn_label_tight(func, &entry_point))
		{
			return 0;
		}
		if(!jit_insn_label(func, &label_end))
		{
			return 0;
		}
		if(!jit_insn_move_blocks_to_start(func, entry_point, label_end))
		{
			return 0;
		}
	}
	else
	{
		/* Functions that call out are not leaves */
		func->builder->non_leaf = 1;

		/* Performing a regular call, or a tail call to someone else */
		jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
		if(!insn)
		{
			return 0;
		}
		if((flags & JIT_CALL_TAIL) != 0)
		{
			func->builder->has_tail_call = 1;
			insn->opcode = JIT_OP_CALL_TAIL;
		}
		else
		{
			insn->opcode = JIT_OP_CALL;
		}
		insn->flags = JIT_INSN_DEST_IS_FUNCTION | JIT_INSN_VALUE1_IS_NAME;
		insn->dest = (jit_value_t) jit_func;
		insn->value1 = (jit_value_t) name;
	}

	/* Handle return to the caller */
	return handle_return(func, signature, flags, is_nested, new_args, num_args, return_value);
}

/*@
 * @deftypefun jit_value_t jit_insn_call_indirect (jit_function_t @var{func}, jit_value_t @var{value}, jit_type_t @var{signature}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Call a function via an indirect pointer.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call_nested_indirect(jit_function_t func, jit_value_t value,
	jit_value_t parent_frame, jit_type_t signature, jit_value_t *args,
	unsigned int num_args, int flags)
{
	int is_nested = parent_frame ? 1 : 0;

	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Verify that tail calls are possible to the destination */
#if defined(JIT_BACKEND_INTERP)
	flags &= ~JIT_CALL_TAIL;
#else
	if((flags & JIT_CALL_TAIL) != 0)
	{
		if(is_nested || func->nested_parent)
		{
			flags &= ~JIT_CALL_TAIL;
		}
		else if(!signature_identical(signature, func->signature))
		{
			flags &= ~JIT_CALL_TAIL;
		}
	}
#endif

	/* We are making a native call */
	flags |= JIT_CALL_NATIVE;

	/* Convert the arguments to the actual parameter types */
	jit_value_t *new_args;
	if(num_args > 0)
	{
		new_args = (jit_value_t *) alloca(sizeof(jit_value_t) * num_args);
		if(!convert_call_parameters(func, signature, args, num_args, new_args))
		{
			return 0;
		}
	}
	else
	{
		new_args = args;
	}

	/* Set up exception frame information for the call */
	if(!setup_eh_frame_for_call(func, flags))
	{
		return 0;
	}

	/* Create the instructions to push the parameters onto the stack */
	jit_value_t return_value;
	if(!create_call_setup_insns(func, 0, signature, new_args, num_args,
		is_nested, parent_frame, &return_value, flags))
	{
		return 0;
	}

	/* Move the indirect pointer value into an appropriate register */
	if(!_jit_setup_indirect_pointer(func, value))
	{
		return 0;
	}

	/* Functions that call out are not leaves */
	func->builder->non_leaf = 1;

	/* Output the "call_indirect" instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	if((flags & JIT_CALL_TAIL) != 0)
	{
		func->builder->has_tail_call = 1;
		insn->opcode = JIT_OP_CALL_INDIRECT_TAIL;
	}
	else
	{
		insn->opcode = JIT_OP_CALL_INDIRECT;
	}
	insn->flags = JIT_INSN_VALUE2_IS_SIGNATURE;
	insn->value1 = value;
	jit_value_ref(func, value);
	insn->value2 = (jit_value_t) jit_type_copy(signature);

	/* Handle return to the caller */
	return handle_return(func, signature, flags, is_nested, new_args, num_args,
		return_value);
}

/*@
 * @deftypefun jit_value_t jit_insn_call_nested_indirect (jit_function_t @var{func}, jit_value_t @var{value}, jit_value_t @var{parent_frame}, jit_type_t @var{signature}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Call a jit function that is nested via an indirect pointer.
 * @var{parent_frame} should be a pointer to the frame of the parent of *value.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call_indirect(jit_function_t func, jit_value_t value,
	jit_type_t signature, jit_value_t *args,
	unsigned int num_args, int flags)
{
	return jit_insn_call_nested_indirect(func, value, 0, signature,
		args, num_args, flags);
}

/*@
 * @deftypefun jit_value_t jit_insn_call_indirect_vtable (jit_function_t @var{func}, jit_value_t @var{value}, jit_type_t @var{signature}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Call a function via an indirect pointer.  This version differs from
 * @code{jit_insn_call_indirect} in that we assume that @var{value}
 * contains a pointer that resulted from calling
 * @code{jit_function_to_vtable_pointer}.  Indirect vtable pointer
 * calls may be more efficient on some platforms than regular indirect calls.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call_indirect_vtable(jit_function_t func, jit_value_t value, jit_type_t signature,
			      jit_value_t *args, unsigned int num_args, int flags)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Verify that tail calls are possible to the destination */
	if((flags & JIT_CALL_TAIL) != 0)
	{
		if(func->nested_parent)
		{
			flags &= ~JIT_CALL_TAIL;
		}
		else if(!signature_identical(signature, func->signature))
		{
			flags &= ~JIT_CALL_TAIL;
		}
	}

	/* Convert the arguments to the actual parameter types */
	jit_value_t *new_args;
	if(num_args > 0)
	{
		new_args = (jit_value_t *) alloca(sizeof(jit_value_t) * num_args);
		if(!convert_call_parameters(func, signature, args, num_args, new_args))
		{
			return 0;
		}
	}
	else
	{
		new_args = args;
	}

	/* Set up exception frame information for the call */
	if(!setup_eh_frame_for_call(func, flags))
	{
		return 0;
	}

	/* Create the instructions to push the parameters onto the stack */
	jit_value_t return_value;
	if(!create_call_setup_insns(func, 0, signature, new_args, num_args,
				    0, 0, &return_value, flags))
	{
		return 0;
	}

	/* Move the indirect pointer value into an appropriate register */
	if(!_jit_setup_indirect_pointer(func, value))
	{
		return 0;
	}

	/* Functions that call out are not leaves */
	func->builder->non_leaf = 1;

	/* Output the "call_vtable_ptr" instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	if((flags & JIT_CALL_TAIL) != 0)
	{
		func->builder->has_tail_call = 1;
		insn->opcode = JIT_OP_CALL_VTABLE_PTR_TAIL;
	}
	else
	{
		insn->opcode = JIT_OP_CALL_VTABLE_PTR;
	}
	insn->value1 = value;
	jit_value_ref(func, value);

	/* Handle return to the caller */
	return handle_return(func, signature, flags, 0, new_args, num_args, return_value);
}

/*@
 * @deftypefun jit_value_t jit_insn_call_native (jit_function_t @var{func}, const char *@var{name}, void *@var{native_func}, jit_type_t @var{signature}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Output an instruction that calls an external native function.
 * The @var{name} is for diagnostic purposes only, and can be NULL.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call_native(jit_function_t func, const char *name, void *native_func,
		     jit_type_t signature, jit_value_t *args, unsigned int num_args, int flags)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Verify that tail calls are possible to the destination */
#if defined(JIT_BACKEND_INTERP)
	flags &= ~JIT_CALL_TAIL;
#else
	if((flags & JIT_CALL_TAIL) != 0)
	{
		if(func->nested_parent)
		{
			flags &= ~JIT_CALL_TAIL;
		}
		else if(!signature_identical(signature, func->signature))
		{
			flags &= ~JIT_CALL_TAIL;
		}
	}
#endif

	/* We are making a native call */
	flags |= JIT_CALL_NATIVE;

	/* Convert the arguments to the actual parameter types */
	jit_value_t *new_args;
	if(num_args > 0)
	{
		new_args = (jit_value_t *) alloca(sizeof(jit_value_t) * num_args);
		if(!convert_call_parameters(func, signature, args, num_args, new_args))
		{
			return 0;
		}
	}
	else
	{
		new_args = args;
	}

	/* Set up exception frame information for the call */
	if(!setup_eh_frame_for_call(func, flags))
	{
		return 0;
	}

	/* Create the instructions to push the parameters onto the stack */
	jit_value_t return_value;
	if(!create_call_setup_insns(func, 0, signature, new_args, num_args,
				    0, 0, &return_value, flags))
	{
		return 0;
	}

	/* Functions that call out are not leaves */
	func->builder->non_leaf = 1;

	/* Output the "call_external" instruction */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	if((flags & JIT_CALL_TAIL) != 0)
	{
		func->builder->has_tail_call = 1;
		insn->opcode = JIT_OP_CALL_EXTERNAL_TAIL;
	}
	else
	{
		insn->opcode = JIT_OP_CALL_EXTERNAL;
	}
	insn->flags = JIT_INSN_DEST_IS_NATIVE | JIT_INSN_VALUE1_IS_NAME;
	insn->dest = (jit_value_t) native_func;
	insn->value1 = (jit_value_t) name;
#ifdef JIT_BACKEND_INTERP
	insn->flags |= JIT_INSN_VALUE2_IS_SIGNATURE;
	insn->value2 = (jit_value_t) jit_type_copy(signature);
#endif

	/* Handle return to the caller */
	return_value = handle_return(func, signature, flags, 0, new_args, num_args, return_value);

	/* Make sure that returned byte / short values get zero / sign extended */
	jit_type_t return_type = jit_type_remove_tags(return_value->type);
	switch(return_type->kind)
	{
	case JIT_TYPE_SBYTE:
		/* Force sbyte sign extension to int */
		return_value = apply_conversion(func, JIT_OP_TRUNC_SBYTE, return_value,
						return_type);
		break;
	case JIT_TYPE_UBYTE:
		/* Force ubyte zero extension to uint */
		return_value = apply_conversion(func, JIT_OP_TRUNC_UBYTE, return_value,
						return_type);
		break;
	case JIT_TYPE_SHORT:
		/* Force short sign extension to int */
		return_value = apply_conversion(func, JIT_OP_TRUNC_SHORT, return_value,
						return_type);
		break;
	case JIT_TYPE_USHORT:
		/* Force ushort zero extension to uint */
		return_value = apply_conversion(func, JIT_OP_TRUNC_USHORT, return_value,
						return_type);
		break;
	}

	/* Return the value containing the result to the caller */
	return return_value;
}

/*@
 * @deftypefun jit_value_t jit_insn_call_intrinsic (jit_function_t @var{func}, const char *@var{name}, void *@var{intrinsic_func}, const jit_intrinsic_descr_t *@var{descriptor}, jit_value_t @var{arg1}, jit_value_t @var{arg2})
 * Output an instruction that calls an intrinsic function.  The descriptor
 * contains the following fields:
 *
 * @table @code
 * @item return_type
 * The type of value that is returned from the intrinsic.
 *
 * @item ptr_result_type
 * This should be NULL for an ordinary intrinsic, or the result type
 * if the intrinsic reports exceptions.
 *
 * @item arg1_type
 * The type of the first argument.
 *
 * @item arg2_type
 * The type of the second argument, or NULL for a unary intrinsic.
 * @end table
 *
 * If all of the arguments are constant, then @code{jit_insn_call_intrinsic}
 * will call the intrinsic directly to calculate the constant result.
 * If the constant computation will result in an exception, then
 * code is output to cause the exception at runtime.
 *
 * The @var{name} is for diagnostic purposes only, and can be NULL.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call_intrinsic(jit_function_t func, const char *name, void *intrinsic_func,
			const jit_intrinsic_descr_t *descriptor, jit_value_t arg1,
			jit_value_t arg2)
{
	jit_type_t signature;
	jit_type_t param_types[3];
	jit_value_t param_values[3];
	jit_value_t return_value;
	jit_value_t temp_value;
	jit_value_t cond_value;
	jit_label_t label;
	jit_constant_t const1;
	jit_constant_t const2;
	jit_constant_t return_const;
	jit_constant_t temp_const;
	void *apply_args[3];

	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Coerce the arguments to the desired types */
	arg1 = jit_insn_convert(func, arg1, descriptor->arg1_type, 0);
	if(!arg1)
	{
		return 0;
	}
	if(arg2)
	{
		arg2 = jit_insn_convert(func, arg2, descriptor->arg2_type, 0);
		if(!arg2)
		{
			return 0;
		}
	}

	/* Allocate space for a return value if the intrinsic reports exceptions */
	if(descriptor->ptr_result_type)
	{
		return_value = jit_value_create(func, descriptor->ptr_result_type);
		if(!return_value)
		{
			return 0;
		}
	}
	else
	{
		return_value = 0;
	}

	/* Construct the signature for the intrinsic */
	unsigned int num_params = 0;
	if(return_value)
	{
		/* Pass a pointer to "return_value" as the first argument */
		temp_value = jit_insn_address_of(func, return_value);
		if(!temp_value)
		{
			return 0;
		}
		param_types[num_params] = jit_value_get_type(temp_value);
		param_values[num_params] = temp_value;
		++num_params;
	}
	param_types[num_params] = jit_value_get_type(arg1);
	param_values[num_params] = arg1;
	++num_params;
	if(arg2)
	{
		param_types[num_params] = jit_value_get_type(arg2);
		param_values[num_params] = arg2;
		++num_params;
	}
	signature = jit_type_create_signature(jit_abi_cdecl, descriptor->return_type,
					      param_types, num_params, 1);
	if(!signature)
	{
		return 0;
	}

	/* If the arguments are constant, then invoke the intrinsic now */
	if(jit_value_is_constant(arg1)
	   && (!arg2 || jit_value_is_constant(arg2))
	   && !jit_context_get_meta_numeric(func->context, JIT_OPTION_DONT_FOLD))
	{
		const1 = jit_value_get_constant(arg1);
		const2 = jit_value_get_constant(arg2);
		if(return_value)
		{
			jit_int result;
			return_const.type = descriptor->ptr_result_type;
			temp_const.un.ptr_value = &return_const.un;
			apply_args[0] = &temp_const.un;
			apply_args[1] = &const1.un;
			apply_args[2] = &const2.un;
			jit_apply(signature, intrinsic_func, apply_args, num_params,
				  &result);
			if(result >= 1)
			{
				/* No exception occurred, so return the constant value */
				jit_type_free(signature);
				return jit_value_create_constant(func, &return_const);
			}
		}
		else
		{
			return_const.type = descriptor->return_type;
			apply_args[0] = &const1.un;
			apply_args[1] = &const2.un;
			jit_apply(signature, intrinsic_func, apply_args, num_params,
				  &return_const.un);
			jit_type_free(signature);
			return jit_value_create_constant(func, &return_const);
		}
	}

	/* Call the intrinsic as a native function */
	temp_value = jit_insn_call_native(func, name, intrinsic_func, signature,
					  param_values, num_params, JIT_CALL_NOTHROW);
	if(!temp_value)
	{
		jit_type_free(signature);
		return 0;
	}
	jit_type_free(signature);

	/* If no exceptions to report, then return "temp_value" as the result */
	if(!return_value)
	{
		return temp_value;
	}

	/* Determine if an exception was reported */
	cond_value = jit_value_create_nint_constant(func, jit_type_int, 1);
	cond_value = jit_insn_ge(func, temp_value, cond_value);
	if(!cond_value)
	{
		return 0;
	}
	label = jit_label_undefined;
	if(!jit_insn_branch_if(func, cond_value, &label))
	{
		return 0;
	}

	/* Call the "jit_exception_builtin" function to report the exception */
	param_types[0] = jit_type_int;
	signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void,
					      param_types, 1, 1);
	if(!signature)
	{
		return 0;
	}
	param_values[0] = temp_value;
	jit_insn_call_native(func, "jit_exception_builtin",
			     (void *) jit_exception_builtin, signature,
			     param_values, 1, JIT_CALL_NORETURN);
	jit_type_free(signature);

	/* Execution continues here if there was no exception */
	if(!jit_insn_label_tight(func, &label))
	{
		return 0;
	}

	/* Return the temporary that contains the result value */
	return return_value;
}

/*@
 * @deftypefun int jit_insn_incoming_reg (jit_function_t @var{func}, jit_value_t @var{value}, int @var{reg})
 * Output an instruction that notes that the contents of @var{value}
 * can be found in the register @var{reg} at this point in the code.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the function's entry frame and the
 * values of registers on return from a subroutine call.
 * @end deftypefun
@*/
int
jit_insn_incoming_reg(jit_function_t func, jit_value_t value, int reg)
{
	jit_value_t reg_value = jit_value_create_nint_constant(func, jit_type_int, reg);
	if(!reg_value)
	{
		return 0;
	}
	if(value->is_parameter)
	{
		value->is_reg_parameter = 1;
	}
	return create_note(func, JIT_OP_INCOMING_REG, value, reg_value);
}

/*@
 * @deftypefun int jit_insn_incoming_frame_posn (jit_function_t @var{func}, jit_value_t @var{value}, jit_nint @var{frame_offset})
 * Output an instruction that notes that the contents of @var{value}
 * can be found in the stack frame at @var{frame_offset}. This should only be
 * called once per value, to prevent values from changing their address when
 * they might be addressable.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the function's entry frame.
 * @end deftypefun
@*/
int
jit_insn_incoming_frame_posn(jit_function_t func, jit_value_t value,
			     jit_nint frame_offset)
{
	jit_value_t frame_offset_value;

	/* We need to set the value's frame_offset right now. As children have to be
	   compiled before their parents there would otherwise be no way for a child
	   to know the frame_offset the value will be in. */
	if(!value->has_frame_offset)
	{
		value->has_frame_offset = 1;
		value->frame_offset = frame_offset;
	}

	frame_offset_value
		= jit_value_create_nint_constant(func, jit_type_int, frame_offset);
	if(!frame_offset_value)
	{
		return 0;
	}
	return create_note(func, JIT_OP_INCOMING_FRAME_POSN, value, frame_offset_value);
}

/*@
 * @deftypefun int jit_insn_outgoing_reg (jit_function_t @var{func}, jit_value_t @var{value}, int @var{reg})
 * Output an instruction that copies the contents of @var{value}
 * into the register @var{reg} at this point in the code.  This is
 * typically used just before making an outgoing subroutine call.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the registers for a subroutine call.
 * @end deftypefun
@*/
int
jit_insn_outgoing_reg(jit_function_t func, jit_value_t value, int reg)
{
	jit_value_t reg_value = jit_value_create_nint_constant(func, jit_type_int, reg);
	if(!reg_value)
	{
		return 0;
	}
	return create_note(func, JIT_OP_OUTGOING_REG, value, reg_value);
}

/*@
 * @deftypefun int jit_insn_outgoing_frame_posn (jit_function_t @var{func}, jit_value_t @var{value}, jit_nint @var{frame_offset})
 * Output an instruction that stores the contents of @var{value} in
 * the stack frame at @var{frame_offset}.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up an outgoing frame for tail calls.
 * @end deftypefun
@*/
int
jit_insn_outgoing_frame_posn(jit_function_t func, jit_value_t value,
			     jit_nint frame_offset)
{
	jit_value_t frame_pointer;

	frame_pointer = jit_insn_get_frame_pointer(func);
	if(!frame_pointer)
	{
		return 0;
	}

	return jit_insn_store_relative(func, frame_pointer, frame_offset, value);
}

/*@
 * @deftypefun int jit_insn_return_reg (jit_function_t @var{func}, jit_value_t @var{value}, int @var{reg})
 * Output an instruction that notes that the contents of @var{value}
 * can be found in the register @var{reg} at this point in the code.
 * This is similar to @code{jit_insn_incoming_reg}, except that it
 * refers to return values, not parameter values.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to handle returns from subroutine calls.
 * @end deftypefun
@*/
int
jit_insn_return_reg(jit_function_t func, jit_value_t value, int reg)
{
	jit_value_t reg_value = jit_value_create_nint_constant(func, jit_type_int, reg);
	if(!reg_value)
	{
		return 0;
	}
	return create_note(func, JIT_OP_RETURN_REG, value, reg_value);
}

/*@
 * @deftypefun int jit_insn_flush_struct (jit_function_t @var{func}, jit_value_t @var{value})
 * Flush a small structure return value out of registers and back
 * into the local variable frame.  You normally wouldn't call this
 * yourself - it is used internally by the CPU back ends to handle
 * structure returns from functions.
 * @end deftypefun
@*/
int
jit_insn_flush_struct(jit_function_t func, jit_value_t value)
{
	if(value)
	{
		jit_value_set_addressable(value);
	}
	return create_unary_note(func, JIT_OP_FLUSH_SMALL_STRUCT, value);
}

/*@
 * @deftypefun jit_value_t jit_insn_get_frame_pointer (jit_function_t @var{func})
 * Retrieve the frame pointer of function @var{func}
 * Returns NULL if out of memory.
 * @end deftypefun
@*/
jit_value_t
jit_insn_get_frame_pointer(jit_function_t func)
{
	return create_dest_note(func, JIT_OP_RETRIEVE_FRAME_POINTER,
		jit_type_void_ptr);
}

static jit_value_t
find_frame_of(jit_function_t func, jit_function_t target,
	jit_function_t func_start, jit_value_t frame_start)
{
	/* Find the nesting level */
	int nesting_level = 0;
	jit_function_t current_func = func_start;
	while(current_func != 0 && current_func != target)
	{
		/* Ensure that current_func has a function builder */
		if(!_jit_function_ensure_builder(current_func))
		{
			return 0;
		}

		if(!current_func->parent_frame)
		{
			/* One of the ancestors is not correctly set up */
			return 0;
		}

#ifdef JIT_BACKEND_INTERP
		if(!current_func->arguments_pointer)
		{
			/* Make sure the ancestor has an arguments_pointer, in case we are
			   importing a parameter */
			current_func->arguments_pointer = jit_value_create(current_func,
				jit_type_void_ptr);

			if(!current_func->arguments_pointer)
			{
				return 0;
			}
		}
#endif

		current_func = current_func->nested_parent;
		nesting_level++;
	}
	if(!current_func)
	{
		/* The value is not accessible from this scope */
		return 0;
	}

	/* When we are importing a multi level nested value we need to import the
	   frame pointer of the next nesting level using the frame pointer of the
	   current level, until we reach our target function */
	current_func = func_start;
	while(frame_start != 0 && nesting_level-- > 0)
	{
		frame_start = apply_binary(func, JIT_OP_IMPORT, frame_start,
			current_func->parent_frame, jit_type_void_ptr);
		frame_start = jit_insn_load_relative(func, frame_start, 0,
			jit_type_void_ptr);

		current_func = current_func->nested_parent;
	}

	if(!frame_start)
	{
		return 0;
	}

	return frame_start;
}

/*@
 * @deftypefun jit_value_t jit_insn_get_parent_frame_pointer_of (jit_function_t @var{func}, jit_function_t @var{target})
 * Retrieve the frame pointer of the parent of @var{target}. Returns NULL when
 * @var{target} is not a sibling, an ancestor, or a sibling of one of the
 * ancestors of @var{func}.
 * Returns NULL if out of memory.
 * @end deftypefun
@*/
jit_value_t
jit_insn_get_parent_frame_pointer_of(jit_function_t func, jit_function_t target)
{
	if(func == target->nested_parent)
	{
		/* target is a child of the current function. We just need return
		   our frame pointer */
		return jit_insn_get_frame_pointer(func);
	}
	else
	{
		/* target is a sibling or a sibling of one of the ancestors of func.
		   We need to find the parent of target in the ancestor tree of func */
		return find_frame_of(func, target->nested_parent,
			func->nested_parent, func->parent_frame);
	}
}

/*@
 * @deftypefun jit_value_t jit_insn_import (jit_function_t @var{func}, jit_value_t @var{value})
 * Import @var{value} from an outer nested scope into @var{func}.  Returns
 * the effective address of the value for local access via a pointer.
 * Returns NULL if out of memory or the value is not accessible via a
 * parent, grandparent, or other ancestor of @var{func}.
 * @end deftypefun
@*/
jit_value_t
jit_insn_import(jit_function_t func, jit_value_t value)
{
	jit_function_t value_func;
	jit_value_t value_frame;
	jit_type_t result_type;
	jit_value_t result;

	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* If the value is already local, then return the local address */
	value_func = jit_value_get_function(value);
	if(value_func == func)
	{
		return jit_insn_address_of(func, value);
	}

#ifdef JIT_BACKEND_INTERP
	if(!value_func->arguments_pointer)
	{
		/* Make sure the ancestor has an arguments_pointer, in case we are
		   importing a parameter */
		value_func->arguments_pointer = jit_value_create(value_func,
			jit_type_void_ptr);

		if(!value_func->arguments_pointer)
		{
			return 0;
		}
	}
#endif

	result_type = jit_type_create_pointer(jit_value_get_type(value), 1);
	if(!result_type)
	{
		return 0;
	}

	/* Often there are multiple values imported from the same ancestor in a row,
	   thats why the last ancestor a value was imported from is cached so its
	   frame can be reused as finding it would require multiple memory loads */
	if(value_func == func->cached_parent && func->cached_parent_frame)
	{
		value_frame = func->cached_parent_frame;
	}
	else
	{
		value_frame = find_frame_of(func, value_func,
			func->nested_parent, func->parent_frame);

		func->cached_parent = value_func;
		func->cached_parent_frame = value_frame;
	}

	if(!value_frame)
	{
		jit_type_free(result_type);
		return 0;
	}

	result = apply_binary(func, JIT_OP_IMPORT, value_frame, value, result_type);
	jit_type_free(result_type);

	return result;
}

/*@
 * @deftypefun int jit_insn_push (jit_function_t @var{func}, jit_value_t @var{value})
 * Push a value onto the function call stack, in preparation for a call.
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the stack for a subroutine call.
 * @end deftypefun
@*/
int
jit_insn_push(jit_function_t func, jit_value_t value)
{
	jit_type_t type = jit_value_get_type(value);
	type = jit_type_promote_int(jit_type_normalize(type));

	jit_value_t size_value;
	switch(type->kind)
	{
	case JIT_TYPE_SBYTE:
	case JIT_TYPE_UBYTE:
	case JIT_TYPE_SHORT:
	case JIT_TYPE_USHORT:
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		return create_unary_note(func, JIT_OP_PUSH_INT, value);

	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		return create_unary_note(func, JIT_OP_PUSH_LONG, value);

	case JIT_TYPE_FLOAT32:
		return create_unary_note(func, JIT_OP_PUSH_FLOAT32, value);

	case JIT_TYPE_FLOAT64:
		return create_unary_note(func, JIT_OP_PUSH_FLOAT64, value);

	case JIT_TYPE_NFLOAT:
		return create_unary_note(func, JIT_OP_PUSH_NFLOAT, value);

	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		/* We need the address of the value for "push_struct" */
		value = jit_insn_address_of(func, value);
		size_value = jit_value_create_nint_constant(func, jit_type_nint,
							    jit_type_get_size(type));
		if(!value || !size_value)
		{
			return 0;
		}
		return create_note(func, JIT_OP_PUSH_STRUCT, value, size_value);
	}

	return 1;
}

/*@
 * @deftypefun int jit_insn_push_ptr (jit_function_t @var{func}, jit_value_t @var{value}, jit_type_t @var{type})
 * Push @code{*@var{value}} onto the function call stack, in preparation for a call.
 * This is normally used for returning @code{struct} and @code{union}
 * values where you have the effective address of the structure, rather
 * than the structure's contents, in @var{value}.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the stack for a subroutine call.
 * @end deftypefun
@*/
int
jit_insn_push_ptr(jit_function_t func, jit_value_t value, jit_type_t type)
{
	jit_value_t size_value;
	switch(jit_type_remove_tags(type)->kind)
	{
	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		/* Push the structure onto the stack by address */
		size_value = jit_value_create_nint_constant(func, jit_type_nint,
							    jit_type_get_size(type));
		if(!size_value)
		{
			return 0;
		}
		return create_note(func, JIT_OP_PUSH_STRUCT, value, size_value);

	default:
		/* Load the value from the address and push it normally */
		value = jit_insn_load_relative(func, value, 0, type);
		if(!value)
		{
			return 0;
		}
		return jit_insn_push(func, value);
	}
}

/*@
 * @deftypefun int jit_insn_set_param (jit_function_t @var{func}, jit_value_t @var{value}, jit_nint @var{offset})
 * Set the parameter slot at @var{offset} in the outgoing parameter area
 * to @var{value}.  This may be used instead of @code{jit_insn_push}
 * if it is more efficient to store directly to the stack than to push.
 * The outgoing parameter area is allocated within the frame when
 * the function is first entered.
 *
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the stack for a subroutine call.
 * @end deftypefun
@*/
int
jit_insn_set_param(jit_function_t func, jit_value_t value, jit_nint offset)
{
	jit_type_t type = jit_value_get_type(value);
	type = jit_type_promote_int(jit_type_normalize(type));

	jit_value_t offset_value, size_value;
	offset_value = jit_value_create_nint_constant(func, jit_type_nint, offset);
	if(!offset_value)
	{
		return 0;
	}

	switch(type->kind)
	{
	case JIT_TYPE_SBYTE:
	case JIT_TYPE_UBYTE:
	case JIT_TYPE_SHORT:
	case JIT_TYPE_USHORT:
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		return create_note(func, JIT_OP_SET_PARAM_INT, value, offset_value);

	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		return create_note(func, JIT_OP_SET_PARAM_LONG, value, offset_value);

	case JIT_TYPE_FLOAT32:
		return create_note(func, JIT_OP_SET_PARAM_FLOAT32, value, offset_value);

	case JIT_TYPE_FLOAT64:
		return create_note(func, JIT_OP_SET_PARAM_FLOAT64, value, offset_value);

	case JIT_TYPE_NFLOAT:
		return create_note(func, JIT_OP_SET_PARAM_NFLOAT, value, offset_value);

	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		/* We need the address of the value for "push_struct" */
		value = jit_insn_address_of(func, value);
		size_value = jit_value_create_nint_constant(func, jit_type_nint,
							    jit_type_get_size(type));
		if(!value || !size_value)
		{
			return 0;
		}
		return apply_ternary(func, JIT_OP_SET_PARAM_STRUCT, offset_value, value,
				     size_value);
	}
	return 1;
}

/*@
 * @deftypefun int jit_insn_set_param_ptr (jit_function_t @var{func}, jit_value_t @var{value}, jit_type_t @var{type}, jit_nint @var{offset})
 * Same as @code{jit_insn_set_param}, except that the parameter is
 * at @code{*@var{value}}.
 * @end deftypefun
@*/
int
jit_insn_set_param_ptr(jit_function_t func, jit_value_t value, jit_type_t type,
		       jit_nint offset)
{
	jit_value_t offset_value, size_value;
	switch(jit_type_remove_tags(type)->kind)
	{
	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		/* Set the structure into the parameter area by address */
		offset_value = jit_value_create_nint_constant(func, jit_type_nint, offset);
		size_value = jit_value_create_nint_constant(func, jit_type_nint,
							    jit_type_get_size(type));
		if(!offset_value || !size_value)
		{
			return 0;
		}
		return apply_ternary(func, JIT_OP_SET_PARAM_STRUCT, offset_value, value,
				     size_value);

	default:
		/* Load the value from the address and set it normally */
		value = jit_insn_load_relative(func, value, 0, type);
		if(!value)
		{
			return 0;
		}
		return jit_insn_set_param(func, value, offset);
	}
}

/*@
 * @deftypefun int jit_insn_push_return_area_ptr (jit_function_t @var{func})
 * Push the interpreter's return area pointer onto the stack.
 * You normally wouldn't call this yourself - it is used internally
 * by the CPU back ends to set up the stack for a subroutine call.
 * @end deftypefun
@*/
int
jit_insn_push_return_area_ptr(jit_function_t func)
{
	return create_noarg_note(func, JIT_OP_PUSH_RETURN_AREA_PTR);
}

/*@
 * @deftypefun int jit_insn_pop_stack (jit_function_t @var{func}, jit_nint @var{num_items})
 * Pop @var{num_items} items from the function call stack.  You normally
 * wouldn't call this yourself - it is used by CPU back ends to clean up
 * the stack after calling a subroutine.  The size of an item is specific
 * to the back end (it could be bytes, words, or some other measurement).
 * @end deftypefun
@*/
int
jit_insn_pop_stack(jit_function_t func, jit_nint num_items)
{
	jit_value_t num_value = jit_value_create_nint_constant(func, jit_type_nint, num_items);
	return create_unary_note(func, JIT_OP_POP_STACK, num_value);
}

/*@
 * @deftypefun int jit_insn_defer_pop_stack (jit_function_t @var{func}, jit_nint @var{num_items})
 * This is similar to @code{jit_insn_pop_stack}, except that it tries to
 * defer the pop as long as possible.  Multiple subroutine calls may
 * result in parameters collecting up on the stack, and only being popped
 * at the next branch or label instruction.  You normally wouldn't
 * call this yourself - it is used by CPU back ends.
 * @end deftypefun
@*/
int
jit_insn_defer_pop_stack(jit_function_t func, jit_nint num_items)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	func->builder->deferred_items += num_items;
	return 1;
}

/*@
 * @deftypefun int jit_insn_flush_defer_pop (jit_function_t @var{func}, jit_nint @var{num_items})
 * Flush any deferred items that were scheduled for popping by
 * @code{jit_insn_defer_pop_stack} if there are @var{num_items}
 * or more items scheduled.  You normally wouldn't call this
 * yourself - it is used by CPU back ends to clean up the stack just
 * prior to a subroutine call when too many items have collected up.
 * Calling @code{jit_insn_flush_defer_pop(func, 0)} will flush
 * all deferred items.
 * @end deftypefun
@*/
int
jit_insn_flush_defer_pop(jit_function_t func, jit_nint num_items)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_nint current_items = func->builder->deferred_items;
	if(current_items >= num_items && current_items > 0)
	{
		func->builder->deferred_items = 0;
		return jit_insn_pop_stack(func, current_items);
	}

	return 1;
}

/*@
 * @deftypefun int jit_insn_return (jit_function_t @var{func}, jit_value_t @var{value})
 * Output an instruction to return @var{value} as the function's result.
 * If @var{value} is NULL, then the function is assumed to return
 * @code{void}.  If the function returns a structure, this will copy
 * the value into the memory at the structure return address.
 * @end deftypefun
@*/
int
jit_insn_return(jit_function_t func, jit_value_t value)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

#if !defined(JIT_BACKEND_INTERP)
	/* We need to pop the "setjmp" context */
	if(func->has_try)
	{
		jit_type_t type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, 0, 0, 1);
		if(!type)
		{
			return 0;
		}
		jit_insn_call_native(func, "_jit_unwind_pop_setjmp",
				     (void *) _jit_unwind_pop_setjmp, type,
				     0, 0, JIT_CALL_NOTHROW);
		jit_type_free(type);
	}
#endif

	/* This function has an ordinary return path */
	func->builder->ordinary_return = 1;

	/* Output an appropriate instruction to return to the caller */
	jit_type_t type = jit_type_get_return(func->signature);
	type = jit_type_promote_int(jit_type_normalize(type));
	if(!value || type == jit_type_void)
	{
		/* This function returns "void" */
		if(!create_noarg_note(func, JIT_OP_RETURN))
		{
			return 0;
		}
	}
	else
	{
		/* Convert the value into the desired return type */
		value = jit_insn_convert(func, value, type, 0);
		if(!value)
		{
			return 0;
		}

		/* Create the "return" instruction */
		jit_value_t return_ptr, value_addr, size_value;
		switch(type->kind)
		{
		case JIT_TYPE_SBYTE:
		case JIT_TYPE_UBYTE:
		case JIT_TYPE_SHORT:
		case JIT_TYPE_USHORT:
		case JIT_TYPE_INT:
		case JIT_TYPE_UINT:
			if(!create_unary_note(func, JIT_OP_RETURN_INT, value))
			{
				return 0;
			}
			break;

		case JIT_TYPE_LONG:
		case JIT_TYPE_ULONG:
			if(!create_unary_note(func, JIT_OP_RETURN_LONG, value))
			{
				return 0;
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(!create_unary_note(func, JIT_OP_RETURN_FLOAT32, value))
			{
				return 0;
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(!create_unary_note(func, JIT_OP_RETURN_FLOAT64, value))
			{
				return 0;
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(!create_unary_note(func, JIT_OP_RETURN_NFLOAT, value))
			{
				return 0;
			}
			break;

		case JIT_TYPE_STRUCT:
		case JIT_TYPE_UNION:
			value_addr = jit_insn_address_of(func, value);
			size_value = jit_value_create_nint_constant(func, jit_type_nint,
								    jit_type_get_size(type));
			if(!value_addr || !size_value)
			{
				return 0;
			}

			/* Determine the kind of structure return to use */
			return_ptr = jit_value_get_struct_pointer(func);
			if(return_ptr)
			{
				/* Copy the structure's contents to the supplied
				   pointer */
				if(!jit_insn_memcpy(func, return_ptr, value_addr, size_value))
				{
					return 0;
				}
				/* Output a regular return for the function */
				if(!create_noarg_note(func, JIT_OP_RETURN))
				{
					return 0;
				}
			}
			else
			{
				/* Return the structure via registers */
				if(!create_note(func, JIT_OP_RETURN_SMALL_STRUCT, value_addr,
						size_value))
				{
					return 0;
				}
			}
			break;
		}
	}

	/* Mark the current block as "ends in dead" */
	func->builder->current_block->ends_in_dead = 1;

	/* Start a new block just after the "return" instruction */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_return_ptr (jit_function_t @var{func}, jit_value_t @var{value}, jit_type_t @var{type})
 * Output an instruction to return @code{*@var{value}} as the function's result.
 * This is normally used for returning @code{struct} and @code{union}
 * values where you have the effective address of the structure, rather
 * than the structure's contents, in @var{value}.
 * @end deftypefun
@*/
int
jit_insn_return_ptr(jit_function_t func, jit_value_t value, jit_type_t type)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

#if !defined(JIT_BACKEND_INTERP)
	/* We need to pop the "setjmp" context */
	if(func->has_try)
	{
		type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, 0, 0, 1);
		if(!type)
		{
			return 0;
		}
		jit_insn_call_native(func, "_jit_unwind_pop_setjmp",
				     (void *) _jit_unwind_pop_setjmp, type,
				     0, 0, JIT_CALL_NOTHROW);
		jit_type_free(type);
	}
#endif

	/* This function has an ordinary return path */
	func->builder->ordinary_return = 1;

	/* Convert the value into a pointer */
	value = jit_insn_convert(func, value, jit_type_void_ptr, 0);
	if(!value)
	{
		return 0;
	}

	/* Determine how to return the value, based on the pointed-to type */
	jit_value_t return_ptr, size_value;
	switch(jit_type_remove_tags(type)->kind)
	{
	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
		size_value = jit_value_create_nint_constant(func, jit_type_nint,
							    jit_type_get_size(type));
		if(!size_value)
		{
			return 0;
		}

		/* Determine the kind of structure return to use */
		return_ptr = jit_value_get_struct_pointer(func);
		if(return_ptr)
		{
			/* Copy the structure's contents to the supplied pointer */
			if(!jit_insn_memcpy(func, return_ptr, value, size_value))
			{
				return 0;
			}
			/* Output a regular return for the function */
			if(!create_noarg_note(func, JIT_OP_RETURN))
			{
				return 0;
			}
		}
		else
		{
			/* Return the structure via registers */
			if(!create_note(func, JIT_OP_RETURN_SMALL_STRUCT, value, size_value))
			{
				return 0;
			}
		}
		break;

	default:
		/* Everything else uses the normal return logic */
		value = jit_insn_load_relative(func, value, 0, type);
		if(!value)
		{
			return 0;
		}
		return jit_insn_return(func, value);
	}

	/* Mark the current block as "ends in dead" */
	func->builder->current_block->ends_in_dead = 1;

	/* Start a new block just after the "return" instruction */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_default_return (jit_function_t @var{func})
 * Add an instruction to return a default value if control reaches this point.
 * This is typically used at the end of a function to ensure that all paths
 * return to the caller.  Returns zero if out of memory, 1 if a default
 * return was added, and 2 if a default return was not needed.
 *
 * Note: if this returns 1, but the function signature does not return
 * @code{void}, then it indicates that a higher-level language error
 * has occurred and the function should be abandoned.
 * @end deftypefun
@*/
int
jit_insn_default_return(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* If the last block ends in an unconditional branch, or is dead,
	   then we don't need to add a default return */
	if(jit_block_current_is_dead(func))
	{
		return 2;
	}

	/* Add a simple "void" return to terminate the function */
	return jit_insn_return(func, 0);
}

/*@
 * @deftypefun int jit_insn_throw (jit_function_t @var{func}, jit_value_t @var{value})
 * Throw a pointer @var{value} as an exception object.  This can also
 * be used to "rethrow" an object from a catch handler that is not
 * interested in handling the exception.
 * @end deftypefun
@*/
int
jit_insn_throw(jit_function_t func, jit_value_t value)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	func->builder->may_throw = 1;
	func->builder->non_leaf = 1;	/* May have to call out to throw */
	if(!create_unary_note(func, JIT_OP_THROW, value))
	{
		return 0;
	}
	func->builder->current_block->ends_in_dead = 1;
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun jit_value_t jit_insn_get_call_stack (jit_function_t @var{func})
 * Get an object that represents the current position in the code,
 * and all of the functions that are currently on the call stack.
 * This is equivalent to calling @code{jit_exception_get_stack_trace},
 * and is normally used just prior to @code{jit_insn_throw} to record
 * the location of the exception that is being thrown.
 * @end deftypefun
@*/
jit_value_t
jit_insn_get_call_stack(jit_function_t func)
{
	/* Create a signature prototype for "void *()" */
	jit_type_t type = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, 0, 0, 1);
	if(!type)
	{
		return 0;
	}

	/* Call "jit_exception_get_stack_trace" to obtain the stack trace */
	jit_value_t value
		= jit_insn_call_native(func, "jit_exception_get_stack_trace",
				       (void *) jit_exception_get_stack_trace,
				       type, 0, 0, 0);

	/* Clean up and exit */
	jit_type_free(type);
	return value;
}

/*@
 * @deftypefun jit_value_t jit_insn_thrown_exception (jit_function_t @var{func})
 * Get the value that holds the most recent thrown exception.  This is
 * typically used in @code{catch} clauses.
 * @end deftypefun
@*/
jit_value_t
jit_insn_thrown_exception(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	if(!func->builder->thrown_exception)
	{
		func->builder->thrown_exception =
			jit_value_create(func, jit_type_void_ptr);
	}
	return func->builder->thrown_exception;
}

/*
 * Initialize the "setjmp" setup block that is needed to catch exceptions
 * thrown back to this level of execution.  The block looks like this:
 *
 *	jit_jmp_buf jbuf;
 *	void *catcher;
 *
 *	_jit_unwind_push_setjmp(&jbuf);
 *	if(setjmp(&jbuf.buf))
 *	{
 *		catch_pc = jbuf.catch_pc;
 *		if(catch_pc)
 *		{
 *			jbuf.catch_pc = 0;
 *			goto *catcher;
 *		}
 *		else
 *		{
 *			_jit_unwind_pop_and_rethrow();
 *		}
 *     	}
 *
 * The field "jbuf.catch_pc" will be set to the address of the relevant
 * "catch" block just before a subroutine call that may involve exceptions.
 * It will be reset to NULL after such subroutine calls.
 *
 * Native back ends are responsible for outputting a call to the function
 * "_jit_unwind_pop_setjmp()" just before "return" instructions if the
 * "has_try" flag is set on the function.
 */
static int
initialize_setjmp_block(jit_function_t func)
{
#if !defined(JIT_BACKEND_INTERP)
	jit_label_t start_label = jit_label_undefined;
	jit_label_t end_label = jit_label_undefined;
	jit_label_t code_label = jit_label_undefined;
	jit_label_t rethrow_label = jit_label_undefined;
	jit_value_t args[2];
	jit_value_t value;

	/* Bail out if we have already done this before */
	if(func->builder->setjmp_value)
	{
		return 1;
	}
	func->builder->catcher_label = jit_label_undefined;

	/* Force the start of a new block to mark the start of the init code */
	if(!jit_insn_label_tight(func, &start_label))
	{
		return 0;
	}

	/* Create a value to hold an item of type "jit_jmp_buf" */
	jit_type_t type = jit_type_create_struct(0, 0, 1);
	if(!type)
	{
		return 0;
	}
	jit_type_set_size_and_alignment(type, sizeof(jit_jmp_buf), JIT_BEST_ALIGNMENT);
	func->builder->setjmp_value = jit_value_create(func, type);
	if(!func->builder->setjmp_value)
	{
		jit_type_free(type);
		return 0;
	}
	jit_type_free(type);

	/* Call "_jit_unwind_push_setjmp" with "&setjmp_value" as its argument */
	type = jit_type_void_ptr;
	type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, &type, 1, 1);
	if(!type)
	{
		return 0;
	}
	args[0] = jit_insn_address_of(func, func->builder->setjmp_value);
	if(!args[0])
	{
		return 0;
	}
	jit_insn_call_native(func, "_jit_unwind_push_setjmp",
			     (void *) _jit_unwind_push_setjmp, type,
			     args, 1, JIT_CALL_NOTHROW);
	jit_type_free(type);

	/* Call "__sigsetjmp" or "setjmp" with "&setjmp_value" as its argument.
	   We prefer "__sigsetjmp" because it is least likely to be a macro */
#if defined(HAVE___SIGSETJMP) || defined(HAVE_SIGSETJMP)
	{
		jit_type_t params[2];
		params[0] = jit_type_void_ptr;
		params[1] = jit_type_sys_int;
		type = jit_type_create_signature(jit_abi_cdecl, jit_type_int, params, 2, 1);
		if(!type)
		{
			return 0;
		}
	}
	args[0] = jit_insn_address_of(func, func->builder->setjmp_value);
	args[1] = jit_value_create_nint_constant(func, jit_type_sys_int, 1);
	if(!args[0] || !args[1])
	{
		jit_type_free(type);
		return 0;
	}
#if defined(HAVE___SIGSETJMP)
	value = jit_insn_call_native(func, "__sigsetjmp", (void *) __sigsetjmp,
				     type, args, 2, JIT_CALL_NOTHROW);
#else
	value = jit_insn_call_native(func, "sigsetjmp", (void *) sigsetjmp,
				     type, args, 2, JIT_CALL_NOTHROW);
#endif
	jit_type_free(type);
	if(!value)
	{
		return 0;
	}
#else	/* !HAVE_SIGSETJMP */
	type = jit_type_void_ptr;
	type = jit_type_create_signature(jit_abi_cdecl, jit_type_int, &type, 1, 1);
	if(!type)
	{
		return 0;
	}
	args[0] = jit_insn_address_of(func, func->builder->setjmp_value);
	if(!args[0])
	{
		jit_type_free(type);
		return 0;
	}
#if defined(HAVE__SETJMP)
	value = jit_insn_call_native(func, "_setjmp", (void *) _setjmp,
				     type, args, 1, JIT_CALL_NOTHROW);

#else
	value = jit_insn_call_native(func, "setjmp", (void *) setjmp,
				     type, args, 1, JIT_CALL_NOTHROW);
#endif

	jit_type_free(type);
	if(!value)
	{
		return 0;
	}
#endif	/* !HAVE_SIGSETJMP */

	/* Branch to the end of the init code if "setjmp" returned zero */
	if(!jit_insn_branch_if_not(func, value, &code_label))
	{
		return 0;
	}

	/* We need a value to hold the location of the thrown exception */
	func->builder->thrown_pc = jit_value_create(func, jit_type_void_ptr);
	if(func->builder->thrown_pc == 0)
	{
		return 0;
	}

	/* Get the value of "catch_pc" from within "setjmp_value" and store it
	   into the current frame.  This indicates where the exception occurred */
	value = jit_insn_address_of(func, func->builder->setjmp_value);
	if(!value)
	{
		return 0;
	}
	value = jit_insn_load_relative(func, value, jit_jmp_catch_pc_offset, jit_type_void_ptr);
	if(!value)
	{
		return 0;
	}
	if(!jit_insn_store(func, func->builder->thrown_pc, value))
	{
		return 0;
	}
	if(!jit_insn_branch_if_not(func, value, &rethrow_label))
	{
		return 0;
	}

	/* Clear the original "catch_pc" value within "setjmp_value" */
	jit_value_t null = jit_value_create_nint_constant(func, jit_type_void_ptr, 0);
	value = jit_insn_address_of(func, func->builder->setjmp_value);
	if(!null || !value)
	{
		return 0;
	}
	if(!jit_insn_store_relative(func, value, jit_jmp_catch_pc_offset, null))
	{
		return 0;
	}

	/* Jump to this function's exception catcher */
	if(!jit_insn_branch(func, &func->builder->catcher_label))
	{
		return 0;
	}

	/* Mark the position of the rethrow label */
	if(!jit_insn_label_tight(func, &rethrow_label))
	{
		return 0;
	}

	/* Call "_jit_unwind_pop_and_rethrow" to pop the current
	   "setjmp" context and then rethrow the current exception */
	type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, 0, 0, 1);
	if(!type)
	{
		return 0;
	}
	jit_insn_call_native(func, "_jit_unwind_pop_and_rethrow",
			     (void *) _jit_unwind_pop_and_rethrow, type, 0, 0,
			     JIT_CALL_NOTHROW | JIT_CALL_NORETURN);
	jit_type_free(type);

	/* Insert the target to jump to the normal code. */
	if(!jit_insn_label_tight(func, &code_label))
	{
		return 0;
	}

	/* Force the start of a new block to mark the end of the init code */
	if(!jit_insn_label(func, &end_label))
	{
		return 0;
	}

	/* Move the initialization code to the head of the function so that
	   it is performed once upon entry to the function */
	return jit_insn_move_blocks_to_start(func, start_label, end_label);
#else
	/* The interpreter doesn't need the "setjmp" setup block */
	func->builder->catcher_label = jit_label_undefined;
	return 1;
#endif
}

/*@
 * @deftypefun int jit_insn_uses_catcher (jit_function_t @var{func})
 * Notify the function building process that @var{func} contains
 * some form of @code{catch} clause for catching exceptions.  This must
 * be called before any instruction that is covered by a @code{try},
 * ideally at the start of the function output process.
 * @end deftypefun
@*/
int
jit_insn_uses_catcher(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	if(func->has_try)
	{
		return 1;
	}
	func->has_try = 1;
	func->builder->may_throw = 1;
	func->builder->non_leaf = 1;
	return initialize_setjmp_block(func);
}

/*@
 * @deftypefun jit_value_t jit_insn_start_catcher (jit_function_t @var{func})
 * Start the catcher block for @var{func}.  There should be exactly one
 * catcher block for any function that involves a @code{try}.  All
 * exceptions that are thrown within the function will cause control
 * to jump to this point.  Returns a value that holds the exception
 * that was thrown.
 * @end deftypefun
@*/
jit_value_t
jit_insn_start_catcher(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	if(!jit_insn_label_tight(func, &func->builder->catcher_label))
	{
		return 0;
	}
	jit_value_t value = jit_insn_thrown_exception(func);
	if(!value)
	{
		return 0;
	}
#if defined(JIT_BACKEND_INTERP)
	/* In the interpreter, the exception object will be on the top of
	   the operand stack when control reaches the catcher */
	if(!jit_insn_incoming_reg(func, value, 0))
	{
		return 0;
	}
#else
	jit_type_t type = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, 0, 0, 1);
	if(!type)
	{
		return 0;
	}
	jit_value_t last_exception
		= jit_insn_call_native(func, "jit_exception_get_last",
				       (void *) jit_exception_get_last,
				       type, 0, 0, JIT_CALL_NOTHROW);
	jit_insn_store(func, value, last_exception);
	jit_type_free(type);
#endif
	return value;
}

/*@
 * @deftypefun int jit_insn_branch_if_pc_not_in_range (jit_function_t @var{func}, jit_label_t @var{start_label}, jit_label_t @var{end_label}, jit_label_t *@var{label})
 * Branch to @var{label} if the program counter where an exception occurred
 * does not fall between @var{start_label} and @var{end_label}.
 * @end deftypefun
@*/
int
jit_insn_branch_if_pc_not_in_range(jit_function_t func, jit_label_t start_label,
				   jit_label_t end_label, jit_label_t *label)
{
	/* Ensure that we have a function builder and a try block */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}
	if(!func->has_try)
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Get the location where the exception occurred in this function */
#if defined(JIT_BACKEND_INTERP)
	jit_value_t value1 = create_dest_note(func, JIT_OP_LOAD_EXCEPTION_PC, jit_type_void_ptr);
#else
	jit_value_t value1 = func->builder->thrown_pc;
#endif
	if(!value1)
	{
		return 0;
	}

	/* Compare the location against the start and end labels */
	jit_value_t value2 = jit_insn_address_of_label(func, &start_label);
	if(!value2)
	{
		return 0;
	}
	value2 = jit_insn_lt(func, value1, value2);
	if(!value2 || !jit_insn_branch_if(func, value2, label))
	{
		return 0;
	}
	value2 = jit_insn_address_of_label(func, &end_label);
	if(!value2)
	{
		return 0;
	}
	value2 = jit_insn_ge(func, value1, value2);
	if(!value2 || !jit_insn_branch_if(func, value2, label))
	{
		return 0;
	}

	/* If control gets here, then we have a location match */
	return 1;
}

/*@
 * @deftypefun int jit_insn_rethrow_unhandled (jit_function_t @var{func})
 * Rethrow the current exception because it cannot be handled by
 * any of the @code{catch} blocks in the current function.
 *
 * Note: this is intended for use within catcher blocks.  It should not
 * be used to rethrow exceptions in response to programmer requests
 * (e.g. @code{throw;} in C#).  The @code{jit_insn_throw} function
 * should be used for that purpose.
 * @end deftypefun
@*/
int
jit_insn_rethrow_unhandled(jit_function_t func)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Get the current exception value to be thrown */
	jit_value_t value = jit_insn_thrown_exception(func);
	if(!value)
	{
		return 0;
	}

#if defined(JIT_BACKEND_INTERP)

	/* Rethrow the current exception (interpreter version) */
	if(!create_unary_note(func, JIT_OP_RETHROW, value))
	{
		return 0;
	}

#else /* !JIT_BACKEND_INTERP */

	/* Call "_jit_unwind_pop_setjmp" to remove the current exception catcher */
	jit_type_t type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, 0, 0, 1);
	if(!type)
	{
		return 0;
	}
	jit_insn_call_native(func, "_jit_unwind_pop_setjmp",
			     (void *) _jit_unwind_pop_setjmp, type, 0, 0,
			     JIT_CALL_NOTHROW);
	jit_type_free(type);

	/* Call the "jit_exception_throw" function to effect the rethrow */
	type = jit_type_void_ptr;
	type = jit_type_create_signature(jit_abi_cdecl, jit_type_void, &type, 1, 1);
	if(!type)
	{
		return 0;
	}
	jit_insn_call_native(func, "jit_exception_throw",
			     (void *) jit_exception_throw, type, &value, 1,
			     JIT_CALL_NOTHROW | JIT_CALL_NORETURN);
	jit_type_free(type);

#endif /* !JIT_BACKEND_INTERP */

	/* The current block ends in dead and we need to start a new block */
	func->builder->current_block->ends_in_dead = 1;
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_start_finally (jit_function_t @var{func}, jit_label_t *@var{finally_label})
 * Start a @code{finally} clause.
 * @end deftypefun
@*/
int
jit_insn_start_finally(jit_function_t func, jit_label_t *finally_label)
{
	if(!jit_insn_label_tight(func, finally_label))
	{
		return 0;
	}
	return create_noarg_note(func, JIT_OP_ENTER_FINALLY);
}

/*@
 * @deftypefun int jit_insn_return_from_finally (jit_function_t @var{func})
 * Return from the @code{finally} clause to where it was called from.
 * This is usually the last instruction in a @code{finally} clause.
 * @end deftypefun
@*/
int
jit_insn_return_from_finally(jit_function_t func)
{
	/* Flush any deferred stack pops before we return */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Mark the end of the "finally" clause */
	if(!create_noarg_note(func, JIT_OP_LEAVE_FINALLY))
	{
		return 0;
	}

	/* The current block ends in a dead instruction */
	func->builder->current_block->ends_in_dead = 1;

	/* Create a new block for the following code */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_call_finally (jit_function_t @var{func}, jit_label_t *@var{finally_label})
 * Call a @code{finally} clause.
 * @end deftypefun
@*/
int
jit_insn_call_finally(jit_function_t func, jit_label_t *finally_label)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Allocate the label number if necessary */
	if(*finally_label == jit_label_undefined)
	{
		*finally_label = func->builder->next_label++;
	}

	/* Calling a finally handler makes the function not a leaf because
	   we may need to do a native "call" to invoke the handler */
	func->builder->non_leaf = 1;

	/* Add a new branch instruction to branch to the finally handler */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) JIT_OP_CALL_FINALLY;
	insn->flags = JIT_INSN_DEST_IS_LABEL;
	insn->dest = (jit_value_t) *finally_label;

	/* Create a new block for the following code */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun jit_value_t jit_insn_start_filter (jit_function_t @var{func}, jit_label_t *@var{label}, jit_type_t @var{type})
 * Define the start of a filter.  Filters are embedded subroutines within
 * functions that are used to filter exceptions in @code{catch} blocks.
 *
 * A filter subroutine takes a single argument (usually a pointer) and
 * returns a single result (usually a boolean).  The filter has complete
 * access to the local variables of the function, and can use any of
 * them in the filtering process.
 *
 * This function returns a temporary value of the specified @var{type},
 * indicating the parameter that is supplied to the filter.
 * @end deftypefun
@*/
jit_value_t
jit_insn_start_filter(jit_function_t func, jit_label_t *label, jit_type_t type)
{
	/* Set a label at this point to start a new block */
	if(!jit_insn_label_tight(func, label))
	{
		return 0;
	}

	/* Create a note to load the filter's parameter at runtime */
	return create_dest_note(func, JIT_OP_ENTER_FILTER, type);
}

/*@
 * @deftypefun int jit_insn_return_from_filter (jit_function_t @var{func}, jit_value_t @var{value})
 * Return from a filter subroutine with the specified @code{value} as
 * its result.
 * @end deftypefun
@*/
int
jit_insn_return_from_filter(jit_function_t func, jit_value_t value)
{
	/* Flush any deferred stack pops before we return */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Mark the end of the "filter" clause */
	if(!create_unary_note(func, JIT_OP_LEAVE_FILTER, value))
	{
		return 0;
	}

	/* The current block ends in a dead instruction */
	func->builder->current_block->ends_in_dead = 1;

	/* Create a new block for the following code */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun jit_value_t jit_insn_call_filter (jit_function_t @var{func}, jit_label_t *@var{label}, jit_value_t @var{value}, jit_type_t @var{type})
 * Call the filter subroutine at @var{label}, passing it @var{value} as
 * its argument.  This function returns a value of the specified
 * @var{type}, indicating the filter's result.
 * @end deftypefun
@*/
jit_value_t
jit_insn_call_filter(jit_function_t func, jit_label_t *label,
		     jit_value_t value, jit_type_t type)
{
	/* Ensure that we have a function builder */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Flush any stack pops that were deferred previously */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Allocate the label number if necessary */
	if(*label == jit_label_undefined)
	{
		*label = func->builder->next_label++;
	}

	/* Calling a filter makes the function not a leaf because we may
	   need to do a native "call" to invoke the handler */
	func->builder->non_leaf = 1;

	/* Add a new branch instruction to branch to the filter */
	jit_insn_t insn = _jit_block_add_insn(func->builder->current_block);
	if(!insn)
	{
		return 0;
	}
	insn->opcode = (short) JIT_OP_CALL_FILTER;
	insn->flags = JIT_INSN_DEST_IS_LABEL;
	insn->dest = (jit_value_t) *label;
	insn->value1 = value;
	jit_value_ref(func, value);

	/* Create a new block, and add the filter return logic to it */
	if(!jit_insn_new_block(func))
	{
		return 0;
	}
	return create_dest_note(func, JIT_OP_CALL_FILTER_RETURN, type);
}

/*@
 * @deftypefun int jit_insn_memcpy (jit_function_t @var{func}, jit_value_t @var{dest}, jit_value_t @var{src}, jit_value_t @var{size})
 * Copy the @var{size} bytes of memory at @var{src} to @var{dest}.
 * It is assumed that the source and destination do not overlap.
 * @end deftypefun
@*/
int
jit_insn_memcpy(jit_function_t func, jit_value_t dest, jit_value_t src, jit_value_t size)
{
	size = jit_insn_convert(func, size, jit_type_nint, 0);
	if(!size)
	{
		return 0;
	}
	return apply_ternary(func, JIT_OP_MEMCPY, dest, src, size);
}

/*@
 * @deftypefun int jit_insn_memmove (jit_function_t @var{func}, jit_value_t @var{dest}, jit_value_t @var{src}, jit_value_t @var{size})
 * Copy the @var{size} bytes of memory at @var{src} to @var{dest}.
 * This is save to use if the source and destination overlap.
 * @end deftypefun
@*/
int
jit_insn_memmove(jit_function_t func, jit_value_t dest, jit_value_t src, jit_value_t size)
{
	size = jit_insn_convert(func, size, jit_type_nint, 0);
	if(!size)
	{
		return 0;
	}
	return apply_ternary(func, JIT_OP_MEMMOVE, dest, src, size);
}

/*@
 * @deftypefun int jit_insn_memset (jit_function_t @var{func}, jit_value_t @var{dest}, jit_value_t @var{value}, jit_value_t @var{size})
 * Set the @var{size} bytes at @var{dest} to @var{value}.
 * @end deftypefun
@*/
int
jit_insn_memset(jit_function_t func, jit_value_t dest, jit_value_t value, jit_value_t size)
{
	value = jit_insn_convert(func, value, jit_type_int, 0);
	size = jit_insn_convert(func, size, jit_type_nint, 0);
	if(!value || !size)
	{
		return 0;
	}
	return apply_ternary(func, JIT_OP_MEMSET, dest, value, size);
}

/*@
 * @deftypefun jit_value_t jit_insn_alloca (jit_function_t @var{func}, jit_value_t @var{size})
 * Allocate @var{size} bytes of memory from the stack.
 * @end deftypefun
@*/
jit_value_t
jit_insn_alloca(jit_function_t func, jit_value_t size)
{
	/* Make sure that all deferred pops have been done */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Round the size to the best alignment boundary on this platform */
	size = jit_insn_convert(func, size, jit_type_nuint, 0);
	jit_value_t addon = jit_value_create_nint_constant(
		func, jit_type_nuint, JIT_BEST_ALIGNMENT - 1);
	jit_value_t mask = jit_value_create_nint_constant(
		func, jit_type_nuint, ~((jit_nint) (JIT_BEST_ALIGNMENT - 1)));
	if(!size || !addon || !mask)
	{
		return 0;
	}
	size = jit_insn_add(func, size, addon);
	if(!size)
	{
		return 0;
	}
	size = jit_insn_and(func, size, mask);
	if(!size)
	{
		return 0;
	}

	/* Allocate "size" bytes of memory from the stack */
	return apply_unary(func, JIT_OP_ALLOCA, size, jit_type_void_ptr);
}

/*@
 * @deftypefun int jit_insn_move_blocks_to_end (jit_function_t @var{func}, jit_label_t @var{from_label}, jit_label_t @var{to_label})
 * Move all of the blocks between @var{from_label} (inclusive) and
 * @var{to_label} (exclusive) to the end of the current function.
 * This is typically used to move the expression in a @code{while}
 * loop to the end of the body, where it can be executed more
 * efficiently.
 * @end deftypefun
@*/
int
jit_insn_move_blocks_to_end(jit_function_t func, jit_label_t from_label,
			    jit_label_t to_label)
{
	/* Make sure that deferred stack pops are flushed */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Find the first block that needs to be moved */
	jit_block_t first = jit_block_from_label(func, from_label);
	if(!first)
	{
		return 0;
	}

	/* Find the last block that needs to be moved */
	jit_block_t last = jit_block_from_label(func, to_label);
	if(!last)
	{
		return 0;
	}

	/* Sanity check -- the last block has to be after the first */
	jit_block_t block;
	for(block = first->next; block != last; block = block->next)
	{
		if(!block)
		{
			return 0;
		}
	}

	/* The last block is excluded from the blocks to move */
	block = last->prev;

	/* Move the blocks to the end */
	_jit_block_detach(first, block);
	_jit_block_attach_before(func->builder->exit_block, first, block);
	func->builder->current_block = block;

	/* Create a new block after the last one we moved, to start fresh */
	return jit_insn_new_block(func);
}

/*@
 * @deftypefun int jit_insn_move_blocks_to_start (jit_function_t @var{func}, jit_label_t @var{from_label}, jit_label_t @var{to_label})
 * Move all of the blocks between @var{from_label} (inclusive) and
 * @var{to_label} (exclusive) to the start of the current function.
 * This is typically used to move initialization code to the head
 * of the function.
 * @end deftypefun
@*/
int
jit_insn_move_blocks_to_start(jit_function_t func, jit_label_t from_label, jit_label_t to_label)
{
	/* Make sure that deferred stack pops are flushed */
	if(!jit_insn_flush_defer_pop(func, 0))
	{
		return 0;
	}

	/* Find the first block that needs to be moved */
	jit_block_t first = jit_block_from_label(func, from_label);
	if(!first)
	{
		return 0;
	}

	/* Find the last block that needs to be moved */
	jit_block_t last = jit_block_from_label(func, to_label);
	if(!last)
	{
		return 0;
	}

	/* Init block */
	jit_block_t init = func->builder->init_block;

	/* Sanity check -- the first block has to be after the init */
	jit_block_t block;
	for(block = init->next; block != first; block = block->next)
	{
		if(!block)
		{
			return 0;
		}
	}
	/* Sanity check -- the last block has to be after the first */
	for(block = first->next; block != last; block = block->next)
	{
		if(!block)
		{
			return 0;
		}
	}

	/* The last block is excluded from the blocks to move */
	block = last->prev;

	/* Update the init block pointer */
	func->builder->init_block = block;

	/* Move the blocks after the original init block */
	if(init->next != first)
	{
		_jit_block_detach(first, block);
		_jit_block_attach_after(init, first, block);
	}

	/* Done */
	return 1;
}

/*@
 * @deftypefun int jit_insn_mark_offset (jit_function_t @var{func}, jit_int @var{offset})
 * Mark the current position in @var{func} as corresponding to the
 * specified bytecode @var{offset}.  This value will be returned
 * by @code{jit_stack_trace_get_offset}, and is useful for associating
 * code positions with source line numbers.
 * @end deftypefun
@*/
int
jit_insn_mark_offset(jit_function_t func, jit_int offset)
{
	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_value_t value = jit_value_create_nint_constant(func, jit_type_int, offset);
	if(!value)
	{
		return 0;
	}

	/* If the previous instruction is mark offset too
	   then just replace the offset value in place --
	   we are not interested in bytecodes that produce
	   no real code. */
	jit_block_t block = func->builder->current_block;
	jit_insn_t last = _jit_block_get_last(block);
	if(last && last->opcode == JIT_OP_MARK_OFFSET)
	{
		last->value1 = value;
		return 1;
	}

	return create_unary_note(func, JIT_OP_MARK_OFFSET, value);
}

/* Documentation is in jit-debugger.c */
int
jit_insn_mark_breakpoint_variable(jit_function_t func, jit_value_t data1, jit_value_t data2)
{
#if defined(JIT_BACKEND_INTERP)
	/* Use the "mark_breakpoint" instruction for the interpreter */
	if(!jit_insn_new_block(func))
	{
		return 0;
	}
	return create_note(func, JIT_OP_MARK_BREAKPOINT, data1, data2);
#else
	/* Insert a call to "_jit_debugger_hook" on native platforms */
	jit_type_t params[3];
	jit_type_t signature;
	jit_value_t values[3];
	params[0] = jit_type_void_ptr;
	params[1] = jit_type_nint;
	params[2] = jit_type_nint;
	signature = jit_type_create_signature(jit_abi_cdecl, jit_type_void, params, 3, 0);
	if(!signature)
	{
		return 0;
	}
	values[0] = jit_value_create_nint_constant(func, jit_type_void_ptr, (jit_nint) func);
	if(!values[0])
	{
		jit_type_free(signature);
		return 0;
	}
	values[1] = data1;
	values[2] = data2;
	jit_insn_call_native(func, "_jit_debugger_hook", (void *) _jit_debugger_hook,
			     signature, values, 3, JIT_CALL_NOTHROW);
	jit_type_free(signature);
	return 1;
#endif
}

/* Documentation is in jit-debugger.c */
int
jit_insn_mark_breakpoint(jit_function_t func, jit_nint data1, jit_nint data2)
{
	jit_value_t value1;
	jit_value_t value2;

	value1 = jit_value_create_nint_constant(func, jit_type_nint, data1);
	value2 = jit_value_create_nint_constant(func, jit_type_nint, data2);
	if(!value1 || !value2)
	{
		return 0;
	}

	return jit_insn_mark_breakpoint_variable(func, value1, value2);
}

/*@
 * @deftypefun void jit_insn_iter_init (jit_insn_iter_t *@var{iter}, jit_block_t @var{block})
 * Initialize an iterator to point to the first instruction in @var{block}.
 * @end deftypefun
@*/
void
jit_insn_iter_init(jit_insn_iter_t *iter, jit_block_t block)
{
	iter->block = block;
	iter->posn = 0;
}

/*@
 * @deftypefun void jit_insn_iter_init_last (jit_insn_iter_t *@var{iter}, jit_block_t @var{block})
 * Initialize an iterator to point to the last instruction in @var{block}.
 * @end deftypefun
@*/
void
jit_insn_iter_init_last(jit_insn_iter_t *iter, jit_block_t block)
{
	iter->block = block;
	iter->posn = block->num_insns;
}

/*@
 * @deftypefun jit_insn_t jit_insn_iter_next (jit_insn_iter_t *@var{iter})
 * Get the next instruction in an iterator's block.  Returns NULL
 * when there are no further instructions in the block.
 * @end deftypefun
@*/
jit_insn_t
jit_insn_iter_next(jit_insn_iter_t *iter)
{
	if(iter->posn < iter->block->num_insns)
	{
		return &iter->block->insns[iter->posn++];
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_insn_t jit_insn_iter_previous (jit_insn_iter_t *@var{iter})
 * Get the previous instruction in an iterator's block.  Returns NULL
 * when there are no further instructions in the block.
 * @end deftypefun
@*/
jit_insn_t
jit_insn_iter_previous(jit_insn_iter_t *iter)
{
	if(iter->posn > 0)
	{
		return &iter->block->insns[--iter->posn];
	}
	else
	{
		return 0;
	}
}
