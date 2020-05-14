package jit

import (
	"github.com/goccy/go-jit/internal/ccall"
)

type Label struct {
	*ccall.Label
}

type Labels []*Label

func (l Labels) raw() ccall.Labels {
	labels := ccall.Labels{}
	for _, ll := range l {
		labels = append(labels, ll.Label)
	}
	return labels
}

func toLabel(c *ccall.Label) *Label {
	return &Label{c}
}
