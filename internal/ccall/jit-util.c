/*
 * jit-memory.c - Memory copy/set/compare routines.
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

#include <jit/jit-util.h>
#include "jit-config.h"

#include <stdio.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if defined(HAVE_STRING_H)
# include <string.h>
#elif defined(HAVE_STRINGS_H)
# include <strings.h>
#endif
#if defined(HAVE_MEMORY_H)
# include <memory.h>
#endif
#if defined(HAVE_STDARG_H)
# include <stdarg.h>
#elif defined(HAVE_VARARGS_H)
# include <varargs.h>
#endif

/*
 * Undefine the macros in "jit-internal.h" so that we
 * can define the real function forms.
 */
#undef jit_memset
#undef jit_memcpy
#undef jit_memmove
#undef jit_memcmp
#undef jit_memchr

/*@
 * @section Memory allocation
 *
 * The @code{libjit} library provides an interface to the traditional
 * system @code{malloc} routines.  All heap allocation in @code{libjit}
 * goes through these functions.  If you need to perform some other kind
 * of memory allocation, you can replace these functions with your
 * own versions.
@*/

/*@
 * @deftypefun {void *} jit_malloc (unsigned int @var{size})
 * Allocate @var{size} bytes of memory from the heap.
 * @end deftypefun
 *
 * @deftypefun {type *} jit_new (@var{type})
 * Allocate @code{sizeof(@var{type})} bytes of memory from the heap and
 * cast the return pointer to @code{@var{type} *}.  This is a macro that
 * wraps up the underlying @code{jit_malloc} function and is less
 * error-prone when allocating structures.
 * @end deftypefun
@*/
void *jit_malloc(unsigned int size)
{
	return malloc(size);
}

/*@
 * @deftypefun {void *} jit_calloc (unsigned int @var{num}, unsigned int @var{size})
 * Allocate @code{@var{num} * @var{size}} bytes of memory from the heap and clear
 * them to zero.
 * @end deftypefun
 *
 * @deftypefun {type *} jit_cnew (@var{type})
 * Allocate @code{sizeof(@var{type})} bytes of memory from the heap and
 * cast the return pointer to @code{@var{type} *}.  The memory is cleared
 * to zero.
 * @end deftypefun
@*/
void *jit_calloc(unsigned int num, unsigned int size)
{
	return calloc(num, size);
}

/*@
 * @deftypefun {void *} jit_realloc (void *@var{ptr}, unsigned int @var{size})
 * Re-allocate the memory at @var{ptr} to be @var{size} bytes in size.
 * The memory block at @var{ptr} must have been allocated by a previous
 * call to @code{jit_malloc}, @code{jit_calloc}, or @code{jit_realloc}.
 * @end deftypefun
@*/
void *jit_realloc(void *ptr, unsigned int size)
{
	return realloc(ptr, size);
}

/*@
 * @deftypefun void jit_free (void *@var{ptr})
 * Free the memory at @var{ptr}.  It is safe to pass a NULL pointer.
 * @end deftypefun
@*/
void jit_free(void *ptr)
{
	if(ptr)
	{
		free(ptr);
	}
}

/*@
 * @section Memory set, copy, compare, etc
 * @cindex Memory operations
 *
 * The following functions are provided to set, copy, compare, and search
 * memory blocks.
@*/

/*@
 * @deftypefun {void *} jit_memset (void *@var{dest}, int @var{ch}, unsigned int @var{len})
 * Set the @var{len} bytes at @var{dest} to the value @var{ch}.
 * Returns @var{dest}.
 * @end deftypefun
@*/
void *jit_memset(void *dest, int ch, unsigned int len)
{
#ifdef HAVE_MEMSET
	return memset(dest, ch, len);
#else
	unsigned char *d = (unsigned char *)dest;
	while(len > 0)
	{
		*d++ = (unsigned char)ch;
		--len;
	}
	return dest;
#endif
}

