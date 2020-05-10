/*
 * jit-objmodel.c - Interfaces for pluggable object models.
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

#include <jit/jit.h>
#include <jit/jit-objmodel-private.h>

/*@

The @code{libjit} library does not implement a particular object model
of its own, so that it is generic across bytecode formats and front end
languages.  However, it does provide support for plugging object models
into the JIT process, and for transparently proxying to external libraries
that may use a foreign object model.

There may be more than one object model active in the system at any
one time.  For example, a JVM implementation might have a primary
object model for its own use, and a secondary object model for
calling methods in an imported Objective C library.

The functions in this section support pluggable object models.  There is
no requirement that you use them: you can ignore them and use the rest
of @code{libjit} directly if you wish.

To create a new object model, you would include the file
@code{<jit/jit-objmodel-private.h>} and create an instance of
the @code{struct jit_objmodel} type.  This instance should be
populated with pointers to your object model's handler routines.
You then use functions from the list below to access the
object model.

Some standard object models are distributed with @code{libjit}
for invoking methods in external C++, Objective C, GCJ, and JNI
based libraries.

@*/

/*@
 * @deftypefun void jitom_destroy_model (jit_objmodel_t @var{model})
 * Destroy an object model handler that is no longer required.
 * It is undefined what will happen to the objects and classes
 * that were being managed by the object model: they may still persist,
 * or they may now be invalid.
 * @end deftypefun
@*/
void jitom_destroy_model(jit_objmodel_t model)
{
	if(model)
	{
		(*(model->destroy_model))(model);
	}
}

/*@
 * @deftypefun jitom_class_t jitom_get_class_by_name (jit_objmodel_t @var{model}, const char *@var{name})
 * Get the class descriptor from the object model for a class
 * called @var{name}.  Returns NULL if the class was not found.
 * If the name includes namespace or nested scope qualifiers,
 * they must be separated by periods (@code{.}).
 * @end deftypefun
@*/
jitom_class_t jitom_get_class_by_name(jit_objmodel_t model, const char *name)
{
	return (*(model->get_class_by_name))(model, name);
}

/*@
 * @deftypefun {char *} jitom_class_get_name (jit_objmodel_t @var{model}, jitom_class_t @var{klass})
 * Get the name of a particular class.  The return value must be freed
 * with @code{jit_free}.
 * @end deftypefun
@*/
char *jitom_class_get_name(jit_objmodel_t model, jitom_class_t klass)
{
	return (*(model->class_get_name))(model, klass);
}

