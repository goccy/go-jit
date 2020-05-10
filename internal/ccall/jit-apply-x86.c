/*
 * jit-apply-x86.c - Apply support routines for x86.
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

#if defined(__i386) || defined(__i386__) || defined(_M_IX86)

#include "jit-gen-x86.h"

void _jit_create_closure(unsigned char *buf, void *func,
                         void *closure, void *_type)
{
	jit_type_t signature = (jit_type_t)_type;
	jit_type_t type;
#if JIT_APPLY_X86_FASTCALL == 1
	jit_abi_t abi = jit_type_get_abi(signature);
#endif
	unsigned int num_bytes = 0;
	int struct_return_offset = 0;

	/* Set up the local stack frame */
	x86_push_reg(buf, X86_EBP);
	x86_mov_reg_reg(buf, X86_EBP, X86_ESP, 4);

	/* Create the apply argument block on the stack */
#if JIT_APPLY_X86_FASTCALL == 1
	if(abi == jit_abi_fastcall)
	{
		x86_push_reg(buf, X86_EDX);
		x86_push_reg(buf, X86_ECX);
	}
#endif
	x86_lea_membase(buf, X86_EAX, X86_EBP, 8);
	x86_push_reg(buf, X86_EAX);

	/* Push the arguments for calling "func" */
	x86_mov_reg_reg(buf, X86_EAX, X86_ESP, 4);
	x86_push_reg(buf, X86_EAX);
	x86_push_imm(buf, (int)closure);

	/* Call the closure handling function */
	x86_call_code(buf, func);

	/* Determine the number of bytes to pop when we return */
#if JIT_APPLY_X86_FASTCALL == 1
	if(abi == jit_abi_stdcall || abi == jit_abi_fastcall)
	{
		unsigned int word_regs;
		unsigned int size;
		unsigned int num_params;
		unsigned int param;
		if(abi == jit_abi_stdcall)
		{
			word_regs = 0;
		}
		else
		{
			word_regs = 2;
		}
		type = jit_type_normalize(jit_type_get_return(signature));
		if(jit_type_return_via_pointer(type))
		{
			if(word_regs > 0)
			{
				--word_regs;
			}
			else
			{
				num_bytes += sizeof(void *);
				struct_return_offset = 2 * sizeof(void *);
			}
		}
		num_params = jit_type_num_params(signature);
		for(param = 0; param < num_params; ++param)
		{
			type = jit_type_normalize(jit_type_get_param(signature, param));
			size = jit_type_get_size(type);
			if(word_regs > 0)
			{
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
					case JIT_TYPE_SIGNATURE:
					case JIT_TYPE_PTR:
					{
						--word_regs;
					}
					continue;

					case JIT_TYPE_LONG:
					case JIT_TYPE_ULONG:
					{
						if(word_regs == 1)
						{
							num_bytes += sizeof(void *);
						}
						word_regs = 0;
					}
					continue;
				}
				word_regs = 0;
			}
			num_bytes += (size + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
		}
	}
	else
#endif
	{
		type = jit_type_normalize(jit_type_get_return(signature));
		if(jit_type_return_via_pointer(type))
		{
#if JIT_APPLY_X86_POP_STRUCT_RETURN == 1
			/* Pop the structure return pointer as we return back */
			num_bytes += sizeof(void *);
#endif
			struct_return_offset = 2 * sizeof(void *);
		}
	}

	/* If we are returning a structure via a pointer, then load
	   the address of the structure into the EAX register */
	if(struct_return_offset != 0)
	{
		x86_mov_reg_membase(buf, X86_EAX, X86_EBP, struct_return_offset, 4);
	}

	/* Pop the current stack frame */
	x86_mov_reg_reg(buf, X86_ESP, X86_EBP, 4);
	x86_pop_reg(buf, X86_EBP);

	/* Return from the closure */
	if(num_bytes > 0)
	{
		x86_ret_imm(buf, num_bytes);
	}
	else
	{
		x86_ret(buf);
	}
}

void *_jit_create_redirector(unsigned char *buf, void *func,
							 void *user_data, int abi)
{
	void *start = (void *)buf;

	/* Save the fastcall registers, if necessary */
#if JIT_APPLY_X86_FASTCALL == 1
	if(abi == (int)jit_abi_fastcall)
	{
		x86_push_reg(buf, X86_EDX);
		x86_push_reg(buf, X86_ECX);
	}
#endif

	/* Push the user data onto the stack */
	x86_push_imm(buf, (int)user_data);

	/* Call "func" (the pointer result will be in EAX) */
	x86_call_code(buf, func);

	/* Remove the user data from the stack */
	x86_pop_reg(buf, X86_ECX);

	/* Restore the fastcall registers, if necessary */
#if JIT_APPLY_X86_FASTCALL == 1
	if(abi == (int)jit_abi_fastcall)
	{
		x86_pop_reg(buf, X86_ECX);
		x86_pop_reg(buf, X86_EDX);
	}
#endif

	/* Jump to the function that the redirector indicated */
	x86_jump_reg(buf, X86_EAX);

	/* Return the start of the buffer as the redirector entry point */
	return start;
}

void *_jit_create_indirector(unsigned char *buf, void **entry)
{
	void *start = (void *)buf;

	/* Jump to the entry point. */
	x86_jump_mem(buf, entry);

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

#endif /* x86 */
