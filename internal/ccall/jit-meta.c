/*
 * jit-meta.c - Functions for manipulating metadata lists.
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

/*@

@section Metadata handling
@cindex Metadata handling
@cindex jit-meta.h

Many of the structures in the @code{libjit} library can have user-supplied
metadata associated with them.  Metadata may be used to store dependency
graphs, branch prediction information, or any other information that is
useful to optimizers or code generators.

Metadata can also be used by higher level user code to store information
about the structures that is specific to the user's virtual machine or
language.

The library structures have special-purpose metadata routines associated
with them (e.g. @code{jit_function_set_meta}, @code{jit_block_get_meta}).
However, sometimes you may wish to create your own metadata lists and
attach them to your own structures.  The functions below enable you
to do this:

@*/

/*@
 * @deftypefun int jit_meta_set (jit_meta_t *@var{list}, int @var{type}, void *@var{data}, jit_meta_free_func @var{free_data}, jit_function_t @var{pool_owner})
 * Set a metadata value on a list.  If the @var{type} is already present
 * in the list, then its previous value will be freed.  The @var{free_func}
 * is called when the metadata value is freed with @code{jit_meta_free}
 * or @code{jit_meta_destroy}.  Returns zero if out of memory.
 *
 * If @var{pool_owner} is not NULL, then the metadata value will persist
 * until the specified function is finished building.  Normally you would
 * set this to NULL.
 *
 * Metadata type values of 10000 or greater are reserved for internal use.
 * They should never be used by external user code.
 * @end deftypefun
@*/
int jit_meta_set(jit_meta_t *list, int type, void *data,
				 jit_meta_free_func free_data, jit_function_t pool_owner)
{
	jit_meta_t current;

	/* See if we already have this type in the list */
	current = *list;
	while(current != 0)
	{
		if(current->type == type)
		{
			if(data == current->data)
			{
				/* The value is unchanged, so don't free the previous value */
				return 1;
			}
			if(current->free_data)
			{
				(*(current->free_data))(current->data);
			}
			current->data = data;
			current->free_data = free_data;
			return 1;
		}
		current = current->next;
	}

	/* Create a new metadata block and add it to the list */
	if(pool_owner)
	{
		if((current = jit_memory_pool_alloc
				(&(pool_owner->builder->meta_pool), struct _jit_meta)) == 0)
		{
			return 0;
		}
	}
	else
	{
		if((current = jit_new(struct _jit_meta)) == 0)
		{
			return 0;
		}
	}
	current->type = type;
	current->data = data;
	current->free_data = free_data;
	current->next = *list;
	current->pool_owner = pool_owner;
	*list = current;
	return 1;
}

/*@
 * @deftypefun {void *} jit_meta_get (jit_meta_t @var{list}, int @var{type})
 * Get the value associated with @var{type} in the specified @var{list}.
 * Returns NULL if @var{type} is not present.
 * @end deftypefun
@*/
void *jit_meta_get(jit_meta_t list, int type)
{
	while(list != 0)
	{
		if(list->type == type)
		{
			return list->data;
		}
		list = list->next;
	}
	return 0;
}

/*@
 * @deftypefun void jit_meta_free (jit_meta_t *@var{list}, int @var{type})
 * Free the metadata value in the @var{list} that has the
 * specified @var{type}.  Does nothing if the @var{type}
 * is not present.
 * @end deftypefun
@*/
void jit_meta_free(jit_meta_t *list, int type)
{
	jit_meta_t current = *list;
	jit_meta_t prev = 0;
	while(current != 0)
	{
		if(current->type == type)
		{
			if(current->free_data)
			{
				(*(current->free_data))(current->data);
				current->free_data = 0;
			}
			if(prev)
			{
				prev->next = current->next;
			}
			else
			{
				*list = current->next;
			}
			if(current->pool_owner)
			{
				jit_memory_pool_dealloc
					(&(current->pool_owner->builder->meta_pool), current);
			}
			else
			{
				jit_free(current);
			}
			return;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

/*@
 * @deftypefun void jit_meta_destroy (jit_meta_t *@var{list})
 * Destroy all of the metadata values in the specified @var{list}.
 * @end deftypefun
@*/
void jit_meta_destroy(jit_meta_t *list)
{
	jit_meta_t current = *list;
	jit_meta_t next;
	while(current != 0)
	{
		next = current->next;
		if(current->free_data)
		{
			(*(current->free_data))(current->data);
			current->free_data = 0;
		}
		if(current->pool_owner)
		{
			jit_memory_pool_dealloc
				(&(current->pool_owner->builder->meta_pool), current);
		}
		else
		{
			jit_free(current);
		}
		current = next;
	}
}

void _jit_meta_free_one(void *meta)
{
	jit_meta_t current = (jit_meta_t)meta;
	if(current->free_data)
	{
		(*(current->free_data))(current->data);
	}
}
