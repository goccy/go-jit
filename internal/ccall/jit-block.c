/*
 * jit-block.c - Functions for manipulating blocks.
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
#include <stdlib.h>

/*@

@cindex jit-block.h

@*/

/* helper data structure for CFG DFS traversal */
typedef struct _jit_block_stack_entry
{
	jit_block_t block;
	int index;
} _jit_block_stack_entry_t;


static void
create_edge(jit_function_t func, jit_block_t src, jit_block_t dst, int flags, int create)
{
	_jit_edge_t edge;

	/* Create edge if required */
	if(create)
	{
		/* Allocate memory for it */
		edge = jit_memory_pool_alloc(&func->builder->edge_pool, struct _jit_edge);
		if(!edge)
		{
			jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
		}

		/* Initialize edge fields */
		edge->src = src;
		edge->dst = dst;
		edge->flags = flags;

		/* Store edge pointers in source and destination nodes */
		src->succs[src->num_succs] = edge;
		dst->preds[dst->num_preds] = edge;
	}

	/* Count it */
	++(src->num_succs);
	++(dst->num_preds);
}

static void
build_edges(jit_function_t func, int create)
{
	jit_block_t src, dst;
	jit_insn_t insn;
	int opcode, flags;
	jit_label_t *labels;
	int index, num_labels;

	/* TODO: Handle catch, finally, filter blocks. */

	for(src = func->builder->entry_block; src != func->builder->exit_block; src = src->next)
	{
		/* Check the last instruction of the block */
		insn = _jit_block_get_last(src);
		opcode = insn ? insn->opcode : JIT_OP_NOP;
		if(opcode >= JIT_OP_RETURN && opcode <= JIT_OP_RETURN_SMALL_STRUCT)
		{
			flags = _JIT_EDGE_RETURN;
			dst = func->builder->exit_block;
		}
		else if(opcode == JIT_OP_BR)
		{
			flags = _JIT_EDGE_BRANCH;
			dst = jit_block_from_label(func, (jit_label_t) insn->dest);
			if(!dst)
			{
				/* Bail out on undefined label */
				jit_exception_builtin(JIT_RESULT_UNDEFINED_LABEL);
			}
		}
		else if(opcode > JIT_OP_BR && opcode <= JIT_OP_BR_NFGE_INV)
		{
			flags = _JIT_EDGE_BRANCH;
			dst = jit_block_from_label(func, (jit_label_t) insn->dest);
			if(!dst)
			{
				/* Bail out on undefined label */
				jit_exception_builtin(JIT_RESULT_UNDEFINED_LABEL);
			}
		}
		else if(opcode == JIT_OP_THROW || opcode == JIT_OP_RETHROW)
		{
			flags = _JIT_EDGE_EXCEPT;
			dst = jit_block_from_label(func, func->builder->catcher_label);
			if(!dst)
			{
				dst = func->builder->exit_block;
			}
		}
		else if(opcode == JIT_OP_CALL_FINALLY || opcode == JIT_OP_CALL_FILTER)
		{
			flags = _JIT_EDGE_EXCEPT;
			dst = jit_block_from_label(func, (jit_label_t) insn->dest);
			if(!dst)
			{
				/* Bail out on undefined label */
				jit_exception_builtin(JIT_RESULT_UNDEFINED_LABEL);
			}
		}
		else if(opcode >= JIT_OP_CALL && opcode <= JIT_OP_CALL_EXTERNAL_TAIL)
		{
			flags = _JIT_EDGE_EXCEPT;
			dst = jit_block_from_label(func, func->builder->catcher_label);
			if(!dst)
			{
				dst = func->builder->exit_block;
			}
		}
		else if(opcode == JIT_OP_JUMP_TABLE)
		{
			labels = (jit_label_t *) insn->value1->address;
			num_labels = (int) insn->value2->address;
			for(index = 0; index < num_labels; index++)
			{
				dst = jit_block_from_label(func, labels[index]);
				if(!dst)
				{
					/* Bail out on undefined label */
					jit_exception_builtin(JIT_RESULT_UNDEFINED_LABEL);
				}
				create_edge(func, src, dst, _JIT_EDGE_BRANCH, create);
			}
			dst = 0;
		}
		else
		{
			dst = 0;
		}

		/* create a branch or exception edge if appropriate */
		if(dst)
		{
			create_edge(func, src, dst, flags, create);
		}
		/* create a fall-through edge if appropriate */
		if(!src->ends_in_dead)
		{
			create_edge(func, src, src->next, _JIT_EDGE_FALLTHRU, create);
		}
	}
}

static void
alloc_edges(jit_function_t func)
{
	jit_block_t block;

	for(block = func->builder->entry_block; block; block = block->next)
	{
		/* Allocate edges to successor nodes */
		if(block->num_succs == 0)
		{
			block->succs = 0;
		}
		else
		{
			block->succs = jit_calloc(block->num_succs, sizeof(_jit_edge_t));
			if(!block->succs)
			{
				jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
			}
			/* Reset edge count for the next build pass */
			block->num_succs = 0;
		}

		/* Allocate edges to predecessor nodes */
		if(block->num_preds == 0)
		{
			block->preds = 0;
		}
		else
		{
			block->preds = jit_calloc(block->num_preds, sizeof(_jit_edge_t));
			if(!block->preds)
			{
				jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
			}
			/* Reset edge count for the next build pass */
			block->num_preds = 0;
		}
	}
}

