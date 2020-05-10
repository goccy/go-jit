/*
 * jit-vmem.c - Virtual memory routines.
 *
 * Copyright (C) 2011  Aleksey Demakov
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

#include <jit/jit-vmem.h>
#include "jit-config.h"

#if defined(JIT_VMEM_WIN32)
# include <windows.h>
#elif defined(JIT_VMEM_MMAP)
# include <sys/mman.h>
#endif

#if !defined(JIT_WIN32_PLATFORM) && defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif

/*
 * Define getpagesize() if not provided
 */
#if !defined(JIT_WIN32_PLATFORM) && !defined(HAVE_GETPAGESIZE)
# if defined(NBPG)
#  define getpagesize() (NBPG)
# elif defined(PAGE_SIZE)
#  define getpagesize() (PAGE_SIZE)
# else
#  define getpagesize() (4096)
# endif
#endif

/*
 * Make sure that "MAP_ANONYMOUS" is correctly defined, because it
 * may not exist on some variants of Unix.
 */
#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
# define MAP_ANONYMOUS        MAP_ANON
#endif

static jit_uint page_size;

#if defined(JIT_VMEM_WIN32)
static DWORD
convert_prot(jit_prot_t prot)
{
	switch(prot)
	{
	case JIT_PROT_NONE:
		return PAGE_NOACCESS;
	case JIT_PROT_READ:
		return PAGE_READONLY;
	case JIT_PROT_READ_WRITE:
		return PAGE_READWRITE;
	case JIT_PROT_EXEC_READ:
		return PAGE_EXECUTE_READ;
	case JIT_PROT_EXEC_READ_WRITE:
		return PAGE_EXECUTE_READWRITE;
	}
	return PAGE_NOACCESS;
}
#elif defined(JIT_VMEM_MMAP)
static int
convert_prot(jit_prot_t prot)
{
	switch(prot)
	{
	case JIT_PROT_NONE:
		return PROT_NONE;
	case JIT_PROT_READ:
		return PROT_READ;
	case JIT_PROT_READ_WRITE:
		return PROT_READ | PROT_WRITE;
	case JIT_PROT_EXEC_READ:
		return PROT_EXEC | PROT_READ;
	case JIT_PROT_EXEC_READ_WRITE:
		return PROT_EXEC | PROT_READ | PROT_WRITE;
	}
	return PROT_NONE;
}
#endif

void
jit_vmem_init(void)
{
#if defined(JIT_VMEM_WIN32)
	/* Get the page size from a Windows-specific API */
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	page_size = (jit_uint) (sysInfo.dwPageSize);
#else
	/* Get the page size using a Unix-like sequence */
	page_size = (jit_uint) getpagesize();
#endif
}

/*@
 * @deftypefun {unsigned int} jit_vmem_page_size (void)
 * Get the page allocation size for the system.  This is the preferred
 * unit when making calls to @code{_jit_malloc_exec}.  It is not
 * required that you supply a multiple of this size when allocating,
 * but it can lead to better performance on some systems.
 * @end deftypefun
@*/
jit_uint
jit_vmem_page_size(void)
{
	return page_size;
}

jit_nuint
jit_vmem_round_up(jit_nuint value)
{
	return (value + page_size - 1) & ~(page_size - 1);
}

jit_nuint
jit_vmem_round_down(jit_nuint value)
{
	return ((jit_nuint) value) & ~(page_size - 1);
}

void *
jit_vmem_reserve(jit_uint size)
{
#if defined(JIT_VMEM_WIN32)

	return VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS);

#elif defined(JIT_VMEM_MMAP)

	void *addr;

	addr = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if(addr == MAP_FAILED)
	{
		return (void *) 0;
	}
	return addr;

#else
	return (void *) 0;
#endif
}

void *
jit_vmem_reserve_committed(jit_uint size, jit_prot_t prot)
{
#if defined(JIT_VMEM_WIN32)

	DWORD nprot;

	nprot = convert_prot(prot);
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, nprot);

#elif defined(JIT_VMEM_MMAP)

	void *addr;
	int nprot;

	nprot = convert_prot(prot);
	addr = mmap(0, size, nprot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(addr == MAP_FAILED)
	{
		return (void *) 0;
	}
	return addr;

#else
	return (void *) 0;
#endif
}

int
jit_vmem_release(void *addr, jit_uint size)
{
#if defined(JIT_VMEM_WIN32)

	return VirtualFree(addr, 0, MEM_RELEASE) != 0;

#elif defined(JIT_VMEM_MMAP)

	return munmap(addr, size) == 0;

#else
	return 0;
#endif
}

int
jit_vmem_commit(void *addr, jit_uint size, jit_prot_t prot)
{
#if defined(JIT_VMEM_WIN32)

	DWORD nprot;

	nprot = convert_prot(prot);
	return VirtualAlloc(addr, size, MEM_COMMIT, nprot) != 0;

#elif defined(JIT_VMEM_MMAP)

	int nprot;
	void *raddr;

	nprot = convert_prot(prot);
	raddr = mmap(addr, size, nprot, MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if(raddr != addr)
	{
		if(raddr != MAP_FAILED)
		{
			munmap(raddr, size);
		}
		return 0;
	}
	return 1;

#else
	return 0;
#endif
}

int
jit_vmem_decommit(void *addr, jit_uint size)
{
#if defined(JIT_VMEM_WIN32)

	return VirtualFree(addr, size, MEM_DECOMMIT) != 0;

#elif defined(JIT_VMEM_MMAP)
# if defined(MADV_FREE)

	int result = madvise(addr, size, MADV_FREE);
	if(result < 0)
	{
		return 0;
	}

# elif defined(MADV_DONTNEED) && defined(JIT_LINUX_PLATFORM)

	int result = madvise(addr, size, MADV_DONTNEED);
	if(result < 0)
	{
		return 0;
	}

# endif

	addr = mmap(addr, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if(addr == MAP_FAILED)
	{
		return 0;
	}
	return 1;

#else
	return 0;
#endif
}

int
jit_vmem_protect(void *addr, jit_uint size, jit_prot_t prot)
{
#if defined(JIT_VMEM_WIN32)

	DWORD nprot, oprot;

	nprot = convert_prot(prot);
	if(VirtualProtect(addr, size, nprot, &oprot) == 0)
	{
		return 0;
	}
	return 1;

#elif defined(JIT_VMEM_MMAP)

	int nprot;

	nprot = convert_prot(prot);
	if(mprotect(addr, size, nprot) < 0)
	{
		return 0;
	}
	return 1;

#else
	return 0;
#endif
}
