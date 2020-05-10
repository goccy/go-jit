/*
 * jit-apply-x86-64.c - Apply support routines for x86_64.
 *
 * Copyright (C) 2008  Southern Storm Software, Pty Ltd.
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

#if defined(__amd64) || defined(__amd64__) || defined(_x86_64) || defined(_x86_64__)

#include "jit-gen-x86-64.h"

/*
 * X86_64 argument types as specified in the X86_64 SysV ABI.
 */
#define X86_64_ARG_NO_CLASS		0x00
#define X86_64_ARG_INTEGER		0x01
#define X86_64_ARG_MEMORY		0x02
#define X86_64_ARG_SSE			0x11
#define X86_64_ARG_SSEUP		0x12
#define X86_64_ARG_X87			0x21
#define X86_64_ARG_X87UP		0x22

#define X86_64_ARG_IS_SSE(arg)	(((arg) & 0x10) != 0)
#define X86_64_ARG_IS_X87(arg)	(((arg) & 0x20) != 0)


void _jit_create_closure(unsigned char *buf, void *func,
                         void *closure, void *_type)
{
	jit_nint offset;

	/* Set up the local stack frame */
	x86_64_push_reg_size(buf, X86_64_RBP, 8);
	x86_64_mov_reg_reg_size(buf, X86_64_RBP, X86_64_RSP, 8);

	/* Create the apply argument block on the stack */
	x86_64_sub_reg_imm_size(buf, X86_64_RSP, 192, 8);

	/* fill the apply buffer */
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x08, X86_64_RDI, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x10, X86_64_RSI, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x18, X86_64_RDX, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x20, X86_64_RCX, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x28, X86_64_R8, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x30, X86_64_R9, 8);

	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x40, X86_64_XMM0);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x50, X86_64_XMM1);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x60, X86_64_XMM2);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x70, X86_64_XMM3);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x80, X86_64_XMM4);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x90, X86_64_XMM5);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0xA0, X86_64_XMM6);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0xB0, X86_64_XMM7);

	/* Now fill the arguments for the closure function */
	/* the closure function is #1 */
	x86_64_mov_reg_imm_size(buf, X86_64_RDI, (jit_nint)closure, 8);
	/* the apply buff is #2 */
	x86_64_mov_reg_reg_size(buf, X86_64_RSI, X86_64_RSP, 8);

	/* Call the closure handling function */
	offset = (jit_nint)func - ((jit_nint)buf + 5);
	if((offset < jit_min_int) || (offset > jit_max_int))
	{
		/* offset is outside the 32 bit offset range */
		/* so we have to do an indirect call */
		/* We use R11 here because it's the only temporary caller saved */
		/* register not used for argument passing. */
		x86_64_mov_reg_imm_size(buf, X86_64_R11, (jit_nint)func, 8);
		x86_64_call_reg(buf, X86_64_R11);
	}
	else
	{
		x86_64_call_imm(buf, (jit_int)offset);
	}

	/* Pop the current stack frame */
	x86_64_mov_reg_reg_size(buf, X86_64_RSP, X86_64_RBP, 8);
	x86_64_pop_reg_size(buf, X86_64_RBP, 8);

	/* Return from the closure */
	x86_64_ret(buf);
}