/*@
 * @deftypefun {void *} jit_memcpy (void *@var{dest}, const void *@var{src}, unsigned int @var{len})
 * Copy the @var{len} bytes at @var{src} to @var{dest}.  Returns
 * @var{dest}.  The behavior is undefined if the blocks overlap
 * (use @var{jit_memmove} instead for that case).
 * @end deftypefun
@*/
void *jit_memcpy(void *dest, const void *src, unsigned int len)
{
#if defined(HAVE_MEMCPY)
	return memcpy(dest, src, len);
#elif defined(HAVE_BCOPY)
	bcopy(src, dest, len);
	return dest;
#else
	unsigned char *d = (unsigned char *)dest;
	const unsigned char *s = (const unsigned char *)src;
	while(len > 0)
	{
		*d++ = *s++;
		--len;
	}
	return dest;
#endif
}

/*@
 * @deftypefun {void *} jit_memmove (void *@var{dest}, const void *@var{src}, unsigned int @var{len})
 * Copy the @var{len} bytes at @var{src} to @var{dest} and handle
 * overlapping blocks correctly.  Returns @var{dest}.
 * @end deftypefun
@*/
void *jit_memmove(void *dest, const void *src, unsigned int len)
{
#ifdef HAVE_MEMMOVE
	return memmove(dest, src, len);
#else
	unsigned char *d = (unsigned char *)dest;
	const unsigned char *s = (const unsigned char *)src;
	if(((const unsigned char *)d) < s)
	{
		while(len > 0)
		{
			*d++ = *s++;
			--len;
		}
	}
	else
	{
		d += len;
		s += len;
		while(len > 0)
		{
			*(--d) = *(--s);
			--len;
		}
	}
	return dest;
#endif
}

/*@
 * @deftypefun int jit_memcmp (const void *@var{s1}, const void *@var{s2}, unsigned int @var{len})
 * Compare @var{len} bytes at @var{s1} and @var{s2}, returning a negative,
 * zero, or positive result depending upon their relationship.  It is
 * system-specific as to whether this function uses signed or unsigned
 * byte comparisons.
 * @end deftypefun
@*/
int jit_memcmp(const void *s1, const void *s2, unsigned int len)
{
#if defined(HAVE_MEMCMP)
	return memcmp(s1, s2, len);
#elif defined(HAVE_BCMP)
	return bcmp(s1, s2, len);
#else
	const unsigned char *str1 = (const unsigned char *)s1;
	const unsigned char *str2 = (const unsigned char *)s2;
	while(len > 0)
	{
		if(*str1 < *str2)
			return -1;
		else if(*str1 > *str2)
			return 1;
		++str1;
		++str2;
		--len;
	}
	return 0;
#endif
}

/*@
 * @deftypefun {void *} jit_memchr (void *@var{str}, int @var{ch}, unsigned int @var{len})
 * Search the @var{len} bytes at @var{str} for the first instance of
 * the value @var{ch}.  Returns the location of @var{ch} if it was found,
 * or NULL if it was not found.
 * @end deftypefun
@*/
void *jit_memchr(const void *str, int ch, unsigned int len)
{
#ifdef HAVE_MEMCHR
	return memchr(str, ch, len);
#else
	const unsigned char *s = (const unsigned char *)str;
	while(len > 0)
	{
		if(*s == (unsigned char)ch)
		{
			return (void *)s;
		}
		++s;
		--len;
	}
	return (void *)0;
#endif
}

/*@
 * @section String operations
 * @cindex String operations
 *
 * The following functions are provided to manipulate NULL-terminated
 * strings.  It is highly recommended that you use these functions in
 * preference to system functions, because the corresponding system
 * functions are extremely non-portable.
@*/

/*@
 * @deftypefun {unsigned int} jit_strlen (const char *@var{str})
 * Returns the length of @var{str}.
 * @end deftypefun
@*/
unsigned int jit_strlen(const char *str)
{
#ifdef HAVE_STRLEN
	return (unsigned int)(strlen(str));
#else
	unsigned int len = 0;
	while(*str++ != '\0')
	{
		++len;
	}
	return len;
#endif
}

/*@
 * @deftypefun {char *} jit_strcpy (char *@var{dest}, const char *@var{src})
 * Copy the string at @var{src} to @var{dest}.  Returns @var{dest}.
 * @end deftypefun
@*/
char *jit_strcpy(char *dest, const char *src)
{
#ifdef HAVE_STRCPY
	return strcpy(dest, src);
#else
	char ch;
	char *d = dest;
	while((ch = *src++) != '\0')
	{
		*d++ = ch;
	}
	*d = '\0';
	return dest;
#endif
}

