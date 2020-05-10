/*
 * jit-rules-interp.h - Rules that define the interpreter characteristics.
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

#ifndef	_JIT_RULES_INTERP_H
#define	_JIT_RULES_INTERP_H

#include "jit-interp.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Information about all of the registers, in allocation order.
 */
#define	JIT_REG_INFO \
	{"r0", 0, -1, JIT_REG_ALL | JIT_REG_CALL_USED}, \
	{"r1", 1, -1, JIT_REG_ALL | JIT_REG_CALL_USED}, \
	{"r2", 2, -1, JIT_REG_ALL | JIT_REG_CALL_USED},
#define	JIT_NUM_REGS		3
#define	JIT_NUM_GLOBAL_REGS	0

/*
 * Define to 1 if we should always load values into registers
 * before operating on them.  i.e. the CPU does not have reg-mem
 * and mem-reg addressing modes.
 */
#define	JIT_ALWAYS_REG_REG		1

/*
 * The maximum number of bytes to allocate for the prolog.
 * This may be shortened once we know the true prolog size.
 */
#define	JIT_PROLOG_SIZE			jit_function_interp_size

/*
 * Preferred alignment for the start of functions.
 */
#define	JIT_FUNCTION_ALIGNMENT	(sizeof(void *))

/*
 * Define this to 1 if the platform allows reads and writes on
 * any byte boundary.  Define to 0 if only properly-aligned
 * memory accesses are allowed.
 */
#define	JIT_ALIGN_OVERRIDES		0

/*
 * Extra state information that is added to the "jit_gencode" structure.
 */
#define	jit_extra_gen_state	\
			int working_area; \
			int max_working_area; \
			int extra_working_space
#define	jit_extra_gen_init(gen)	\
			do { \
				(gen)->working_area = 0; \
				(gen)->max_working_area = 0; \
				(gen)->extra_working_space = 0; \
			} while (0)
#define	jit_extra_gen_cleanup(gen)	do { ; } while (0)

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_RULES_INTERP_H */