void *_jit_create_redirector(unsigned char *buf, void *func,
							 void *user_data, int abi)
{
	jit_nint offset;
	void *start = (void *)buf;

	/* Save all registers used for argument passing */
	/* At this point RSP is not aligned on a 16 byte boundary because */
	/* the return address is pushed on the stack. */
	/* We need (7 * 8) + (8 * 16) bytes for the registers */
	x86_64_sub_reg_imm_size(buf, X86_64_RSP, 0xB8, 8);

	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0xB0, X86_64_RAX, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0xA8, X86_64_RDI, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0xA0, X86_64_RSI, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x98, X86_64_RDX, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x90, X86_64_RCX, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x88, X86_64_R8, 8);
	x86_64_mov_membase_reg_size(buf, X86_64_RSP, 0x80, X86_64_R9, 8);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x70, X86_64_XMM0);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x60, X86_64_XMM1);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x50, X86_64_XMM2);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x40, X86_64_XMM3);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x30, X86_64_XMM4);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x20, X86_64_XMM5);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x10, X86_64_XMM6);
	x86_64_movaps_membase_reg(buf, X86_64_RSP, 0x00, X86_64_XMM7);

	/* Fill the pointer to the stack args */
	x86_64_lea_membase_size(buf, X86_64_RDI, X86_64_RSP, 0xD0, 8);
	x86_64_mov_regp_reg_size(buf, X86_64_RSP, X86_64_RDI, 8);

	/* Load the user data argument */
	x86_64_mov_reg_imm_size(buf, X86_64_RDI, (jit_nint)user_data, 8);

	/* Call "func" (the pointer result will be in RAX) */
	offset = (jit_nint)func - ((jit_nint)buf + 5);
	if((offset < jit_min_int) || (offset > jit_max_int))
	{
		/* offset is outside the 32 bit offset range */
		/* so we have to do an indirect call */
		/* We use R11 here because it's the only temporary caller saved */
		/* register not used for argument passing. */
		x86_64_mov_reg_imm_size(buf, X86_64_R11, (jit_nint)func, 8);
		x86_64_call_reg(buf, X86_64_R11);
	}
	else
	{
		x86_64_call_imm(buf, (jit_int)offset);
	}

	/* store the returned address in R11 */
	x86_64_mov_reg_reg_size(buf, X86_64_R11, X86_64_RAX, 8);

	/* Restore the argument registers */
	x86_64_mov_reg_membase_size(buf, X86_64_RAX, X86_64_RSP, 0xB0, 8);
	x86_64_mov_reg_membase_size(buf, X86_64_RDI, X86_64_RSP, 0xA8, 8);
	x86_64_mov_reg_membase_size(buf, X86_64_RSI, X86_64_RSP, 0xA0, 8);
	x86_64_mov_reg_membase_size(buf, X86_64_RDX, X86_64_RSP, 0x98, 8);
	x86_64_mov_reg_membase_size(buf, X86_64_RCX, X86_64_RSP, 0x90, 8);
	x86_64_mov_reg_membase_size(buf, X86_64_R8, X86_64_RSP, 0x88, 8);
	x86_64_mov_reg_membase_size(buf, X86_64_R9, X86_64_RSP, 0x80, 8);
	x86_64_movaps_reg_membase(buf, X86_64_XMM0, X86_64_RSP, 0x70);
	x86_64_movaps_reg_membase(buf, X86_64_XMM1, X86_64_RSP, 0x60);
	x86_64_movaps_reg_membase(buf, X86_64_XMM2, X86_64_RSP, 0x50);
	x86_64_movaps_reg_membase(buf, X86_64_XMM3, X86_64_RSP, 0x40);
	x86_64_movaps_reg_membase(buf, X86_64_XMM4, X86_64_RSP, 0x30);
	x86_64_movaps_reg_membase(buf, X86_64_XMM5, X86_64_RSP, 0x20);
	x86_64_movaps_reg_membase(buf, X86_64_XMM6, X86_64_RSP, 0x10);
	x86_64_movaps_reg_membase(buf, X86_64_XMM7, X86_64_RSP, 0x00);

	/* Restore the stack pointer */
	x86_64_add_reg_imm_size(buf, X86_64_RSP, 0xB8, 8);

	/* Jump to the function that the redirector indicated */
	x86_64_jmp_reg(buf, X86_64_R11);

	/* Return the start of the buffer as the redirector entry point */
	return start;
}

void *_jit_create_indirector(unsigned char *buf, void **entry)
{
	void *start = (void *)buf;

	/* Jump to the entry point. */
	if(((jit_nint)entry >= jit_min_int) && ((jit_nint)entry <= jit_max_int))
	{
		/* We are in the 32bit range so we can use the entry directly. */
		x86_64_jmp_mem(buf, (jit_nint)entry);
	}
	else
	{
		jit_nint offset = (jit_nint)entry - ((jit_nint)buf + 6);

		if((offset >= jit_min_int) && (offset <= jit_max_int))
		{
			/* We are in the 32bit range so we can use RIP relative addressing. */
			x86_64_jmp_membase(buf, X86_64_RIP, offset);
		}
		else
		{
			/* offset is outside the 32 bit offset range */
			/* so we have to do an indirect jump via register. */
			x86_64_mov_reg_imm_size(buf, X86_64_R11, (jit_nint)entry, 8);
			x86_64_jmp_regp(buf, X86_64_R11);
		}
	}

	return start;
}