/*@
 * @deftypefun int jitom_class_get_modifiers (jit_objmodel_t @var{model}, jitom_class_t @var{klass})
 * Get the access modifiers for a particular class.  The following lists
 * all access modifiers, for classes, fields and methods:
 *
 * @table @code
 * @vindex JITOM_MODIFIER_ACCESS_MASK
 * @item JITOM_MODIFIER_ACCESS_MASK
 * Mask to strip out just the public, private, etc access flags.
 *
 * @vindex JITOM_MODIFIER_PUBLIC
 * @vindex JITOM_MODIFIER_PRIVATE
 * @vindex JITOM_MODIFIER_PROTECTED
 * @vindex JITOM_MODIFIER_PACKAGE
 * @vindex JITOM_MODIFIER_PACKAGE_OR_PROTECTED
 * @vindex JITOM_MODIFIER_PACKAGE_AND_PROTECTED
 * @vindex JITOM_MODIFIER_OTHER1
 * @vindex JITOM_MODIFIER_OTHER2
 * @item JITOM_MODIFIER_PUBLIC
 * @item JITOM_MODIFIER_PRIVATE
 * @item JITOM_MODIFIER_PROTECTED
 * @item JITOM_MODIFIER_PACKAGE
 * @item JITOM_MODIFIER_PACKAGE_OR_PROTECTED
 * @item JITOM_MODIFIER_PACKAGE_AND_PROTECTED
 * @item JITOM_MODIFIER_OTHER1
 * @item JITOM_MODIFIER_OTHER2
 * The declared access level on the class, field, or method.  The scope
 * of a "package" will typically be model-specific: Java uses namespaces
 * to define packages, whereas C# uses compile units ("assemblies").
 *
 * Object model handlers do not need to enforce the above access levels.
 * It is assumed that any caller with access to @code{libjit} has complete
 * access to the entire system, and will use its own rules for determining
 * when it is appropriate to grant access to fields and methods.
 *
 * @vindex JITOM_MODIFIER_STATIC
 * @item JITOM_MODIFIER_STATIC
 * The field or method is static, rather than instance-based.
 *
 * @vindex JITOM_MODIFIER_VIRTUAL
 * @item JITOM_MODIFIER_VIRTUAL
 * The method is instance-based and virtual.
 *
 * @vindex JITOM_MODIFIER_NEW_SLOT
 * @item JITOM_MODIFIER_NEW_SLOT
 * The method is virtual, but occupies a new slot.  Provided for languages
 * like C# that can declare virtual methods with the same name as in a
 * superclass, but within a new slot in the vtable.
 *
 * @vindex JITOM_MODIFIER_ABSTRACT
 * @item JITOM_MODIFIER_ABSTRACT
 * When used on a class, this indicates that the class contains abstract
 * methods.  When used on a method, this indicates that the method does
 * not have any associated code in its defining class.  It is not possible
 * to invoke such a method with @code{jitom_method_invoke}, only
 * @code{jitom_method_invoke_virtual}.
 *
 * @vindex JITOM_MODIFIER_LITERAL
 * @item JITOM_MODIFIER_LITERAL
 * A hint flag, used on fields, to indicate that the field has a constant
 * value associated with it and does not occupy any real space.  If the
 * @code{jitom_field_load} function is used on such a field, it will load
 * the constant value.
 *
 * @vindex JITOM_MODIFIER_CTOR
 * @item JITOM_MODIFIER_CTOR
 * A hint flag that indicates that the method is an instance constructor.
 *
 * @vindex JITOM_MODIFIER_STATIC_CTOR
 * @item JITOM_MODIFIER_STATIC_CTOR
 * A hint flag that indicates that the method is a static constructor.
 * The object model handler is normally responsible for outputting calls to
 * static constructors; the caller shouldn't need to perform any
 * special handling.
 *
 * @vindex JITOM_MODIFIER_DTOR
 * @item JITOM_MODIFIER_DTOR
 * A hint flag that indicates that the method is an instance destructor.
 * The class should also have the @code{JITOM_MODIFIER_DELETE} flag.
 * Note: do not use this for object models that support automatic
 * garbage collection and finalization.
 *
 * @vindex JITOM_MODIFIER_INTERFACE
 * @item JITOM_MODIFIER_INTERFACE
 * The class is an interface.
 *
 * @vindex JITOM_MODIFIER_VALUE
 * @item JITOM_MODIFIER_VALUE
 * Instances of this class can be stored inline on the stack.
 *
 * @vindex JITOM_MODIFIER_FINAL
 * @item JITOM_MODIFIER_FINAL
 * When used on a class, this indicates that it cannot be subclassed.
 * When used on a virtual method, this indicates that it cannot be
 * overridden in subclasses.
 *
 * @vindex JITOM_MODIFIER_DELETE
 * @item JITOM_MODIFIER_DELETE
 * When used on a class, this indicates that its objects must be
 * explicitly deleted when no longer required.
 *
 * @vindex JITOM_MODIFIER_REFERENCE_COUNTED
 * @item JITOM_MODIFIER_REFERENCE_COUNTED
 * When used on a class, this indicates that its objects are
 * reference-counted.  Calling @code{jitom_class_delete} will
 * reduce the reference count and delete the object for real
 * when the count reaches zero.
 * @end table
 * @end deftypefun
@*/
int jitom_class_get_modifiers(jit_objmodel_t model, jitom_class_t klass)
{
	return (*(model->class_get_modifiers))(model, klass);
}

