/*
 * jit-dump.c - Functions for dumping JIT structures, for debugging.
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
#include "jit-rules.h"
#include <jit/jit-dump.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#if defined(JIT_BACKEND_INTERP)
# include "jit-interp.h"
#endif

/*@

@cindex jit-dump.h

The library provides some functions for dumping various objects to a
stream.  These functions are declared in the header
@code{<jit/jit-dump.h>}.

@*/

/*@
 * @deftypefun void jit_dump_type (FILE *@var{stream}, jit_type_t @var{type})
 * Dump the name of a type to a stdio stream.
 * @end deftypefun
@*/
void jit_dump_type(FILE *stream, jit_type_t type)
{
	const char *name;
	type = jit_type_remove_tags(type);
	if(!type || !stream)
	{
		return;
	}
	switch(type->kind)
	{
		case JIT_TYPE_VOID:		name = "void"; break;
		case JIT_TYPE_SBYTE:		name = "sbyte"; break;
		case JIT_TYPE_UBYTE:		name = "ubyte"; break;
		case JIT_TYPE_SHORT:		name = "short"; break;
		case JIT_TYPE_USHORT:		name = "ushort"; break;
		case JIT_TYPE_INT:		name = "int"; break;
		case JIT_TYPE_UINT:		name = "uint"; break;
		case JIT_TYPE_NINT:		name = "nint"; break;
		case JIT_TYPE_NUINT:		name = "nuint"; break;
		case JIT_TYPE_LONG:		name = "long"; break;
		case JIT_TYPE_ULONG:		name = "ulong"; break;
		case JIT_TYPE_FLOAT32:		name = "float32"; break;
		case JIT_TYPE_FLOAT64:		name = "float64"; break;
		case JIT_TYPE_NFLOAT:		name = "nfloat"; break;

		case JIT_TYPE_STRUCT:
		{
			fprintf(stream, "struct<%u>",
				(unsigned int)(jit_type_get_size(type)));
			return;
		}
		/* Not reached */

		case JIT_TYPE_UNION:
		{
			fprintf(stream, "union<%u>",
				(unsigned int)(jit_type_get_size(type)));
			return;
		}
		/* Not reached */

		case JIT_TYPE_SIGNATURE:	name = "signature"; break;
		case JIT_TYPE_PTR:		name = "ptr"; break;
		default: 			name = "<unknown-type>"; break;
	}
	fputs(name, stream);
}

/*
 * Format an integer value of arbitrary precision.
 */
static char *format_integer(char *buf, int is_neg, jit_ulong value)
{
	buf += 64;
	*(--buf) = '\0';
	if(value == 0)
	{
		*(--buf) = '0';
	}
	else
	{
		while(value != 0)
		{
			*(--buf) = '0' + (int)(value % 10);
			value /= 10;
		}
	}
	if(is_neg)
	{
		*(--buf) = '-';
	}
	return buf;
}

