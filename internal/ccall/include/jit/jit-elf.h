/*
 * <jit/jit-elf.h> - Routines to read and write ELF-format binaries.
 *
 * Copyright (C) 2004  Southern Storm Software, Pty Ltd.
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

#ifndef	_JIT_ELF_H
#define	_JIT_ELF_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Opaque types that represent a loaded ELF binary in read or write mode.
 */
typedef struct jit_readelf *jit_readelf_t;
typedef struct jit_writeelf *jit_writeelf_t;

/*
 * Flags for "jit_readelf_open".
 */
#define	JIT_READELF_FLAG_FORCE	(1 << 0)	/* Force file to load */
#define	JIT_READELF_FLAG_DEBUG	(1 << 1)	/* Print debugging information */

/*
 * Result codes from "jit_readelf_open".
 */
#define	JIT_READELF_OK			0	/* File was opened successfully */
#define	JIT_READELF_CANNOT_OPEN	1	/* Could not open the file */
#define	JIT_READELF_NOT_ELF		2	/* Not an ELF-format binary */
#define	JIT_READELF_WRONG_ARCH	3	/* Wrong architecture for local system */
#define	JIT_READELF_BAD_FORMAT	4	/* ELF file, but badly formatted */
#define	JIT_READELF_MEMORY		5	/* Insufficient memory to load the file */

/*
 * External function declarations.
 */
int jit_readelf_open
	(jit_readelf_t *readelf, const char *filename, int flags) JIT_NOTHROW;
void jit_readelf_close(jit_readelf_t readelf) JIT_NOTHROW;
const char *jit_readelf_get_name(jit_readelf_t readelf) JIT_NOTHROW;
void *jit_readelf_get_symbol
	(jit_readelf_t readelf, const char *name) JIT_NOTHROW;
void *jit_readelf_get_section
	(jit_readelf_t readelf, const char *name, jit_nuint *size) JIT_NOTHROW;
void *jit_readelf_get_section_by_type
	(jit_readelf_t readelf, jit_int type, jit_nuint *size) JIT_NOTHROW;
void *jit_readelf_map_vaddr
	(jit_readelf_t readelf, jit_nuint vaddr) JIT_NOTHROW;
unsigned int jit_readelf_num_needed(jit_readelf_t readelf) JIT_NOTHROW;
const char *jit_readelf_get_needed
	(jit_readelf_t readelf, unsigned int index) JIT_NOTHROW;
void jit_readelf_add_to_context
	(jit_readelf_t readelf, jit_context_t context) JIT_NOTHROW;
int jit_readelf_resolve_all
	(jit_context_t context, int print_failures) JIT_NOTHROW;
int jit_readelf_register_symbol
	(jit_context_t context, const char *name,
	 void *value, int after) JIT_NOTHROW;

jit_writeelf_t jit_writeelf_create(const char *library_name) JIT_NOTHROW;
void jit_writeelf_destroy(jit_writeelf_t writeelf) JIT_NOTHROW;
int jit_writeelf_write
	(jit_writeelf_t writeelf, const char *filename) JIT_NOTHROW;
int jit_writeelf_add_function
	(jit_writeelf_t writeelf, jit_function_t func,
	 const char *name) JIT_NOTHROW;
int jit_writeelf_add_needed
	(jit_writeelf_t writeelf, const char *library_name) JIT_NOTHROW;
int jit_writeelf_write_section
	(jit_writeelf_t writeelf, const char *name, jit_int type,
	 const void *buf, unsigned int len, int discardable) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_ELF_H */
