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

type Label struct {
	c C.jit_label_t
}

var (
	UndefinedLabel = &Label{C.jit_label_undefined}
)

type Labels []*Label

func (l Labels) c() *C.jit_label_t {
	clabels := make([]C.jit_label_t, len(l))
	for idx, label := range l {
		clabels[idx] = label.c
	}
	return (*C.jit_label_t)(unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&clabels)).Data))
}

func toLabel(c C.jit_label_t) *Label {
	return &Label{c}
}
