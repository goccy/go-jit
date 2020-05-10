/*
 * jit-apply.c - Dynamic invocation and closure support functions.
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
#include "jit-apply-rules.h"
#include "jit-apply-func.h"
#if HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_ALLOCA_H
# include <alloca.h>
#endif
#ifdef JIT_WIN32_PLATFORM
# include <malloc.h>
# ifndef alloca
#  define alloca	_alloca
# endif
#endif

/*

If you need to tweak the way that this code behaves for a specific
platform, then you would normally do it in "tools/gen-apply.c" or
the CPU-specific "jit-apply-XXX.h" file, not here.

*/

/*@

@section Function application and closures
@cindex Function application
@cindex Closures
@cindex jit-apply.h

Sometimes all you have for a function is a pointer to it and a dynamic
description of its arguments.  Calling such a function can be extremely
difficult in standard C.  The routines in this section, particularly
@code{jit_apply}, provide a convenient interface for doing this.

At other times, you may wish to wrap up one of your own dynamic functions
in such a way that it appears to be a regular C function.  This is
performed with @code{jit_closure_create}.

@*/

typedef enum
{
	_JIT_APPLY_RETURN_TYPE_OTHER = 0,
	_JIT_APPLY_RETURN_TYPE_FLOAT32 = 1,
	_JIT_APPLY_RETURN_TYPE_FLOAT64 = 2,
	_JIT_APPLY_RETURN_TYPE_NFLOAT = 3
} _jit_apply_return_type;

/*
 * Flags that indicate which structure sizes are returned in registers.
 */
unsigned char const _jit_apply_return_in_reg[] =
		JIT_APPLY_STRUCT_RETURN_IN_REG_INIT;

/*
 * Get the maximum argument stack size of a signature type.
 */
static unsigned int
jit_type_get_max_arg_size(jit_type_t signature)
{
	unsigned int size;
	unsigned int typeSize;
	unsigned int param;
	jit_type_t type;
	if(signature->size)
	{
		/* We have a cached argument size from last time */
		return signature->size;
	}
	size = 0;
	param = jit_type_num_params(signature);
	while(param > 0)
	{
		--param;
		type = jit_type_remove_tags(jit_type_get_param(signature, param));
		switch(type->kind)
		{
		case JIT_TYPE_SBYTE:
		case JIT_TYPE_UBYTE:
		case JIT_TYPE_SHORT:
		case JIT_TYPE_USHORT:
		case JIT_TYPE_INT:
		case JIT_TYPE_UINT:
		case JIT_TYPE_NINT:
		case JIT_TYPE_NUINT:
		case JIT_TYPE_PTR:
		case JIT_TYPE_SIGNATURE:
			size += sizeof(jit_nint);
			break;

		case JIT_TYPE_LONG:
		case JIT_TYPE_ULONG:
#ifdef JIT_NATIVE_INT32
			/* Add one extra word for possible alignment padding */
			size += sizeof(jit_long) + sizeof(jit_nint);
#else
			size += sizeof(jit_nint);
#endif
			break;

		case JIT_TYPE_FLOAT32:
		case JIT_TYPE_FLOAT64:
		case JIT_TYPE_NFLOAT:
			/* Allocate space for an "nfloat" and an alignment word */
			size += sizeof(jit_nfloat) + sizeof(jit_nint) * 2 - 1;
			size &= ~(sizeof(jit_nint) - 1);
			break;

		case JIT_TYPE_STRUCT:
		case JIT_TYPE_UNION:
			/* Allocate space for the structure and an alignment word */
			typeSize = jit_type_get_size(type);
			size += typeSize + sizeof(jit_nint) * 2 - 1;
			size &= ~(sizeof(jit_nint) - 1);
			break;
		}
	}
	type = jit_type_get_return(signature);
	if(jit_type_is_struct(type) || jit_type_is_union(type))
	{
		/* Add one extra word for the possibility of a structure pointer */
		size += sizeof(jit_nint);
	}
	signature->size = size;
	return size;
}