/*@
 * @deftypefun {char *} jit_strcat (char *@var{dest}, const char *@var{src})
 * Copy the string at @var{src} to the end of the string at @var{dest}.
 * Returns @var{dest}.
 * @end deftypefun
@*/
char *jit_strcat(char *dest, const char *src)
{
#ifdef HAVE_STRCAT
	return strcat(dest, src);
#else
	char ch;
	char *d = dest + jit_strlen(dest);
	while((ch = *src++) != '\0')
	{
		*d++ = ch;
	}
	*d = '\0';
	return dest;
#endif
}

/*@
 * @deftypefun {char *} jit_strncpy (char *@var{dest}, const char *@var{src}, unsigned int @var{len})
 * Copy at most @var{len} characters from the string at @var{src} to
 * @var{dest}.  Returns @var{dest}.
 * @end deftypefun
@*/
char *jit_strncpy(char *dest, const char *src, unsigned int len)
{
#ifdef HAVE_STRNCPY
	return strncpy(dest, src, len);
#else
	char ch;
	char *d = dest;
	while(len > 0 && (ch = *src++) != '\0')
	{
		*d++ = ch;
		--len;
	}
	while(len > 0)
	{
		*d++ = '\0';
		--len;
	}
	return dest;
#endif
}

/*@
 * @deftypefun {char *} jit_strdup (const char *@var{str})
 * Allocate a block of memory using @code{jit_malloc} and copy
 * @var{str} into it.  Returns NULL if @var{str} is NULL or there
 * is insufficient memory to perform the @code{jit_malloc} operation.
 * @end deftypefun
@*/
char *jit_strdup(const char *str)
{
	char *new_str;
	if(!str)
	{
		return 0;
	}
	new_str = jit_malloc(strlen(str) + 1);
	if(!new_str)
	{
		return 0;
	}
	strcpy(new_str, str);
	return new_str;
}

/*@
 * @deftypefun {char *} jit_strndup (const char *@var{str}, unsigned int @var{len})
 * Allocate a block of memory using @code{jit_malloc} and copy at most
 * @var{len} characters of @var{str} into it.  The copied string is then
 * NULL-terminated.  Returns NULL if @var{str} is NULL or there
 * is insufficient memory to perform the @code{jit_malloc} operation.
 * @end deftypefun
@*/
char *jit_strndup(const char *str, unsigned int len)
{
	char *new_str;
	if(!str)
	{
		return 0;
	}
	new_str = jit_malloc(len + 1);
	if(!new_str)
	{
		return 0;
	}
	jit_memcpy(new_str, str, len);
	new_str[len] = '\0';
	return new_str;
}

/*@
 * @deftypefun int jit_strcmp (const char *@var{str1}, const char *@var{str2})
 * Compare the two strings @var{str1} and @var{str2}, returning
 * a negative, zero, or positive value depending upon their relationship.
 * @end deftypefun
@*/
int jit_strcmp(const char *str1, const char *str2)
{
#ifdef HAVE_STRCMP
	return strcmp(str1, str2);
#else
	int ch1, ch2;
	for(;;)
	{
		ch1 = *str1++;
		ch2 = *str2++;
		if(ch1 != ch2 || !ch1 || !ch2)
		{
			break;
		}
	}
	return (ch1 - ch2);
#endif
}

/*@
 * @deftypefun int jit_strncmp (const char *@var{str1}, const char *@var{str2}, unsigned int @var{len})
 * Compare the two strings @var{str1} and @var{str2}, returning
 * a negative, zero, or positive value depending upon their relationship.
 * At most @var{len} characters are compared.
 * @end deftypefun
@*/
int jit_strncmp(const char *str1, const char *str2, unsigned int len)
{
#ifdef HAVE_STRNCMP
	return strncmp(str1, str2, len);
#else
	int ch1, ch2;
	while(len > 0)
	{
		ch1 = *str1++;
		ch2 = *str2++;
		if(ch1 != ch2 || !ch1 || !ch2)
		{
			return (ch1 - ch2);
		}
		--len;
	}
	return 0;
#endif
}

