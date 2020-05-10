/*
 * jit-insn.h - Functions for manipulating instructions.
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

#ifndef	_JIT_INSN_H
#define	_JIT_INSN_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Descriptor for an intrinsic function.
 */
typedef struct
{
	jit_type_t		return_type;
	jit_type_t		ptr_result_type;
	jit_type_t		arg1_type;
	jit_type_t		arg2_type;

} jit_intrinsic_descr_t;

/*
 * Structure for iterating over the instructions in a block.
 * This should be treated as opaque.
 */
typedef struct
{
	jit_block_t		block;
	int				posn;

} jit_insn_iter_t;

/*
 * Flags for "jit_insn_call" and friends.
 */
#define	JIT_CALL_NOTHROW		(1 << 0)
#define	JIT_CALL_NORETURN		(1 << 1)
#define	JIT_CALL_TAIL			(1 << 2)

int jit_insn_get_opcode(jit_insn_t insn) JIT_NOTHROW;
jit_value_t jit_insn_get_dest(jit_insn_t insn) JIT_NOTHROW;
jit_value_t jit_insn_get_value1(jit_insn_t insn) JIT_NOTHROW;
jit_value_t jit_insn_get_value2(jit_insn_t insn) JIT_NOTHROW;
jit_label_t jit_insn_get_label(jit_insn_t insn) JIT_NOTHROW;
jit_function_t jit_insn_get_function(jit_insn_t insn) JIT_NOTHROW;
void *jit_insn_get_native(jit_insn_t insn) JIT_NOTHROW;
const char *jit_insn_get_name(jit_insn_t insn) JIT_NOTHROW;
jit_type_t jit_insn_get_signature(jit_insn_t insn) JIT_NOTHROW;
int jit_insn_dest_is_value(jit_insn_t insn) JIT_NOTHROW;

int jit_insn_label(jit_function_t func, jit_label_t *label) JIT_NOTHROW;
int jit_insn_label_tight(jit_function_t func, jit_label_t *label) JIT_NOTHROW;

int jit_insn_new_block(jit_function_t func) JIT_NOTHROW;

jit_value_t jit_insn_load(jit_function_t func, jit_value_t value) JIT_NOTHROW;
jit_value_t jit_insn_dup(jit_function_t func, jit_value_t value) JIT_NOTHROW;
int jit_insn_store
	(jit_function_t func, jit_value_t dest, jit_value_t value) JIT_NOTHROW;
jit_value_t jit_insn_load_relative
	(jit_function_t func, jit_value_t value,
	 jit_nint offset, jit_type_t type) JIT_NOTHROW;
int jit_insn_store_relative
	(jit_function_t func, jit_value_t dest,
	 jit_nint offset, jit_value_t value) JIT_NOTHROW;
jit_value_t jit_insn_add_relative
	(jit_function_t func, jit_value_t value, jit_nint offset) JIT_NOTHROW;
jit_value_t jit_insn_load_elem
	(jit_function_t func, jit_value_t base_addr,
	 jit_value_t index, jit_type_t elem_type) JIT_NOTHROW;
jit_value_t jit_insn_load_elem_address
	(jit_function_t func, jit_value_t base_addr,
	 jit_value_t index, jit_type_t elem_type) JIT_NOTHROW;
int jit_insn_store_elem
	(jit_function_t func, jit_value_t base_addr,
	 jit_value_t index, jit_value_t value) JIT_NOTHROW;
int jit_insn_check_null(jit_function_t func, jit_value_t value) JIT_NOTHROW;
int jit_insn_nop(jit_function_t func) JIT_NOTHROW;

jit_value_t jit_insn_add
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_add_ovf
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_sub
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_sub_ovf
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_mul
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_mul_ovf
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_div
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_rem
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_rem_ieee
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_neg
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_and
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_or
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_xor
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_not
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_shl
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_shr
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_ushr
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_sshr
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_eq
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_ne
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_lt
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_le
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_gt
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_ge
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_cmpl
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_cmpg
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_to_bool
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_to_not_bool
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_acos
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_asin
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_atan
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_atan2
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_ceil
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_cos
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_cosh
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_exp
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_floor
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_log
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_log10
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_pow
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_rint
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_round
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_sin
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_sinh
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_sqrt
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_tan
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_tanh
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_trunc
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_is_nan
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_is_finite
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_is_inf
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_abs
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_min
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_max
	(jit_function_t func, jit_value_t value1, jit_value_t value2) JIT_NOTHROW;
