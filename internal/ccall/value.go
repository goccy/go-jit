package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>
*/
import "C"
import (
	"reflect"
	"unsafe"
)

type Value struct {
	c C.jit_value_t
}

type Values []*Value

func (v Values) c() *C.jit_value_t {
	cvalues := make([]C.jit_value_t, len(v))
	for idx, vv := range v {
		cvalues[idx] = vv.c
	}
	return (*C.jit_value_t)(unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&cvalues)).Data))
}

func toValue(c C.jit_value_t) *Value {
	return &Value{c}
}

func (v *Value) IsTemporary() bool {
	return int(C.jit_value_is_temporary(v.c)) == 1
}

func (v *Value) IsLocal() bool {
	return int(C.jit_value_is_local(v.c)) == 1
}

func (v *Value) IsConstant() bool {
	return int(C.jit_value_is_constant(v.c)) == 1
}

func (v *Value) IsParameter() bool {
	return int(C.jit_value_is_parameter(v.c)) == 1
}

func (v *Value) SetVolatile() {
	C.jit_value_set_volatile(v.c)
}

func (v *Value) IsVolatile() bool {
	return int(C.jit_value_is_volatile(v.c)) == 1
}

func (v *Value) SetAddressable() {
	C.jit_value_set_addressable(v.c)
}

func (v *Value) IsAddressable() bool {
	return int(C.jit_value_is_addressable(v.c)) == 1
}

func (v *Value) Type() *Type {
	return toType(C.jit_value_get_type(v.c))
}

func (v *Value) Function() *Function {
	return toFunction(C.jit_value_get_function(v.c))
}

func (v *Value) Block() *Block {
	return toBlock(C.jit_value_get_block(v.c))
}

func (v *Value) Context() *Context {
	return toContext(C.jit_value_get_context(v.c))
}

//func (v *Value) Constant() *Constant {
//	return toConstant(C.jit_value_get_constant(v.c))
//}

func (v *Value) Int() int {
	return int(C.jit_value_get_nint_constant(v.c))
}

func (v *Value) Long() int64 {
	return int64(C.jit_value_get_long_constant(v.c))
}

func (v *Value) Float32() float32 {
	return float32(C.jit_value_get_float32_constant(v.c))
}

func (v *Value) Float64() float64 {
	return float64(C.jit_value_get_float64_constant(v.c))
}

func (v *Value) IsTrue() bool {
	return int(C.jit_value_is_true(v.c)) == 1
}