/*@
 * @deftypefun int jit_stricmp (const char *@var{str1}, const char *@var{str2})
 * Compare the two strings @var{str1} and @var{str2}, returning
 * a negative, zero, or positive value depending upon their relationship.
 * Instances of the English letters A to Z are converted into their
 * lower case counterparts before comparison.
 *
 * Note: this function is guaranteed to use English case comparison rules,
 * no matter what the current locale is set to, making it suitable for
 * comparing token tags and simple programming language identifiers.
 *
 * Locale-sensitive string comparison is complicated and usually specific
 * to the front end language or its supporting runtime library.  We
 * deliberately chose not to handle this in @code{libjit}.
 * @end deftypefun
@*/
int jit_stricmp(const char *str1, const char *str2)
{
	int ch1, ch2;
	for(;;)
	{
		ch1 = *str1++;
		ch2 = *str2++;
		if(ch1 >= 'A' && ch1 <= 'Z')
		{
			ch1 = ch1 - 'A' + 'a';
		}
		if(ch2 >= 'A' && ch2 <= 'Z')
		{
			ch2 = ch2 - 'A' + 'a';
		}
		if(ch1 != ch2 || !ch1 || !ch2)
		{
			break;
		}
	}
	return (ch1 - ch2);
}

/*@
 * @deftypefun int jit_strnicmp (const char *@var{str1}, const char *@var{str2}, unsigned int @var{len})
 * Compare the two strings @var{str1} and @var{str2}, returning
 * a negative, zero, or positive value depending upon their relationship.
 * At most @var{len} characters are compared.  Instances of the English
 * letters A to Z are converted into their lower case counterparts
 * before comparison.
 * @end deftypefun
@*/
int jit_strnicmp(const char *str1, const char *str2, unsigned int len)
{
	int ch1, ch2;
	while(len > 0)
	{
		ch1 = *str1++;
		ch2 = *str2++;
		if(ch1 >= 'A' && ch1 <= 'Z')
		{
			ch1 = ch1 - 'A' + 'a';
		}
		if(ch2 >= 'A' && ch2 <= 'Z')
		{
			ch2 = ch2 - 'A' + 'a';
		}
		if(ch1 != ch2 || !ch1 || !ch2)
		{
			return (ch1 - ch2);
		}
		--len;
	}
	return 0;
}

/*@
 * @deftypefun {char *} jit_strchr (const char *@var{str}, int @var{ch})
 * Search @var{str} for the first occurrence of @var{ch}.  Returns
 * the address where @var{ch} was found, or NULL if not found.
 * @end deftypefun
@*/
char *jit_strchr(const char *str, int ch)
{
#ifdef HAVE_STRCHR
	return strchr(str, ch);
#else
	char *s = (char *)str;
	for(;;)
	{
		if(*s == (char)ch)
		{
			return s;
		}
		else if(*s == '\0')
		{
			break;
		}
		++s;
	}
	return 0;
#endif
}

/*@
 * @deftypefun {char *} jit_strrchr (const char *@var{str}, int @var{ch})
 * Search @var{str} for the first occurrence of @var{ch}, starting
 * at the end of the string.  Returns the address where @var{ch}
 * was found, or NULL if not found.
 * @end deftypefun
@*/
char *jit_strrchr(const char *str, int ch)
{
#ifdef HAVE_STRRCHR
	return strrchr(str, ch);
#else
	unsigned int len = jit_strlen(str);
	char *s = (char *)(str + len);
	while(len > 0)
	{
		--s;
		if(*s == (char)ch)
		{
			return s;
		}
		--len;
	}
	return 0;
#endif
}

int jit_sprintf(char *str, const char *format, ...)
{
	va_list va;
	int result;
#ifdef HAVE_STDARG_H
	va_start(va, format);
#else
	va_start(va);
#endif
#ifdef VSPRINTF
	result = vsprintf(str, format, va);
#else
	*str = '\0';
	result = 0;
#endif
	va_end(va);
	return result;
}

int jit_snprintf(char *str, unsigned int len, const char *format, ...)
{
	va_list va;
	int result;
#ifdef HAVE_STDARG_H
	va_start(va, format);
#else
	va_start(va);
#endif
#if defined(HAVE_VSNPRINTF)
	result = vsnprintf(str, len, format, va);
#elif defined(HAVE__VSNPRINTF)
	result = _vsnprintf(str, len, format, va);
#else
	*str = '\0';
	result = 0;
#endif
	va_end(va);
	return result;
}
