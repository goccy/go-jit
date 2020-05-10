/*
 * jit-type.c - Functions for manipulating type descriptors.
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
#include "jit-apply-rules.h"
#include "jit-rules.h"

/*@

@cindex jit-type.h
@tindex jit_type_t

The functions that are defined in @code{<jit/jit-type.h>} allow
the library user to create and manipulate objects that represent
native system types.  For example, @code{jit_type_int} represents
the signed 32-bit integer type.

Each @code{jit_type_t} object represents a basic system type,
be it a primitive, a struct, a union, a pointer, or a function signature.
The library uses this information to lay out values in memory.

The following pre-defined types are available:

@table @code
@vindex jit_type_void
@item jit_type_void
Represents the @code{void} type.

@vindex jit_type_sbyte
@item jit_type_sbyte
Represents a signed 8-bit integer type.

@vindex jit_type_ubyte
@item jit_type_ubyte
Represents an unsigned 8-bit integer type.

@vindex jit_type_short
@item jit_type_short
Represents a signed 16-bit integer type.

@vindex jit_type_ushort
@item jit_type_ushort
Represents an unsigned 16-bit integer type.

@vindex jit_type_int
@item jit_type_int
Represents a signed 32-bit integer type.

@vindex jit_type_uint
@item jit_type_uint
Represents an unsigned 32-bit integer type.

@vindex jit_type_nint
@item jit_type_nint
Represents a signed integer type that has the same size and
alignment as a native pointer.

@vindex jit_type_nuint
@item jit_type_nuint
Represents an unsigned integer type that has the same size and
alignment as a native pointer.

@vindex jit_type_long
@item jit_type_long
Represents a signed 64-bit integer type.

@vindex jit_type_ulong
@item jit_type_ulong
Represents an unsigned 64-bit integer type.

@vindex jit_type_float32
@item jit_type_float32
Represents a 32-bit floating point type.

@vindex jit_type_float64
@item jit_type_float64
Represents a 64-bit floating point type.

@vindex jit_type_nfloat
@item jit_type_nfloat
Represents a floating point type that represents the greatest
precision supported on the native platform.

@vindex jit_type_void_ptr
@item jit_type_void_ptr
Represents the system's @code{void *} type.  This can be used wherever
a native pointer type is required.
@end table

Type descriptors are reference counted.  You can make a copy of a type
descriptor using the @code{jit_type_copy} function, and free the copy with
@code{jit_type_free}.

Some languages have special versions of the primitive numeric types
(e.g. boolean types, 16-bit Unicode character types, enumerations, etc).
If it is important to distinguish these special versions from the
numeric types, then you should use the @code{jit_type_create_tagged}
function below.

The following types correspond to the system types on the local
platform.  i.e. @code{jit_type_sys_int} will be the same size as
@code{long} on the local platform, whereas @code{jit_type_long} is
always 64 bits in size.  These types should not be used to compile
code that is intended to work identically on all platforms:

@table @code
@vindex jit_type_sys_bool
@item jit_type_sys_bool
Corresponds to the system @code{bool} type.

@vindex jit_type_sys_char
@item jit_type_sys_char
Corresponds to the system @code{char} type.  This may be either signed
or unsigned, depending upon the underlying system.

@vindex jit_type_sys_schar
@item jit_type_sys_schar
Corresponds to the system @code{signed char} type.

@vindex jit_type_sys_uchar
@item jit_type_sys_uchar
Corresponds to the system @code{unsigned char} type.

@vindex jit_type_sys_short
@item jit_type_sys_short
Corresponds to the system @code{short} type.

@vindex jit_type_sys_ushort
@item jit_type_sys_ushort
Corresponds to the system @code{unsigned short} type.

@vindex jit_type_sys_int
@item jit_type_sys_int
Corresponds to the system @code{int} type.

@vindex jit_type_sys_uint
@item jit_type_sys_uint
Corresponds to the system @code{unsigned int} type.

@vindex jit_type_sys_long
@item jit_type_sys_long
Corresponds to the system @code{long} type.

@vindex jit_type_sys_ulong
@item jit_type_sys_ulong
Corresponds to the system @code{unsigned long} type.

@vindex jit_type_sys_longlong
@item jit_type_sys_longlong
Corresponds to the system @code{long long} type (@code{__int64} under Win32).

@vindex jit_type_sys_ulonglong
@item jit_type_sys_ulonglong
Corresponds to the system @code{unsigned long long} type
(@code{unsigned __int64} under Win32).

@vindex jit_type_sys_float
@item jit_type_sys_float
Corresponds to the system @code{float} type.

@vindex jit_type_sys_double
@item jit_type_sys_double
Corresponds to the system @code{double} type.

@vindex jit_type_sys_long_double
@item jit_type_sys_long_double
Corresponds to the system @code{long double} type.
@end table

@*/

/*
 * Pre-defined type descriptors.
 */
struct _jit_type const _jit_type_void_def =
	{1, JIT_TYPE_VOID, 0, 1, 0, 1, 1};
jit_type_t const jit_type_void = (jit_type_t)&_jit_type_void_def;
struct _jit_type const _jit_type_sbyte_def =
	{1, JIT_TYPE_SBYTE, 0, 1, 0, sizeof(jit_sbyte), JIT_ALIGN_SBYTE};
jit_type_t const jit_type_sbyte = (jit_type_t)&_jit_type_sbyte_def;
struct _jit_type const _jit_type_ubyte_def =
	{1, JIT_TYPE_UBYTE, 0, 1, 0, sizeof(jit_ubyte), JIT_ALIGN_UBYTE};
jit_type_t const jit_type_ubyte = (jit_type_t)&_jit_type_ubyte_def;
struct _jit_type const _jit_type_short_def =
	{1, JIT_TYPE_SHORT, 0, 1, 0, sizeof(jit_short), JIT_ALIGN_SHORT};
jit_type_t const jit_type_short = (jit_type_t)&_jit_type_short_def;
struct _jit_type const _jit_type_ushort_def =
	{1, JIT_TYPE_USHORT, 0, 1, 0, sizeof(jit_ushort), JIT_ALIGN_USHORT};
jit_type_t const jit_type_ushort = (jit_type_t)&_jit_type_ushort_def;
struct _jit_type const _jit_type_int_def =
	{1, JIT_TYPE_INT, 0, 1, 0, sizeof(jit_int), JIT_ALIGN_INT};
jit_type_t const jit_type_int = (jit_type_t)&_jit_type_int_def;
struct _jit_type const _jit_type_uint_def =
	{1, JIT_TYPE_UINT, 0, 1, 0, sizeof(jit_uint), JIT_ALIGN_UINT};
