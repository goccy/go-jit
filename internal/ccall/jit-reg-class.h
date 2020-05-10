/*
 * jit-reg-class.h - Register class routines for the JIT.
 *
 * Copyright (C) 2006  Southern Storm Software, Pty Ltd.
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

#ifndef	_JIT_REG_CLASS_H
#define	_JIT_REG_CLASS_H

#include "jit-rules.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Information about a register class.
 */
typedef struct
{
	const char	*name;		/* Name of the register class, for debugging */
	int		flags;		/* Register flags */
	int		num_regs;	/* The number of registers in the class */
	int		regs[1];	/* JIT_REG_INFO index for each register */

} _jit_regclass_t;

/* Create a register class.  */
_jit_regclass_t *_jit_regclass_create(const char *name, int flags, int num_regs, ...);

/* Combine two register classes into another one.  */
_jit_regclass_t *_jit_regclass_combine(const char *name, int flags,
				       _jit_regclass_t *class1,
				       _jit_regclass_t *class2);

/* Free a register class.  */
void _jit_regclass_free(_jit_regclass_t *regclass);

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_REG_CLASS_H */
