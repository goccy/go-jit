# go-jit

[![GoDoc](https://godoc.org/github.com/goccy/go-jit?status.svg)](https://pkg.go.dev/github.com/goccy/go-jit?tab=doc)

JIT compile library for Go

# Status

WIP

# Synopsis

## Basic Operation

```go
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
```

## Call defined Go function during JIT runtime

```go
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
  fmt.Println("result = ", f.Run(nil))
}
```

# Installation

```
go get github.com/goccy/go-jit
```