jit_type_t const jit_type_uint = (jit_type_t)&_jit_type_uint_def;
struct _jit_type const _jit_type_nint_def =
	{1, JIT_TYPE_NINT, 0, 1, 0, sizeof(jit_nint), JIT_ALIGN_NINT};
jit_type_t const jit_type_nint = (jit_type_t)&_jit_type_nint_def;
struct _jit_type const _jit_type_nuint_def =
	{1, JIT_TYPE_NUINT, 0, 1, 0, sizeof(jit_nuint), JIT_ALIGN_NUINT};
jit_type_t const jit_type_nuint = (jit_type_t)&_jit_type_nuint_def;
struct _jit_type const _jit_type_long_def =
	{1, JIT_TYPE_LONG, 0, 1, 0, sizeof(jit_long), JIT_ALIGN_LONG};
jit_type_t const jit_type_long = (jit_type_t)&_jit_type_long_def;
struct _jit_type const _jit_type_ulong_def =
	{1, JIT_TYPE_ULONG, 0, 1, 0, sizeof(jit_ulong), JIT_ALIGN_ULONG};
jit_type_t const jit_type_ulong = (jit_type_t)&_jit_type_ulong_def;
struct _jit_type const _jit_type_float32_def =
	{1, JIT_TYPE_FLOAT32, 0, 1, 0, sizeof(jit_float32), JIT_ALIGN_FLOAT32};
jit_type_t const jit_type_float32 = (jit_type_t)&_jit_type_float32_def;
struct _jit_type const _jit_type_float64_def =
	{1, JIT_TYPE_FLOAT64, 0, 1, 0, sizeof(jit_float64), JIT_ALIGN_FLOAT64};
jit_type_t const jit_type_float64 = (jit_type_t)&_jit_type_float64_def;
struct _jit_type const _jit_type_nfloat_def =
	{1, JIT_TYPE_NFLOAT, 0, 1, 0, sizeof(jit_nfloat), JIT_ALIGN_NFLOAT};
jit_type_t const jit_type_nfloat = (jit_type_t)&_jit_type_nfloat_def;
struct _jit_type const _jit_type_void_ptr_def =
	{1, JIT_TYPE_PTR, 0, 1, 0, sizeof(void *), JIT_ALIGN_PTR,
	 (jit_type_t)&_jit_type_void_def};
jit_type_t const jit_type_void_ptr = (jit_type_t)&_jit_type_void_ptr_def;

/*
 * Type descriptors for the system "char", "int", "long", etc types.
 * These are defined to one of the above values, tagged with a value
 * that indicates which system type it is referring to.
 */