void _jit_pad_buffer(unsigned char *buf, int len)
{
	while(len >= 6)
	{
		/* "leal 0(%esi), %esi" with 32-bit displacement */
		*buf++ = (unsigned char)0x8D;
		x86_address_byte(buf, 2, X86_ESI, X86_ESI);
		x86_imm_emit32(buf, 0);
		len -= 6;
	}
	if(len >= 3)
	{
		/* "leal 0(%esi), %esi" with 8-bit displacement */
		*buf++ = (unsigned char)0x8D;
		x86_address_byte(buf, 1, X86_ESI, X86_ESI);
		x86_imm_emit8(buf, 0);
		len -= 3;
	}
	if(len == 1)
	{
		/* Traditional x86 NOP */
		x86_nop(buf);
	}
	else if(len == 2)
	{
		/* movl %esi, %esi */
		x86_mov_reg_reg(buf, X86_ESI, X86_ESI, 4);
	}
}

/*
 * Allcate the slot for a parameter passed on the stack.
 */
static void
_jit_alloc_param_slot(jit_param_passing_t *passing, _jit_param_t *param,
					  jit_type_t type)
{
	jit_int size = jit_type_get_size(type);
	jit_int alignment = jit_type_get_alignment(type);

	/* Expand the size to a multiple of the stack slot size */
	size = ROUND_STACK(size);

	/* Expand the alignment to a multiple of the stack slot size */
	/* We expect the alignment to be a power of two after this step */
	alignment = ROUND_STACK(alignment);

	/* Make sure the current offset is aligned propperly for the type */
	if((passing->stack_size & (alignment -1)) != 0)
	{
		/* We need padding on the stack to fix the alignment constraint */
		jit_int padding = passing->stack_size & (alignment -1);

		/* Add the padding to the stack region */
		passing->stack_size += padding;

		/* record the number of pad words needed after pushing this arg */
		param->stack_pad = STACK_SLOTS_USED(padding);
	}
	/* Record the offset of the parameter in the arg region. */
	param->un.offset = passing->stack_size;

	/* And increase the argument region used. */
	passing->stack_size += size;
}

/*
 * Determine if a type corresponds to a structure or union.
 */
static int
is_struct_or_union(jit_type_t type)
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
 * Classify the argument type.
 * The type has to be in it's normalized form.
 */
static int
_jit_classify_arg(jit_type_t arg_type, int is_return)
{
	switch(arg_type->kind)
	{
		case JIT_TYPE_SBYTE:
		case JIT_TYPE_UBYTE:
		case JIT_TYPE_SHORT:
		case JIT_TYPE_USHORT:
		case JIT_TYPE_INT:
		case JIT_TYPE_UINT:
		case JIT_TYPE_NINT:
		case JIT_TYPE_NUINT:
		case JIT_TYPE_LONG:
		case JIT_TYPE_ULONG:
		case JIT_TYPE_SIGNATURE:
		case JIT_TYPE_PTR:
		{
			return X86_64_ARG_INTEGER;
		}
		break;

		case JIT_TYPE_FLOAT32:
		case JIT_TYPE_FLOAT64:
		{
			return X86_64_ARG_SSE;
		}
		break;

		case JIT_TYPE_NFLOAT:
		{
			/* we assume the nfloat type to be long double (80bit) */
			if(is_return)
			{
				return X86_64_ARG_X87;
			}
			else
			{
				return X86_64_ARG_MEMORY;
			}
		}
		break;

		case JIT_TYPE_STRUCT:
		case JIT_TYPE_UNION:
		{
			int size = jit_type_get_size(arg_type);

			if(size > 16)
			{
				return X86_64_ARG_MEMORY;
			}
			else if(size <= 8)
			{
				return X86_64_ARG_INTEGER;
			}
			/* For structs and unions with sizes between 8 ant 16 bytes */
			/* we have to look at the elements. */
			/* TODO */
		}
	}
	return X86_64_ARG_NO_CLASS;
}

/*
 * On X86_64 the alignment of native types matches their size.
 * This leads to the result that all types except nfloats and aggregates
 * (structs and unions) must start and end in an eightbyte (or the part
 * we are looking at).
 */
