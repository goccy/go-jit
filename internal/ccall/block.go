package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>
*/
import "C"

type Block struct {
	c C.jit_block_t
}

func toBlock(c C.jit_block_t) *Block {
	return &Block{c}
}

func (b *Block) Function() *Function {
	return toFunction(C.jit_block_get_function(b.c))
}

func (b *Block) Context() *Context {
	return toContext(C.jit_block_get_context(b.c))
}

func (b *Block) Label() *Label {
	return toLabel(C.jit_block_get_label(b.c))
}

func (b *Block) NextLabel(label *Label) *Label {
	return toLabel(C.jit_block_get_next_label(b.c, label.c))
}

func (b *Block) IsReachable() bool {
	return int(C.jit_block_is_reachable(b.c)) == 1
}

func (b *Block) EndsInDead() bool {
	return int(C.jit_block_ends_in_dead(b.c)) == 1
}