#define DECLARE_TAGGED(name,real,tag)	\
static struct jit_tagged_type const name##_tagged = \
	{{1, JIT_TYPE_FIRST_TAGGED + (tag), 0, 1, 0, 0, 0, \
	 (jit_type_t)&_jit_type_##real}, 0, 0}; \
jit_type_t const jit_type_##name = (jit_type_t)&name##_tagged
DECLARE_TAGGED(sys_bool, ubyte_def, JIT_TYPETAG_SYS_BOOL);
#ifdef __CHAR_UNSIGNED__
DECLARE_TAGGED(sys_char, ubyte_def, JIT_TYPETAG_SYS_CHAR);
#else
DECLARE_TAGGED(sys_char, sbyte_def, JIT_TYPETAG_SYS_CHAR);
#endif
DECLARE_TAGGED(sys_schar, sbyte_def, JIT_TYPETAG_SYS_SCHAR);
DECLARE_TAGGED(sys_uchar, ubyte_def, JIT_TYPETAG_SYS_UCHAR);
#if SIZEOF_SHORT == 4
DECLARE_TAGGED(sys_short, int_def, JIT_TYPETAG_SYS_SHORT);
DECLARE_TAGGED(sys_ushort, uint_def, JIT_TYPETAG_SYS_USHORT);
#elif SIZEOF_SHORT == 8
DECLARE_TAGGED(sys_short, long_def, JIT_TYPETAG_SYS_SHORT);
DECLARE_TAGGED(sys_ushort, ulong_def, JIT_TYPETAG_SYS_USHORT);
#else
DECLARE_TAGGED(sys_short, short_def, JIT_TYPETAG_SYS_SHORT);
DECLARE_TAGGED(sys_ushort, ushort_def, JIT_TYPETAG_SYS_USHORT);
#endif
#if SIZEOF_INT == 8
DECLARE_TAGGED(sys_int, long_def, JIT_TYPETAG_SYS_INT);
DECLARE_TAGGED(sys_uint, ulong_def, JIT_TYPETAG_SYS_UINT);
#elif SIZEOF_INT == 2
DECLARE_TAGGED(sys_int, short_def, JIT_TYPETAG_SYS_INT);
DECLARE_TAGGED(sys_uint, ushort_def, JIT_TYPETAG_SYS_UINT);
#else
DECLARE_TAGGED(sys_int, int_def, JIT_TYPETAG_SYS_INT);
DECLARE_TAGGED(sys_uint, uint_def, JIT_TYPETAG_SYS_UINT);
#endif
#if SIZEOF_LONG == 8
DECLARE_TAGGED(sys_long, long_def, JIT_TYPETAG_SYS_LONG);
DECLARE_TAGGED(sys_ulong, ulong_def, JIT_TYPETAG_SYS_ULONG);
#elif SIZEOF_LONG == 2
DECLARE_TAGGED(sys_long, short_def, JIT_TYPETAG_SYS_LONG);
DECLARE_TAGGED(sys_ulong, ushort_def, JIT_TYPETAG_SYS_ULONG);
#else
DECLARE_TAGGED(sys_long, int_def, JIT_TYPETAG_SYS_LONG);
DECLARE_TAGGED(sys_ulong, uint_def, JIT_TYPETAG_SYS_ULONG);
#endif
#if SIZEOF_LONG_LONG == 8 || SIZEOF___INT64 == 8
DECLARE_TAGGED(sys_longlong, long_def, JIT_TYPETAG_SYS_LONGLONG);
DECLARE_TAGGED(sys_ulonglong, ulong_def, JIT_TYPETAG_SYS_ULONGLONG);
#elif SIZEOF_LONG_LONG == 4
DECLARE_TAGGED(sys_longlong, int_def, JIT_TYPETAG_SYS_LONGLONG);
DECLARE_TAGGED(sys_ulonglong, uint_def, JIT_TYPETAG_SYS_ULONGLONG);
#elif SIZEOF_LONG_LONG == 2
DECLARE_TAGGED(sys_longlong, short_def, JIT_TYPETAG_SYS_LONGLONG);
DECLARE_TAGGED(sys_ulonglong, ushort_def, JIT_TYPETAG_SYS_ULONGLONG);
#else
DECLARE_TAGGED(sys_longlong, long_def, JIT_TYPETAG_SYS_LONGLONG);
DECLARE_TAGGED(sys_ulonglong, ulong_def, JIT_TYPETAG_SYS_ULONGLONG);
#endif
DECLARE_TAGGED(sys_float, float32_def, JIT_TYPETAG_SYS_FLOAT);
DECLARE_TAGGED(sys_double, float64_def, JIT_TYPETAG_SYS_DOUBLE);
DECLARE_TAGGED(sys_long_double, nfloat_def, JIT_TYPETAG_SYS_LONGDOUBLE);

/*
 * Special offset flags.
 */
#define	JIT_OFFSET_IS_INTERNAL	(((jit_nuint)1) << (sizeof(jit_nint) * 8 - 1))
#define	JIT_OFFSET_NOT_SET		(~((jit_nuint)0))

/*
 * Layout flags.
 */
#define	JIT_LAYOUT_NEEDED			1
#define	JIT_LAYOUT_EXPLICIT_SIZE	2
#define	JIT_LAYOUT_EXPLICIT_ALIGN	4

/*
 * Perform layout on a structure or union type.
 */
static void perform_layout(jit_type_t type)
{
	jit_nuint size = 0;
	jit_nuint maxSize = 0;
	jit_nuint maxAlign = 1;
	jit_nuint alignLimit;
	jit_nuint fieldSize;
	jit_nuint fieldAlign;
	unsigned int index;

	/* Determine the alignment limit, if there is an override */
#ifdef JIT_ALIGN_OVERRIDES
	if((type->layout_flags & JIT_LAYOUT_EXPLICIT_ALIGN) != 0)
	{
		alignLimit = type->alignment;
	}
	else
#endif
	{
		alignLimit = 0;
	}

	/* Lay out all of the fields in this structure */
	for(index = 0; index < type->num_components; ++index)
	{
		/* Get the size and alignment of the field */
		fieldSize = jit_type_get_size(type->components[index].type);
		fieldAlign = jit_type_get_alignment(type->components[index].type);

		/* Clamp the alignment if we have a limit */
		if(alignLimit != 0 && fieldAlign > alignLimit)
		{
			fieldAlign = alignLimit;
		}

		/* Update the size and alignment values */
		if(type->kind == JIT_TYPE_STRUCT)
		{
			/* Perform layout for a struct type */
			if((type->components[index].offset & JIT_OFFSET_IS_INTERNAL) != 0)
			{
				/* Calculate the offset for the field automatically */
				if((size % fieldAlign) != 0)
				{
					size += fieldAlign - (size % fieldAlign);
				}
				type->components[index].offset = JIT_OFFSET_IS_INTERNAL | size;
				size += fieldSize;
			}
			else
			{
				/* Use the explicitly-supplied offset for the field */
				size = type->components[index].offset + fieldSize;
			}
			if(size > maxSize)
			{
				maxSize = size;
			}
		}
		else
		{
			/* Perform layout for a union type (offset is always zero) */
			type->components[index].offset = JIT_OFFSET_IS_INTERNAL | 0;
			if((fieldSize % fieldAlign) != 0)
			{
				fieldSize += fieldAlign - (fieldSize % fieldAlign);
			}
			if(fieldSize > maxSize)
			{
				maxSize = fieldSize;
			}
		}
		if(fieldAlign > maxAlign)
		{
			maxAlign = fieldAlign;
		}
	}

	/* Align the full structure */
	if((maxSize % maxAlign) != 0)
	{
		maxSize += maxAlign - (maxSize % maxAlign);
	}

	/* Record the final size and alignment values */
	if((type->layout_flags & JIT_LAYOUT_EXPLICIT_SIZE) != 0)
	{
		if(maxSize > type->size)
		{
			type->size = maxSize;
		}
	}
	else
	{
		type->size = maxSize;
	}
	if(maxAlign > type->alignment)
	{
		type->alignment = maxAlign;
	}
}

/*@
 * @deftypefun jit_type_t jit_type_copy (jit_type_t @var{type})
 * Make a copy of the type descriptor @var{type} by increasing
 * its reference count.
 * @end deftypefun
@*/
jit_type_t jit_type_copy(jit_type_t type)
{
	if(!type || type->is_fixed)
	{
		return type;
	}
	++(type->ref_count);
	return type;
}

/*@
 * @deftypefun void jit_type_free (jit_type_t @var{type})
 * Free a type descriptor by decreasing its reference count.
 * This function is safe to use on pre-defined types, which are
 * never actually freed.
 * @end deftypefun
@*/
void jit_type_free(jit_type_t type)
{
	unsigned int index;
	if(!type || type->is_fixed)
	{
		return;
	}
	if(--(type->ref_count) != 0)
	{
		return;
	}
	jit_type_free(type->sub_type);
	for(index = 0; index < type->num_components; ++index)
	{
		jit_type_free(type->components[index].type);
		if(type->components[index].name)
		{
			jit_free(type->components[index].name);
		}
	}
	if(type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		struct jit_tagged_type *tagged = (struct jit_tagged_type *)type;
		if(tagged->free_func)
		{
			(*(tagged->free_func))(tagged->data);
		}
	}
	jit_free(type);
}

static jit_type_t create_complex(int kind, jit_type_t *types,
								 unsigned int num, int incref)
{
	jit_type_t type;
	unsigned int index;
	if(num <= 1)
	{
		type = jit_cnew(struct _jit_type);
	}
	else
	{
		type = (jit_type_t)jit_calloc
				(1, sizeof(struct _jit_type) +
				    (num - 1) * sizeof(struct jit_component));
	}
	if(!type)
	{
		return 0;
	}
	type->ref_count = 1;
	type->kind = kind;
	type->layout_flags = JIT_LAYOUT_NEEDED;
	type->num_components = num;
	for(index = 0; index < num; ++index)
	{
		if(incref)
		{
			type->components[index].type = jit_type_copy(types[index]);
		}
		else
		{
			type->components[index].type = types[index];
		}
		type->components[index].offset = JIT_OFFSET_NOT_SET;
		type->components[index].name = 0;
	}
	return type;
}

/*@
 * @deftypefun jit_type_t jit_type_create_struct (jit_type_t *@var{fields}, unsigned int @var{num_fields}, int @var{incref})
 * Create a type descriptor for a structure.  Returns NULL if out of memory.
 * If there are no fields, then the size of the structure will be zero.
 * It is necessary to add a padding field if the language does not allow
 * zero-sized structures.  The reference counts on the field types are
 * incremented if @var{incref} is non-zero.
 *
 * The @code{libjit} library does not provide any special support for
 * implementing structure inheritance, where one structure extends the
 * definition of another.  The effect of inheritance can be achieved
 * by always allocating the first field of a structure to be an instance
 * of the inherited structure.  Multiple inheritance can be supported
 * by allocating several special fields at the front of an inheriting
 * structure.
 *
 * Similarly, no special support is provided for vtables.  The program
 * is responsible for allocating an appropriate slot in a structure to
 * contain the vtable pointer, and dereferencing it wherever necessary.
 * The vtable will itself be a structure, containing signature types
 * for each of the method slots.
 *
 * The choice not to provide special support for inheritance and vtables
 * in @code{libjit} was deliberate.  The layout of objects and vtables
 * is highly specific to the language and virtual machine being emulated,
 * and no single scheme can hope to capture all possibilities.
 * @end deftypefun
@*/
jit_type_t jit_type_create_struct(jit_type_t *fields, unsigned int num_fields,
								  int incref)
{
	return create_complex(JIT_TYPE_STRUCT, fields, num_fields, incref);
}

/*@
 * @deftypefun jit_type_t jit_type_create_union (jit_type_t *@var{fields}, unsigned int @var{num_fields}, int @var{incref})
 * Create a type descriptor for a union.  Returns NULL if out of memory.
 * If there are no fields, then the size of the union will be zero.
 * It is necessary to add a padding field if the language does not allow
 * zero-sized unions.  The reference counts on the field types are
 * incremented if @var{incref} is non-zero.
 * @end deftypefun
@*/
jit_type_t jit_type_create_union(jit_type_t *fields, unsigned int num_fields,
								 int incref)
{
	return create_complex(JIT_TYPE_UNION, fields, num_fields, incref);
}

/*@
 * @deftypefun jit_type_t jit_type_create_signature (jit_abi_t @var{abi}, jit_type_t @var{return_type}, jit_type_t *@var{params}, unsigned int @var{num_params}, int @var{incref})
 * Create a type descriptor for a function signature.  Returns NULL if out
 * of memory.  The reference counts on the component types are incremented
 * if @var{incref} is non-zero.
 *
 * When used as a structure or union field, function signatures are laid
 * out like pointers.  That is, they represent a pointer to a function
 * that has the specified parameters and return type.
 *
 * @tindex jit_abi_t
 * The @var{abi} parameter specifies the Application Binary Interface (ABI)
 * that the function uses.  It may be one of the following values:
 *
 * @table @code
 * @vindex jit_abi_cdecl
 * @item jit_abi_cdecl
 * Use the native C ABI definitions of the underlying platform.
 *
 * @vindex jit_abi_vararg
 * @item jit_abi_vararg
 * Use the native C ABI definitions of the underlying platform,
 * and allow for an optional list of variable argument parameters.
 *
 * @vindex jit_abi_stdcall
 * @item jit_abi_stdcall
 * Use the Win32 STDCALL ABI definitions, whereby the callee pops
 * its arguments rather than the caller.  If the platform does
 * not support this type of ABI, then @code{jit_abi_stdcall} will be
 * identical to @code{jit_abi_cdecl}.
 *
 * @vindex jit_abi_fastcall
 * @item jit_abi_fastcall
 * Use the Win32 FASTCALL ABI definitions, whereby the callee pops
 * its arguments rather than the caller, and the first two word
 * arguments are passed in ECX and EDX.  If the platform does
 * not support this type of ABI, then @code{jit_abi_fastcall} will be
 * identical to @code{jit_abi_cdecl}.
 * @end table
 * @end deftypefun
@*/
jit_type_t jit_type_create_signature(jit_abi_t abi, jit_type_t return_type,
                                     jit_type_t *params,
									 unsigned int num_params, int incref)
{
	jit_type_t type;
	type = create_complex(JIT_TYPE_SIGNATURE, params, num_params, incref);
	if(type)
	{
		type->abi = (int)abi;
		type->layout_flags = 0;
		type->size = 0;
		type->alignment = JIT_ALIGN_PTR;
		if(incref)
		{
			type->sub_type = jit_type_copy(return_type);
		}
		else
		{
			type->sub_type = return_type;
		}
	}
	return type;
}

/*@
 * @deftypefun jit_type_t jit_type_create_pointer (jit_type_t @var{type}, int @var{incref})
 * Create a type descriptor for a pointer to another type.  Returns NULL
 * if out of memory.  The reference count on @var{type} is incremented if
 * @var{incref} is non-zero.
 * @end deftypefun
@*/
jit_type_t jit_type_create_pointer(jit_type_t type, int incref)
{
	jit_type_t ntype;
	if(type == jit_type_void)
	{
		return jit_type_void_ptr;
	}
	if((ntype = jit_cnew(struct _jit_type)) == 0)
	{
		return 0;
	}
	ntype->ref_count = 1;
	ntype->kind = JIT_TYPE_PTR;
	ntype->size = sizeof(void *);
	ntype->alignment = JIT_ALIGN_PTR;
	if(incref)
	{
		ntype->sub_type = jit_type_copy(type);
	}
	else
	{
		ntype->sub_type = type;
	}
	return ntype;
}

/*@
 * @deftypefun jit_type_t jit_type_create_tagged (jit_type_t @var{type}, int @var{kind}, void *@var{data}, jit_meta_free_func @var{free_func}, int @var{incref})
 * Tag a type with some additional user data.  Tagging is typically used by
 * higher-level programs to embed extra information about a type that
 * @code{libjit} itself does not support.
 *
 * As an example, a language might have a 16-bit Unicode character type
 * and a 16-bit unsigned integer type that are distinct types, even though
 * they share the same fundamental representation (@code{jit_ushort}).
 * Tagging allows the program to distinguish these two types, when
 * it is necessary to do so, without affecting @code{libjit}'s ability
 * to compile the code efficiently.
 *
 * The @var{kind} is a small positive integer value that the program
 * can use to distinguish multiple tag types.  The @var{data} pointer is
 * the actual data that you wish to store.  And @var{free_func} is a
 * function that is used to free @var{data} when the type is freed
 * with @code{jit_type_free}.
 *
 * If you need to store more than one piece of information, you can
 * tag a type multiple times.  The order in which multiple tags are
 * applied is irrelevant to @code{libjit}, although it may be relevant
 * to the higher-level program.
 *
 * Tag kinds of 10000 or greater are reserved for @code{libjit} itself.
 * The following special tag kinds are currently provided in the
 * base implementation:
 *
 * @table @code
 * @vindex JIT_TYPETAG_NAME
 * @item JIT_TYPETAG_NAME
 * The @var{data} pointer is a @code{char *} string indicating a friendly
 * name to display for the type.
 *
 * @vindex JIT_TYPETAG_STRUCT_NAME
 * @vindex JIT_TYPETAG_UNION_NAME
 * @vindex JIT_TYPETAG_ENUM_NAME
 * @item JIT_TYPETAG_STRUCT_NAME
 * @itemx JIT_TYPETAG_UNION_NAME
 * @itemx JIT_TYPETAG_ENUM_NAME
 * The @var{data} pointer is a @code{char *} string indicating a friendly
 * name to display for a @code{struct}, @code{union}, or @code{enum} type.
 * This is for languages like C that have separate naming scopes for
 * typedef's and structures.
 *
 * @vindex JIT_TYPETAG_CONST
 * @item JIT_TYPETAG_CONST
 * The underlying value is assumed to have @code{const} semantics.
 * The @code{libjit} library doesn't enforce such semantics: it is
 * up to the front-end to only use constant values in appopriate contexts.
 *
 * @vindex JIT_TYPETAG_VOLATILE
 * @item JIT_TYPETAG_VOLATILE
 * The underlying value is assumed to be volatile.  The @code{libjit}
 * library will automatically call @code{jit_value_set_volatile} when a
 * value is constructed using this type.
 *
 * @vindex JIT_TYPETAG_REFERENCE
 * @item JIT_TYPETAG_REFERENCE
 * The underlying value is a pointer, but it is assumed to refer to a
 * pass-by-reference parameter.
 *
 * @vindex JIT_TYPETAG_OUTPUT
 * @item JIT_TYPETAG_OUTPUT
 * This is similar to @code{JIT_TYPETAG_REFERENCE}, except that the
 * underlying parameter is assumed to be output-only.
 *
 * @vindex JIT_TYPETAG_RESTRICT
 * @item JIT_TYPETAG_RESTRICT
 * The underlying type is marked as @code{restrict}.  Normally ignored.
 *
 * @vindex JIT_TYPETAG_SYS_BOOL
 * @vindex JIT_TYPETAG_SYS_CHAR
 * @vindex JIT_TYPETAG_SYS_SCHAR
 * @vindex JIT_TYPETAG_SYS_UCHAR
 * @vindex JIT_TYPETAG_SYS_SHORT
 * @vindex JIT_TYPETAG_SYS_USHORT
 * @vindex JIT_TYPETAG_SYS_INT
 * @vindex JIT_TYPETAG_SYS_UINT
 * @vindex JIT_TYPETAG_SYS_LONG
 * @vindex JIT_TYPETAG_SYS_ULONG
 * @vindex JIT_TYPETAG_SYS_LONGLONG
 * @vindex JIT_TYPETAG_SYS_ULONGLONG
 * @vindex JIT_TYPETAG_SYS_FLOAT
 * @vindex JIT_TYPETAG_SYS_DOUBLE
 * @vindex JIT_TYPETAG_SYS_LONGDOUBLE
 * @item JIT_TYPETAG_SYS_BOOL
 * @itemx JIT_TYPETAG_SYS_CHAR
 * @itemx JIT_TYPETAG_SYS_SCHAR
 * @itemx JIT_TYPETAG_SYS_UCHAR
 * @itemx JIT_TYPETAG_SYS_SHORT
 * @itemx JIT_TYPETAG_SYS_USHORT
 * @itemx JIT_TYPETAG_SYS_INT
 * @itemx JIT_TYPETAG_SYS_UINT
 * @itemx JIT_TYPETAG_SYS_LONG
 * @itemx JIT_TYPETAG_SYS_ULONG
 * @itemx JIT_TYPETAG_SYS_LONGLONG
 * @itemx JIT_TYPETAG_SYS_ULONGLONG
 * @itemx JIT_TYPETAG_SYS_FLOAT
 * @itemx JIT_TYPETAG_SYS_DOUBLE
 * @itemx JIT_TYPETAG_SYS_LONGDOUBLE
 * Used to mark types that we know for a fact correspond to the system
 * C types of the corresponding names.  This is primarily used to distinguish
 * system types like @code{int} and @code{long} types on 32-bit platforms
 * when it is necessary to do so.  The @code{jit_type_sys_xxx} values are
 * all tagged in this manner.
 * @end table
 * @end deftypefun
@*/
jit_type_t jit_type_create_tagged(jit_type_t type, int kind, void *data,
								  jit_meta_free_func free_func, int incref)
{
	struct jit_tagged_type *ntype;
	if((ntype = jit_cnew(struct jit_tagged_type)) == 0)
	{
		return 0;
	}
	ntype->type.ref_count = 1;
	ntype->type.kind = JIT_TYPE_FIRST_TAGGED + kind;
	ntype->type.size = 0;
	ntype->type.alignment = 1;
	if(incref)
	{
		ntype->type.sub_type = jit_type_copy(type);
	}
	else
	{
		ntype->type.sub_type = type;
	}
	ntype->data = data;
	ntype->free_func = free_func;
	return &(ntype->type);
}

/*@
 * @deftypefun int jit_type_set_names (jit_type_t @var{type}, char **@var{names}, unsigned int @var{num_names})
 * Set the field or parameter names for @var{type}.  Returns zero
 * if there is insufficient memory to set the names.
 *
 * Normally fields are accessed via their index.  Field names are a
 * convenience for front ends that prefer to use names to indices.
 * @end deftypefun
@*/
int jit_type_set_names(jit_type_t type, char **names, unsigned int num_names)
{
	char *temp;
	if(!type || type->is_fixed || !names)
	{
		return 1;
	}
	if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION ||
	   type->kind == JIT_TYPE_SIGNATURE)
	{
		if(num_names > type->num_components)
		{
			num_names = type->num_components;
		}
		while(num_names > 0)
		{
			--num_names;
			if(type->components[num_names].name)
			{
				jit_free(type->components[num_names].name);
				type->components[num_names].name = 0;
			}
			if(names[num_names])
			{
				temp = jit_strdup(names[num_names]);
				if(!temp)
				{
					return 0;
				}
				type->components[num_names].name = temp;
			}
		}
	}
	return 1;
}

/*@
 * @deftypefun void jit_type_set_size_and_alignment (jit_type_t @var{type}, jit_nint @var{size}, jit_nint @var{alignment})
 * Set the size and alignment information for a structure or union
 * type.  Use this for performing explicit type layout.  Normally
 * the size is computed automatically.  Ignored if not a
 * structure or union type.  Setting either value to -1 will cause
 * that value to be computed automatically.
 * @end deftypefun
@*/
void jit_type_set_size_and_alignment(jit_type_t type, jit_nint size,
									 jit_nint alignment)
{
	if(!type)
	{
		return;
	}
	if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION)
	{
		type->size = (jit_nuint)size;
		type->alignment = (jit_nuint)alignment;
		if(size != -1)
		{
			type->layout_flags |= JIT_LAYOUT_EXPLICIT_SIZE;
		}
		if(alignment != -1)
		{
			type->layout_flags |= JIT_LAYOUT_EXPLICIT_ALIGN;
		}
		type->layout_flags |= JIT_LAYOUT_NEEDED;
	}
}