/*@
 * @deftypefun void jit_dump_value (FILE *@var{stream}, jit_function_t @var{func}, jit_value_t @var{value}, const char *@var{prefix})
 * Dump the name of a value to a stdio stream.  If @var{prefix} is not
 * NULL, then it indicates a type prefix to add to the value name.
 * If @var{prefix} is NULL, then this function intuits the type prefix.
 * @end deftypefun
@*/
void jit_dump_value(FILE *stream, jit_function_t func, jit_value_t value, const char *prefix)
{
	jit_pool_block_t block;
	unsigned int block_size;
	unsigned int posn;

	/* Bail out if we have insufficient informaition for the dump */
	if(!stream || !func || !(func->builder) || !value)
	{
		return;
	}

	/* Handle constants and non-local variables */
	if(value->is_constant)
	{
		jit_constant_t const_value;
		char buf[64];
		char *name;
		const_value = jit_value_get_constant(value);
		switch((jit_type_promote_int
					(jit_type_normalize(const_value.type)))->kind)
		{
			case JIT_TYPE_INT:
			{
				if(const_value.un.int_value < 0)
				{
					name = format_integer
						(buf, 1, (jit_ulong)(jit_uint)
							(-(const_value.un.int_value)));
				}
				else
				{
					name = format_integer
						(buf, 0, (jit_ulong)(jit_uint)
							(const_value.un.int_value));
				}
			}
			break;

			case JIT_TYPE_UINT:
			{
				name = format_integer
					(buf, 0, (jit_ulong)(const_value.un.uint_value));
			}
			break;

			case JIT_TYPE_LONG:
			{
				if(const_value.un.long_value < 0)
				{
					name = format_integer
						(buf, 1, (jit_ulong)(-(const_value.un.long_value)));
				}
				else
				{
					name = format_integer
						(buf, 0, (jit_ulong)(const_value.un.long_value));
				}
			}
			break;

			case JIT_TYPE_ULONG:
			{
				name = format_integer(buf, 0, const_value.un.ulong_value);
			}
			break;

			case JIT_TYPE_FLOAT32:
			{
				jit_snprintf(buf, sizeof(buf), "%f",
							 (double)(const_value.un.float32_value));
				name = buf;
			}
			break;

			case JIT_TYPE_FLOAT64:
			{
				jit_snprintf(buf, sizeof(buf), "%f",
							 (double)(const_value.un.float64_value));
				name = buf;
			}
			break;

			case JIT_TYPE_NFLOAT:
			{
				jit_snprintf(buf, sizeof(buf), "%f",
							 (double)(const_value.un.nfloat_value));
				name = buf;
			}
			break;

			default:
			{
				name = "<unknown-constant>";
			}
			break;
		}
		fputs(name, stream);
		return;
	}
	else if(value->is_local && value->block->func != func)
	{
		/* Accessing a local variable in an outer function frame */
		int scope = 0;
		while(func && func->builder && func != value->block->func)
		{
			++scope;
			func = func->nested_parent;
		}
		fprintf(stream, "{%d}", scope);
		if(!func || !(func->builder))
		{
			return;
		}
	}

	/* Intuit the prefix if one was not supplied */
	if(!prefix)
	{
		switch(jit_type_normalize(jit_value_get_type(value))->kind)
		{
			case JIT_TYPE_VOID:			prefix = "v"; break;
			case JIT_TYPE_SBYTE:		prefix = "i"; break;
			case JIT_TYPE_UBYTE:		prefix = "i"; break;
			case JIT_TYPE_SHORT:		prefix = "i"; break;
			case JIT_TYPE_USHORT:		prefix = "i"; break;
			case JIT_TYPE_INT:			prefix = "i"; break;
			case JIT_TYPE_UINT:			prefix = "i"; break;
			case JIT_TYPE_LONG:			prefix = "l"; break;
			case JIT_TYPE_ULONG:		prefix = "l"; break;
			case JIT_TYPE_FLOAT32:		prefix = "f"; break;
			case JIT_TYPE_FLOAT64:		prefix = "d"; break;
			case JIT_TYPE_NFLOAT:		prefix = "D"; break;
			case JIT_TYPE_STRUCT:		prefix = "s"; break;
			case JIT_TYPE_UNION:		prefix = "u"; break;
			default:					prefix = "?"; break;
		}
	}

	/* Get the position of the value within the function's value pool */
	block = func->builder->value_pool.blocks;
	block_size = func->builder->value_pool.elem_size *
				 func->builder->value_pool.elems_per_block;
	posn = 1;
	while(block != 0)
	{
		if(((char *)value) >= block->data &&
		   ((char *)value) < (block->data + block_size))
		{
			posn += (((char *)value) - block->data) /
					func->builder->value_pool.elem_size;
			break;
		}
		posn += func->builder->value_pool.elems_per_block;
		block = block->next;
	}

	/* Dump the prefix and the position, as the value's final name */
	fprintf(stream, "%s%u", prefix, posn);
}

/*
 * Dump a temporary value, prefixed by its type.
 */