static int
_jit_classify_structpart(jit_type_t struct_type, unsigned int start,
						 unsigned int start_offset, unsigned int end_offset)
{
	int arg_class = X86_64_ARG_NO_CLASS;
	unsigned int num_fields = jit_type_num_fields(struct_type);
	unsigned int current_field;
	
	for(current_field = 0; current_field < num_fields; ++current_field)
	{
		jit_nuint field_offset = jit_type_get_offset(struct_type,
													 current_field);

		if(field_offset <= end_offset)
		{
			/* The field starts at a place that's inerresting for us */
			jit_type_t field_type = jit_type_get_field(struct_type,
													   current_field);
			jit_nuint field_size = jit_type_get_size(field_type); 

			if(field_offset + field_size > start_offset)
			{
				/* The field is at least partially in the part we are */
				/* looking at */
				int arg_class2 = X86_64_ARG_NO_CLASS;

				if(is_struct_or_union(field_type))
				{
					/* We have to check this struct recursively */
					unsigned int current_start;
					unsigned int nested_struct_start;
					unsigned int nested_struct_end;

					current_start = start + start_offset;
					if(field_offset < current_start)
					{
						nested_struct_start = current_start - field_offset;
					}
					else
					{
						nested_struct_start = 0;
					}
					if(field_offset + field_size - 1 > end_offset)
					{
						/* The struct ends beyond the part we are looking at */
						nested_struct_end = field_offset + field_size -
												(nested_struct_start + 1);
					}
					else
					{
						nested_struct_end = field_size - 1;
					}
					arg_class2 = _jit_classify_structpart(field_type,
														  start + field_offset,
														  nested_struct_start,
														  nested_struct_end);
				}
				else
				{
					if((start + start_offset) & (field_size - 1))
					{
						/* The field is misaligned */
						return X86_64_ARG_MEMORY;
					}
					arg_class2 = _jit_classify_arg(field_type, 0);
				}
				if(arg_class == X86_64_ARG_NO_CLASS)
				{
					arg_class = arg_class2;
				}
				else if(arg_class != arg_class2)
				{
					if(arg_class == X86_64_ARG_MEMORY ||
					   arg_class2 == X86_64_ARG_MEMORY)
					{
						arg_class = X86_64_ARG_MEMORY;
					}
					else if(arg_class == X86_64_ARG_INTEGER ||
					   arg_class2 == X86_64_ARG_INTEGER)
					{
						arg_class = X86_64_ARG_INTEGER;
					}
					else if(arg_class == X86_64_ARG_X87 ||
					   arg_class2 == X86_64_ARG_X87)
					{
						arg_class = X86_64_ARG_MEMORY;
					}
					else
					{
						arg_class = X86_64_ARG_SSE;
					}
				}
			}
		}
	}
	return arg_class;
}