/*@
 * @deftypefun void jit_type_set_offset (jit_type_t @var{type}, unsigned int @var{field_index}, jit_nuint @var{offset})
 * Set the offset of a specific structure field.  Use this for
 * performing explicit type layout.  Normally the offset is
 * computed automatically.  Ignored if not a structure type,
 * or the field index is out of range.
 * @end deftypefun
@*/
void jit_type_set_offset(jit_type_t type, unsigned int field_index,
						 jit_nuint offset)
{
	if(!type || field_index >= type->num_components)
	{
		return;
	}
	if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION)
	{
		type->components[field_index].offset = offset;
		type->layout_flags |= JIT_LAYOUT_NEEDED;
	}
}

/*@
 * @deftypefun int jit_type_get_kind (jit_type_t @var{type})
 * Get a value that indicates the kind of @var{type}.  This allows
 * callers to quickly classify a type to determine how it should be
 * handled further.
 *
 * @table @code
 * @vindex JIT_TYPE_INVALID
 * @item JIT_TYPE_INVALID
 * The value of the @var{type} parameter is NULL.
 *
 * @vindex JIT_TYPE_VOID
 * @item JIT_TYPE_VOID
 * The type is @code{jit_type_void}.
 *
 * @vindex JIT_TYPE_SBYTE
 * @item JIT_TYPE_SBYTE
 * The type is @code{jit_type_sbyte}.
 *
 * @vindex JIT_TYPE_UBYTE
 * @item JIT_TYPE_UBYTE
 * The type is @code{jit_type_ubyte}.
 *
 * @vindex JIT_TYPE_SHORT
 * @item JIT_TYPE_SHORT
 * The type is @code{jit_type_short}.
 *
 * @vindex JIT_TYPE_USHORT
 * @item JIT_TYPE_USHORT
 * The type is @code{jit_type_ushort}.
 *
 * @vindex JIT_TYPE_INT
 * @item JIT_TYPE_INT
 * The type is @code{jit_type_int}.
 *
 * @vindex JIT_TYPE_UINT
 * @item JIT_TYPE_UINT
 * The type is @code{jit_type_uint}.
 *
 * @vindex JIT_TYPE_NINT
 * @item JIT_TYPE_NINT
 * The type is @code{jit_type_nint}.
 *
 * @vindex JIT_TYPE_NUINT
 * @item JIT_TYPE_NUINT
 * The type is @code{jit_type_nuint}.
 *
 * @vindex JIT_TYPE_LONG
 * @item JIT_TYPE_LONG
 * The type is @code{jit_type_long}.
 *
 * @vindex JIT_TYPE_ULONG
 * @item JIT_TYPE_ULONG
 * The type is @code{jit_type_ulong}.
 *
 * @vindex JIT_TYPE_FLOAT32
 * @item JIT_TYPE_FLOAT32
 * The type is @code{jit_type_float32}.
 *
 * @vindex JIT_TYPE_FLOAT64
 * @item JIT_TYPE_FLOAT64
 * The type is @code{jit_type_float64}.
 *
 * @vindex JIT_TYPE_NFLOAT
 * @item JIT_TYPE_NFLOAT
 * The type is @code{jit_type_nfloat}.
 *
 * @vindex JIT_TYPE_STRUCT
 * @item JIT_TYPE_STRUCT
 * The type is the result of calling @code{jit_type_create_struct}.
 *
 * @vindex JIT_TYPE_UNION
 * @item JIT_TYPE_UNION
 * The type is the result of calling @code{jit_type_create_union}.
 *
 * @vindex JIT_TYPE_SIGNATURE
 * @item JIT_TYPE_SIGNATURE
 * The type is the result of calling @code{jit_type_create_signature}.
 *
 * @vindex JIT_TYPE_PTR
 * @item JIT_TYPE_PTR
 * The type is the result of calling @code{jit_type_create_pointer}.
 * @end table
 *
 * @vindex JIT_TYPE_FIRST_TAGGED
 * If this function returns @code{JIT_TYPE_FIRST_TAGGED} or higher,
 * then the type is tagged and its tag kind is the return value minus
 * @code{JIT_TYPE_FIRST_TAGGED}.  That is, the following two expressions
 * will be identical if @var{type} is tagged:
 *
 * @example
 * jit_type_get_tagged_kind(type)
 * jit_type_get_kind(type) - JIT_TYPE_FIRST_TAGGED
 * @end example
 * @end deftypefun
@*/
int jit_type_get_kind(jit_type_t type)
{
	if(!type)
	{
		return JIT_TYPE_INVALID;
	}
	else
	{
		return type->kind;
	}
}