static void
detach_edge_src(_jit_edge_t edge)
{
	jit_block_t block;
	int index;

	block = edge->src;
	for(index = 0; index < block->num_succs; index++)
	{
		if(block->succs[index] == edge)
		{
			for(block->num_succs--; index < block->num_succs; index++)
			{
				block->succs[index] = block->succs[index + 1];
			}
			block->succs = jit_realloc(block->succs, block->num_succs * sizeof(_jit_edge_t));
			return;
		}
	}
}

static void
detach_edge_dst(_jit_edge_t edge)
{
	jit_block_t block;
	int index;

	block = edge->dst;
	for(index = 0; index < block->num_preds; index++)
	{
		if(block->preds[index] == edge)
		{
			for(block->num_preds--; index < block->num_preds; index++)
			{
				block->preds[index] = block->preds[index + 1];
			}
			block->preds = jit_realloc(block->preds,
						   block->num_preds * sizeof(_jit_edge_t));
			return;
		}
	}
}

static void
attach_edge_dst(_jit_edge_t edge, jit_block_t block)
{
	_jit_edge_t *preds;

	preds = jit_realloc(block->preds, (block->num_preds + 1) * sizeof(_jit_edge_t));
	if(!preds)
	{
		jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
	}

	preds[block->num_preds++] = edge;
	block->preds = preds;
	edge->dst = block;
}

/* Delete edge along with references to it */
static void
delete_edge(jit_function_t func, _jit_edge_t edge)
{
	detach_edge_src(edge);
	detach_edge_dst(edge);
	jit_memory_pool_dealloc(&func->builder->edge_pool, edge);
}

/* Block may not be deleted right when it was found useless from
   the control flow perspective as it might be referenced from
   elsewhere, for instance, from some jit_value_t */
static void
delete_block(jit_block_t block)
{
	jit_free(block->succs);
	block->succs = 0;
	jit_free(block->preds);
	block->preds = 0;
	jit_free(block->insns);
	block->insns = 0;

	block->next = block->func->builder->deleted_blocks;
	block->func->builder->deleted_blocks = block;
}

/* The block is empty if it contains nothing apart from an unconditional branch */
static int
is_empty_block(jit_block_t block)
{
	int index, opcode;

	index = block->num_insns;
	if(index == 0)
	{
		return 1;
	}

	opcode = block->insns[--index].opcode;
	if(opcode != JIT_OP_NOP && opcode != JIT_OP_MARK_OFFSET && opcode != JIT_OP_BR)
	{
		return 0;
	}

	while(index > 0)
	{
		opcode = block->insns[--index].opcode;
		if(opcode != JIT_OP_NOP && opcode != JIT_OP_MARK_OFFSET)
		{
			return 0;
		}
	}

	return 1;
}

#ifdef _JIT_BLOCK_DEBUG
static void
label_loop_check(jit_function_t func, jit_block_t block, jit_label_t label)
{
	jit_label_t block_label = block->label;
	while(block_label != jit_label_undefined)
	{
		if(block_label == label)
		{
			abort();
		}
		block_label = func->builder->label_info[block_label].alias;
	}
}
#endif

/* Merge labels of the src block with labels of the dst block but retain
   the address_of labels.  This requires the address_of labels to be used
   exclusively as such, no branches are allowed to use address_of labels.
   This is ensured by the split_address_of() function. */
static void
merge_labels(jit_function_t func, jit_block_t src, jit_block_t dst)
{
	_jit_label_info_t *info;
	jit_label_t label, alias;

	label = src->label;
	src->label = jit_label_undefined;

#ifdef _JIT_BLOCK_DEBUG
	label_loop_check(func, dst, label);
#endif

	while(label != jit_label_undefined)
	{
		info = &func->builder->label_info[label];
		alias = info->alias;

		if((info->flags & JIT_LABEL_ADDRESS_OF) == 0)
		{
			info->block = dst;
			info->alias = dst->label;
			dst->label = label;
		}
		else
		{
			info->alias = src->label;
			src->label = label;
		}

		label = alias;
	}
}

/* Merge empty block with its successor */
static void
merge_empty(jit_function_t func, jit_block_t block, int *changed)
{
	_jit_edge_t succ_edge, pred_edge, fallthru_edge;
	jit_block_t succ_block;
	int index;

	/* Find block successor */
	succ_edge = block->succs[0];
	succ_block = succ_edge->dst;

	/* Retarget labels bound to this block to the successor block. */
	merge_labels(func, block, succ_block);

	/* Retarget all incoming edges except a fallthrough edge */
	fallthru_edge = 0;
	for(index = 0; index < block->num_preds; index++)
	{
		pred_edge = block->preds[index];
		if(pred_edge->flags == _JIT_EDGE_FALLTHRU)
		{
			fallthru_edge = pred_edge;
		}
		else
		{
			*changed = 1;
			attach_edge_dst(pred_edge, succ_block);
		}
	}

	/* Unless the block is taken address of, the incoming fallthrough edge
	   can be retargeted and then the block deleted if the outgoing edge is
	   also fallthrough. */
	if(!block->address_of && fallthru_edge && succ_edge->flags == _JIT_EDGE_FALLTHRU)
	{
		*changed = 1;
		attach_edge_dst(fallthru_edge, succ_block);
		fallthru_edge = 0;
	}

	/* Free the block if there is no incoming edge left and it is not taken
	   address of. Otherwise adjust the preds array accordingly.  */
	if(fallthru_edge)
	{
		if(block->num_preds > 1)
		{
			block->num_preds = 1;
			block->preds = jit_realloc(block->preds, sizeof(_jit_edge_t));
			block->preds[0] = fallthru_edge;
		}
	}
	else if(block->address_of)
	{
		if(block->num_preds > 0)
		{
			block->num_preds = 0;
			jit_free(block->preds);
			block->preds = 0;
		}
	}
	else
	{
		detach_edge_dst(succ_edge);
		jit_memory_pool_dealloc(&func->builder->edge_pool, succ_edge);
		_jit_block_detach(block, block);
		delete_block(block);
	}
}

