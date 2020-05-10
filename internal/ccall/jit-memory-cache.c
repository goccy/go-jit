/*
 * jit-cache.c - Translated function cache implementation.
 *
 * Copyright (C) 2002, 2003, 2008  Southern Storm Software, Pty Ltd.
 * Copyright (C) 2012  Aleksey Demakov
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

/*
See the bottom of this file for documentation on the cache system.
*/

#include "jit-internal.h"
#include "jit-apply-func.h"

#include <stddef.h> /* for offsetof */

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Tune the default size of a cache page.  Memory is allocated from
 * the system in chunks of this size.
 */
#ifndef	JIT_CACHE_PAGE_SIZE
#define	JIT_CACHE_PAGE_SIZE		(64 * 1024)
#endif

/*
 * Tune the maximum size of a cache page.  The size of a page might be
 * up to (JIT_CACHE_PAGE_SIZE * JIT_CACHE_MAX_PAGE_FACTOR).  This will
 * also determine the maximum method size that can be translated.
 */
#ifndef JIT_CACHE_MAX_PAGE_FACTOR
#define JIT_CACHE_MAX_PAGE_FACTOR	1024
#endif

/*
 * Method information block, organised as a red-black tree node.
 * There may be more than one such block associated with a method
 * if the method contains exception regions.
 */
typedef struct jit_cache_node *jit_cache_node_t;
struct jit_cache_node
{
	jit_cache_node_t	left;		/* Left sub-tree and red/black bit */
	jit_cache_node_t	right;		/* Right sub-tree */
	unsigned char		*start;		/* Start of the cache region */
	unsigned char		*end;		/* End of the cache region */
	jit_function_t		func;		/* Function info block slot */
};

/*
 * Structure of the page list entry.
 */
struct jit_cache_page
{
	void			*page;		/* Page memory */
	long			factor;		/* Page size factor */
};

/*
 * Structure of the method cache.
 */
typedef struct jit_cache *jit_cache_t;
struct jit_cache
{
	struct jit_cache_page	*pages;		/* List of pages currently in the cache */
	unsigned long		numPages;	/* Number of pages currently in the cache */
	unsigned long		maxNumPages;	/* Maximum number of pages that could be in the list */
	unsigned long		pageSize;	/* Default size of a page for allocation */
	unsigned int		maxPageFactor;	/* Maximum page size factor */
	long			pagesLeft;	/* Number of pages left to allocate */
	unsigned char		*free_start;	/* Current start of the free region */
	unsigned char		*free_end;	/* Current end of the free region */
	unsigned char		*prev_start;	/* Previous start of the free region */
	unsigned char		*prev_end;	/* Previous end of the free region */
	jit_cache_node_t	node;		/* Information for the current function */
	struct jit_cache_node	head;		/* Head of the lookup tree */
	struct jit_cache_node	nil;		/* Nil pointer for the lookup tree */
};

/*
 * Get or set the sub-trees of a node.
 */
#define	GetLeft(node)	\
	((jit_cache_node_t)(((jit_nuint)(node)->left) & ~((jit_nuint)1)))
#define	GetRight(node)	\
	((node)->right)
#define	SetLeft(node,value)	\
	((node)->left = (jit_cache_node_t)(((jit_nuint)value) | \
						(((jit_nuint)(node)->left) & ((jit_nuint)1))))
#define	SetRight(node,value)	\
	((node)->right = (value))

/*
 * Get or set the red/black state of a node.
 */
#define	GetRed(node)	\
	((((jit_nuint)((node)->left)) & ((jit_nuint)1)) != 0)
#define	SetRed(node)	\
	((node)->left = (jit_cache_node_t)(((jit_nuint)(node)->left) | ((jit_nuint)1)))
#define	SetBlack(node)	\
	((node)->left = (jit_cache_node_t)(((jit_nuint)(node)->left) & ~((jit_nuint)1)))

void _jit_cache_destroy(jit_cache_t cache);
void * _jit_cache_alloc_data(jit_cache_t cache, unsigned long size, unsigned long align);

/*
 * Allocate a cache page and add it to the cache.
 */