/*@
 * @deftypefun jit_nuint jit_type_get_size (jit_type_t @var{type})
 * Get the size of a type in bytes.
 * @end deftypefun
@*/
jit_nuint jit_type_get_size(jit_type_t type)
{
	if(!type)
	{
		return 0;
	}
	if(type->kind == JIT_TYPE_SIGNATURE)
	{
		/* The "size" field is used for argument size, not type size,
		   so we ignore it and return the real size here */
		return sizeof(void *);
	}
	else if(type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		return jit_type_get_size(type->sub_type);
	}
	if((type->layout_flags & JIT_LAYOUT_NEEDED) != 0)
	{
		perform_layout(type);
	}
	return type->size;
}

/*@
 * @deftypefun jit_nuint jit_type_get_alignment (jit_type_t @var{type})
 * Get the alignment of a type.  An alignment value of 2 indicates
 * that the type should be aligned on a two-byte boundary, for example.
 * @end deftypefun
@*/
jit_nuint jit_type_get_alignment(jit_type_t type)
{
	if(!type)
	{
		return 0;
	}
	if(type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		return jit_type_get_alignment(type->sub_type);
	}
	if((type->layout_flags & JIT_LAYOUT_NEEDED) != 0)
	{
		perform_layout(type);
	}
	return type->alignment;
}

