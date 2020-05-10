/* Automatically generated from ./jit-rules-x86-64.ins - DO NOT EDIT */
/*
 * Copyright (C) 2004, 2006-2007  Southern Storm Software, Pty Ltd.
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

#if defined(JIT_INCLUDE_RULES)

case JIT_OP_TRUNC_SBYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movsx8_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_TRUNC_UBYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movzx8_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_TRUNC_SHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movsx16_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_TRUNC_USHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movzx16_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_TRUNC_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			if(reg != reg2)
			{
				x86_64_mov_reg_reg_size(inst, reg, reg2, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_TRUNC_UINT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			if(reg != reg2)
			{
				x86_64_mov_reg_reg_size(inst, reg, reg2, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOW_WORD:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value1->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		{
			x86_64_mov_reg_imm_size(inst, reg, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_mov_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			if(reg != reg2)
			{
				x86_64_mov_reg_reg_size(inst, reg, reg2, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_EXPAND_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movsx32_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_EXPAND_UINT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_mov_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INT_TO_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_fild_membase_size(inst, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#ifdef HAVE_RED_ZONE
			x86_64_mov_membase_reg_size(inst, X86_64_RSP, -8, reg2, 4);
			x86_64_fild_membase_size(inst, X86_64_RSP, -8, 4);
	#else
			x86_64_push_reg_size(inst, reg2, 8);
			x86_64_fild_membase_size(inst, X86_64_RSP, 0, 4);
			x86_64_add_reg_imm_size(inst, X86_64_RSP, sizeof(jit_nint), 8);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LONG_TO_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_fild_membase_size(inst, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#ifdef HAVE_RED_ZONE
			x86_64_mov_membase_reg_size(inst, X86_64_RSP, -8, reg2, 8);
			x86_64_fild_membase_size(inst, X86_64_RSP, -8, 8);
	#else
			x86_64_push_reg_size(inst, reg2, 8);
			x86_64_fild_membase_size(inst, X86_64_RSP, 0, 8);
			x86_64_add_reg_imm_size(inst, X86_64_RSP, sizeof(jit_nint), 8);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT32_TO_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvttss2si_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvttss2si_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT32_TO_UINT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvttss2si_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvttss2si_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT32_TO_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvttss2si_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvttss2si_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INT_TO_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvtsi2ss_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvtsi2ss_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_UINT_TO_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_mov_reg_reg_size(inst, reg2, reg2, 4);
			x86_64_cvtsi2ss_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LONG_TO_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvtsi2ss_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvtsi2ss_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT64_TO_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvtsd2ss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvtsd2ss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT64_TO_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvttsd2si_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvttsd2si_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT64_TO_UINT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvttsd2si_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvttsd2si_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT64_TO_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvttsd2si_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvttsd2si_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INT_TO_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvtsi2sd_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvtsi2sd_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_UINT_TO_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_mov_reg_reg_size(inst, reg2, reg2, 4);
			x86_64_cvtsi2sd_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LONG_TO_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvtsi2sd_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvtsi2sd_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT32_TO_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_cvtss2sd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_cvtss2sd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFLOAT_TO_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_nfloat_to_int(inst, reg, reg3, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFLOAT_TO_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_nfloat_to_int(inst, reg, reg3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT32_TO_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_fld_membase_size(inst, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#ifdef HAVE_RED_ZONE
			x86_64_movss_membase_reg(inst, X86_64_RSP, -8, reg2);
			x86_64_fld_membase_size(inst, X86_64_RSP, -8, 4);
	#else
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_movss_regp_reg(inst, X86_64_RSP, reg2);
			x86_64_fld_regp_size(inst, X86_64_RSP, 4);
			x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLOAT64_TO_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_fld_membase_size(inst, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#ifdef HAVE_RED_ZONE
			x86_64_movsd_membase_reg(inst, X86_64_RSP, -8, reg2);
			x86_64_fld_membase_size(inst, X86_64_RSP, -8, 8);
	#else
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_movsd_regp_reg(inst, X86_64_RSP, reg2);
			x86_64_fld_regp_size(inst, X86_64_RSP, 8);
			x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFLOAT_TO_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_STACK);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_fstp_membase_size(inst, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#ifdef HAVE_RED_ZONE
			/* Avoid modifying the stack pointer by simply using negative */
			/* offsets here. */
			x86_64_fstp_membase_size(inst, X86_64_RSP, -8, 4);
			x86_64_movss_reg_membase(inst, reg, X86_64_RSP, -8);
	#else
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_fstp_regp_size(inst, X86_64_RSP, 4);
			x86_64_movss_reg_regp(inst, reg, X86_64_RSP);
			x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFLOAT_TO_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_STACK);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_fstp_membase_size(inst, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#ifdef HAVE_RED_ZONE
			/* Avoid modifying the stack pointer by simply using negative */
			/* offsets here. */
			x86_64_fstp_membase_size(inst, X86_64_RSP, -8, 8);
			x86_64_movsd_reg_membase(inst, reg, X86_64_RSP, -8);
	#else
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_fstp_regp_size(inst, X86_64_RSP, 8);
			x86_64_movsd_reg_regp(inst, reg, X86_64_RSP);
			x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_LOAD_SBYTE:
case JIT_OP_COPY_LOAD_UBYTE:
case JIT_OP_COPY_STORE_BYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0 && insn->value1->is_constant)
	{
		_jit_gen_fix_value(insn->dest);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		imm_value = insn->value1->address;
		{
			x86_64_mov_membase_imm_size(inst, X86_64_RBP, local_offset, imm_value, 1);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COPY);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_mov_membase_reg_size(inst, X86_64_RBP, local_offset, reg, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_LOAD_SHORT:
case JIT_OP_COPY_LOAD_USHORT:
case JIT_OP_COPY_STORE_SHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0 && insn->value1->is_constant)
	{
		_jit_gen_fix_value(insn->dest);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		imm_value = insn->value1->address;
		{
			x86_64_mov_membase_imm_size(inst, X86_64_RBP, local_offset, imm_value, 2);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COPY);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_mov_membase_reg_size(inst, X86_64_RBP, local_offset, reg, 2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0 && insn->value1->is_constant)
	{
		_jit_gen_fix_value(insn->dest);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		imm_value = insn->value1->address;
		{
			x86_64_mov_membase_imm_size(inst, X86_64_RBP, local_offset, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0 && insn->value1->is_nint_constant && insn->value1->address >= -2147483648 && insn->value1->address <= 2147483647)
	{
		_jit_gen_fix_value(insn->dest);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		imm_value = insn->value1->address;
		{
			x86_64_mov_membase_imm_size(inst, X86_64_RBP, local_offset, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COPY);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movss_membase_reg(inst, X86_64_RBP, local_offset, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->in_register && !insn->dest->has_global_register&& (insn->flags & JIT_INSN_DEST_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COPY);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_movsd_membase_reg(inst, X86_64_RBP, local_offset, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COPY | _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_COPY_STRUCT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset, local_offset2;
	if(!insn->dest->is_constant && !insn->dest->has_global_register && !insn->value1->is_constant && !insn->value1->has_global_register && (jit_type_get_size(jit_value_get_type(insn->dest)) <= _JIT_MAX_MEMCPY_INLINE))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_force_out(gen, insn->dest, 1);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_force_out(gen, insn->value1, 0);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		local_offset2 = insn->value1->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			inst = small_struct_copy(gen, inst, X86_64_RBP, local_offset, X86_64_RBP, local_offset2,
									 jit_value_get_type(insn->dest), reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_force_out(gen, insn->dest, 1);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_force_out(gen, insn->value1, 0);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		local_offset2 = insn->value1->frame_offset;
		{
			inst = memory_copy(gen, inst, X86_64_RBP, local_offset, X86_64_RBP, local_offset2,
					   jit_type_get_size(jit_value_get_type(insn->dest)));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ADDRESS_OF:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint local_offset;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_force_out(gen, insn->value1, 0);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_lea_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INCOMING_REG:
case JIT_OP_RETURN_REG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/*
			 * This rule does nothing itself. Also at this point
			 * the value is supposed to be already in the register
			 * so the "reg" pattern does not load it either. But
			 * it allows the allocator to check the liveness flags
			 * and free the register if the value is dead.
			 */
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETRIEVE_FRAME_POINTER:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		{
			x86_64_mov_reg_reg_size(inst, reg, X86_64_RBP, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_PUSH_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value1->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		{
			x86_64_push_imm(inst, imm_value);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_gen_fix_value(insn->value1);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->value1->frame_offset;
		{
			x86_64_push_membase_size(inst, X86_64_RBP, local_offset, 4);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_push_reg_size(inst, reg, 4);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_PUSH_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value1->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		{
			if((imm_value >= (jit_nint)jit_min_int) && (imm_value <= (jit_nint)jit_max_int))
			{
				x86_64_push_imm(inst, imm_value);
			}
			else
			{
				jit_int *ptr = (jit_int *)&(imm_value);
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
				x86_64_mov_membase_imm_size(inst, X86_64_RSP, 4, ptr[1], 4);
				x86_64_mov_membase_imm_size(inst, X86_64_RSP, 0, ptr[0], 4);
			}
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_gen_fix_value(insn->value1);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->value1->frame_offset;
		{
			x86_64_push_membase_size(inst, X86_64_RBP, local_offset, 8);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_push_reg_size(inst, reg, 8);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_PUSH_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value1->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		{
			jit_int *ptr = (jit_int *)(imm_value);
			x86_64_push_imm_size(inst, ptr[0], 4);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_gen_fix_value(insn->value1);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->value1->frame_offset;
		{
			x86_64_push_membase_size(inst, X86_64_RBP, local_offset, 4);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_movss_membase_reg(inst, X86_64_RSP, 0, reg);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_PUSH_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value1->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		{
			jit_int *ptr = (jit_int *)(imm_value);
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, 4, ptr[1], 4);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, 0, ptr[0], 4);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_gen_fix_value(insn->value1);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->value1->frame_offset;
		{
			x86_64_push_membase_size(inst, X86_64_RBP, local_offset, 8);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_movsd_membase_reg(inst, X86_64_RSP, 0, reg);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_PUSH_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value1->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		{
			jit_int *ptr = (jit_int *)(imm_value);
			if(sizeof(jit_nfloat) != sizeof(jit_float64))
			{
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
				x86_64_mov_membase_imm_size(inst, X86_64_RSP, 8, ptr[2], 4);
			}
			else
			{
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, sizeof(jit_float64), 8);
			}
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, 4, ptr[1], 4);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, 0, ptr[0], 4);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_STACK);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->value1->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			if(sizeof(jit_nfloat) != sizeof(jit_float64))
			{
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
				x86_64_mov_reg_membase_size(inst, reg, X86_64_RBP, local_offset + 8, 4);
				x86_64_mov_membase_reg_size(inst, X86_64_RSP, 8, reg, 4);
			}
			else
			{
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
			}
			x86_64_mov_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			x86_64_mov_membase_reg_size(inst, X86_64_RSP, 0, reg, 8);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_STACK);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			if(sizeof(jit_nfloat) != sizeof(jit_float64))
			{
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, 16, 8);
				x86_64_fstp_membase_size(inst, X86_64_RSP, 0, 10);
			}
			else
			{
				x86_64_sub_reg_imm_size(inst, X86_64_RSP, sizeof(jit_float64), 8);
				x86_64_fstp_membase_size(inst, X86_64_RSP, 0, 8);
			}
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_PUSH_STRUCT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	if((((jit_nuint)jit_value_get_nint_constant(insn->value2)) <= 32))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			jit_nuint size;
			jit_nuint last_part;
			size = (jit_nuint)jit_value_get_nint_constant(insn->value2);
			last_part = size & 0x7;
			if(last_part)
			{
				/* Handle the possible last part smaller than 8 bytes */
				size -= last_part;
	
				/* We don't care about the last not needed bytes */
				x86_64_push_membase_size(inst, reg, size, 8);
			}
			/* Handle full multiple pointer sized parts */
			while(size > 0)
			{
				size -= sizeof(void *);
				x86_64_push_membase_size(inst, reg, size, 8);
			}
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/* Handle arbitrary-sized structures */
			jit_nuint size;
			size = (jit_nuint)jit_value_get_nint_constant(insn->value2);
			/* TODO: Maybe we should check for sizes > 2GB? */
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, ROUND_STACK(size), 8);
			inst = memory_copy(gen, inst, X86_64_RSP, 0, reg, 0, size);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_POP_STACK:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			x86_64_add_reg_imm_size(inst, X86_64_RSP, insn->value1->address, 8);
			gen->stack_changed = 1;
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_SET_PARAM_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_mov_membase_reg_size(inst, X86_64_RSP, imm_value, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_SET_PARAM_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_nint_constant && insn->value1->address >= -2147483648 && insn->value1->address <= 2147483647 && insn->value2->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			jit_int *ptr = (jit_int *)&(imm_value);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2 + 4, ptr[1], 4);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2, ptr[0], 4);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_mov_membase_reg_size(inst, X86_64_RSP, imm_value, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_SET_PARAM_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			jit_int *ptr = (jit_int *)(imm_value);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2, ptr[0], 4);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_movss_membase_reg(inst, X86_64_RSP, imm_value, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_SET_PARAM_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			jit_int *ptr = (jit_int *)(imm_value);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2 + 4, ptr[1], 4);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2, ptr[0], 4);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_movsd_membase_reg(inst, X86_64_RSP, imm_value, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_SET_PARAM_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			jit_int *ptr = (jit_int *)(imm_value);
			if(sizeof(jit_nfloat) != sizeof(jit_float64))
			{
				x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2 + 8, ptr[2], 4);
			}
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2 + 4, ptr[1], 4);
			x86_64_mov_membase_imm_size(inst, X86_64_RSP, imm_value2, ptr[0], 4);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(sizeof(jit_nfloat) != sizeof(jit_float64))
			{
				x86_64_fstp_membase_size(inst, X86_64_RSP, imm_value, 10);
			}
			else
			{
				x86_64_fstp_membase_size(inst, X86_64_RSP, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_SET_PARAM_STRUCT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* Handle arbitrary-sized structures */
			jit_nint offset = jit_value_get_nint_constant(insn->dest);
			/* TODO: Maybe we should check for sizes > 2GB? */
			inst = memory_copy(gen, inst, X86_64_RSP, offset, reg, 0, imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLUSH_SMALL_STRUCT:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = flush_return_struct(inst, insn->value1);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_RETURN:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_RETURN_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETURN_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETURN_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("xmm0"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETURN_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("xmm0"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETURN_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_STACK);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_clobber_class(gen, &regs, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/* clobber(freg) frees all registers on the fp stack */
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETURN_SMALL_STRUCT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_rreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = return_struct(inst, func, reg);
			inst = jump_to_epilog(gen, inst, block);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_SBYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movsx8_reg_regp_size(inst, reg, reg2, 8);
			}
			else
			{
				x86_64_movsx8_reg_membase_size(inst, reg, reg2, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_UBYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movzx8_reg_regp_size(inst, reg, reg2, 8);
			}
			else
			{
				x86_64_movzx8_reg_membase_size(inst, reg, reg2, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_SHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movsx16_reg_regp_size(inst, reg, reg2, 8);
			}
			else
			{
				x86_64_movsx16_reg_membase_size(inst, reg, reg2, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_USHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movzx16_reg_regp_size(inst, reg, reg2, 8);
			}
			else
			{
				x86_64_movzx16_reg_membase_size(inst, reg, reg2, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_mov_reg_regp_size(inst, reg, reg2, 4);
			}
			else
			{
				x86_64_mov_reg_membase_size(inst, reg, reg2, imm_value, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_mov_reg_regp_size(inst, reg, reg2, 8);
			}
			else
			{
				x86_64_mov_reg_membase_size(inst, reg, reg2, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movss_reg_regp(inst, reg, reg2);
			}
			else
			{
				x86_64_movss_reg_membase(inst, reg, reg2, imm_value);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movsd_reg_regp(inst, reg, reg2);
			}
			else
			{
				x86_64_movsd_reg_membase(inst, reg, reg2, imm_value);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_NFLOAT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant && (sizeof(jit_nfloat) != sizeof(jit_float64)))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_fld_membase_size(inst, reg2, imm_value, 10);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_fld_membase_size(inst, reg2, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_RELATIVE_STRUCT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(!insn->dest->is_constant && !insn->dest->has_global_register && insn->value2->is_constant && (jit_type_get_size(jit_value_get_type(insn->dest)) <= _JIT_MAX_MEMCPY_INLINE))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_force_out(gen, insn->dest, 1);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			inst = small_struct_copy(gen, inst, X86_64_RBP, local_offset, reg, imm_value,
									 jit_value_get_type(insn->dest), reg2, reg3);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_force_out(gen, insn->dest, 1);
		_jit_gen_fix_value(insn->dest);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		local_offset = insn->dest->frame_offset;
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = memory_copy(gen, inst, X86_64_RBP, local_offset, reg, imm_value,
					   jit_type_get_size(jit_value_get_type(insn->dest)));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_BYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			if(imm_value2 == 0)
			{
				x86_64_mov_regp_imm_size(inst, reg, imm_value, 1);
			}
			else
			{
				x86_64_mov_membase_imm_size(inst, reg, imm_value2, imm_value, 1);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_mov_regp_reg_size(inst, reg, reg2, 1);
			}
			else
			{
				x86_64_mov_membase_reg_size(inst, reg, imm_value, reg2, 1);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_SHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			if(imm_value2 == 0)
			{
				x86_64_mov_regp_imm_size(inst, reg, imm_value, 2);
			}
			else
			{
				x86_64_mov_membase_imm_size(inst, reg, imm_value2, imm_value, 2);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_mov_regp_reg_size(inst, reg, reg2, 2);
			}
			else
			{
				x86_64_mov_membase_reg_size(inst, reg, imm_value, reg2, 2);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			if(imm_value2 == 0)
			{
				x86_64_mov_regp_imm_size(inst, reg, imm_value, 4);
			}
			else
			{
				x86_64_mov_membase_imm_size(inst, reg, imm_value2, imm_value, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_mov_regp_reg_size(inst, reg, reg2, 4);
			}
			else
			{
				x86_64_mov_membase_reg_size(inst, reg, imm_value, reg2, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_nint_constant && insn->value1->address >= -2147483648 && insn->value1->address <= 2147483647 && insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			if(imm_value2 == 0)
			{
				x86_64_mov_regp_imm_size(inst, reg, imm_value, 8);
			}
			else
			{
				x86_64_mov_membase_imm_size(inst, reg, imm_value2, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_mov_regp_reg_size(inst, reg, reg2, 8);
			}
			else
			{
				x86_64_mov_membase_reg_size(inst, reg, imm_value, reg2, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			if(imm_value2 == 0)
			{
				x86_64_mov_regp_imm_size(inst, reg, ((jit_int *)(imm_value))[0], 4);
			}
			else
			{
				x86_64_mov_membase_imm_size(inst, reg, imm_value2, ((jit_int *)(imm_value))[0], 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movss_regp_reg(inst, reg, reg2);
			}
			else
			{
				x86_64_movss_membase_reg(inst, reg, imm_value, reg2);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	if(insn->value1->is_constant && insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		{
			x86_64_mov_membase_imm_size(inst, reg, imm_value2, ((int *)(imm_value))[0], 4);
			x86_64_mov_membase_imm_size(inst, reg, imm_value2 + 4, ((int *)(imm_value))[1], 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_movsd_regp_reg(inst, reg, reg2);
			}
			else
			{
				x86_64_movsd_membase_reg(inst, reg, imm_value, reg2);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_RELATIVE_STRUCT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->has_global_register && insn->value2->is_constant && (jit_type_get_size(jit_value_get_type(insn->value1)) <= _JIT_MAX_MEMCPY_INLINE))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_force_out(gen, insn->value1, 0);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			inst = small_struct_copy(gen, inst, reg, imm_value, X86_64_RBP, local_offset,
									 jit_value_get_type(insn->value1), reg2, reg3);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_force_out(gen, insn->value1, 0);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		imm_value = insn->value2->address;
		{
			inst = memory_copy(gen, inst, reg, imm_value, X86_64_RBP, local_offset,
					   jit_type_get_size(jit_value_get_type(insn->value1)));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ADD_RELATIVE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_lea_membase_size(inst, reg, reg2, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_SBYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movsx8_reg_memindex_size(inst, reg, reg2, 0, reg3, 0, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_UBYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movzx8_reg_memindex_size(inst, reg, reg2, 0, reg3, 0, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_SHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movsx16_reg_memindex_size(inst, reg, reg2, 0, reg3, 1, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_USHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movzx16_reg_memindex_size(inst, reg, reg2, 0, reg3, 1, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mov_reg_memindex_size(inst, reg, reg2, 0, reg3, 2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mov_reg_memindex_size(inst, reg, reg2, 0, reg3, 3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movss_reg_memindex(inst, reg, reg2, 0, reg3, 2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_ELEMENT_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movsd_reg_memindex(inst, reg, reg2, 0, reg3, 3);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_ELEMENT_BYTE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mov_memindex_reg_size(inst, reg, 0, reg2, 0, reg3, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_ELEMENT_SHORT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mov_memindex_reg_size(inst, reg, 0, reg2, 1, reg3, 2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_ELEMENT_INT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mov_memindex_reg_size(inst, reg, 0, reg2, 2, reg3, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_ELEMENT_LONG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value >= (jit_nint)jit_min_int && imm_value <= (jit_nint)jit_max_int)
			{
				x86_64_mov_memindex_imm_size(inst, reg, 0, reg2, 3, imm_value, 8);
			}
			else
			{
				jit_int *long_ptr = (jit_int *)(&(imm_value));
	
				x86_64_mov_memindex_imm_size(inst, reg, 0, reg2, 3, long_ptr[0], 4);
				x86_64_mov_memindex_imm_size(inst, reg, 4, reg2, 3, long_ptr[1], 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mov_memindex_reg_size(inst, reg, 0, reg2, 3, reg3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_ELEMENT_FLOAT32:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movss_memindex_reg(inst, reg, 0, reg2, 2, reg3);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_STORE_ELEMENT_FLOAT64:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_movsd_memindex_reg(inst, reg, 0, reg2, 3, reg3);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IADD:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(reg != reg2)
			{
				x86_64_lea_membase_size(inst, reg, reg2, imm_value, 4);
			}
			else if(imm_value == 1)
			{
				x86_64_inc_reg_size(inst, reg, 4);
			}
			else if(imm_value == -1)
			{
				x86_64_dec_reg_size(inst, reg, 4);
			}
			else
			{
				x86_64_add_reg_imm_size(inst, reg, imm_value, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_add_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_add_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ISUB:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(reg != reg2)
			{
				x86_64_lea_membase_size(inst, reg, reg2, -imm_value, 4);
			}
			else if(imm_value == 1)
			{
				x86_64_dec_reg_size(inst, reg, 4);
			}
			else if(imm_value == -1)
			{
				x86_64_inc_reg_size(inst, reg, 4);
			}
			else
			{
				x86_64_sub_reg_imm_size(inst, reg, imm_value, 4);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_sub_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_sub_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INEG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_neg_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IMUL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == -1))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_neg_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == 2))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_add_reg_reg_size(inst, reg, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			jit_nuint shift, value = imm_value >> 1;
			for(shift = 0; value; value >>= 1)
			{
			    ++shift;
			}
			x86_64_shl_reg_imm_size(inst, reg, shift, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_imul_reg_reg_imm_size(inst, reg, reg, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_imul_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_imul_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IDIV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == -1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* Dividing by -1 gives an exception if the argument
			   is minint, or simply negates for other values */
			jit_int min_int = jit_min_int;
			unsigned char *patch;
			x86_64_cmp_reg_imm_size(inst, reg, min_int, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_64_neg_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == 2))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			/* move the value to be divided to the temporary */
			x86_64_mov_reg_reg_size(inst, reg2, reg, 4);
			/* shift the temporary to the 31 bits right */
			/* The result is 1 for negative values and 0 for zero or */
			/* positive values. (corrective value for negatives) */
			x86_64_shr_reg_imm_size(inst, reg2, 0x1f, 4);
			/* Add the corrective value to the divident */
			x86_64_add_reg_reg_size(inst, reg, reg2, 4);
			/* and do the right shift */
			x86_64_sar_reg_imm_size(inst, reg, 1, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((insn->value2->address > 0) && (((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			jit_nuint shift, corr, value = imm_value >> 1;
			for(shift = 0; value; value >>= 1)
			{
			    ++shift;
			}
			corr = imm_value - 1;
			x86_64_lea_membase_size(inst, reg2, reg, corr, 4);
			x86_64_test_reg_reg_size(inst, reg, reg, 4);
			x86_64_cmov_reg_reg_size(inst, X86_CC_S, reg, reg2, 1, 4);
			x86_64_sar_reg_imm_size(inst, reg, shift, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg2, imm_value, 4);
			x86_64_cdq(inst);
			x86_64_idiv_reg_size(inst, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			jit_int min_int = jit_min_int;
			unsigned char *patch, *patch2;
	#ifndef JIT_USE_SIGNALS
			x86_64_test_reg_reg_size(inst, reg2, reg2, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_cmp_reg_imm_size(inst, reg2, -1, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			x86_64_cmp_reg_imm_size(inst, reg, min_int, 4);
			patch2 = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_patch(patch2, inst);
			x86_64_cdq(inst);
			x86_64_idiv_reg_size(inst, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IDIV_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			jit_nuint shift, value = imm_value >> 1;
			for(shift = 0; value; value >>= 1)
			{
			    ++shift;
			}
			x86_64_shr_reg_imm_size(inst, reg, shift, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg2, imm_value, 4);
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
	#ifndef JIT_USE_SIGNALS
			unsigned char *patch;
			x86_64_test_reg_reg_size(inst, reg2, reg2, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IREM:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == -1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* Dividing by -1 gives an exception if the argument
			   is minint, or simply gives a remainder of zero */
			jit_int min_int = jit_min_int;
			unsigned char *patch;
			x86_64_cmp_reg_imm_size(inst, reg, min_int, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg3, imm_value, 4);
			x86_64_cdq(inst);
			x86_64_idiv_reg_size(inst, reg3, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			jit_int min_int = jit_min_int;
			unsigned char *patch, *patch2;
	#ifndef JIT_USE_SIGNALS
			x86_64_test_reg_reg_size(inst, reg3, reg3, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_cmp_reg_imm_size(inst, reg3, -1, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			x86_64_cmp_reg_imm_size(inst, reg2, min_int, 4);
			patch2 = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_patch(patch2, inst);
			x86_64_cdq(inst);
			x86_64_idiv_reg_size(inst, reg3, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IREM_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((insn->value2->address & (insn->value2->address - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			x86_64_and_reg_imm_size(inst, reg, imm_value - 1, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg3, imm_value, 4);
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg3, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
	#ifndef JIT_USE_SIGNALS
			unsigned char *patch;
			x86_64_test_reg_reg_size(inst, reg3, reg3, 4);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg3, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LADD:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(reg != reg2)
			{
				x86_64_lea_membase_size(inst, reg, reg2, imm_value, 8);
			}
			else if(imm_value == 1)
			{
				x86_64_inc_reg_size(inst, reg, 8);
			}
			else if(imm_value == -1)
			{
				x86_64_dec_reg_size(inst, reg, 8);
			}
			else
			{
				x86_64_add_reg_imm_size(inst, reg, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_add_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_add_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LSUB:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(reg != reg2)
			{
				x86_64_lea_membase_size(inst, reg, reg2, -imm_value, 8);
			}
			else if(imm_value == 1)
			{
				x86_64_dec_reg_size(inst, reg, 8);
			}
			else if(imm_value == -1)
			{
				x86_64_inc_reg_size(inst, reg, 8);
			}
			else
			{
				x86_64_sub_reg_imm_size(inst, reg, imm_value, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_sub_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_sub_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LNEG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_neg_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LMUL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == -1))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_neg_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == 2))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_add_reg_reg_size(inst, reg, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			jit_nuint shift, value = imm_value >> 1;
			for(shift = 0; value; value >>= 1)
			{
			    ++shift;
			}
			x86_64_shl_reg_imm_size(inst, reg, shift, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_imul_reg_reg_imm_size(inst, reg, reg, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_imul_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_imul_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LDIV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == -1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			/* Dividing by -1 gives an exception if the argument
			   is minint, or simply negates for other values */
			jit_long min_long = jit_min_long;
			unsigned char *patch;
			x86_64_mov_reg_imm_size(inst, reg2, min_long, 8);
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_64_neg_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == 2))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			/* move the value to be divided to the temporary */
			x86_64_mov_reg_reg_size(inst, reg2, reg, 8);
			/* shift the temporary to the 63 bits right */
			/* The result is 1 for negative values and 0 for zero or */
			/* positive values. (corrective value for negatives) */
			x86_64_shr_reg_imm_size(inst, reg2, 0x3f, 8);
			/* Add the corrective value to the divident */
			x86_64_add_reg_reg_size(inst, reg, reg2, 8);
			/* and do the right shift */
			x86_64_sar_reg_imm_size(inst, reg, 1, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((insn->value2->address > 0) && (((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			jit_nuint shift, value = imm_value >> 1;
			for(shift = 0; value; value >>= 1)
			{
			    ++shift;
			}
			if((jit_nuint)imm_value <= (jit_nuint)jit_max_uint)
			{
				jit_nuint corr = (imm_value - 1);
	
				x86_64_lea_membase_size(inst, reg2, reg, corr, 8);
				x86_64_test_reg_reg_size(inst, reg, reg, 8);
			}
			else
			{
				jit_nuint corr = (imm_value - 1);
	
				if(corr <= (jit_nuint)jit_max_uint)
				{
					x86_64_mov_reg_imm_size(inst, reg2, corr, 4);
				}
				else
				{
					x86_64_mov_reg_imm_size(inst, reg2, corr, 8);
				}
				x86_64_test_reg_reg_size(inst, reg, reg, 8);
				x86_64_lea_memindex_size(inst, reg2, reg, 0, reg2, 0, 8);
			}
			x86_64_cmov_reg_reg_size(inst, X86_CC_S, reg, reg2, 1, 8);
			x86_64_sar_reg_imm_size(inst, reg, shift, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg2, imm_value, 8);
			x86_64_cqo(inst);
			x86_64_idiv_reg_size(inst, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			jit_long min_long = jit_min_long;
			unsigned char *patch, *patch2;
	#ifndef JIT_USE_SIGNALS
			x86_64_or_reg_reg_size(inst, reg2, reg2, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_cmp_reg_imm_size(inst, reg2, -1, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			x86_64_mov_reg_imm_size(inst, reg3, min_long, 8);
			x86_64_cmp_reg_reg_size(inst, reg, reg3, 8);
			patch2 = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_patch(patch2, inst);
			x86_64_cqo(inst);
			x86_64_idiv_reg_size(inst, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LDIV_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			jit_nuint shift, value = imm_value >> 1;
			for(shift = 0; value; value >>= 1)
			{
			    ++shift;
			}
			x86_64_shr_reg_imm_size(inst, reg, shift, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg2, imm_value, 8);
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
	#ifndef JIT_USE_SIGNALS
			unsigned char *patch;
			x86_64_test_reg_reg_size(inst, reg2, reg2, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LREM:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && (insn->value2->address == -1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			/* Dividing by -1 gives an exception if the argument
			   is minint, or simply gives a remainder of zero */
			jit_long min_long = jit_min_long;
			unsigned char *patch;
			x86_64_cmp_reg_imm_size(inst, reg, min_long, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg3, imm_value, 8);
			x86_64_cqo(inst);
			x86_64_idiv_reg_size(inst, reg3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			jit_long min_long = jit_min_long;
			unsigned char *patch, *patch2;
	#ifndef JIT_USE_SIGNALS
			x86_64_test_reg_reg_size(inst, reg3, reg3, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_mov_reg_imm_size(inst, reg, min_long, 8);
			x86_64_cmp_reg_imm_size(inst, reg3, -1, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			x86_64_cmp_reg_reg_size(inst, reg2, reg, 8);
			patch2 = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
			x86_patch(patch, inst);
			x86_patch(patch2, inst);
			x86_64_cqo(inst);
			x86_64_idiv_reg_size(inst, reg3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LREM_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_gen_check_space(gen, 128);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
		}
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address == 1))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_clear_reg(inst, reg);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant && ((((jit_nuint)insn->value2->address) & (((jit_nuint)insn->value2->address) - 1)) == 0))
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			/* x & (x - 1) is equal to zero if x is a power of 2  */
			if((imm_value >= jit_min_int) && (imm_value <= jit_max_int))
			{
				x86_64_and_reg_imm_size(inst, reg, imm_value - 1, 8);
			}
			else
			{
				jit_long temp = imm_value - 1;
	
				x86_64_mov_reg_imm_size(inst, reg2, temp, 8);
				x86_64_and_reg_reg_size(inst, reg, reg2, 8);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_add_scratch(&regs, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 1, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			x86_64_mov_reg_imm_size(inst, reg3, imm_value, 8);
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_CLOBBER, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rax"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_dreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_set_scratch(gen, &regs, 0, _jit_regs_lookup("rdx"));
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
	#ifndef JIT_USE_SIGNALS
			unsigned char *patch;
			x86_64_test_reg_reg_size(inst, reg3, reg3, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
			x86_patch(patch, inst);
	#endif
			x86_64_clear_reg(inst, X86_64_RDX);
			x86_64_div_reg_size(inst, reg3, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FADD:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float32(gen, &inst, XMM1_ADD, reg, (jit_float32 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_addss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_addss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FSUB:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float32(gen, &inst, XMM1_SUB, reg, (jit_float32 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(1)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_subss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_subss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FMUL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float32(gen, &inst, XMM1_MUL, reg, (jit_float32 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(1)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mulss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_mulss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FDIV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float32(gen, &inst, XMM1_DIV, reg, (jit_float32 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(1)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_divss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_divss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FABS:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/* Simply clear the sign */
			jit_uint values[4] = {0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff};
	
			_jit_plops_reg_imm(gen, &inst, XMM_ANDP, reg, &(values[0]));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FNEG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/* Simply toggle the sign */
			jit_uint values[4] = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
	
			_jit_plops_reg_imm(gen, &inst, XMM_XORP, reg, &(values[0]));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DADD:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float64(gen, &inst, XMM1_ADD, reg, (jit_float64 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_addsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_addsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DSUB:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float64(gen, &inst, XMM1_SUB, reg, (jit_float64 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_subsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_subsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DMUL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float64(gen, &inst, XMM1_MUL, reg, (jit_float64 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_mulsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_mulsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DDIV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			_jit_xmm1_reg_imm_size_float64(gen, &inst, XMM1_DIV, reg, (jit_float64 *)imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_divsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_divsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DABS:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/* Simply clear the sign */
			jit_ulong values[2] = {0x7fffffffffffffff, 0x7fffffffffffffff};
	
			_jit_plopd_reg_imm(gen, &inst, XMM_ANDP, reg, &(values[0]));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DNEG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			/* Simply toggle the sign */
			jit_ulong values[2] = {0x8000000000000000, 0x8000000000000000};
	
			_jit_plopd_reg_imm(gen, &inst, XMM_XORP, reg, &(values[0]));
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFABS:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_fabs(inst);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFNEG:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_STACK);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_fchs(inst);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IAND:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_and_reg_imm_size(inst, reg, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_and_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_and_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_or_reg_imm_size(inst, reg, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_or_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_or_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IXOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_xor_reg_imm_size(inst, reg, imm_value, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_xor_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_xor_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INOT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_not_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ISHL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_shl_reg_imm_size(inst, reg, (imm_value & 0x1F), 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rcx"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_shl_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ISHR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_sar_reg_imm_size(inst, reg, (imm_value & 0x1F), 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rcx"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_sar_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ISHR_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_shr_reg_imm_size(inst, reg, (imm_value & 0x1F), 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rcx"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_shr_reg_size(inst, reg, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LAND:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_and_reg_imm_size(inst, reg, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_and_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_and_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_or_reg_imm_size(inst, reg, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_or_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_or_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LXOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_xor_reg_imm_size(inst, reg, imm_value, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_xor_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_xor_reg_reg_size(inst, reg, reg2, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LNOT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_not_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LSHL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_shl_reg_imm_size(inst, reg, (imm_value & 0x3F), 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rcx"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_shl_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LSHR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_sar_reg_imm_size(inst, reg, (imm_value & 0x3F), 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rcx"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_sar_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LSHR_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_shr_reg_imm_size(inst, reg, (imm_value & 0x3F), 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_sreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rcx"), -1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_shr_reg_size(inst, reg, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR:
{
	unsigned char * inst;
	{
		_jit_regs_spill_all(gen);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = output_branch(func, inst, 0xEB /* jmp */, insn);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_BR_IFALSE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 4);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_ITRUE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 4);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_IEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 4);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_INE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 4);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_ILT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_test_reg_reg_size(inst, reg, reg, 4);
			}
			else
			{
				x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			}
			inst = output_branch(func, inst, 0x7C /* lt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x7C /* lt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x7C /* lt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_ILT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			inst = output_branch(func, inst, 0x72 /* lt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x72 /* lt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x72 /* lt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_ILE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_test_reg_reg_size(inst, reg, reg, 4);
			}
			else
			{
				x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			}
			inst = output_branch(func, inst, 0x7E /* le */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x7E /* le */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x7E /* le */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_ILE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			inst = output_branch(func, inst, 0x76 /* le_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x76 /* le_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x76 /* le_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_IGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_test_reg_reg_size(inst, reg, reg, 4);
			}
			else
			{
				x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			}
			inst = output_branch(func, inst, 0x7F /* gt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x7F /* gt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x7F /* gt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_IGT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			inst = output_branch(func, inst, 0x77 /* gt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x77 /* gt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x77 /* gt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_IGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			if(imm_value == 0)
			{
				x86_64_test_reg_reg_size(inst, reg, reg, 4);
			}
			else
			{
				x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			}
			inst = output_branch(func, inst, 0x7D /* ge */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x7D /* ge */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x7D /* ge */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_IGE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 4);
			inst = output_branch(func, inst, 0x73 /* ge_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 4);
			inst = output_branch(func, inst, 0x73 /* ge_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			inst = output_branch(func, inst, 0x73 /* ge_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LFALSE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 8);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LTRUE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 8);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 8);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x74 /* eq */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LNE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg, reg, 8);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x75 /* ne */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LLT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x7C /* lt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x7C /* lt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x7C /* lt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LLT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x72 /* lt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x72 /* lt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x72 /* lt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x7E /* le */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x7E /* le */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x7E /* le */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LLE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x76 /* le_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x76 /* le_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x76 /* le_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x7F /* gt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x7F /* gt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x7F /* gt */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LGT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x77 /* gt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x77 /* gt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x77 /* gt_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x7D /* ge */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x7D /* ge */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x7D /* ge */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_LGE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg, imm_value, 8);
			inst = output_branch(func, inst, 0x73 /* ge_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg, X86_64_RBP, local_offset, 8);
			inst = output_branch(func, inst, 0x73 /* ge_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			inst = output_branch(func, inst, 0x73 /* ge_un */, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_Z, reg, (void *)imm_value, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_Z, reg, X86_64_RBP, local_offset, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_Z, reg, reg2, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FNE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NZ, reg, (void *)imm_value, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NZ, reg, X86_64_RBP, local_offset, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NZ, reg, reg2, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FLT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_C, reg, (void *)imm_value, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_C, reg, X86_64_RBP, local_offset, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_C, reg, reg2, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FLT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_C, reg, (void *)imm_value, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_C, reg, X86_64_RBP, local_offset, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_C, reg, reg2, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_BE, reg, (void *)imm_value, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_BE, reg, X86_64_RBP, local_offset, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_BE, reg, reg2, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FLE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_BE, reg, (void *)imm_value, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_BE, reg, X86_64_RBP, local_offset, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_BE, reg, reg2, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NBE, reg, (void *)imm_value, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NBE, reg, X86_64_RBP, local_offset, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NBE, reg, reg2, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FGT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NBE, reg, (void *)imm_value, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NBE, reg, X86_64_RBP, local_offset, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NBE, reg, reg2, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NC, reg, (void *)imm_value, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NC, reg, X86_64_RBP, local_offset, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NC, reg, reg2, 0, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_FGE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NC, reg, (void *)imm_value, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NC, reg, X86_64_RBP, local_offset, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NC, reg, reg2, 0, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_Z, reg, (void *)imm_value, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_Z, reg, X86_64_RBP, local_offset, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_Z, reg, reg2, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DNE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NZ, reg, (void *)imm_value, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NZ, reg, X86_64_RBP, local_offset, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NZ, reg, reg2, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DLT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_C, reg, (void *)imm_value, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_C, reg, X86_64_RBP, local_offset, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_C, reg, reg2, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DLT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_C, reg, (void *)imm_value, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_C, reg, X86_64_RBP, local_offset, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_C, reg, reg2, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_BE, reg, (void *)imm_value, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_BE, reg, X86_64_RBP, local_offset, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_BE, reg, reg2, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DLE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_BE, reg, (void *)imm_value, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_BE, reg, X86_64_RBP, local_offset, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_BE, reg, reg2, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NBE, reg, (void *)imm_value, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NBE, reg, X86_64_RBP, local_offset, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NBE, reg, reg2, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DGT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NBE, reg, (void *)imm_value, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NBE, reg, X86_64_RBP, local_offset, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NBE, reg, reg2, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NC, reg, (void *)imm_value, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NC, reg, X86_64_RBP, local_offset, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NC, reg, reg2, 1, 0, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_BR_DGE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = xmm_cmp_brcc_reg_imm(gen, func, inst, X86_CC_NC, reg, (void *)imm_value, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			inst = xmm_cmp_brcc_reg_membase(func, inst, X86_CC_NC, reg, X86_64_RBP, local_offset, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = xmm_cmp_brcc_reg_reg(func, inst, X86_CC_NC, reg, reg2, 1, 1, insn);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg2, reg2, 4);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_INE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg2, reg2, 4);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ILT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_LT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_LT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_LT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ILT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_LT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_LT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_LT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ILE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_LE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_LE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_LE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ILE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_LE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_LE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_LE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_GT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_GT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_GT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IGT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_GT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_GT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_GT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_GE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_GE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_GE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IGE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 4);
			inst = setcc_reg(inst, reg, X86_CC_GE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 4);
			inst = setcc_reg(inst, reg, X86_CC_GE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 4);
			inst = setcc_reg(inst, reg, X86_CC_GE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg2, reg2, 8);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_EQ, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LNE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_test_reg_reg_size(inst, reg2, reg2, 8);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_NE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LLT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_LT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_LT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_LT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LLT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_LT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_LT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_LT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_LE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_LE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_LE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LLE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_LE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_LE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_LE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_GT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_GT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_GT, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LGT_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_GT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_GT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_GT, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_GE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_GE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_GE, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LGE_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value;
	jit_nint local_offset;
	if(insn->value2->is_nint_constant && insn->value2->address >= -2147483648 && insn->value2->address <= 2147483647)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			x86_64_cmp_reg_imm_size(inst, reg2, imm_value, 8);
			inst = setcc_reg(inst, reg, X86_CC_GE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_cmp_reg_membase_size(inst, reg2, X86_64_RBP, local_offset, 8);
			inst = setcc_reg(inst, reg, X86_CC_GE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg2, reg3, 8);
			inst = setcc_reg(inst, reg, X86_CC_GE, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_Z, reg2, (void *)imm_value, reg3, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_Z, reg2, reg3, reg4, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FNE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NZ, reg2, (void *)imm_value, reg3, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NZ, reg2, reg3, reg4, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_C, reg2, (void *)imm_value, reg3, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_C, reg2, reg3, reg4, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_C, reg2, (void *)imm_value, reg3, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_C, reg2, reg3, reg4, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_BE, reg2, (void *)imm_value, reg3, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_BE, reg2, reg3, reg4, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FLE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_BE, reg2, (void *)imm_value, reg3, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_BE, reg2, reg3, reg4, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NBE, reg2, (void *)imm_value, reg3, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NBE, reg2, reg3, reg4, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FGT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NBE, reg2, (void *)imm_value, reg3, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NBE, reg2, reg3, reg4, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NC, reg2, (void *)imm_value, reg3, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NC, reg2, reg3, reg4, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FGE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (23));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NC, reg2, (void *)imm_value, reg3, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NC, reg2, reg3, reg4, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DEQ:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_Z, reg2, (void *)imm_value, reg3, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_Z, reg2, reg3, reg4, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DNE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NZ, reg2, (void *)imm_value, reg3, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST | _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NZ, reg2, reg3, reg4, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DLT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_C, reg2, (void *)imm_value, reg3, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_C, reg2, reg3, reg4, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DLT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_C, reg2, (void *)imm_value, reg3, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_C, reg2, reg3, reg4, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_BE, reg2, (void *)imm_value, reg3, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_BE, reg2, reg3, reg4, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DLE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_BE, reg2, (void *)imm_value, reg3, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_BE, reg2, reg3, reg4, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DGT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NBE, reg2, (void *)imm_value, reg3, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NBE, reg2, reg3, reg4, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DGT_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NBE, reg2, (void *)imm_value, reg3, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NBE, reg2, reg3, reg4, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DGE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NC, reg2, (void *)imm_value, reg3, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NC, reg2, reg3, reg4, 1, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DGE_INV:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (24));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_imm(gen, inst, reg, X86_CC_NC, reg2, (void *)imm_value, reg3, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, (20));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = xmm_cmp_setcc_reg_reg(inst, reg, X86_CC_NC, reg2, reg3, reg4, 1, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FSQRT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_sqrtss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_sqrtss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DSQRT:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		{
			x86_64_sqrtsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_sqrtsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IMAX:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			x86_64_cmov_reg_reg_size(inst, X86_CC_LT, reg, reg2, 1, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IMAX_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			x86_64_cmov_reg_reg_size(inst, X86_CC_LT, reg, reg2, 0, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IMIN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			x86_64_cmov_reg_reg_size(inst, X86_CC_GT, reg, reg2, 1, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_IMIN_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 4);
			x86_64_cmov_reg_reg_size(inst, X86_CC_GT, reg, reg2, 0, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ISIGN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value1->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		{
			if(imm_value < 0)
			{
				x86_64_mov_reg_imm_size(inst, reg, -1, 4);
			}
			else if(imm_value > 0)
			{
				x86_64_mov_reg_imm_size(inst, reg, 1, 4);
			}
			else
			{
				x86_64_clear_reg(inst, reg);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_clear_reg(inst, reg);
			x86_64_test_reg_reg_size(inst, reg2, reg2, 4);
			x86_64_set_reg(inst, X86_CC_NZ, reg, 0);
			x86_64_sar_reg_imm_size(inst, reg2, 31, 4);
			x86_64_or_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LMAX:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			x86_64_cmov_reg_reg_size(inst, X86_CC_LT, reg, reg2, 1, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LMAX_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			x86_64_cmov_reg_reg_size(inst, X86_CC_LT, reg, reg2, 0, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LMIN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			x86_64_cmov_reg_reg_size(inst, X86_CC_GT, reg, reg2, 1, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LMIN_UN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_cmp_reg_reg_size(inst, reg, reg2, 8);
			x86_64_cmov_reg_reg_size(inst, X86_CC_GT, reg, reg2, 0, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LSIGN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value;
	if(insn->value1->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		{
			if(imm_value < 0)
			{
				x86_64_mov_reg_imm_size(inst, reg, -1, 4);
			}
			else if(imm_value > 0)
			{
				x86_64_mov_reg_imm_size(inst, reg, 1, 4);
			}
			else
			{
				x86_64_clear_reg(inst, reg);
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, _JIT_REGS_EARLY_CLOBBER, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_clear_reg(inst, reg);
			x86_64_test_reg_reg_size(inst, reg2, reg2, 8);
			x86_64_set_reg(inst, X86_CC_NZ, reg, 0);
			x86_64_sar_reg_imm_size(inst, reg2, 63, 8);
			x86_64_or_reg_reg_size(inst, reg, reg2, 4);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FMAX:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_maxss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_maxss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FMIN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_minss_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_minss_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DMAX:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_maxsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_maxsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DMIN:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint local_offset;
	if(!insn->value2->is_constant && !insn->value2->in_register && !insn->value2->has_global_register&& (insn->flags & JIT_INSN_VALUE2_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value2);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		local_offset = insn->value2->frame_offset;
		{
			x86_64_minsd_reg_membase(inst, reg, X86_64_RBP, local_offset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_COMMUTATIVE);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			x86_64_minsd_reg_reg(inst, reg, reg2);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FFLOOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_rounds_reg_membase(inst, reg, local_offset, reg2, X86_ROUND_DOWN);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_rounds_reg_reg(inst, reg, reg2, reg3, X86_ROUND_DOWN);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DFLOOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_roundd_reg_membase(inst, reg, local_offset, reg2, X86_ROUND_DOWN);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_roundd_reg_reg(inst, reg, reg2, reg3, X86_ROUND_DOWN);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFFLOOR:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_roundnf(inst, reg2, X86_ROUND_DOWN);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_FCEIL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_rounds_reg_membase(inst, reg, local_offset, reg2, X86_ROUND_UP);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_rounds_reg_reg(inst, reg, reg2, reg3, X86_ROUND_UP);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_DCEIL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint local_offset;
	if(!insn->value1->is_constant && !insn->value1->in_register && !insn->value1->has_global_register&& (insn->flags & JIT_INSN_VALUE1_NEXT_USE) == 0)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_gen_fix_value(insn->value1);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		local_offset = insn->value1->frame_offset;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_roundd_reg_membase(inst, reg, local_offset, reg2, X86_ROUND_UP);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_xreg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_xreg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_roundd_reg_reg(inst, reg, reg2, reg3, X86_ROUND_UP);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_NFCEIL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_freg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_freg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 128);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = x86_64_roundnf(inst, reg2, X86_ROUND_UP);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_CHECK_NULL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
	#if 0 && defined(JIT_USE_SIGNALS)
			/* if reg contains NULL this generates SEGV and the signal
			   handler will throw the exception  */
			x86_64_cmp_reg_membase_size(inst, reg, reg, 0, 8);
	#else
			unsigned char *patch;
			x86_64_test_reg_reg_size(inst, reg, reg, 8);
			patch = inst;
			x86_branch8(inst, X86_CC_NE, 0, 0);
			inst = throw_builtin(inst, func, JIT_RESULT_NULL_REFERENCE);
			x86_patch(patch, inst);
	#endif
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_CALL:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			jit_function_t func = (jit_function_t)(insn->dest);
			inst = x86_64_call_code(inst, (jit_nint)jit_function_to_closure(func));
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_TAIL:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			jit_function_t func = (jit_function_t)(insn->dest);
			x86_64_mov_reg_reg_size(inst, X86_64_RSP, X86_64_RBP, 8);
			x86_64_pop_reg_size(inst, X86_64_RBP, 8);
			x86_64_jump_to_code(inst, (jit_nint)jit_function_to_closure(func));
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_INDIRECT:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			x86_64_mov_reg_imm_size(inst, X86_64_RAX, 8, 4);
			x86_64_call_reg(inst, X86_64_SCRATCH);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_INDIRECT_TAIL:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			x86_64_mov_reg_reg_size(inst, X86_64_RSP, X86_64_RBP, 8);
			x86_64_pop_reg_size(inst, X86_64_RBP, 8);
			x86_64_jmp_reg(inst, X86_64_SCRATCH);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_VTABLE_PTR:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			x86_64_mov_reg_imm_size(inst, X86_64_RAX, 8, 4);
			x86_64_call_reg(inst, X86_64_SCRATCH);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_VTABLE_PTR_TAIL:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			x86_64_mov_reg_reg_size(inst, X86_64_RSP, X86_64_RBP, 8);
			x86_64_pop_reg_size(inst, X86_64_RBP, 8);
			x86_64_jmp_reg(inst, X86_64_SCRATCH);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_EXTERNAL:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			inst = x86_64_call_code(inst, (jit_nint)(insn->dest));
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_EXTERNAL_TAIL:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			x86_64_mov_reg_reg_size(inst, X86_64_RSP, X86_64_RBP, 8);
			x86_64_pop_reg_size(inst, X86_64_RBP, 8);
			x86_64_jump_to_code(inst, (jit_nint)(insn->dest));
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_THROW:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_BRANCH);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_mov_reg_reg_size(inst, X86_64_RDI, reg, 8);
			if(func->builder->setjmp_value != 0)
			{
				jit_nint pc_offset;
	
				/* We have a "setjmp" block in the current function,
				   so we must record the location of the throw first */
				_jit_gen_fix_value(func->builder->setjmp_value);
				pc_offset = func->builder->setjmp_value->frame_offset +
								jit_jmp_catch_pc_offset;
	
				x86_64_lea_membase_size(inst, X86_64_SCRATCH, X86_64_RIP, 0, 8);
				x86_64_mov_membase_reg_size(inst, X86_64_RBP, pc_offset,
											X86_64_SCRATCH, 8);
			}
			inst = x86_64_call_code(inst, (jit_nint)jit_exception_throw);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_RETHROW:
{
		{ /* Not used in native code back ends */ }
}
break;

case JIT_OP_LOAD_PC:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		{
			x86_64_lea_membase_size(inst, reg, X86_64_RIP, 0, 8);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_LOAD_EXCEPTION_PC:
{
		{ /* Not used in native code back ends */ }
}
break;

case JIT_OP_ENTER_FINALLY:
{
	unsigned char * inst;
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			/* The return address is on the stack */
			x86_64_sub_reg_imm_size(inst, X86_64_RSP, 8, 8);
		 }
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_LEAVE_FINALLY:
{
	unsigned char * inst;
	{
		_jit_regs_spill_all(gen);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			/* The "finally" return address is on the stack */
			x86_64_add_reg_imm_size(inst, X86_64_RSP, 8, 8);
			x86_64_ret(inst);
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_CALL_FINALLY:
{
	unsigned char * inst;
	{
		_jit_regs_spill_all(gen);
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		{
			jit_block_t block;
	
			block = jit_block_from_label(func, (jit_label_t)(insn->dest));
			if(!block)
			{
				return;
			}
	
			if(block->address)
			{
				inst = x86_64_call_code(inst, (jit_nint)block->address);
			}
			else
			{
				jit_int fixup;
	
				if(block->fixup_list)
				{
					fixup = _JIT_CALC_FIXUP(block->fixup_list, inst + 1);
				}
				else
				{
					fixup = 0;
				}
				block->fixup_list = (void *)(inst + 1);
				x86_64_call_imm(inst, fixup);
			}
		}
		gen->ptr = (unsigned char *)inst;
	}
}
break;

case JIT_OP_ADDRESS_OF_LABEL:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_FREE_DEST);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		{
			jit_int *fixup;
	
			block = jit_block_from_label(func, (jit_label_t)(insn->value1));
			if(block->address)
			{
				/* The label is in the current function so we assume that the */
				/* displacement to the current instruction is in the +-2GB range */
	
				x86_64_lea_membase_size(inst, reg, X86_64_RIP, 0, 8);
				fixup = (jit_int *)(inst - 4);
				fixup[0] = (jit_int)((jit_nint)block->address - (jit_nint)inst);
			}
			else
			{
				/* Output a placeholder and record on the block's fixup list */
				/* The label is in the current function so we assume that the */
				/* displacement to the current instruction will be in the +-2GB range */
				x86_64_lea_membase_size(inst, reg, X86_64_RIP, 0, 8);
				fixup = (jit_int *)(inst - 4);
				if(block->fixup_list)
				{
					fixup[0] = _JIT_CALC_FIXUP(block->fixup_list, fixup);
				}
				block->fixup_list = (void *)fixup;
			}
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_MEMCPY:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3, reg4;
	jit_nint imm_value;
	if(insn->value2->is_constant && (insn->value2->address <= 0))
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value2->address;
		{ }
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value2->is_constant && (insn->value2->address <= _JIT_MAX_MEMCPY_INLINE))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg4 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			inst = small_block_copy(gen, inst, reg, 0, reg2, 0, imm_value, reg3, reg4, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value2->is_constant)
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		imm_value = insn->value2->address;
		{
			inst = memory_copy(gen, inst, reg, 0, reg2, 0, imm_value);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdi"), -1);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rsi"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = x86_64_call_code(inst, (jit_nint)jit_memcpy);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_MEMSET:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2, reg3;
	jit_nint imm_value, imm_value2;
	if(insn->value2->is_constant && (insn->value2->address <= 0))
	{
		_jit_gen_check_space(gen, 32);
		inst = (unsigned char *)(gen->ptr);
		imm_value = insn->value2->address;
		{ }
		gen->ptr = (unsigned char *)inst;
	}
	else if(insn->value1->is_constant && insn->value2->is_constant && (insn->value1->address == 0 && insn->value2->address <= _JIT_MAX_MEMSET_INLINE && insn->value2->address % 16 == 0))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = small_block_set(gen, inst, reg, 0, imm_value, imm_value2, 0, reg2, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value1->is_constant && insn->value2->is_constant && (insn->value2->address <= _JIT_MAX_MEMSET_INLINE && insn->value2->address < 32))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			inst = small_block_set(gen, inst, reg, 0, imm_value, imm_value2, reg2, 0, 0, 0);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else if(insn->value1->is_constant && insn->value2->is_constant && (insn->value2->address <= _JIT_MAX_MEMSET_INLINE))
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_scratch(&regs, 1)].cpu_reg;
		{
			inst = small_block_set(gen, inst, reg, 0, imm_value, imm_value2, reg2, reg3, 0, 1);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
	else
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_dest(gen, &regs, _jit_regs_lookup("rdi"), -1);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value1(gen, &regs, _jit_regs_lookup("rsi"), -1);
		_jit_regs_init_value2(&regs, insn, 0, x86_64_reg);
		_jit_regs_set_value2(gen, &regs, _jit_regs_lookup("rdx"), -1);
		_jit_regs_clobber_class(gen, &regs, x86_64_creg);
		_jit_regs_clobber_class(gen, &regs, x86_64_xreg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		reg2 = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		reg3 = _jit_reg_info[_jit_regs_get_value2(&regs)].cpu_reg;
		{
			inst = x86_64_call_code(inst, (jit_nint)jit_memset);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_ALLOCA:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg;
	{
		_jit_regs_init(gen, &regs, 0);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_init_value1(&regs, insn, 0, x86_64_reg);
		_jit_regs_begin(gen, &regs, 32);
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_value1(&regs)].cpu_reg;
		{
			x86_64_sub_reg_reg_size(inst, X86_64_RSP, reg, 8);
			x86_64_mov_reg_reg_size(inst, reg, X86_64_RSP, 8);
			inst = fixup_alloca(gen, inst, reg);
			gen->stack_changed = 1;
	}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

case JIT_OP_JUMP_TABLE:
{
	unsigned char * inst;
	_jit_regs_t regs;
	int reg, reg2;
	jit_nint imm_value, imm_value2;
	{
		_jit_regs_init(gen, &regs, _JIT_REGS_TERNARY | _JIT_REGS_BRANCH);
		_jit_regs_init_dest(&regs, insn, 0, x86_64_reg);
		_jit_regs_add_scratch(&regs, x86_64_reg);
		_jit_regs_clobber_all(gen, &regs);
		_jit_regs_begin(gen, &regs, (64 + sizeof(void *) * insn->value2->address));
		inst = (unsigned char *)(gen->ptr);
		reg = _jit_reg_info[_jit_regs_get_dest(&regs)].cpu_reg;
		imm_value = insn->value1->address;
		imm_value2 = insn->value2->address;
		reg2 = _jit_reg_info[_jit_regs_get_scratch(&regs, 0)].cpu_reg;
		{
			unsigned char *patch_jump_table;
			unsigned char *patch_fall_through;
			int index;
			jit_label_t *labels;
			jit_nint num_labels;
			jit_block_t block;
	
			labels = (jit_label_t *) imm_value;
			num_labels = imm_value2;
	
			patch_jump_table = (unsigned char *)_jit_gen_alloc(gen, sizeof(void *) * imm_value2);
			if(!patch_jump_table)
			{
				/* The cache is full */
				return;
			}
	
			x86_64_mov_reg_imm_size(inst, reg2, (jit_nint)patch_jump_table, 8);
			x86_64_cmp_reg_imm_size(inst, reg, num_labels, 8);
			patch_fall_through = inst;
			x86_branch32(inst, X86_CC_AE, 0, 0);
	
			if(func->builder->position_independent)
			{
				/* TODO */
				TODO();
			}
			else
			{
				x86_64_jmp_memindex(inst, reg2, 0, reg, 3);
			}
	
			for(index = 0; index < num_labels; index++)
			{
				block = jit_block_from_label(func, labels[index]);
				if(!block)
				{
					return;
				}
	
				if(func->builder->position_independent)
				{
					/* TODO */
					TODO();
				}
				else
				{
					if(block->address)
					{
						x86_64_imm_emit64(patch_jump_table, (jit_nint)(block->address));
					}
					else
					{
						/* Output a placeholder and record on the block's absolute fixup list */
						x86_64_imm_emit64(patch_jump_table, (jit_nint)(block->fixup_absolute_list));
						block->fixup_absolute_list = (void *)(patch_jump_table - 8);
					}
				}
			}
	
			x86_patch(patch_fall_through, inst);
		}
		gen->ptr = (unsigned char *)inst;
		_jit_regs_commit(gen, &regs);
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_TRUNC_SBYTE:
case JIT_OP_TRUNC_UBYTE:
case JIT_OP_TRUNC_SHORT:
case JIT_OP_TRUNC_USHORT:
case JIT_OP_TRUNC_INT:
case JIT_OP_TRUNC_UINT:
case JIT_OP_LOW_WORD:
case JIT_OP_EXPAND_INT:
case JIT_OP_EXPAND_UINT:
case JIT_OP_INT_TO_NFLOAT:
case JIT_OP_LONG_TO_NFLOAT:
case JIT_OP_FLOAT32_TO_INT:
case JIT_OP_FLOAT32_TO_UINT:
case JIT_OP_FLOAT32_TO_LONG:
case JIT_OP_INT_TO_FLOAT32:
case JIT_OP_UINT_TO_FLOAT32:
case JIT_OP_LONG_TO_FLOAT32:
case JIT_OP_FLOAT64_TO_FLOAT32:
case JIT_OP_FLOAT64_TO_INT:
case JIT_OP_FLOAT64_TO_UINT:
case JIT_OP_FLOAT64_TO_LONG:
case JIT_OP_INT_TO_FLOAT64:
case JIT_OP_UINT_TO_FLOAT64:
case JIT_OP_LONG_TO_FLOAT64:
case JIT_OP_FLOAT32_TO_FLOAT64:
case JIT_OP_NFLOAT_TO_INT:
case JIT_OP_NFLOAT_TO_LONG:
case JIT_OP_FLOAT32_TO_NFLOAT:
case JIT_OP_FLOAT64_TO_NFLOAT:
case JIT_OP_NFLOAT_TO_FLOAT32:
case JIT_OP_NFLOAT_TO_FLOAT64:
case JIT_OP_COPY_LOAD_SBYTE:
case JIT_OP_COPY_LOAD_UBYTE:
case JIT_OP_COPY_STORE_BYTE:
case JIT_OP_COPY_LOAD_SHORT:
case JIT_OP_COPY_LOAD_USHORT:
case JIT_OP_COPY_STORE_SHORT:
case JIT_OP_COPY_INT:
case JIT_OP_COPY_LONG:
case JIT_OP_COPY_FLOAT32:
case JIT_OP_COPY_FLOAT64:
case JIT_OP_COPY_NFLOAT:
case JIT_OP_COPY_STRUCT:
case JIT_OP_ADDRESS_OF:
case JIT_OP_INCOMING_REG:
case JIT_OP_RETURN_REG:
case JIT_OP_RETRIEVE_FRAME_POINTER:
case JIT_OP_PUSH_INT:
case JIT_OP_PUSH_LONG:
case JIT_OP_PUSH_FLOAT32:
case JIT_OP_PUSH_FLOAT64:
case JIT_OP_PUSH_NFLOAT:
case JIT_OP_PUSH_STRUCT:
case JIT_OP_POP_STACK:
case JIT_OP_SET_PARAM_INT:
case JIT_OP_SET_PARAM_LONG:
case JIT_OP_SET_PARAM_FLOAT32:
case JIT_OP_SET_PARAM_FLOAT64:
case JIT_OP_SET_PARAM_NFLOAT:
case JIT_OP_SET_PARAM_STRUCT:
case JIT_OP_FLUSH_SMALL_STRUCT:
case JIT_OP_RETURN:
case JIT_OP_RETURN_INT:
case JIT_OP_RETURN_LONG:
case JIT_OP_RETURN_FLOAT32:
case JIT_OP_RETURN_FLOAT64:
case JIT_OP_RETURN_NFLOAT:
case JIT_OP_RETURN_SMALL_STRUCT:
case JIT_OP_LOAD_RELATIVE_SBYTE:
case JIT_OP_LOAD_RELATIVE_UBYTE:
case JIT_OP_LOAD_RELATIVE_SHORT:
case JIT_OP_LOAD_RELATIVE_USHORT:
case JIT_OP_LOAD_RELATIVE_INT:
case JIT_OP_LOAD_RELATIVE_LONG:
case JIT_OP_LOAD_RELATIVE_FLOAT32:
case JIT_OP_LOAD_RELATIVE_FLOAT64:
case JIT_OP_LOAD_RELATIVE_NFLOAT:
case JIT_OP_LOAD_RELATIVE_STRUCT:
case JIT_OP_STORE_RELATIVE_BYTE:
case JIT_OP_STORE_RELATIVE_SHORT:
case JIT_OP_STORE_RELATIVE_INT:
case JIT_OP_STORE_RELATIVE_LONG:
case JIT_OP_STORE_RELATIVE_FLOAT32:
case JIT_OP_STORE_RELATIVE_FLOAT64:
case JIT_OP_STORE_RELATIVE_STRUCT:
case JIT_OP_ADD_RELATIVE:
case JIT_OP_LOAD_ELEMENT_SBYTE:
case JIT_OP_LOAD_ELEMENT_UBYTE:
case JIT_OP_LOAD_ELEMENT_SHORT:
case JIT_OP_LOAD_ELEMENT_USHORT:
case JIT_OP_LOAD_ELEMENT_INT:
case JIT_OP_LOAD_ELEMENT_LONG:
case JIT_OP_LOAD_ELEMENT_FLOAT32:
case JIT_OP_LOAD_ELEMENT_FLOAT64:
case JIT_OP_STORE_ELEMENT_BYTE:
case JIT_OP_STORE_ELEMENT_SHORT:
case JIT_OP_STORE_ELEMENT_INT:
case JIT_OP_STORE_ELEMENT_LONG:
case JIT_OP_STORE_ELEMENT_FLOAT32:
case JIT_OP_STORE_ELEMENT_FLOAT64:
case JIT_OP_IADD:
case JIT_OP_ISUB:
case JIT_OP_INEG:
case JIT_OP_IMUL:
case JIT_OP_IDIV:
case JIT_OP_IDIV_UN:
case JIT_OP_IREM:
case JIT_OP_IREM_UN:
case JIT_OP_LADD:
case JIT_OP_LSUB:
case JIT_OP_LNEG:
case JIT_OP_LMUL:
case JIT_OP_LDIV:
case JIT_OP_LDIV_UN:
case JIT_OP_LREM:
case JIT_OP_LREM_UN:
case JIT_OP_FADD:
case JIT_OP_FSUB:
case JIT_OP_FMUL:
case JIT_OP_FDIV:
case JIT_OP_FABS:
case JIT_OP_FNEG:
case JIT_OP_DADD:
case JIT_OP_DSUB:
case JIT_OP_DMUL:
case JIT_OP_DDIV:
case JIT_OP_DABS:
case JIT_OP_DNEG:
case JIT_OP_NFABS:
case JIT_OP_NFNEG:
case JIT_OP_IAND:
case JIT_OP_IOR:
case JIT_OP_IXOR:
case JIT_OP_INOT:
case JIT_OP_ISHL:
case JIT_OP_ISHR:
case JIT_OP_ISHR_UN:
case JIT_OP_LAND:
case JIT_OP_LOR:
case JIT_OP_LXOR:
case JIT_OP_LNOT:
case JIT_OP_LSHL:
case JIT_OP_LSHR:
case JIT_OP_LSHR_UN:
case JIT_OP_BR:
case JIT_OP_BR_IFALSE:
case JIT_OP_BR_ITRUE:
case JIT_OP_BR_IEQ:
case JIT_OP_BR_INE:
case JIT_OP_BR_ILT:
case JIT_OP_BR_ILT_UN:
case JIT_OP_BR_ILE:
case JIT_OP_BR_ILE_UN:
case JIT_OP_BR_IGT:
case JIT_OP_BR_IGT_UN:
case JIT_OP_BR_IGE:
case JIT_OP_BR_IGE_UN:
case JIT_OP_BR_LFALSE:
case JIT_OP_BR_LTRUE:
case JIT_OP_BR_LEQ:
case JIT_OP_BR_LNE:
case JIT_OP_BR_LLT:
case JIT_OP_BR_LLT_UN:
case JIT_OP_BR_LLE:
case JIT_OP_BR_LLE_UN:
case JIT_OP_BR_LGT:
case JIT_OP_BR_LGT_UN:
case JIT_OP_BR_LGE:
case JIT_OP_BR_LGE_UN:
case JIT_OP_BR_FEQ:
case JIT_OP_BR_FNE:
case JIT_OP_BR_FLT:
case JIT_OP_BR_FLT_INV:
case JIT_OP_BR_FLE:
case JIT_OP_BR_FLE_INV:
case JIT_OP_BR_FGT:
case JIT_OP_BR_FGT_INV:
case JIT_OP_BR_FGE:
case JIT_OP_BR_FGE_INV:
case JIT_OP_BR_DEQ:
case JIT_OP_BR_DNE:
case JIT_OP_BR_DLT:
case JIT_OP_BR_DLT_INV:
case JIT_OP_BR_DLE:
case JIT_OP_BR_DLE_INV:
case JIT_OP_BR_DGT:
case JIT_OP_BR_DGT_INV:
case JIT_OP_BR_DGE:
case JIT_OP_BR_DGE_INV:
case JIT_OP_IEQ:
case JIT_OP_INE:
case JIT_OP_ILT:
case JIT_OP_ILT_UN:
case JIT_OP_ILE:
case JIT_OP_ILE_UN:
case JIT_OP_IGT:
case JIT_OP_IGT_UN:
case JIT_OP_IGE:
case JIT_OP_IGE_UN:
case JIT_OP_LEQ:
case JIT_OP_LNE:
case JIT_OP_LLT:
case JIT_OP_LLT_UN:
case JIT_OP_LLE:
case JIT_OP_LLE_UN:
case JIT_OP_LGT:
case JIT_OP_LGT_UN:
case JIT_OP_LGE:
case JIT_OP_LGE_UN:
case JIT_OP_FEQ:
case JIT_OP_FNE:
case JIT_OP_FLT:
case JIT_OP_FLT_INV:
case JIT_OP_FLE:
case JIT_OP_FLE_INV:
case JIT_OP_FGT:
case JIT_OP_FGT_INV:
case JIT_OP_FGE:
case JIT_OP_FGE_INV:
case JIT_OP_DEQ:
case JIT_OP_DNE:
case JIT_OP_DLT:
case JIT_OP_DLT_INV:
case JIT_OP_DLE:
case JIT_OP_DLE_INV:
case JIT_OP_DGT:
case JIT_OP_DGT_INV:
case JIT_OP_DGE:
case JIT_OP_DGE_INV:
case JIT_OP_FSQRT:
case JIT_OP_DSQRT:
case JIT_OP_IMAX:
case JIT_OP_IMAX_UN:
case JIT_OP_IMIN:
case JIT_OP_IMIN_UN:
case JIT_OP_ISIGN:
case JIT_OP_LMAX:
case JIT_OP_LMAX_UN:
case JIT_OP_LMIN:
case JIT_OP_LMIN_UN:
case JIT_OP_LSIGN:
case JIT_OP_FMAX:
case JIT_OP_FMIN:
case JIT_OP_DMAX:
case JIT_OP_DMIN:
case JIT_OP_FFLOOR:
case JIT_OP_DFLOOR:
case JIT_OP_NFFLOOR:
case JIT_OP_FCEIL:
case JIT_OP_DCEIL:
case JIT_OP_NFCEIL:
case JIT_OP_CHECK_NULL:
case JIT_OP_CALL:
case JIT_OP_CALL_TAIL:
case JIT_OP_CALL_INDIRECT:
case JIT_OP_CALL_INDIRECT_TAIL:
case JIT_OP_CALL_VTABLE_PTR:
case JIT_OP_CALL_VTABLE_PTR_TAIL:
case JIT_OP_CALL_EXTERNAL:
case JIT_OP_CALL_EXTERNAL_TAIL:
case JIT_OP_THROW:
case JIT_OP_RETHROW:
case JIT_OP_LOAD_PC:
case JIT_OP_LOAD_EXCEPTION_PC:
case JIT_OP_ENTER_FINALLY:
case JIT_OP_LEAVE_FINALLY:
case JIT_OP_CALL_FINALLY:
case JIT_OP_ADDRESS_OF_LABEL:
case JIT_OP_MEMCPY:
case JIT_OP_MEMSET:
case JIT_OP_ALLOCA:
case JIT_OP_JUMP_TABLE:
	return 1;

#endif
