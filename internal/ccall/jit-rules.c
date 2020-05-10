/*
 * jit-rules.c - Rules that define the characteristics of the back-end.
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

/*
 * The information blocks for all registers in the system.
 */
jit_reginfo_t const _jit_reg_info[JIT_NUM_REGS] = {JIT_REG_INFO};

#ifdef JIT_CDECL_WORD_REG_PARAMS

/*
 * List of registers to use for simple parameter passing.
 */
static int const cdecl_word_regs[] = JIT_CDECL_WORD_REG_PARAMS;
#ifdef JIT_FASTCALL_WORD_REG_PARAMS
static int const fastcall_word_regs[] = JIT_FASTCALL_WORD_REG_PARAMS;
#endif

/*
 * Structure that is used to help with parameter passing.
 */
typedef struct
{
	jit_nint		offset;
	unsigned int	index;
	unsigned int	max_regs;
	const int	   *word_regs;
	jit_value_t		word_values[JIT_MAX_WORD_REG_PARAMS];

} jit_param_passing_t;

/*
 * Round a size up to a multiple of the stack word size.
 */
#define	ROUND_STACK(size)	\
		(((size) + (sizeof(void *) - 1)) & ~(sizeof(void *) - 1))
#define	STACK_WORDS(size)	\
		(((size) + (sizeof(void *) - 1)) / sizeof(void *))

/*
 * Allocate a word register or incoming frame position to a value.
 */
static int alloc_incoming_word
	(jit_function_t func, jit_param_passing_t *passing,
	 jit_value_t value, int extra_offset)
{
	int reg;
	reg = passing->word_regs[passing->index];
	if(reg != -1 && passing->word_values[passing->index] != 0)
	{
		/* The value was already forced out previously, so just copy it */
		if(!jit_insn_store(func, value, passing->word_values[passing->index]))
		{
			return 0;
		}
		++(passing->index);
	}
	else if(reg != -1)
	{
		if(!jit_insn_incoming_reg(func, value, reg))
		{
			return 0;
		}
		++(passing->index);
	}
	else
	{
		if(!jit_insn_incoming_frame_posn
				(func, value, passing->offset + extra_offset))
		{
			return 0;
		}
		passing->offset += sizeof(void *);
	}
	return 1;
}

/*
 * Force the remaining word registers out into temporary values,
 * to protect them from being accidentally overwritten by the code
 * that deals with multi-word parameters.
 */
static int force_remaining_out
	(jit_function_t func, jit_param_passing_t *passing)
{
	unsigned int index = passing->index;
	jit_value_t value;
	while(index < passing->max_regs && passing->word_regs[index] != -1)
	{
		if(passing->word_values[index] != 0)
		{
			/* We've already done this before */
			return 1;
		}
		value = jit_value_create(func, jit_type_void_ptr);
		if(!value)
		{
			return 0;
		}
		if(!jit_insn_incoming_reg(func, value, passing->word_regs[index]))
		{
			return 0;
		}
		passing->word_values[index] = value;
		++index;
	}
	return 1;
}

