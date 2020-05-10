/*
 * jit-util.h - Utility functions.
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

#ifndef	_JIT_UTIL_H
#define	_JIT_UTIL_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Memory allocation routines.
 */
void *jit_malloc(unsigned int size) JIT_NOTHROW;
void *jit_calloc(unsigned int num, unsigned int size) JIT_NOTHROW;
void *jit_realloc(void *ptr, unsigned int size) JIT_NOTHROW;
void jit_free(void *ptr) JIT_NOTHROW;

#define	jit_new(type)		((type *)jit_malloc(sizeof(type)))
#define	jit_cnew(type)		((type *)jit_calloc(1, sizeof(type)))

/*
 * Memory set/copy/compare routines.
 */
void *jit_memset(void *dest, int ch, unsigned int len) JIT_NOTHROW;
void *jit_memcpy(void *dest, const void *src, unsigned int len) JIT_NOTHROW;
void *jit_memmove(void *dest, const void *src, unsigned int len) JIT_NOTHROW;
int   jit_memcmp(const void *s1, const void *s2, unsigned int len) JIT_NOTHROW;
void *jit_memchr(const void *str, int ch, unsigned int len) JIT_NOTHROW;

/*
 * String routines.
 */
unsigned int jit_strlen(const char *str) JIT_NOTHROW;
char *jit_strcpy(char *dest, const char *src) JIT_NOTHROW;
char *jit_strcat(char *dest, const char *src) JIT_NOTHROW;
char *jit_strncpy(char *dest, const char *src, unsigned int len) JIT_NOTHROW;
char *jit_strdup(const char *str) JIT_NOTHROW;
char *jit_strndup(const char *str, unsigned int len) JIT_NOTHROW;
int jit_strcmp(const char *str1, const char *str2) JIT_NOTHROW;
int jit_strncmp
	(const char *str1, const char *str2, unsigned int len) JIT_NOTHROW;
int jit_stricmp(const char *str1, const char *str2) JIT_NOTHROW;
int jit_strnicmp
	(const char *str1, const char *str2, unsigned int len) JIT_NOTHROW;
char *jit_strchr(const char *str, int ch) JIT_NOTHROW;
char *jit_strrchr(const char *str, int ch) JIT_NOTHROW;
int jit_sprintf(char *str, const char *format, ...) JIT_NOTHROW;
int jit_snprintf
	(char *str, unsigned int len, const char *format, ...) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_UTIL_H */