/* Combine non-empty block with its successor */
static void
combine_block(jit_function_t func, jit_block_t block, int *changed)
{
	jit_block_t succ_block;
	int branch, num_insns, max_insns;
	jit_insn_t insns;

	/* Find block successor */
	succ_block = block->succs[0]->dst;

	/* Does block end with a (redundant) branch instruction? */
	branch = (block->succs[0]->flags == _JIT_EDGE_BRANCH);

	/* If the branch is there then preallocate memory for it,
	   doing it here simplifies handling of the out-of-memory
	   condition */
	if(branch && !succ_block->max_insns)
	{
		succ_block->insns = jit_malloc(sizeof(struct _jit_insn));
		if(!succ_block->insns)
		{
			jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
		}
		succ_block->max_insns = 1;
	}

	/* Allocate enough memory for combined instructions */
	max_insns = block->max_insns;
	num_insns = block->num_insns + succ_block->num_insns;
	if(num_insns > max_insns)
	{
		max_insns = num_insns;
		insns = (jit_insn_t) jit_realloc(block->insns,
						 max_insns * sizeof(struct _jit_insn));
		if(!insns)
		{
			jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
		}
	}
	else
	{
		insns = block->insns;
	}

	/* Copy instruction from the successor block after the instructions
	   of the original block */
	if(succ_block->num_insns)
	{
		jit_memcpy(insns + block->num_insns,
			   succ_block->insns,
			   succ_block->num_insns * sizeof(struct _jit_insn));
	}

	/* Move combined instructions to the successor, but if there was
	   a branch in the original block then keep the branch around,
	   merge_empty() will take care of it if it may be optimized away.
	   To reduce the number of allocations, swap the arrays around
	   rather than allocating a fresh array for the empty block. */
	block->insns = succ_block->insns;
	block->max_insns = succ_block->max_insns;
	if(branch)
	{
		/* Copy the branch instruction */
		jit_memcpy(block->insns,
			   insns + block->num_insns - 1,
			   sizeof(struct _jit_insn));
		/* In the combined block turn branch into NOP */
		insns[block->num_insns - 1].opcode = JIT_OP_NOP;
	}
	block->num_insns = branch;
	succ_block->insns = insns;
	succ_block->max_insns = max_insns;
	succ_block->num_insns = num_insns;

	merge_empty(func, block, changed);
}

/* Allow branch optimization by splitting the label that is both a branch target
   and an address-of opcode source into two separate labels with single role.
   TODO: handle jump tables. */
static void
split_address_of(jit_function_t func, jit_block_t block, jit_label_t label)
{
	int index;
	jit_insn_t insn;
	jit_label_t branch_label;
	jit_label_t *jump_labels;
	int jump_index, num_labels;

	branch_label = jit_label_undefined;

	for(index = 0; index < block->num_preds; index++)
	{
		if(block->preds[index]->flags != _JIT_EDGE_BRANCH)
		{
			continue;
		}

		if(branch_label == jit_label_undefined)
		{
			branch_label = (func->builder->next_label)++;
			if(!_jit_block_record_label(block, branch_label))
			{
				jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
			}
		}

		insn = _jit_block_get_last(block->preds[index]->src);
		if(insn->opcode != JIT_OP_JUMP_TABLE)
		{
			insn->dest = (jit_value_t)branch_label;
		}
		else
		{
			jump_labels = (jit_label_t *) insn->value1->address;
			num_labels = (int) insn->value2->address;
			for(jump_index = 0; jump_index < num_labels; jump_index++)
			{
				if(jump_labels[jump_index] == label)
				{
					jump_labels[jump_index] = branch_label;
				}
			}
		}
	}
}

/* Mark blocks that might be taken address of */
static void
set_address_of(jit_function_t func)
{
	int index, count;
	jit_block_t block;

	count = func->builder->max_label_info;
	for(index = 0; index < count; index++)
	{
		block = func->builder->label_info[index].block;
		if(block && (func->builder->label_info[index].flags & JIT_LABEL_ADDRESS_OF) != 0)
		{
			block->address_of = 1;
			split_address_of(func, block, index);
		}
	}
}

