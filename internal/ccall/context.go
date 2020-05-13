package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>

extern void *get_crosscall2addr();
extern void *get_cgo_wait_runtime_init_done_addr();
*/
import "C"

var (
	JIT_OPTION_CACHE_LIMIT           = C.JIT_OPTION_CACHE_LIMIT
	JIT_OPTION_CACHE_PAGE_SIZE       = C.JIT_OPTION_CACHE_PAGE_SIZE
	JIT_OPTION_PRE_COMPILE           = C.JIT_OPTION_PRE_COMPILE
	JIT_OPTION_DONT_FOLD             = C.JIT_OPTION_DONT_FOLD
	JIT_OPTION_POSITION_INDEPENDENT  = C.JIT_OPTION_POSITION_INDEPENDENT
	JIT_OPTION_CACHE_MAX_PAGE_FACTOR = C.JIT_OPTION_CACHE_MAX_PAGE_FACTOR
)

type Context struct {
	c                          C.jit_context_t
	crosscall2                 *Function
	cgo_wait_runtime_init_done *Function
}

func toContext(c C.jit_context_t) *Context {
	return &Context{c: c}
}

func CreateContext() *Context {
	ctx := toContext(C.jit_context_create())
	ctx.crosscall2 = ctx.createCrossCall2()
	ctx.cgo_wait_runtime_init_done = ctx.createCgoWaitRuntimeInitDone()
	return ctx
}

func (c *Context) Destroy() {
	C.jit_context_destroy(c.c)
}

func (c *Context) BuildStart() {
	C.jit_context_build_start(c.c)
}

func (c *Context) BuildEnd() {
	C.jit_context_build_end(c.c)
}

func (c *Context) CreateFunction(signature *Type) *Function {
	fn := toFunction(C.jit_function_create(c.c, signature.c))
	fn.crosscall2 = c.crosscall2
	fn.cgo_wait_runtime_init_done = c.cgo_wait_runtime_init_done
	return fn
}

func (c *Context) CreateNestedFunction(signature *Type, parent *Function) *Function {
	return toFunction(C.jit_function_create_nested(c.c, signature.c, parent.c))
}

func (c *Context) createCrossCall2() *Function {
	sig := CreateSignature([]*Type{TypeVoidPtr, TypeVoidPtr, TypeInt, TypeInt}, nil)
	fn := c.CreateFunction(sig)
	defer sig.Free()
	C.jit_function_setup_entry(fn.c, C.get_crosscall2addr())
	return fn
}

func (c *Context) createCgoWaitRuntimeInitDone() *Function {
	sig := CreateSignature(nil, TypeInt)
	fn := c.CreateFunction(sig)
	defer sig.Free()
	C.jit_function_setup_entry(fn.c, C.get_cgo_wait_runtime_init_done_addr())
	return fn
}