/*@
 * @deftypefun jit_nuint jit_type_best_alignment (void)
 * Get the best alignment value for this platform.
 * @end deftypefun
@*/
jit_nuint
jit_type_best_alignment(void)
{
	return JIT_BEST_ALIGNMENT;
}

/*@
 * @deftypefun {unsigned int} jit_type_num_fields (jit_type_t @var{type})
 * Get the number of fields in a structure or union type.
 * @end deftypefun
@*/
unsigned int jit_type_num_fields(jit_type_t type)
{
	if(!type ||
	   (type->kind != JIT_TYPE_STRUCT && type->kind != JIT_TYPE_UNION))
	{
		return 0;
	}
	else
	{
		return type->num_components;
	}
}

/*@
 * @deftypefun jit_type_t jit_type_get_field (jit_type_t @var{type}, unsigned int @var{field_index})
 * Get the type of a specific field within a structure or union.
 * Returns NULL if not a structure or union, or the index is out of range.
 * @end deftypefun
@*/
jit_type_t jit_type_get_field(jit_type_t type, unsigned int field_index)
{
	if(!type || field_index >= type->num_components)
	{
		return 0;
	}
	if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION)
	{
		return type->components[field_index].type;
	}
	return 0;
}

/*@
 * @deftypefun jit_nuint jit_type_get_offset (jit_type_t @var{type}, unsigned int @var{field_index})
 * Get the offset of a specific field within a structure.
 * Returns zero if not a structure, or the index is out of range,
 * so this is safe to use on non-structure types.
 * @end deftypefun
@*/
jit_nuint jit_type_get_offset(jit_type_t type, unsigned int field_index)
{
	if(!type || field_index >= type->num_components)
	{
		return 0;
	}
	if(type->kind != JIT_TYPE_STRUCT && type->kind != JIT_TYPE_UNION)
	{
		return 0;
	}
	if((type->layout_flags & JIT_LAYOUT_NEEDED) != 0)
	{
		perform_layout(type);
	}
	return type->components[field_index].offset & ~JIT_OFFSET_IS_INTERNAL;
}

