package main

import (
	"fmt"

	"github.com/goccy/go-jit"
)

// func f() int {
//   return callback(7, 8)
// }

func callback(i, j int) int {
	fmt.Printf("callback: i = %d j = %d\n", i, j)
	return i * j
}

func main() {
	ctx := jit.NewContext()
	defer ctx.Close()
	f, err := ctx.Build(func(ctx *jit.Context) (*jit.Function, error) {
		f := ctx.CreateFunction(nil, jit.TypeInt)
		rvalues, err := f.GoCall(callback, []*jit.Value{
			f.CreateIntValue(7), f.CreateIntValue(8),
		})
		if err != nil {
			return nil, err
		}
		f.Return(rvalues[0])
		f.Compile()
		return f, nil
	})
	if err != nil {
		panic(err)
	}
	fmt.Println("result = ", f.Run())
}