static void dump_value(FILE *stream, jit_function_t func,
					   jit_value_t value, int type)
{
	/* Normalize the type, so that it reflects JIT_OPCODE_DEST_xxx values */
	if((type & JIT_OPCODE_SRC1_MASK) != 0)
	{
		type >>= 4;
	}
	if((type & JIT_OPCODE_SRC2_MASK) != 0)
	{
		type >>= 8;
	}

	/* Dump the value, prefixed appropriately */
	switch(type)
	{
		case JIT_OPCODE_DEST_INT:
		{
			jit_dump_value(stream, func, value, "i");
		}
		break;

		case JIT_OPCODE_DEST_LONG:
		{
			jit_dump_value(stream, func, value, "l");
		}
		break;

		case JIT_OPCODE_DEST_FLOAT32:
		{
			jit_dump_value(stream, func, value, "f");
		}
		break;

		case JIT_OPCODE_DEST_FLOAT64:
		{
			jit_dump_value(stream, func, value, "d");
		}
		break;

		case JIT_OPCODE_DEST_NFLOAT:
		{
			jit_dump_value(stream, func, value, "D");
		}
		break;

		case JIT_OPCODE_DEST_ANY:
		{
			/* Intuit the prefix from the value if the type is "any" */
			jit_dump_value(stream, func, value, 0);
		}
		break;
	}
}

