/*
 * jit-apply-x86-64.h - Special definitions for x86-64 function application.
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

#ifndef	_JIT_APPLY_X86_64_H
#define	_JIT_APPLY_X86_64_H

#include <jit/jit-common.h>

/*
 * Flag that a parameter is passed on the stack.
 */
#define JIT_ARG_CLASS_STACK	0xFFFF

/*
 * Define the way the parameter is passed to a specific function
 */
typedef struct
{
	int reg;
	jit_value_t value;
} _jit_structpassing_t;

typedef struct
{
	jit_value_t value;
	jit_ushort arg_class;
	jit_ushort stack_pad;		/* Number of stack words needed for padding */
	union
	{
		_jit_structpassing_t reg_info[4];
		jit_int offset;
	} un;
} _jit_param_t;

/*
 * Structure that is used to help with parameter passing.
 */
typedef struct
{
	int				stack_size;			/* Number of bytes needed on the */
										/* stack for parameter passing */
	int				stack_pad;			/* Number of stack words we have */
										/* to push before pushing the */
										/* parameters for keeping the stack */
										/* aligned */
	unsigned int	word_index;			/* Number of word registers */
										/* allocated */
	unsigned int	max_word_regs;		/* Number of word registers */
										/* available for parameter passing */
	const int	   *word_regs;
	unsigned int	float_index;
	unsigned int	max_float_regs;
	const int	   *float_regs;
	_jit_param_t   *params;

} jit_param_passing_t;

/*
 * Determine how a parameter is passed.
 */
int
_jit_classify_param(jit_param_passing_t *passing,
					_jit_param_t *param, jit_type_t param_type);

/*
 * Determine how a struct type is passed.
 */
int
_jit_classify_struct(jit_param_passing_t *passing,
					_jit_param_t *param, jit_type_t param_type);

/*
 * We handle struct passing ourself
 */
#define HAVE_JIT_BUILTIN_APPLY_STRUCT 1

/*
 * We handle struct returning ourself
 */
#define HAVE_JIT_BUILTIN_APPLY_STRUCT_RETURN 1

/*
 * The granularity of the stack
 */
#define STACK_SLOT_SIZE	sizeof(void *)

/*
 * Get he number of complete stack slots used
 */
#define STACK_SLOTS_USED(size) ((size) >> 3)

/*
 * Round a size up to a multiple of the stack word size.
 */
#define	ROUND_STACK(size)	\
		(((size) + (STACK_SLOT_SIZE - 1)) & ~(STACK_SLOT_SIZE - 1))

/*
 * The "__builtin_apply" functionality in gcc orders the registers
 * in a strange way, which makes it difficult to use.  Our replacement
 * apply structure is laid out in the following order:
 *
 *		stack pointer
 *		%rdi, %rsi, %rdx, %rcx, %r8, %r9
 *		64-bit pad word
 *		%xmm0-%xmm7
 *
 * The total size of the apply structure is 192 bytes.  The return structure
 * is laid out as follows:
 *
 *		%rax, %rdx
 *		%xmm0
 *		%st0
 *
 * The total size of the return structure is 48 bytes.
 */

#if defined(__GNUC__)

#ifndef	JIT_MEMCPY
# if defined(__APPLE__) && defined(__MACH__)
#  define JIT_MEMCPY "_jit_memcpy"
# else
#  define JIT_MEMCPY "jit_memcpy@PLT"
# endif
#endif

/*
 * We have to add all registers not saved by the caller to the clobber list
 * and not only the registers used for parameter passing because we call
 * arbitrary functions.
 * Maybe we should add the mmx* registers too?
 */
#define	jit_builtin_apply(func,args,size,return_float,return_buf)	\
		do { \
			void *__func = (void *)(func); \
			void *__args = (void *)(args); \
			long __size = (((long)(size) + (long)0xf) & ~(long)0xf); \
			void *__return_buf = alloca(64); \
			(return_buf) = __return_buf; \
			__asm__ ( \
				"movq %1, %%rax\n\t" \
				"movq (%%rax), %%rsi\n\t" \
				"movq %2, %%rdx\n\t" \
				"subq %%rdx, %%rsp\n\t" \
				"movq %%rsp, %%rdi\n\t" \
				"callq " JIT_MEMCPY "\n\t" \
				"movq %1, %%rax\n\t" \
				"movq 0x08(%%rax), %%rdi\n\t" \
				"movq 0x10(%%rax), %%rsi\n\t" \
				"movq 0x18(%%rax), %%rdx\n\t" \
				"movq 0x20(%%rax), %%rcx\n\t" \
				"movq 0x28(%%rax), %%r8\n\t" \
				"movq 0x30(%%rax), %%r9\n\t" \
				"movaps 0x40(%%rax), %%xmm0\n\t" \
				"movaps 0x50(%%rax), %%xmm1\n\t" \
				"movaps 0x60(%%rax), %%xmm2\n\t" \
				"movaps 0x70(%%rax), %%xmm3\n\t" \
				"movaps 0x80(%%rax), %%xmm4\n\t" \
				"movaps 0x90(%%rax), %%xmm5\n\t" \
				"movaps 0xA0(%%rax), %%xmm6\n\t" \
				"movaps 0xB0(%%rax), %%xmm7\n\t" \
				"movq %0, %%r11\n\t" \
				"movl $8, %%eax\n\t" \
				"callq *%%r11\n\t" \
				"movq %3, %%rcx\n\t" \
				"movq %%rax, (%%rcx)\n\t" \
				"movq %%rdx, 0x08(%%rcx)\n\t" \
				"movaps %%xmm0, 0x10(%%rcx)\n\t" \
				"movq %2, %%rdx\n\t" \
				"addq %%rdx, %%rsp\n\t" \
				: : "m"(__func), "m"(__args), "m"(__size), "m"(__return_buf) \
				: "rax", "rcx", "rdx", "rdi", "rsi", "r8", "r9", \
				  "r10", "r11", \
				  "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", \
				  "xmm5", "xmm6", "xmm7" \
			); \
			if((return_float)) \
			{ \
				__asm__ ( \
					"movq %0, %%rax\n\t" \
					"fstpt 0x20(%%rax)\n\t" \
					: : "m"(__return_buf) \
					: "rax", "st" \
				); \
			} \
		} while (0)

