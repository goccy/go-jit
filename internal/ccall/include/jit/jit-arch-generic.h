/*
 * jit-arch-generic.h - Architecture-specific definitions.
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

#ifndef	_JIT_ARCH_GENERIC_H
#define	_JIT_ARCH_GENERIC_H

/*
 * If defined _JIT_ARCH_GET_CURRENT_FRAME() macro assigns the current frame
 * pointer to the supplied argument that has to be a void pointer.
 */
#undef _JIT_ARCH_GET_CURRENT_FRAME

/*
 * If defined _JIT_ARCH_GET_NEXT_FRAME() assigns the frame address following
 * the frame supplied as second arg to the value supplied as first argument.
 */
#undef _JIT_ARCH_GET_NEXT_FRAME

/*
 * If defined _JIT_ARCH_GET_RETURN_ADDRESS() assigns the return address of
 * the frame supplied as second arg to the value supplied as first argument.
 */
#undef _JIT_ARCH_GET_RETURN_ADDRESS

/*
 * If defined _JIT_ARCH_GET_CURRENT_RETURN() assigns the return address of
 * the current to the supplied argument.
 */
#define _JIT_ARCH_GET_CURRENT_RETURN

#endif /* _JIT_ARCH_GENERIC_H */
