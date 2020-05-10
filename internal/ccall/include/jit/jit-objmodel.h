/*
 * jit-objmodel.h - Interfaces for pluggable object models.
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

#ifndef	_JIT_OBJMODEL_H
#define	_JIT_OBJMODEL_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Opaque types that describe object model elements.
 */
typedef struct jit_objmodel *jit_objmodel_t;
typedef struct jitom_class *jitom_class_t;
typedef struct jitom_field *jitom_field_t;
typedef struct jitom_method *jitom_method_t;

/*
 * Modifier flags that describe an item's properties.
 */
#define	JITOM_MODIFIER_ACCESS_MASK				0x0007
#define	JITOM_MODIFIER_PUBLIC					0x0000
#define	JITOM_MODIFIER_PRIVATE					0x0001
#define	JITOM_MODIFIER_PROTECTED				0x0002
#define	JITOM_MODIFIER_PACKAGE					0x0003
#define	JITOM_MODIFIER_PACKAGE_OR_PROTECTED		0x0004
#define	JITOM_MODIFIER_PACKAGE_AND_PROTECTED	0x0005
#define	JITOM_MODIFIER_OTHER1					0x0006
#define	JITOM_MODIFIER_OTHER2					0x0007
#define	JITOM_MODIFIER_STATIC					0x0008
#define	JITOM_MODIFIER_VIRTUAL					0x0010
#define	JITOM_MODIFIER_NEW_SLOT					0x0020
#define	JITOM_MODIFIER_ABSTRACT					0x0040
#define	JITOM_MODIFIER_LITERAL					0x0080
#define	JITOM_MODIFIER_CTOR						0x0100
#define	JITOM_MODIFIER_STATIC_CTOR				0x0200
#define	JITOM_MODIFIER_DTOR						0x0400
#define	JITOM_MODIFIER_INTERFACE				0x0800
#define	JITOM_MODIFIER_VALUE					0x1000
#define	JITOM_MODIFIER_FINAL					0x2000
#define	JITOM_MODIFIER_DELETE					0x4000
#define	JITOM_MODIFIER_REFERENCE_COUNTED		0x8000

/*
 * Type tags that are used to mark instances of object model classes.
 */
#define	JITOM_TYPETAG_CLASS		11000	/* Object reference */
#define	JITOM_TYPETAG_VALUE		11001	/* Inline stack value */

/*
 * Operations on object models.
 */
void jitom_destroy_model(jit_objmodel_t model) JIT_NOTHROW;
jitom_class_t jitom_get_class_by_name
	(jit_objmodel_t model, const char *name) JIT_NOTHROW;

/*
 * Operations on object model classes.
 */
char *jitom_class_get_name
	(jit_objmodel_t model, jitom_class_t klass) JIT_NOTHROW;
int jitom_class_get_modifiers
	(jit_objmodel_t model, jitom_class_t klass) JIT_NOTHROW;
jit_type_t jitom_class_get_type
	(jit_objmodel_t model, jitom_class_t klass) JIT_NOTHROW;
jit_type_t jitom_class_get_value_type
	(jit_objmodel_t model, jitom_class_t klass) JIT_NOTHROW;
jitom_class_t jitom_class_get_primary_super
	(jit_objmodel_t model, jitom_class_t klass) JIT_NOTHROW;
jitom_class_t *jitom_class_get_all_supers
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num) JIT_NOTHROW;
jitom_class_t *jitom_class_get_interfaces
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num) JIT_NOTHROW;
jitom_field_t *jitom_class_get_fields
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num) JIT_NOTHROW;
jitom_method_t *jitom_class_get_methods
	(jit_objmodel_t model, jitom_class_t klass, unsigned int *num) JIT_NOTHROW;
jit_value_t jitom_class_new
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t ctor, jit_function_t func,
	 jit_value_t *args, unsigned int num_args, int flags) JIT_NOTHROW;
jit_value_t jitom_class_new_value
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t ctor, jit_function_t func,
	 jit_value_t *args, unsigned int num_args, int flags) JIT_NOTHROW;
int jitom_class_delete
	(jit_objmodel_t model, jitom_class_t klass,
	 jit_value_t obj_value) JIT_NOTHROW;
int jitom_class_add_ref
	(jit_objmodel_t model, jitom_class_t klass,
	 jit_value_t obj_value) JIT_NOTHROW;

/*
 * Operations on object model fields.
 */
char *jitom_field_get_name
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_field_t field) JIT_NOTHROW;
jit_type_t jitom_field_get_type
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_field_t field) JIT_NOTHROW;
int jitom_field_get_modifiers
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_field_t field) JIT_NOTHROW;
jit_value_t jitom_field_load
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
	 jit_function_t func, jit_value_t obj_value) JIT_NOTHROW;
jit_value_t jitom_field_load_address
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
	 jit_function_t func, jit_value_t obj_value) JIT_NOTHROW;
int jitom_field_store
	(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
	 jit_function_t func, jit_value_t obj_value, jit_value_t value) JIT_NOTHROW;

/*
 * Operations on object model methods.
 */
char *jitom_method_get_name
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t method) JIT_NOTHROW;
jit_type_t jitom_method_get_type
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t method) JIT_NOTHROW;
int jitom_method_get_modifiers
	(jit_objmodel_t model, jitom_class_t klass,
	 jitom_method_t method) JIT_NOTHROW;
jit_value_t jitom_method_invoke
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method,
	 jit_function_t func, jit_value_t *args,
	 unsigned int num_args, int flags) JIT_NOTHROW;
jit_value_t jitom_method_invoke_virtual
	(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method,
	 jit_function_t func, jit_value_t *args,
	 unsigned int num_args, int flags) JIT_NOTHROW;

/*
 * Manipulate types that represent objects and inline values.
 */
jit_type_t jitom_type_tag_as_class
	(jit_type_t type, jit_objmodel_t model,
	 jitom_class_t klass, int incref) JIT_NOTHROW;
jit_type_t jitom_type_tag_as_value
	(jit_type_t type, jit_objmodel_t model,
	 jitom_class_t klass, int incref) JIT_NOTHROW;
int jitom_type_is_class(jit_type_t type) JIT_NOTHROW;
int jitom_type_is_value(jit_type_t type) JIT_NOTHROW;
jit_objmodel_t jitom_type_get_model(jit_type_t type) JIT_NOTHROW;
jitom_class_t jitom_type_get_class(jit_type_t type) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_OBJMODEL_H */
