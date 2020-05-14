package jit

import (
	"unsafe"

	"github.com/goccy/go-jit/internal/ccall"
)

type Instruction struct {
	*ccall.Instruction
}

func toInstruction(c *ccall.Instruction) *Instruction {
	return &Instruction{c}
}

func (i *Instruction) Code() int {
	return i.Instruction.Code()
}

func (i *Instruction) Dest() *Value {
	return toValue(i.Instruction.Dest())
}

func (i *Instruction) Value1() *Value {
	return toValue(i.Instruction.Value1())
}

func (i *Instruction) Value2() *Value {
	return toValue(i.Instruction.Value2())
}

func (i *Instruction) Label() *Label {
	return toLabel(i.Instruction.Label())
}

func (i *Instruction) Function() *Function {
	return toFunction(i.Instruction.Function())
}

func (i *Instruction) Native() unsafe.Pointer {
	return i.Instruction.Native()
}

func (i *Instruction) Name() string {
	return i.Instruction.Name()
}

func (i *Instruction) Signature() *Type {
	return toType(i.Instruction.Signature())
}

func (i *Instruction) DestIsValue() bool {
	return i.Instruction.DestIsValue()
}