/*@
 * @deftypefun {const char *} jit_type_get_name (jit_type_t @var{type}, unsigned int @var{index})
 * Get the name of a structure, union, or signature field/parameter.
 * Returns NULL if not a structure, union, or signature, the index
 * is out of range, or there is no name associated with the component.
 * @end deftypefun
@*/
const char *jit_type_get_name(jit_type_t type, unsigned int index)
{
	if(!type || index >= type->num_components)
	{
		return 0;
	}
	else
	{
		return type->components[index].name;
	}
}

/*@
 * @deftypefun {unsigned int} jit_type_find_name (jit_type_t @var{type}, const char *@var{name})
 * Find the field/parameter index for a particular name.  Returns
 * @code{JIT_INVALID_NAME} if the name was not present.
 * @end deftypefun
@*/
unsigned int jit_type_find_name(jit_type_t type, const char *name)
{
	unsigned int index;
	if(!type || !name)
	{
		return JIT_INVALID_NAME;
	}
	if(type->kind == JIT_TYPE_STRUCT || type->kind == JIT_TYPE_UNION ||
	   type->kind == JIT_TYPE_SIGNATURE)
	{
		for(index = 0; index < type->num_components; ++index)
		{
			if(type->components[index].name &&
			   !jit_strcmp(type->components[index].name, name))
			{
				return index;
			}
		}
	}
	return JIT_INVALID_NAME;
}

/*@
 * @deftypefun {unsigned int} jit_type_num_params (jit_type_t @var{type})
 * Get the number of parameters in a signature type.
 * @end deftypefun
@*/
unsigned int jit_type_num_params(jit_type_t type)
{
	if(!type || type->kind != JIT_TYPE_SIGNATURE)
	{
		return 0;
	}
	else
	{
		return type->num_components;
	}
}

/*@
 * @deftypefun jit_type_t jit_type_get_return (jit_type_t @var{type})
 * Get the return type from a signature type.  Returns NULL if
 * not a signature type.
 * @end deftypefun
@*/
jit_type_t jit_type_get_return(jit_type_t type)
{
	if(type)
	{
		if(type->kind == JIT_TYPE_SIGNATURE)
		{
			return type->sub_type;
		}
	}
	return 0;
}

/*@
 * @deftypefun jit_type_t jit_type_get_param (jit_type_t @var{type}, unsigned int @var{param_index})
 * Get a specific parameter from a signature type.  Returns NULL
 * if not a signature type or the index is out of range.
 * @end deftypefun
@*/
jit_type_t jit_type_get_param(jit_type_t type, unsigned int param_index)
{
	if(!type || param_index >= type->num_components)
	{
		return 0;
	}
	if(type->kind == JIT_TYPE_SIGNATURE)
	{
		return type->components[param_index].type;
	}
	return 0;
}

/*@
 * @deftypefun jit_abi_t jit_type_get_abi (jit_type_t @var{type})
 * Get the ABI code from a signature type.  Returns @code{jit_abi_cdecl}
 * if not a signature type.
 * @end deftypefun
@*/
jit_abi_t jit_type_get_abi(jit_type_t type)
{
	if(type)
	{
		return (jit_abi_t)(type->abi);
	}
	else
	{
		return jit_abi_cdecl;
	}
}

/*@
 * @deftypefun jit_type_t jit_type_get_ref (jit_type_t @var{type})
 * Get the type that is referred to by a pointer type.  Returns NULL
 * if not a pointer type.
 * @end deftypefun
@*/
jit_type_t jit_type_get_ref(jit_type_t type)
{
	if(type)
	{
		if(type->kind == JIT_TYPE_PTR)
		{
			return type->sub_type;
		}
	}
	return 0;
}

