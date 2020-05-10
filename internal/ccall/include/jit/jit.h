/*
 * jit.h - General definitions for JIT back-ends.
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

#ifndef	_JIT_H
#define	_JIT_H

#ifdef	__cplusplus
extern	"C" {
#endif

#include <jit/jit-defs.h>
#include <jit/jit-common.h>
#include <jit/jit-context.h>
#include <jit/jit-apply.h>
#include <jit/jit-block.h>
#include <jit/jit-debugger.h>
#include <jit/jit-dump.h>
#include <jit/jit-elf.h>
#include <jit/jit-except.h>
#include <jit/jit-function.h>
#include <jit/jit-init.h>
#include <jit/jit-insn.h>
#include <jit/jit-intrinsic.h>
#include <jit/jit-memory.h>
#include <jit/jit-meta.h>
#include <jit/jit-objmodel.h>
#include <jit/jit-opcode.h>
#include <jit/jit-type.h>
#include <jit/jit-unwind.h>
#include <jit/jit-util.h>
#include <jit/jit-value.h>
#include <jit/jit-vmem.h>
#include <jit/jit-walk.h>

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_H */
