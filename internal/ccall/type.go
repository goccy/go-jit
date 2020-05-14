package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>
*/
import "C"
import (
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"reflect"
	"unsafe"
)

type Type struct {
	c C.jit_type_t
}

type Types []*Type

func (t Types) c() *C.jit_type_t {
	types := make([]C.jit_type_t, len(t))
	for idx, tt := range t {
		types[idx] = tt.c
	}
	return (*C.jit_type_t)(unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&types)).Data))
}

// Type kinds that may be returned by Kind().
var (
	JIT_TYPE_INVALID       = C.JIT_TYPE_INVALID
	JIT_TYPE_VOID          = C.JIT_TYPE_VOID
	JIT_TYPE_SBYTE         = C.JIT_TYPE_SBYTE
	JIT_TYPE_UBYTE         = C.JIT_TYPE_UBYTE
	JIT_TYPE_SHORT         = C.JIT_TYPE_SHORT
	JIT_TYPE_USHORT        = C.JIT_TYPE_USHORT
	JIT_TYPE_INT           = C.JIT_TYPE_INT
	JIT_TYPE_UINT          = C.JIT_TYPE_UINT
	JIT_TYPE_NINT          = C.JIT_TYPE_NINT
	JIT_TYPE_NUINT         = C.JIT_TYPE_NUINT
	JIT_TYPE_LONG          = C.JIT_TYPE_LONG
	JIT_TYPE_ULONG         = C.JIT_TYPE_ULONG
	JIT_TYPE_FLOAT32       = C.JIT_TYPE_FLOAT32
	JIT_TYPE_FLOAT64       = C.JIT_TYPE_FLOAT64
	JIT_TYPE_NFLOAT        = C.JIT_TYPE_NFLOAT
	JIT_TYPE_MAX_PRIMITIVE = JIT_TYPE_NFLOAT
	JIT_TYPE_STRUCT        = C.JIT_TYPE_STRUCT
	JIT_TYPE_UNION         = C.JIT_TYPE_UNION
	JIT_TYPE_SIGNATURE     = C.JIT_TYPE_SIGNATURE
	JIT_TYPE_PTR           = C.JIT_TYPE_PTR
	JIT_TYPE_FIRST_TAGGED  = C.JIT_TYPE_FIRST_TAGGED
)

// Special tag types.
var (
	JIT_TYPETAG_NAME           = C.JIT_TYPETAG_NAME
	JIT_TYPETAG_STRUCT_NAME    = C.JIT_TYPETAG_STRUCT_NAME
	JIT_TYPETAG_UNION_NAME     = C.JIT_TYPETAG_UNION_NAME
	JIT_TYPETAG_ENUM_NAME      = C.JIT_TYPETAG_ENUM_NAME
	JIT_TYPETAG_CONST          = C.JIT_TYPETAG_CONST
	JIT_TYPETAG_VOLATILE       = C.JIT_TYPETAG_VOLATILE
	JIT_TYPETAG_REFERENCE      = C.JIT_TYPETAG_REFERENCE
	JIT_TYPETAG_OUTPUT         = C.JIT_TYPETAG_OUTPUT
	JIT_TYPETAG_RESTRICT       = C.JIT_TYPETAG_RESTRICT
	JIT_TYPETAG_SYS_BOOL       = C.JIT_TYPETAG_SYS_BOOL
	JIT_TYPETAG_SYS_CHAR       = C.JIT_TYPETAG_SYS_CHAR
	JIT_TYPETAG_SYS_SCHAR      = C.JIT_TYPETAG_SYS_SCHAR
	JIT_TYPETAG_SYS_UCHAR      = C.JIT_TYPETAG_SYS_UCHAR
	JIT_TYPETAG_SYS_SHORT      = C.JIT_TYPETAG_SYS_SHORT
	JIT_TYPETAG_SYS_USHORT     = C.JIT_TYPETAG_SYS_USHORT
	JIT_TYPETAG_SYS_INT        = C.JIT_TYPETAG_SYS_INT
	JIT_TYPETAG_SYS_UINT       = C.JIT_TYPETAG_SYS_UINT
	JIT_TYPETAG_SYS_LONG       = C.JIT_TYPETAG_SYS_LONG
	JIT_TYPETAG_SYS_ULONG      = C.JIT_TYPETAG_SYS_ULONG
	JIT_TYPETAG_SYS_LONGLONG   = C.JIT_TYPETAG_SYS_LONGLONG
	JIT_TYPETAG_SYS_ULONGLONG  = C.JIT_TYPETAG_SYS_ULONGLONG
	JIT_TYPETAG_SYS_FLOAT      = C.JIT_TYPETAG_SYS_FLOAT
	JIT_TYPETAG_SYS_DOUBLE     = C.JIT_TYPETAG_SYS_DOUBLE
	JIT_TYPETAG_SYS_LONGDOUBLE = C.JIT_TYPETAG_SYS_LONGDOUBLE
)

var (
	ptrsize         = uint(unsafe.Sizeof(unsafe.Pointer(nil)))
	TypeInt         = &Type{C.jit_type_int}
	TypeVoidPtr     = &Type{C.jit_type_void_ptr}
	TypeCharPtr     = &Type{C.jit_type_create_pointer(C.jit_type_sys_char, 0)}
	TypeGoInt       = &Type{C.jit_type_sys_longlong}
	TypeGoString    = CreateStruct([]*Type{TypeCharPtr, TypeInt}, 0)
	TypeGoInterface = CreateStruct([]*Type{TypeVoidPtr, TypeVoidPtr}, 0)
	TypeVoid        = &Type{C.jit_type_void}
	TypeFloat32     = &Type{C.jit_type_float32}
	TypeFloat64     = &Type{C.jit_type_float64}
)

