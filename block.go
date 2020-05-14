package jit

import (
	"github.com/goccy/go-jit/internal/ccall"
)

type Block struct {
	*ccall.Block
}

func toBlock(c *ccall.Block) *Block {
	return &Block{c}
}

func (b *Block) Function() *Function {
	return toFunction(b.Block.Function())
}

func (b *Block) Context() *Context {
	return toContext(b.Block.Context())
}

func (b *Block) Label() *Label {
	return toLabel(b.Block.Label())
}

func (b *Block) NextLabel(label *Label) *Label {
	return toLabel(b.Block.NextLabel(label.Label))
}

func (b *Block) IsReachable() bool {
	return b.Block.IsReachable()
}

func (b *Block) EndsInDead() bool {
	return b.Block.EndsInDead()
}
