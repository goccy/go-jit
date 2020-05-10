/*
 * jit-objmodel-private.h - Private object model definitions.
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

#ifndef	_JIT_OBJMODEL_PRIVATE_H
#define	_JIT_OBJMODEL_PRIVATE_H

#include <jit/jit-objmodel.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Internal structure of an object model handler.
 */
struct jit_objmodel
{
	/*
	 * Size of this structure, for versioning.
	 */
	unsigned int size;

	/*
	 * Reserved fields that can be used by the handler to store its state.
	 */
	void *reserved0;
	void *reserved1;
	void *reserved2;
	void *reserved3;

	/*
	 * Operations on object models.
	 */
	void (*destroy_model)(jit_objmodel_t model);
	jitom_class_t (*get_class_by_name)(jit_objmodel_t model, const char *name);

	/*
	 * Operations on object model classes.
 	 */
	char *(*class_get_name)(jit_objmodel_t model, jitom_class_t klass);
	int (*class_get_modifiers)(jit_objmodel_t model, jitom_class_t klass);
	jit_type_t (*class_get_type)(jit_objmodel_t model, jitom_class_t klass);
	jit_type_t (*class_get_value_type)
		(jit_objmodel_t model, jitom_class_t klass);
	jitom_class_t (*class_get_primary_super)
		(jit_objmodel_t model, jitom_class_t klass);
	jitom_class_t *(*class_get_all_supers)
		(jit_objmodel_t model, jitom_class_t klass, unsigned int *num);
	jitom_class_t *(*class_get_interfaces)
		(jit_objmodel_t model, jitom_class_t klass, unsigned int *num);
	jitom_field_t *(*class_get_fields)
		(jit_objmodel_t model, jitom_class_t klass, unsigned int *num);
	jitom_method_t *(*class_get_methods)
		(jit_objmodel_t model, jitom_class_t klass, unsigned int *num);
	jit_value_t (*class_new)
		(jit_objmodel_t model, jitom_class_t klass,
		 jitom_method_t ctor, jit_function_t func,
		 jit_value_t *args, unsigned int num_args, int flags);
	jit_value_t (*class_new_value)
		(jit_objmodel_t model, jitom_class_t klass,
		 jitom_method_t ctor, jit_function_t func,
		 jit_value_t *args, unsigned int num_args, int flags);
	int (*class_delete)
		(jit_objmodel_t model, jitom_class_t klass, jit_value_t obj_value);
	int (*class_add_ref)
		(jit_objmodel_t model, jitom_class_t klass, jit_value_t obj_value);

	/*
	 * Operations on object model fields.
	 */
	char *(*field_get_name)
		(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field);
	jit_type_t (*field_get_type)
		(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field);
	int (*field_get_modifiers)
		(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field);
	jit_value_t (*field_load)
		(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
		 jit_function_t func, jit_value_t obj_value);
	jit_value_t (*field_load_address)
		(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
		 jit_function_t func, jit_value_t obj_value);
	int (*field_store)
		(jit_objmodel_t model, jitom_class_t klass, jitom_field_t field,
		 jit_function_t func, jit_value_t obj_value, jit_value_t value);

	/*
	 * Operations on object model methods.
	 */
	char *(*method_get_name)
		(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method);
	jit_type_t (*method_get_type)
		(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method);
	int (*method_get_modifiers)
		(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method);
	jit_value_t (*method_invoke)
		(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method,
		 jit_function_t func, jit_value_t *args,
		 unsigned int num_args, int flags);
	jit_value_t (*method_invoke_virtual)
		(jit_objmodel_t model, jitom_class_t klass, jitom_method_t method,
		 jit_function_t func, jit_value_t *args,
		 unsigned int num_args, int flags);

};

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_OBJMODEL_PRIVATE_H */
