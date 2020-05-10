/*
 * jit-cpuid-x86.h - Wrapper for the CPUID instruction.
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

#ifndef	_JIT_CPUID_X86_H
#define	_JIT_CPUID_X86_H

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Structure that is used to return CPU identification information.
 */
typedef struct
{
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

} jit_cpuid_x86_t;

/*
 * Indexes for querying cpuid information.
 */
#define	JIT_X86CPUID_FEATURES			1
#define	JIT_X86CPUID_CACHE_TLB			2
#define	JIT_X86CPUID_SERIAL_NUMBER		3

/*
 * Feature information.
 */
#define	JIT_X86FEATURE_FPU				0x00000001
#define	JIT_X86FEATURE_VME				0x00000002
#define	JIT_X86FEATURE_DE				0x00000004
#define	JIT_X86FEATURE_PSE				0x00000008
#define	JIT_X86FEATURE_TSC				0x00000010
#define	JIT_X86FEATURE_MSR				0x00000020
#define	JIT_X86FEATURE_PAE				0x00000040
#define	JIT_X86FEATURE_MCE				0x00000080
#define	JIT_X86FEATURE_CX8				0x00000100
#define	JIT_X86FEATURE_APIC				0x00000200
#define	JIT_X86FEATURE_RESERVED_1		0x00000400
#define	JIT_X86FEATURE_SEP				0x00000800
#define	JIT_X86FEATURE_MTRR				0x00001000
#define	JIT_X86FEATURE_PGE				0x00002000
#define	JIT_X86FEATURE_MCA				0x00004000
#define	JIT_X86FEATURE_CMOV				0x00008000
#define	JIT_X86FEATURE_PAT				0x00010000
#define	JIT_X86FEATURE_PSE36			0x00020000
#define	JIT_X86FEATURE_PSN				0x00040000
#define	JIT_X86FEATURE_CLFSH			0x00080000
#define	JIT_X86FEATURE_RESERVED_2		0x00100000
#define	JIT_X86FEATURE_DS				0x00200000
#define	JIT_X86FEATURE_ACPI				0x00400000
#define	JIT_X86FEATURE_MMX				0x00800000
#define	JIT_X86FEATURE_FXSR				0x01000000
#define	JIT_X86FEATURE_SSE				0x02000000
#define	JIT_X86FEATURE_SSE2				0x04000000
#define	JIT_X86FEATURE_SS				0x08000000
#define	JIT_X86FEATURE_RESERVED_3		0x10000000
#define	JIT_X86FEATURE_TM				0x20000000
#define	JIT_X86FEATURE_RESERVED_4		0x40000000
#define	JIT_X86FEATURE_RESERVED_5		0x80000000

/*
 * Get CPU identification information.  Returns zero if the requested
 * information is not available.
 */
int _jit_cpuid_x86_get(unsigned int index, jit_cpuid_x86_t *info);

/*
 * Determine if the CPU has a particular feature.
 */
int _jit_cpuid_x86_has_feature(unsigned int feature);

/*
 * Get the size of the CPU cache line, or zero if flushing is not required.
 */
unsigned int _jit_cpuid_x86_line_size(void);

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_CPUID_X86_H */
