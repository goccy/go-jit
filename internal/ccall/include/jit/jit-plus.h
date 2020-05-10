/*
 * jit-plus.h - C++ binding for the JIT library.
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

#ifndef	_JIT_PLUS_H
#define	_JIT_PLUS_H

#include <jit/jit.h>

#ifdef __cplusplus

class jit_build_exception
{
public:
	jit_build_exception(int result) { this->result = result; }
	~jit_build_exception() {}

	int result;
};

class jit_value
{
public:
	jit_value() { this->value = 0; }
	jit_value(jit_value_t value) { this->value = value; }
	jit_value(const jit_value& value) { this->value = value.value; }
	~jit_value() {}

	jit_value& operator=(const jit_value& value)
		{ this->value = value.value; return *this; }

	jit_value_t raw() const { return value; }
	int is_valid() const { return (value != 0); }

	int is_temporary() const { return jit_value_is_temporary(value); }
	int is_local() const { return jit_value_is_local(value); }
	int is_constant() const { return jit_value_is_constant(value); }
	int is_parameter() const { return jit_value_is_parameter(value); }

	void set_volatile() { jit_value_set_volatile(value); }
	int is_volatile() const { return jit_value_is_volatile(value); }

	void set_addressable() { jit_value_set_addressable(value); }
	int is_addressable() const { return jit_value_is_addressable(value); }

	jit_type_t type() const { return jit_value_get_type(value); }
	jit_function_t function() const { return jit_value_get_function(value); }
	jit_block_t block() const { return jit_value_get_block(value); }
	jit_context_t context() const { return jit_value_get_context(value); }

	jit_constant_t constant() const
		{ return jit_value_get_constant(value); }
	jit_nint nint_constant() const
		{ return jit_value_get_nint_constant(value); }
	jit_long long_constant() const
		{ return jit_value_get_long_constant(value); }
	jit_float32 float32_constant() const
		{ return jit_value_get_float32_constant(value); }
	jit_float64 float64_constant() const
		{ return jit_value_get_float64_constant(value); }
	jit_nfloat nfloat_constant() const
		{ return jit_value_get_nfloat_constant(value); }

private:
	jit_value_t value;
};

jit_value operator+(const jit_value& value1, const jit_value& value2);
jit_value operator-(const jit_value& value1, const jit_value& value2);
jit_value operator*(const jit_value& value1, const jit_value& value2);
jit_value operator/(const jit_value& value1, const jit_value& value2);
jit_value operator%(const jit_value& value1, const jit_value& value2);
jit_value operator-(const jit_value& value1);
jit_value operator&(const jit_value& value1, const jit_value& value2);
jit_value operator|(const jit_value& value1, const jit_value& value2);
jit_value operator^(const jit_value& value1, const jit_value& value2);
jit_value operator~(const jit_value& value1);
jit_value operator<<(const jit_value& value1, const jit_value& value2);
jit_value operator>>(const jit_value& value1, const jit_value& value2);
jit_value operator==(const jit_value& value1, const jit_value& value2);
jit_value operator!=(const jit_value& value1, const jit_value& value2);
jit_value operator<(const jit_value& value1, const jit_value& value2);
jit_value operator<=(const jit_value& value1, const jit_value& value2);
jit_value operator>(const jit_value& value1, const jit_value& value2);
jit_value operator>=(const jit_value& value1, const jit_value& value2);

class jit_label
{
public:
	jit_label() { label = jit_label_undefined; }
	jit_label(jit_label_t label) { this->label = label; }
	jit_label(const jit_label& label) { this->label = label.label; }
	~jit_label() {}

	jit_label_t raw() const { return label; }
	jit_label_t *rawp() { return &label; }
	int is_valid() const { return (label != jit_label_undefined); }

	jit_label& operator=(const jit_label& value)
		{ this->label = value.label; return *this; }

private:
	jit_label_t label;
};

class jit_jump_table
{
 public:

	jit_jump_table(int size);
	~jit_jump_table();

	int size() { return num_labels; }
	jit_label_t *raw() { return labels; }

	jit_label get(int index);

	void set(int index, jit_label label);

 private:

	jit_label_t *labels;
	int num_labels;

	// forbid copying
	jit_jump_table(const jit_jump_table&);
	jit_jump_table& operator=(const jit_jump_table&);
};

class jit_context
{
public:
	jit_context();
	jit_context(jit_context_t context);
	~jit_context();

	void build_start() { jit_context_build_start(context); }
	void build_end() { jit_context_build_end(context); }
	jit_context_t raw() const { return context; }

private:
	jit_context_t context;
	int copied;
};

class jit_function
{
public:
	jit_function(jit_context& context, jit_type_t signature);
	jit_function(jit_context& context);
	jit_function(jit_function_t func);
	virtual ~jit_function();

	jit_function_t raw() const { return func; }
	int is_valid() const { return (func != 0); }

	static jit_function *from_raw(jit_function_t func);

	jit_type_t signature() const { return jit_function_get_signature(func); }

	void create(jit_type_t signature);
	void create();

	int compile();

	int is_compiled() const { return jit_function_is_compiled(func); }

	int is_recompilable() const { return jit_function_is_recompilable(func); }

	void set_recompilable() { jit_function_set_recompilable(func); }
	void clear_recompilable() { jit_function_clear_recompilable(func); }
	void set_recompilable(int flag)
		{ if(flag) set_recompilable(); else clear_recompilable(); }

	void set_optimization_level(unsigned int level)
		{ jit_function_set_optimization_level(func, level); }
	unsigned int optimization_level() const
		{ return jit_function_get_optimization_level(func); }
	static unsigned int max_optimization_level()
		{ return jit_function_get_max_optimization_level(); }

	void *closure() const { return jit_function_to_closure(func); }
	void *vtable_pointer() const
		{ return jit_function_to_vtable_pointer(func); }

	int apply(void **args, void *result)
		{ return jit_function_apply(func, args, result); }
	int apply(jit_type_t signature, void **args, void *return_area)
		{ return jit_function_apply_vararg
			(func, signature, args, return_area); }

	static jit_type_t const end_params;
	static jit_type_t signature_helper(jit_type_t return_type, ...);

protected:
	virtual void build();
	virtual jit_type_t create_signature();
	void fail();
	void out_of_memory();

public:
	void build_start()
		{ jit_context_build_start(jit_function_get_context(func)); }
	void build_end()
		{ jit_context_build_end(jit_function_get_context(func)); }

	jit_value new_value(jit_type_t type);
	jit_value new_constant(jit_sbyte value, jit_type_t type=0);
	jit_value new_constant(jit_ubyte value, jit_type_t type=0);
	jit_value new_constant(jit_short value, jit_type_t type=0);
	jit_value new_constant(jit_ushort value, jit_type_t type=0);
	jit_value new_constant(jit_int value, jit_type_t type=0);
	jit_value new_constant(jit_uint value, jit_type_t type=0);
	jit_value new_constant(jit_long value, jit_type_t type=0);
	jit_value new_constant(jit_ulong value, jit_type_t type=0);
	jit_value new_constant(jit_float32 value, jit_type_t type=0);
	jit_value new_constant(jit_float64 value, jit_type_t type=0);
#ifndef JIT_NFLOAT_IS_DOUBLE
	jit_value new_constant(jit_nfloat value, jit_type_t type=0);
#endif
	jit_value new_constant(void *value, jit_type_t type=0);
	jit_value new_constant(const jit_constant_t& value);
	jit_value get_param(unsigned int param);
	jit_value get_struct_pointer();

	jit_label new_label();

	void insn_label(jit_label& label);
	void insn_label_tight(jit_label& label);

	void insn_new_block();

	jit_value insn_load(const jit_value& value);
	jit_value insn_dup(const jit_value& value);
	void store(const jit_value& dest, const jit_value& value);
	jit_value insn_load_relative
		(const jit_value& value, jit_nint offset, jit_type_t type);
	void insn_store_relative
		(const jit_value& dest, jit_nint offset, const jit_value& value);
	jit_value insn_add_relative(const jit_value& value, jit_nint offset);
	jit_value insn_load_elem
		(const jit_value& base_addr, const jit_value& index,
		 jit_type_t elem_type);
	jit_value insn_load_elem_address
		(const jit_value& base_addr, const jit_value& index,
		 jit_type_t elem_type);
	void insn_store_elem
		(const jit_value& base_addr, const jit_value& index,
		 const jit_value& value);
	void insn_check_null(const jit_value& value);
	jit_value insn_add(const jit_value& value1, const jit_value& value2);
	jit_value insn_add_ovf(const jit_value& value1, const jit_value& value2);
	jit_value insn_sub(const jit_value& value1, const jit_value& value2);
	jit_value insn_sub_ovf(const jit_value& value1, const jit_value& value2);
	jit_value insn_mul(const jit_value& value1, const jit_value& value2);
	jit_value insn_mul_ovf(const jit_value& value1, const jit_value& value2);
	jit_value insn_div(const jit_value& value1, const jit_value& value2);
	jit_value insn_rem(const jit_value& value1, const jit_value& value2);
	jit_value insn_rem_ieee(const jit_value& value1, const jit_value& value2);
	jit_value insn_neg(const jit_value& value1);
	jit_value insn_and(const jit_value& value1, const jit_value& value2);
	jit_value insn_or(const jit_value& value1, const jit_value& value2);
	jit_value insn_xor(const jit_value& value1, const jit_value& value2);
	jit_value insn_not(const jit_value& value1);
	jit_value insn_shl(const jit_value& value1, const jit_value& value2);
	jit_value insn_shr(const jit_value& value1, const jit_value& value2);
	jit_value insn_ushr(const jit_value& value1, const jit_value& value2);
	jit_value insn_sshr(const jit_value& value1, const jit_value& value2);
	jit_value insn_eq(const jit_value& value1, const jit_value& value2);
	jit_value insn_ne(const jit_value& value1, const jit_value& value2);
	jit_value insn_lt(const jit_value& value1, const jit_value& value2);
	jit_value insn_le(const jit_value& value1, const jit_value& value2);
	jit_value insn_gt(const jit_value& value1, const jit_value& value2);
	jit_value insn_ge(const jit_value& value1, const jit_value& value2);
	jit_value insn_cmpl(const jit_value& value1, const jit_value& value2);
	jit_value insn_cmpg(const jit_value& value1, const jit_value& value2);
	jit_value insn_to_bool(const jit_value& value1);
	jit_value insn_to_not_bool(const jit_value& value1);
	jit_value insn_acos(const jit_value& value1);
	jit_value insn_asin(const jit_value& value1);
	jit_value insn_atan(const jit_value& value1);
	jit_value insn_atan2(const jit_value& value1, const jit_value& value2);
	jit_value insn_ceil(const jit_value& value1);
	jit_value insn_cos(const jit_value& value1);
	jit_value insn_cosh(const jit_value& value1);
	jit_value insn_exp(const jit_value& value1);
	jit_value insn_floor(const jit_value& value1);
	jit_value insn_log(const jit_value& value1);
	jit_value insn_log10(const jit_value& value1);
	jit_value insn_pow(const jit_value& value1, const jit_value& value2);
	jit_value insn_rint(const jit_value& value1);
	jit_value insn_round(const jit_value& value1);
	jit_value insn_sin(const jit_value& value1);
	jit_value insn_sinh(const jit_value& value1);
	jit_value insn_sqrt(const jit_value& value1);
	jit_value insn_tan(const jit_value& value1);
	jit_value insn_tanh(const jit_value& value1);
	jit_value insn_trunc(const jit_value& value1);
	jit_value insn_is_nan(const jit_value& value1);
	jit_value insn_is_finite(const jit_value& value1);
	jit_value insn_is_inf(const jit_value& value1);
	jit_value insn_abs(const jit_value& value1);
	jit_value insn_min(const jit_value& value1, const jit_value& value2);
	jit_value insn_max(const jit_value& value1, const jit_value& value2);
	jit_value insn_sign(const jit_value& value1);
	void insn_branch(jit_label& label);
	void insn_branch_if(const jit_value& value, jit_label& label);
	void insn_branch_if_not(const jit_value& value, jit_label& label);
	void insn_jump_table(const jit_value& value, jit_jump_table& jump_table);
	jit_value insn_address_of(const jit_value& value1);
	jit_value insn_address_of_label(jit_label& label);
	jit_value insn_convert
		(const jit_value& value, jit_type_t type, int overflow_check=0);
	jit_value insn_call
		(const char *name, jit_function_t jit_func,
		 jit_type_t signature, jit_value_t *args, unsigned int num_args,
		 int flags=0);
	jit_value insn_call_indirect
		(const jit_value& value, jit_type_t signature,
	 	 jit_value_t *args, unsigned int num_args, int flags=0);
	jit_value insn_call_indirect_vtable
		(const jit_value& value, jit_type_t signature,
	 	 jit_value_t *args, unsigned int num_args, int flags=0);
	jit_value insn_call_native
		(const char *name, void *native_func, jit_type_t signature,
	     jit_value_t *args, unsigned int num_args, int flags=0);
	jit_value insn_call_intrinsic
		(const char *name, void *intrinsic_func,
	 	 const jit_intrinsic_descr_t& descriptor,
		 const jit_value& arg1, const jit_value& arg2);
	void insn_incoming_reg(const jit_value& value, int reg);
	void insn_incoming_frame_posn(const jit_value& value, jit_nint posn);
	void insn_outgoing_reg(const jit_value& value, int reg);
	void insn_outgoing_frame_posn(const jit_value& value, jit_nint posn);
	void insn_return_reg(const jit_value& value, int reg);
	void insn_setup_for_nested(int nested_level, int reg);
	void insn_flush_struct(const jit_value& value);
	jit_value insn_import(jit_value value);
	void insn_push(const jit_value& value);
	void insn_push_ptr(const jit_value& value, jit_type_t type);
	void insn_set_param(const jit_value& value, jit_nint offset);
	void insn_set_param_ptr
		(const jit_value& value, jit_type_t type, jit_nint offset);
	void insn_push_return_area_ptr();
	void insn_return(const jit_value& value);
	void insn_return();
	void insn_return_ptr(const jit_value& value, jit_type_t type);
	void insn_default_return();
	void insn_throw(const jit_value& value);
	jit_value insn_get_call_stack();
	jit_value insn_thrown_exception();
	void insn_uses_catcher();
	jit_value insn_start_catcher();
	void insn_branch_if_pc_not_in_range
		(const jit_label& start_label, const jit_label& end_label,
		 jit_label& label);
	void insn_rethrow_unhandled();
	void insn_start_finally(jit_label& label);
	void insn_return_from_finally();
	void insn_call_finally(jit_label& label);
	jit_value insn_start_filter(jit_label& label, jit_type_t type);
	void insn_return_from_filter(const jit_value& value);
	jit_value insn_call_filter
		(jit_label& label, const jit_value& value, jit_type_t type);
	void insn_memcpy
		(const jit_value& dest, const jit_value& src, const jit_value& size);
	void insn_memmove
		(const jit_value& dest, const jit_value& src, const jit_value& size);
	void insn_memset
		(const jit_value& dest, const jit_value& value, const jit_value& size);
	jit_value insn_alloca(const jit_value& size);
	void insn_move_blocks_to_end
		(const jit_label& from_label, const jit_label& to_label);
	void insn_move_blocks_to_start
		(const jit_label& from_label, const jit_label& to_label);
	void insn_mark_offset(jit_int offset);
	void insn_mark_breakpoint(jit_nint data1, jit_nint data2);

private:
	jit_function_t func;
	jit_context_t context;

	void register_on_demand();
	static int on_demand_compiler(jit_function_t func);
	static void free_mapping(void *data);
};

#endif /* __cplusplus */

#endif /* _JIT_PLUS_H */
