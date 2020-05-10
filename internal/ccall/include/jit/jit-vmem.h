/*
 * jit-vmem.h - Virtual memory routines.
 *
 * Copyright (C) 2011  Aleksey Demakov
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

#ifndef _JIT_VMEM_H
#define	_JIT_VMEM_H

#include <jit/jit-defs.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
	JIT_PROT_NONE,
	JIT_PROT_READ,
	JIT_PROT_READ_WRITE,
	JIT_PROT_EXEC_READ,
	JIT_PROT_EXEC_READ_WRITE,
} jit_prot_t;


void jit_vmem_init(void);

jit_uint jit_vmem_page_size(void);
jit_nuint jit_vmem_round_up(jit_nuint value);
jit_nuint jit_vmem_round_down(jit_nuint value);

void *jit_vmem_reserve(jit_uint size);
void *jit_vmem_reserve_committed(jit_uint size, jit_prot_t prot);
int jit_vmem_release(void *addr, jit_uint size);

int jit_vmem_commit(void *addr, jit_uint size, jit_prot_t prot);
int jit_vmem_decommit(void *addr, jit_uint size);

int jit_vmem_protect(void *addr, jit_uint size, jit_prot_t prot);

#ifdef	__cplusplus
}
#endif

#endif	/* _JIT_VMEM_H */
