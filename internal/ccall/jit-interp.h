/*
 * jit-interp.h - Bytecode interpreter for platforms without native support.
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

#ifndef	_JIT_INTERP_H
#define	_JIT_INTERP_H

#include "jit-internal.h"
#include "jit-apply-rules.h"
#include "jit-interp-opcode.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Structure of a stack item.
 */
typedef union
{
	jit_int		int_value;
	jit_uint	uint_value;
	jit_long	long_value;
	jit_ulong	ulong_value;
	jit_float32	float32_value;
	jit_float64	float64_value;
	jit_nfloat	nfloat_value;
	void	   *ptr_value;
#if JIT_APPLY_MAX_STRUCT_IN_REG != 0
	char		struct_value[JIT_APPLY_MAX_STRUCT_IN_REG];
#endif

} jit_item;

/*
 * Number of items that make up a struct or union value on the stack.
 */
#define	JIT_NUM_ITEMS_IN_STRUCT(size)		\
	(((size) + (sizeof(jit_item) - 1)) / sizeof(jit_item))

/*
 * Information that is prefixed to a function that describes
 * its interpretation context.  The code starts just after this.
 */
typedef struct jit_function_interp *jit_function_interp_t;
struct jit_function_interp
{
	/* The function that this structure is associated with */
	jit_function_t	func;

	/* Size of the argument area to allocate, in bytes */
	unsigned int	args_size;

	/* Size of the local stack frame to allocate, in bytes */
	unsigned int	frame_size;

	/* Size of the working stack area of the frame, in items */
	unsigned int	working_area;
};

/*
 * Get the size of the "jit_function_interp" structure, rounded
 * up to a multiple of "void *".
 */
#define	jit_function_interp_size	\
			((sizeof(struct jit_function_interp) + sizeof(void *) - 1) & \
			 ~(sizeof(void *) - 1))

/*
 * Get the entry point for a function, from its "jit_function_interp_t" block.
 */
#define	jit_function_interp_entry_pc(info)	\
			((void **)(((unsigned char *)(info)) + jit_function_interp_size))

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_INTERP_H */
