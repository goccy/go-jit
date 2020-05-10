/*
 * jit-unwind.h - Routines for performing stack unwinding.
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

#ifndef	_JIT_UNWIND_H
#define	_JIT_UNWIND_H

#include <jit/jit-common.h>
#include <jit/jit-arch.h>

#ifdef	__cplusplus
extern	"C" {
#endif

typedef struct
{
	void *frame;
	void *cache;
	jit_context_t context;
#ifdef _JIT_ARCH_UNWIND_DATA
	_JIT_ARCH_UNWIND_DATA
#endif
} jit_unwind_context_t;

int jit_unwind_init(jit_unwind_context_t *unwind, jit_context_t context);
void jit_unwind_free(jit_unwind_context_t *unwind);

int jit_unwind_next(jit_unwind_context_t *unwind);
int jit_unwind_next_pc(jit_unwind_context_t *unwind);
void *jit_unwind_get_pc(jit_unwind_context_t *unwind);

int jit_unwind_jump(jit_unwind_context_t *unwind, void *pc);

jit_function_t jit_unwind_get_function(jit_unwind_context_t *unwind);
unsigned int jit_unwind_get_offset(jit_unwind_context_t *unwind);

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_UNWIND_H */
