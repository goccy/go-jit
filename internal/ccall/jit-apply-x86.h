/*
 * jit-apply-x86.h - Special definitions for x86 function application.
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

#ifndef	_JIT_APPLY_X86_H
#define	_JIT_APPLY_X86_H

/*
 * The "__builtin_apply" functionality in gcc has some problems
 * dealing with floating-point values, and it also doesn't handle
 * STDCALL and FASTCALL functions well.  Therefore, we use assembly
 * code instead.
 *
 * There are three versions here: gcc/non-Win32, gcc/Win32, and msvc/Win32.
 * Cygwin is included in the gcc/Win32 case.
 */

#if defined(__GNUC__)

#if !defined(__CYGWIN__) && !defined(_WIN32) && !defined(WIN32)

/* Mac OS X prefixes static symbols with an underscore, and external symbol
   references are late-bound through a PIC stub by the dynamic linker */
#ifndef JIT_MEMCPY
# if defined(__APPLE__) && defined(__MACH__)
#  define JIT_MEMCPY "L_memcpy$stub"
# else
#  define JIT_MEMCPY "memcpy@PLT"
# endif
#endif

#define	jit_builtin_apply(func,args,size,return_float,return_buf)	\
		do { \
			void *__func = (void *)(func); \
			void *__args = (void *)(args); \
			void *__size = (void *)(size); \
			void *__return_buf = alloca(20); \
			(return_buf) = __return_buf; \
			__asm__ ( \
				"pushl %%esi\n\t" \
				"movl %%esp, %%esi\n\t" \
				"subl %2, %%esp\n\t" \
				"movl %%esp, %%eax\n\t" \
				"movl %1, %%ecx\n\t" \
				"movl (%%ecx), %%ecx\n\t" \
				"pushl %2\n\t" \
				"pushl %%ecx\n\t" \
				"pushl %%eax\n\t" \
				"call " JIT_MEMCPY "\n\t" \
				"addl $12, %%esp\n\t" \
				"movl %1, %%ecx\n\t" \
				"movl %0, %%eax\n\t" \
				"call *%%eax\n\t" \
				"movl %3, %%ecx\n\t" \
				"movl %%eax, (%%ecx)\n\t" \
				"movl %%edx, 4(%%ecx)\n\t" \
				"movl %%esi, %%esp\n\t" \
				"popl %%esi\n\t" \
				: : "m"(__func), "m"(__args), "m"(__size), "m"(__return_buf) \
				: "eax", "ecx", "edx" \
			); \
			if((return_float)) \
			{ \
				if(sizeof(jit_nfloat) == sizeof(double)) \
				{ \
					__asm__ ( \
						"movl %0, %%ecx\n\t" \
						"fstpl 8(%%ecx)\n\t" \
						: : "m"(__return_buf) \
						: "ecx", "st" \
					); \
				} \
				else \
				{ \
					__asm__ ( \
						"movl %0, %%ecx\n\t" \
						"fstpt 8(%%ecx)\n\t" \
						: : "m"(__return_buf) \
						: "ecx", "st" \
					); \
				} \
	    		} \
		} while (0)

#define	jit_builtin_apply_args(type,args)	\
		do { \
			void *__args = alloca(4); \
			__asm__ ( \
				"leal 8(%%ebp), %%eax\n\t" \
				"movl %0, %%ecx\n\t" \
				"movl %%eax, (%%ecx)\n\t" \
				: : "m"(__args) \
				: "eax", "ecx" \
			); \
			(args) = (type)__args; \
		} while (0)

#define	jit_builtin_return_int(return_buf)	\
		do { \
			__asm__ ( \
				"leal %0, %%ecx\n\t" \
				"movl (%%ecx), %%eax\n\t" \
				"movl 4(%%ecx), %%edx\n\t" \
				: : "m"(*(return_buf)) \
				: "eax", "ecx", "edx" \
			); \
			return; \
		} while (0)

