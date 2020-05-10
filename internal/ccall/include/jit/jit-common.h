/*
 * jit-common.h - Common type definitions that are used by the JIT.
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

#ifndef	_JIT_COMMON_H
#define	_JIT_COMMON_H

#include <jit/jit-defs.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Opaque structure that represents a context.
 */
typedef struct _jit_context *jit_context_t;

/*
 * Opaque structure that represents a function.
 */
typedef struct _jit_function *jit_function_t;

/*
 * Opaque structure that represents a block.
 */
typedef struct _jit_block *jit_block_t;

/*
 * Opaque structure that represents an instruction.
 */
typedef struct _jit_insn *jit_insn_t;

/*
 * Opaque structure that represents a value.
 */
typedef struct _jit_value *jit_value_t;

/*
 * Opaque structure that represents a type descriptor.
 */
typedef struct _jit_type *jit_type_t;

/*
 * Opaque type that represents an exception stack trace.
 */
typedef struct jit_stack_trace *jit_stack_trace_t;

/*
 * Block label identifier.
 */
typedef jit_nuint jit_label_t;

/*
 * Value that represents an undefined label.
 */
#define	jit_label_undefined	((jit_label_t)~((jit_uint)0))

/*
 * Value that represents an undefined offset.
 */
#define	JIT_NO_OFFSET		(~((unsigned int)0))

/*
 * Function that is used to free user-supplied metadata.
 */
typedef void (*jit_meta_free_func)(void *data);

/*
 * Function that is used to compile a function on demand.
 * Returns zero if the compilation process failed for some reason.
 */
typedef int (*jit_on_demand_func)(jit_function_t func);

/*
 * Function that is used to control on demand compilation.
 * Typically, it should take care of the context locking and unlocking,
 * calling function's on demand compiler, and final compilation.
 */
typedef void *(*jit_on_demand_driver_func)(jit_function_t func);

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_COMMON_H */
