/*
 * jit-arch-arm.h - Architecture-specific definitions.
 *
 * Copyright (C) 2006  Southern Storm Software, Pty Ltd.
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

#ifndef	_JIT_ARCH_ARM_H
#define	_JIT_ARCH_ARM_H

/*
 * If defined _JIT_ARCH_GET_CURRENT_FRAME() macro assigns the current frame
 * pointer to the supplied argument that has to be a void pointer.
 */
#if defined(__GNUC__)
#define _JIT_ARCH_GET_CURRENT_FRAME(f)		\
	do {					\
		register void *__f asm("fp");	\
		f = __f;			\
	} while(0)
#else
#undef _JIT_ARCH_GET_CURRENT_FRAME
#endif

#endif /* _JIT_ARCH_ARM_H */