/*@
 * @deftypefun void jit_dump_insn (FILE *@var{stream}, jit_function_t @var{func}, jit_insn_t @var{insn})
 * Dump the contents of an instruction to a stdio stream.
 * @end deftypefun
@*/
void jit_dump_insn(FILE *stream, jit_function_t func, jit_insn_t insn)
{
	const char *name;
	const char *infix_name;
	int opcode, flags;
	jit_nint reg;

	/* Bail out if we have insufficient information for the dump */
	if(!stream || !func || !insn)
	{
		return;
	}

	/* Get the opcode details */
	opcode = insn->opcode;
	if(opcode < JIT_OP_NOP || opcode >= JIT_OP_NUM_OPCODES)
	{
		fprintf(stream, "unknown opcode %d\n", opcode);
		return;
	}
	name = jit_opcodes[opcode].name;
	flags = jit_opcodes[opcode].flags;
	infix_name = 0;

	/* Dump branch, call, or register information */
	if((flags & JIT_OPCODE_IS_BRANCH) != 0)
	{
		if(opcode == JIT_OP_BR)
		{
			fprintf(stream, "goto .L%ld", (long)(jit_insn_get_label(insn)));
			return;
		}
		if(opcode == JIT_OP_CALL_FINALLY || opcode == JIT_OP_CALL_FILTER)
		{
			fprintf(stream, "%s .L%ld", name, (long)(jit_insn_get_label(insn)));
			return;
		}
		fprintf(stream, "if ");
	}
	else if((flags & JIT_OPCODE_IS_CALL) != 0)
	{
		if(insn->value1)
			fprintf(stream, "%s %s", name, (const char *)(insn->value1));
		else
			fprintf(stream, "%s 0x08%lx", name, (long)(jit_nuint)(insn->dest));
		return;
	}
	else if((flags & JIT_OPCODE_IS_CALL_EXTERNAL) != 0)
	{
		if(insn->value1)
			fprintf(stream, "%s %s (0x%08lx)", name,
					(const char *)(insn->value1),
					(long)(jit_nuint)(insn->dest));
		else
			fprintf(stream, "%s 0x08%lx", name,
					(long)(jit_nuint)(insn->dest));
		return;
	}
	else if((flags & JIT_OPCODE_IS_REG) != 0)
	{
		reg = jit_value_get_nint_constant(jit_insn_get_value2(insn));
		fputs(name, stream);
		putc('(', stream);
		jit_dump_value(stream, func, jit_insn_get_value1(insn), 0);
		fputs(", ", stream);
		fputs(jit_reg_name(reg), stream);
		putc(')', stream);
		return;
	}
	else if((flags & JIT_OPCODE_IS_ADDROF_LABEL) != 0)
	{
		dump_value(stream, func, jit_insn_get_dest(insn), flags & JIT_OPCODE_DEST_MASK);
		fprintf(stream, " = ");
		fprintf(stream, "address_of_label .L%ld",
				(long)(jit_insn_get_label(insn)));
		return;
	}
	else if((flags & JIT_OPCODE_IS_JUMP_TABLE) != 0)
	{
		jit_label_t *labels;
		jit_nint num_labels, label;
		labels = (jit_label_t *)jit_value_get_nint_constant(jit_insn_get_value1(insn));
		num_labels = jit_value_get_nint_constant(jit_insn_get_value2(insn));
		fprintf(stream, "%s ", name);
		dump_value(stream, func, jit_insn_get_dest(insn), flags & JIT_OPCODE_DEST_MASK);
		printf(" : {");
		for(label = 0; label < num_labels; label++)
		{
			printf(" .L%ld", (long) labels[label]);
		}
		printf(" }");
		return;
	}

	/* Output the destination information */
	if((flags & JIT_OPCODE_DEST_MASK) != JIT_OPCODE_DEST_EMPTY &&
	   !jit_insn_dest_is_value(insn))
	{
		dump_value(stream, func, jit_insn_get_dest(insn),
				   flags & JIT_OPCODE_DEST_MASK);
		fprintf(stream, " = ");
	}

	/* Dump the details of the operation */
	switch(flags & JIT_OPCODE_OPER_MASK)
	{
		case JIT_OPCODE_OPER_ADD:			infix_name = " + ";   break;
		case JIT_OPCODE_OPER_SUB:			infix_name = " - ";   break;
		case JIT_OPCODE_OPER_MUL:			infix_name = " * ";   break;
		case JIT_OPCODE_OPER_DIV:			infix_name = " / ";   break;
		case JIT_OPCODE_OPER_REM:			infix_name = " % ";   break;
		case JIT_OPCODE_OPER_NEG:			infix_name = "-";    break;
		case JIT_OPCODE_OPER_AND:			infix_name = " & ";   break;
		case JIT_OPCODE_OPER_OR:			infix_name = " | ";   break;
		case JIT_OPCODE_OPER_XOR:			infix_name = " ^ ";   break;
		case JIT_OPCODE_OPER_NOT:			infix_name = "~";    break;
		case JIT_OPCODE_OPER_EQ:			infix_name = " == ";  break;
		case JIT_OPCODE_OPER_NE:			infix_name = " != ";  break;
		case JIT_OPCODE_OPER_LT:			infix_name = " < ";   break;
		case JIT_OPCODE_OPER_LE:			infix_name = " <= ";  break;
		case JIT_OPCODE_OPER_GT:			infix_name = " > ";   break;
		case JIT_OPCODE_OPER_GE:			infix_name = " >= ";  break;
		case JIT_OPCODE_OPER_SHL:			infix_name = " << ";  break;
		case JIT_OPCODE_OPER_SHR:			infix_name = " >> ";  break;
		case JIT_OPCODE_OPER_SHR_UN:		infix_name = " >>> "; break;
		case JIT_OPCODE_OPER_COPY:			infix_name = "";      break;
		case JIT_OPCODE_OPER_ADDRESS_OF:	infix_name = "&";    break;
	}
	if(infix_name)
	{
		if((flags & JIT_OPCODE_SRC2_MASK) != 0)
		{
			/* Binary operation with a special operator name */
			dump_value(stream, func, jit_insn_get_value1(insn),
				   	   flags & JIT_OPCODE_SRC1_MASK);
			fputs(infix_name, stream);
			dump_value(stream, func, jit_insn_get_value2(insn),
				   	   flags & JIT_OPCODE_SRC2_MASK);
		}
		else
		{
			/* Unary operation with a special operator name */
			fputs(infix_name, stream);
			dump_value(stream, func, jit_insn_get_value1(insn),
				   	   flags & JIT_OPCODE_SRC1_MASK);
		}
	}
	else
	{
		/* Not a special operator, so use the opcode name */
		if(!jit_strncmp(name, "br_", 3))
		{
			name += 3;
		}
		fputs(name, stream);
		if((flags & (JIT_OPCODE_SRC1_MASK | JIT_OPCODE_SRC2_MASK)) != 0)
		{
			putc('(', stream);
			if(jit_insn_dest_is_value(insn))
			{
				dump_value(stream, func, jit_insn_get_dest(insn),
					   	   flags & JIT_OPCODE_DEST_MASK);
				fputs(", ", stream);
			}
			dump_value(stream, func, jit_insn_get_value1(insn),
				   	   flags & JIT_OPCODE_SRC1_MASK);
			if((flags & JIT_OPCODE_SRC2_MASK) != 0)
			{
				fputs(", ", stream);
				dump_value(stream, func, jit_insn_get_value2(insn),
					   	   flags & JIT_OPCODE_SRC2_MASK);
			}
			putc(')', stream);
		}
	}

	/* Dump the "then" information on a conditional branch */
	if((flags & JIT_OPCODE_IS_BRANCH) != 0)
	{
		fprintf(stream, " then goto .L%ld", (long)(jit_insn_get_label(insn)));
	}
}