/*@
 * @deftypefun jit_type_t jitom_class_get_type (jit_objmodel_t @var{model}, jitom_class_t @var{klass})
 * Get the JIT type descriptor that represents pointer-based
 * object references to a class.  The object model handler should
 * use @code{jitom_type_tag_as_class} to tag the return value.
 * The caller should use @code{jit_type_free} to free the returned
 * value when it is no longer required.
 * @end deftypefun
@*/
jit_type_t jitom_class_get_type(jit_objmodel_t model, jitom_class_t klass)
{
	return (*(model->class_get_type))(model, klass);
}

/*@
 * @deftypefun jit_type_t jitom_class_get_value_type (jit_objmodel_t @var{model}, jitom_class_t @var{klass})
 * Get the JIT type descriptor that represents inline stack instances
 * of the class.  The object model handler should use
 * @code{jitom_type_tag_as_value} to tag the return value.
 * The caller should use @code{jit_type_free} to free the returned
 * value when it is no longer required.
 * @end deftypefun
@*/
jit_type_t jitom_class_get_value_type
	(jit_objmodel_t model, jitom_class_t klass)
{
	return (*(model->class_get_value_type))(model, klass);
}

/*@
 * @deftypefun jitom_class_t jitom_class_get_primary_super (jit_objmodel_t @var{model}, jitom_class_t @var{klass})
 * Get the primary superclass for @var{klass}.  If the object model supports
 * true multiple inheritance, then this should return the first superclass.
 * Note: for the purposes of this function, interfaces are not considered
 * superclasses.  Use @code{jitom_class_get_interfaces} instead.
 * @end deftypefun
@*/
jitom_class_t jitom_class_get_primary_super
	(jit_objmodel_t model, jitom_class_t klass)
{
	return (*(model->class_get_primary_super))(model, klass);
}

/*@
 * @deftypefun {jitom_class_t *} jitom_class_get_all_supers (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, unsigned int *@var{num})
 * Return an array of all superclasses for @var{klass}, with the number of
 * elements returned in @var{num}.  Returns NULL if out of memory.
 * Use @code{jit_free} to free the return array.
 * @end deftypefun
@*/
jitom_class_t *jitom_class_get_all_supers
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num)
{
	return (*(model->class_get_all_supers))(model, klass, num);
}

/*@
 * @deftypefun {jitom_class_t *} jitom_class_get_interfaces (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, unsigned int *@var{num})
 * Return an array of all interfaces for @var{klass}, with the number
 * of elements returned in @var{num}.  The list does not include interfaces
 * that are declared on superclasses.  Returns NULL if out of memory.
 * Use @code{jit_free} to free the return array.
 * @end deftypefun
@*/
jitom_class_t *jitom_class_get_interfaces
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num)
{
	return (*(model->class_get_interfaces))(model, klass, num);
}

/*@
 * @deftypefun {jitom_field_t *} jitom_class_get_fields (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, unsigned int *@var{num})
 * Return an array of all fields for @var{klass}, with the number
 * of elements returned in @var{num}.  The list does not include fields
 * that are declared on superclasses.  Returns NULL if out of memory.
 * Use @code{jit_free} to free the return array.
 * @end deftypefun
@*/
jitom_field_t *jitom_class_get_fields
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num)
{
	return (*(model->class_get_fields))(model, klass, num);
}

/*@
 * @deftypefun {jitom_method_t *} jitom_class_get_methods (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, unsigned int *@var{num})
 * Return an array of all methods for @var{klass}, with the number
 * of elements returned in @var{num}.  The list does not include methods
 * that are declared on superclasses.  Returns NULL if out of memory.
 * Use @code{jit_free} to free the return array.
 * @end deftypefun
@*/
jitom_method_t *jitom_class_get_methods
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num)
{
	return (*(model->class_get_methods))(model, klass, num);
}

