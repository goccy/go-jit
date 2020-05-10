/*
 * jit-opcode-compat.h - Definition of obsolete opcodes for compatibility
 *						 reasons.
 *
 * Copyright (C) 2011  Southern Storm Software, Pty Ltd.
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

#ifndef _JIT_OPCODE_COMPAT_H
#define	_JIT_OPCODE_COMPAT_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Some obsolete opcodes that have been removed because they are duplicates
 * of other opcodes.
 */
#define JIT_OP_FEQ_INV		JIT_OP_FEQ
#define JIT_OP_FNE_INV		JIT_OP_FNE
#define JIT_OP_DEQ_INV		JIT_OP_DEQ
#define JIT_OP_DNE_INV		JIT_OP_DNE
#define JIT_OP_NFEQ_INV		JIT_OP_NFEQ
#define JIT_OP_NFNE_INV		JIT_OP_NFNE
#define JIT_OP_BR_FEQ_INV	JIT_OP_BR_FEQ
#define JIT_OP_BR_FNE_INV	JIT_OP_BR_FNE
#define JIT_OP_BR_DEQ_INV	JIT_OP_BR_DEQ
#define JIT_OP_BR_DNE_INV	JIT_OP_BR_DNE
#define JIT_OP_BR_NFEQ_INV	JIT_OP_BR_NFEQ
#define JIT_OP_BR_NFNE_INV	JIT_OP_BR_NFNE

#ifdef	__cplusplus
}
#endif

#endif	/* _JIT_VMEM_H */
