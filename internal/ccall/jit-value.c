/*
 * jit-value.c - Functions for manipulating temporary values.
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

/*@

@cindex jit-value.h

Values form the backbone of the storage system in @code{libjit}.
Every value in the system, be it a constant, a local variable,
or a temporary result, is represented by an object of type
@code{jit_value_t}.  The JIT then allocates registers or memory
locations to the values as appropriate.

We will demonstrate how to use values with a simple example of
adding two local variables together and putting the result into a
third local variable.  First, we allocate storage for the
three local variables:

@example
value1 = jit_value_create(func, jit_type_int);
value2 = jit_value_create(func, jit_type_int);
value3 = jit_value_create(func, jit_type_int);
@end example

Here, @code{func} is the function that we are building.  To add
@code{value1} and @code{value2} and put the result into @code{value3},
we use the following code:

@example
temp = jit_insn_add(func, value1, value2);
jit_insn_store(func, value3, temp);
@end example

The @code{jit_insn_add} function allocates a temporary value
(@code{temp}) and places the result of the addition into it.
The @code{jit_insn_store} function then stores the temporary
result into @code{value3}.

You might be tempted to think that the above code is inefficient.
Why do we copy the result into a temporary variable first?
Why not put the result directly to @code{value3}?

Behind the scenes, the JIT will typically optimize @code{temp} away,
resulting in the final code that you expect (i.e. @code{value3 = value1 +
value2}).  It is simply easier to use @code{libjit} if all results
end up in temporary variables first, so that's what we do.

Using temporary values, it is very easy to convert stack machine
bytecodes into JIT instructions.  Consider the following Java
Virtual Machine bytecode (representing @code{value4 = value1 * value2 +
value3}):

@example
iload 1
iload 2
imul
iload 3
iadd
istore 4
@end example

Let us demonstrate how this code would be translated, instruction
by instruction.  We assume that we have a @code{stack} available,
which keeps track of the temporary values in the system.  We also
assume that @code{jit_value_t} objects representing the local variables
are already stored in an array called @code{locals}.

@noindent
First, we load local variable 1 onto the stack:

@example
stack[size++] = jit_insn_load(func, locals[1]);
@end example

@noindent
We repeat this for local variable 2:

@example
stack[size++] = jit_insn_load(func, locals[2]);
@end example

@noindent
Now we pop these two values and push their multiplication:

@example
stack[size - 2] = jit_insn_mul(func, stack[size - 2], stack[size - 1]);
--size;
@end example

@noindent
Next, we need to push the value of local variable 3 and add it
to the product that we just computed:

@example
stack[size++] = jit_insn_load(func, locals[3]);
stack[size - 2] = jit_insn_add(func, stack[size - 2], stack[size - 1]);
--size;
@end example

@noindent
Finally, we store the result into local variable 4:

@example
jit_insn_store(func, locals[4], stack[--size]);
@end example

@noindent
Collecting up all of the above code, we get the following:

@example
stack[size++] = jit_insn_load(func, locals[1]);
stack[size++] = jit_insn_load(func, locals[2]);
stack[size - 2] = jit_insn_mul(func, stack[size - 2], stack[size - 1]);
--size;
stack[size++] = jit_insn_load(func, locals[3]);
stack[size - 2] = jit_insn_add(func, stack[size - 2], stack[size - 1]);
--size;
jit_insn_store(func, locals[4], stack[--size]);
@end example

The JIT will optimize away most of these temporary results, leaving
the final machine code that you expect.

If the virtual machine was register-based, then a slightly different
translation strategy would be used.  Consider the following code,
which computes @code{reg4 = reg1 * reg2 + reg3}, with the intermediate
result stored temporarily in @code{reg5}:

@example
mul reg5, reg1, reg2
add reg4, reg5, reg3
@end example

You would start by allocating value objects for all of the registers
in your system (with @code{jit_value_create}):

@example
reg[1] = jit_value_create(func, jit_type_int);
reg[2] = jit_value_create(func, jit_type_int);
reg[3] = jit_value_create(func, jit_type_int);
reg[4] = jit_value_create(func, jit_type_int);
reg[5] = jit_value_create(func, jit_type_int);
@end example

@noindent
Then, the virtual register machine code is translated as follows:

@example
temp1 = jit_insn_mul(func, reg[1], reg[2]);
jit_insn_store(reg[5], temp1);
temp2 = jit_insn_add(func, reg[5], reg[3]);
jit_insn_store(reg[4], temp2);
@end example

Each virtual register machine instruction turns into two @code{libjit}
function calls.  The JIT will normally optimize away the temporary
results.  If the value in @code{reg5} is not used further down the code,
then the JIT may also be able to optimize @code{reg5} away.

The rest of this section describes the functions that are available
to create and manipulate values.

@*/

/*
 * Allocate a new value from a function's memory pool.
 */
