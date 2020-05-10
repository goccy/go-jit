/* Automatically generated from ./jit-interp-opcodes.ops - DO NOT EDIT */

/*
 * jit-interp-opcode.c - Information about interpreter specific JIT opcodes.
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

#include "jit-internal.h"
#include "jit-interp-opcode.h"
#include "jit-rules.h"

#if defined(JIT_BACKEND_INTERP)

jit_opcode_info_t const _jit_interp_opcodes[JIT_INTERP_OP_NUM_OPCODES] = {
	{"lda_0_sbyte", JIT_OPCODE_NINT_ARG},
	{"lda_0_ubyte", JIT_OPCODE_NINT_ARG},
	{"lda_0_short", JIT_OPCODE_NINT_ARG},
	{"lda_0_ushort", JIT_OPCODE_NINT_ARG},
	{"lda_0_int", JIT_OPCODE_NINT_ARG},
	{"lda_0_long", JIT_OPCODE_NINT_ARG},
	{"lda_0_float32", JIT_OPCODE_NINT_ARG},
	{"lda_0_float64", JIT_OPCODE_NINT_ARG},
	{"lda_0_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldaa_0", JIT_OPCODE_NINT_ARG},
	{"lda_1_sbyte", JIT_OPCODE_NINT_ARG},
	{"lda_1_ubyte", JIT_OPCODE_NINT_ARG},
	{"lda_1_short", JIT_OPCODE_NINT_ARG},
	{"lda_1_ushort", JIT_OPCODE_NINT_ARG},
	{"lda_1_int", JIT_OPCODE_NINT_ARG},
	{"lda_1_long", JIT_OPCODE_NINT_ARG},
	{"lda_1_float32", JIT_OPCODE_NINT_ARG},
	{"lda_1_float64", JIT_OPCODE_NINT_ARG},
	{"lda_1_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldaa_1", JIT_OPCODE_NINT_ARG},
	{"lda_2_sbyte", JIT_OPCODE_NINT_ARG},
	{"lda_2_ubyte", JIT_OPCODE_NINT_ARG},
	{"lda_2_short", JIT_OPCODE_NINT_ARG},
	{"lda_2_ushort", JIT_OPCODE_NINT_ARG},
	{"lda_2_int", JIT_OPCODE_NINT_ARG},
	{"lda_2_long", JIT_OPCODE_NINT_ARG},
	{"lda_2_float32", JIT_OPCODE_NINT_ARG},
	{"lda_2_float64", JIT_OPCODE_NINT_ARG},
	{"lda_2_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldaa_2", JIT_OPCODE_NINT_ARG},
	{"sta_0_byte", JIT_OPCODE_NINT_ARG},
	{"sta_0_short", JIT_OPCODE_NINT_ARG},
	{"sta_0_int", JIT_OPCODE_NINT_ARG},
	{"sta_0_long", JIT_OPCODE_NINT_ARG},
	{"sta_0_float32", JIT_OPCODE_NINT_ARG},
	{"sta_0_float64", JIT_OPCODE_NINT_ARG},
	{"sta_0_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldl_0_sbyte", JIT_OPCODE_NINT_ARG},
	{"ldl_0_ubyte", JIT_OPCODE_NINT_ARG},
	{"ldl_0_short", JIT_OPCODE_NINT_ARG},
	{"ldl_0_ushort", JIT_OPCODE_NINT_ARG},
	{"ldl_0_int", JIT_OPCODE_NINT_ARG},
	{"ldl_0_long", JIT_OPCODE_NINT_ARG},
	{"ldl_0_float32", JIT_OPCODE_NINT_ARG},
	{"ldl_0_float64", JIT_OPCODE_NINT_ARG},
	{"ldl_0_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldla_0", JIT_OPCODE_NINT_ARG},
	{"ldl_1_sbyte", JIT_OPCODE_NINT_ARG},
	{"ldl_1_ubyte", JIT_OPCODE_NINT_ARG},
	{"ldl_1_short", JIT_OPCODE_NINT_ARG},
	{"ldl_1_ushort", JIT_OPCODE_NINT_ARG},
	{"ldl_1_int", JIT_OPCODE_NINT_ARG},
	{"ldl_1_long", JIT_OPCODE_NINT_ARG},
	{"ldl_1_float32", JIT_OPCODE_NINT_ARG},
	{"ldl_1_float64", JIT_OPCODE_NINT_ARG},
	{"ldl_1_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldla_1", JIT_OPCODE_NINT_ARG},
	{"ldl_2_sbyte", JIT_OPCODE_NINT_ARG},
	{"ldl_2_ubyte", JIT_OPCODE_NINT_ARG},
	{"ldl_2_short", JIT_OPCODE_NINT_ARG},
	{"ldl_2_ushort", JIT_OPCODE_NINT_ARG},
	{"ldl_2_int", JIT_OPCODE_NINT_ARG},
	{"ldl_2_long", JIT_OPCODE_NINT_ARG},
	{"ldl_2_float32", JIT_OPCODE_NINT_ARG},
	{"ldl_2_float64", JIT_OPCODE_NINT_ARG},
	{"ldl_2_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldla_2", JIT_OPCODE_NINT_ARG},
	{"stl_0_byte", JIT_OPCODE_NINT_ARG},
	{"stl_0_short", JIT_OPCODE_NINT_ARG},
	{"stl_0_int", JIT_OPCODE_NINT_ARG},
	{"stl_0_long", JIT_OPCODE_NINT_ARG},
	{"stl_0_float32", JIT_OPCODE_NINT_ARG},
	{"stl_0_float64", JIT_OPCODE_NINT_ARG},
	{"stl_0_nfloat", JIT_OPCODE_NINT_ARG},
	{"ldc_0_int", JIT_OPCODE_NINT_ARG},
	{"ldc_1_int", JIT_OPCODE_NINT_ARG},
	{"ldc_2_int", JIT_OPCODE_NINT_ARG},
	{"ldc_0_long", JIT_OPCODE_CONST_LONG},
	{"ldc_1_long", JIT_OPCODE_CONST_LONG},
	{"ldc_2_long", JIT_OPCODE_CONST_LONG},
	{"ldc_0_float32", JIT_OPCODE_CONST_FLOAT32},
	{"ldc_1_float32", JIT_OPCODE_CONST_FLOAT32},
	{"ldc_2_float32", JIT_OPCODE_CONST_FLOAT32},
	{"ldc_0_float64", JIT_OPCODE_CONST_FLOAT64},
	{"ldc_1_float64", JIT_OPCODE_CONST_FLOAT64},
	{"ldc_2_float64", JIT_OPCODE_CONST_FLOAT64},
	{"ldc_0_nfloat", JIT_OPCODE_CONST_NFLOAT},
	{"ldc_1_nfloat", JIT_OPCODE_CONST_NFLOAT},
	{"ldc_2_nfloat", JIT_OPCODE_CONST_NFLOAT},
	{"ldr_0_int", 0},
	{"ldr_0_long", 0},
	{"ldr_0_float32", 0},
	{"ldr_0_float64", 0},
	{"ldr_0_nfloat", 0},
	{"pop", 0},
	{"pop_2", 0},
	{"pop_3", 0},
	{"end_marker", 0}
};

#endif /* defined(JIT_BACKEND_INTERP) */