/*
 * Copy apply arguments into position.
 */
static void
jit_apply_builder_add_arguments(jit_apply_builder *builder, jit_type_t signature,
				void **args, unsigned int index, unsigned int num_args)
{
	unsigned int param;
	for(param = 0; param < num_args; ++param)
	{
		void *arg = args[param];
		jit_type_t type = jit_type_get_param(signature, index + param);
		type = jit_type_remove_tags(type);
		switch(type->kind)
		{
		case JIT_TYPE_SBYTE:
			jit_apply_builder_add_sbyte(builder, *((jit_sbyte *) arg));
			break;

		case JIT_TYPE_UBYTE:
			jit_apply_builder_add_ubyte(builder, *((jit_ubyte *) arg));
			break;

		case JIT_TYPE_SHORT:
			jit_apply_builder_add_short(builder, *((jit_short *) arg));
			break;

		case JIT_TYPE_USHORT:
			jit_apply_builder_add_ushort(builder, *((jit_ushort *) arg));
			break;

		case JIT_TYPE_INT:
			jit_apply_builder_add_int(builder, *((jit_int *) arg));
			break;

		case JIT_TYPE_UINT:
			jit_apply_builder_add_uint(builder, *((jit_uint *) arg));
			break;

		case JIT_TYPE_NINT:
		case JIT_TYPE_PTR:
		case JIT_TYPE_SIGNATURE:
			jit_apply_builder_add_nint(builder, *((jit_nint *) arg));
			break;

		case JIT_TYPE_NUINT:
			jit_apply_builder_add_nuint(builder, *((jit_nuint *) arg));
			break;

		case JIT_TYPE_LONG:
			jit_apply_builder_add_long(builder, *((jit_long *) arg));
			break;

		case JIT_TYPE_ULONG:
			jit_apply_builder_add_ulong(builder, *((jit_ulong *) arg));
			break;

		case JIT_TYPE_FLOAT32:
			jit_apply_builder_add_float32(builder, *((jit_float32 *) arg));
			break;

		case JIT_TYPE_FLOAT64:
			jit_apply_builder_add_float64(builder, *((jit_float64 *) arg));
			break;

		case JIT_TYPE_NFLOAT:
			jit_apply_builder_add_nfloat(builder, *((jit_nfloat *) arg));
			break;

		case JIT_TYPE_STRUCT:
		case JIT_TYPE_UNION:
#ifdef HAVE_JIT_BUILTIN_APPLY_STRUCT
			_jit_builtin_apply_add_struct(builder, arg, type);
#else
			jit_apply_builder_add_struct(builder, arg,
						     jit_type_get_size(type),
						     jit_type_get_alignment(type));
#endif
			break;
		}
	}
}

/*
 * Get the return value after calling a function using "__builtin_apply".
 */
