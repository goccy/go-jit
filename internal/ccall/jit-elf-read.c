/*
 * jit-elf-read.c - Routines to read ELF-format binaries.
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
#include "jit-rules.h"
#include "jit-elf-defs.h"
#ifdef JIT_WIN32_PLATFORM
	#ifdef HAVE_SYS_TYPES_H
		#include <sys/types.h>
	#endif
	#include <windows.h>
	#include <io.h>
	#include <fcntl.h>
#else
#ifdef HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef HAVE_SYS_MMAN_H
	#include <sys/mman.h>
	#if defined(HAVE_MMAP) && defined(HAVE_MUNMAP) && defined(HAVE_MPROTECT)
		#define	JIT_USE_MMAP_TO_LOAD 1
		#ifndef	MAP_ANON
			#ifdef MAP_ANONYMOUS
				#define	MAP_ANON	MAP_ANONYMOUS
			#else
				#define	MAP_ANON	0
			#endif
		#endif
		#ifndef MAP_FIXED
			#define	MAP_FIXED		0
		#endif
		#ifndef MAP_COPY
			#define	MAP_COPY		MAP_PRIVATE
		#endif
	#endif
#endif
#endif
#include <stdio.h>

/*@

The @code{libjit} library contains routines that permit pre-compiling
JIT'ed functions into an on-disk representation.  This representation
can be loaded at some future time, to avoid the overhead of compiling
the functions at runtime.

We use the ELF format for this purpose, which is a common binary format
used by modern operating systems and compilers.

It isn't necessary for your operating system to be based on ELF natively.
We use our own routines to read and write ELF binaries.  We chose ELF
because it has all of the features that we require, and reusing an
existing format was better than inventing a completely new one.

@section Reading ELF binaries

@*/

/*
 * Determine whether we should be using the 32-bit or 64-bit ELF structures.
 */
#ifdef JIT_NATIVE_INT32
	typedef Elf32_Ehdr  Elf_Ehdr;
	typedef Elf32_Shdr  Elf_Shdr;
	typedef Elf32_Phdr  Elf_Phdr;
	typedef Elf32_Addr  Elf_Addr;
	typedef Elf32_Word  Elf_Word;
	typedef Elf32_Xword Elf_Xword;
	typedef Elf32_Off   Elf_Off;
	typedef Elf32_Dyn   Elf_Dyn;
	typedef Elf32_Sym   Elf_Sym;
	typedef Elf32_Rel   Elf_Rel;
	typedef Elf32_Rela  Elf_Rela;
	#define ELF_R_SYM(val)	ELF32_R_SYM((val))
	#define ELF_R_TYPE(val)	ELF32_R_TYPE((val))
#else
	typedef Elf64_Ehdr  Elf_Ehdr;
	typedef Elf64_Shdr  Elf_Shdr;
	typedef Elf64_Phdr  Elf_Phdr;
	typedef Elf64_Addr  Elf_Addr;
	typedef Elf64_Word  Elf_Word;
	typedef Elf64_Xword Elf_Xword;
	typedef Elf64_Off   Elf_Off;
	typedef Elf64_Dyn   Elf_Dyn;
	typedef Elf64_Sym   Elf_Sym;
	typedef Elf64_Rel   Elf_Rel;
	typedef Elf64_Rela  Elf_Rela;
	#define ELF_R_SYM(val)	ELF64_R_SYM((val))
	#define ELF_R_TYPE(val)	ELF64_R_TYPE((val))
#endif

/*
 * Deal with platform differences in the file descriptor routines.
 */
#ifdef JIT_WIN32_PLATFORM
	#define sys_open		_open
	#define	sys_close		_close
	#define	sys_read		_read
	#define	sys_lseek		_lseek
#else
	#define sys_open		open
	#define	sys_close		close
	#define	sys_read		read
	#define	sys_lseek		lseek
#endif
#ifndef O_BINARY
	#define	O_BINARY		0
#endif

/*
 * Define the relocation function type.
 */
typedef int (*jit_reloc_func)(jit_readelf_t readelf, void *address,
							  int type, jit_nuint value, int has_addend,
							  jit_nuint addend);

/*
 * Get the relocation function for a particular machine type.
 */
static jit_reloc_func get_reloc(unsigned int machine);

/*
 * Structure of an ELF binary once it has been loaded into memory.
 */
struct jit_readelf
{
	jit_readelf_t	next;
	int				resolved;
	Elf_Ehdr		ehdr;
	unsigned char  *phdrs;
	unsigned char  *shdrs;
	char		   *regular_strings;
	jit_nuint		regular_strings_size;
	char		   *dynamic_strings;
	jit_nuint		dynamic_strings_size;
	Elf_Sym		   *symbol_table;
	jit_nuint		symbol_table_size;
	Elf_Word	   *symbol_hash;
	jit_nuint		symbol_hash_size;
	Elf_Word		symbol_hash_buckets;
	jit_reloc_func	reloc_func;
	void		   *map_address;
	jit_nuint		map_size;
	int				free_with_munmap;
};

/*
 * Flag that indicates that an auxillary section was malloc'ed,
 * and isn't part of the main memory range at "map_address".
 */
#define	JIT_ELF_IS_MALLOCED			0x01000000

/*
 * Get the address of a particular phdr.
 */
static Elf_Phdr *get_phdr(jit_readelf_t readelf, unsigned int index)
{
	if(index < readelf->ehdr.e_phnum &&
	   readelf->ehdr.e_phentsize >= sizeof(Elf_Phdr))
	{
		return (Elf_Phdr *)
			(readelf->phdrs +
			 index * ((unsigned int)(readelf->ehdr.e_phentsize)));
	}
	else
	{
		return 0;
	}
}

/*
 * Get the address of a particular shdr.
 */
static Elf_Shdr *get_shdr(jit_readelf_t readelf, unsigned int index)
{
	if(index < readelf->ehdr.e_shnum &&
	   readelf->ehdr.e_shentsize >= sizeof(Elf_Shdr))
	{
		return (Elf_Shdr *)
			(readelf->shdrs +
			 index * ((unsigned int)(readelf->ehdr.e_shentsize)));
	}
	else
	{
		return 0;
	}
}

/*
 * Find a specific string in the regular string table.
 */
static const char *get_string(jit_readelf_t readelf, Elf_Word _index)
{
	jit_nuint index = (jit_nuint)_index;
	if(index < readelf->regular_strings_size)
	{
		return readelf->regular_strings + index;
	}
	else
	{
		return 0;
	}
}

/*
 * Find a specific string in the dynamic string table.
 */
static const char *get_dyn_string(jit_readelf_t readelf, Elf_Addr _index)
{
	jit_nuint index = (jit_nuint)_index;
	if(index < readelf->dynamic_strings_size)
	{
		return readelf->dynamic_strings + index;
	}
	else
	{
		return 0;
	}
}

/*
 * Map all of the program segments into memory and set up the bss section.
 */