static void
AllocCachePage(jit_cache_t cache, int factor)
{
	long num;
	unsigned char *ptr;
	struct jit_cache_page *list;

	/* The minimum page factor is 1 */
	if(factor <= 0)
	{
		factor = 1;
	}

	/* If too big a page is requested, then bail out */
	if(((unsigned int) factor) > cache->maxPageFactor)
	{
		goto failAlloc;
	}

	/* If the page limit is hit, then bail out */
	if(cache->pagesLeft >= 0 && cache->pagesLeft < factor)
	{
		goto failAlloc;
	}

	/* Try to allocate a physical page */
	ptr = (unsigned char *) _jit_malloc_exec((unsigned int) cache->pageSize * factor);
	if(!ptr)
	{
		goto failAlloc;
	}

	/* Add the page to the page list.  We keep this in an array
	   that is separate from the pages themselves so that we don't
	   have to "touch" the pages to free them.  Touching the pages
	   may cause them to be swapped in if they are currently out.
	   There's no point doing that if we are trying to free them */
	if(cache->numPages == cache->maxNumPages)
	{
		if(cache->numPages == 0)
		{
			num = 16;
		}
		else
		{
			num = cache->numPages * 2;
		}
		if(cache->pagesLeft > 0 && num > (cache->numPages + cache->pagesLeft - factor + 1))
		{
			num = cache->numPages + cache->pagesLeft - factor + 1;
		}

		list = (struct jit_cache_page *) jit_realloc(cache->pages,
							     sizeof(struct jit_cache_page) * num);
		if(!list)
		{
			_jit_free_exec(ptr, cache->pageSize * factor);
		failAlloc:
			cache->free_start = 0;
			cache->free_end = 0;
			return;
		}

		cache->maxNumPages = num;
		cache->pages = list;
	}
	cache->pages[cache->numPages].page = ptr;
	cache->pages[cache->numPages].factor = factor;
	++(cache->numPages);

	/* Adjust te number of pages left before we hit the limit */
	if(cache->pagesLeft > 0)
	{
		cache->pagesLeft -= factor;
	}

	/* Set up the working region within the new page */
	cache->free_start = ptr;
	cache->free_end = ptr + (int) cache->pageSize * factor;
}

/*
 * Compare a key against a node, being careful of sentinel nodes.
 */
