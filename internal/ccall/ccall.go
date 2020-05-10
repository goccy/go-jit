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

type Context struct {
	c C.jit_context_t
}

type Type struct {
	c C.jit_type_t
}

type Function struct {
	c C.jit_function_t
}

type Value struct {
	c C.jit_value_t
}

var (
	TypeInt = &Type{C.jit_type_int}
)

func toContext(c C.jit_context_t) *Context {
	return &Context{c}
}

func toType(c C.jit_type_t) *Type {
	return &Type{c}
}

func toFunction(c C.jit_function_t) *Function {
	return &Function{c}
}

func toValue(c C.jit_value_t) *Value {
	return &Value{c}
}

func CreateContext() *Context {
	return toContext(C.jit_context_create())
}

func (c *Context) BuildStart() {
	C.jit_context_build_start(c.c)
}

func (c *Context) BuildEnd() {
	C.jit_context_build_end(c.c)
}

func (c *Context) Destroy() {
	C.jit_context_destroy(c.c)
}

func (c *Context) CreateFunction(signature *Type) *Function {
	return toFunction(C.jit_function_create(c.c, signature.c))
}

func CreateSignature(args []*Type, rtype *Type) *Type {
	retType := C.jit_type_void
	retCount := 0
	if rtype != nil {
		retType = rtype.c
		retCount = 1
	}
	params := make([]C.jit_type_t, len(args))
	for idx, arg := range args {
		params[idx] = arg.c
	}
	cparams := (*C.jit_type_t)(unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&params)).Data))
	return toType(C.jit_type_create_signature(C.jit_abi_cdecl, retType, cparams, C.uint(len(args)), C.int(retCount)))
}

func FreeType(typ *Type) {
	C.jit_type_free(typ.c)
}

func (f *Function) GetParam(p uint) *Value {
	return toValue(C.jit_value_get_param(f.c, C.uint(p)))
}

func (f *Function) Mul(x, y *Value) *Value {
	return toValue(C.jit_insn_mul(f.c, x.c, y.c))
}

func (f *Function) Add(x, y *Value) *Value {
	return toValue(C.jit_insn_add(f.c, x.c, y.c))
}

func (f *Function) Ret(v *Value) {
	C.jit_insn_return(f.c, v.c)
}

func (f *Function) Compile() {
	C.jit_function_compile(f.c)
}

type interfaceHeader struct {
	typ unsafe.Pointer
	ptr unsafe.Pointer
}

func (f *Function) Apply(args []interface{}) interface{} {
	params := make([]unsafe.Pointer, len(args))
	for idx, arg := range args {
		header := (*interfaceHeader)(unsafe.Pointer(&arg))
		params[idx] = header.ptr
	}
	cparams := (*unsafe.Pointer)(unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&params)).Data))
	var result C.jit_int
	C.jit_function_apply(f.c, cparams, unsafe.Pointer(&result))
	return int(result)
}