static int map_program(jit_readelf_t readelf, int fd)
{
	Elf_Off file_size;
	Elf_Off memory_size;
	Elf_Off start, end;
	Elf_Phdr *phdr;
	unsigned int index;
	void *base_address;
	unsigned char *segment_address;

	/* Get the maximum file and memory sizes for the program.
	   The bytes between "file_size" and "memory_size" are bss */
	file_size = 0;
	memory_size = 0;
	for(index = 0; index < readelf->ehdr.e_phnum; ++index)
	{
		phdr = get_phdr(readelf, index);
		if(!phdr)
		{
			continue;
		}
		start = phdr->p_offset;
		end = start + phdr->p_filesz;
		if(end > file_size)
		{
			file_size = end;
		}
		start = phdr->p_vaddr;
		end = start + phdr->p_memsz;
		if(end > memory_size)
		{
			memory_size = end;
		}
	}
	if(memory_size < file_size)
	{
		memory_size = file_size;
	}

	/* Try to map the program segments into memory using mmap */
	base_address = 0;
#ifdef JIT_USE_MMAP_TO_LOAD
	{
		Elf_Off page_size;
		Elf_Off rounded_file_size;
		Elf_Off temp_start;
		Elf_Off temp_end;
		int zero_fd, prot;

		/* Round the total memory and file sizes up to the CPU page size */
		page_size = (Elf_Off)(jit_vmem_page_size());
		end = memory_size;
		if((end % page_size) != 0)
		{
			end += page_size - (end % page_size);
		}
		rounded_file_size = file_size;
		if((rounded_file_size % page_size) != 0)
		{
			rounded_file_size += page_size - (rounded_file_size % page_size);
		}

		/* Allocate memory for the program from /dev/zero.  Once we have
		   the memory, we will overlay the program segments on top */
		zero_fd = sys_open("/dev/zero", O_RDWR, 0);
		if(zero_fd < -1)
		{
			goto failed_mmap;
		}
		base_address = mmap(0, (size_t)end, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, zero_fd, 0);
		close(zero_fd);
		if(base_address == (void *)(jit_nint)(-1))
		{
			base_address = 0;
			goto failed_mmap;
		}

		/* Lay down the program sections at their mapped locations */
		for(index = 0; index < readelf->ehdr.e_phnum; ++index)
		{
			phdr = get_phdr(readelf, index);
			if(phdr)
			{
				temp_start = phdr->p_offset;
				temp_end = temp_start + phdr->p_filesz;
				temp_start -= (temp_start % page_size);
				if((temp_end % page_size) != 0)
				{
					temp_end += page_size - (temp_end % page_size);
				}
				start = phdr->p_vaddr;
				start -= (start % page_size);
				if(temp_start < temp_end)
				{
					segment_address =
						((unsigned char *)base_address) + (jit_nuint)start;
					prot = 0;
					if((phdr->p_flags & PF_X) != 0)
					{
						prot |= PROT_EXEC;
					}
					if((phdr->p_flags & PF_W) != 0)
					{
						prot |= PROT_WRITE;
					}
					if((phdr->p_flags & PF_R) != 0)
					{
						prot |= PROT_READ;
					}
					if(mmap(segment_address, (size_t)(temp_end - temp_start),
						    prot, MAP_COPY | MAP_FILE | MAP_FIXED, fd,
							(off_t)temp_start) == (void *)(jit_nint)(-1))
					{
						munmap(base_address, (size_t)end);
						base_address = 0;
						goto failed_mmap;
					}
				}
			}
		}

		/* We need to free the memory with munmap when the program is closed */
		readelf->free_with_munmap = 1;

		/* Clear the left-over ".bss" bits that did not get cleared above */
		for(index = 0; index < readelf->ehdr.e_phnum; ++index)
		{
			phdr = get_phdr(readelf, index);
			if(phdr && phdr->p_filesz < phdr->p_memsz)
			{
				temp_start = phdr->p_vaddr + phdr->p_filesz;
				start = (temp_start % page_size);
				temp_start -= start;
				if(start != 0)
				{
					segment_address =
						((unsigned char *)base_address) +
						(jit_nuint)temp_start;
					mprotect(segment_address, (size_t)page_size,
							 PROT_READ | PROT_WRITE);
					jit_memzero(segment_address + (jit_nuint)start,
							    (unsigned int)(page_size - start));
					prot = 0;
					if((phdr->p_flags & PF_X) != 0)
					{
						prot |= PROT_EXEC;
					}
					if((phdr->p_flags & PF_W) != 0)
					{
						prot |= PROT_WRITE;
					}
					if((phdr->p_flags & PF_R) != 0)
					{
						prot |= PROT_READ;
					}
					mprotect(segment_address, (size_t)page_size, prot);
				}
			}
		}
	}
failed_mmap:
#endif /* JIT_USE_MMAP_TO_LOAD */

	/* If we haven't mapped the file yet, then fall back to "malloc" */
	if(!base_address)
	{
		base_address = _jit_malloc_exec(memory_size);
		if(!base_address)
		{
			return 0;
		}
		for(index = 0; index < readelf->ehdr.e_phnum; ++index)
		{
			phdr = get_phdr(readelf, index);
			if(phdr)
			{
				segment_address = ((unsigned char *)base_address) +
								  (jit_nuint)(phdr->p_vaddr);
				if(lseek(fd, (off_t)(phdr->p_offset), 0) !=
						(off_t)(phdr->p_offset) ||
	               read(fd, segment_address, (size_t)(phdr->p_filesz))
				   		!= (int)(size_t)(phdr->p_filesz))
				{
					_jit_free_exec(base_address, memory_size);
					return 0;
				}
			}
		}
	}

	/* Record the mapped address and size for later */
	readelf->map_address = base_address;
	readelf->map_size = memory_size;
	return 1;
}

/*
 * Map an auxillary section into memory and return its base address.
 * Returns NULL if we ran out of memory.
 */
static void *map_section(int fd, Elf_Off offset, Elf_Xword file_size,
						 Elf_Xword memory_size, Elf_Word flags)
{
	void *address;
	if(memory_size < file_size)
	{
		memory_size = file_size;
	}
	address = _jit_malloc_exec(memory_size);
	if(!address)
	{
		return 0;
	}
	if(lseek(fd, (off_t)offset, 0) != (off_t)offset)
	{
		_jit_free_exec(address, memory_size);
		return 0;
	}
	if(read(fd, address, (size_t)file_size) != (int)(size_t)file_size)
	{
		_jit_free_exec(address, memory_size);
		return 0;
	}
	return address;
}

/*
 * Unmap an auxillary section from memory.
 */
static void unmap_section(void *address, Elf_Xword file_size,
						  Elf_Xword memory_size, Elf_Word flags)
{
	if(memory_size < file_size)
	{
		memory_size = file_size;
	}
	if((flags & JIT_ELF_IS_MALLOCED) != 0)
	{
		_jit_free_exec(address, (unsigned int)memory_size);
	}
}

/*
 * Iterate over the contents of the ".dynamic" section.
 */