jit_value_t jit_insn_sign
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
int jit_insn_branch
	(jit_function_t func, jit_label_t *label) JIT_NOTHROW;
int jit_insn_branch_if
	(jit_function_t func, jit_value_t value, jit_label_t *label) JIT_NOTHROW;
int jit_insn_branch_if_not
	(jit_function_t func, jit_value_t value, jit_label_t *label) JIT_NOTHROW;
int jit_insn_jump_table
	(jit_function_t func, jit_value_t value,
	 jit_label_t *labels, unsigned int num_labels) JIT_NOTHROW;
jit_value_t jit_insn_address_of
	(jit_function_t func, jit_value_t value1) JIT_NOTHROW;
jit_value_t jit_insn_address_of_label
	(jit_function_t func, jit_label_t *label) JIT_NOTHROW;
jit_value_t jit_insn_convert
	(jit_function_t func, jit_value_t value,
	 jit_type_t type, int overflow_check) JIT_NOTHROW;

jit_value_t jit_insn_call
	(jit_function_t func, const char *name,
	 jit_function_t jit_func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args, int flags) JIT_NOTHROW;
jit_value_t jit_insn_call_indirect
	(jit_function_t func, jit_value_t value, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args, int flags) JIT_NOTHROW;
jit_value_t jit_insn_call_nested_indirect
	(jit_function_t func, jit_value_t value, jit_value_t parent_frame,
	 jit_type_t signature, jit_value_t *args, unsigned int num_args,
	 int flags) JIT_NOTHROW;
jit_value_t jit_insn_call_indirect_vtable
	(jit_function_t func, jit_value_t value, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args, int flags) JIT_NOTHROW;
jit_value_t jit_insn_call_native
	(jit_function_t func, const char *name,
	 void *native_func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args, int flags) JIT_NOTHROW;
jit_value_t jit_insn_call_intrinsic
	(jit_function_t func, const char *name, void *intrinsic_func,
	 const jit_intrinsic_descr_t *descriptor,
	 jit_value_t arg1, jit_value_t arg2) JIT_NOTHROW;
int jit_insn_incoming_reg
	(jit_function_t func, jit_value_t value, int reg) JIT_NOTHROW;
int jit_insn_incoming_frame_posn
	(jit_function_t func, jit_value_t value, jit_nint frame_offset) JIT_NOTHROW;
int jit_insn_outgoing_reg
	(jit_function_t func, jit_value_t value, int reg) JIT_NOTHROW;
int jit_insn_outgoing_frame_posn
	(jit_function_t func, jit_value_t value, jit_nint frame_offset) JIT_NOTHROW;
int jit_insn_return_reg
	(jit_function_t func, jit_value_t value, int reg) JIT_NOTHROW;
int jit_insn_setup_for_nested
	(jit_function_t func, int nested_level, int reg) JIT_NOTHROW;
int jit_insn_flush_struct(jit_function_t func, jit_value_t value) JIT_NOTHROW;
jit_value_t jit_insn_get_frame_pointer(jit_function_t func) JIT_NOTHROW;
jit_value_t jit_insn_get_parent_frame_pointer_of
	(jit_function_t func, jit_function_t target) JIT_NOTHROW;
jit_value_t jit_insn_import
	(jit_function_t func, jit_value_t value) JIT_NOTHROW;
int jit_insn_push(jit_function_t func, jit_value_t value) JIT_NOTHROW;
int jit_insn_push_ptr
	(jit_function_t func, jit_value_t value, jit_type_t type) JIT_NOTHROW;
int jit_insn_set_param
	(jit_function_t func, jit_value_t value, jit_nint offset) JIT_NOTHROW;
int jit_insn_set_param_ptr
	(jit_function_t func, jit_value_t value, jit_type_t type,
	 jit_nint offset) JIT_NOTHROW;