static void
jit_apply_builder_get_return(jit_apply_builder *builder, void *rv,
			     jit_type_t type, jit_apply_return *result)
{
#ifndef HAVE_JIT_BUILTIN_APPLY_STRUCT_RETURN
	unsigned int size;
#endif

	switch(type->kind)
	{
	case JIT_TYPE_SBYTE:
		*((jit_sbyte *) rv) = jit_apply_return_get_sbyte(result);
		break;

	case JIT_TYPE_UBYTE:
		*((jit_ubyte *) rv) = jit_apply_return_get_ubyte(result);
		break;

	case JIT_TYPE_SHORT:
		*((jit_short *) rv) = jit_apply_return_get_short(result);
		break;

	case JIT_TYPE_USHORT:
		*((jit_ushort *) rv) = jit_apply_return_get_ushort(result);
		break;

	case JIT_TYPE_INT:
		*((jit_int *) rv) = jit_apply_return_get_int(result);
		break;

	case JIT_TYPE_UINT:
		*((jit_uint *) rv) = jit_apply_return_get_uint(result);
		break;

	case JIT_TYPE_NINT:
	case JIT_TYPE_PTR:
	case JIT_TYPE_SIGNATURE:
		*((jit_nint *) rv) = jit_apply_return_get_nint(result);
		break;

	case JIT_TYPE_NUINT:
		*((jit_nuint *) rv) = jit_apply_return_get_nuint(result);
		break;

	case JIT_TYPE_LONG:
		*((jit_long *) rv) = jit_apply_return_get_long(result);
		break;

	case JIT_TYPE_ULONG:
		*((jit_ulong *) rv) = jit_apply_return_get_ulong(result);
		break;

	case JIT_TYPE_FLOAT32:
		*((jit_float32 *) rv) = jit_apply_return_get_float32(result);
		break;

	case JIT_TYPE_FLOAT64:
		*((jit_float64 *) rv) = jit_apply_return_get_float64(result);
		break;

	case JIT_TYPE_NFLOAT:
		*((jit_nfloat *) rv) = jit_apply_return_get_nfloat(result);
		break;

	case JIT_TYPE_STRUCT:
	case JIT_TYPE_UNION:
#ifdef HAVE_JIT_BUILTIN_APPLY_STRUCT_RETURN
		_jit_builtin_apply_get_struct_return(builder, rv, result, type);
#else
		size = jit_type_get_size(type);
		jit_apply_builder_get_struct_return(builder, size, rv, result);
#endif
		break;
	}
}

/*@
 * @deftypefun void jit_apply (jit_type_t signature, void *@var{func}, void **@var{args}, unsigned int @var{num_fixed_args}, void *@var{return_value})
 * Call a function that has a particular function signature.
 * If the signature has more than @var{num_fixed_args} arguments,
 * then it is assumed to be a vararg call, with the additional
 * arguments passed in the vararg argument area on the stack.
 * The @var{signature} must specify the type of all arguments,
 * including those in the vararg argument area.
 * @end deftypefun
@*/
void
jit_apply(jit_type_t signature, void *func,
	  void **args, unsigned int num_fixed_args,
	  void *return_value)
{
	jit_apply_builder builder;
	unsigned int size;
	jit_apply_return *apply_return;
	jit_type_t type;

	/* Initialize the argument builder */
	jit_apply_builder_init(&builder, signature);

	/* Handle the structure return argument */
	type = jit_type_remove_tags(jit_type_get_return(signature));
	if(jit_type_is_struct(type) || jit_type_is_union(type))
	{
		size = jit_type_get_size(type);
		jit_apply_builder_add_struct_return(&builder, size,
						    return_value);
	}

	/* Copy the arguments into position */
	jit_apply_builder_add_arguments(&builder, signature, args, 0,
					num_fixed_args);
	jit_apply_builder_start_varargs(&builder);
	jit_apply_builder_add_arguments(&builder, signature,
					args + num_fixed_args, num_fixed_args,
					jit_type_num_params(signature) - num_fixed_args);

	/* Call the function using "__builtin_apply" or something similar */
	if(type->kind < JIT_TYPE_FLOAT32 || type->kind > JIT_TYPE_NFLOAT)
	{
		jit_builtin_apply(func, builder.apply_args, builder.stack_used,
				  0, apply_return);
	}
	else
	{
		jit_builtin_apply(func, builder.apply_args, builder.stack_used,
				  1, apply_return);
	}

	/* Copy the return value into position */
	if(return_value != 0 && type != jit_type_void)
	{
		jit_apply_builder_get_return(&builder, return_value, type,
					     apply_return);
	}
}

