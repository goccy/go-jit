/*
 * jit-gen-arm.c - Machine-dependent definitions for ARM.
 *
 * Copyright (C) 2003, 2004  Southern Storm Software, Pty Ltd.
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

#if defined(__arm) || defined(__arm__)

#define	arm_execute		execute_prefix
#define	arm_execute_cc	(execute_prefix | (1 << 20))
#define	arm_execute_imm	(execute_prefix | (1 << 25))

#include "jit-gen-arm.h"

void _arm_mov_reg_imm
	(arm_inst_buf *inst, int reg, int value, int execute_prefix)
{
	int bit;

	/* Handle bytes in various positions */
	for(bit = 0; bit <= (32 - 8); bit += 2)
	{
		if((value & (0xFF << bit)) == value)
		{
			arm_mov_reg_imm8_rotate
				(*inst, reg, ((value >> bit) & 0xFF), (16 - bit / 2) & 0x0F);
			return;
		}
	}

	/* Handle inverted bytes in various positions */
	value = ~value;
	for(bit = 0; bit <= (32 - 8); bit += 2)
	{
		if((value & (0xFF << bit)) == value)
		{
			arm_alu_reg_imm8_rotate
				(*inst, ARM_MVN, reg, 0,
				 ((value >> bit) & 0xFF), (16 - bit / 2) & 0x0F);
			return;
		}
	}

	/* Build the value the hard way, byte by byte */
	value = ~value;
	if((value & 0xFF000000) != 0)
	{
		arm_mov_reg_imm8_rotate(*inst, reg, ((value >> 24) & 0xFF), 4);
		if((value & 0x00FF0000) != 0)
		{
			arm_alu_reg_imm8_rotate
				(*inst, ARM_ADD, reg, reg, ((value >> 16) & 0xFF), 8);
		}
		if((value & 0x0000FF00) != 0)
		{
			arm_alu_reg_imm8_rotate
				(*inst, ARM_ADD, reg, reg, ((value >> 8) & 0xFF), 12);
		}
		if((value & 0x000000FF) != 0)
		{
			arm_alu_reg_imm8(*inst, ARM_ADD, reg, reg, (value & 0xFF));
		}
	}
	else if((value & 0x00FF0000) != 0)
	{
		arm_mov_reg_imm8_rotate(*inst, reg, ((value >> 16) & 0xFF), 8);
		if((value & 0x0000FF00) != 0)
		{
			arm_alu_reg_imm8_rotate
				(*inst, ARM_ADD, reg, reg, ((value >> 8) & 0xFF), 12);
		}
		if((value & 0x000000FF) != 0)
		{
			arm_alu_reg_imm8(*inst, ARM_ADD, reg, reg, (value & 0xFF));
		}
	}
	else if((value & 0x0000FF00) != 0)
	{
		arm_mov_reg_imm8_rotate(*inst, reg, ((value >> 8) & 0xFF), 12);
		if((value & 0x000000FF) != 0)
		{
			arm_alu_reg_imm8(*inst, ARM_ADD, reg, reg, (value & 0xFF));
		}
	}
	else
	{
		arm_mov_reg_imm8(*inst, reg, (value & 0xFF));
	}
}

int arm_is_complex_imm(int value)
{
	int bit;
	int inv_value = ~value;
	for(bit = 0; bit <= (32 - 8); bit += 2)
	{
		if((value & (0xFF << bit)) == value)
		{
			return 0;
		}
		if((inv_value & (0xFF << bit)) == inv_value)
		{
			return 0;
		}
	}
	return 1;
}

void _arm_alu_reg_imm
	(arm_inst_buf *inst, int opc, int dreg,
	 int sreg, int imm, int saveWork, int execute_prefix)
{
	int bit, tempreg;
	for(bit = 0; bit <= (32 - 8); bit += 2)
	{
		if((imm & (0xFF << bit)) == imm)
		{
			arm_alu_reg_imm8_rotate
				(*inst, opc, dreg, sreg,
				 ((imm >> bit) & 0xFF), (16 - bit / 2) & 0x0F);
			return;
		}
	}
	if(saveWork)
	{
		if(dreg != ARM_R2 && sreg != ARM_R2)
		{
			tempreg = ARM_R2;
		}
		else if(dreg != ARM_R3 && sreg != ARM_R3)
		{
			tempreg = ARM_R3;
		}
		else
		{
			tempreg = ARM_R4;
		}
		arm_push_reg(*inst, tempreg);
	}
	else
	{
		tempreg = ARM_WORK;
	}
	_arm_mov_reg_imm(inst, tempreg, imm, execute_prefix);
	arm_alu_reg_reg(*inst, opc, dreg, sreg, tempreg);
	if(saveWork)
	{
		arm_pop_reg(*inst, tempreg);
	}
}

#endif /* arm */