typedef struct
{
	Elf_Dyn    *dyn;
	jit_nuint	size;

} jit_dynamic_iter_t;
static void dynamic_iter_init(jit_dynamic_iter_t *iter, jit_readelf_t readelf)
{
	iter->dyn = jit_readelf_get_section_by_type
		(readelf, SHT_DYNAMIC, &(iter->size));
}
static int dynamic_iter_next
	(jit_dynamic_iter_t *iter, jit_uint *type, Elf_Addr *value)
{
	if(iter->size >= sizeof(Elf_Dyn))
	{
		*type = (jit_uint)(iter->dyn->d_tag);
		*value = iter->dyn->d_un.d_ptr;
		if(*type == DT_NULL)
		{
			/* Explicitly-marked end of the list */
			return 0;
		}
		++(iter->dyn);
		iter->size -= sizeof(Elf_Dyn);
		return 1;
	}
	else
	{
		/* Implicitly-marked end of the list */
		return 0;
	}
}
static int dynamic_for_type
	(jit_readelf_t readelf, jit_uint type, Elf_Addr *value)
{
	Elf_Addr temp_value;
	jit_dynamic_iter_t iter;
	jit_uint iter_type;
	dynamic_iter_init(&iter, readelf);
	while(dynamic_iter_next(&iter, &iter_type, &temp_value))
	{
		if(iter_type == type)
		{
			if(value)
			{
				*value = temp_value;
			}
			return 1;
		}
	}
	return 0;
}

/*
 * Load interesting values from the ".dynamic" section, for quicker lookups.
 */
static void load_dynamic_section(jit_readelf_t readelf, int flags)
{
	Elf_Addr value;
	Elf_Addr value2;
	jit_dynamic_iter_t iter;
	jit_uint type;
	jit_nuint size;

	/* Get the position and size of the dynamic string table */
	if(dynamic_for_type(readelf, DT_STRTAB, &value) &&
	   dynamic_for_type(readelf, DT_STRSZ, &value2))
	{
		readelf->dynamic_strings = jit_readelf_map_vaddr
			(readelf, (jit_nuint)value);
		if(readelf->dynamic_strings)
		{
			readelf->dynamic_strings_size = (jit_nuint)value2;
		}
	}

	/* Get the position and size of the dynamic symbol table */
	readelf->symbol_table = jit_readelf_get_section_by_type
		(readelf, SHT_DYNSYM, &size);
	if(readelf->symbol_table)
	{
		if(dynamic_for_type(readelf, DT_SYMENT, &value) &&
		   value == sizeof(Elf_Sym))
		{
			readelf->symbol_table_size = size / sizeof(Elf_Sym);
			readelf->symbol_hash = jit_readelf_get_section_by_type
				(readelf, SHT_HASH, &size);
			if(readelf->symbol_hash)
			{
				readelf->symbol_hash_size = size / sizeof(Elf_Word);
				if(readelf->symbol_hash_size >= 2)
				{
					readelf->symbol_hash_buckets = readelf->symbol_hash[0];
				}
			}
		}
		else
		{
			readelf->symbol_table = 0;
		}
	}

	/* Bail out if we don't need to print debugging information */
	if((flags & JIT_READELF_FLAG_DEBUG) == 0)
	{
		return;
	}

	/* Iterate through the ".dynamic" section, dumping all that we find */
	dynamic_iter_init(&iter, readelf);
	while(dynamic_iter_next(&iter, &type, &value))
	{
		switch(type)
		{
			case DT_NEEDED:
			{
				printf("needed library: %s\n", get_dyn_string(readelf, value));
			}
			break;

			case DT_PLTRELSZ:
			{
				printf("total size of PLT relocs: %ld\n", (long)value);
			}
			break;

			case DT_PLTGOT:
			{
				printf("address of PLTGOT table: 0x%lx\n", (long)value);
			}
			break;

			case DT_HASH:
			{
				printf("address of symbol hash table: 0x%lx\n", (long)value);
			}
			break;

			case DT_STRTAB:
			{
				printf("address of string table: 0x%lx\n", (long)value);
			}
			break;

			case DT_SYMTAB:
			{
				printf("address of symbol table: 0x%lx\n", (long)value);
			}
			break;

			case DT_STRSZ:
			{
				printf("size of string table: %ld\n", (long)value);
			}
			break;

			case DT_SYMENT:
			{
				printf("size of one symbol table entry: %ld\n", (long)value);
			}
			break;

			case DT_INIT:
			{
				printf("address of init function: 0x%lx\n", (long)value);
			}
			break;

			case DT_FINI:
			{
				printf("address of fini function: 0x%lx\n", (long)value);
			}
			break;

			case DT_SONAME:
			{
				printf("library name: %s\n", get_dyn_string(readelf, value));
			}
			break;

			case DT_REL:
			{
				printf("address of Rel relocs: 0x%lx\n", (long)value);
			}
			break;

			case DT_RELSZ:
			{
				printf("total size of Rel relocs: %ld\n", (long)value);
			}
			break;

			case DT_RELENT:
			{
				printf("size of one Rel reloc: %ld\n", (long)value);
			}
			break;

			case DT_RELA:
			{
				printf("address of Rela relocs: 0x%lx\n", (long)value);
			}
			break;

			case DT_RELASZ:
			{
				printf("total size of Rela relocs: %ld\n", (long)value);
			}
			break;

			case DT_RELAENT:
			{
				printf("size of one Rela reloc: %ld\n", (long)value);
			}
			break;

			case DT_PLTREL:
			{
				printf("type of PLT relocs: %ld\n", (long)value);
			}
			break;

			case DT_JMPREL:
			{
				printf("address of PLT relocs: 0x%lx\n", (long)value);
			}
			break;

			default:
			{
				printf("dynamic info of type 0x%x: 0x%lx\n",
					   (int)type, (long)value);
			}
			break;
		}
	}

	/* Iterate through the symbol table, dumping all of the entries */
	for(size = 0; size < readelf->symbol_table_size; ++size)
	{
		printf("%08lX %02X%02X %2d %s\n",
			   (long)(readelf->symbol_table[size].st_value),
			   (int)(readelf->symbol_table[size].st_info),
			   (int)(readelf->symbol_table[size].st_other),
			   (int)(readelf->symbol_table[size].st_shndx),
			   get_dyn_string(readelf, readelf->symbol_table[size].st_name));
	}
	printf("number of symbols: %ld\n", (long)(readelf->symbol_table_size));
	printf("number of symbol hash entries: %ld\n",
		   (long)(readelf->symbol_hash_size));
}