/*@
 * @deftypefun void jit_apply_raw (jit_type_t @var{signature}, void *@var{func}, void *@var{args}, void *@var{return_value})
 * Call a function, passing a set of raw arguments.  This can only
 * be used if @code{jit_raw_supported} returns non-zero for the signature.
 * The @var{args} value is assumed to be an array of @code{jit_nint} values
 * that correspond to each of the arguments.  Raw function calls
 * are slightly faster than their non-raw counterparts, but can
 * only be used in certain circumstances.
 * @end deftypefun
@*/
void
jit_apply_raw(jit_type_t signature, void *func,
	      void *args, void *return_value)
{
	jit_apply_return *apply_return;
	unsigned int size;
	jit_type_t type;

	/* Call the function using "__builtin_apply" or something similar */
	type = jit_type_remove_tags(jit_type_get_return(signature));
	size = jit_type_num_params(signature) * sizeof(jit_nint);
	if(type->kind < JIT_TYPE_FLOAT32 || type->kind > JIT_TYPE_NFLOAT)
	{
		jit_builtin_apply(func, args, size, 0, apply_return);
	}
	else
	{
		jit_builtin_apply(func, args, size, 1, apply_return);
	}

	/* Copy the return value into position */
	if(return_value != 0 && type != jit_type_void)
	{
		jit_apply_builder_get_return(0, return_value, type,
					     apply_return);
	}
}

/*@
 * @deftypefun int jit_raw_supported (jit_type_t @var{signature})
 * Determine if @code{jit_apply_raw} can be used to call functions
 * with a particular signature.  Returns zero if not.
 * @end deftypefun
@*/
int jit_raw_supported(jit_type_t signature)
{
#if JIT_APPLY_NUM_WORD_REGS == 0 && JIT_APPLY_NUM_FLOAT_REGS == 0 && \
	JIT_APPLY_STRUCT_RETURN_SPECIAL_REG == 0

	unsigned int param;
	jit_type_t type;

#if JIT_APPLY_X86_FASTCALL != 0
	/* Cannot use raw calls with fastcall functions */
	if(jit_type_get_abi(signature) == jit_abi_fastcall)
	{
		return 0;
	}
#endif

	/* Check that all of the arguments are word-sized */
	param = jit_type_num_params(signature);
	while(param > 0)
	{
		--param;
		type = jit_type_normalize(jit_type_get_param(signature, param));
		if(type->kind < JIT_TYPE_SBYTE || type->kind > JIT_TYPE_NUINT)
		{
			return 0;
		}
	}

	/* Check that the return value does not involve structures */
	type = jit_type_get_return(signature);
	if(jit_type_is_struct(type) || jit_type_is_union(type))
	{
		return 0;
	}

	/* The signature is suitable for use with "jit_apply_raw" */
	return 1;

#else
	/* We cannot use raw calls if we need to use registers in applys */
	return 0;
#endif
}

/*
 * Define the structure of a vararg list for closures.
 */
struct jit_closure_va_list
{
	jit_apply_builder builder;
};

#ifdef jit_closure_size

/*
 * Define the closure structure.
 */
typedef struct jit_closure *jit_closure_t;
struct jit_closure
{
	unsigned char 		buf[jit_closure_size];
	jit_type_t    		signature;
	jit_closure_func	func;
	void		       *user_data;
};

/*
 * Handler that is called when a closure is invoked.
 */
