package jit

import (
	"github.com/goccy/go-jit/internal/ccall"
)

var (
	TypeInt = &Type{ccall.TypeGoInt}
)

type Context struct {
	*ccall.Context
}

type Function struct {
	*ccall.Function
}

type Type struct {
	*ccall.Type
}

type Value struct {
	*ccall.Value
}

func NewContext() *Context {
	return &Context{ccall.CreateContext()}
}

func (c *Context) Close() {
	c.Destroy()
}

func (c *Context) Build(cb func(*Context) (*Function, error)) (*Function, error) {
	c.BuildStart()
	fn, err := cb(c)
	if err != nil {
		return nil, err
	}
	c.BuildEnd()
	return fn, nil
}

func createSignature(argtypes []*Type, rtype *Type) *Type {
	args := []*ccall.Type{}
	for _, argtype := range argtypes {
		args = append(args, argtype.Type)
	}
	return &Type{ccall.CreateSignature(args, rtype.Type)}
}

func (c *Context) CreateFunction(argtypes []*Type, rtype *Type) *Function {
	sig := createSignature(argtypes, rtype)
	defer sig.Free()
	return &Function{c.Context.CreateFunction(sig.Type)}
}

func toCCALLValues(v []*Value) []*ccall.Value {
	values := []*ccall.Value{}
	for _, e := range v {
		values = append(values, e.Value)
	}
	return values
}

func toValues(v []*ccall.Value) []*Value {
	values := []*Value{}
	for _, e := range v {
		values = append(values, &Value{e})
	}
	return values
}

func (f *Function) GoCall(fn interface{}, args []*Value) ([]*Value, error) {
	values, err := f.Function.GoCall(fn, toCCALLValues(args))
	if err != nil {
		return nil, err
	}
	return toValues(values), nil
}

func (f *Function) Ret(v *Value) {
	f.Function.Return(v.Value)
}

func (f *Function) Compile() {
	f.Function.Compile()
}

func (f *Function) Run(args ...interface{}) interface{} {
	return f.Function.Apply(args)
}

func (f *Function) CreateIntValue(v int) *Value {
	return &Value{f.Function.CreateIntValue(v)}
}

func (f *Function) GetParam(v uint) *Value {
	return &Value{f.Function.Param(v)}
}

func (f *Function) Mul(x, y *Value) *Value {
	return &Value{f.Function.Mul(x.Value, y.Value)}
}

func (f *Function) Add(x, y *Value) *Value {
	return &Value{f.Function.Add(x.Value, y.Value)}
}