/*@
 * @deftypefun int jit_readelf_open (jit_readelf_t *@var{readelf}, const char *@var{filename}, int @var{force})
 * Open the specified @var{filename} and load the ELF binary that is
 * contained within it.  Returns one of the following result codes:
 *
 * @table @code
 * @vindex JIT_READELF_OK
 * @item JIT_READELF_OK
 * The ELF binary was opened successfully.
 *
 * @vindex JIT_READELF_CANNOT_OPEN
 * @item JIT_READELF_CANNOT_OPEN
 * Could not open the file at the filesystem level (reason in @code{errno}).
 *
 * @vindex JIT_READELF_NOT_ELF
 * @item JIT_READELF_NOT_ELF
 * The file was opened, but it is not an ELF binary.
 *
 * @vindex JIT_READELF_WRONG_ARCH
 * @item JIT_READELF_WRONG_ARCH
 * The file is an ELF binary, but it does not pertain to the architecture
 * of this machine.
 *
 * @vindex JIT_READELF_BAD_FORMAT
 * @item JIT_READELF_BAD_FORMAT
 * The file is an ELF binary, but the format is corrupted in some fashion.
 *
 * @vindex JIT_READELF_MEMORY
 * @item JIT_READELF_MEMORY
 * There is insufficient memory to open the ELF binary.
 * @end table
 *
 * The following flags may be supplied to alter the manner in which
 * the ELF binary is loaded:
 *
 * @table @code
 * @vindex JIT_READELF_FLAG_FORCE
 * @item JIT_READELF_FLAG_FORCE
 * Force @code{jit_readelf_open} to open the ELF binary, even if
 * the architecture does not match this machine.  Useful for debugging.
 *
 * @vindex JIT_READELF_FLAG_DEBUG
 * @item JIT_READELF_FLAG_DEBUG
 * Print additional debug information to stdout.
 * @end table
 * @end deftypefun
@*/
int jit_readelf_open(jit_readelf_t *_readelf, const char *filename, int flags)
{
	int fd;
	Elf_Ehdr ehdr;
	Elf_Phdr *phdr;
	Elf_Shdr *shdr;
	jit_elf_info_t elf_info;
	jit_readelf_t readelf;
	unsigned int phdr_size;
	unsigned int shdr_size;
	unsigned int index;
	void *address;
	union
	{
		jit_ushort value;
		unsigned char bytes[2];

	} un;

	/* Get the machine and ABI values that we expect in the header */
	_jit_gen_get_elf_info(&elf_info);

	/* Open the file and read the ELF magic number information */
	if((fd = sys_open(filename, O_RDONLY | O_BINARY, 0)) < 0)
	{
		return JIT_READELF_CANNOT_OPEN;
	}
	if(sys_read(fd, ehdr.e_ident, EI_NIDENT) != EI_NIDENT)
	{
		sys_close(fd);
		return JIT_READELF_NOT_ELF;
	}

	/* Determine if the magic number matches what we expect to see */
	if(ehdr.e_ident[EI_MAG0] != ELFMAG0 || ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
	   ehdr.e_ident[EI_MAG2] != ELFMAG2 || ehdr.e_ident[EI_MAG3] != ELFMAG3)
	{
		sys_close(fd);
		return JIT_READELF_NOT_ELF;
	}
#ifdef JIT_NATIVE_INT32
	if(ehdr.e_ident[EI_CLASS] != ELFCLASS32)
	{
		sys_close(fd);
		return JIT_READELF_WRONG_ARCH;
	}
#else
	if(ehdr.e_ident[EI_CLASS] != ELFCLASS64)
	{
		sys_close(fd);
		return JIT_READELF_WRONG_ARCH;
	}
#endif
	un.value = 0x0102;
	if(un.bytes[0] == 0x01)
	{
		/* Looking for a big-endian binary */
		if(ehdr.e_ident[EI_DATA] != ELFDATA2MSB)
		{
			sys_close(fd);
			return JIT_READELF_WRONG_ARCH;
		}
	}
	else
	{
		/* Looking for a little-endian binary */
		if(ehdr.e_ident[EI_DATA] != ELFDATA2LSB)
		{
			sys_close(fd);
			return JIT_READELF_WRONG_ARCH;
		}
	}
	if(ehdr.e_ident[EI_VERSION] != EV_CURRENT)
	{
		sys_close(fd);
		return JIT_READELF_BAD_FORMAT;
	}

	/* Read the rest of the ELF header and validate it */
	if(sys_read(fd, &(ehdr.e_type), sizeof(Elf_Ehdr) - EI_NIDENT)
			!= (sizeof(Elf_Ehdr) - EI_NIDENT))
	{
		sys_close(fd);
		return JIT_READELF_BAD_FORMAT;
	}
	if(ehdr.e_type != ET_DYN)
	{
		/* We can only load files that are marked as dynamic shared objects */
		sys_close(fd);
		return JIT_READELF_WRONG_ARCH;
	}
	if((flags & JIT_READELF_FLAG_FORCE) == 0)
	{
		if(ehdr.e_machine != elf_info.machine ||
		   ehdr.e_ident[EI_OSABI] != elf_info.abi ||
		   ehdr.e_ident[EI_ABIVERSION] != elf_info.abi_version)
		{
			/* The ELF binary does not pertain to this machine or ABI type */
			sys_close(fd);
			return JIT_READELF_WRONG_ARCH;
		}
	}
	if(ehdr.e_version != EV_CURRENT)
	{
		sys_close(fd);
		return JIT_READELF_BAD_FORMAT;
	}
	if(ehdr.e_ehsize < sizeof(ehdr))
	{
		sys_close(fd);
		return JIT_READELF_BAD_FORMAT;
	}

	/* Allocate space for the ELF reader object */
	if((readelf = jit_cnew(struct jit_readelf)) == 0)
	{
		sys_close(fd);
		return JIT_READELF_MEMORY;
	}
	readelf->ehdr = ehdr;
	phdr_size = ((unsigned int)(ehdr.e_phnum)) *
				((unsigned int)(ehdr.e_phentsize));
	shdr_size = ((unsigned int)(ehdr.e_shnum)) *
				((unsigned int)(ehdr.e_shentsize));
	if(phdr_size > 0)
	{
		readelf->phdrs = (unsigned char *)jit_malloc(phdr_size);
		if(!(readelf->phdrs))
		{
			jit_free(readelf);
			sys_close(fd);
			return JIT_READELF_MEMORY;
		}
	}
	if(shdr_size > 0)
	{
		readelf->shdrs = (unsigned char *)jit_malloc(shdr_size);
		if(!(readelf->shdrs))
		{
			jit_free(readelf->phdrs);
			jit_free(readelf);
			sys_close(fd);
			return JIT_READELF_MEMORY;
		}
	}

	/* Seek to the program and section header tables and read them */
	if(phdr_size > 0)
	{
		if(lseek(fd, (off_t)(ehdr.e_phoff), 0) != (off_t)(ehdr.e_phoff) ||
		   read(fd, readelf->phdrs, phdr_size) != (int)phdr_size)
		{
			jit_free(readelf->shdrs);
			jit_free(readelf->phdrs);
			jit_free(readelf);
			sys_close(fd);
			return JIT_READELF_BAD_FORMAT;
		}
	}
	if(shdr_size > 0)
	{
		if(lseek(fd, (off_t)(ehdr.e_shoff), 0) != (off_t)(ehdr.e_shoff) ||
		   read(fd, readelf->shdrs, shdr_size) != (int)shdr_size)
		{
			jit_free(readelf->shdrs);
			jit_free(readelf->phdrs);
			jit_free(readelf);
			sys_close(fd);
			return JIT_READELF_BAD_FORMAT;
		}
	}

	/* Load the program segments */
	if(!map_program(readelf, fd))
	{
		jit_readelf_close(readelf);
		sys_close(fd);
		return JIT_READELF_MEMORY;
	}

	/* Load the auxillary sections */
	if(shdr_size > 0)
	{
		for(index = 0; index < ehdr.e_shnum; ++index)
		{
			shdr = get_shdr(readelf, index);
			if(!shdr)
			{
				continue;
			}
			if((shdr->sh_flags & SHF_ALLOC) != 0 || shdr->sh_addr != 0)
			{
				/* This may be mapped inside one of the program segments.
				   If so, we don't want to load a second copy of it */
				address = jit_readelf_map_vaddr(readelf, shdr->sh_addr);
				if(address)
				{
					continue;
				}
			}
			if(shdr->sh_size == 0)
			{
				/* Ignore zero-sized segments */
				continue;
			}
			address = map_section
				(fd, shdr->sh_offset, shdr->sh_size, shdr->sh_size,
				 ((shdr->sh_flags & SHF_WRITE) != 0 ? (PF_W | PF_R) : PF_R));
			if(!address)
			{
				jit_readelf_close(readelf);
				sys_close(fd);
				return JIT_READELF_MEMORY;
			}
			shdr->sh_offset = (Elf_Off)(jit_nuint)address;
			shdr->sh_flags |= JIT_ELF_IS_MALLOCED;
		}
	}

	/* Close the file descriptor because we don't need it any more */
	sys_close(fd);

	/* Find the regular string table */
	shdr = get_shdr(readelf, ehdr.e_shstrndx);
	if(shdr)
	{
		if((shdr->sh_flags & JIT_ELF_IS_MALLOCED) != 0)
		{
			readelf->regular_strings = (char *)(jit_nuint)(shdr->sh_offset);
		}
		else
		{
			readelf->regular_strings =
				(char *)jit_readelf_map_vaddr(readelf, shdr->sh_addr);
		}
		if(readelf->regular_strings)
		{
			readelf->regular_strings_size = (jit_nuint)(shdr->sh_size);
		}
	}

	/* Dump debug information about the program segments and sections */
	if((flags & JIT_READELF_FLAG_DEBUG) != 0)
	{
		printf("header: machine=%d, abi=%d, abi_version=%d\n",
			   (int)(ehdr.e_machine), (int)(ehdr.e_ident[EI_OSABI]),
			   (int)(ehdr.e_ident[EI_ABIVERSION]));
		for(index = 0; index < ehdr.e_phnum; ++index)
		{
			phdr = get_phdr(readelf, index);
			if(phdr)
			{
				printf("program segment: type=%d, flags=0x%x, "
							"vaddr=0x%lx, file_size=%ld, memory_size=%ld\n",
				       (int)(phdr->p_type),
				       (int)(phdr->p_flags & ~JIT_ELF_IS_MALLOCED),
				       (long)(phdr->p_vaddr),
				       (long)(phdr->p_filesz),
				       (long)(phdr->p_memsz));
			}
		}
		for(index = 0; index < ehdr.e_shnum; ++index)
		{
			shdr = get_shdr(readelf, index);
			if(shdr)
			{
				printf("section %2d: name=\"%s\", type=%d, flags=0x%x, "
							"vaddr=0x%lx, size=%ld\n",
					   index,
				       get_string(readelf, shdr->sh_name),
				       (int)(shdr->sh_type),
				       (int)(shdr->sh_flags & ~JIT_ELF_IS_MALLOCED),
				           (long)(shdr->sh_addr),
				       (long)(shdr->sh_size));
			}
		}
	}

	/* Get the relocation function for this machine type */
	readelf->reloc_func = get_reloc((unsigned int)(ehdr.e_machine));

	/* Load useful values from the dynamic section that we want to cache */
	load_dynamic_section(readelf, flags);

	/* The ELF binary is loaded and ready to go */
	*_readelf = readelf;
	return JIT_READELF_OK;
}