static void closure_handler(jit_closure_t closure, void *apply_args)
{
	jit_type_t signature = closure->signature;
	jit_type_t type;
	jit_apply_builder parser;
	void *return_buffer;
	void **args;
	void *temp_arg;
	unsigned int num_params;
	unsigned int param;
	jit_apply_return apply_return;
	_jit_apply_return_type return_type;

	/* Initialize the argument parser */
	jit_apply_parser_init(&parser, closure->signature, apply_args);

	/* Allocate space for the return value */
	type = jit_type_normalize(jit_type_get_return(signature));
	if(!type || type == jit_type_void)
	{
		return_buffer = 0;
	}
	else if(jit_type_return_via_pointer(type))
	{
		jit_apply_parser_get_struct_return(&parser, return_buffer);
	}
	else
	{
		return_buffer = alloca(jit_type_get_size(type));
	}

	/* Allocate space for the argument buffer.  We allow for one
	   extra argument to hold the "va" list */
	num_params = jit_type_num_params(signature);
	args = (void **)alloca((num_params + 1) * sizeof(void *));

	/* Extract the fixed arguments */
	for(param = 0; param < num_params; ++param)
	{
		type = jit_type_normalize(jit_type_get_param(signature, param));
		if(!type)
		{
			args[param] = 0;
			continue;
		}
		temp_arg = alloca(jit_type_get_size(type));
		args[param] = temp_arg;
		switch(type->kind)
		{
			case JIT_TYPE_SBYTE:
			{
				jit_apply_parser_get_sbyte
					(&parser, *((jit_sbyte *)temp_arg));
			}
			break;

			case JIT_TYPE_UBYTE:
			{
				jit_apply_parser_get_ubyte
					(&parser, *((jit_ubyte *)temp_arg));
			}
			break;

			case JIT_TYPE_SHORT:
			{
				jit_apply_parser_get_short
					(&parser, *((jit_short *)temp_arg));
			}
			break;

			case JIT_TYPE_USHORT:
			{
				jit_apply_parser_get_ushort
					(&parser, *((jit_ushort *)temp_arg));
			}
			break;

			case JIT_TYPE_INT:
			{
				jit_apply_parser_get_int
					(&parser, *((jit_int *)temp_arg));
			}
			break;

			case JIT_TYPE_UINT:
			{
				jit_apply_parser_get_uint
					(&parser, *((jit_uint *)temp_arg));
			}
			break;

			case JIT_TYPE_LONG:
			{
				jit_apply_parser_get_long
					(&parser, *((jit_long *)temp_arg));
			}
			break;

			case JIT_TYPE_ULONG:
			{
				jit_apply_parser_get_ulong
					(&parser, *((jit_ulong *)temp_arg));
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				jit_apply_parser_get_float32
					(&parser, *((jit_float32 *)temp_arg));
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				jit_apply_parser_get_float64
					(&parser, *((jit_float64 *)temp_arg));
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				jit_apply_parser_get_nfloat
					(&parser, *((jit_nfloat *)temp_arg));
			}
			break;

			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
#ifdef HAVE_JIT_BUILTIN_APPLY_STRUCT
				_jit_builtin_apply_get_struct(&parser, temp_arg, type);
#else
				jit_apply_parser_get_struct(&parser, jit_type_get_size(type),
							    jit_type_get_alignment(type), temp_arg);
#endif
			}
			break;
		}
	}

	/* Adjust the argument parser for the start of the va arguments */
	jit_apply_parser_start_varargs(&parser);

	/* Record the address of the va handler in the last argument slot.
	   Not all functions will need this, but it doesn't hurt to include it */
	args[num_params] = &parser;

	/* Call the user's closure handling function */
	(*(closure->func))(signature, return_buffer, args, closure->user_data);

	/* Set up the "apply return" buffer */
	jit_memzero(&apply_return, sizeof(apply_return));
	type = jit_type_normalize(jit_type_get_return(signature));
	return_type = _JIT_APPLY_RETURN_TYPE_OTHER;
	if(type)
	{
		switch(type->kind)
		{
			case JIT_TYPE_SBYTE:
			{
				jit_apply_return_set_sbyte
					(&apply_return, *((jit_sbyte *)return_buffer));
			}
			break;

			case JIT_TYPE_UBYTE:
			{
				jit_apply_return_set_ubyte
					(&apply_return, *((jit_ubyte *)return_buffer));
			}
			break;

			case JIT_TYPE_SHORT:
			{
				jit_apply_return_set_short
					(&apply_return, *((jit_short *)return_buffer));
			}
			break;

			case JIT_TYPE_USHORT:
			{
				jit_apply_return_set_ushort
					(&apply_return, *((jit_ushort *)return_buffer));
			}
			break;

			case JIT_TYPE_INT:
			{
				jit_apply_return_set_int
					(&apply_return, *((jit_int *)return_buffer));
			}
			break;

			case JIT_TYPE_UINT:
			{
				jit_apply_return_set_uint
					(&apply_return, *((jit_uint *)return_buffer));
			}
			break;

			case JIT_TYPE_LONG:
			{
				jit_apply_return_set_long
					(&apply_return, *((jit_long *)return_buffer));
			}
			break;

			case JIT_TYPE_ULONG:
			{
				jit_apply_return_set_ulong
					(&apply_return, *((jit_ulong *)return_buffer));
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				jit_apply_return_set_float32
					(&apply_return, *((jit_float32 *)return_buffer));
				return_type = _JIT_APPLY_RETURN_TYPE_FLOAT32;
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				jit_apply_return_set_float64
					(&apply_return, *((jit_float64 *)return_buffer));
				return_type = _JIT_APPLY_RETURN_TYPE_FLOAT64;
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				jit_apply_return_set_nfloat
					(&apply_return, *((jit_nfloat *)return_buffer));
				return_type = _JIT_APPLY_RETURN_TYPE_NFLOAT;
			}
			break;

			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				if(!jit_type_return_via_pointer(type))
				{
					jit_memcpy(&apply_return, return_buffer,
							   jit_type_get_size(type));
				}
			}
			break;
		}
	}

	/* Return the result to the caller */
	switch(return_type)
	{
		case _JIT_APPLY_RETURN_TYPE_FLOAT32:
		{
			jit_builtin_return_float(&apply_return);
		}
		break;

		case _JIT_APPLY_RETURN_TYPE_FLOAT64:
		{
			jit_builtin_return_double(&apply_return);
		}
		break;

		case _JIT_APPLY_RETURN_TYPE_NFLOAT:
		{
			jit_builtin_return_nfloat(&apply_return);
		}
		break;

		default:
		{
			jit_builtin_return_int(&apply_return);
		}
	}
}