int _jit_create_entry_insns(jit_function_t func)
{
	jit_type_t signature = func->signature;
	jit_type_t type;
	jit_value_t value;
	jit_value_t temp;
	jit_value_t addr_of;
	unsigned int num_params;
	unsigned int param;
	unsigned int size;
	jit_param_passing_t passing;
	jit_nint partial_offset;

	/* Reset the local variable frame size for this function */
	func->builder->frame_size = JIT_INITIAL_FRAME_SIZE;

	/* Initialize the parameter passing information block */
	passing.offset = JIT_INITIAL_STACK_OFFSET;
	passing.index = 0;
#ifdef JIT_FASTCALL_WORD_REG_PARAMS
	if(jit_type_get_abi(signature) == jit_abi_fastcall)
	{
		passing.word_regs = fastcall_word_regs;
	}
	else
#endif
	{
		passing.word_regs = cdecl_word_regs;
	}
	for(size = 0; size < JIT_MAX_WORD_REG_PARAMS; ++size)
	{
		passing.word_values[size] = 0;
	}

	/* If the function is nested, then we need an extra parameter
	   to pass the pointer to the parent's local variable frame */
	if(func->nested_parent)
	{
		value = jit_value_create(func, jit_type_void_ptr);
		if(!value)
		{
			return 0;
		}

		jit_function_set_parent_frame(func, value);
		if(!alloc_incoming_word(func, &passing, value, 0))
		{
			return 0;
		}
	}

	/* Allocate the structure return pointer */
	value = jit_value_get_struct_pointer(func);
	if(value)
	{
		if(!alloc_incoming_word(func, &passing, value, 0))
		{
			return 0;
		}
	}

	/* Determine the maximum number of registers that may be needed
	   to pass the function's parameters */
	num_params = jit_type_num_params(signature);
	passing.max_regs = passing.index;
	for(param = 0; param < num_params; ++param)
	{
		value = jit_value_get_param(func, param);
		if(value)
		{
			size = STACK_WORDS(jit_type_get_size(jit_value_get_type(value)));
			passing.max_regs += size;
		}
	}

	/* Allocate the parameter offsets */
	for(param = 0; param < num_params; ++param)
	{
		value = jit_value_get_param(func, param);
		if(!value)
		{
			continue;
		}
		type = jit_type_remove_tags(jit_value_get_type(value));
		switch(type->kind)
		{
			case JIT_TYPE_SBYTE:
			case JIT_TYPE_UBYTE:
			{
				if(!alloc_incoming_word
					(func, &passing, value, _jit_nint_lowest_byte()))
				{
					return 0;
				}
			}
			break;

			case JIT_TYPE_SHORT:
			case JIT_TYPE_USHORT:
			{
				if(!alloc_incoming_word
					(func, &passing, value, _jit_nint_lowest_short()))
				{
					return 0;
				}
			}
			break;

			case JIT_TYPE_INT:
			case JIT_TYPE_UINT:
			{
				if(!alloc_incoming_word
					(func, &passing, value, _jit_nint_lowest_int()))
				{
					return 0;
				}
			}
			break;

			case JIT_TYPE_NINT:
			case JIT_TYPE_NUINT:
			case JIT_TYPE_SIGNATURE:
			case JIT_TYPE_PTR:
			{
				if(!alloc_incoming_word(func, &passing, value, 0))
				{
					return 0;
				}
			}
			break;

			case JIT_TYPE_LONG:
			case JIT_TYPE_ULONG:
		#ifdef JIT_NATIVE_INT64
			{
				if(!alloc_incoming_word(func, &passing, value, 0))
				{
					return 0;
				}
			}
			break;
		#endif
			/* Fall through on 32-bit platforms */

			case JIT_TYPE_FLOAT32:
			case JIT_TYPE_FLOAT64:
			case JIT_TYPE_NFLOAT:
			case JIT_TYPE_STRUCT:
			case JIT_TYPE_UNION:
			{
				/* Force the remaining registers out into temporary copies */
				if(!force_remaining_out(func, &passing))
				{
					return 0;
				}

				/* Determine how many words that we need to copy */
				size = STACK_WORDS(jit_type_get_size(type));

				/* If there are no registers left, then alloc on the stack */
				if(passing.word_regs[passing.index] == -1)
				{
					if(!jit_insn_incoming_frame_posn
						(func, value, passing.offset))
					{
						return 0;
					}
					passing.offset += size * sizeof(void *);
					break;
				}

				/* Copy the register components across */
				partial_offset = 0;
				addr_of = jit_insn_address_of(func, value);
				if(!addr_of)
				{
					return 0;
				}
				while(size > 0 && passing.word_regs[passing.index] != -1)
				{
					temp = passing.word_values[passing.index];
					++(passing.index);
					if(!jit_insn_store_relative
							(func, addr_of, partial_offset, temp))
					{
						return 0;
					}
					partial_offset += sizeof(void *);
					--size;
				}

				/* Copy the stack components across */
				while(size > 0)
				{
					temp = jit_value_create(func, jit_type_void_ptr);
					if(!temp)
					{
						return 0;
					}
					if(!jit_insn_incoming_frame_posn
							(func, temp, passing.offset))
					{
						return 0;
					}
					if(!jit_insn_store_relative
							(func, addr_of, partial_offset, temp))
					{
						return 0;
					}
					passing.offset += sizeof(void *);
					partial_offset += sizeof(void *);
					--size;
				}
			}
			break;
		}
	}
	return 1;
}