/*@
 * @deftypefun void jit_readelf_close (jit_readelf_t @var{readelf})
 * Close an ELF reader, reclaiming all of the memory that was used.
 * @end deftypefun
@*/
void jit_readelf_close(jit_readelf_t readelf)
{
	unsigned int index;
	Elf_Shdr *shdr;
	if(!readelf)
	{
		return;
	}
#ifdef JIT_USE_MMAP_TO_LOAD
	if(readelf->free_with_munmap)
	{
		munmap(readelf->map_address, (size_t)(readelf->map_size));
	}
	else
#endif
	{
		_jit_free_exec(readelf->map_address, readelf->map_size);
	}
	for(index = 0; index < readelf->ehdr.e_shnum; ++index)
	{
		shdr = get_shdr(readelf, index);
		if(shdr && (shdr->sh_flags & JIT_ELF_IS_MALLOCED) != 0)
		{
			unmap_section
				((void *)(jit_nuint)(shdr->sh_offset),
				 shdr->sh_size, shdr->sh_size, shdr->sh_flags);
		}
	}
	jit_free(readelf->phdrs);
	jit_free(readelf->shdrs);
	jit_free(readelf);
}

/*@
 * @deftypefun {const char *} jit_readelf_get_name (jit_readelf_t @var{readelf})
 * Get the library name that is embedded inside an ELF binary.
 * ELF binaries can refer to each other using this name.
 * @end deftypefun
@*/
const char *jit_readelf_get_name(jit_readelf_t readelf)
{
	Elf_Addr value;
	if(dynamic_for_type(readelf, DT_SONAME, &value))
	{
		return get_dyn_string(readelf, value);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun void *jit_readelf_get_symbol (jit_readelf_t @var{readelf}, const char *@var{name})
 * Look up the symbol called @var{name} in the ELF binary represented
 * by @var{readelf}.  Returns NULL if the symbol is not present.
 *
 * External references from this ELF binary to others are not resolved
 * until the ELF binary is loaded into a JIT context using
 * @code{jit_readelf_add_to_context} and @code{jit_readelf_resolve_all}.
 * You should not call functions within this ELF binary until after you
 * have fully resolved it.
 * @end deftypefun
@*/
void *jit_readelf_get_symbol(jit_readelf_t readelf, const char *name)
{
	unsigned long hash;
	unsigned long temp;
	unsigned int index;
	jit_nuint num_symbols;
	Elf_Sym *symbol;
	const char *symbol_name;

	/* Bail out if we have insufficient information to resolve the name */
	if(!readelf || !name || !(readelf->symbol_table))
	{
		return 0;
	}

	/* Hash the name to get the starting index in the symbol hash */
	hash = 0;
	index = 0;
	while(name[index] != 0)
	{
		hash = (hash << 4) + (unsigned long)(name[index] & 0xFF);
		temp = (hash & 0xF0000000);
		if(temp != 0)
		{
			hash ^= temp | (temp >> 24);
		}
		++index;
	}

	/* Look in the hash table for the name */
	if(readelf->symbol_hash_buckets != 0)
	{
		hash %= (unsigned long)(readelf->symbol_hash_buckets);
		temp = (unsigned long)(readelf->symbol_hash[hash + 2]);
		while(temp != 0 && temp < readelf->symbol_table_size)
		{
			symbol = &(readelf->symbol_table[temp]);
			symbol_name = get_dyn_string(readelf, symbol->st_name);
			if(symbol_name && !jit_strcmp(symbol_name, name))
			{
				/* Ignore symbols in section 0, as they are external */
				if(symbol->st_shndx)
				{
					return jit_readelf_map_vaddr
						(readelf, (jit_nuint)(symbol->st_value));
				}
				break;
			}
			temp = (unsigned long)(readelf->symbol_hash
				[temp + readelf->symbol_hash_buckets + 2]);
		}
		return 0;
	}

	/* There is no hash table, so search for the symbol the hard way */
	symbol = readelf->symbol_table;
	for(num_symbols = readelf->symbol_table_size;
		num_symbols > 0; --num_symbols)
	{
		symbol_name = get_dyn_string(readelf, symbol->st_name);
		if(symbol_name && !jit_strcmp(symbol_name, name))
		{
			/* Ignore symbols in section 0, as they are external */
			if(symbol->st_shndx)
			{
				return jit_readelf_map_vaddr
					(readelf, (jit_nuint)(symbol->st_value));
			}
		}
		++symbol;
	}
	return 0;
}

/*@
 * @deftypefun {void *} jit_readelf_get_section (jit_readelf_t @var{readelf}, const char *@var{name}, jit_nuint *@var{size})
 * Get the address and size of a particular section from an ELF binary.
 * Returns NULL if the section is not present in the ELF binary.
 *
 * The virtual machine may have stored auxillary information
 * in the section when the binary was first generated.  This function
 * allows the virtual machine to retrieve its auxillary information.
 *
 * Examples of such information may be version numbers, timestamps,
 * checksums, and other identifying information for the bytecode that
 * was previously compiled by the virtual machine.  The virtual machine
 * can use this to determine if the ELF binary is up to date and
 * relevant to its needs.
 *
 * It is recommended that virtual machines prefix their special sections
 * with a unique string (e.g. @code{.foovm}) to prevent clashes with
 * system-defined section names.  The prefix @code{.libjit} is reserved
 * for use by @code{libjit} itself.
 * @end deftypefun
@*/
void *jit_readelf_get_section
	(jit_readelf_t readelf, const char *name, jit_nuint *size)
{
	unsigned int index;
	Elf_Shdr *shdr;
	const char *temp_name;
	if(!readelf || !name)
	{
		return 0;
	}
	for(index = 0; index < readelf->ehdr.e_shnum; ++index)
	{
		shdr = get_shdr(readelf, index);
		if(shdr)
		{
			temp_name = get_string(readelf, shdr->sh_name);
			if(temp_name && !jit_strcmp(name, temp_name))
			{
				if(size)
				{
					*size = (jit_nuint)(shdr->sh_size);
				}
				if((shdr->sh_flags & JIT_ELF_IS_MALLOCED) != 0)
				{
					return (void *)(jit_nuint)(shdr->sh_offset);
				}
				else
				{
					return jit_readelf_map_vaddr
						(readelf, (jit_nuint)(shdr->sh_addr));
				}
			}
		}
	}
	return 0;
}

/*@
 * @deftypefun {void *} jit_readelf_get_section_by_type (jit_readelf_t @var{readelf}, jit_int @var{type}, jit_nuint *@var{size})
 * Get a particular section using its raw ELF section type (i.e. one of
 * the @code{SHT_*} constants in @code{jit-elf-defs.h}).  This is mostly
 * for internal use, but some virtual machines may find it useful for
 * debugging purposes.
 * @end deftypefun
@*/
void *jit_readelf_get_section_by_type
	(jit_readelf_t readelf, jit_int type, jit_nuint *size)
{
	unsigned int index;
	Elf_Shdr *shdr;
	if(!readelf)
	{
		return 0;
	}
	for(index = 0; index < readelf->ehdr.e_shnum; ++index)
	{
		shdr = get_shdr(readelf, index);
		if(shdr && type == (jit_int)(shdr->sh_type))
		{
			if(size)
			{
				*size = (jit_nuint)(shdr->sh_size);
			}
			if((shdr->sh_flags & JIT_ELF_IS_MALLOCED) != 0)
			{
				return (void *)(jit_nuint)(shdr->sh_offset);
			}
			else
			{
				return jit_readelf_map_vaddr
					(readelf, (jit_nuint)(shdr->sh_addr));
			}
		}
	}
	return 0;
}

/*@
 * @deftypefun {void *} jit_readelf_map_vaddr (jit_readelf_t @var{readelf}, jit_nuint @var{vaddr})
 * Map a virtual address to an actual address in a loaded ELF binary.
 * Returns NULL if @var{vaddr} could not be mapped.
 * @end deftypefun
@*/
void *jit_readelf_map_vaddr(jit_readelf_t readelf, jit_nuint vaddr)
{
	unsigned int index;
	Elf_Phdr *phdr;
	if(!readelf)
	{
		return 0;
	}
	for(index = 0; index < readelf->ehdr.e_phnum; ++index)
	{
		phdr = get_phdr(readelf, index);
		if(phdr && vaddr >= phdr->p_vaddr &&
		   vaddr < (phdr->p_vaddr + phdr->p_memsz))
		{
			return (void *)(((unsigned char *)(readelf->map_address)) + vaddr);
		}
	}
	return 0;
}

/*@
 * @deftypefun {unsigned int} jit_readelf_num_needed (jit_readelf_t @var{readelf})
 * Get the number of dependent libraries that are needed by this
 * ELF binary.  The virtual machine will normally need to arrange
 * to load these libraries with @code{jit_readelf_open} as well,
 * so that all of the necessary symbols can be resolved.
 * @end deftypefun
@*/
unsigned int jit_readelf_num_needed(jit_readelf_t readelf)
{
	jit_dynamic_iter_t iter;
	unsigned int count = 0;
	jit_uint type;
	Elf_Addr value;
	dynamic_iter_init(&iter, readelf);
	while(dynamic_iter_next(&iter, &type, &value))
	{
		if(type == DT_NEEDED)
		{
			++count;
		}
	}
	return count;
}

/*@
 * @deftypefun {const char *} jit_readelf_get_needed (jit_readelf_t @var{readelf}, unsigned int @var{index})
 * Get the name of the dependent library at position @var{index} within
 * the needed libraries list of this ELF binary.  Returns NULL if
 * the @var{index} is invalid.
 * @end deftypefun
@*/
const char *jit_readelf_get_needed(jit_readelf_t readelf, unsigned int index)
{
	jit_dynamic_iter_t iter;
	jit_uint type;
	Elf_Addr value;
	dynamic_iter_init(&iter, readelf);
	while(dynamic_iter_next(&iter, &type, &value))
	{
		if(type == DT_NEEDED)
		{
			if(index == 0)
			{
				return get_dyn_string(readelf, value);
			}
			--index;
		}
	}
	return 0;
}

/*@
 * @deftypefun void jit_readelf_add_to_context (jit_readelf_t @var{readelf}, jit_context_t @var{context})
 * Add this ELF binary to a JIT context, so that its contents can be used
 * when executing JIT-managed code.  The binary will be closed automatically
 * if the context is destroyed and @code{jit_readelf_close} has not been
 * called explicitly yet.
 *
 * The functions in the ELF binary cannot be used until you also call
 * @code{jit_readelf_resolve_all} to resolve cross-library symbol references.
 * The reason why adding and resolution are separate steps is to allow for
 * resolving circular dependencies between ELF binaries.
 * @end deftypefun
@*/
void jit_readelf_add_to_context(jit_readelf_t readelf, jit_context_t context)
{
	if(!readelf || !context)
	{
		return;
	}

	_jit_memory_lock(context);

	readelf->next = context->elf_binaries;
	context->elf_binaries = readelf;

	_jit_memory_unlock(context);
}

/*
 * Import the internal symbol table from "jit-symbol.c".
 */
typedef struct
{
	const char *name;
	void       *value;

} jit_internalsym;
extern jit_internalsym const _jit_internal_symbols[];
extern int const _jit_num_internal_symbols;

/*
 * Resolve a symbol to an address.
 */
static void *resolve_symbol
	(jit_context_t context, jit_readelf_t readelf,
	 int print_failures, const char *name, jit_nuint symbol)
{
	Elf_Sym *sym;
	void *value;
	const char *symbol_name;
	jit_readelf_t library;
	int index, left, right, cmp;

	/* Find the actual symbol details */
	if(symbol >= readelf->symbol_table_size)
	{
		if(print_failures)
		{
			printf("%s: invalid symbol table index %lu\n",
				   name, (unsigned long)symbol);
		}
		return 0;
	}
	sym = &(readelf->symbol_table[symbol]);

	/* Does the symbol have a locally-defined value? */
	if(sym->st_value)
	{
		value = jit_readelf_map_vaddr(readelf, (jit_nuint)(sym->st_value));
		if(!value)
		{
			if(print_failures)
			{
				printf("%s: could not map virtual address 0x%lx\n",
					   name, (long)(sym->st_value));
			}
		}
		return value;
	}

	/* Get the symbol's name, so that we can look it up in other libraries */
	symbol_name = get_dyn_string(readelf, sym->st_name);
	if(!symbol_name)
	{
		if(print_failures)
		{
			printf("%s: symbol table index %lu does not have a valid name\n",
				   name, (unsigned long)symbol);
		}
		return 0;
	}

	/* Look for "before" symbols that are registered with the context */
	for(index = 0; index < context->num_registered_symbols; ++index)
	{
		if(!jit_strcmp(symbol_name, context->registered_symbols[index]->name) &&
		   !(context->registered_symbols[index]->after))
		{
			return context->registered_symbols[index]->value;
		}
	}

	/* Search all loaded ELF libraries for the name */
	library = context->elf_binaries;
	while(library != 0)
	{
		value = jit_readelf_get_symbol(library, symbol_name);
		if(value)
		{
			return value;
		}
		library = library->next;
	}

	/* Look for libjit internal symbols (i.e. intrinsics) */
	left = 0;
	right = _jit_num_internal_symbols - 1;
	while(left <= right)
	{
		index = (left + right) / 2;
		cmp = jit_strcmp(symbol_name, _jit_internal_symbols[index].name);
		if(cmp == 0)
		{
			return _jit_internal_symbols[index].value;
		}
		else if(cmp < 0)
		{
			right = index - 1;
		}
		else
		{
			left = index + 1;
		}
	}

	/* Look for "after" symbols that are registered with the context */
	for(index = 0; index < context->num_registered_symbols; ++index)
	{
		if(!jit_strcmp(symbol_name, context->registered_symbols[index]->name) &&
		   context->registered_symbols[index]->after)
		{
			return context->registered_symbols[index]->value;
		}
	}

	/* If we get here, then we could not resolve the symbol */
	printf("%s: could not resolve `%s'\n", name, symbol_name);
	return 0;
}

/*
 * Perform a DT_REL style relocation on an ELF binary.
 */
static int perform_rel
	(jit_context_t context, jit_readelf_t readelf, 
	 int print_failures, const char *name, Elf_Rel *reloc)
{
	void *address;
	void *value;

	/* Get the address to apply the relocation at */
	address = jit_readelf_map_vaddr(readelf, (jit_nuint)(reloc->r_offset));
	if(!address)
	{
		if(print_failures)
		{
			printf("%s: cannot map virtual address 0x%lx\n",
				   name, (long)(reloc->r_offset));
		}
		return 0;
	}

	/* Resolve the designated symbol to its actual value */
	value = resolve_symbol
		(context, readelf, print_failures, name,
		 (jit_nuint)ELF_R_SYM(reloc->r_info));
	if(!value)
	{
		return 0;
	}

	/* Perform the relocation */
	if(!(*(readelf->reloc_func))
		(readelf, address, (int)(ELF_R_TYPE(reloc->r_info)),
		 (jit_nuint)value, 0, 0))
	{
		if(print_failures)
		{
			printf("%s: relocation type %d was not recognized\n",
				   name, (int)(ELF_R_TYPE(reloc->r_info)));
		}
		return 0;
	}
	return 1;
}

/*
 * Perform a DT_RELA style relocation on an ELF binary.
 */
static int perform_rela
	(jit_context_t context, jit_readelf_t readelf,
	 int print_failures, const char *name, Elf_Rela *reloc)
{
	void *address;
	void *value;

	/* Get the address to apply the relocation at */
	address = jit_readelf_map_vaddr(readelf, (jit_nuint)(reloc->r_offset));
	if(!address)
	{
		if(print_failures)
		{
			printf("%s: cannot map virtual address 0x%lx\n",
				   name, (long)(reloc->r_offset));
		}
		return 0;
	}

	/* Resolve the designated symbol to its actual value */
	value = resolve_symbol
		(context, readelf, print_failures, name,
		 (jit_nuint)ELF_R_SYM(reloc->r_info));
	if(!value)
	{
		return 0;
	}

	/* Perform the relocation */
	if(!(*(readelf->reloc_func))
		(readelf, address, (int)(ELF_R_TYPE(reloc->r_info)),
		 (jit_nuint)value, 1, (jit_nuint)(reloc->r_addend)))
	{
		if(print_failures)
		{
			printf("%s: relocation type %d was not recognized\n",
				   name, (int)(ELF_R_TYPE(reloc->r_info)));
		}
		return 0;
	}
	return 1;
}

/*
 * Perform relocations on an ELF binary.  Returns zero on failure.
 */
static int perform_relocations
	(jit_context_t context, jit_readelf_t readelf, int print_failures)
{
	Elf_Addr address;
	Elf_Addr table_size;
	Elf_Addr entry_size;
	unsigned char *table;
	const char *name;
	int ok = 1;

	/* Get the library name, for printing diagnostic messages */
	name = jit_readelf_get_name(readelf);
	if(!name)
	{
		name = "unknown-elf-binary";
	}

	/* Bail out if we don't know how to perform relocations */
	if(!(readelf->reloc_func))
	{
		if(print_failures)
		{
			printf("%s: do not know how to perform relocations\n", name);
		}
		return 0;
	}

	/* Apply the "Rel" relocations in the dynamic section */
	if(dynamic_for_type(readelf, DT_REL, &address) &&
	   dynamic_for_type(readelf, DT_RELSZ, &table_size) &&
	   dynamic_for_type(readelf, DT_RELENT, &entry_size) && entry_size)
	{
		table = (unsigned char *)jit_readelf_map_vaddr
			(readelf, (jit_nuint)address);
		while(table && table_size >= entry_size)
		{
			if(!perform_rel(context, readelf, print_failures, name,
					        (Elf_Rel *)table))
			{
				ok = 0;
			}
			table += (jit_nuint)entry_size;
			table_size -= entry_size;
		}
	}

	/* Apply the "Rela" relocations in the dynamic section */
	if(dynamic_for_type(readelf, DT_RELA, &address) &&
	   dynamic_for_type(readelf, DT_RELASZ, &table_size) &&
	   dynamic_for_type(readelf, DT_RELAENT, &entry_size) && entry_size)
	{
		table = (unsigned char *)jit_readelf_map_vaddr
			(readelf, (jit_nuint)address);
		while(table && table_size >= entry_size)
		{
			if(!perform_rela(context, readelf, print_failures, name,
					         (Elf_Rela *)table))
			{
				ok = 0;
			}
			table += (jit_nuint)entry_size;
			table_size -= entry_size;
		}
	}

	/* Apply the "PLT" relocations in the dynamic section, which
	   may be either DT_REL or DT_RELA style relocations */
	if(dynamic_for_type(readelf, DT_JMPREL, &address) &&
	   dynamic_for_type(readelf, DT_PLTRELSZ, &table_size) &&
	   dynamic_for_type(readelf, DT_PLTREL, &entry_size))
	{
		if(entry_size == DT_REL)
		{
			if(dynamic_for_type(readelf, DT_RELENT, &entry_size) && entry_size)
			{
				table = (unsigned char *)jit_readelf_map_vaddr
					(readelf, (jit_nuint)address);
				while(table && table_size >= entry_size)
				{
					if(!perform_rel(context, readelf, print_failures, name,
							        (Elf_Rel *)table))
					{
						ok = 0;
					}
					table += (jit_nuint)entry_size;
					table_size -= entry_size;
				}
			}
		}
		else if(entry_size == DT_RELA)
		{
			if(dynamic_for_type(readelf, DT_RELAENT, &entry_size) && entry_size)
			{
				table = (unsigned char *)jit_readelf_map_vaddr
					(readelf, (jit_nuint)address);
				while(table && table_size >= entry_size)
				{
					if(!perform_rela(context, readelf, print_failures, name,
							         (Elf_Rela *)table))
					{
						ok = 0;
					}
					table += (jit_nuint)entry_size;
					table_size -= entry_size;
				}
			}
		}
	}

	/* Return to the caller */
	return ok;
}

/*@
 * @deftypefun int jit_readelf_resolve_all (jit_context_t @var{context}, int @var{print_failures})
 * Resolve all of the cross-library symbol references in ELF binaries
 * that have been added to @var{context} but which were not resolved
 * in the previous call to this function.  If @var{print_failures}
 * is non-zero, then diagnostic messages will be written to stdout
 * for any symbol resolutions that fail.
 *
 * Returns zero on failure, or non-zero if all symbols were successfully
 * resolved.  If there are no ELF binaries awaiting resolution, then
 * this function will return a non-zero result.
 * @end deftypefun
@*/
int jit_readelf_resolve_all(jit_context_t context, int print_failures)
{
	int ok = 1;
	jit_readelf_t readelf;
	if(!context)
	{
		return 0;
	}

	_jit_memory_lock(context);

	readelf = context->elf_binaries;
	while(readelf != 0)
	{
		if(!(readelf->resolved))
		{
			readelf->resolved = 1;
			if(!perform_relocations(context, readelf, print_failures))
			{
				ok = 0;
			}
		}
		readelf = readelf->next;
	}

	_jit_memory_unlock(context);
	return ok;
}

/*@
 * @deftypefun int jit_readelf_register_symbol (jit_context_t @var{context}, const char *@var{name}, void *@var{value}, int @var{after})
 * Register @var{value} with @var{name} on the specified @var{context}.
 * Whenever symbols are resolved with @code{jit_readelf_resolve_all},
 * and the symbol @var{name} is encountered, @var{value} will be
 * substituted.  Returns zero if out of memory or there is something
 * wrong with the parameters.
 *
 * If @var{after} is non-zero, then @var{name} will be resolved after all
 * other ELF libraries; otherwise it will be resolved before the ELF
 * libraries.
 *
 * This function is used to register intrinsic symbols that are specific to
 * the front end virtual machine.  References to intrinsics within
 * @code{libjit} itself are resolved automatically.
 * @end deftypefun
@*/
int jit_readelf_register_symbol
	(jit_context_t context, const char *name, void *value, int after)
{
	jit_regsym_t sym;
	jit_regsym_t *new_list;

	/* Bail out if there is something wrong with the parameters */
	if(!context || !name || !value)
	{
		return 0;
	}

	/* Allocate and populate the symbol information block */
	sym = (jit_regsym_t)jit_malloc
		(sizeof(struct jit_regsym) + jit_strlen(name));
	if(!sym)
	{
		return 0;
	}
	sym->value = value;
	sym->after = after;
	jit_strcpy(sym->name, name);

	/* Add the symbol details to the registered list */
	new_list = (jit_regsym_t *)jit_realloc
		(context->registered_symbols,
		 sizeof(jit_regsym_t) * (context->num_registered_symbols + 1));
	if(!new_list)
	{
		jit_free(sym);
		return 0;
	}
	new_list[(context->num_registered_symbols)++] = sym;
	context->registered_symbols = new_list;
	return 1;
}

/************************************************************************

		             Warning!  Warning!  Warning!

The following code is very system-dependent, as every ELF target has its
own peculiar mechanism for performing relocations.  Consult your target's
documentation for the precise details.

To make things a little easier, you only need to support the relocation
types that you intend to use in the JIT's ELF writer.  And many types
only pertain to ELF executable or object files, which we don't use.

************************************************************************/

#if defined(__i386) || defined(__i386__) || defined(_M_IX86)

/*
 * Apply relocations for i386 platforms.
 */
static int i386_reloc(jit_readelf_t readelf, void *address, int type,
					  jit_nuint value, int has_addend, jit_nuint addend)
{
	if(type == R_386_32)
	{
		if(has_addend)
		{
			*((jit_nuint *)address) = value + addend;
		}
		else
		{
			*((jit_nuint *)address) += value;
		}
		return 1;
	}
	else if(type == R_386_PC32)
	{
		value -= (jit_nuint)address;
		if(has_addend)
		{
			*((jit_nuint *)address) = value + addend;
		}
		else
		{
			*((jit_nuint *)address) += value;
		}
		return 1;
	}
	return 0;
}

#endif /* i386 */

#if defined(__arm) || defined(__arm__)

/*
 * Apply relocations for ARM platforms.
 */
static int arm_reloc(jit_readelf_t readelf, void *address, int type,
					 jit_nuint value, int has_addend, jit_nuint addend)
{
	if(type == R_ARM_PC24)
	{
		value -= (jit_nuint)address;
		if(has_addend)
		{
			*((jit_nuint *)address) =
				(*((jit_nuint *)address) & 0xFF000000) + value + addend;
		}
		else
		{
			*((jit_nuint *)address) += value;
		}
		return 1;
	}
	else if(type == R_ARM_ABS32)
	{
		if(has_addend)
		{
			*((jit_nuint *)address) = value + addend;
		}
		else
		{
			*((jit_nuint *)address) += value;
		}
		return 1;
	}
	else if(type == R_ARM_REL32)
	{
		value -= (jit_nuint)address;
		if(has_addend)
		{
			*((jit_nuint *)address) = value + addend;
		}
		else
		{
			*((jit_nuint *)address) += value;
		}
		return 1;
	}
	return 0;
}

#endif /* arm */

/*
 * Apply relocations for the interpreted platform.
 */
static int interp_reloc(jit_readelf_t readelf, void *address, int type,
						jit_nuint value, int has_addend, jit_nuint addend)
{
	/* We only have one type of relocation for the interpreter: direct */
	if(type == 1)
	{
		*((jit_nuint *)address) = value;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * Get the relocation function for a particular machine type.
 */
static jit_reloc_func get_reloc(unsigned int machine)
{
#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
	if(machine == EM_386)
	{
		return i386_reloc;
	}
#endif
#if defined(__arm) || defined(__arm__)
	if(machine == EM_ARM)
	{
		return arm_reloc;
	}
#endif
	if(machine == 0x4C6A)		/* "Lj" for the libjit interpreter */
	{
		return interp_reloc;
	}
	return 0;
}