#endif /* jit_closure_size */

/*@
 * @deftypefun {void *} jit_closure_create (jit_context_t @var{context}, jit_type_t @var{signature}, jit_closure_func @var{func}, void *@var{user_data})
 * Create a closure from a function signature, a closure handling function,
 * and a user data value.  Returns NULL if out of memory, or if closures are
 * not supported.  The @var{func} argument should have the following
 * prototype:
 *
 * @example
 * void func(jit_type_t signature, void *result, void **args, void *user_data);
 * @end example
 *
 * If the closure signature includes variable arguments, then @code{args}
 * will contain pointers to the fixed arguments, followed by a
 * @code{jit_closure_va_list_t} value for accessing the remainder of
 * the arguments.
 *
 * The memory for the closure will be reclaimed when the @var{context}
 * is destroyed.
 * @end deftypefun
@*/
void *
jit_closure_create(jit_context_t context, jit_type_t signature, jit_closure_func func, void *user_data)
{
#ifdef jit_closure_size
	jit_closure_t closure;

	/* Validate the parameters */
	if(!context || !signature || !func)
	{
		return 0;
	}

	/* Acquire the memory context */
	_jit_memory_lock(context);
	if(!_jit_memory_ensure(context))
	{
		_jit_memory_unlock(context);
		return 0;
	}

	/* Allocate memory space for the closure */
	closure = (jit_closure_t) _jit_memory_alloc_closure(context);
	if(!closure)
	{
		_jit_memory_unlock(context);
		return 0;
	}

	/* Fill in the closure fields */
	_jit_create_closure(closure->buf, (void *)closure_handler, closure, signature);
	closure->signature = signature;
	closure->func = func;
	closure->user_data = user_data;

	/* Release the memory context, as we are finished with it */
	_jit_memory_unlock(context);

	/* Perform a cache flush on the closure's code */
	_jit_flush_exec(closure->buf, sizeof(closure->buf));

	/* Return the completed closure to the caller */
	return closure;

#else
	/* Closures are not supported on this platform */
	return 0;
#endif
}

/*@
 * @deftypefun int jit_supports_closures (void)
 * Determine if this platform has support for closures.
 * @end deftypefun
@*/
int
jit_supports_closures(void)
{
#ifdef jit_closure_size
	return 1;
#else
	return 0;
#endif
}

unsigned int
jit_get_closure_size(void)
{
#ifdef jit_closure_size
	return jit_closure_size;
#else
	return 0;
#endif
}

