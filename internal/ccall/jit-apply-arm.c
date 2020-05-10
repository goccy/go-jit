/*
 * jit-apply-arm.c - Apply support routines for ARM.
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

#if defined(__arm) || defined(__arm__)

#include "jit-gen-arm.h"

void _jit_create_closure(unsigned char *buf, void *func,
                         void *closure, void *_type)
{
	arm_inst_buf inst;

	/* Initialize the instruction buffer */
	arm_inst_buf_init(inst, buf, buf + jit_closure_size);

	/* Set up the local stack frame */
	arm_setup_frame(inst, 0);
	arm_alu_reg_imm8(inst, ARM_SUB, ARM_SP, ARM_SP, 24);

	/* Create the apply argument block on the stack */
	arm_store_membase(inst, ARM_R0, ARM_FP, -28);
	arm_store_membase(inst, ARM_R1, ARM_FP, -24);
	arm_store_membase(inst, ARM_R2, ARM_FP, -20);
	arm_store_membase(inst, ARM_R3, ARM_FP, -16);
	arm_alu_reg_imm(inst, ARM_ADD, ARM_R3, ARM_FP, 4);
	arm_store_membase(inst, ARM_R3, ARM_FP, -36);
	arm_store_membase(inst, ARM_R0, ARM_FP, -32);

	/* Set up the arguments for calling "func" */
	arm_mov_reg_imm(inst, ARM_R0, (int)(jit_nint)closure);
	arm_mov_reg_reg(inst, ARM_R1, ARM_SP);

	/* Call the closure handling function */
	arm_call(inst, func);

	/* Pop the current stack frame and return */
	arm_pop_frame(inst, 0);
}

void *_jit_create_redirector(unsigned char *buf, void *func,
							 void *user_data, int abi)
{
	arm_inst_buf inst;

	/* Align "buf" on an appropriate boundary */
	if((((jit_nint)buf) % jit_closure_align) != 0)
	{
		buf += jit_closure_align - (((jit_nint)buf) % jit_closure_align);
	}

	/* Initialize the instruction buffer */
	arm_inst_buf_init(inst, buf, buf + jit_redirector_size);

	/* Set up the local stack frame, and save R0-R3 */
	arm_setup_frame(inst, 0x000F);

	/* Set up the arguments for calling "func" */
	arm_mov_reg_imm(inst, ARM_R0, (int)(jit_nint)user_data);

	/* Call the redirector handling function */
	arm_call(inst, func);

	/* Shift the result into R12, because we are about to restore R0 */
	arm_mov_reg_reg(inst, ARM_R12, ARM_R0);

	/* Pop the current stack frame, but don't change PC yet */
	arm_pop_frame_tail(inst, 0x000F);

	/* Jump to the function that the redirector indicated */
	arm_mov_reg_reg(inst, ARM_PC, ARM_R12);

	/* Flush the cache lines that we just wrote */
	_jit_flush_exec(buf, ((unsigned char *)(inst.current)) - buf);

	/* Return the aligned start of the buffer as the entry point */
	return (void *)buf;
}

/**
 * Creates the indirector, that is the trampoline that permits the Just In Time 
 * compilation of a method the first time that it is executed and its direct execution
 * the following times
 */
void *_jit_create_indirector(unsigned char *buf, void **entry)
{
	arm_inst_buf inst;
	void *start = (void *)buf;

	/* Initialize the instruction buffer */
	arm_inst_buf_init(inst, buf, buf + jit_indirector_size);

	//Load the content of memory at address "entry", that is, the entry point of the function
	arm_mov_reg_imm(inst,ARM_WORK,entry);
	arm_mov_reg_membase(inst,ARM_WORK,ARM_WORK,0,4);
	
	/* Jump to the entry point. */
	arm_mov_reg_reg(inst, ARM_PC, ARM_WORK);

	/* Flush the cache lines that we just wrote */
	_jit_flush_exec(buf, ((unsigned char *)(inst.current)) - buf);
	
	return start;
}

void _jit_pad_buffer(unsigned char *buf, int len)
{
	arm_inst_buf inst;
	
	/* Initialize the instruction buffer */
	arm_inst_buf_init(inst, buf, buf + len*4);
	while(len > 0)
	{
		/* Traditional arm NOP */
		arm_nop(inst);
		--len;
	}
	
	/* Flush the cache lines that we just wrote */
	_jit_flush_exec(buf, ((unsigned char *)(inst.current)) - buf);
}

#endif /* arm */