/* Delete block along with references to it */
static void
eliminate_block(jit_block_t block)
{
	_jit_edge_t edge;
	int index;

	/* Detach block from the list */
	_jit_block_detach(block, block);

	/* Remove control flow graph edges */
	for(index = 0; index < block->num_succs; index++)
	{
		edge = block->succs[index];
		detach_edge_dst(edge);
		jit_memory_pool_dealloc(&block->func->builder->edge_pool, edge);
	}
	for(index = 0; index < block->num_preds; index++)
	{
		edge = block->preds[index];
		detach_edge_src(edge);
		jit_memory_pool_dealloc(&block->func->builder->edge_pool, edge);
	}

	/* Finally delete the block */
	delete_block(block);
}

#if 0

/* Visit all successive blocks recursively */
static void
visit_reachable(jit_block_t block)
{
	int index;

	if(!block->visited)
	{
		block->visited = 1;
		for(index = 0; index < block->num_succs; index++)
		{
			visit_reachable(block->succs[index]->dst);
		}
	}
}

#endif

/* Eliminate unreachable blocks */
static void
eliminate_unreachable(jit_function_t func)
{
	jit_block_t block, next_block;

	block = func->builder->entry_block;
	while(block != func->builder->exit_block)
	{
		next_block = block->next;
		if(block->visited)
		{
			block->visited = 0;
		}
		else if(!block->address_of)
		{
			eliminate_block(block);
		}
		block = next_block;
	}
}

/* Clear visited blocks */
static void
clear_visited(jit_function_t func)
{
	jit_block_t block;

	for(block = func->builder->entry_block; block; block = block->next)
	{
		block->visited = 0;
	}
}

/* TODO: maintain the block count as the blocks are created/deleted */
static int
count_blocks(jit_function_t func)
{
	int count;
	jit_block_t block;

	count = 0;
	for(block = func->builder->entry_block; block; block = block->next)
	{
		++count;
	}
	return count;
}

/* Release block order memory */
static void
free_order(jit_function_t func)
{
	jit_free(func->builder->block_order);
	func->builder->block_order = NULL;
	func->builder->num_block_order = 0;
}

int
_jit_block_init(jit_function_t func)
{
	func->builder->entry_block = _jit_block_create(func);
	if(!func->builder->entry_block)
	{
		return 0;
	}

	func->builder->exit_block = _jit_block_create(func);
	if(!func->builder->exit_block)
	{
		return 0;
	}

	func->builder->entry_block->next = func->builder->exit_block;
	func->builder->exit_block->prev = func->builder->entry_block;
	return 1;
}

void
_jit_block_free(jit_function_t func)
{
	jit_block_t block, next;

	free_order(func);

	block = func->builder->entry_block;
	while(block)
	{
		next = block->next;
		_jit_block_destroy(block);
		block = next;
	}

	block = func->builder->deleted_blocks;
	while(block)
	{
		next = block->next;
		_jit_block_destroy(block);
		block = next;
	}

	func->builder->entry_block = 0;
	func->builder->exit_block = 0;
}

void
_jit_block_build_cfg(jit_function_t func)
{
	/* Count the edges */
	build_edges(func, 0);

	/* Allocate memory for edges */
	alloc_edges(func);

	/* Actually build the edges */
	build_edges(func, 1);
}

