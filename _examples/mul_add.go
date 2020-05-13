package main

import (
	"fmt"

	"github.com/goccy/go-jit"
)

// func f(x, y, z int) int {
//   temp1 := x * y
//   temp2 := temp1 + z
//   return temp2
// }

func main() {
	ctx := jit.NewContext()
	defer ctx.Close()
	f, err := ctx.Build(func(ctx *jit.Context) (*jit.Function, error) {
		f := ctx.CreateFunction([]*jit.Type{jit.TypeInt, jit.TypeInt, jit.TypeInt}, jit.TypeInt)
		x := f.Param(0)
		y := f.Param(1)
		z := f.Param(2)
		temp1 := f.Mul(x, y)
		temp2 := f.Add(temp1, z)
		f.Return(temp2)
		f.Compile()
		return f, nil
	})
	if err != nil {
		panic(err)
	}
	fmt.Println("result = ", f.Run(2, 3, 4))
}