#if defined(JIT_BACKEND_INTERP)

/*
 * Dump the interpreted bytecode representation of a function.
 */
static void dump_interp_code(FILE *stream, void **pc, void **end)
{
	int opcode;
	const jit_opcode_info_t *info;
	while(pc < end)
	{
		/* Fetch the next opcode */
		opcode = (int)(jit_nint)(*pc);

		/* Dump the address of the opcode */
		fprintf(stream, "\t%08lX: ", (long)(jit_nint)pc);
		++pc;

		/* Get information about this opcode */
		if(opcode < JIT_OP_NUM_OPCODES)
		{
			info = &(jit_opcodes[opcode]);
		}
		else
		{
			info = &(_jit_interp_opcodes[opcode - JIT_OP_NUM_OPCODES]);
		}

		/* Dump the name of the opcode */
		fputs(info->name, stream);

		/* Dump additional parameters from the opcode stream */
		switch(info->flags & JIT_OPCODE_INTERP_ARGS_MASK)
		{
			case JIT_OPCODE_NINT_ARG:
			{
				fprintf(stream, " %ld", (long)(jit_nint)(*pc));
				++pc;
			}
			break;

			case JIT_OPCODE_NINT_ARG_TWO:
			{
				fprintf(stream, " %ld, %ld",
						(long)(jit_nint)(pc[0]), (long)(jit_nint)(pc[1]));
				pc += 2;
			}
			break;

			case JIT_OPCODE_CONST_LONG:
			{
				jit_ulong value;
				jit_memcpy(&value, pc, sizeof(jit_ulong));
				pc += (sizeof(jit_ulong) + sizeof(void *) - 1) /
					  sizeof(void *);
				fprintf(stream, " 0x%lX%08lX",
						(long)((value >> 32) & jit_max_uint),
						(long)(value & jit_max_uint));
			}
			break;

			case JIT_OPCODE_CONST_FLOAT32:
			{
				jit_float32 value;
				jit_memcpy(&value, pc, sizeof(jit_float32));
				pc += (sizeof(jit_float32) + sizeof(void *) - 1) /
					  sizeof(void *);
				fprintf(stream, " %f", (double)value);
			}
			break;

			case JIT_OPCODE_CONST_FLOAT64:
			{
				jit_float64 value;
				jit_memcpy(&value, pc, sizeof(jit_float64));
				pc += (sizeof(jit_float64) + sizeof(void *) - 1) /
					  sizeof(void *);
				fprintf(stream, " %f", (double)value);
			}
			break;

			case JIT_OPCODE_CONST_NFLOAT:
			{
				jit_nfloat value;
				jit_memcpy(&value, pc, sizeof(jit_nfloat));
				pc += (sizeof(jit_nfloat) + sizeof(void *) - 1) /
					  sizeof(void *);
				fprintf(stream, " %f", (double)value);
			}
			break;

			case JIT_OPCODE_CALL_INDIRECT_ARGS:
			{
				fprintf(stream, " %ld", (long)(jit_nint)(pc[1]));
				pc += 2;
			}
			break;

			default:
			{
				if((info->flags & (JIT_OPCODE_IS_BRANCH |
								   JIT_OPCODE_IS_ADDROF_LABEL)) != 0)
				{
					fprintf(stream, " %08lX",
							(long)(jit_nint)((pc - 1) + (jit_nint)(*pc)));
					++pc;
				}
				else if((info->flags & JIT_OPCODE_IS_CALL) != 0)
				{
					fprintf(stream, " 0x%lX", (long)(jit_nint)(*pc));
					++pc;
				}
				else if((info->flags & JIT_OPCODE_IS_CALL_EXTERNAL) != 0)
				{
					fprintf(stream, " 0x%lX, %ld",
							(long)(jit_nint)(pc[1]), (long)(jit_nint)(pc[2]));
					pc += 3;
				}
				else if((info->flags & JIT_OPCODE_IS_JUMP_TABLE) != 0)
				{
					jit_nint label, num_labels;
					num_labels = (jit_nint)pc[0];
					for(label = 1; label <= num_labels; label++)
					{
						fprintf(stream,	" %lX",
							(long)(jit_nint)pc[label]);
					}
					pc += 1 + num_labels;
				}
			}
			break;
		}

		/* Terminate the current disassembly line */
		putc('\n', stream);
	}
}

