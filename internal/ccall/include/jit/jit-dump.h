/*
 * jit-dump.h - Functions for dumping JIT structures, for debugging.
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

#ifndef	_JIT_DUMP_H
#define	_JIT_DUMP_H

#include <stdio.h>
#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

void jit_dump_type(FILE *stream, jit_type_t type) JIT_NOTHROW;
void jit_dump_value
	(FILE *stream, jit_function_t func,
	 jit_value_t value, const char *prefix) JIT_NOTHROW;
void jit_dump_insn
	(FILE *stream, jit_function_t func, jit_insn_t insn) JIT_NOTHROW;
void jit_dump_function
	(FILE *stream, jit_function_t func, const char *name) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_DUMP_H */