/*@
 * @deftypefun jit_value_t jitom_class_new (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{ctor}, jit_function_t @var{func}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Add instructions to @var{func} to create a new instance of the
 * specified class.  If @var{ctor} is not NULL, then it indicates a
 * constructor that should be invoked with the arguments in @var{args}.
 * If @var{ctor} is NULL, then memory should be allocated, but no
 * constructor should be invoked.  Returns a JIT value representing
 * the newly allocated object.  The type of the value will be the same
 * as the the result from @code{jitom_class_get_type}.
 * @end deftypefun
@*/
jit_value_t jitom_class_new
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t ctor, jit_function_t func,
	 jit_value_t *args, unsigned int num_args, int flags)
{
	return (*(model->class_new))
		(model, klass, ctor, func, args, num_args, flags);
}

/*@
 * @deftypefun jit_value_t jitom_class_new_value (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{ctor}, jit_function_t @var{func}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Add instructions to @var{func} to create a new instance of the
 * specified class, inline on the stack.  If @var{ctor} is not NULL, then
 * it indicates a constructor that should be invoked with the arguments
 * in @var{args}.  If @var{ctor} is NULL, then stack space should be
 * allocated, but no constructor should be invoked.  Returns a JIT
 * value representing the newly allocated stack space.  The type of the
 * value will be the same as the the result from
 * @code{jitom_class_get_value_type}.
 * @end deftypefun
@*/
jit_value_t jitom_class_new_value
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t ctor, jit_function_t func,
	 jit_value_t *args, unsigned int num_args, int flags)
{
	return (*(model->class_new_value))
		(model, klass, ctor, func, args, num_args, flags);
}

/*@
 * @deftypefun int jitom_class_delete (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jit_value_t @var{obj_value})
 * Delete an instance of a particular class, calling the destructor if
 * necessary.  The @var{obj_value} may be an inline stack value,
 * in which case the destructor is called, but the memory is not freed.
 * Ignored if the class does not have the @code{JITOM_MODIFIER_DELETE}
 * modifier.
 *
 * Note: no attempt is made to destroy inline stack values automatically when
 * they go out of scope.  It is up to the caller to output instructions
 * in the appropriate places to do this.
 * @end deftypefun
@*/
int jitom_class_delete
	(jit_objmodel_t model, jitom_class_t klass, jit_value_t obj_value)
{
	return (*(model->class_delete))(model, klass, obj_value);
}

/*@
 * @deftypefun int jitom_class_add_ref (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jit_value_t @var{obj_value})
 * Add a reference to a reference-counted object.  Ignored if the
 * class does not have the @code{JITOM_MODIFIER_REFERENCE_COUNTED} modifier,
 * or the value is stored inline on the stack.
 *
 * Other functions, such as @code{jitom_field_store}, may also change the
 * reference count, but such behavior is object model specific.
 * @end deftypefun
@*/
int jitom_class_add_ref
	(jit_objmodel_t model, jitom_class_t klass, jit_value_t obj_value)
{
	return (*(model->class_add_ref))(model, klass, obj_value);
}

/*@
 * @deftypefun {char *} jitom_field_get_name (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_field_t @var{field})
 * Get the name of a particular object model field.  The return value must
 * be freed with @code{jit_free}.
 * @end deftypefun
@*/
char *jitom_field_get_name
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field)
{
	return (*(model->field_get_name))(model, klass, field);
}

/*@
 * @deftypefun jit_type_t jitom_field_get_type (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_field_t @var{field})
 * Get the type of a particular object model field.
 * @end deftypefun
@*/
jit_type_t jitom_field_get_type
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field)
{
	return (*(model->field_get_type))(model, klass, field);
}

/*@
 * @deftypefun int jitom_field_get_modifiers (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_field_t @var{field})
 * Get the access modifiers that are associated with a particular
 * object model field.
 * @end deftypefun
@*/
int jitom_field_get_modifiers
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field)
{
	return (*(model->field_get_modifiers))(model, klass, field);
}