static int
_jit_invert_condition (int opcode)
{
	switch(opcode)
	{
	case JIT_OP_BR_IEQ:	opcode = JIT_OP_BR_INE;      break;
	case JIT_OP_BR_INE:	opcode = JIT_OP_BR_IEQ;      break;
	case JIT_OP_BR_ILT:	opcode = JIT_OP_BR_IGE;      break;
	case JIT_OP_BR_ILT_UN:	opcode = JIT_OP_BR_IGE_UN;   break;
	case JIT_OP_BR_ILE:	opcode = JIT_OP_BR_IGT;      break;
	case JIT_OP_BR_ILE_UN:	opcode = JIT_OP_BR_IGT_UN;   break;
	case JIT_OP_BR_IGT:	opcode = JIT_OP_BR_ILE;      break;
	case JIT_OP_BR_IGT_UN:	opcode = JIT_OP_BR_ILE_UN;   break;
	case JIT_OP_BR_IGE:	opcode = JIT_OP_BR_ILT;      break;
	case JIT_OP_BR_IGE_UN:	opcode = JIT_OP_BR_ILT_UN;   break;
	case JIT_OP_BR_LEQ:	opcode = JIT_OP_BR_LNE;      break;
	case JIT_OP_BR_LNE:	opcode = JIT_OP_BR_LEQ;      break;
	case JIT_OP_BR_LLT:	opcode = JIT_OP_BR_LGE;      break;
	case JIT_OP_BR_LLT_UN:	opcode = JIT_OP_BR_LGE_UN;   break;
	case JIT_OP_BR_LLE:	opcode = JIT_OP_BR_LGT;      break;
	case JIT_OP_BR_LLE_UN:	opcode = JIT_OP_BR_LGT_UN;   break;
	case JIT_OP_BR_LGT:	opcode = JIT_OP_BR_LLE;      break;
	case JIT_OP_BR_LGT_UN:	opcode = JIT_OP_BR_LLE_UN;   break;
	case JIT_OP_BR_LGE:	opcode = JIT_OP_BR_LLT;      break;
	case JIT_OP_BR_LGE_UN:	opcode = JIT_OP_BR_LLT_UN;   break;
	case JIT_OP_BR_FEQ:	opcode = JIT_OP_BR_FNE;      break;
	case JIT_OP_BR_FNE:	opcode = JIT_OP_BR_FEQ;      break;
	case JIT_OP_BR_FLT:	opcode = JIT_OP_BR_FGE_INV;      break;
	case JIT_OP_BR_FLE:	opcode = JIT_OP_BR_FGT_INV;      break;
	case JIT_OP_BR_FGT:	opcode = JIT_OP_BR_FLE_INV;      break;
	case JIT_OP_BR_FGE:	opcode = JIT_OP_BR_FLT_INV;      break;
	case JIT_OP_BR_FLT_INV:	opcode = JIT_OP_BR_FGE;  break;
	case JIT_OP_BR_FLE_INV:	opcode = JIT_OP_BR_FGT;  break;
	case JIT_OP_BR_FGT_INV:	opcode = JIT_OP_BR_FLE;  break;
	case JIT_OP_BR_FGE_INV:	opcode = JIT_OP_BR_FLT;  break;
	case JIT_OP_BR_DEQ:	opcode = JIT_OP_BR_DNE;      break;
	case JIT_OP_BR_DNE:	opcode = JIT_OP_BR_DEQ;      break;
	case JIT_OP_BR_DLT:	opcode = JIT_OP_BR_DGE_INV;      break;
	case JIT_OP_BR_DLE:	opcode = JIT_OP_BR_DGT_INV;      break;
	case JIT_OP_BR_DGT:	opcode = JIT_OP_BR_DLE_INV;      break;
	case JIT_OP_BR_DGE:	opcode = JIT_OP_BR_DLT_INV;      break;
	case JIT_OP_BR_DLT_INV:	opcode = JIT_OP_BR_DGE;  break;
	case JIT_OP_BR_DLE_INV:	opcode = JIT_OP_BR_DGT;  break;
	case JIT_OP_BR_DGT_INV:	opcode = JIT_OP_BR_DLE;  break;
	case JIT_OP_BR_DGE_INV:	opcode = JIT_OP_BR_DLT;  break;
	case JIT_OP_BR_NFEQ:	opcode = JIT_OP_BR_NFNE;     break;
	case JIT_OP_BR_NFNE:	opcode = JIT_OP_BR_NFEQ;     break;
	case JIT_OP_BR_NFLT:	opcode = JIT_OP_BR_NFGE_INV;     break;
	case JIT_OP_BR_NFLE:	opcode = JIT_OP_BR_NFGT_INV;     break;
	case JIT_OP_BR_NFGT:	opcode = JIT_OP_BR_NFLE_INV;     break;
	case JIT_OP_BR_NFGE:	opcode = JIT_OP_BR_NFLT_INV;     break;
	case JIT_OP_BR_NFLT_INV:	opcode = JIT_OP_BR_NFGE; break;
	case JIT_OP_BR_NFLE_INV:	opcode = JIT_OP_BR_NFGT; break;
	case JIT_OP_BR_NFGT_INV:	opcode = JIT_OP_BR_NFLE; break;
	case JIT_OP_BR_NFGE_INV:	opcode = JIT_OP_BR_NFLT; break;
	default:		abort();
	}
	return opcode;
}