#define	jit_builtin_return_float(return_buf)	\
		do { \
			jit_nfloat __value = \
				((jit_apply_return *)(return_buf))-> \
					nfloat_value.f_value; \
			if(sizeof(jit_nfloat) == sizeof(double)) \
			{ \
				__asm__ ( \
					"leal %0, %%ecx\n\t" \
					"fldl (%%ecx)\n\t" \
					: : "m"(__value) \
					: "ecx", "st" \
				); \
			} \
			else \
			{ \
				__asm__ ( \
					"leal %0, %%ecx\n\t" \
					"fldt (%%ecx)\n\t" \
					: : "m"(__value) \
					: "ecx", "st" \
				); \
			} \
			return; \
		} while (0)

#else /* Win32 */

#define	jit_builtin_apply(func,args,size,return_float,return_buf)	\
		do { \
			void *__func = (void *)(func); \
			void *__args = (void *)(args); \
			void *__size = (void *)(size); \
			void *__return_buf = alloca(20); \
			(return_buf) = __return_buf; \
			__asm__ ( \
				"pushl %%esi\n\t" \
				"movl %%esp, %%esi\n\t" \
				"subl %2, %%esp\n\t" \
				"movl %%esp, %%eax\n\t" \
				"movl %1, %%ecx\n\t" \
				"movl (%%ecx), %%ecx\n\t" \
				"pushl %2\n\t" \
				"pushl %%ecx\n\t" \
				"pushl %%eax\n\t" \
				"call _memcpy\n\t" \
				"addl $12, %%esp\n\t" \
				"movl %1, %%ecx\n\t" \
				"movl 8(%%ecx), %%edx\n\t" \
				"movl 4(%%ecx), %%ecx\n\t" \
				"movl %0, %%eax\n\t" \
				"call *%%eax\n\t" \
				"movl %3, %%ecx\n\t" \
				"movl %%eax, (%%ecx)\n\t" \
				"movl %%edx, 4(%%ecx)\n\t" \
				"movl %%esi, %%esp\n\t" \
				"popl %%esi\n\t" \
				: : "m"(__func), "m"(__args), "m"(__size), "m"(__return_buf) \
				: "eax", "ecx", "edx" \
			); \
			if((return_float)) \
			{ \
				if(sizeof(jit_nfloat) == sizeof(double)) \
				{ \
					__asm__ ( \
						"movl %0, %%ecx\n\t" \
						"fstpl 8(%%ecx)\n\t" \
						: : "m"(__return_buf) \
						: "ecx", "st" \
					); \
				} \
				else \
				{ \
					__asm__ ( \
						"movl %0, %%ecx\n\t" \
						"fstpt 8(%%ecx)\n\t" \
						: : "m"(__return_buf) \
						: "ecx", "st" \
					); \
				} \
			} \
		} while (0)

#define	jit_builtin_apply_args(type,args)	\
		do { \
			void *__args = alloca(12); \
			__asm__ ( \
				"movl %0, %%eax\n\t" \
				"movl %%ecx, 4(%%eax)\n\t" \
				"movl %%edx, 8(%%eax)\n\t" \
				"leal 8(%%ebp), %%ecx\n\t" \
				"movl %%ecx, (%%eax)\n\t" \
				: : "m"(__args) \
				: "eax", "ecx", "edx" \
			); \
			(args) = (type)__args; \
		} while (0)

#define	jit_builtin_return_int(return_buf)	\
		do { \
			__asm__ ( \
				"leal %0, %%ecx\n\t" \
				"movl (%%ecx), %%eax\n\t" \
				"movl 4(%%ecx), %%edx\n\t" \
				: : "m"(*(return_buf)) \
				: "eax", "ecx", "edx" \
			); \
			return; \
		} while (0)

