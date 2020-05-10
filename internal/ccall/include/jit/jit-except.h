/*
 * jit-except.h - Exception handling functions.
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

#ifndef	_JIT_EXCEPT_H
#define	_JIT_EXCEPT_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Builtin exception type codes, and result values for intrinsic functions.
 */
#define JIT_RESULT_OK			(1)
#define JIT_RESULT_OVERFLOW		(0)
#define JIT_RESULT_ARITHMETIC		(-1)
#define JIT_RESULT_DIVISION_BY_ZERO	(-2)
#define JIT_RESULT_COMPILE_ERROR	(-3)
#define JIT_RESULT_OUT_OF_MEMORY	(-4)
#define JIT_RESULT_NULL_REFERENCE	(-5)
#define JIT_RESULT_NULL_FUNCTION	(-6)
#define JIT_RESULT_CALLED_NESTED	(-7)
#define JIT_RESULT_OUT_OF_BOUNDS	(-8)
#define JIT_RESULT_UNDEFINED_LABEL	(-9)
#define JIT_RESULT_MEMORY_FULL		(-10000)

/*
 * Exception handling function for builtin exceptions.
 */
typedef void *(*jit_exception_func)(int exception_type);

/*
 * External function declarations.
 */
void *jit_exception_get_last(void);
void *jit_exception_get_last_and_clear(void);
void jit_exception_set_last(void *object);
void jit_exception_clear_last(void);
void jit_exception_throw(void *object);
void jit_exception_builtin(int exception_type);
jit_exception_func jit_exception_set_handler(jit_exception_func handler);
jit_exception_func jit_exception_get_handler(void);
jit_stack_trace_t jit_exception_get_stack_trace(void);
unsigned int jit_stack_trace_get_size(jit_stack_trace_t trace);
jit_function_t jit_stack_trace_get_function(jit_context_t context,
					    jit_stack_trace_t trace,
					    unsigned int posn);
void *jit_stack_trace_get_pc(jit_stack_trace_t trace, unsigned int posn);
unsigned int jit_stack_trace_get_offset(jit_context_t context,
					jit_stack_trace_t trace,
					unsigned int posn);
void jit_stack_trace_free(jit_stack_trace_t trace);

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_EXCEPT_H */