void
_jit_block_clean_cfg(jit_function_t func)
{
	int index, changed;
	jit_block_t block;
	jit_insn_t insn;

	/*
	 * The code below is based on the Clean algorithm described in
	 * "Engineering a Compiler" by Keith D. Cooper and Linda Torczon,
	 * section 10.3.1 "Eliminating Useless and Unreachable Code"
	 * (originally presented in a paper by Rob Shillner and John Lu
	 * http://www.cs.princeton.edu/~ras/clean.ps).
	 *
	 * Because libjit IR differs from ILOC the algorithm here has
	 * some differences too.
	 */

	if(!_jit_block_compute_postorder(func))
	{
		jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
	}

	set_address_of(func);
	eliminate_unreachable(func);

 loop:
	changed = 0;

	/* Go through blocks in post order skipping the entry and exit blocks */
	for(index = 1; index < (func->builder->num_block_order - 1); index++)
	{
		block = func->builder->block_order[index];
		if(block->num_succs == 0)
		{
			continue;
		}

		/* Take care of redundant branches that is, if possible, either
		   replace a branch with NOP turning it to a fallthrough case
		   or reduce a conditional branch to unconditional */
		if(block->succs[0]->flags == _JIT_EDGE_BRANCH)
		{
			insn = _jit_block_get_last(block);
			if(insn->opcode == JIT_OP_JUMP_TABLE)
			{
				/* skip jump tables, handle only branches */
				continue;
			}
			if(block->succs[0]->dst == block->next)
			{
				/* Replace useless branch with NOP */
				changed = 1;
				insn->opcode = JIT_OP_NOP;
				if(block->num_succs == 2)
				{
					/* For conditional branch delete the branch
					   edge while leaving the fallthough edge
					   intact */
#ifdef _JIT_BLOCK_DEBUG
					printf("%d cbranch->fallthru %d\n", index, block->label);
#endif
					delete_edge(func, block->succs[0]);
				}
				else
				{
					/* For unconditional branch replace the branch
					   edge with a fallthrough edge */
#ifdef _JIT_BLOCK_DEBUG
					printf("%d ubranch->fallthru %d\n", index, block->label);
#endif
					block->ends_in_dead = 0;
					block->succs[0]->flags = _JIT_EDGE_FALLTHRU;
				}
			}
			else if(block->num_succs == 2
				&& block->next->num_succs == 1
				&& block->next->succs[0]->flags == _JIT_EDGE_BRANCH
				&& block->succs[0]->dst == block->next->succs[0]->dst
				&& is_empty_block(block->next))
			{
				/* For conditional branch followed by unconditional
				   that has the same target make the first branch
				   unconditional too, remove the fallthrough edge while
				   leaving the branch edge intact */
#ifdef _JIT_BLOCK_DEBUG
				printf("%d cbranch->ubranch %d\n", index, block->label);
#endif
				changed = 1;
				insn->opcode = JIT_OP_BR;
				block->ends_in_dead = 1;
				delete_edge(func, block->succs[1]);
			}
			else if(block->num_succs == 2
				&& is_empty_block(block->next)
				&& block->next->num_succs == 1
				/* This transformation is not safe if
				   the block we're rewriting has other
				   predecessors, or has had its
				   address taken.  */
				&& block->next->num_preds == 1
				&& block->next->address_of == 0
				&& block->next->succs[0]->flags == _JIT_EDGE_BRANCH
				&& block->succs[0]->dst == block->next->next)
			{
				/* We have a conditional branch, that
				   branches around the next block, and
				   the next block consists of just a
				   jump, like:

					if l7 != 3 then goto .L0
					goto .L1
					.L0:

				   In this case we can invert the
				   condition and retarget the jump,
				   resulting in:
				   
					if l7 == 3 then goto .L1
					nop
					.L0:
				*/
				insn->opcode = _jit_invert_condition(insn->opcode);
				detach_edge_dst(block->succs[0]);
				attach_edge_dst(block->succs[0], block->next->succs[0]->dst);
				insn->dest = (jit_value_t) block->next->succs[0]->dst->label;
				detach_edge_dst(block->next->succs[0]);
				attach_edge_dst(block->next->succs[0], block->next->next);
				block->next->succs[0]->flags = _JIT_EDGE_FALLTHRU;
				/* Rewrite the last instruction of the
				   unnecessary block to be a NOP.  */
				block->next->insns[block->next->num_insns - 1].opcode = JIT_OP_NOP;
				block->next->ends_in_dead = 0;
				changed = 1;
			}
		}

		/* Try to simplify basic blocks that end with fallthrough or
		   unconditional branch */
		if(block->num_succs == 1
		   && (block->succs[0]->flags == _JIT_EDGE_BRANCH
		       || block->succs[0]->flags == _JIT_EDGE_FALLTHRU))
		{
			if(is_empty_block(block))
			{
				/* Remove empty block */
#ifdef _JIT_BLOCK_DEBUG
				printf("%d merge_empty %d\n", index, block->label);
#endif
				merge_empty(func, block, &changed);
			}
			else if(block->succs[0]->dst->num_preds == 1
				&& block->succs[0]->dst->address_of == 0)
			{
				/* Combine with the successor block if it has
				   only one predecessor */
#ifdef _JIT_BLOCK_DEBUG
				printf("%d combine_block %d\n", index, block->label);
#endif
				combine_block(func, block, &changed);
			}

			/* TODO: "hoist branch" part of the Clean algorithm.
			   It is somewhat complicated as libjit conditional
			   branches differ too much from ILOC conditional
			   branches */
		}
	}

	if(changed)
	{
		if(!_jit_block_compute_postorder(func))
		{
			jit_exception_builtin(JIT_RESULT_OUT_OF_MEMORY);
		}
		clear_visited(func);
		goto loop;
	}
}

int
_jit_block_compute_postorder(jit_function_t func)
{
	int num_blocks, index, num, top;
	jit_block_t *blocks, block, succ;
	_jit_block_stack_entry_t *stack;

	if(func->builder->block_order)
	{
		free_order(func);
	}

	num_blocks = count_blocks(func);

	blocks = (jit_block_t *) jit_malloc(num_blocks * sizeof(jit_block_t));
	if(!blocks)
	{
		return 0;
	}

	stack = (_jit_block_stack_entry_t *) jit_malloc(num_blocks * sizeof(_jit_block_stack_entry_t));
	if(!stack)
	{
		jit_free(blocks);
		return 0;
	}

	func->builder->entry_block->visited = 1;
	stack[0].block = func->builder->entry_block;
	stack[0].index = 0;
	top = 1;
	num = 0;
	do
	{
		block = stack[top - 1].block;
		index = stack[top - 1].index;

		if(index == block->num_succs)
		{
			blocks[num++] = block;
			--top;
		}
		else
		{
			succ = block->succs[index]->dst;
			if(succ->visited)
			{
				stack[top - 1].index = index + 1;
			}
			else
			{
				succ->visited = 1;
				stack[top].block = succ;
				stack[top].index = 0;
				++top;
			}
		}
	}
	while(top);

	jit_free(stack);
	if(num < num_blocks)
	{
		blocks = jit_realloc(blocks, num * sizeof(jit_block_t));
	}

	func->builder->block_order = blocks;
	func->builder->num_block_order = num;
	return 1;
}