#define	jit_builtin_return_float(return_buf)	\
		do { \
			jit_nfloat __value = \
				((jit_apply_return *)(return_buf))-> \
					nfloat_value.f_value; \
			if(sizeof(jit_nfloat) == sizeof(double)) \
			{ \
				__asm__ ( \
					"leal %0, %%ecx\n\t" \
					"fldl (%%ecx)\n\t" \
					: : "m"(__value) \
					: "ecx", "st" \
				); \
			} \
			else \
			{ \
				__asm__ ( \
					"leal %0, %%ecx\n\t" \
					"fldt (%%ecx)\n\t" \
					: : "m"(__value) \
					: "ecx", "st" \
				); \
			} \
			return; \
		} while (0)

#endif /* Win32 */

#elif defined(_MSC_VER)

#define	jit_builtin_apply(func,args,size,return_float,return_buf)	\
		do { \
			void *__func = (void *)(func); \
			void *__args = (void *)(args); \
			void *__size = (void *)(size); \
			void *__return_buf = alloca(20); \
			(return_buf) = __return_buf; \
			__asm { \
				__asm push esi \
				__asm mov esi, esp \
				__asm sub esp, dword ptr __size \
				__asm mov eax, esp \
				__asm mov ecx, dword ptr __args \
				__asm mov ecx, [ecx] \
				__asm push dword ptr __size \
				__asm push ecx \
				__asm push eax \
				__asm call jit_memcpy \
				__asm add esp, 12 \
				__asm mov ecx, dword ptr __args \
				__asm mov edx, [ecx + 8] \
				__asm mov ecx, [ecx + 4] \
				__asm mov eax, dword ptr __func \
				__asm call eax \
				__asm mov ecx, dword ptr __return_buf \
				__asm mov [ecx], eax \
				__asm mov [ecx + 4], edx \
				__asm mov esp, esi \
				__asm pop esi \
			} \
			if((return_float)) \
			{ \
				__asm { \
					__asm mov ecx, dword ptr __return_buf \
					/*__asm fstpl [ecx + 8]*/ \
					__asm _emit 0xDD \
					__asm _emit 0x59 \
					__asm _emit 0x08 \
				} \
			} \
		} while (0)

#define	jit_builtin_apply_args(type,args)	\
		do { \
			void *__args = alloca(12); \
			__asm { \
				__asm mov eax, dword ptr __args \
				__asm mov [eax + 4], ecx \
				__asm mov [eax + 8], edx \
				__asm lea ecx, [ebp + 8] \
				__asm mov [eax], ecx \
			} \
			(args) = (type)(__args); \
		} while (0)

#define	jit_builtin_return_int(return_buf)	\
		do { \
			void *__return_buf = (void *)(return_buf); \
			__asm { \
				__asm mov ecx, dword ptr __return_buf \
				__asm mov eax, [ecx] \
				__asm mov edx, [ecx + 4] \
			} \
			return; \
		} while (0)

#define	jit_builtin_return_float(return_buf)	\
		do { \
			double __dvalue = \
				((jit_apply_return *)(return_buf))-> \
					nfloat_value.f_value; \
			__asm { \
				__asm lea ecx, dword ptr __dvalue \
				/* __asm fldl [ecx] */ \
				__asm _emit 0xDD \
				__asm _emit 0x01 \
			} \
			return; \
		} while (0)

#endif /* MSC_VER */

#define	jit_builtin_return_double(return_buf)	\
	jit_builtin_return_float((return_buf))

#define	jit_builtin_return_nfloat(return_buf)	\
	jit_builtin_return_float((return_buf))

/*
 * The maximum number of bytes that are needed to represent a closure,
 * and the alignment to use for the closure.
 */
#define	jit_closure_size		64
#define	jit_closure_align		32

/*
 * The number of bytes that are needed for a redirector stub.
 * This includes any extra bytes that are needed for alignment.
 */
#define	jit_redirector_size		24

/*
 * The number of bytes that are needed for a indirector stub.
 * This includes any extra bytes that are needed for alignment.
 */
#define	jit_indirector_size		8

/*
 * We should pad unused code space with NOP's.
 */
#define	jit_should_pad			1

#endif	/* _JIT_APPLY_X86_H */
