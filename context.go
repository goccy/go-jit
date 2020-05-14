package jit

import (
	"github.com/goccy/go-jit/internal/ccall"
)

type Context struct {
	*ccall.Context
}

func NewContext() *Context {
	return &Context{ccall.CreateContext()}
}

func toContext(c *ccall.Context) *Context {
	return &Context{c}
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

func (c *Context) CreateFunction(argtypes Types, rtype *Type) *Function {
	signature := CreateSignature(argtypes, rtype)
	defer signature.Free()
	return &Function{c.Context.CreateFunction(signature.Type)}
}

func (c *Context) CreateNestedFunction(signature *Type, parent *Function) *Function {
	return toFunction(c.Context.CreateNestedFunction(signature.Type, parent.Function))
}
