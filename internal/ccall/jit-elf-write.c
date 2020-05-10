/*
 * jit-elf-write.c - Routines to write ELF-format binaries.
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
#include "jit-elf-defs.h"
#include "jit-rules.h"

/*@

@section Writing ELF binaries

@*/

/*
 * Determine whether we should be using the 32-bit or 64-bit ELF structures.
 */
#ifdef JIT_NATIVE_INT32
	typedef Elf32_Ehdr  Elf_Ehdr;
	typedef Elf32_Shdr  Elf_Shdr;
	typedef Elf32_Phdr  Elf_Phdr;
	typedef Elf32_Addr  Elf_Addr;
	typedef Elf32_Half  Elf_Half;
	typedef Elf32_Word  Elf_Word;
	typedef Elf32_Xword Elf_Xword;
	typedef Elf32_Off   Elf_Off;
	typedef Elf32_Dyn   Elf_Dyn;
	typedef Elf32_Sym   Elf_Sym;
#else
	typedef Elf64_Ehdr  Elf_Ehdr;
	typedef Elf64_Shdr  Elf_Shdr;
	typedef Elf64_Phdr  Elf_Phdr;
	typedef Elf64_Addr  Elf_Addr;
	typedef Elf64_Half  Elf_Half;
	typedef Elf64_Word  Elf_Word;
	typedef Elf64_Xword Elf_Xword;
	typedef Elf64_Off   Elf_Off;
	typedef Elf64_Dyn   Elf_Dyn;
	typedef Elf64_Sym   Elf_Sym;
#endif

/*
 * Information about the contents of a section.
 */
typedef struct jit_section *jit_section_t;
struct jit_section
{
	Elf_Shdr			shdr;
	char			   *data;
	unsigned int		data_len;
};

/*
 * Control structure for writing an ELF binary.
 */
struct jit_writeelf
{
	Elf_Ehdr			ehdr;
	jit_section_t	    sections;
	int					num_sections;
	int					regular_string_section;
	int					dynamic_string_section;
};

/*
 * Get a string from the regular string section.
 */
static const char *get_string(jit_writeelf_t writeelf, Elf_Word index)
{
	if(writeelf->regular_string_section < 0)
	{
		/* The regular string section has not been created yet */
		return 0;
	}
	else
	{
		/* Retrieve the pointer to the string's starting point */
		return writeelf->sections[writeelf->regular_string_section].data +
					(jit_nuint)index;
	}
}

/*
 * Add a string to the regular string section.  We don't worry about
 * duplicate names because we only store section names here.  And
 * section names are only added when a new section is created.
 */
static Elf_Word add_string(jit_writeelf_t writeelf, const char *name)
{
	jit_section_t section;
	char *data;
	Elf_Word index;
	unsigned int name_len = jit_strlen(name) + 1;
	section = &(writeelf->sections[writeelf->regular_string_section]);
	data = (char *)jit_realloc(section->data, section->data_len + name_len);
	if(!data)
	{
		return 0;
	}
	section->data = data;
	jit_strcpy(data + section->data_len, name);
	index = (Elf_Word)(section->data_len);
	section->data_len += name_len;
	return index;
}

/*
 * Get a string from the dynamic string section.
 */
static const char *get_dyn_string(jit_writeelf_t writeelf, Elf_Word index)
{
	if(writeelf->dynamic_string_section < 0)
	{
		/* The dynamic string section has not been created yet */
		return 0;
	}
	else
	{
		/* Retrieve the pointer to the string's starting point */
		return writeelf->sections[writeelf->dynamic_string_section].data +
					(jit_nuint)index;
	}
}

/*
 * Add a string to the dynamic string section.
 *
 * TODO: use a hash table to cache previous names.
 */
static Elf_Word add_dyn_string(jit_writeelf_t writeelf, const char *name)
{
	jit_section_t section;
	char *data;
	Elf_Word index;
	unsigned int name_len = jit_strlen(name) + 1;
	section = &(writeelf->sections[writeelf->dynamic_string_section]);
	data = (char *)jit_realloc(section->data, section->data_len + name_len);
	if(!data)
	{
		return 0;
	}
	section->data = data;
	jit_strcpy(data + section->data_len, name);
	index = (Elf_Word)(section->data_len);
	section->data_len += name_len;
	return index;
}