/*@
 * @deftypefun jit_value_t jitom_field_load (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_field_t @var{field}, jit_function_t @var{func}, jit_value_t @var{obj_value})
 * Create instructions within @var{func} to effect a load from a
 * field within the object @var{obj_value}.  If @var{obj_value} is
 * NULL, then it indicates a static field reference.
 *
 * If the field has the @code{JITOM_MODIFIER_LITERAL} modifier, then this
 * function will load the constant value associated with the field.
 * Literal fields cannot be stored to.
 * @end deftypefun
@*/
jit_value_t jitom_field_load
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
	 jit_function_t func, jit_value_t obj_value)
{
	return (*(model->field_load))(model, klass, field, func, obj_value);
}

/*@
 * @deftypefun jit_value_t jitom_field_load_address (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_field_t @var{field}, jit_function_t @var{func}, jit_value_t @var{obj_value})
 * Create instructions within @var{func} to get the address of a
 * field within the object @var{obj_value}.  If @var{obj_value} is
 * NULL, then it indicates that we want the address of a static field.
 * Some object models may not support this function, and will return NULL.
 * @end deftypefun
@*/
jit_value_t jitom_field_load_address
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
	 jit_function_t func, jit_value_t obj_value)
{
	return (*(model->field_load_address))
		(model, klass, field, func, obj_value);
}

/*@
 * @deftypefun int jitom_field_store (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_field_t @var{field}, jit_function_t @var{func}, jit_value_t @var{obj_value}, jit_value_t @var{value})
 * Create instructions within @var{func} to store @var{value} into a
 * field within the object @var{obj_value}.  If @var{obj_value} is
 * NULL, then it indicates a static field store.
 * @end deftypefun
@*/
int jitom_field_store
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
	 jit_function_t func, jit_value_t obj_value, jit_value_t value)
{
	return (*(model->field_store))
		(model, klass, field, func, obj_value, value);
}

/*@
 * @deftypefun {char *} jitom_method_get_name (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{method})
 * Get the name of an object model method.  The return value must
 * be freed with @code{jit_free}.
 * @end deftypefun
@*/
char *jitom_method_get_name
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method)
{
	return (*(model->method_get_name))(model, klass, method);
}

/*@
 * @deftypefun jit_type_t jitom_method_get_type (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{method})
 * Get the signature type of an object model method.  If the method
 * is instance-based, then the first argument will be an object reference
 * type indicating the @code{this} pointer.
 * @end deftypefun
@*/
jit_type_t jitom_method_get_type
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method)
{
	return (*(model->method_get_type))(model, klass, method);
}

/*@
 * @deftypefun int jitom_method_get_modifiers (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{method})
 * Get the access modifiers for an object model method.
 * @end deftypefun
@*/
int jitom_method_get_modifiers
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method)
{
	return (*(model->method_get_modifiers))(model, klass, method);
}

/*@
 * @deftypefun jit_value_t jitom_method_invoke (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{method}, jit_function_t @var{func}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Create instructions within @var{func} to invoke a static or
 * instance method.  If an instance method is virtual, then this will
 * ignore the virtual property to invoke the designated method directly.
 * The first element in @var{args} should be the @code{this} pointer
 * for instance methods.
 * @end deftypefun
@*/
jit_value_t jitom_method_invoke
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method,
	 jit_function_t func, jit_value_t *args, unsigned int num_args, int flags)
{
	return (*(model->method_invoke))
		(model, klass, method, func, args, num_args, flags);
}

/*@
 * @deftypefun jit_value_t jitom_method_invoke_virtual (jit_objmodel_t @var{model}, jitom_class_t @var{klass}, jitom_method_t @var{method}, jit_function_t @var{func}, jit_value_t *@var{args}, unsigned int @var{num_args}, int @var{flags})
 * Create instructions within @var{func} to invoke a virtual or interface
 * method.  The first element in @var{args} should be the @code{this}
 * pointer for the call.
 * @end deftypefun
@*/
jit_value_t jitom_method_invoke_virtual
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method,
	 jit_function_t func, jit_value_t *args, unsigned int num_args, int flags)
{
	return (*(model->method_invoke_virtual))
		(model, klass, method, func, args, num_args, flags);
}

