/*
 * jit-rules.h - Rules that define the characteristics of the back-end.
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

#ifndef	_JIT_RULES_H
#define	_JIT_RULES_H

#include "jit-config.h"

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Information about a register.
 */
typedef struct
{
	const char		*name;		/* Name of the register, for debugging */
	short			cpu_reg;	/* CPU register number */
	short			other_reg;	/* Other register for a "long" pair, or -1 */
	int			flags;		/* Flags that define the register type */

} jit_reginfo_t;

/*
 * Register information flags.
 */
#define	JIT_REG_WORD		(1 << 0)	/* Can be used for word values */
#define	JIT_REG_LONG		(1 << 1)	/* Can be used for long values */
#define	JIT_REG_FLOAT32		(1 << 2)	/* Can be used for float32 values */
#define	JIT_REG_FLOAT64		(1 << 3)	/* Can be used for float64 values */
#define	JIT_REG_NFLOAT		(1 << 4)	/* Can be used for nfloat values */
#define	JIT_REG_FRAME		(1 << 5)	/* Contains frame pointer */
#define	JIT_REG_STACK_PTR	(1 << 6)	/* Contains CPU stack pointer */
#define	JIT_REG_FIXED		(1 << 7)	/* Fixed use; not for allocation */
#define	JIT_REG_CALL_USED	(1 << 8)	/* Destroyed by a call */
#define	JIT_REG_IN_STACK	(1 << 9)	/* Middle of stack-like allocation */
#define	JIT_REG_GLOBAL		(1 << 10)	/* Candidate for global allocation */
#define	JIT_REG_ALL		(JIT_REG_WORD | JIT_REG_LONG \
				 | JIT_REG_FLOAT32 | JIT_REG_FLOAT64 \
 				 | JIT_REG_NFLOAT)

/*
 * Include definitions that are specific to the backend.
 */
#if defined(JIT_BACKEND_INTERP)
# include "jit-rules-interp.h"
#elif defined(JIT_BACKEND_ALPHA)
# include "jit-rules-alpha.h"
#elif defined(JIT_BACKEND_ARM)
# include "jit-rules-arm.h"
#elif defined(JIT_BACKEND_X86)
# include "jit-rules-x86.h"
#elif defined(JIT_BACKEND_X86_64)
# include "jit-rules-x86-64.h"
#else
# error "unknown jit backend type"
#endif

/*
 * The information blocks for all registers in the system.
 */
extern jit_reginfo_t const _jit_reg_info[JIT_NUM_REGS];

/*
 * Macros for getting register information
 */

/* Get register name. */
#define jit_reg_name(reg)		(_jit_reg_info[reg].name)

/* Get register flags. */
#define jit_reg_flags(reg)		(_jit_reg_info[reg].flags)

/* Get CPU register number for machine instruction encoding. */
#define jit_reg_code(reg)		(_jit_reg_info[reg].cpu_reg)

/* Given the first register of a register pair get the other one. */
#define jit_reg_other_reg(reg)		(_jit_reg_info[reg].other_reg)

/* Given a register find if a value of the specified type requires
 * a register pair. Return the other register of the pair if it is
 * required and return -1 otherwise. */
#if defined(JIT_NATIVE_INT32) && !defined(JIT_BACKEND_INTERP)
# define jit_reg_get_pair(type,reg)	_jit_reg_get_pair(type, reg)
#else
# define jit_reg_get_pair(type,reg)	(-1)
#endif

/*
 * Manipulate register usage masks.  The backend may override these
 * definitions if it has more registers than can fit in a "jit_uint".
 */
#if !defined(jit_regused_init)

typedef jit_uint jit_regused_t;

#define	jit_regused_init		(0)
#define	jit_regused_init_used		(~0)

#define	jit_reg_is_used(mask,reg)	(((mask) & (((jit_uint)1) << (reg))) != 0)

#define	jit_reg_set_used(mask,reg)	((mask) |= (((jit_uint)1) << (reg)))

#define	jit_reg_clear_used(mask,reg)	((mask) &= ~(((jit_uint)1) << (reg)))

#endif /* !defined(jit_regused_init) */

/*
 * Information about a register's contents.
 */
#define	JIT_MAX_REG_VALUES		8
typedef struct jit_regcontents jit_regcontents_t;
struct jit_regcontents
{
	/* List of values that are currently stored in this register */
	jit_value_t		values[JIT_MAX_REG_VALUES];
	int			num_values;

	/* Current age of this register.  Older registers are reclaimed first */
	int			age;

	/* Flag that indicates if this register is holding the first
	   word of a double-word long value (32-bit platforms only) */
	char			is_long_start;

	/* Flag that indicates if this register is holding the second
	   word of a double-word long value (32-bit platforms only) */
	char			is_long_end;

