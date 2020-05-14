package jit

import (
	"github.com/goccy/go-jit/internal/ccall"
)

var (
	TypeInt = &Type{ccall.TypeGoInt}
)
