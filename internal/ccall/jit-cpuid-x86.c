/*
 * jit-cpuid-x86.c - Wrapper for the CPUID instruction.
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

#include "jit-cpuid-x86.h"

#if defined(__i386) || defined(__i386__) || defined(_M_IX86)

/*
 * Determine if the "cpuid" instruction is present by twiddling
 * bit 21 of the EFLAGS register.
 */
static int cpuid_present(void)
{
#if defined(__GNUC__)
	int result;
	__asm__ __volatile__ (
		"\tpushfl\n"
		"\tpopl %%eax\n"
		"\tmovl %%eax, %%ecx\n"
		"\tandl $0x200000, %%ecx\n"
		"\txorl $0x200000, %%eax\n"
		"\tpushl %%eax\n"
		"\tpopfl\n"
		"\tpushfl\n"
		"\tandl $0x200000, %%eax\n"
		"\txorl %%ecx, %%eax\n"
		"\tmovl %%eax, %0\n"
		: "=m" (result) : : "eax", "ecx"
	);
	return (result != 0);
#else
	return 0;
#endif
}

/*
 * Issue a "cpuid" query and get the result.
 */
static void cpuid_query(unsigned int index, jit_cpuid_x86_t *info)
{
#if defined(__GNUC__)
	__asm__ __volatile__ (
		"\tmovl %0, %%eax\n"
		"\tpushl %%ebx\n"
		"\txorl %%ebx, %%ebx\n"
		"\txorl %%ecx, %%ecx\n"
		"\txorl %%edx, %%edx\n"
		"\t.byte 0x0F\n"			/* cpuid, safe against old assemblers */
		"\t.byte 0xA2\n"
		"\tmovl %1, %%esi\n"
		"\tmovl %%eax, (%%esi)\n"
		"\tmovl %%ebx, 4(%%esi)\n"
		"\tmovl %%ecx, 8(%%esi)\n"
		"\tmovl %%edx, 12(%%esi)\n"
		"\tpopl %%ebx\n"
		: : "m"(index), "m"(info) : "eax", "ecx", "edx", "esi"
	);
#endif
}

int _jit_cpuid_x86_get(unsigned int index, jit_cpuid_x86_t *info)
{
	/* Determine if this cpu has the "cpuid" instruction */
	if(!cpuid_present())
	{
		return 0;
	}

	/* Validate the index */
	if((index & 0x80000000) == 0)
	{
		cpuid_query(0, info);
	}
	else
	{
		cpuid_query(0x80000000, info);
	}
	if(index > info->eax)
	{
		return 0;
	}

	/* Execute the actual requested query */
	cpuid_query(index, info);
	return 1;
}

int _jit_cpuid_x86_has_feature(unsigned int feature)
{
	jit_cpuid_x86_t info;
	if(!_jit_cpuid_x86_get(JIT_X86CPUID_FEATURES, &info))
	{
		return 0;
	}
	return ((info.edx & feature) != 0);
}

unsigned int _jit_cpuid_x86_line_size(void)
{
	jit_cpuid_x86_t info;
	if(!_jit_cpuid_x86_get(JIT_X86CPUID_FEATURES, &info))
	{
		return 0;
	}
	if((info.edx & JIT_X86FEATURE_CLFSH) == 0)
	{
		return 0;
	}
	return ((info.ebx & 0x0000FF00) >> 5);
}

#endif /* i386 */
