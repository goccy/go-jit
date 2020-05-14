package jit

import (
	"github.com/goccy/go-jit/internal/ccall"
)

type Value struct {
	*ccall.Value
}

type Values []*Value

func (v Values) raw() ccall.Values {
	values := ccall.Values{}
	for _, vv := range v {
		values = append(values, vv.Value)
	}
	return values
}

func toValues(v ccall.Values) Values {
	values := Values{}
	for _, vv := range v {
		values = append(values, toValue(vv))
	}
	return values
}

func toValue(c *ccall.Value) *Value {
	return &Value{c}
}

func (v *Value) IsTemporary() bool {
	return v.Value.IsTemporary()
}

func (v *Value) IsLocal() bool {
	return v.Value.IsLocal()
}

func (v *Value) IsConstant() bool {
	return v.Value.IsConstant()
}

func (v *Value) IsParameter() bool {
	return v.Value.IsParameter()
}

func (v *Value) SetVolatile() {
	v.Value.SetVolatile()
}

func (v *Value) IsVolatile() bool {
	return v.Value.IsVolatile()
}

func (v *Value) SetAddressable() {
	v.Value.SetAddressable()
}

func (v *Value) IsAddressable() bool {
	return v.Value.IsAddressable()
}

func (v *Value) Type() *Type {
	return toType(v.Value.Type())
}

func (v *Value) Function() *Function {
	return toFunction(v.Value.Function())
}

func (v *Value) Block() *Block {
	return toBlock(v.Value.Block())
}

func (v *Value) Context() *Context {
	return toContext(v.Value.Context())
}

//func (v *Value) Constant() *Constant {
//	return toConstant(C.jit_value_get_constant(v.c))
//}

func (v *Value) Int() int {
	return v.Value.Int()
}

func (v *Value) Float32() float32 {
	return v.Value.Float32()
}

func (v *Value) Float64() float64 {
	return v.Value.Float64()
}

func (v *Value) IsTrue() bool {
	return v.Value.IsTrue()
}