int
_jit_classify_struct(jit_param_passing_t *passing,
					_jit_param_t *param, jit_type_t param_type)
{
	jit_nuint size = (jit_nuint)jit_type_get_size(param_type);

	if(size <= 8)
	{
		int arg_class;
	
		arg_class = _jit_classify_structpart(param_type, 0, 0, size - 1);
		if(arg_class == X86_64_ARG_NO_CLASS)
		{
			arg_class = X86_64_ARG_SSE;
		}
		if(arg_class == X86_64_ARG_INTEGER)
		{
			if(passing->word_index < passing->max_word_regs)
			{
				/* Set the arg class to the number of registers used */
				param->arg_class = 1;

				/* Set the first register to the register used */
				param->un.reg_info[0].reg = passing->word_regs[passing->word_index];
				param->un.reg_info[0].value = param->value;
				++(passing->word_index);
			}
			else
			{
				/* Set the arg class to stack */
				param->arg_class = JIT_ARG_CLASS_STACK;

				/* Allocate the slot in the arg passing frame */
				_jit_alloc_param_slot(passing, param, param_type);
			}			
		}
		else if(arg_class == X86_64_ARG_SSE)
		{
			if(passing->float_index < passing->max_float_regs)
			{
				/* Set the arg class to the number of registers used */
				param->arg_class = 1;

				/* Set the first register to the register used */
				param->un.reg_info[0].reg =	passing->float_regs[passing->float_index];
				param->un.reg_info[0].value = param->value;
				++(passing->float_index);
			}
			else
			{
				/* Set the arg class to stack */
				param->arg_class = JIT_ARG_CLASS_STACK;

				/* Allocate the slot in the arg passing frame */
				_jit_alloc_param_slot(passing, param, param_type);
			}
		}
		else
		{
			/* Set the arg class to stack */
			param->arg_class = JIT_ARG_CLASS_STACK;

			/* Allocate the slot in the arg passing frame */
			_jit_alloc_param_slot(passing, param, param_type);
		}
	}
	else if(size <= 16)
	{
		int arg_class1;
		int arg_class2;

		arg_class1 = _jit_classify_structpart(param_type, 0, 0, 7);
		arg_class2 = _jit_classify_structpart(param_type, 0, 8, size - 1);
		if(arg_class1 == X86_64_ARG_NO_CLASS)
		{
			arg_class1 = X86_64_ARG_SSE;
		}
		if(arg_class2 == X86_64_ARG_NO_CLASS)
		{
			arg_class2 = X86_64_ARG_SSE;
		}
		if(arg_class1 == X86_64_ARG_SSE && arg_class2 == X86_64_ARG_SSE)
		{
			/* We use only one sse register in this case */
			if(passing->float_index < passing->max_float_regs)
			{
				/* Set the arg class to the number of registers used */
				param->arg_class = 1;

				/* Set the first register to the register used */
				param->un.reg_info[0].reg =	passing->float_regs[passing->float_index];
				param->un.reg_info[0].value = param->value;
				++(passing->float_index);
			}
			else
			{
				/* Set the arg class to stack */
				param->arg_class = JIT_ARG_CLASS_STACK;

				/* Allocate the slot in the arg passing frame */
				_jit_alloc_param_slot(passing, param, param_type);
			}
		}
		else if(arg_class1 == X86_64_ARG_MEMORY ||
				arg_class2 == X86_64_ARG_MEMORY)
		{
			/* Set the arg class to stack */
			param->arg_class = JIT_ARG_CLASS_STACK;

			/* Allocate the slot in the arg passing frame */
			_jit_alloc_param_slot(passing, param, param_type);
		}
		else if(arg_class1 == X86_64_ARG_INTEGER &&
				arg_class2 == X86_64_ARG_INTEGER)
		{
			/* We need two general purpose registers in this case */
			if((passing->word_index + 1) < passing->max_word_regs)
			{
				/* Set the arg class to the number of registers used */
				param->arg_class = 2;

				/* Assign the registers */
				param->un.reg_info[0].reg = passing->word_regs[passing->word_index];
				++(passing->word_index);
				param->un.reg_info[1].reg = passing->word_regs[passing->word_index];
				++(passing->word_index);
			}
			else
			{
				/* Set the arg class to stack */
				param->arg_class = JIT_ARG_CLASS_STACK;

				/* Allocate the slot in the arg passing frame */
				_jit_alloc_param_slot(passing, param, param_type);
			}			
		}
		else
		{
			/* We need one xmm and one general purpose register */
			if((passing->word_index < passing->max_word_regs) &&
			   (passing->float_index < passing->max_float_regs))
			{
				/* Set the arg class to the number of registers used */
				param->arg_class = 2;

				if(arg_class1 == X86_64_ARG_INTEGER)
				{
					param->un.reg_info[0].reg = passing->word_regs[passing->word_index];
					++(passing->word_index);
					param->un.reg_info[1].reg =	passing->float_regs[passing->float_index];
					++(passing->float_index);
				}
				else
				{
					param->un.reg_info[0].reg =	passing->float_regs[passing->float_index];
					++(passing->float_index);
					param->un.reg_info[1].reg = passing->word_regs[passing->word_index];
					++(passing->word_index);
				}
			}
			else
			{
				/* Set the arg class to stack */
				param->arg_class = JIT_ARG_CLASS_STACK;

				/* Allocate the slot in the arg passing frame */
				_jit_alloc_param_slot(passing, param, param_type);
			}
		}
	}
	else
	{
		/* Set the arg class to stack */
		param->arg_class = JIT_ARG_CLASS_STACK;

		/* Allocate the slot in the arg passing frame */
		_jit_alloc_param_slot(passing, param, param_type);
	}
	return 1;
}

