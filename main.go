package main

import (
	"fmt"

	"github.com/goccy/go-jit/internal/ccall"
)

func main() {
	ctx := ccall.CreateContext()
	ctx.BuildStart()
	args := []*ccall.Type{
		ccall.TypeInt, ccall.TypeInt, ccall.TypeInt,
	}
	signature := ccall.CreateSignature(args, ccall.TypeInt)
	function := ctx.CreateFunction(signature)
	defer ccall.FreeType(signature)

	x := function.GetParam(0)
	y := function.GetParam(1)
	z := function.GetParam(2)
	temp1 := function.Mul(x, y)
	temp2 := function.Add(temp1, z)
	function.Ret(temp2)
	function.Compile()
	ctx.BuildEnd()

	funcArgs := []interface{}{3, 5, 2}
	fmt.Println("mul_add(3, 5, 2) = ", function.Apply(funcArgs))

	ctx.Destroy()
}