/*
 * Get or add a section.
 */
static jit_section_t get_section
	(jit_writeelf_t writeelf, const char *name, jit_int type,
	 Elf_Word flags, Elf_Word entry_size, Elf_Word alignment)
{
	int index;
	jit_section_t section;

	/* Search the section table for an existing section by this name */
	for(index = 0; index < writeelf->num_sections; ++index)
	{
		section = &(writeelf->sections[index]);
		if(!jit_strcmp(get_string(writeelf, section->shdr.sh_name), name))
		{
			return section;
		}
	}

	/* Create a new section and clear it */
	section = (jit_section_t)jit_realloc
		(writeelf->sections,
		 (writeelf->num_sections + 1) * sizeof(struct jit_section));
	if(!section)
	{
		return 0;
	}
	writeelf->sections = section;
	section += writeelf->num_sections;
	jit_memzero(section, sizeof(struct jit_section));

	/* Set the section's name.  If this is the first section created,
	   then it is the string table itself, and we have to add the
	   name to the section itself to start the ball rolling */
	if(writeelf->regular_string_section < 0)
	{
		section->data = (char *)jit_malloc(jit_strlen(name) + 2);
		if(!(section->data))
		{
			return 0;
		}
		section->data_len = jit_strlen(name) + 2;
		section->data[0] = '\0';	/* Empty string is always first */
		jit_strcpy(section->data + 1, name);
		section->shdr.sh_name = 1;
		writeelf->regular_string_section = writeelf->num_sections;
	}
	else
	{
		section->shdr.sh_name = add_string(writeelf, name);
		if(!(section->shdr.sh_name))
		{
			return 0;
		}
	}

	/* Set the other section properties */
	section->shdr.sh_type = (Elf_Word)type;
	section->shdr.sh_flags = flags;
	section->shdr.sh_entsize = entry_size;
	section->shdr.sh_addralign = alignment;

	/* Increase the section count and return */
	++(writeelf->num_sections);
	return section;
}

/*
 * Append data to a section.
 */
static int add_to_section
	(jit_section_t section, const void *buf, unsigned int len)
{
	char *data = (char *)jit_realloc(section->data, section->data_len + len);
	if(!data)
	{
		return 0;
	}
	section->data = data;
	jit_memcpy(data + section->data_len, buf, len);
	section->data_len += len;
	return 1;
}

/*
 * Add an entry to the dynamic linking information section.
 */
static int add_dyn_info
	(jit_writeelf_t writeelf, int type, Elf_Addr value, int modify_existing)
{
	jit_section_t section;
	Elf_Dyn dyn;

	/* Get or create the ".dynamic" section */
	section = get_section(writeelf, ".dynamic", SHT_DYNAMIC,
						  SHF_WRITE | SHF_ALLOC,
						  sizeof(Elf_Dyn), sizeof(Elf_Dyn));
	if(!section)
	{
		return 0;
	}

	/* See if we already have this entry, and modify it as appropriate */
	if(modify_existing)
	{
		Elf_Dyn *existing = (Elf_Dyn *)(section->data);
		unsigned int num = section->data_len / sizeof(Elf_Dyn);
		while(num > 0)
		{
			if(existing->d_tag == type)
			{
				existing->d_un.d_ptr = value;
				return 1;
			}
			++existing;
			--num;
		}
	}

	/* Format the dynamic entry */
	jit_memzero(&dyn, sizeof(dyn));
	dyn.d_tag = type;
	dyn.d_un.d_ptr = value;

	/* Add the entry to the section's contents */
	return add_to_section(section, &dyn, sizeof(dyn));
}