static jit_value_t
alloc_value(jit_function_t func, jit_type_t type)
{
	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	jit_value_t value = jit_memory_pool_alloc(&func->builder->value_pool, struct _jit_value);
	if(!value)
	{
		return 0;
	}
	value->block = func->builder->current_block;
	value->type = jit_type_copy(type);
	value->reg = -1;
	value->frame_offset = JIT_INVALID_FRAME_OFFSET;
	value->index = -1;

	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create (jit_function_t @var{func}, jit_type_t @var{type})
 * Create a new value in the context of a function's current block.
 * The value initially starts off as a block-specific temporary.
 * It will be converted into a function-wide local variable if
 * it is ever referenced from a different block.  Returns NULL
 * if out of memory.
 *
 * Note: It isn't possible to refer to global variables directly using
 * values.  If you need to access a global variable, then load its
 * address into a temporary and use @code{jit_insn_load_relative}
 * or @code{jit_insn_store_relative} to manipulate it.  It simplifies
 * the JIT if it can assume that all values are local.
 * @end deftypefun
@*/
jit_value_t
jit_value_create(jit_function_t func, jit_type_t type)
{
	jit_value_t value = alloc_value(func, type);
	if(!value)
	{
		return 0;
	}
	value->is_temporary = 1;
	if(jit_type_has_tag(type, JIT_TYPETAG_VOLATILE))
	{
		value->is_volatile = 1;
	}
	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create_nint_constant (jit_function_t @var{func}, jit_type_t @var{type}, jit_nint @var{const_value})
 * Create a new native integer constant in the specified function.
 * Returns NULL if out of memory.
 *
 * The @var{type} parameter indicates the actual type of the constant,
 * if it happens to be something other than @code{jit_type_nint}.
 * For example, the following will create an unsigned byte constant:
 *
 * @example
 * value = jit_value_create_nint_constant(func, jit_type_ubyte, 128);
 * @end example
 *
 * This function can be used to create constants of type @code{jit_type_sbyte},
 * @code{jit_type_ubyte}, @code{jit_type_short}, @code{jit_type_ushort},
 * @code{jit_type_int}, @code{jit_type_uint}, @code{jit_type_nint},
 * @code{jit_type_nuint}, and all pointer types.
 * @end deftypefun
@*/
jit_value_t
jit_value_create_nint_constant(jit_function_t func, jit_type_t type, jit_nint const_value)
{
	jit_type_t stripped = 0;
	if(!const_value)
	{
		/* Special cases: see if this is the NULL or zero constant */
		stripped = jit_type_remove_tags(type);
		if(stripped->kind == JIT_TYPE_SIGNATURE
		   || stripped->kind == JIT_TYPE_PTR
		   || stripped->kind == JIT_TYPE_NINT)
		{
			if(func && func->builder && func->builder->null_constant)
			{
				return func->builder->null_constant;
			}
		}
		else if(stripped->kind == JIT_TYPE_INT)
		{
			if(func && func->builder && func->builder->zero_constant)
			{
				return func->builder->zero_constant;
			}
		}
	}

	jit_value_t value = alloc_value(func, type);
	if(!value)
	{
		return 0;
	}
	value->is_constant = 1;
	value->is_nint_constant = 1;
	value->address = const_value;

	if(stripped)
	{
		/* Special cases: see if we need to cache this constant for later */
		if(stripped->kind == JIT_TYPE_SIGNATURE
		   || stripped->kind == JIT_TYPE_PTR
		   || stripped->kind == JIT_TYPE_NINT)
		{
			func->builder->null_constant = value;
		}
		else if(stripped->kind == JIT_TYPE_INT)
		{
			func->builder->zero_constant = value;
		}
	}

	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create_long_constant (jit_function_t @var{func}, jit_type_t @var{type}, jit_long @var{const_value})
 * Create a new 64-bit integer constant in the specified
 * function.  This can also be used to create constants of
 * type @code{jit_type_ulong}.  Returns NULL if out of memory.
 * @end deftypefun
@*/
jit_value_t
jit_value_create_long_constant(jit_function_t func, jit_type_t type, jit_long const_value)
{
	jit_value_t value = alloc_value(func, type);
	if(!value)
	{
		return 0;
	}
	value->is_constant = 1;
#ifdef JIT_NATIVE_INT64
	value->is_nint_constant = 1;
	value->address = (jit_nint) const_value;
#else
	value->address = (jit_nint) jit_malloc(sizeof(jit_long));
	if(!value->address)
	{
		return 0;
	}
	*((jit_long *) value->address) = const_value;
	value->free_address = 1;
#endif
	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create_float32_constant (jit_function_t @var{func}, jit_type_t @var{type}, jit_float32 @var{const_value})
 * Create a new 32-bit floating-point constant in the specified
 * function.  Returns NULL if out of memory.
 * @end deftypefun
@*/
jit_value_t
jit_value_create_float32_constant(jit_function_t func, jit_type_t type, jit_float32 const_value)
{
	jit_value_t value = alloc_value(func, type);
	if(!value)
	{
		return 0;
	}
	value->is_constant = 1;
	value->address = (jit_nint) jit_malloc(sizeof(jit_float32));
	if(!value->address)
	{
		return 0;
	}
	*((jit_float32 *) value->address) = const_value;
	value->free_address = 1;
	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create_float64_constant (jit_function_t @var{func}, jit_type_t @var{type}, jit_float64 @var{const_value})
 * Create a new 64-bit floating-point constant in the specified
 * function.  Returns NULL if out of memory.
 * @end deftypefun
@*/
jit_value_t
jit_value_create_float64_constant(jit_function_t func, jit_type_t type, jit_float64 const_value)
{
	jit_value_t value = alloc_value(func, type);
	if(!value)
	{
		return 0;
	}
	value->is_constant = 1;
	value->address = (jit_nint) jit_malloc(sizeof(jit_float64));
	if(!value->address)
	{
		return 0;
	}
	*((jit_float64 *) value->address) = const_value;
	value->free_address = 1;
	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create_nfloat_constant (jit_function_t @var{func}, jit_type_t @var{type}, jit_nfloat @var{const_value})
 * Create a new native floating-point constant in the specified
 * function.  Returns NULL if out of memory.
 * @end deftypefun
@*/
jit_value_t
jit_value_create_nfloat_constant(jit_function_t func, jit_type_t type, jit_nfloat const_value)
{
	jit_value_t value = alloc_value(func, type);
	if(!value)
	{
		return 0;
	}
	value->is_constant = 1;
	value->address = (jit_nint) jit_malloc(sizeof(jit_nfloat));
	if(!value->address)
	{
		return 0;
	}
	*((jit_nfloat *) value->address) = const_value;
	value->free_address = 1;
	return value;
}

/*@
 * @deftypefun jit_value_t jit_value_create_constant (jit_function_t @var{func}, const jit_constant_t *@var{const_value})
 * Create a new constant from a generic constant structure in the specified
 * function.  Returns NULL if out of memory or if the type in
 * @var{const_value} is not suitable for a constant.
 * @end deftypefun
@*/
jit_value_t
jit_value_create_constant(jit_function_t func, const jit_constant_t *const_value)
{
	jit_type_t stripped = jit_type_remove_tags(const_value->type);
	if(!stripped)
	{
		return 0;
	}
	switch(stripped->kind)
	{
	case JIT_TYPE_SBYTE:
	case JIT_TYPE_UBYTE:
	case JIT_TYPE_SHORT:
	case JIT_TYPE_USHORT:
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		return jit_value_create_nint_constant(func, const_value->type,
						      const_value->un.int_value);

	case JIT_TYPE_NINT:
	case JIT_TYPE_NUINT:
	case JIT_TYPE_PTR:
	case JIT_TYPE_SIGNATURE:
		return jit_value_create_nint_constant(func, const_value->type,
						      const_value->un.nint_value);

	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		return jit_value_create_long_constant(func, const_value->type,
						      const_value->un.long_value);

	case JIT_TYPE_FLOAT32:
		return jit_value_create_float32_constant(func, const_value->type,
							 const_value->un.float32_value);

	case JIT_TYPE_FLOAT64:
		return jit_value_create_float64_constant(func, const_value->type,
							 const_value->un.float64_value);

	case JIT_TYPE_NFLOAT:
		return jit_value_create_nfloat_constant(func, const_value->type,
							const_value->un.nfloat_value);
	}
	return 0;
}

/*@
 * @deftypefun jit_value_t jit_value_get_param (jit_function_t @var{func}, unsigned int @var{param})
 * Get the value that corresponds to a specified function parameter.
 * Returns NULL if out of memory or @var{param} is invalid.
 * @end deftypefun
@*/
jit_value_t
jit_value_get_param(jit_function_t func, unsigned int param)
{
	unsigned int num_params, current;

	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	/* Ensure valid param number. */
	jit_type_t signature = func->signature;
	num_params = jit_type_num_params(signature);
	if(param >= num_params)
	{
		return 0;
	}

	/* If we have already created the values, then exit immediately */
	jit_value_t *values = func->builder->param_values;
	if(values)
	{
		return values[param];
	}

	/* Create the values for the first time */
	values = (jit_value_t *) jit_calloc(num_params, sizeof(jit_value_t));
	if(!values)
	{
		return 0;
	}
	func->builder->param_values = values;
	for(current = 0; current < num_params; ++current)
	{
		jit_type_t type = jit_type_get_param(signature, current);
		values[current] = jit_value_create(func, type);
		if(values[current])
		{
			/* The value belongs to the entry block, no matter
			   where it happens to be created */
			values[current]->block = func->builder->entry_block;
			values[current]->is_parameter = 1;
		}
	}

	/* Return the value block for the desired parameter */
	return values[param];
}

/*@
 * @deftypefun jit_value_t jit_value_get_struct_pointer (jit_function_t @var{func})
 * Get the value that contains the structure return pointer for
 * a function.  If the function does not have a structure return pointer
 * (i.e. structures are returned in registers), then this returns NULL.
 * @end deftypefun
@*/
jit_value_t
jit_value_get_struct_pointer(jit_function_t func)
{
	jit_type_t type;
	jit_value_t value;

	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return 0;
	}

	type = jit_type_remove_tags(jit_type_get_return(func->signature));
	if(jit_type_is_struct(type) || jit_type_is_union(type))
	{
		if(jit_type_return_via_pointer(type))
		{
			if(!func->builder->struct_return)
			{
				type = jit_type_create_pointer(type, 1);
				if(!type)
				{
					return 0;
				}
				value = jit_value_create(func, type);
				func->builder->struct_return = value;
				if(value)
				{
					/* The value belongs to the entry block, no matter
					   where it happens to be created */
					value->block = func->builder->entry_block;
					value->is_parameter = 1;
				}
				jit_type_free(type);
			}
			return func->builder->struct_return;
		}
	}
	return 0;
}

/*@
 * @deftypefun int jit_value_is_temporary (jit_value_t @var{value})
 * Determine if a value is temporary.  i.e. its scope extends
 * over a single block within its function.
 * @end deftypefun
@*/
int
jit_value_is_temporary(jit_value_t value)
{
	return value->is_temporary;
}

/*@
 * @deftypefun int jit_value_is_local (jit_value_t @var{value})
 * Determine if a value is local.  i.e. its scope extends
 * over multiple blocks within its function.
 * @end deftypefun
@*/
int
jit_value_is_local(jit_value_t value)
{
	return value->is_local;
}

/*@
 * @deftypefun int jit_value_is_constant (jit_value_t @var{value})
 * Determine if a value is a constant.
 * @end deftypefun
@*/
int
jit_value_is_constant(jit_value_t value)
{
	return value->is_constant;
}

/*@
 * @deftypefun int jit_value_is_parameter (jit_value_t @var{value})
 * Determine if a value is a function parameter.
 * @end deftypefun
@*/
int
jit_value_is_parameter(jit_value_t value)
{
	return value->is_parameter;
}

/*@
 * @deftypefun void jit_value_ref (jit_function_t @var{func}, jit_value_t @var{value})
 * Create a reference to the specified @var{value} from the current
 * block in @var{func}.  This will convert a temporary value into
 * a local value if @var{value} is being referenced from a different
 * block than its original.
 *
 * It is not necessary that @var{func} be the same function as the
 * one where the value was originally created.  It may be a nested
 * function, referring to a local variable in its parent function.
 * @end deftypefun
@*/
void
jit_value_ref(jit_function_t func, jit_value_t value)
{
	/* Ensure that we have a builder for this function */
	if(!_jit_function_ensure_builder(func))
	{
		return;
	}

	value->usage_count++;
	if(value->is_temporary)
	{
		if(value->block->func != func)
		{
			/* Reference from a different function: local and addressable */
			value->is_temporary = 0;
			value->is_local = 1;
			value->is_addressable = 1;

			/* Mark the two functions as not leaves because we will need
			   them to set up proper frame pointers to allow us to access
			   the local variable across the nested function boundary */
			value->block->func->builder->non_leaf = 1;
			func->builder->non_leaf = 1;
		}
		else if(value->block != func->builder->current_block)
		{
			/* Reference from another block in same function: local */
			value->is_temporary = 0;
			value->is_local = 1;
			if(_jit_gen_is_global_candidate(value->type))
			{
				value->global_candidate = 1;
			}
		}
	}
	else if(value->is_local && value->block->func != func)
	{
		/* Convert a previously local value into an addressable one */
		value->is_addressable = 1;
		value->block->func->builder->non_leaf = 1;
		func->builder->non_leaf = 1;
	}
}

/* TODO: seems to be unused */
void _jit_value_ref_params(jit_function_t func)
{
	unsigned int num_params;
	unsigned int param;
	if(func->builder->param_values)
	{
		num_params = jit_type_num_params(func->signature);
		for(param = 0; param < num_params; ++param)
		{
			jit_value_ref(func, func->builder->param_values[param]);
		}
	}
	jit_value_ref(func, func->builder->struct_return);
	jit_value_ref(func, func->builder->parent_frame);
}

/*@
 * @deftypefun void jit_value_set_volatile (jit_value_t @var{value})
 * Set a flag on a value to indicate that it is volatile.  The contents
 * of the value must always be reloaded from memory, never from a
 * cached register copy.
 * @end deftypefun
@*/
void
jit_value_set_volatile(jit_value_t value)
{
	value->is_volatile = 1;
}

/*@
 * @deftypefun int jit_value_is_volatile (jit_value_t @var{value})
 * Determine if a value is volatile.
 * @end deftypefun
@*/
int
jit_value_is_volatile(jit_value_t value)
{
	return value->is_volatile;
}

/*@
 * @deftypefun void jit_value_set_addressable (jit_value_t @var{value})
 * Set a flag on a value to indicate that it is addressable.
 * This should be used when you want to take the address of a
 * value (e.g. @code{&variable} in C).  The value is guaranteed
 * to not be stored in a register across a function call.
 * If you refer to a value from a nested function (@code{jit_value_ref}),
 * then the value will be automatically marked as addressable.
 * @end deftypefun
@*/
void
jit_value_set_addressable(jit_value_t value)
{
	value->is_addressable = 1;
}

/*@
 * @deftypefun int jit_value_is_addressable (jit_value_t @var{value})
 * Determine if a value is addressable.
 * @end deftypefun
@*/
int
jit_value_is_addressable(jit_value_t value)
{
	return value->is_addressable;
}

/*@
 * @deftypefun jit_type_t jit_value_get_type (jit_value_t @var{value})
 * Get the type that is associated with a value.
 * @end deftypefun
@*/
jit_type_t
jit_value_get_type(jit_value_t value)
{
	return value->type;
}

/*@
 * @deftypefun jit_function_t jit_value_get_function (jit_value_t @var{value})
 * Get the function which owns a particular @var{value}.
 * @end deftypefun
@*/
jit_function_t
jit_value_get_function(jit_value_t value)
{
	return value->block->func;
}

/*@
 * @deftypefun jit_block_t jit_value_get_block (jit_value_t @var{value})
 * Get the block which owns a particular @var{value}.
 * @end deftypefun
@*/
jit_block_t
jit_value_get_block(jit_value_t value)
{
	return value->block;
}

/*@
 * @deftypefun jit_context_t jit_value_get_context (jit_value_t @var{value})
 * Get the context which owns a particular @var{value}.
 * @end deftypefun
@*/
jit_context_t
jit_value_get_context(jit_value_t value)
{
	return value->block->func->context;
}

/*@
 * @deftypefun jit_constant_t jit_value_get_constant (jit_value_t @var{value})
 * Get the constant value within a particular @var{value}.  The returned
 * structure's @code{type} field will be @code{jit_type_void} if
 * @code{value} is not a constant.
 * @end deftypefun
@*/
jit_constant_t
jit_value_get_constant(jit_value_t value)
{
	jit_constant_t result;
	if(!value->is_constant)
	{
		result.type = jit_type_void;
		return result;
	}
	result.type = value->type;
	switch(jit_type_remove_tags(value->type)->kind)
	{
	case JIT_TYPE_SBYTE:
	case JIT_TYPE_UBYTE:
	case JIT_TYPE_SHORT:
	case JIT_TYPE_USHORT:
	case JIT_TYPE_INT:
	case JIT_TYPE_UINT:
		result.un.int_value = (jit_int) value->address;
		break;

	case JIT_TYPE_NINT:
	case JIT_TYPE_NUINT:
	case JIT_TYPE_PTR:
	case JIT_TYPE_SIGNATURE:
		result.un.nint_value = value->address;
		break;

	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
#ifdef JIT_NATIVE_INT64
		result.un.long_value = (jit_long) value->address;
#else
		result.un.long_value = *((jit_long *) value->address);
#endif
		break;

	case JIT_TYPE_FLOAT32:
		result.un.float32_value = *((jit_float32 *) value->address);
		break;

	case JIT_TYPE_FLOAT64:
		result.un.float64_value = *((jit_float64 *) value->address);
		break;

	case JIT_TYPE_NFLOAT:
		result.un.nfloat_value = *((jit_nfloat *) value->address);
		break;

	default:
		result.type = jit_type_void;
		break;
	}
	return result;
}

/*@
 * @deftypefun jit_nint jit_value_get_nint_constant (jit_value_t @var{value})
 * Get the constant value within a particular @var{value}, assuming
 * that its type is compatible with @code{jit_type_nint}.
 * @end deftypefun
@*/
jit_nint
jit_value_get_nint_constant(jit_value_t value)
{
	if(!value->is_nint_constant)
	{
		return 0;
	}
	return (jit_nint) value->address;
}

/*@
 * @deftypefun jit_nint jit_value_get_long_constant (jit_value_t @var{value})
 * Get the constant value within a particular @var{value}, assuming
 * that its type is compatible with @code{jit_type_long}.
 * @end deftypefun
@*/
jit_long
jit_value_get_long_constant(jit_value_t value)
{
	if(!value->is_constant)
	{
		return 0;
	}
	switch(jit_type_normalize(value->type)->kind)
	{
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
#ifdef JIT_NATIVE_INT64
		return (jit_long) value->address;
#else
		return *((jit_long *) value->address);
#endif
	}
	return 0;
}

/*@
 * @deftypefun jit_float32 jit_value_get_float32_constant (jit_value_t @var{value})
 * Get the constant value within a particular @var{value}, assuming
 * that its type is compatible with @code{jit_type_float32}.
 * @end deftypefun
@*/
jit_float32
jit_value_get_float32_constant(jit_value_t value)
{
	if(!value->is_constant || jit_type_normalize(value->type)->kind != JIT_TYPE_FLOAT32)
	{
		return (jit_float32) 0.0;
	}
	return *((jit_float32 *) value->address);
}

/*@
 * @deftypefun jit_float64 jit_value_get_float64_constant (jit_value_t @var{value})
 * Get the constant value within a particular @var{value}, assuming
 * that its type is compatible with @code{jit_type_float64}.
 * @end deftypefun
@*/
jit_float64
jit_value_get_float64_constant(jit_value_t value)
{
	if(!value->is_constant || jit_type_normalize(value->type)->kind != JIT_TYPE_FLOAT64)
	{
		return (jit_float64) 0.0;
	}
	return *((jit_float64 *) value->address);
}

/*@
 * @deftypefun jit_nfloat jit_value_get_nfloat_constant (jit_value_t @var{value})
 * Get the constant value within a particular @var{value}, assuming
 * that its type is compatible with @code{jit_type_nfloat}.
 * @end deftypefun
@*/
jit_nfloat
jit_value_get_nfloat_constant(jit_value_t value)
{
	if(!value->is_constant || jit_type_normalize(value->type)->kind != JIT_TYPE_NFLOAT)
	{
		return (jit_nfloat) 0.0;
	}
	return *((jit_nfloat *) value->address);
}

/*@
 * @deftypefun int jit_value_is_true (jit_value_t @var{value})
 * Determine if @var{value} is constant and non-zero.
 * @end deftypefun
@*/
int
jit_value_is_true(jit_value_t value)
{
	if(!value || !value->is_constant)
	{
		return 0;
	}
	if(value->is_nint_constant)
	{
		return (value->address != 0);
	}
	switch(jit_type_remove_tags(value->type)->kind)
	{
	case JIT_TYPE_LONG:
	case JIT_TYPE_ULONG:
		return (jit_value_get_long_constant(value) != 0);

	case JIT_TYPE_FLOAT32:
		return (jit_value_get_float32_constant(value) != (jit_float32) 0.0);

	case JIT_TYPE_FLOAT64:
		return (jit_value_get_float64_constant(value) != (jit_float64) 0.0);

	case JIT_TYPE_NFLOAT:
		return (jit_value_get_nfloat_constant(value) != (jit_nfloat) 0.0);
	}
	return 0;
}

/*@
 * @deftypefun int jit_constant_convert (jit_constant_t *@var{result}, const jit_constant_t *@var{value}, jit_type_t @var{type}, int @var{overflow_check})
 * Convert a the constant @var{value} into a new @var{type}, and
 * return its value in @var{result}.  Returns zero if the conversion
 * is not possible, usually due to overflow.
 * @end deftypefun
@*/
int
jit_constant_convert(jit_constant_t *result, const jit_constant_t *value, jit_type_t type,
		     int overflow_check)
{
	jit_type_t srctype;
	jit_type_t desttype;

	/* Normalize the source and destination types.  The source type
	   is also promoted, to reduce the number of cases in the
	   inner switch statements below */
	srctype = jit_type_promote_int(jit_type_normalize(value->type));
	if(!srctype)
	{
		return 0;
	}
	desttype = jit_type_normalize(type);
	if(!desttype)
	{
		return 0;
	}

	/* Determine what kind of conversion to perform */
	result->type = type;
	switch(desttype->kind)
	{
	case JIT_TYPE_SBYTE:
		/* Convert to a signed 8-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			if(overflow_check)
			{
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 value->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_sbyte(value->un.int_value);
			}
			break;

		case JIT_TYPE_UINT:
			if(overflow_check)
			{
				if(!jit_uint_to_int_ovf(&result->un.int_value,
							value->un.uint_value))
				{
					return 0;
				}
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_sbyte(value->un.int_value);
			}
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_int_ovf(&result->un.int_value,
							value->un.long_value))
				{
					return 0;
				}
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_sbyte(jit_long_to_int(value->un.long_value));
			}
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_int_ovf(&result->un.int_value,
							 value->un.ulong_value))
				{
					return 0;
				}
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_sbyte(jit_ulong_to_int(value->un.ulong_value));
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_int_ovf(&result->un.int_value,
							   value->un.float32_value))
				{
					return 0;
				}
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_sbyte(jit_float32_to_int(value->un.float32_value));
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_int_ovf(&result->un.int_value,
							   value->un.float64_value))
				{
					return 0;
				}
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_sbyte(jit_float64_to_int(value->un.float64_value));
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_int_ovf(&result->un.int_value,
							  value->un.nfloat_value))
				{
					return 0;
				}
				if(!jit_int_to_sbyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_sbyte(jit_nfloat_to_int(value->un.nfloat_value));
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_UBYTE:
		/* Convert to an unsigned 8-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			if(overflow_check)
			{
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 value->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_ubyte(value->un.int_value);
			}
			break;

		case JIT_TYPE_UINT:
			if(overflow_check)
			{
				if(!jit_uint_to_int_ovf(&result->un.int_value,
							value->un.uint_value))
				{
					return 0;
				}
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_ubyte(value->un.int_value);
			}
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_int_ovf(&result->un.int_value,
							value->un.long_value))
				{
					return 0;
				}
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ubyte(jit_long_to_int(value->un.long_value));
			}
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_int_ovf(&result->un.int_value,
							 value->un.ulong_value))
				{
					return 0;
				}
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ubyte(jit_ulong_to_int(value->un.ulong_value));
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_int_ovf(&result->un.int_value,
							   value->un.float32_value))
				{
					return 0;
				}
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ubyte(jit_float32_to_int(value->un.float32_value));
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_int_ovf(&result->un.int_value,
							   value->un.float64_value))
				{
					return 0;
				}
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ubyte(jit_float64_to_int(value->un.float64_value));
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_int_ovf(&result->un.int_value,
							  value->un.nfloat_value))
				{
					return 0;
				}
				if(!jit_int_to_ubyte_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ubyte(jit_nfloat_to_int(value->un.nfloat_value));
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_SHORT:
		/* Convert to a signed 16-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			if(overflow_check)
			{
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 value->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_short(value->un.int_value);
			}
			break;

		case JIT_TYPE_UINT:
			if(overflow_check)
			{
				if(!jit_uint_to_int_ovf(&result->un.int_value,
							value->un.uint_value))
				{
					return 0;
				}
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_short(value->un.int_value);
			}
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_int_ovf(&result->un.int_value,
							value->un.long_value))
				{
					return 0;
				}
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_short(
						jit_long_to_int(value->un.long_value));
			}
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_int_ovf(&result->un.int_value,
							 value->un.ulong_value))
				{
					return 0;
				}
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_short(
						jit_ulong_to_int(value->un.ulong_value));
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_int_ovf(&result->un.int_value,
							   value->un.float32_value))
				{
					return 0;
				}
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_short(
						jit_float32_to_int(value->un.float32_value));
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_int_ovf(&result->un.int_value,
							   value->un.float64_value))
				{
					return 0;
				}
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_short(
						jit_float64_to_int(value->un.float64_value));
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_int_ovf(&result->un.int_value,
							  value->un.nfloat_value))
				{
					return 0;
				}
				if(!jit_int_to_short_ovf(&result->un.int_value,
							 result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_short(
						jit_nfloat_to_int(value->un.nfloat_value));
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_USHORT:
		/* Convert to an unsigned 16-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			if(overflow_check)
			{
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  value->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_ushort(value->un.int_value);
			}
			break;

		case JIT_TYPE_UINT:
			if(overflow_check)
			{
				if(!jit_uint_to_int_ovf(&result->un.int_value,
							value->un.uint_value))
				{
					return 0;
				}
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_int_to_ushort(value->un.int_value);
			}
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_int_ovf(&result->un.int_value,
							value->un.long_value))
				{
					return 0;
				}
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ushort(
						jit_long_to_int(value->un.long_value));
			}
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_int_ovf(&result->un.int_value,
							 value->un.ulong_value))
				{
					return 0;
				}
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ushort(
						jit_ulong_to_int(value->un.ulong_value));
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_int_ovf(&result->un.int_value,
							   value->un.float32_value))
				{
					return 0;
				}
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ushort(
						jit_float32_to_int(value->un.float32_value));
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_int_ovf(&result->un.int_value,
							   value->un.float64_value))
				{
					return 0;
				}
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ushort(
						jit_float64_to_int(value->un.float64_value));
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_int_ovf(&result->un.int_value,
							  value->un.nfloat_value))
				{
					return 0;
				}
				if(!jit_int_to_ushort_ovf(&result->un.int_value,
							  result->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value =
					jit_int_to_ushort(
						jit_nfloat_to_int(value->un.nfloat_value));
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_INT:
		/* Convert to a signed 32-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			result->un.int_value = value->un.int_value;
			break;

		case JIT_TYPE_UINT:
			if(overflow_check)
			{
				if(!jit_uint_to_int_ovf(&result->un.int_value,
							value->un.uint_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_uint_to_int(value->un.uint_value);
			}
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_int_ovf(&result->un.int_value,
							value->un.long_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_long_to_int(value->un.long_value);
			}
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_int_ovf(&result->un.int_value,
							 value->un.ulong_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_ulong_to_int(value->un.ulong_value);
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_int_ovf(&result->un.int_value,
							   value->un.float32_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_float32_to_int(value->un.float32_value);
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_int_ovf(&result->un.int_value,
							   value->un.float64_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_float64_to_int(value->un.float64_value);
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_int_ovf(&result->un.int_value,
							  value->un.nfloat_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.int_value = jit_nfloat_to_int(value->un.nfloat_value);
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_UINT:
		/* Convert to an unsigned 32-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			if(overflow_check)
			{
				if(!jit_int_to_uint_ovf(&result->un.uint_value,
							value->un.uint_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.uint_value = jit_int_to_uint(value->un.int_value);
			}
			break;

		case JIT_TYPE_UINT:
			result->un.uint_value = value->un.uint_value;
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_uint_ovf(&result->un.uint_value,
							 value->un.long_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.uint_value = jit_long_to_uint(value->un.long_value);
			}
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_uint_ovf(&result->un.uint_value,
							  value->un.ulong_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.uint_value = jit_ulong_to_uint(value->un.ulong_value);
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_uint_ovf(&result->un.uint_value,
							    value->un.float32_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.uint_value = jit_float32_to_uint(value->un.float32_value);
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_uint_ovf(&result->un.uint_value,
							    value->un.float64_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.uint_value = jit_float64_to_uint(value->un.float64_value);
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_uint_ovf(&result->un.uint_value,
							   value->un.nfloat_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.uint_value = jit_nfloat_to_uint(value->un.nfloat_value);
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_LONG:
		/* Convert to a signed 64-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			result->un.long_value =
				jit_int_to_long(value->un.int_value);
			break;

		case JIT_TYPE_UINT:
			result->un.long_value =
				jit_uint_to_long(value->un.int_value);
			break;

		case JIT_TYPE_LONG:
			result->un.long_value = value->un.long_value;
			break;

		case JIT_TYPE_ULONG:
			if(overflow_check)
			{
				if(!jit_ulong_to_long_ovf(&result->un.long_value,
							  value->un.ulong_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.long_value = jit_ulong_to_long(value->un.ulong_value);
			}
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_long_ovf(&result->un.long_value,
							    value->un.float32_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.long_value = jit_float32_to_long(value->un.float32_value);
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_long_ovf(&result->un.long_value,
							    value->un.float64_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.long_value = jit_float64_to_long(value->un.float64_value);
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_long_ovf(&result->un.long_value,
							   value->un.nfloat_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.long_value = jit_nfloat_to_long(value->un.nfloat_value);
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_ULONG:
		/* Convert to an unsigned 64-bit integer */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			if(overflow_check)
			{
				if(!jit_int_to_ulong_ovf(&result->un.ulong_value,
							 value->un.int_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.ulong_value = jit_int_to_ulong(value->un.int_value);
			}
			break;

		case JIT_TYPE_UINT:
			result->un.ulong_value =
				jit_uint_to_ulong(value->un.uint_value);
			break;

		case JIT_TYPE_LONG:
			if(overflow_check)
			{
				if(!jit_long_to_ulong_ovf(&result->un.ulong_value,
							  value->un.long_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.ulong_value = jit_long_to_ulong(value->un.long_value);
			}
			break;

		case JIT_TYPE_ULONG:
			result->un.ulong_value = value->un.ulong_value;
			break;

		case JIT_TYPE_FLOAT32:
			if(overflow_check)
			{
				if(!jit_float32_to_ulong_ovf(&result->un.ulong_value,
							     value->un.float32_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.ulong_value = jit_float32_to_ulong(value->un.float32_value);
			}
			break;

		case JIT_TYPE_FLOAT64:
			if(overflow_check)
			{
				if(!jit_float64_to_ulong_ovf(&result->un.ulong_value,
							     value->un.float64_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.ulong_value = jit_float64_to_ulong(value->un.float64_value);
			}
			break;

		case JIT_TYPE_NFLOAT:
			if(overflow_check)
			{
				if(!jit_nfloat_to_ulong_ovf(&result->un.ulong_value,
							    value->un.nfloat_value))
				{
					return 0;
				}
			}
			else
			{
				result->un.ulong_value = jit_nfloat_to_ulong(value->un.nfloat_value);
			}
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_FLOAT32:
		/* Convert to a 32-bit float */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			result->un.float32_value = jit_int_to_float32(value->un.int_value);
			break;

		case JIT_TYPE_UINT:
			result->un.float32_value = jit_uint_to_float32(value->un.uint_value);
			break;

		case JIT_TYPE_LONG:
			result->un.float32_value = jit_long_to_float32(value->un.long_value);
			break;

		case JIT_TYPE_ULONG:
			result->un.float32_value = jit_ulong_to_float32(value->un.ulong_value);
			break;

		case JIT_TYPE_FLOAT32:
			result->un.float32_value = value->un.float32_value;
			break;

		case JIT_TYPE_FLOAT64:
			result->un.float32_value = jit_float64_to_float32(value->un.float64_value);
			break;

		case JIT_TYPE_NFLOAT:
			result->un.float32_value = jit_nfloat_to_float32(value->un.nfloat_value);
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_FLOAT64:
		/* Convert to a 64-bit float */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			result->un.float64_value = jit_int_to_float64(value->un.int_value);
			break;

		case JIT_TYPE_UINT:
			result->un.float64_value = jit_uint_to_float64(value->un.uint_value);
			break;

		case JIT_TYPE_LONG:
			result->un.float64_value = jit_long_to_float64(value->un.long_value);
			break;

		case JIT_TYPE_ULONG:
			result->un.float64_value = jit_ulong_to_float64(value->un.ulong_value);
			break;

		case JIT_TYPE_FLOAT32:
			result->un.float64_value = jit_float32_to_float64(value->un.float32_value);
			break;

		case JIT_TYPE_FLOAT64:
			result->un.float64_value = value->un.float64_value;
			break;

		case JIT_TYPE_NFLOAT:
			result->un.float64_value = jit_nfloat_to_float64(value->un.nfloat_value);
			break;

		default:
			return 0;
		}
		break;

	case JIT_TYPE_NFLOAT:
		/* Convert to a native float */
		switch(srctype->kind)
		{
		case JIT_TYPE_INT:
			result->un.nfloat_value = jit_int_to_nfloat(value->un.int_value);
			break;

		case JIT_TYPE_UINT:
			result->un.nfloat_value = jit_uint_to_nfloat(value->un.uint_value);
			break;

		case JIT_TYPE_LONG:
			result->un.nfloat_value = jit_long_to_nfloat(value->un.long_value);
			break;

		case JIT_TYPE_ULONG:
			result->un.nfloat_value = jit_ulong_to_nfloat(value->un.ulong_value);
			break;

		case JIT_TYPE_FLOAT32:
			result->un.nfloat_value = jit_float32_to_nfloat(value->un.float32_value);
			break;

		case JIT_TYPE_FLOAT64:
			result->un.nfloat_value	= jit_float64_to_nfloat(value->un.float64_value);
			break;

		case JIT_TYPE_NFLOAT:
			result->un.nfloat_value = value->un.nfloat_value;
			break;

		default:
			return 0;
		}
		break;

	default:
		return 0;
	}
	return 1;
}

void
_jit_value_free(void *_value)
{
	jit_value_t value = (jit_value_t) _value;
	jit_type_free(value->type);
	if(value->free_address && value->address)
	{
		/* We need to free the memory for a large constant */
		jit_free((void *) value->address);
	}
}