/*@
 * @deftypefun jit_type_t jit_type_get_tagged_type (jit_type_t @var{type})
 * Get the type that underlies a tagged type.  Returns NULL
 * if not a tagged type.
 * @end deftypefun
@*/
jit_type_t jit_type_get_tagged_type(jit_type_t type)
{
	if(type && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		return type->sub_type;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun void jit_type_set_tagged_type (jit_type_t @var{type}, jit_type_t @var{underlying}, int @var{incref})
 * Set the type that underlies a tagged type.  Ignored if @var{type}
 * is not a tagged type.  If @var{type} already has an underlying
 * type, then the original is freed.  The reference count on @var{underlying}
 * is incremented if @var{incref} is non-zero.
 *
 * This function is typically used to flesh out the body of a
 * forward-declared type.  The tag is used as a placeholder
 * until the definition can be located.
 * @end deftypefun
@*/
void jit_type_set_tagged_type(jit_type_t type, jit_type_t underlying,
                              int incref)
{
	if(type && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		if(type->sub_type != underlying)
		{
			jit_type_free(type->sub_type);
			if(incref)
			{
				type->sub_type = jit_type_copy(underlying);
			}
			else
			{
				type->sub_type = underlying;
			}
		}
	}
}

/*@
 * @deftypefun int jit_type_get_tagged_kind (jit_type_t @var{type})
 * Get the kind of tag that is applied to a tagged type.  Returns -1
 * if not a tagged type.
 * @end deftypefun
@*/
int jit_type_get_tagged_kind(jit_type_t type)
{
	if(type && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		return type->kind - JIT_TYPE_FIRST_TAGGED;
	}
	else
	{
		return -1;
	}
}

/*@
 * @deftypefun {void *} jit_type_get_tagged_data (jit_type_t @var{type})
 * Get the user data is associated with a tagged type.  Returns NULL
 * if not a tagged type.
 * @end deftypefun
@*/
void *jit_type_get_tagged_data(jit_type_t type)
{
	if(type && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		return ((struct jit_tagged_type *)type)->data;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun void jit_type_set_tagged_data (jit_type_t @var{type}, void *@var{data}, jit_meta_free_func @var{free_func})
 * Set the user data is associated with a tagged type.  The original data,
 * if any, is freed.
 * @end deftypefun
@*/
void jit_type_set_tagged_data(jit_type_t type, void *data,
                              jit_meta_free_func free_func)
{
	if(type && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		struct jit_tagged_type *tagged = (struct jit_tagged_type *)type;
		if(tagged->data != data)
		{
			if(tagged->free_func)
			{
				(*(tagged->free_func))(tagged->data);
			}
			tagged->data = data;
			tagged->free_func = free_func;
		}
	}
}

/*@
 * @deftypefun int jit_type_is_primitive (jit_type_t @var{type})
 * Determine if a type is primitive.
 * @end deftypefun
@*/
int jit_type_is_primitive(jit_type_t type)
{
	if(type)
	{
		return (type->kind <= JIT_TYPE_MAX_PRIMITIVE);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_type_is_struct (jit_type_t @var{type})
 * Determine if a type is a structure.
 * @end deftypefun
@*/
int jit_type_is_struct(jit_type_t type)
{
	if(type)
	{
		return (type->kind == JIT_TYPE_STRUCT);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_type_is_union (jit_type_t @var{type})
 * Determine if a type is a union.
 * @end deftypefun
@*/
int jit_type_is_union(jit_type_t type)
{
	if(type)
	{
		return (type->kind == JIT_TYPE_UNION);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_type_is_signature (jit_type_t @var{type})
 * Determine if a type is a function signature.
 * @end deftypefun
@*/
int jit_type_is_signature(jit_type_t type)
{
	if(type)
	{
		return (type->kind == JIT_TYPE_SIGNATURE);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_type_is_pointer (jit_type_t @var{type})
 * Determine if a type is a pointer.
 * @end deftypefun
@*/
int jit_type_is_pointer(jit_type_t type)
{
	if(type)
	{
		return (type->kind == JIT_TYPE_PTR);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_type_is_tagged (jit_type_t @var{type})
 * Determine if a type is a tagged type.
 * @end deftypefun
@*/
int jit_type_is_tagged(jit_type_t type)
{
	if(type)
	{
		return (type->kind >= JIT_TYPE_FIRST_TAGGED);
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_type_t jit_type_remove_tags (jit_type_t @var{type})
 * Remove tags from a type, and return the underlying type.
 * This is different from normalization, which also collapses
 * native types to their basic numeric counterparts.
 * @end deftypefun
@*/
jit_type_t
jit_type_remove_tags(jit_type_t type)
{
	while(type && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		type = type->sub_type;
	}
	return type;
}

/*@
 * @deftypefun jit_type_t jit_type_normalize (jit_type_t @var{type})
 * Normalize a type to its basic numeric form.  e.g. "jit_type_nint" is
 * turned into "jit_type_int" or "jit_type_long", depending upon
 * the underlying platform.  Pointers are normalized like "jit_type_nint".
 * If the type does not have a normalized form, it is left unchanged.
 *
 * Normalization is typically used prior to applying a binary numeric
 * instruction, to make it easier to determine the common type.
 * It will also remove tags from the specified type.
 * @end deftypefun
@*/
jit_type_t
jit_type_normalize(jit_type_t type)
{
	/* Remove any tags that are attached to the type */
	type = jit_type_remove_tags(type);
	if(!type)
	{
		return type;
	}

	/* Put a platform-dependent type to its basic form */
	if(type == jit_type_nint || type->kind == JIT_TYPE_PTR ||
	   type->kind == JIT_TYPE_SIGNATURE)
	{
#ifdef JIT_NATIVE_INT32
		return jit_type_int;
#else
		return jit_type_long;
#endif
	}
	else if(type == jit_type_nuint)
	{
#ifdef JIT_NATIVE_INT32
		return jit_type_uint;
#else
		return jit_type_ulong;
#endif
	}
	else if(type == jit_type_nfloat)
	{
		if(sizeof(jit_nfloat) == sizeof(jit_float64))
		{
			return jit_type_float64;
		}
		else if(sizeof(jit_nfloat) == sizeof(jit_float32))
		{
			return jit_type_float32;
		}
	}
	return type;
}

/*@
 * @deftypefun jit_type_t jit_type_promote_int (jit_type_t @var{type})
 * If @var{type} is @code{jit_type_sbyte} or @code{jit_type_short},
 * then return @code{jit_type_int}.  If @var{type} is
 * @code{jit_type_ubyte} or @code{jit_type_ushort}, then return
 * @code{jit_type_uint}.  Otherwise return @var{type} as-is.
 * @end deftypefun
@*/
jit_type_t jit_type_promote_int(jit_type_t type)
{
	if(type == jit_type_sbyte || type == jit_type_short)
	{
		return jit_type_int;
	}
	else if(type == jit_type_ubyte || type == jit_type_ushort)
	{
		return jit_type_uint;
	}
	else
	{
		return type;
	}
}

/*@
 * @deftypefun int jit_type_return_via_pointer (jit_type_t @var{type})
 * Determine if a type should be returned via a pointer if it appears
 * as the return type in a signature.
 * @end deftypefun
@*/
int
jit_type_return_via_pointer(jit_type_t type)
{
	extern unsigned char const _jit_apply_return_in_reg[];
	unsigned int size;

	/* Remove tags if any */
	type = jit_type_remove_tags(type);

	/* Only structure and union types require special handling */
	if(!jit_type_is_struct(type) && !jit_type_is_union(type))
	{
		return 0;
	}

	/* Determine if the structure can be returned in a register */
	size = jit_type_get_size(type);
	if(size >= 1 && size <= 64)
	{
		if((_jit_apply_return_in_reg[(size - 1) / 8] &
		    	(1 << ((size - 1) % 8))) != 0)
		{
			return 0;
		}
	}
	return 1;
}

/*@
 * @deftypefun int jit_type_has_tag (jit_type_t @var{type}, int @var{kind})
 * Determine if @var{type} has a specific kind of tag.  This will
 * resolve multiple levels of tagging.
 * @end deftypefun
@*/
int jit_type_has_tag(jit_type_t type, int kind)
{
	while(type != 0 && type->kind >= JIT_TYPE_FIRST_TAGGED)
	{
		if(type->kind == (JIT_TYPE_FIRST_TAGGED + kind))
		{
			return 1;
		}
		type = type->sub_type;
	}
	return 0;
}