jit_block_t
_jit_block_create(jit_function_t func)
{
	jit_block_t block;

	/* Allocate memory for the block */
	block = jit_cnew(struct _jit_block);
	if(!block)
	{
		return 0;
	}

	/* Initialize the block */
	block->func = func;
	block->label = jit_label_undefined;

	return block;
}

void
_jit_block_destroy(jit_block_t block)
{
	/* Free all the memory owned by the block. CFG edges are not freed
	   because each edge is shared between two blocks so the ownership
	   of the edge is ambiguous. Sometimes an edge may be redirected to
	   another block rather than freed. Therefore edges are freed (or
	   not freed) separately. However succs and preds arrays are freed,
	   these contain pointers to edges, not edges themselves. */
	jit_meta_destroy(&block->meta);
	jit_free(block->succs);
	jit_free(block->preds);
	jit_free(block->insns);
	jit_free(block);
}

void
_jit_block_detach(jit_block_t first, jit_block_t last)
{
	last->next->prev = first->prev;
	first->prev->next = last->next;
}

void
_jit_block_attach_after(jit_block_t block, jit_block_t first, jit_block_t last)
{
	first->prev = block;
	last->next = block->next;
	block->next->prev = last;
	block->next = first;
}

void
_jit_block_attach_before(jit_block_t block, jit_block_t first, jit_block_t last)
{
	first->prev = block->prev;
	last->next = block;
	block->prev->next = first;
	block->prev = last;
}

/* Make space for the label in the label info table */
static int
ensure_label_table(jit_function_t func, jit_label_t label)
{
	jit_label_t num;
	_jit_label_info_t *info;

	if(label >= func->builder->max_label_info)
	{
		num = func->builder->max_label_info;
		if(num < 64)
		{
			num = 64;
		}
		while(num <= label)
		{
			num *= 2;
		}

		info = (_jit_label_info_t *) jit_realloc(func->builder->label_info,
							 num * sizeof(_jit_label_info_t));
		if(!info)
		{
			return 0;
		}

		jit_memzero(info + func->builder->max_label_info,
			    sizeof(_jit_label_info_t) * (num - func->builder->max_label_info));
		func->builder->label_info = info;
		func->builder->max_label_info = num;
	}

	return 1;
}

int
_jit_block_record_label(jit_block_t block, jit_label_t label)
{
	jit_builder_t builder;

	if(!ensure_label_table(block->func, label))
	{
		return 0;
	}

	builder = block->func->builder;

	/* Bail out on previously recorded label */
	if(builder->label_info[label].block)
	{
		return 0;
	}

	/* Record label info to the table */
	builder->label_info[label].block = block;
	builder->label_info[label].alias = block->label;
	block->label = label;

	return 1;
}

int
_jit_block_record_label_flags(jit_function_t func, jit_label_t label, int flags)
{
	if(!ensure_label_table(func, label))
	{
		return 0;
	}

	func->builder->label_info[label].flags = flags;
	return 1;
}

jit_insn_t
_jit_block_add_insn(jit_block_t block)
{
	int max_insns;
	jit_insn_t insns;

	/* Make space for the instruction in the block's instruction list */
	if(block->num_insns == block->max_insns)
	{
		max_insns = block->max_insns ? block->max_insns * 2 : 4;
		insns = (jit_insn_t) jit_realloc(block->insns,
						 max_insns * sizeof(struct _jit_insn));
		if(!insns)
		{
			return 0;
		}

		block->insns = insns;
		block->max_insns = max_insns;
	}

	/* Zero-init the instruction */
	jit_memzero(&block->insns[block->num_insns], sizeof(struct _jit_insn));

	/* Return the instruction, which is now ready to fill in */
	return &block->insns[block->num_insns++];
}

jit_insn_t
_jit_block_get_last(jit_block_t block)
{
	if(block->num_insns > 0)
	{
		return &block->insns[block->num_insns - 1];
	}
	else
	{
		return 0;
	}
}

int
_jit_block_is_final(jit_block_t block)
{
	for(block = block->next; block; block = block->next)
	{
		if(block->num_insns)
		{
			return 0;
		}
	}
	return 1;
}