int jit_insn_push_return_area_ptr(jit_function_t func) JIT_NOTHROW;
int jit_insn_pop_stack(jit_function_t func, jit_nint num_items) JIT_NOTHROW;
int jit_insn_defer_pop_stack
	(jit_function_t func, jit_nint num_items) JIT_NOTHROW;
int jit_insn_flush_defer_pop
	(jit_function_t func, jit_nint num_items) JIT_NOTHROW;
int jit_insn_return(jit_function_t func, jit_value_t value) JIT_NOTHROW;
int jit_insn_return_ptr
	(jit_function_t func, jit_value_t value, jit_type_t type) JIT_NOTHROW;
int jit_insn_default_return(jit_function_t func) JIT_NOTHROW;
int jit_insn_throw(jit_function_t func, jit_value_t value) JIT_NOTHROW;
jit_value_t jit_insn_get_call_stack(jit_function_t func) JIT_NOTHROW;

jit_value_t jit_insn_thrown_exception(jit_function_t func) JIT_NOTHROW;
int jit_insn_uses_catcher(jit_function_t func) JIT_NOTHROW;
jit_value_t jit_insn_start_catcher(jit_function_t func) JIT_NOTHROW;
int jit_insn_branch_if_pc_not_in_range
	(jit_function_t func, jit_label_t start_label,
	 jit_label_t end_label, jit_label_t *label) JIT_NOTHROW;
int jit_insn_rethrow_unhandled(jit_function_t func) JIT_NOTHROW;
int jit_insn_start_finally
	(jit_function_t func, jit_label_t *finally_label) JIT_NOTHROW;
int jit_insn_return_from_finally(jit_function_t func) JIT_NOTHROW;
int jit_insn_call_finally
	(jit_function_t func, jit_label_t *finally_label) JIT_NOTHROW;
jit_value_t jit_insn_start_filter
	(jit_function_t func, jit_label_t *label, jit_type_t type) JIT_NOTHROW;
int jit_insn_return_from_filter
	(jit_function_t func, jit_value_t value) JIT_NOTHROW;
jit_value_t jit_insn_call_filter
	(jit_function_t func, jit_label_t *label,
	 jit_value_t value, jit_type_t type) JIT_NOTHROW;

int jit_insn_memcpy
	(jit_function_t func, jit_value_t dest,
	 jit_value_t src, jit_value_t size) JIT_NOTHROW;
int jit_insn_memmove
	(jit_function_t func, jit_value_t dest,
	 jit_value_t src, jit_value_t size) JIT_NOTHROW;
int jit_insn_memset
	(jit_function_t func, jit_value_t dest,
	 jit_value_t value, jit_value_t size) JIT_NOTHROW;
jit_value_t jit_insn_alloca
	(jit_function_t func, jit_value_t size) JIT_NOTHROW;

int jit_insn_move_blocks_to_end
	(jit_function_t func, jit_label_t from_label, jit_label_t to_label)
		JIT_NOTHROW;
int jit_insn_move_blocks_to_start
	(jit_function_t func, jit_label_t from_label, jit_label_t to_label)
		JIT_NOTHROW;

int jit_insn_mark_offset
	(jit_function_t func, jit_int offset) JIT_NOTHROW;
int jit_insn_mark_breakpoint
	(jit_function_t func, jit_nint data1, jit_nint data2) JIT_NOTHROW;
int jit_insn_mark_breakpoint_variable
	(jit_function_t func, jit_value_t data1, jit_value_t data2) JIT_NOTHROW;

void jit_insn_iter_init(jit_insn_iter_t *iter, jit_block_t block) JIT_NOTHROW;
void jit_insn_iter_init_last
	(jit_insn_iter_t *iter, jit_block_t block) JIT_NOTHROW;
jit_insn_t jit_insn_iter_next(jit_insn_iter_t *iter) JIT_NOTHROW;
jit_insn_t jit_insn_iter_previous(jit_insn_iter_t *iter) JIT_NOTHROW;

#ifdef	__cplusplus
};
#endif

#endif /* _JIT_INSN_H */