/*@
 * @deftypefun jit_writeelf_t jit_writeelf_create (const char *@var{library_name})
 * Create an object to assist with the process of writing an ELF binary.
 * The @var{library_name} will be embedded into the binary.  Returns NULL
 * if out of memory.
 * @end deftypefun
@*/
jit_writeelf_t jit_writeelf_create(const char *library_name)
{
	jit_writeelf_t writeelf;
	Elf_Word name_index;
	union
	{
		jit_ushort value;
		unsigned char bytes[2];

	} un;
	jit_elf_info_t elf_info;

	/* Create the writer control structure */
	writeelf = jit_cnew(struct jit_writeelf);
	if(!writeelf)
	{
		return 0;
	}
	writeelf->regular_string_section = -1;
	writeelf->dynamic_string_section = -1;

	/* Create the regular string section for section names,
	   which must be the first section that we create */
	if(!get_section(writeelf, ".shstrtab", SHT_STRTAB, 0, 0, 0))
	{
		jit_writeelf_destroy(writeelf);
		return 0;
	}

	/* Create the dynamic string section, for dynamic linking symbols */
	if(!get_section(writeelf, ".dynstr", SHT_STRTAB, SHF_ALLOC, 0, 0))
	{
		jit_writeelf_destroy(writeelf);
		return 0;
	}
	writeelf->dynamic_string_section = writeelf->num_sections - 1;
	if(!add_dyn_string(writeelf, ""))
	{
		jit_writeelf_destroy(writeelf);
		return 0;
	}

	/* Add the library name to the dynamic linking information section */
	name_index = add_dyn_string(writeelf, library_name);
	if(!name_index)
	{
		jit_writeelf_destroy(writeelf);
		return 0;
	}
	if(!add_dyn_info(writeelf, DT_SONAME, (Elf_Addr)name_index, 0))
	{
		jit_writeelf_destroy(writeelf);
		return 0;
	}

	/* Fill in the Ehdr fields */
	writeelf->ehdr.e_ident[EI_MAG0] = ELFMAG0;
	writeelf->ehdr.e_ident[EI_MAG1] = ELFMAG1;
	writeelf->ehdr.e_ident[EI_MAG2] = ELFMAG2;
	writeelf->ehdr.e_ident[EI_MAG3] = ELFMAG3;
#ifdef JIT_NATIVE_INT32
	writeelf->ehdr.e_ident[EI_CLASS] = ELFCLASS32;
#else
	writeelf->ehdr.e_ident[EI_CLASS] = ELFCLASS64;
#endif
	un.value = 0x0102;
	if(un.bytes[0] == 0x01)
	{
		writeelf->ehdr.e_ident[EI_DATA] = ELFDATA2MSB;
	}
	else
	{
		writeelf->ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
	}
	writeelf->ehdr.e_ident[EI_VERSION] = EV_CURRENT;
	_jit_gen_get_elf_info(&elf_info);
	writeelf->ehdr.e_ident[EI_OSABI] = (unsigned char)(elf_info.abi);
	writeelf->ehdr.e_ident[EI_ABIVERSION] =
		(unsigned char)(elf_info.abi_version);
	writeelf->ehdr.e_machine = (Elf_Half)(elf_info.machine);
	writeelf->ehdr.e_version = EV_CURRENT;
	writeelf->ehdr.e_ehsize = sizeof(writeelf->ehdr);

	/* Every ELF binary that we generate will need "libjit.so" */
	if(!jit_writeelf_add_needed(writeelf, "libjit.so"))
	{
		jit_writeelf_destroy(writeelf);
		return 0;
	}

	/* We are ready to go */
	return writeelf;
}

/*@
 * @deftypefun void jit_writeelf_destroy (jit_writeelf_t @var{writeelf})
 * Destroy the memory structures that were used while @var{writeelf}
 * was being built.
 * @end deftypefun
@*/
void jit_writeelf_destroy(jit_writeelf_t writeelf)
{
	int index;
	if(!writeelf)
	{
		return;
	}
	for(index = 0; index < writeelf->num_sections; ++index)
	{
		jit_free(writeelf->sections[index].data);
	}
	jit_free(writeelf->sections);
	jit_free(writeelf);
}