unsigned int
jit_get_closure_alignment(void)
{
#ifdef jit_closure_size
	return jit_closure_align;
#else
	return 0;
#endif
}

unsigned int
jit_get_trampoline_size(void)
{
	int size = 0;
#if defined(jit_redirector_size)
	size += jit_redirector_size;
#endif
#if defined(jit_indirector_size)
	size += jit_indirector_size;
#endif
	return size;
}

unsigned int
jit_get_trampoline_alignment(void)
{
#if defined(jit_redirector_size) || defined(jit_indirector_size)
	return 1;
#else
	return 0;
#endif
}

/*@
 * @deftypefun jit_nint jit_closure_va_get_nint (jit_closure_va_list_t @var{va})
 * @deftypefunx jit_nuint jit_closure_va_get_nuint (jit_closure_va_list_t @var{va})
 * @deftypefunx jit_long jit_closure_va_get_long (jit_closure_va_list_t @var{va})
 * @deftypefunx jit_ulong jit_closure_va_get_ulong (jit_closure_va_list_t @var{va})
 * @deftypefunx jit_float32 jit_closure_va_get_float32 (jit_closure_va_list_t @var{va})
 * @deftypefunx jit_float64 jit_closure_va_get_float64 (jit_closure_va_list_t @var{va})
 * @deftypefunx jit_nfloat jit_closure_va_get_nfloat (jit_closure_va_list_t @var{va})
 * @deftypefunx {void *} jit_closure_va_get_ptr (jit_closure_va_list_t @var{va})
 * Get the next value of a specific type from a closure's variable arguments.
 * @end deftypefun
@*/
jit_nint jit_closure_va_get_nint(jit_closure_va_list_t va)
{
	jit_nint value;
	jit_apply_parser_get_nint(&(va->builder), value);
	return value;
}

jit_nuint jit_closure_va_get_nuint(jit_closure_va_list_t va)
{
	jit_nuint value;
	jit_apply_parser_get_nuint(&(va->builder), value);
	return value;
}

jit_long jit_closure_va_get_long(jit_closure_va_list_t va)
{
	jit_long value;
	jit_apply_parser_get_long(&(va->builder), value);
	return value;
}

jit_ulong jit_closure_va_get_ulong(jit_closure_va_list_t va)
{
	jit_ulong value;
	jit_apply_parser_get_ulong(&(va->builder), value);
	return value;
}

jit_float32 jit_closure_va_get_float32(jit_closure_va_list_t va)
{
	jit_float32 value;
	jit_apply_parser_get_float32(&(va->builder), value);
	return value;
}

jit_float64 jit_closure_va_get_float64(jit_closure_va_list_t va)
{
	jit_float64 value;
	jit_apply_parser_get_float64(&(va->builder), value);
	return value;
}

jit_nfloat jit_closure_va_get_nfloat(jit_closure_va_list_t va)
{
	jit_nfloat value;
	jit_apply_parser_get_nfloat(&(va->builder), value);
	return value;
}

void *jit_closure_va_get_ptr(jit_closure_va_list_t va)
{
	jit_nint value;
	jit_apply_parser_get_nint(&(va->builder), value);
	return (void *)value;
}

/*@
 * @deftypefun void jit_closure_va_get_struct (jit_closure_va_list_t @var{va}, void *@var{buf}, jit_type_t @var{type})
 * Get a structure or union value of a specific @var{type} from a closure's
 * variable arguments, and copy it into @var{buf}.
 * @end deftypefun
@*/
void
jit_closure_va_get_struct(jit_closure_va_list_t va, void *buf, jit_type_t type)
{
#ifdef HAVE_JIT_BUILTIN_APPLY_STRUCT
	_jit_builtin_apply_get_struct(&(va->builder), buf, type);
#else
	jit_apply_parser_get_struct
		(&(va->builder), jit_type_get_size(type),
		 jit_type_get_alignment(type), buf);
#endif
}