#else /* !JIT_BACKEND_INTERP */

/*
 * Dump the native object code representation of a function to stream.
 */
static void dump_object_code(FILE *stream, void *start, void *end)
{
	char cmdline[BUFSIZ];
	unsigned char *pc = (unsigned char *)start;
	FILE *file;
	int ch;

#if JIT_WIN32_PLATFORM
	char s_path[BUFSIZ];
	char o_path[BUFSIZ];
	char *tmp_dir = getenv("TMP");
	if(tmp_dir == NULL)
	{
		tmp_dir = getenv("TEMP");
		if(tmp_dir == NULL)
		{
			tmp_dir = "c:/tmp";
		}
	}
	sprintf(s_path, "%s/libjit-dump.s", tmp_dir);
	sprintf(o_path, "%s/libjit-dump.o", tmp_dir);
#else
	const char *s_path = "/tmp/libjit-dump.s";
	const char *o_path = "/tmp/libjit-dump.o";
#endif

	file = fopen(s_path, "w");
	if(!file)
	{
		return;
	}
	fflush(stream);
	while(pc < (unsigned char *)end)
	{
		fprintf(file, ".byte %d\n", (int)(*pc));
		++pc;
	}
	fclose(file);
	sprintf(cmdline, "as %s -o %s", s_path, o_path);
	system(cmdline);
	sprintf(cmdline, "objdump --adjust-vma=%ld -d %s > %s",
			(long)(jit_nint)start, o_path, s_path);
	system(cmdline);
	file = fopen(s_path, "r");
	if(file)
	{
		while((ch = getc(file)) != EOF)
		{
			putc(ch, stream);
		}
		fclose(file);
	}
	unlink(s_path);
	unlink(o_path);
	putc('\n', stream);
	fflush(stream);
}

#endif /* !JIT_BACKEND_INTERP */

