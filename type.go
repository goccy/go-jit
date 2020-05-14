package jit

import (
	"io"
	"unsafe"

	"github.com/goccy/go-jit/internal/ccall"
)

type Type struct {
	*ccall.Type
}

type Types []*Type

func (t Types) raw() ccall.Types {
	types := ccall.Types{}
	for _, tt := range t {
		types = append(types, tt.Type)
	}
	return types
}

func toType(raw *ccall.Type) *Type {
	return &Type{raw}
}

func CreateStruct(fields Types, incref int) *Type {
	return toType(ccall.CreateStruct(fields.raw(), incref))
}

func CreateUnion(fields Types, incref int) *Type {
	return toType(ccall.CreateUnion(fields.raw(), incref))
}

func CreateSignature(args Types, rtype *Type) *Type {
	return toType(ccall.CreateSignature(args.raw(), rtype.Type))
}

func BestAlignment() uint {
	return ccall.BestAlignment()
}

func (t *Type) Copy() *Type {
	return toType(t.Type.Copy())
}

func (t *Type) Free() {
	t.Type.Free()
}

func (t *Type) CreatePointer(incref int) *Type {
	return toType(t.Type.CreatePointer(incref))
}

func (t *Type) SetSizeAndAlignment(size, alignment int) {
	t.Type.SetSizeAndAlignment(size, alignment)
}

func (t *Type) SetOffset(fieldIndex, offset uint) {
	t.Type.SetOffset(fieldIndex, offset)
}

func (t *Type) Kind() int {
	return t.Type.Kind()
}

func (t *Type) Size() uint {
	return t.Type.Size()
}

func (t *Type) Alignment() uint {
	return t.Type.Alignment()
}

func (t *Type) NumFields() uint {
	return t.Type.NumFields()
}

func (t *Type) Field(index uint) *Type {
	return toType(t.Type.Field(index))
}

func (t *Type) Offset(index uint) uint {
	return t.Type.Offset(index)
}

func (t *Type) Name(index uint) string {
	return t.Type.Name(index)
}

func (t *Type) FindName(name string) uint {
	return t.Type.FindName(name)
}

func (t *Type) NumParams() uint {
	return t.Type.NumParams()
}

func (t *Type) Return() *Type {
	return toType(t.Type.Return())
}

func (t *Type) Param(index uint) *Type {
	return toType(t.Type.Param(index))
}

func (t *Type) Ref() *Type {
	return toType(t.Type.Ref())
}

func (t *Type) TaggedType() *Type {
	return toType(t.Type.TaggedType())
}

func (t *Type) SetTaggedType(underlying *Type, incref int) {
	t.Type.SetTaggedType(underlying.Type, incref)
}

func (t *Type) TaggedKind() int {
	return t.Type.TaggedKind()
}

func (t *Type) TaggedData() unsafe.Pointer {
	return t.Type.TaggedData()
}

func (t *Type) IsPrimitive() bool {
	return t.Type.IsPrimitive()
}

func (t *Type) IsStruct() bool {
	return t.Type.IsStruct()
}

func (t *Type) IsUnion() bool {
	return t.Type.IsUnion()
}

func (t *Type) IsSignature() bool {
	return t.Type.IsSignature()
}

func (t *Type) IsPointer() bool {
	return t.Type.IsPointer()
}

func (t *Type) IsTagged() bool {
	return t.Type.IsTagged()
}

func (t *Type) RemoveTags() *Type {
	return toType(t.Type.RemoveTags())
}

func (t *Type) Normalize() *Type {
	return toType(t.Type.Normalize())
}

func (t *Type) PromoteInt() *Type {
	return toType(t.Type.PromoteInt())
}

func (t *Type) ReturnViaPointer() int {
	return t.Type.ReturnViaPointer()
}

func (t *Type) HasTag(kind int) bool {
	return t.Type.HasTag(kind)
}

func (t *Type) Dump(w io.Writer) error {
	return t.Type.Dump(w)
}
