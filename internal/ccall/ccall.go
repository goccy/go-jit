package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>
*/
import "C"

var (
	JIT_OPTLEVEL_NONE   = C.JIT_OPTLEVEL_NONE
	JIT_OPTLEVEL_NORMAL = C.JIT_OPTLEVEL_NORMAL
	JIT_NO_OFFSET       = C.JIT_NO_OFFSET
)