#define	jit_builtin_apply_args(type,args)	\
		do { \
			void *__args = alloca(192); \
			__asm__ ( \
				"pushq %%rdi\n\t" \
				"leaq 16(%%rbp), %%rdi\n\t" \
				"movq %0, %%rax\n\t" \
				"movq %%rdi, (%%rax)\n\t" \
				"popq %%rdi\n\t" \
				"movq %%rdi, 0x08(%%rax)\n\t" \
				"movq %%rsi, 0x10(%%rax)\n\t" \
				"movq %%rdx, 0x18(%%rax)\n\t" \
				"movq %%rcx, 0x20(%%rax)\n\t" \
				"movq %%r8, 0x28(%%rax)\n\t" \
				"movq %%r9, 0x30(%%rax)\n\t" \
				"movaps %%xmm0, 0x40(%%rax)\n\t" \
				"movaps %%xmm1, 0x50(%%rax)\n\t" \
				"movaps %%xmm2, 0x60(%%rax)\n\t" \
				"movaps %%xmm3, 0x70(%%rax)\n\t" \
				"movaps %%xmm4, 0x80(%%rax)\n\t" \
				"movaps %%xmm5, 0x90(%%rax)\n\t" \
				"movaps %%xmm6, 0xA0(%%rax)\n\t" \
				"movaps %%xmm7, 0xB0(%%rax)\n\t" \
				: : "m"(__args) \
				: "rax", "rcx", "rdx", "rdi", "rsi", "r8", "r9", \
				  "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", \
				  "xmm5", "xmm6", "xmm7" \
			); \
			(args) = (type)__args; \
		} while (0)

#define	jit_builtin_return_int(return_buf)	\
		do { \
			__asm__ ( \
				"lea %0, %%rcx\n\t" \
				"movq (%%rcx), %%rax\n\t" \
				"movq 0x08(%%rcx), %%rdx\n\t" \
				"movaps 0x10(%%rcx), %%xmm0\n\t" \
				: : "m"(*(return_buf)) \
				: "rax", "rcx", "rdx", "xmm0" \
			); \
			return; \
		} while (0)

#define	jit_builtin_return_float(return_buf)	\
		do { \
			__asm__ ( \
				"lea %0, %%rcx\n\t" \
				"movaps 0x10(%%rcx), %%xmm0\n\t" \
				: : "m"(*(return_buf)) \
				: "rcx", "xmm0", "st" \
			); \
			return; \
		} while (0)

#define	jit_builtin_return_double(return_buf)	\
		do { \
			__asm__ ( \
				"lea %0, %%rcx\n\t" \
				"movaps 0x10(%%rcx), %%xmm0\n\t" \
				: : "m"(*(return_buf)) \
				: "rcx", "xmm0", "st" \
			); \
			return; \
		} while (0)

#define	jit_builtin_return_nfloat(return_buf)	\
		do { \
			__asm__ ( \
				"lea %0, %%rcx\n\t" \
				"fldt 0x20(%%rcx)\n\t" \
				: : "m"(*(return_buf)) \
				: "rcx", "xmm0", "st" \
			); \
			return; \
		} while (0)

#define jit_builtin_return_struct(return_buf, type) \
		do { \
		} while (0)

#endif /* GNUC */

/*
 * The maximum number of bytes that are needed to represent a closure,
 * and the alignment to use for the closure.
 */
#define	jit_closure_size		0x90
#define	jit_closure_align		0x20

/*
 * The number of bytes that are needed for a redirector stub.
 * This includes any extra bytes that are needed for alignment.
 */
#define	jit_redirector_size		0x100

/*
 * The number of bytes that are needed for a indirector stub.
 * This includes any extra bytes that are needed for alignment.
 */
#define	jit_indirector_size		0x10


#endif	/* _JIT_APPLY_X86_64_H */