int
_jit_classify_param(jit_param_passing_t *passing,
					_jit_param_t *param, jit_type_t param_type)
{
	if(is_struct_or_union(param_type))
	{
		return _jit_classify_struct(passing, param, param_type);
	}
	else
	{
		int arg_class;

		arg_class = _jit_classify_arg(param_type, 0);

		switch(arg_class)
		{
			case X86_64_ARG_INTEGER:
			{
				if(passing->word_index < passing->max_word_regs)
				{
					/* Set the arg class to the number of registers used */
					param->arg_class = 1;

					/* Set the first register to the register used */
					param->un.reg_info[0].reg = passing->word_regs[passing->word_index];
					param->un.reg_info[0].value = param->value;
					++(passing->word_index);
				}
				else
				{
					/* Set the arg class to stack */
					param->arg_class = JIT_ARG_CLASS_STACK;

					/* Allocate the slot in the arg passing frame */
					_jit_alloc_param_slot(passing, param, param_type);
				}
			}
			break;

			case X86_64_ARG_SSE:
			{
				if(passing->float_index < passing->max_float_regs)
				{
					/* Set the arg class to the number of registers used */
					param->arg_class = 1;

					/* Set the first register to the register used */
					param->un.reg_info[0].reg =	passing->float_regs[passing->float_index];
					param->un.reg_info[0].value = param->value;
					++(passing->float_index);
				}
				else
				{
					/* Set the arg class to stack */
					param->arg_class = JIT_ARG_CLASS_STACK;

					/* Allocate the slot in the arg passing frame */
					_jit_alloc_param_slot(passing, param, param_type);
				}
			}
			break;

			case X86_64_ARG_MEMORY:
			{
				/* Set the arg class to stack */
				param->arg_class = JIT_ARG_CLASS_STACK;

				/* Allocate the slot in the arg passing frame */
				_jit_alloc_param_slot(passing, param, param_type);
			}
			break;
		}
	}
	return 1;
}

void
_jit_builtin_apply_add_struct(jit_apply_builder *builder,
							  void *value,
							  jit_type_t struct_type)
{
	unsigned int size = jit_type_get_size(struct_type);

	if(size <= 16)
	{
		if(size <= 8)
		{
			int arg_class;
	
			arg_class = _jit_classify_structpart(struct_type, 0, 0, size - 1);
			if(arg_class == X86_64_ARG_NO_CLASS)
			{
				arg_class = X86_64_ARG_SSE;
			}
			if((arg_class == X86_64_ARG_INTEGER) &&
			   (builder->word_used < JIT_APPLY_NUM_WORD_REGS))
			{
				/* The struct is passed in a general purpose register */
				jit_memcpy(&(builder->apply_args->word_regs[builder->word_used]),
												value, size);
				++(builder->word_used);
			}
			else if((arg_class == X86_64_ARG_SSE) &&
					(builder->float_used < JIT_APPLY_NUM_FLOAT_REGS))
			{
				/* The struct is passed in one sse register */
				jit_memcpy(&(builder->apply_args->float_regs[builder->float_used]),
												value, size);
				++(builder->float_used);
			}
			else
			{
				unsigned int align = jit_type_get_alignment(struct_type);

				jit_apply_builder_add_struct(builder, value, size, align);
			}
		}
		else
		{
			int arg_class1;
			int arg_class2;

			arg_class1 = _jit_classify_structpart(struct_type, 0, 0, 7);
			arg_class2 = _jit_classify_structpart(struct_type, 0, 8, size - 1);
			if(arg_class1 == X86_64_ARG_NO_CLASS)
			{
				arg_class1 = X86_64_ARG_SSE;
			}
			if(arg_class2 == X86_64_ARG_NO_CLASS)
			{
				arg_class2 = X86_64_ARG_SSE;
			}
			if(arg_class1 == X86_64_ARG_SSE && arg_class2 == X86_64_ARG_SSE &&
			   (builder->float_used < JIT_APPLY_NUM_FLOAT_REGS))
			{
				/* The struct is passed in one sse register */
				jit_memcpy(&(builder->apply_args->float_regs[builder->float_used]),
											value, size);
				++(builder->float_used);
			}
			else if(arg_class1 == X86_64_ARG_INTEGER &&
					arg_class2 == X86_64_ARG_INTEGER &&
					(builder->word_used < (JIT_APPLY_NUM_WORD_REGS + 1)))
			{
				/* The struct is passed in two general purpose registers */
				jit_memcpy(&(builder->apply_args->word_regs[builder->word_used]),
											value, size);
				(builder->word_used) += 2;
			}
			else if(arg_class1 == X86_64_ARG_INTEGER &&
					arg_class2 == X86_64_ARG_SSE &&
					(builder->float_used < JIT_APPLY_NUM_FLOAT_REGS) &&
					(builder->word_used < JIT_APPLY_NUM_WORD_REGS))
			{
				/* The first eightbyte is passed in a general purpose */
				/* register and the second eightbyte in a sse register */
				builder->apply_args->word_regs[builder->word_used] =
											((jit_nint *)value)[0];
				++(builder->word_used);
				jit_memcpy(&(builder->apply_args->float_regs[builder->float_used]),
											((char *)value) + 8, size - 8);
				++(builder->float_used);
			}
			else if(arg_class1 == X86_64_ARG_SSE &&
					arg_class2 == X86_64_ARG_INTEGER &&
					(builder->float_used < JIT_APPLY_NUM_FLOAT_REGS) &&
					(builder->word_used < JIT_APPLY_NUM_WORD_REGS))
			{
				/* The first eightbyte is passed in a sse register and */
				/* the second eightbyte in a general purpose  register */
				jit_memcpy(&(builder->apply_args->float_regs[builder->float_used]),
											value, 8);
				++(builder->float_used);
				jit_memcpy(&(builder->apply_args->word_regs[builder->word_used]),
											((char *)value) + 8, size - 8);
				++(builder->word_used);
			}
			else
			{
				unsigned int align = jit_type_get_alignment(struct_type);

				jit_apply_builder_add_struct(builder, value, size, align);
			}
		}
	}
	else
	{
		unsigned int align = jit_type_get_alignment(struct_type);

		jit_apply_builder_add_struct(builder, value, size, align);
	}
}