/*@
 * @deftypefun void jit_dump_function (FILE *@var{stream}, jit_function_t @var{func}, const char *@var{name})
 * Dump the three-address instructions within a function to a stream.
 * The @var{name} is attached to the output as a friendly label, but
 * has no other significance.
 *
 * If the function has not been compiled yet, then this will dump the
 * three address instructions from the build process.  Otherwise it will
 * disassemble and dump the compiled native code.
 * @end deftypefun
@*/
void jit_dump_function(FILE *stream, jit_function_t func, const char *name)
{
	jit_block_t block;
	jit_insn_iter_t iter;
	jit_insn_t insn;
	jit_type_t signature;
	unsigned int param;
	unsigned int num_params;
	jit_value_t value;
	jit_label_t label;

	/* Bail out if we don't have sufficient information to dump */
	if(!stream || !func)
	{
		return;
	}

	/* Output the function header */
	if(name)
		fprintf(stream, "function %s(", name);
	else
		fprintf(stream, "function 0x%08lX(", (long)(jit_nuint)func);
	signature = func->signature;
	num_params = jit_type_num_params(signature);
	if(func->builder)
	{
		value = jit_value_get_struct_pointer(func);
		if(value || func->nested_parent)
		{
			/* We have extra hidden parameters */
			putc('[', stream);
			if(value)
			{
				jit_dump_value(stream, func, value, 0);
				fputs(" : struct_ptr", stream);
				if(func->nested_parent)
				{
					fputs(", ", stream);
				}
			}
			if(func->nested_parent)
			{
				jit_dump_value(stream, func, func->parent_frame, 0);
				fputs(" : parent_frame", stream);
			}
			putc(']', stream);
			if(num_params > 0)
			{
				fputs(", ", stream);
			}
		}
		for(param = 0; param < num_params; ++param)
		{
			if(param != 0)
			{
				fputs(", ", stream);
			}
			value = jit_value_get_param(func, param);
			if(value)
			{
				jit_dump_value(stream, func, value, 0);
			}
			else
			{
				fputs("???", stream);
			}
			fputs(" : ", stream);
			jit_dump_type(stream, jit_type_get_param(signature, param));
		}
	}
	else
	{
		for(param = 0; param < num_params; ++param)
		{
			if(param != 0)
			{
				fputs(", ", stream);
			}
			jit_dump_type(stream, jit_type_get_param(signature, param));
		}
	}
	fprintf(stream, ") : ");
	jit_dump_type(stream, jit_type_get_return(signature));
	putc('\n', stream);

	/* Should we dump the three address code or the native code? */
	if(func->builder)
	{
		/* Output each of the three address blocks in turn */
		block = 0;
		while((block = jit_block_next(func, block)) != 0)
		{
			/* Output the block's labels, if it has any */
			label = jit_block_get_label(block);
			if(block->label != jit_label_undefined)
			{
				for(;;)
				{
					fprintf(stream, ".L%ld:", (long) label);
					label = jit_block_get_next_label(block, label);
					if(label == jit_label_undefined)
					{
						fprintf(stream, "\n");
						break;
					}
					fprintf(stream, " ");
				}
			}
			else if (block != func->builder->entry_block
				 /*&& _jit_block_get_last(block) != 0*/)
			{
				/* A new block was started, but it doesn't have a label yet */
				fprintf(stream, ".L:\n");
			}

			/* Dump the instructions in the block */
			jit_insn_iter_init(&iter, block);
			while((insn = jit_insn_iter_next(&iter)) != 0)
			{
				putc('\t', stream);
				jit_dump_insn(stream, func, insn);
				putc('\n', stream);
			}
			if(block->ends_in_dead)
			{
				fputs("\tends_in_dead\n", stream);
			}
		}
	}
	else if(func->is_compiled)
	{
		void *start = func->entry_point;
		void *info = _jit_memory_find_function_info(func->context, start);
		void *end = _jit_memory_get_function_end(func->context, info);
#if defined(JIT_BACKEND_INTERP)
		/* Dump the interpreter's bytecode representation */
		jit_function_interp_t interp;
		interp = (jit_function_interp_t)(func->entry_point);
		fprintf(stream, "\t%08lX: prolog(0x%lX, %d, %d, %d)\n",
				(long)(jit_nint)interp, (long)(jit_nint)func,
				(int)(interp->args_size), (int)(interp->frame_size),
				(int)(interp->working_area));
		dump_interp_code(stream, (void **)(interp + 1), (void **)end);
#else
		dump_object_code(stream, start, end);
#endif
	}

	/* Output the function footer */
	fprintf(stream, "end\n\n");
	fflush(stream);
}
