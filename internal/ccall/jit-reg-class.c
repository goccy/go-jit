/*
 * jit-reg-class.c - Register class routines for the JIT.
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

#include "jit-internal.h"
#include "jit-reg-class.h"
#include <stdarg.h>

_jit_regclass_t *
_jit_regclass_create(const char *name, int flags, int num_regs, ...)
{
	_jit_regclass_t *regclass;
	va_list args;
	int reg;

	regclass = jit_malloc(sizeof(_jit_regclass_t) + sizeof(int) * (num_regs - 1));
	if(!regclass)
	{
		return 0;
	}
	regclass->name = name;
	regclass->flags = flags;
	regclass->num_regs = num_regs;

	va_start(args, num_regs);
	for(reg = 0; reg < num_regs; reg++)
	{
		regclass->regs[reg] = va_arg(args, int);
	}
	va_end(args);

	return regclass;
}

_jit_regclass_t *
_jit_regclass_combine(const char *name, int flags,
		      _jit_regclass_t *class1,
		      _jit_regclass_t *class2)
{
	_jit_regclass_t *regclass;
	int num_regs;

	num_regs = class1->num_regs + class2->num_regs;

	regclass = jit_malloc(sizeof(_jit_regclass_t) + sizeof(int) * (num_regs - 1));
	if(!regclass)
	{
		return 0;
	}
	regclass->name = name;
	regclass->flags = flags;
	regclass->num_regs = num_regs;

	jit_memcpy(regclass->regs, class1->regs, sizeof(int) * class1->num_regs);
	jit_memcpy(regclass->regs + class1->num_regs, class2->regs, sizeof(int) * class2->num_regs);

	return regclass;
}

void
_jit_regclass_free(_jit_regclass_t *regclass)
{
	jit_free(regclass);
}
