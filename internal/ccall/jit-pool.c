/*
 * jit-pool.c - Functions for managing memory pools.
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

void _jit_memory_pool_init(jit_memory_pool *pool, unsigned int elem_size)
{
	pool->elem_size = elem_size;
	pool->elems_per_block = 4000 / elem_size;
	pool->elems_in_last = pool->elems_per_block;
	pool->blocks = 0;
	pool->free_list = 0;
}

void _jit_memory_pool_free(jit_memory_pool *pool, jit_meta_free_func func)
{
	jit_pool_block_t block;
	while(pool->blocks != 0)
	{
		block = pool->blocks;
		pool->blocks = block->next;
		if(func)
		{
			while(pool->elems_in_last > 0)
			{
				--(pool->elems_in_last);
				(*func)(block->data + pool->elems_in_last * pool->elem_size);
			}
		}
		jit_free(block);
		pool->elems_in_last = pool->elems_per_block;
	}
	pool->free_list = 0;
}

void *_jit_memory_pool_alloc(jit_memory_pool *pool)
{
	void *data;
	if(pool->free_list)
	{
		/* Reclaim an item that was previously deallocated */
		data = pool->free_list;
		pool->free_list = *((void **)data);
		jit_memzero(data, pool->elem_size);
		return data;
	}
	if(pool->elems_in_last >= pool->elems_per_block)
	{
		data = (void *)jit_calloc(1, sizeof(struct jit_pool_block) +
								  pool->elem_size * pool->elems_per_block - 1);
		if(!data)
		{
			return 0;
		}
		((jit_pool_block_t)data)->next = pool->blocks;
		pool->blocks = (jit_pool_block_t)data;
		pool->elems_in_last = 0;
	}
	data = (void *)(pool->blocks->data +
					pool->elems_in_last * pool->elem_size);
	++(pool->elems_in_last);
	return data;
}

void _jit_memory_pool_dealloc(jit_memory_pool *pool, void *item)
{
	*((void **)item) = pool->free_list;
	pool->free_list = item;
}