/*
 * Information that is stored for class-tagged types.
 */
typedef struct
{
	jit_objmodel_t  model;
	jitom_class_t	klass;

} jitom_tag_info;

/*@
 * @deftypefun jit_type_t jitom_type_tag_as_class (jit_type_t @var{type}, jit_objmodel_t @var{model}, jitom_class_t @var{klass}, int @var{incref})
 * Tag a JIT type as an object reference belonging to a specific class.
 * Returns NULL if there is insufficient memory to tag the type.
 * @end deftypefun
@*/
jit_type_t jitom_type_tag_as_class
	(jit_type_t type, jit_objmodel_t model, jitom_class_t klass, int incref)
{
	jitom_tag_info *info;
	if((info = jit_new(jitom_tag_info)) == 0)
	{
		return 0;
	}
	info->model = model;
	info->klass = klass;
	type = jit_type_create_tagged
		(type, JITOM_TYPETAG_CLASS, info, jit_free, incref);
	if(!type)
	{
		jit_free(info);
	}
	return type;
}

/*@
 * @deftypefun jit_type_t jitom_type_tag_as_value (jit_type_t @var{type}, jit_objmodel_t @var{model}, jitom_class_t @var{klass}, int @var{incref})
 * Tag a JIT type as an inline static value belonging to a specific class.
 * Returns NULL if there is insufficient memory to tag the type.
 * @end deftypefun
@*/
jit_type_t jitom_type_tag_as_value
	(jit_type_t type, jit_objmodel_t model, jitom_class_t klass, int incref)
{
	jitom_tag_info *info;
	if((info = jit_new(jitom_tag_info)) == 0)
	{
		return 0;
	}
	info->model = model;
	info->klass = klass;
	type = jit_type_create_tagged
		(type, JITOM_TYPETAG_VALUE, info, jit_free, incref);
	if(!type)
	{
		jit_free(info);
	}
	return type;
}

/*@
 * @deftypefun int jitom_type_is_class (jit_type_t @var{type})
 * Determine if a type is tagged as an object reference.
 * @end deftypefun
@*/
int jitom_type_is_class(jit_type_t type)
{
	return (jit_type_get_tagged_kind(type) == JITOM_TYPETAG_CLASS);
}

/*@
 * @deftypefun int jitom_type_is_value (jit_type_t @var{type})
 * Determine if a type is tagged as an inline static value.
 * @end deftypefun
@*/
int jitom_type_is_value(jit_type_t type)
{
	return (jit_type_get_tagged_kind(type) == JITOM_TYPETAG_VALUE);
}

/*@
 * @deftypefun jit_objmodel_t jitom_type_get_model (jit_type_t @var{type})
 * Get the object model associated with a type that was tagged with
 * @code{jitom_type_tag_as_class} or @code{jitom_type_tag_as_value}.
 * @end deftypefun
@*/
jit_objmodel_t jitom_type_get_model(jit_type_t type)
{
	if(jit_type_get_tagged_kind(type) == JITOM_TYPETAG_CLASS ||
	   jit_type_get_tagged_kind(type) == JITOM_TYPETAG_VALUE)
	{
		return ((jitom_tag_info *)jit_type_get_tagged_data(type))->model;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jitom_class_t jitom_type_get_class (jit_type_t @var{type})
 * Get the class associated with a type that was tagged with
 * @code{jitom_type_tag_as_class} or @code{jitom_type_tag_as_value}.
 * @end deftypefun
@*/
jitom_class_t jitom_type_get_class(jit_type_t type)
{
	if(jit_type_get_tagged_kind(type) == JITOM_TYPETAG_CLASS ||
	   jit_type_get_tagged_kind(type) == JITOM_TYPETAG_VALUE)
	{
		return ((jitom_tag_info *)jit_type_get_tagged_data(type))->klass;
	}
	else
	{
		return 0;
	}
}