/*@
 * @deftypefun jit_function_t jit_block_get_function (jit_block_t @var{block})
 * Get the function that a particular @var{block} belongs to.
 * @end deftypefun
@*/
jit_function_t
jit_block_get_function(jit_block_t block)
{
	if(block)
	{
		return block->func;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_context_t jit_block_get_context (jit_block_t @var{block})
 * Get the context that a particular @var{block} belongs to.
 * @end deftypefun
@*/
jit_context_t
jit_block_get_context(jit_block_t block)
{
	if(block)
	{
		return block->func->context;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_label_t jit_block_get_label (jit_block_t @var{block})
 * Get the label associated with a block.
 * @end deftypefun
@*/
jit_label_t
jit_block_get_label(jit_block_t block)
{
	if(block)
	{
		return block->label;
	}
	return jit_label_undefined;
}

/*@
 * @deftypefun jit_label_t jit_block_get_next_label (jit_block_t @var{block}, jit_label_t @var{label})
 * Get the next label associated with a block.
 * @end deftypefun
@*/
jit_label_t
jit_block_get_next_label(jit_block_t block, jit_label_t label)
{
	jit_builder_t builder;
	if(block)
	{
		if(label == jit_label_undefined)
		{
			return block->label;
		}
		builder = block->func->builder;
		if(builder
		   && label < builder->max_label_info
		   && block == builder->label_info[label].block)
		{
			return builder->label_info[label].alias;
		}
	}
	return jit_label_undefined;
}

/*@
 * @deftypefun jit_block_t jit_block_next (jit_function_t @var{func}, jit_block_t @var{previous})
 * Iterate over the blocks in a function, in order of their creation.
 * The @var{previous} argument should be NULL on the first call.
 * This function will return NULL if there are no further blocks to iterate.
 * @end deftypefun
@*/
jit_block_t
jit_block_next(jit_function_t func, jit_block_t previous)
{
	if(previous)
	{
		return previous->next;
	}
	else if(func && func->builder)
	{
		return func->builder->entry_block;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_block_t jit_block_previous (jit_function_t @var{func}, jit_block_t @var{previous})
 * Iterate over the blocks in a function, in reverse order of their creation.
 * The @var{previous} argument should be NULL on the first call.
 * This function will return NULL if there are no further blocks to iterate.
 * @end deftypefun
@*/
jit_block_t
jit_block_previous(jit_function_t func, jit_block_t previous)
{
	if(previous)
	{
		return previous->prev;
	}
	else if(func && func->builder)
	{
		return func->builder->exit_block;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_block_t jit_block_from_label (jit_function_t @var{func}, jit_label_t @var{label})
 * Get the block that corresponds to a particular @var{label}.
 * Returns NULL if there is no block associated with the label.
 * @end deftypefun
@*/
jit_block_t
jit_block_from_label(jit_function_t func, jit_label_t label)
{
	if(func && func->builder && label < func->builder->max_label_info)
	{
		return func->builder->label_info[label].block;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun int jit_block_set_meta (jit_block_t @var{block}, int @var{type}, void *@var{data}, jit_meta_free_func @var{free_data})
 * Tag a block with some metadata.  Returns zero if out of memory.
 * If the @var{type} already has some metadata associated with it, then
 * the previous value will be freed.  Metadata may be used to store
 * dependency graphs, branch prediction information, or any other
 * information that is useful to optimizers or code generators.
 *
 * Metadata type values of 10000 or greater are reserved for internal use.
 * @end deftypefun
@*/
int
jit_block_set_meta(jit_block_t block, int type, void *data, jit_meta_free_func free_data)
{
	return jit_meta_set(&(block->meta), type, data, free_data, block->func);
}

/*@
 * @deftypefun {void *} jit_block_get_meta (jit_block_t @var{block}, int @var{type})
 * Get the metadata associated with a particular tag.  Returns NULL
 * if @var{type} does not have any metadata associated with it.
 * @end deftypefun
@*/
void *
jit_block_get_meta(jit_block_t block, int type)
{
	return jit_meta_get(block->meta, type);
}

/*@
 * @deftypefun void jit_block_free_meta (jit_block_t @var{block}, int @var{type})
 * Free metadata of a specific type on a block.  Does nothing if
 * the @var{type} does not have any metadata associated with it.
 * @end deftypefun
@*/
void
jit_block_free_meta(jit_block_t block, int type)
{
	jit_meta_free(&(block->meta), type);
}

/*@
 * @deftypefun int jit_block_is_reachable (jit_block_t @var{block})
 * Determine if a block is reachable from some other point in
 * its function.  Unreachable blocks can be discarded in their
 * entirety.  If the JIT is uncertain as to whether a block is
 * reachable, or it does not wish to perform expensive flow
 * analysis to find out, then it will err on the side of caution
 * and assume that it is reachable.
 * @end deftypefun
@*/
int
jit_block_is_reachable(jit_block_t block)
{
	jit_block_t entry;

	/* Simple-minded reachability analysis that bothers only with
	   fall-through control flow. The block is considered reachable
	   if a) it is the entry block b) it has any label c) there is
	   fall-through path to it from one of the above. */
	entry = block->func->builder->entry_block;
	while(block != entry && block->label == jit_label_undefined)
	{
		block = block->prev;
		if(block->ends_in_dead)
		{
			/* There is no fall-through path from the prev block */
			return 0;
		}
	}

	return 1;
}

/*@
 * @deftypefun int jit_block_ends_in_dead (jit_block_t @var{block})
 * Determine if a block ends in a "dead" marker.  That is, control
 * will not fall out through the end of the block.
 * @end deftypefun
@*/
int
jit_block_ends_in_dead(jit_block_t block)
{
	return block->ends_in_dead;
}

/*@
 * @deftypefun int jit_block_current_is_dead (jit_function_t @var{func})
 * Determine if the current point in the function is dead.  That is,
 * there are no existing branches or fall-throughs to this point.
 * This differs slightly from @code{jit_block_ends_in_dead} in that
 * this can skip past zero-length blocks that may not appear to be
 * dead to find the dead block at the head of a chain of empty blocks.
 * @end deftypefun
@*/
int
jit_block_current_is_dead(jit_function_t func)
{
	jit_block_t block = jit_block_previous(func, 0);
	return !block || jit_block_ends_in_dead(block) || !jit_block_is_reachable(block);
}