/*@
 * @deftypefun int jit_writeelf_write (jit_writeelf_t @var{writeelf}, const char *@var{filename})
 * Write a fully-built ELF binary to @var{filename}.  Returns zero
 * if an error occurred (reason in @code{errno}).
 * @end deftypefun
@*/
int jit_writeelf_write(jit_writeelf_t writeelf, const char *filename)
{
	/* TODO */
	return 1;
}

/*@
 * @deftypefun int jit_writeelf_add_function (jit_writeelf_t @var{writeelf}, jit_function_t @var{func}, const char *@var{name})
 * Write the code for @var{func} to the ELF binary represented by
 * @var{writeelf}.  The function must already be compiled, and its
 * context must have the @code{JIT_OPTION_PRE_COMPILE} option set
 * to a non-zero value.  Returns zero if out of memory or the
 * parameters are invalid.
 * @end deftypefun
@*/
int jit_writeelf_add_function
	(jit_writeelf_t writeelf, jit_function_t func, const char *name)
{
	/* TODO */
	return 1;
}

/*@
 * @deftypefun int jit_writeelf_add_needed (jit_writeelf_t @var{writeelf}, const char *@var{library_name})
 * Add @var{library_name} to the list of dependent libraries that are needed
 * when the ELF binary is reloaded.  If @var{library_name} is already on
 * the list, then this request will be silently ignored.  Returns
 * zero if out of memory or the parameters are invalid.
 * @end deftypefun
@*/
int jit_writeelf_add_needed(jit_writeelf_t writeelf, const char *library_name)
{
	jit_section_t section;
	Elf_Dyn *dyn;
	unsigned int num_dyn;
	Elf_Word name_index;
	if(!writeelf || !library_name)
	{
		return 0;
	}
	section = get_section(writeelf, ".dynamic", SHT_DYNAMIC,
						  SHF_WRITE | SHF_ALLOC,
						  sizeof(Elf_Dyn), sizeof(Elf_Dyn));
	if(!section)
	{
		return 0;
	}
	dyn = (Elf_Dyn *)(section->data);
	num_dyn = section->data_len / sizeof(Elf_Dyn);
	while(num_dyn > 0)
	{
		if(dyn->d_tag == DT_NEEDED &&
		   !jit_strcmp(get_dyn_string(writeelf, (Elf_Word)(dyn->d_un.d_ptr)),
		   			   library_name))
		{
			return 1;
		}
		++dyn;
		--num_dyn;
	}
	name_index = add_dyn_string(writeelf, library_name);
	if(!name_index)
	{
		return 0;
	}
	if(!add_dyn_info(writeelf, DT_NEEDED, (Elf_Addr)name_index, 0))
	{
		return 0;
	}
	return 1;
}

/*@
 * @deftypefun int jit_writeelf_write_section (jit_writeelf_t @var{writeelf}, const char *@var{name}, jit_int @var{type}, const void *@var{buf}, unsigned int @var{len}, int @var{discardable})
 * Write auxillary data to a section called @var{name}.  If @var{type}
 * is not zero, then it indicates an ELF section type.  This is used
 * by virtual machines to store auxillary data that can be retrieved
 * later using @code{jit_readelf_get_section}.  If the section already
 * contains data, then this will append the new data.  If @var{discardable}
 * is non-zero, then it is OK for this section to be discarded when the
 * ELF binary is stripped.  Returns zero if out of memory or the
 * parameters are invalid.
 * @end deftypefun
@*/
int jit_writeelf_write_section
	(jit_writeelf_t writeelf, const char *name, jit_int type,
	 const void *buf, unsigned int len, int discardable)
{
	jit_section_t section;
	if(!writeelf || !name)
	{
		return 0;
	}
	if(!type)
	{
		/* Application-specific section type, for storing unspecified data */
		type = (jit_int)(SHT_LOUSER + 0x1234);
	}
	if(discardable)
	{
		section = get_section(writeelf, name, type, 0, 1, 1);
	}
	else
	{
		section = get_section(writeelf, name, type, SHF_ALLOC, 1, 1);
	}
	if(!section)
	{
		return 0;
	}
	if(len > 0)
	{
		return add_to_section(section, buf, len);;
	}
	else
	{
		return 1;
	}
}