static int
CacheCompare(jit_cache_t cache, unsigned char *key, jit_cache_node_t node)
{
	if(node == &cache->nil || node == &cache->head)
	{
		/* Every key is greater than the sentinel nodes */
		return 1;
	}
	else
	{
		/* Compare a regular node */
		if(key < node->start)
		{
			return -1;
		}
		else if(key > node->start)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

/*
 * Rotate a sub-tree around a specific node.
 */
static jit_cache_node_t
CacheRotate(jit_cache_t cache, unsigned char *key, jit_cache_node_t around)
{
	jit_cache_node_t child, grandChild;
	int setOnLeft;
	if(CacheCompare(cache, key, around) < 0)
	{
		child = GetLeft(around);
		setOnLeft = 1;
	}
	else
	{
		child = GetRight(around);
		setOnLeft = 0;
	}
	if(CacheCompare(cache, key, child) < 0)
	{
		grandChild = GetLeft(child);
		SetLeft(child, GetRight(grandChild));
		SetRight(grandChild, child);
	}
	else
	{
		grandChild = GetRight(child);
		SetRight(child, GetLeft(grandChild));
		SetLeft(grandChild, child);
	}
	if(setOnLeft)
	{
		SetLeft(around, grandChild);
	}
	else
	{
		SetRight(around, grandChild);
	}
	return grandChild;
}

/*
 * Split a red-black tree at the current position.
 */
#define	Split()								\
	do {								\
		SetRed(temp);						\
		SetBlack(GetLeft(temp));				\
		SetBlack(GetRight(temp));				\
		if(GetRed(parent))					\
		{							\
			SetRed(grandParent);				\
			if((CacheCompare(cache, key, grandParent) < 0) != \
			   (CacheCompare(cache, key, parent) < 0))	\
			{						\
				parent = CacheRotate(cache, key, grandParent); \
			}						\
			temp = CacheRotate(cache, key, greatGrandParent); \
			SetBlack(temp);					\
		}							\
	} while (0)

/*
 * Add a method region block to the red-black lookup tree
 * that is associated with a method cache.
 */
static void
AddToLookupTree(jit_cache_t cache, jit_cache_node_t method)
{
	unsigned char *key = method->start;
	jit_cache_node_t temp;
	jit_cache_node_t greatGrandParent;
	jit_cache_node_t grandParent;
	jit_cache_node_t parent;
	jit_cache_node_t nil = &(cache->nil);
	int cmp;

	/* Search for the insert position */
	temp = &(cache->head);
	greatGrandParent = temp;
	grandParent = temp;
	parent = temp;
	while(temp != nil)
	{
		/* Adjust our ancestor pointers */
		greatGrandParent = grandParent;
		grandParent = parent;
		parent = temp;

		/* Compare the key against the current node */
		cmp = CacheCompare(cache, key, temp);
		if(cmp == 0)
		{
			/* This is a duplicate, which normally shouldn't happen.
			   If it does happen, then ignore the node and bail out */
			return;
		}
		else if(cmp < 0)
		{
			temp = GetLeft(temp);
		}
		else
		{
			temp = GetRight(temp);
		}

		/* Do we need to split this node? */
		if(GetRed(GetLeft(temp)) && GetRed(GetRight(temp)))
		{
			Split();
		}
	}

	/* Insert the new node into the current position */
	method->left = (jit_cache_node_t)(((jit_nuint)nil) | ((jit_nuint)1));
	method->right = nil;
	if(CacheCompare(cache, key, parent) < 0)
	{
		SetLeft(parent, method);
	}
	else
	{
		SetRight(parent, method);
	}
	Split();
	SetBlack(cache->head.right);
}

jit_cache_t
_jit_cache_create(jit_context_t context)
{
	jit_cache_t cache;
	long limit, cache_page_size;
	int max_page_factor;
	unsigned long exec_page_size;

	limit = (long)
		jit_context_get_meta_numeric(context, JIT_OPTION_CACHE_LIMIT);
	cache_page_size = (long)
		jit_context_get_meta_numeric(context, JIT_OPTION_CACHE_PAGE_SIZE);
	max_page_factor = (int)
		jit_context_get_meta_numeric(context, JIT_OPTION_CACHE_MAX_PAGE_FACTOR);

	/* Allocate space for the cache control structure */
	if((cache = (jit_cache_t) jit_malloc(sizeof(struct jit_cache))) == 0)
	{
		return 0;
	}

	/* determine the default cache page size */
	exec_page_size = jit_vmem_page_size();
	if(cache_page_size <= 0)
	{
		cache_page_size = JIT_CACHE_PAGE_SIZE;
	}
	if(cache_page_size < exec_page_size)
	{
		cache_page_size = exec_page_size;
	}
	else
	{
		cache_page_size = (cache_page_size / exec_page_size) * exec_page_size;
	}

	/* determine the maximum page size factor */
	if(max_page_factor <= 0)
	{
		max_page_factor = JIT_CACHE_MAX_PAGE_FACTOR;
	}

	/* Initialize the rest of the cache fields */
	cache->pages = 0;
	cache->numPages = 0;
	cache->maxNumPages = 0;
	cache->pageSize = cache_page_size;
	cache->maxPageFactor = max_page_factor;
	cache->free_start = 0;
	cache->free_end = 0;
	if(limit > 0)
	{
		cache->pagesLeft = limit / cache_page_size;
		if(cache->pagesLeft < 1)
		{
			cache->pagesLeft = 1;
		}
	}
	else
	{
		cache->pagesLeft = -1;
	}
	cache->node = 0;
	cache->nil.left = &(cache->nil);
	cache->nil.right = &(cache->nil);
	cache->nil.func = 0;
	cache->head.left = 0;
	cache->head.right = &(cache->nil);
	cache->head.func = 0;

	/* Allocate the initial cache page */
	AllocCachePage(cache, 0);
	if(!cache->free_start)
	{
		_jit_cache_destroy(cache);
		return 0;
	}

	/* Ready to go */
	return cache;
}

void
_jit_cache_destroy(jit_cache_t cache)
{
	unsigned long page;

	/* Free all of the cache pages */
	for(page = 0; page < cache->numPages; ++page)
	{
		_jit_free_exec(cache->pages[page].page,
			       cache->pageSize * cache->pages[page].factor);
	}
	if(cache->pages)
	{
		jit_free(cache->pages);
	}

	/* Free the cache object itself */
	jit_free(cache);
}

int
_jit_cache_extend(jit_cache_t cache, int count)
{
	/* Compute the page size factor */
	int factor = 1 << count;

	/* Bail out if there is a started function */
	if(cache->node)
	{
		return JIT_MEMORY_ERROR;
	}

	/* If we had a newly allocated page then it has to be freed
	   to let allocate another new page of appropriate size. */
	struct jit_cache_page *p = &cache->pages[cache->numPages - 1];
	if((cache->free_start == ((unsigned char *)p->page))
	   && (cache->free_end == (cache->free_start + cache->pageSize * p->factor)))
	{
		_jit_free_exec(p->page, cache->pageSize * p->factor);

		--(cache->numPages);
		if(cache->pagesLeft >= 0)
		{
			cache->pagesLeft += p->factor;
		}
		cache->free_start = 0;
		cache->free_end = 0;

		if(factor <= p->factor)
		{
			factor = p->factor << 1;
		}
	}

	/* Allocate a new page now */
	AllocCachePage(cache, factor);
	if(!cache->free_start)
	{
		return JIT_MEMORY_TOO_BIG;
	}
	return JIT_MEMORY_OK;
}

jit_function_t
_jit_cache_alloc_function(jit_cache_t cache)
{
	return jit_cnew(struct _jit_function);
}

void
_jit_cache_free_function(jit_cache_t cache, jit_function_t func)
{
	jit_free(func);
}

int
_jit_cache_start_function(jit_cache_t cache, jit_function_t func)
{
	/* Bail out if there is a started function already */
	if(cache->node)
	{
		return JIT_MEMORY_ERROR;
	}
	/* Bail out if the cache is already full */
	if(!cache->free_start)
	{
		return JIT_MEMORY_TOO_BIG;
	}

	/* Save the cache position */
	cache->prev_start = cache->free_start;
	cache->prev_end = cache->free_end;

	/* Allocate a new cache node */
	cache->node = _jit_cache_alloc_data(
		cache, sizeof(struct jit_cache_node), sizeof(void *));
	if(!cache->node)
	{
		return JIT_MEMORY_RESTART;
	}
	cache->node->func = func;

	/* Initialize the function information */
	cache->node->start = cache->free_start;
	cache->node->end = 0;
	cache->node->left = 0;
	cache->node->right = 0;

	return JIT_MEMORY_OK;
}

int
_jit_cache_end_function(jit_cache_t cache, int result)
{
	/* Bail out if there is no started function */
	if(!cache->node)
	{
		return JIT_MEMORY_ERROR;
	}

	/* Determine if we ran out of space while writing the function */
	if(result != JIT_MEMORY_OK)
	{
		/* Restore the saved cache position */
		cache->free_start = cache->prev_start;
		cache->free_end = cache->prev_end;
		cache->node = 0;

		return JIT_MEMORY_RESTART;
	}

	/* Update the method region block and then add it to the lookup tree */
	cache->node->end = cache->free_start;
	AddToLookupTree(cache, cache->node);
	cache->node = 0;

	/* The method is ready to go */
	return JIT_MEMORY_OK;
}

void *
_jit_cache_get_code_break(jit_cache_t cache)
{
	/* Bail out if there is no started function */
	if(!cache->node)
	{
		return 0;
	}

	/* Return the address of the available code area */
	return cache->free_start;
}

void
_jit_cache_set_code_break(jit_cache_t cache, void *ptr)
{
	/* Bail out if there is no started function */
	if(!cache->node)
	{
		return;
	}
	/* Sanity checks */
	if((unsigned char *) ptr < cache->free_start)
	{
		return;
	}
	if((unsigned char *) ptr > cache->free_end)
	{
		return;
	}

	/* Update the address of the available code area */
	cache->free_start = ptr;
}

void *
_jit_cache_get_code_limit(jit_cache_t cache)
{
	/* Bail out if there is no started function */
	if(!cache->node)
	{
		return 0;
	}

	/* Return the end address of the available code area */
	return cache->free_end;
}

void *
_jit_cache_alloc_data(jit_cache_t cache, unsigned long size, unsigned long align)
{
	unsigned char *ptr;

	/* Get memory from the top of the free region, so that it does not
	   overlap with the function code possibly being written at the bottom
	   of the free region */
	ptr = cache->free_end - size;
	ptr = (unsigned char *) (((jit_nuint) ptr) & ~(align - 1));
	if(ptr < cache->free_start)
	{
		/* When we aligned the block, it caused an overflow */
		return 0;
	}

	/* Allocate the block and return it */
	cache->free_end = ptr;
	return ptr;
}

static void *
alloc_code(jit_cache_t cache, unsigned int size, unsigned int align)
{
	unsigned char *ptr;

	/* Bail out if there is a started function */
	if(cache->node)
	{
		return 0;
	}
	/* Bail out if there is no cache available */
	if(!cache->free_start)
	{
		return 0;
	}

	/* Allocate aligned memory */
	ptr = cache->free_start;
	if(align > 1)
	{
		jit_nuint p = ((jit_nuint) ptr + align - 1) & ~(align - 1);
		ptr = (unsigned char *) p;
	}

	/* Do we need to allocate a new cache page? */
	if((ptr + size) > cache->free_end)
	{
		/* Allocate a new page */
		AllocCachePage(cache, 0);

		/* Bail out if the cache is full */
		if(!cache->free_start)
		{
			return 0;
		}

		/* Allocate memory from the new page */
		ptr = cache->free_start;
		if(align > 1)
		{
			jit_nuint p = ((jit_nuint) ptr + align - 1) & ~(align - 1);
			ptr = (unsigned char *) p;
		}
	}

	/* Allocate the block and return it */
	cache->free_start = ptr + size;
	return (void *) ptr;
}

void *
_jit_cache_alloc_trampoline(jit_cache_t cache)
{
	return alloc_code(cache,
			  jit_get_trampoline_size(),
			  jit_get_trampoline_alignment());
}

void
_jit_cache_free_trampoline(jit_cache_t cache, void *trampoline)
{
	/* not supported yet */
}

void *
_jit_cache_alloc_closure(jit_cache_t cache)
{
	return alloc_code(cache,
			  jit_get_closure_size(),
			  jit_get_closure_alignment());
}

void
_jit_cache_free_closure(jit_cache_t cache, void *closure)
{
	/* not supported yet */
}

#if 0
void *
_jit_cache_alloc_no_method(jit_cache_t cache, unsigned long size, unsigned long align)
{
	unsigned char *ptr;

	/* Bail out if there is a started function */
	if(cache->method)
	{
		return 0;
	}
	/* Bail out if there is no cache available */
	if(!cache->free_start)
	{
		return 0;
	}
	/* Bail out if the request is too big to ever be satisfiable */
	if((size + align - 1) > (cache->pageSize * cache->maxPageFactor))
	{
		return 0;
	}

	/* Allocate memory from the top of the current free region, so
	 * that it does not overlap with the method code being written
	 * at the bottom of the free region */
	ptr = cache->free_end - size;
	ptr = (unsigned char *) (((jit_nuint) ptr) & ~((jit_nuint) align - 1));

	/* Do we need to allocate a new cache page? */
	if(ptr < cache->free_start)
	{
		/* Find the appropriate page size */
		int factor = 1;
		while((size + align - 1) > (factor * cache->pageSize)) {
			factor <<= 1;
		}

		/* Try to allocate it */
		AllocCachePage(cache, factor);

		/* Bail out if the cache is full */
		if(!cache->free_start)
		{
			return 0;
		}

		/* Allocate memory from the new page */
		ptr = cache->free_end - size;
		ptr = (unsigned char *) (((jit_nuint) ptr) & ~((jit_nuint) align - 1));
	}

	/* Allocate the block and return it */
	cache->free_end = ptr;
	return (void *)ptr;
}
#endif

void *
_jit_cache_find_function_info(jit_cache_t cache, void *pc)
{
	jit_cache_node_t node = cache->head.right;
	while(node != &(cache->nil))
	{
		if(((unsigned char *)pc) < node->start)
		{
			node = GetLeft(node);
		}
		else if(((unsigned char *)pc) >= node->end)
		{
			node = GetRight(node);
		}
		else
		{
			return node;
		}
	}
	return 0;
}

jit_function_t
_jit_cache_get_function(jit_cache_t cache, void *func_info)
{
	if(func_info)
	{
		jit_cache_node_t node = (jit_cache_node_t) func_info;
		return node->func;
	}
	return 0;
}

void *
_jit_cache_get_function_start(jit_memory_context_t memctx, void *func_info)
{
	if(func_info)
	{
		jit_cache_node_t node = (jit_cache_node_t) func_info;
		return node->start;
	}
	return 0;
}

void *
_jit_cache_get_function_end(jit_memory_context_t memctx, void *func_info)
{
	if(func_info)
	{
		jit_cache_node_t node = (jit_cache_node_t) func_info;
		return node->end;
	}
	return 0;
}

jit_memory_manager_t
jit_default_memory_manager(void)
{
	static const struct jit_memory_manager mm = {

		(jit_memory_context_t (*)(jit_context_t))
		&_jit_cache_create,

		(void (*)(jit_memory_context_t))
		&_jit_cache_destroy,

		(jit_function_info_t (*)(jit_memory_context_t, void *))
		&_jit_cache_find_function_info,

		(jit_function_t (*)(jit_memory_context_t, jit_function_info_t))
		&_jit_cache_get_function,

		(void * (*)(jit_memory_context_t, jit_function_info_t))
		&_jit_cache_get_function_start,

		(void * (*)(jit_memory_context_t, jit_function_info_t))
		&_jit_cache_get_function_end,

		(jit_function_t (*)(jit_memory_context_t))
		&_jit_cache_alloc_function,

		(void (*)(jit_memory_context_t, jit_function_t))
		&_jit_cache_free_function,

		(int (*)(jit_memory_context_t, jit_function_t))
		&_jit_cache_start_function,

		(int (*)(jit_memory_context_t, int))
		&_jit_cache_end_function,

		(int (*)(jit_memory_context_t, int))
		&_jit_cache_extend,

		(void * (*)(jit_memory_context_t))
		&_jit_cache_get_code_limit,

		(void * (*)(jit_memory_context_t))
		&_jit_cache_get_code_break,

		(void (*)(jit_memory_context_t, void *))
		&_jit_cache_set_code_break,

		(void * (*)(jit_memory_context_t))
		&_jit_cache_alloc_trampoline,

		(void (*)(jit_memory_context_t, void *))
		&_jit_cache_free_trampoline,

		(void * (*)(jit_memory_context_t))
		&_jit_cache_alloc_closure,

		(void (*)(jit_memory_context_t, void *))
		&_jit_cache_free_closure,

		(void * (*)(jit_memory_context_t, jit_size_t, jit_size_t))
		&_jit_cache_alloc_data
	};
	return &mm;
}

/*

Using the cache
---------------

To output the code for a method, first call _jit_cache_start_method:

	jit_cache_posn posn;
	int result;

	result = _jit_cache_start_method(cache, &posn, factor,
					 METHOD_ALIGNMENT, method);

"factor" is used to control cache space allocation for the method.
The cache space is allocated by pages.  The value 0 indicates that
the method has to use the space left after the last allocation.
The value 1 or more indicates that the method has to start on a
newly allocated space that must contain the specified number of
consecutive pages.

"METHOD_ALIGNMENT" is used to align the start of the method on an
appropriate boundary for the target CPU.  Use the value 1 if no
special alignment is required.  Note: this value is a hint to the
cache - it may alter the alignment value.

"method" is a value that uniquely identifies the method that is being
translated.  Usually this is the "jit_function_t" pointer.

The function initializes the "posn" structure to point to the start
and end of the space available for the method output.  The function
returns one of three result codes:

	JIT_CACHE_OK       The function call was successful.
	JIT_CACHE_RESTART  The cache does not currently have enough
	                   space to fit any method.  This code may
			   only be returned if the "factor" value
			   was 0.  In this case it is necessary to
			   restart the method output process by
			   calling _jit_cache_start_method again
			   with a bigger "factor" value.
	JIT_CACHE_TOO_BIG  The cache does not have any space left
	                   for allocation.  In this case a restart
			   won't help.

Some CPU optimization guides recommend that labels should be aligned.
This can be achieved using _jit_cache_align.

Once the method code has been output, call _jit_cache_end_method to finalize
the process.  This function returns one of two result codes:

	JIT_CACHE_OK       The method output process was successful.
	JIT_CACHE_RESTART  The cache space overflowed. It is necessary
	                   to restart the method output process by
			   calling _jit_cache_start_method again
			   with a bigger "factor" value.

The caller should repeatedly translate the method while _jit_cache_end_method
continues to return JIT_CACHE_END_RESTART.  Normally there will be no
more than a single request to restart, but the caller should not rely
upon this.  The cache algorithm guarantees that the restart loop will
eventually terminate.

Cache data structure
--------------------

The cache consists of one or more "cache pages", which contain method
code and auxiliary data.  The default size for a cache page is 64k
(JIT_CACHE_PAGE_SIZE).  The size is adjusted to be a multiple
of the system page size (usually 4k), and then stored in "pageSize".

Method code is written into a cache page starting at the bottom of the
page, and growing upwards.  Auxiliary data is written into a cache page
starting at the top of the page, and growing downwards.  When the two
regions meet, a new cache page is allocated and the process restarts.

To allow methods bigger than a single cache page it is possible to
allocate a block of consecutive pages as a single unit. The method
code and auxiliary data is written to such a multiple-page block in
the same manner as into an ordinary page.

Each method has one or more jit_cache_method auxiliary data blocks associated
with it.  These blocks indicate the start and end of regions within the
method.  Normally these regions correspond to exception "try" blocks, or
regular code between "try" blocks.

The jit_cache_method blocks are organised into a red-black tree, which
is used to perform fast lookups by address (_jit_cache_get_method).  These
lookups are used when walking the stack during exceptions or security
processing.

Each method can also have offset information associated with it, to map
between native code addresses and offsets within the original bytecode.
This is typically used to support debugging.  Offset information is stored
as auxiliary data, attached to the jit_cache_method block.

Threading issues
----------------

Writing a method to the cache, querying a method by address, or querying
offset information for a method, are not thread-safe.  The caller should
arrange for a cache lock to be acquired prior to performing these
operations.

Executing methods from the cache is thread-safe, as the method code is
fixed in place once it has been written.

Note: some CPU's require that a special cache flush instruction be
performed before executing method code that has just been written.
This is especially important in SMP environments.  It is the caller's
responsibility to perform this flush operation.

We do not provide locking or CPU flush capabilities in the cache
implementation itself, because the caller may need to perform other
duties before flushing the CPU cache or releasing the lock.

The following is the recommended way to map an "jit_function_t" pointer
to a starting address for execution:

	Look in "jit_function_t" to see if we already have a starting address.
		If so, then bail out.
	Acquire the cache lock.
	Check again to see if we already have a starting address, just
		in case another thread got here first.  If so, then release
		the cache lock and bail out.
	Translate the method.
	Update the "jit_function_t" structure to contain the starting address.
	Force a CPU cache line flush.
	Release the cache lock.

Why aren't methods flushed when the cache fills up?
---------------------------------------------------

In this cache implementation, methods are never "flushed" when the
cache becomes full.  Instead, all translation stops.  This is not a bug.
It is a feature.

In a multi-threaded environment, it is impossible to know if some
other thread is executing the code of a method that may be a candidate
for flushing.  Impossible that is unless one introduces a huge number
of read-write locks, one per method, to prevent a method from being
flushed.  The read locks must be acquired on entry to a method, and
released on exit.  The write locks are acquired prior to translation.

The overhead of introducing all of these locks and the associated cache
data structures is very high.  The only safe thing to do is to assume
that once a method has been translated, its code must be fixed in place
for all time.

We've looked at the code for other Free Software and Open Source JIT's,
and they all use a constantly-growing method cache.  No one has found
a solution to this problem, it seems.  Suggestions are welcome.

To prevent the cache from chewing up all of system memory, it is possible
to set a limit on how far it will grow.  Once the limit is reached, out
of memory will be reported and there is no way to recover.

*/

#ifdef	__cplusplus
};
#endif