func toType(c C.jit_type_t) *Type {
	return &Type{c}
}

func ReflectTypeToType(typ reflect.Type) *Type {
	switch typ.Kind() {
	case reflect.Int:
		return TypeGoInt
	case reflect.String:
		return TypeGoString
	case reflect.Interface:
		return TypeGoInterface
	}
	fmt.Println("unknown type = ", typ)
	return TypeVoid
}

func CreateStruct(fields Types, incref int) *Type {
	return toType(C.jit_type_create_struct(fields.c(), C.uint(len(fields)), C.int(incref)))
}

func CreateUnion(fields Types, incref int) *Type {
	return toType(C.jit_type_create_union(fields.c(), C.uint(len(fields)), C.int(incref)))
}

func CreateSignature(args Types, rtype *Type) *Type {
	if rtype == nil {
		rtype = TypeVoid
	}
	return toType(C.jit_type_create_signature(C.jit_abi_cdecl, rtype.c, args.c(), C.uint(len(args)), 0))
}

func BestAlignment() uint {
	return uint(C.jit_type_best_alignment())
}

func (t *Type) Copy() *Type {
	return toType(C.jit_type_copy(t.c))
}

func (t *Type) Free() {
	C.jit_type_free(t.c)
}

func (t *Type) CreatePointer(incref int) *Type {
	return toType(C.jit_type_create_pointer(t.c, C.int(incref)))
}

func (t *Type) SetSizeAndAlignment(size, alignment int) {
	C.jit_type_set_size_and_alignment(t.c, C.jit_nint(size), C.jit_nint(alignment))
}

func (t *Type) SetOffset(fieldIndex, offset uint) {
	C.jit_type_set_offset(t.c, C.uint(fieldIndex), C.jit_nuint(offset))
}

func (t *Type) Kind() int {
	return int(C.jit_type_get_kind(t.c))
}

func (t *Type) Size() uint {
	return uint(C.jit_type_get_size(t.c))
}

func (t *Type) Alignment() uint {
	return uint(C.jit_type_get_alignment(t.c))
}

func (t *Type) NumFields() uint {
	return uint(C.jit_type_num_fields(t.c))
}

func (t *Type) Field(index uint) *Type {
	return toType(C.jit_type_get_field(t.c, C.uint(index)))
}

func (t *Type) Offset(index uint) uint {
	return uint(C.jit_type_get_offset(t.c, C.uint(index)))
}

func (t *Type) Name(index uint) string {
	return C.GoString(C.jit_type_get_name(t.c, C.uint(index)))
}

func (t *Type) FindName(name string) uint {
	return uint(C.jit_type_find_name(t.c, C.CString(name)))
}

func (t *Type) NumParams() uint {
	return uint(C.jit_type_num_params(t.c))
}

func (t *Type) Return() *Type {
	return toType(C.jit_type_get_return(t.c))
}

func (t *Type) Param(index uint) *Type {
	return toType(C.jit_type_get_param(t.c, C.uint(index)))
}

func (t *Type) Ref() *Type {
	return toType(C.jit_type_get_ref(t.c))
}

func (t *Type) TaggedType() *Type {
	return toType(C.jit_type_get_tagged_type(t.c))
}

func (t *Type) SetTaggedType(underlying *Type, incref int) {
	C.jit_type_set_tagged_type(t.c, underlying.c, C.int(incref))
}

func (t *Type) TaggedKind() int {
	return int(C.jit_type_get_tagged_kind(t.c))
}

func (t *Type) TaggedData() unsafe.Pointer {
	return C.jit_type_get_tagged_data(t.c)
}

func (t *Type) IsPrimitive() bool {
	return int(C.jit_type_is_primitive(t.c)) == 1
}

func (t *Type) IsStruct() bool {
	return int(C.jit_type_is_struct(t.c)) == 1
}

func (t *Type) IsUnion() bool {
	return int(C.jit_type_is_union(t.c)) == 1
}

func (t *Type) IsSignature() bool {
	return int(C.jit_type_is_signature(t.c)) == 1
}

func (t *Type) IsPointer() bool {
	return int(C.jit_type_is_pointer(t.c)) == 1
}

func (t *Type) IsTagged() bool {
	return int(C.jit_type_is_tagged(t.c)) == 1
}

func (t *Type) RemoveTags() *Type {
	return toType(C.jit_type_remove_tags(t.c))
}

func (t *Type) Normalize() *Type {
	return toType(C.jit_type_normalize(t.c))
}

func (t *Type) PromoteInt() *Type {
	return toType(C.jit_type_promote_int(t.c))
}

func (t *Type) ReturnViaPointer() int {
	return int(C.jit_type_return_via_pointer(t.c))
}

func (t *Type) HasTag(kind int) bool {
	return int(C.jit_type_has_tag(t.c, C.int(kind))) == 1
}

func (t *Type) Dump(w io.Writer) error {
	tmpfile, err := ioutil.TempFile("", "")
	if err != nil {
		return err
	}
	defer os.Remove(tmpfile.Name())
	file := C.fdopen(C.int(tmpfile.Fd()), C.CString("w"))
	C.jit_dump_type(file, t.c)
	C.fclose(file)
	contents, err := ioutil.ReadFile(tmpfile.Name())
	if err != nil {
		return err
	}
	if _, err := w.Write(contents); err != nil {
		return err
	}
	return nil
}