void
_jit_builtin_apply_get_struct(jit_apply_builder *builder,
							  void *value,
							  jit_type_t struct_type)
{
	unsigned int size = jit_type_get_size(struct_type);

	if(size <= 16)
	{
		if(size <= 8)
		{
			int arg_class;
	
			arg_class = _jit_classify_structpart(struct_type, 0, 0, size - 1);
			if(arg_class == X86_64_ARG_NO_CLASS)
			{
				arg_class = X86_64_ARG_SSE;
			}
			if((arg_class == X86_64_ARG_INTEGER) &&
			   (builder->word_used < JIT_APPLY_NUM_WORD_REGS))
			{
				/* The struct is passed in a general purpose register */
				jit_memcpy(value,
						   &(builder->apply_args->word_regs[builder->word_used]),
						   size);
				++(builder->word_used);
			}
			else if((arg_class == X86_64_ARG_SSE) &&
					(builder->float_used < JIT_APPLY_NUM_FLOAT_REGS))
			{
				/* The struct is passed in one sse register */
				jit_memcpy(value,
						   &(builder->apply_args->float_regs[builder->float_used]),
						   size);
				++(builder->float_used);
			}
			else
			{
				/* TODO: always load the value from stack */
				unsigned int align = jit_type_get_alignment(struct_type);

				jit_apply_parser_get_struct(builder, size, align, value);
			}
		}
		else
		{
			int arg_class1;
			int arg_class2;

			arg_class1 = _jit_classify_structpart(struct_type, 0, 0, 7);
			arg_class2 = _jit_classify_structpart(struct_type, 0, 8, size - 1);
			if(arg_class1 == X86_64_ARG_NO_CLASS)
			{
				arg_class1 = X86_64_ARG_SSE;
			}
			if(arg_class2 == X86_64_ARG_NO_CLASS)
			{
				arg_class2 = X86_64_ARG_SSE;
			}
			if(arg_class1 == X86_64_ARG_SSE && arg_class2 == X86_64_ARG_SSE &&
			   (builder->float_used < JIT_APPLY_NUM_FLOAT_REGS))
			{
				/* The struct is passed in one sse register */
				jit_memcpy(value,
						   &(builder->apply_args->float_regs[builder->float_used]),
						   size);
				++(builder->float_used);
			}
			else if(arg_class1 == X86_64_ARG_INTEGER &&
					arg_class2 == X86_64_ARG_INTEGER &&
					(builder->word_used < (JIT_APPLY_NUM_WORD_REGS + 1)))
			{
				/* The struct is passed in two general purpose registers */
				jit_memcpy(value,
						   &(builder->apply_args->word_regs[builder->word_used]),
						   size);
				(builder->word_used) += 2;
			}
			else if(arg_class1 == X86_64_ARG_INTEGER &&
					arg_class2 == X86_64_ARG_SSE &&
					(builder->float_used < JIT_APPLY_NUM_FLOAT_REGS) &&
					(builder->word_used < JIT_APPLY_NUM_WORD_REGS))
			{
				/* The first eightbyte is passed in a general purpose */
				/* register and the second eightbyte in a sse register */
				((jit_nint *)value)[0] =
					builder->apply_args->word_regs[builder->word_used];
				++(builder->word_used);

				jit_memcpy(((char *)value) + 8,
						   &(builder->apply_args->float_regs[builder->float_used]),
						   size - 8);
				++(builder->float_used);
			}
			else if(arg_class1 == X86_64_ARG_SSE &&
					arg_class2 == X86_64_ARG_INTEGER &&
					(builder->float_used < JIT_APPLY_NUM_FLOAT_REGS) &&
					(builder->word_used < JIT_APPLY_NUM_WORD_REGS))
			{
				/* The first eightbyte is passed in a sse register and */
				/* the second eightbyte in a general purpose  register */
				jit_memcpy(value,
						   &(builder->apply_args->float_regs[builder->float_used]),
						   8);
				++(builder->float_used);

				jit_memcpy(((char *)value) + 8,
						   &(builder->apply_args->word_regs[builder->word_used]),
						   size - 8);
				++(builder->word_used);
			}
			else
			{
				/* TODO: always load the value from stack */
				unsigned int align = jit_type_get_alignment(struct_type);

				jit_apply_parser_get_struct(builder, size, align, value);
			}
		}
	}
	else
	{
		/* TODO: always load the value from stack */
		unsigned int align = jit_type_get_alignment(struct_type);

		jit_apply_parser_get_struct(builder, size, align, value);
	}
}

