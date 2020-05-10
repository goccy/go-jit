/*
 * jit-reg-alloc.h - Register allocation routines for the JIT.
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

#ifndef	_JIT_REG_ALLOC_H
#define	_JIT_REG_ALLOC_H

#include "jit-rules.h"
#include "jit-reg-class.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * The maximum number of values per instruction.
 */
#define _JIT_REGS_VALUE_MAX		3

/*
 * The maximum number of temporaries per instruction.
 */
#define _JIT_REGS_SCRATCH_MAX		6

/*
 * Flags for _jit_regs_init().
 */
#define _JIT_REGS_TERNARY		0x0001
#define _JIT_REGS_BRANCH		0x0002
#define _JIT_REGS_COPY			0x0004
#define _JIT_REGS_FREE_DEST		0x0008
#define _JIT_REGS_COMMUTATIVE		0x0010
#define _JIT_REGS_STACK			0x0020
#define _JIT_REGS_X87_ARITH		0x0040
#define _JIT_REGS_REVERSIBLE		0X0080

/*
 * Flags for _jit_regs_init_dest(), _jit_regs_init_value1(), and
 * _jit_regs_init_value2().
 */
#define _JIT_REGS_CLOBBER		0x0001
#define _JIT_REGS_EARLY_CLOBBER		0x0002

/*
 * Flags returned by _jit_regs_select_insn().
 */
#define _JIT_REGS_NO_POP		0x0001
#define _JIT_REGS_FLIP_ARGS		0x0002
#define _JIT_REGS_REVERSE		0x0004

/*
 * Contains register assignment data for single operand.
 */
typedef struct
{
	jit_value_t	value;
	int		reg;
	int		other_reg;
	int		stack_reg;
	_jit_regclass_t	*regclass;
	unsigned	live : 1;
	unsigned	used : 1;
	unsigned	clobber : 1;
	unsigned	early_clobber : 1;
	unsigned	duplicate : 1;
	unsigned	thrash : 1;
	unsigned	store : 1;
	unsigned	load : 1;
	unsigned	copy : 1;
	unsigned	kill : 1;

} _jit_regdesc_t;

/*
 * Contains scratch register assignment data.
 */
typedef struct
{
	int		reg;
	_jit_regclass_t	*regclass;

} _jit_scratch_t;

/*
 * Contains register assignment data for instruction.
 */
typedef struct
{
	_jit_regdesc_t	descs[_JIT_REGS_VALUE_MAX];
	_jit_scratch_t	scratch[_JIT_REGS_SCRATCH_MAX];
	int		num_scratch;

	unsigned	ternary : 1;
	unsigned	branch : 1;
	unsigned	copy : 1;
	unsigned	commutative : 1;
	unsigned	free_dest : 1;

#ifdef JIT_REG_STACK
	unsigned	on_stack : 1;
	unsigned	x87_arith : 1;
	unsigned	reversible : 1;

	unsigned	no_pop : 1;
	unsigned	flip_args : 1;
#endif

	/* The input value index that is going to be overwritten
	   by the destination value. For ordinary binary and unary
	   opcodes it is equal to 1, for notes and three-address
	   opcodes it is equal to 0, and for some x87 instructions
	   it could be equal to 2.  */
	int		dest_input_index;

	jit_regused_t	assigned;
	jit_regused_t	clobber;

#ifdef JIT_REG_STACK
	int		wanted_stack_count;
	int		loaded_stack_count;
#endif

} _jit_regs_t;

int _jit_regs_lookup(char *name);
void _jit_regs_alloc_global(jit_gencode_t gen, jit_function_t func);
void _jit_regs_init_for_block(jit_gencode_t gen);
void _jit_regs_spill_all(jit_gencode_t gen);
void _jit_regs_set_incoming(jit_gencode_t gen, int reg, jit_value_t value);
void _jit_regs_set_outgoing(jit_gencode_t gen, int reg, jit_value_t value);
void _jit_regs_clear_all_outgoing(jit_gencode_t gen);
void _jit_regs_force_out(jit_gencode_t gen, jit_value_t value, int is_dest);
int _jit_regs_load_value(jit_gencode_t gen, jit_value_t value, int destroy, int used_again);

void _jit_regs_init(jit_gencode_t gen, _jit_regs_t *regs, int flags);

void _jit_regs_init_dest(_jit_regs_t *regs, jit_insn_t insn,
			 int flags, _jit_regclass_t *regclass);
void _jit_regs_init_value1(_jit_regs_t *regs, jit_insn_t insn,
			   int flags, _jit_regclass_t *regclass);
void _jit_regs_init_value2(_jit_regs_t *regs, jit_insn_t insn,
			   int flags, _jit_regclass_t *regclass);
void _jit_regs_add_scratch(_jit_regs_t *regs, _jit_regclass_t *regclass);

void _jit_regs_set_dest(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg);
void _jit_regs_set_value1(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg);
void _jit_regs_set_value2(jit_gencode_t gen, _jit_regs_t *regs, int reg, int other_reg);
void _jit_regs_set_scratch(jit_gencode_t gen, _jit_regs_t *regs, int index, int reg);

void _jit_regs_clobber(_jit_regs_t *regs, int reg);
void _jit_regs_clobber_class(jit_gencode_t gen, _jit_regs_t *regs, _jit_regclass_t *regclass);
void _jit_regs_clobber_all(jit_gencode_t gen, _jit_regs_t *regs);

void _jit_regs_assign(jit_gencode_t gen, _jit_regs_t *regs);
void _jit_regs_gen(jit_gencode_t gen, _jit_regs_t *regs);
#ifdef JIT_REG_STACK
int _jit_regs_select(_jit_regs_t *regs);
#endif

void _jit_regs_commit(jit_gencode_t gen, _jit_regs_t *regs);

int _jit_regs_get_dest(_jit_regs_t *regs);
int _jit_regs_get_value1(_jit_regs_t *regs);
int _jit_regs_get_value2(_jit_regs_t *regs);
int _jit_regs_get_dest_other(_jit_regs_t *regs);
int _jit_regs_get_value1_other(_jit_regs_t *regs);
int _jit_regs_get_value2_other(_jit_regs_t *regs);
int _jit_regs_get_scratch(_jit_regs_t *regs, int index);

void _jit_regs_begin(jit_gencode_t gen, _jit_regs_t *regs, int space);

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_REG_ALLOC_H */