/*
 * Record that we need an outgoing register or stack slot for a word value.
 */
static void need_outgoing_word(jit_param_passing_t *passing)
{
	if(passing->word_regs[passing->index] != -1)
	{
		++(passing->index);
	}
	else
	{
		passing->offset += sizeof(void *);
	}
}

/*
 * Record that we need an outgoing register, containing a particular value.
 */
static void need_outgoing_value
	(jit_param_passing_t *passing, jit_value_t value)
{
	passing->word_values[passing->index] = value;
	++(passing->index);
}

/*
 * Count the number of registers that are left for parameter passing.
 */
static jit_nint count_regs_left(jit_param_passing_t *passing)
{
	int left = 0;
	unsigned int index = passing->index;
	while(passing->word_regs[index] != -1)
	{
		++left;
		++index;
	}
	return left;
}

/*
 * Determine if a type corresponds to a structure or union.
 */
static int is_struct_or_union(jit_type_t type)
{
	type = jit_type_normalize(type);
	if(type)
	{
		if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION)
		{
			return 1;
		}
	}
	return 0;
}

/*
 * Push a parameter onto the stack.
 */
static int push_param
	(jit_function_t func, jit_param_passing_t *passing,
	 jit_value_t value, jit_type_t type)
{
	jit_nint size = (jit_nint)(jit_type_get_size(value->type));
	passing->offset -= ROUND_STACK(size);
	if(is_struct_or_union(type) && !is_struct_or_union(value->type))
	{
	#ifdef JIT_USE_PARAM_AREA
		/* Copy the value into the outgoing parameter area, by pointer */
		if(!jit_insn_set_param_ptr(func, value, type, passing->offset))
		{
			return 0;
		}
	#else
		/* Push the parameter value onto the stack, by pointer */
		if(!jit_insn_push_ptr(func, value, type))
		{
			return 0;
		}
	#endif
	}
	else
	{
	#ifdef JIT_USE_PARAM_AREA
		/* Copy the value into the outgoing parameter area */
		if(!jit_insn_set_param(func, value, passing->offset))
		{
			return 0;
		}
	#else
		/* Push the parameter value onto the stack */
		if(!jit_insn_push(func, value))
		{
			return 0;
		}
	#endif
	}
	return 1;
}

/*
 * Allocate an outgoing word register to a value.
 */
static int alloc_outgoing_word
	(jit_function_t func, jit_param_passing_t *passing, jit_value_t value)
{
	int reg;
	--(passing->index);
	reg = passing->word_regs[passing->index];
	if(passing->word_values[passing->index] != 0)
	{
		/* We copied the value previously, so use the copy instead */
		value = passing->word_values[passing->index];
	}
	if(!jit_insn_outgoing_reg(func, value, reg))
	{
		return 0;
	}
	return 1;
}