void
_jit_builtin_apply_get_struct_return(jit_apply_builder *builder,
									 void *return_value,
									 jit_apply_return *apply_return,
									 jit_type_t struct_type)
{
	unsigned int size = jit_type_get_size(struct_type);

	if(size <= 16)
	{
		if(size <= 8)
		{
			int arg_class;
	
			arg_class = _jit_classify_structpart(struct_type, 0, 0, size - 1);
			if(arg_class == X86_64_ARG_NO_CLASS)
			{
				arg_class = X86_64_ARG_SSE;
			}
			if(arg_class == X86_64_ARG_INTEGER)
			{
				/* The struct is returned in %rax */
				jit_memcpy(return_value, (void *)apply_return, size);
				return;
			}
			else if(arg_class == X86_64_ARG_SSE)
			{
				/* The struct is returned in %xmm0 */
				jit_memcpy(return_value,
						   &(((jit_ubyte *)apply_return)[16]), size);
				return;
			}
		}
		else
		{
			int arg_class1;
			int arg_class2;

			arg_class1 = _jit_classify_structpart(struct_type, 0, 0, 7);
			arg_class2 = _jit_classify_structpart(struct_type, 0, 8, size - 1);
			if(arg_class1 == X86_64_ARG_NO_CLASS)
			{
				arg_class1 = X86_64_ARG_SSE;
			}
			if(arg_class2 == X86_64_ARG_NO_CLASS)
			{
				arg_class2 = X86_64_ARG_SSE;
			}
			if(arg_class1 == X86_64_ARG_SSE && arg_class2 == X86_64_ARG_SSE)
			{
				/* The struct is returned in %xmm0 */
				jit_memcpy(return_value,
						   &(((jit_ubyte *)apply_return)[16]), size);
				return;
			}
			else if(arg_class1 == X86_64_ARG_INTEGER &&
					arg_class2 == X86_64_ARG_INTEGER)
			{
				/* The struct is returned in %rax and %rdx */
				jit_memcpy(return_value, (void *)apply_return, size);
				return;
			}
			else if(arg_class1 == X86_64_ARG_INTEGER &&
					arg_class2 == X86_64_ARG_SSE)
			{
				/* The first eightbyte is returned in %rax and the second */
				/* eightbyte in %xmm0 */
				((jit_nint *)return_value)[0] =
					*(jit_nint *)apply_return;

				jit_memcpy(((char *)return_value) + 8,
						   &(((jit_ubyte *)apply_return)[16]), size - 8);
				return;
			}
			else if(arg_class1 == X86_64_ARG_SSE &&
					arg_class2 == X86_64_ARG_INTEGER)
			{
				/* The first eightbyte is returned in %xmm0 and the second */
				/* eightbyte in %rax */
				jit_memcpy(return_value,
						   &(((jit_ubyte *)apply_return)[16]), 8);

				jit_memcpy(((char *)return_value) + 8,
						   (void *)apply_return, size - 8);
				return;
			}
		}
	}
	/* All other cases are returned via return_ptr */
	if(builder->struct_return != return_value)
	{
		jit_memcpy(return_value, (builder)->struct_return, size);
	}
}

#endif /* x86-64 */