	/* Flag that indicates if the register holds a valid value,
	   but there are no actual "jit_value_t" objects associated */
	char			used_for_temp;
};

/*
 * Code generation information.
 */
typedef struct jit_gencode *jit_gencode_t;
struct jit_gencode
{
	jit_context_t		context;	/* Context this position is attached to */
	unsigned char		*ptr;		/* Current code pointer */
	unsigned char		*mem_start;	/* Available space start */
	unsigned char		*mem_limit;	/* Available space limit */
	unsigned char		*code_start;	/* Real code start */
	unsigned char		*code_end;	/* Real code end */
	jit_regused_t		permanent;	/* Permanently allocated global regs */
	jit_regused_t		touched;	/* All registers that were touched */
	jit_regused_t		inhibit;	/* Temporarily inhibited registers */
	jit_regcontents_t	contents[JIT_NUM_REGS]; /* Contents of each register */
	int			current_age;	/* Current age value for registers */
#ifdef JIT_REG_STACK
	int			reg_stack_top;	/* Current register stack top */
#endif
#ifdef jit_extra_gen_state
	jit_extra_gen_state;			/* CPU-specific extra information */
#endif
	void			*epilog_fixup;	/* Fixup list for function epilogs */
	int			stack_changed;	/* Stack top changed since entry */
	jit_varint_encoder_t	offset_encoder;	/* Bytecode offset encoder */
};

/*
 * ELF machine type and ABI information.
 */
typedef struct jit_elf_info jit_elf_info_t;
struct jit_elf_info
{
	int			machine;
	int			abi;
	int			abi_version;
};

/*
 * External function defintions.
 */

/*
 * Determine if there is sufficient space in the code cache.
 * If not throws JIT_RESULT_CACHE_FULL exception.
 */
void _jit_gen_check_space(jit_gencode_t gen, int space);

/*
 * Allocate a memory chunk for data.
 */
void *_jit_gen_alloc(jit_gencode_t gen, unsigned long size);

void _jit_init_backend(void);
void _jit_gen_get_elf_info(jit_elf_info_t *info);
int _jit_create_entry_insns(jit_function_t func);
int _jit_create_call_setup_insns
	(jit_function_t func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args,
	 int is_nested, jit_value_t parent_frame, jit_value_t *struct_return, int flags);
int _jit_setup_indirect_pointer(jit_function_t func, jit_value_t value);
int _jit_create_call_return_insns
	(jit_function_t func, jit_type_t signature,
	 jit_value_t *args, unsigned int num_args,
	 jit_value_t return_value, int is_nested);
int _jit_opcode_is_supported(int opcode);
void *_jit_gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf);
void _jit_gen_epilog(jit_gencode_t gen, jit_function_t func);
void *_jit_gen_redirector(jit_gencode_t gen, jit_function_t func);
void _jit_gen_spill_reg(jit_gencode_t gen, int reg,
					    int other_reg, jit_value_t value);
void _jit_gen_free_reg(jit_gencode_t gen, int reg,
					   int other_reg, int value_used);
void _jit_gen_load_value
	(jit_gencode_t gen, int reg, int other_reg, jit_value_t value);
void _jit_gen_spill_global(jit_gencode_t gen, int reg, jit_value_t value);
void _jit_gen_load_global(jit_gencode_t gen, int reg, jit_value_t value);
void _jit_gen_exch_top(jit_gencode_t gen, int reg);
void _jit_gen_move_top(jit_gencode_t gen, int reg);
void _jit_gen_spill_top(jit_gencode_t gen, int reg, jit_value_t value, int pop);
void _jit_gen_fix_value(jit_value_t value);
void _jit_gen_insn(jit_gencode_t gen, jit_function_t func,
				   jit_block_t block, jit_insn_t insn);
void _jit_gen_start_block(jit_gencode_t gen, jit_block_t block);
void _jit_gen_end_block(jit_gencode_t gen, jit_block_t block);
int _jit_gen_is_global_candidate(jit_type_t type);

#if defined(JIT_NATIVE_INT32) && !defined(JIT_BACKEND_INTERP)
int _jit_reg_get_pair(jit_type_t type, int reg);
#endif

/*
 * Determine the byte number within a "jit_int" where the low
 * order byte can be found.
 */
int _jit_int_lowest_byte(void);

/*
 * Determine the byte number within a "jit_int" where the low
 * order short can be found.
 */
int _jit_int_lowest_short(void);

/*
 * Determine the byte number within a "jit_nint" where the low
 * order byte can be found.
 */
int _jit_nint_lowest_byte(void);

/*
 * Determine the byte number within a "jit_nint" where the low
 * order short can be found.
 */
int _jit_nint_lowest_short(void);

/*
 * Determine the byte number within a "jit_nint" where the low
 * order int can be found.
 */
int _jit_nint_lowest_int(void);

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_RULES_H */