int _jit_create_call_setup_insns
	(jit_function_t func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args,
	 int is_nested, jit_value_t parent_frame, jit_value_t *struct_return, int flags)
{
	jit_type_t type;
	jit_value_t value;
	jit_nint size;
	jit_nint regs_left;
	jit_nint rounded_size;
	jit_nint partial_offset;
	jit_param_passing_t passing;
	jit_value_t return_ptr;
	jit_value_t partial;
	unsigned int param;

	/* Initialize the parameter passing information block */
	passing.offset = 0;
	passing.index = 0;
#ifdef JIT_FASTCALL_WORD_REG_PARAMS
	if(jit_type_get_abi(signature) == jit_abi_fastcall)
	{
		passing.word_regs = fastcall_word_regs;
	}
	else
#endif
	{
		passing.word_regs = cdecl_word_regs;
	}
	for(size = 0; size < JIT_MAX_WORD_REG_PARAMS; ++size)
	{
		passing.word_values[size] = 0;
	}

	/* Determine how many parameters are going to end up in word registers,
	   and compute the largest stack size needed to pass stack parameters */
	if(is_nested)
	{
		need_outgoing_word(&passing);
	}
	type = jit_type_get_return(signature);
	if(jit_type_return_via_pointer(type))
	{
		value = jit_value_create(func, type);
		if(!value)
		{
			return 0;
		}
		*struct_return = value;
		return_ptr = jit_insn_address_of(func, value);
		if(!return_ptr)
		{
			return 0;
		}
		need_outgoing_word(&passing);
	}
	else
	{
		*struct_return = 0;
		return_ptr = 0;
	}
	partial = 0;
	for(param = 0; param < num_args; ++param)
	{
		type = jit_type_get_param(signature, param);
		size = STACK_WORDS(jit_type_get_size(type));
		if(size <= 1)
		{
			/* Allocate a word register or stack position */
			need_outgoing_word(&passing);
		}
		else
		{
			regs_left = count_regs_left(&passing);
			if(regs_left > 0)
			{
				/* May be partly in registers and partly on the stack */
				if(is_struct_or_union(type) &&
				   !is_struct_or_union(jit_value_get_type(args[param])))
				{
					/* Passing a structure by pointer */
					partial = args[param];
				}
				else if(jit_value_is_constant(args[param]))
				{
					if(size <= regs_left)
					{
						/* We can split the constant, without a temporary */
						partial_offset = 0;
						while(size > 0)
						{
							value = jit_value_create_nint_constant
								(func, jit_type_void_ptr,
								 *((jit_nint *)(args[param]->address +
								 				partial_offset)));
							need_outgoing_value(&passing, value);
							partial_offset += sizeof(void *);
							--size;
						}
						continue;
					}
					else
					{
						/* Copy the constant into a temporary local variable */
						partial = jit_value_create(func, type);
						if(!partial)
						{
							return 0;
						}
						if(!jit_insn_store(func, partial, args[param]))
						{
							return 0;
						}
						partial = jit_insn_address_of(func, partial);
					}
				}
				else
				{
					/* Get the address of this parameter */
					partial = jit_insn_address_of(func, args[param]);
				}
				if(!partial)
				{
					return 0;
				}
				partial_offset = 0;
				while(size > 0 && regs_left > 0)
				{
					value = jit_insn_load_relative
						(func, partial, partial_offset, jit_type_void_ptr);
					if(!value)
					{
						return 0;
					}
					need_outgoing_value(&passing, value);
					--size;
					--regs_left;
					partial_offset += sizeof(void *);
				}
				passing.offset += size * sizeof(void *);
			}
			else
			{
				/* Pass this parameter completely on the stack */
				passing.offset += size * sizeof(void *);
			}
		}
	}
#ifdef JIT_USE_PARAM_AREA
	if(passing.offset > func->builder->param_area_size)
	{
		func->builder->param_area_size = passing.offset;
	}
#else
	/* Flush deferred stack pops from previous calls if too many
	   parameters have collected up on the stack since last time */
	if(!jit_insn_flush_defer_pop(func, 32 - passing.offset))
	{
		return 0;
	}
#endif

	/* Move all of the parameters into their final locations */
	param = num_args;
	while(param > 0)
	{
		--param;
		type = jit_type_get_param(signature, param);
		size = (jit_nint)(jit_type_get_size(type));
		rounded_size = ROUND_STACK(size);
		size = STACK_WORDS(size);
		if(rounded_size <= passing.offset)
		{
			/* This parameter is completely on the stack */
			if(!push_param(func, &passing, args[param], type))
			{
				return 0;
			}
		}
		else if(passing.offset > 0)
		{
			/* This parameter is split between the stack and registers */
			while(passing.offset > 0)
			{
				rounded_size -= sizeof(void *);
				value = jit_insn_load_relative
					(func, partial, rounded_size, jit_type_void_ptr);
				if(!value)
				{
					return 0;
				}
				if(!push_param(func, &passing, value, jit_type_void_ptr))
				{
					return 0;
				}
				--size;
			}
			while(size > 0)
			{
				if(!alloc_outgoing_word(func, &passing, 0))
				{
					return 0;
				}
				--size;
			}
		}
		else
		{
			/* This parameter is completely in registers.  If the parameter
			   occupies multiple registers, then it has already been split */
			while(size > 0)
			{
				if(!alloc_outgoing_word(func, &passing, args[param]))
				{
					return 0;
				}
				--size;
			}
		}
	}

	/* Add nested scope information if required */
	if(is_nested)
	{
		if(passing.index > 0)
		{
			if(!alloc_outgoing_word(func, &passing, parent_frame))
			{
				return 0;
			}
		}
		else
		{
			if(!push_param
				(func, &passing, parent_frame, jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* Add the structure return pointer if required */
	if(return_ptr)
	{
		if(passing.index > 0)
		{
			if(!alloc_outgoing_word(func, &passing, return_ptr))
			{
				return 0;
			}
		}
		else
		{
			if(!push_param
				(func, &passing, return_ptr, jit_type_void_ptr))
			{
				return 0;
			}
		}
	}

	/* The call is ready to proceed */
	return 1;
}

#endif /* JIT_CDECL_WORD_REG_PARAMS */

void
_jit_gen_check_space(jit_gencode_t gen, int space)
{
	if((gen->ptr + space) >= gen->mem_limit)
	{
		/* No space left on the current cache page. */
		jit_exception_builtin(JIT_RESULT_MEMORY_FULL);
	}
}

void *
_jit_gen_alloc(jit_gencode_t gen, unsigned long size)
{
	void *ptr;
	_jit_memory_set_break(gen->context, gen->ptr);
	ptr = _jit_memory_alloc_data(gen->context, size, JIT_BEST_ALIGNMENT);
	if(!ptr)
	{
		jit_exception_builtin(JIT_RESULT_MEMORY_FULL);
	}
	gen->mem_limit = _jit_memory_get_limit(gen->context);
	return ptr;
}

int _jit_int_lowest_byte(void)
{
	union
	{
		unsigned char bytes[4];
		jit_int value;
	} volatile un;
	int posn;
	un.value = (jit_int)0x01020304;
	posn = 0;
	while(un.bytes[posn] != 0x04)
	{
		++posn;
	}
	return posn;
}

int _jit_int_lowest_short(void)
{
	union
	{
		unsigned char bytes[4];
		jit_int value;
	} volatile un;
	int posn;
	un.value = (jit_int)0x01020304;
	posn = 0;
	while(un.bytes[posn] != 0x03 && un.bytes[posn] != 0x04)
	{
		++posn;
	}
	return posn;
}

int _jit_nint_lowest_byte(void)
{
#ifdef JIT_NATIVE_INT32
	return _jit_int_lowest_byte();
#else
	union
	{
		unsigned char bytes[8];
		jit_long value;
	} volatile un;
	int posn;
	un.value = (jit_long)0x0102030405060708;
	posn = 0;
	while(un.bytes[posn] != 0x08)
	{
		++posn;
	}
	return posn;
#endif
}

int _jit_nint_lowest_short(void)
{
#ifdef JIT_NATIVE_INT32
	return _jit_int_lowest_short();
#else
	union
	{
		unsigned char bytes[8];
		jit_long value;
	} volatile un;
	int posn;
	un.value = (jit_long)0x0102030405060708;
	posn = 0;
	while(un.bytes[posn] != 0x07 && un.bytes[posn] != 0x08)
	{
		++posn;
	}
	return posn;
#endif
}

int _jit_nint_lowest_int(void)
{
#ifdef JIT_NATIVE_INT32
	return 0;
#else
	union
	{
		unsigned char bytes[8];
		jit_long value;
	} volatile un;
	int posn;
	un.value = (jit_long)0x0102030405060708;
	posn = 0;
	while(un.bytes[posn] <= 0x04)
	{
		++posn;
	}
	return posn;
#endif
}
