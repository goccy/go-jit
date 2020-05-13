package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>
*/
import "C"
import "unsafe"

type Instruction struct {
	c C.jit_insn_t
}

func toInstruction(c C.jit_insn_t) *Instruction {
	return &Instruction{c}
}

func (i *Instruction) Code() int {
	return int(C.jit_insn_get_opcode(i.c))
}

func (i *Instruction) Dest() *Value {
	return toValue(C.jit_insn_get_dest(i.c))
}

func (i *Instruction) Value1() *Value {
	return toValue(C.jit_insn_get_value1(i.c))
}

func (i *Instruction) Value2() *Value {
	return toValue(C.jit_insn_get_value2(i.c))
}

func (i *Instruction) Label() *Label {
	return toLabel(C.jit_insn_get_label(i.c))
}

func (i *Instruction) Function() *Function {
	return toFunction(C.jit_insn_get_function(i.c))
}

func (i *Instruction) Native() unsafe.Pointer {
	return C.jit_insn_get_native(i.c)
}

func (i *Instruction) Name() string {
	return C.GoString(C.jit_insn_get_name(i.c))
}

func (i *Instruction) Signature() *Type {
	return toType(C.jit_insn_get_signature(i.c))
}

func (i *Instruction) DestIsValue() bool {
	return int(C.jit_insn_dest_is_value(i.c)) == 1
}
