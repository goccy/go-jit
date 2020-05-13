package ccall

/*
#cgo CFLAGS: -I../
#cgo CFLAGS: -Iinclude

#include <jit/jit.h>
*/
import "C"
import (
	"errors"
	"io"
	"io/ioutil"
	"os"
	"reflect"
	"unsafe"
)

type Function struct {
	c                          C.jit_function_t
	name                       string
	crosscall2                 *Function
	cgo_wait_runtime_init_done *Function
}

func toFunction(c C.jit_function_t) *Function {
	return &Function{c: c}
}

func (f *Function) Label(label *Label) bool {
	return int(C.jit_insn_label(f.c, &label.c)) == 1
}

func (f *Function) LabelRight(label *Label) bool {
	return int(C.jit_insn_label_tight(f.c, &label.c)) == 1
}

func (f *Function) NewBlock(label *Label) bool {
	return int(C.jit_insn_new_block(f.c)) == 1
}

func (f *Function) Load(value *Value) *Value {
	return toValue(C.jit_insn_load(f.c, value.c))
}

func (f *Function) Dup(value *Value) *Value {
	return toValue(C.jit_insn_dup(f.c, value.c))
}

func (f *Function) Store(dest, value *Value) bool {
	return int(C.jit_insn_store(f.c, dest.c, value.c)) == 1
}

func (f *Function) LoadRelative(value *Value, offset int, typ *Type) *Value {
	return toValue(C.jit_insn_load_relative(f.c, value.c, C.jit_nint(offset), typ.c))
}

func (f *Function) StoreRelative(dest *Value, offset int, value *Value) bool {
	return int(C.jit_insn_store_relative(f.c, dest.c, C.jit_nint(offset), value.c)) == 1
}

func (f *Function) AddRelative(value *Value, offset int) *Value {
	return toValue(C.jit_insn_add_relative(f.c, value.c, C.jit_nint(offset)))
}

func (f *Function) LoadElem(baseAddr *Value, index *Value, elemType *Type) *Value {
	return toValue(C.jit_insn_load_elem(f.c, baseAddr.c, index.c, elemType.c))
}

func (f *Function) LoadElemAddress(baseAddr *Value, index *Value, elemType *Type) *Value {
	return toValue(C.jit_insn_load_elem_address(f.c, baseAddr.c, index.c, elemType.c))
}

func (f *Function) StoreElem(baseAddr, index, value *Value) bool {
	return int(C.jit_insn_store_elem(f.c, baseAddr.c, index.c, value.c)) == 1
}

func (f *Function) CheckNull(value *Value) bool {
	return int(C.jit_insn_check_null(f.c, value.c)) == 1
}

func (f *Function) Nop() bool {
	return int(C.jit_insn_nop(f.c)) == 1
}

func (f *Function) Add(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_add(f.c, value1.c, value2.c))
}

func (f *Function) AddOvf(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_add_ovf(f.c, value1.c, value2.c))
}

func (f *Function) Sub(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_sub(f.c, value1.c, value2.c))
}

func (f *Function) SubOvf(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_sub_ovf(f.c, value1.c, value2.c))
}

func (f *Function) Mul(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_mul(f.c, value1.c, value2.c))
}

func (f *Function) MulOvf(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_mul_ovf(f.c, value1.c, value2.c))
}

func (f *Function) Div(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_div(f.c, value1.c, value2.c))
}

func (f *Function) Rem(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_rem(f.c, value1.c, value2.c))
}

func (f *Function) RemIEEE(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_rem_ieee(f.c, value1.c, value2.c))
}

func (f *Function) Neg(value1 *Value) *Value {
	return toValue(C.jit_insn_neg(f.c, value1.c))
}

func (f *Function) And(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_and(f.c, value1.c, value2.c))
}

func (f *Function) Or(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_or(f.c, value1.c, value2.c))
}

func (f *Function) Xor(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_xor(f.c, value1.c, value2.c))
}

func (f *Function) Not(value1 *Value) *Value {
	return toValue(C.jit_insn_not(f.c, value1.c))
}

func (f *Function) Shl(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_shl(f.c, value1.c, value2.c))
}

func (f *Function) Shr(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_shr(f.c, value1.c, value2.c))
}

func (f *Function) Ushr(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_ushr(f.c, value1.c, value2.c))
}

func (f *Function) Sshr(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_sshr(f.c, value1.c, value2.c))
}

func (f *Function) Eq(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_eq(f.c, value1.c, value2.c))
}

func (f *Function) Ne(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_ne(f.c, value1.c, value2.c))
}

func (f *Function) Lt(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_lt(f.c, value1.c, value2.c))
}

func (f *Function) Le(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_le(f.c, value1.c, value2.c))
}

func (f *Function) Gt(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_gt(f.c, value1.c, value2.c))
}

func (f *Function) Ge(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_ge(f.c, value1.c, value2.c))
}

func (f *Function) Cmpl(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_cmpl(f.c, value1.c, value2.c))
}

func (f *Function) Cmpg(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_cmpg(f.c, value1.c, value2.c))
}

func (f *Function) ToBool(value1 *Value) *Value {
	return toValue(C.jit_insn_to_bool(f.c, value1.c))
}

func (f *Function) ToNotBool(value1 *Value) *Value {
	return toValue(C.jit_insn_to_not_bool(f.c, value1.c))
}

func (f *Function) Acos(value1 *Value) *Value {
	return toValue(C.jit_insn_acos(f.c, value1.c))
}

func (f *Function) Asin(value1 *Value) *Value {
	return toValue(C.jit_insn_asin(f.c, value1.c))
}

func (f *Function) Atan(value1 *Value) *Value {
	return toValue(C.jit_insn_atan(f.c, value1.c))
}

func (f *Function) Atan2(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_atan2(f.c, value1.c, value2.c))
}

func (f *Function) Ceil(value1 *Value) *Value {
	return toValue(C.jit_insn_ceil(f.c, value1.c))
}

func (f *Function) Cos(value1 *Value) *Value {
	return toValue(C.jit_insn_cos(f.c, value1.c))
}

func (f *Function) Cosh(value1 *Value) *Value {
	return toValue(C.jit_insn_cosh(f.c, value1.c))
}

func (f *Function) Exp(value1 *Value) *Value {
	return toValue(C.jit_insn_exp(f.c, value1.c))
}

func (f *Function) Floor(value1 *Value) *Value {
	return toValue(C.jit_insn_floor(f.c, value1.c))
}

func (f *Function) Log(value1 *Value) *Value {
	return toValue(C.jit_insn_log(f.c, value1.c))
}

func (f *Function) Log10(value1 *Value) *Value {
	return toValue(C.jit_insn_log10(f.c, value1.c))
}

func (f *Function) Pow(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_pow(f.c, value1.c, value2.c))
}

func (f *Function) Rint(value1 *Value) *Value {
	return toValue(C.jit_insn_rint(f.c, value1.c))
}

func (f *Function) Round(value1 *Value) *Value {
	return toValue(C.jit_insn_round(f.c, value1.c))
}

func (f *Function) Sin(value1 *Value) *Value {
	return toValue(C.jit_insn_sin(f.c, value1.c))
}

func (f *Function) Sinh(value1 *Value) *Value {
	return toValue(C.jit_insn_sinh(f.c, value1.c))
}

func (f *Function) Sqrt(value1 *Value) *Value {
	return toValue(C.jit_insn_sqrt(f.c, value1.c))
}

func (f *Function) Tan(value1 *Value) *Value {
	return toValue(C.jit_insn_tan(f.c, value1.c))
}

func (f *Function) Tanh(value1 *Value) *Value {
	return toValue(C.jit_insn_tanh(f.c, value1.c))
}

func (f *Function) Trunc(value1 *Value) *Value {
	return toValue(C.jit_insn_trunc(f.c, value1.c))
}

func (f *Function) IsNan(value1 *Value) *Value {
	return toValue(C.jit_insn_is_nan(f.c, value1.c))
}

func (f *Function) IsFinite(value1 *Value) *Value {
	return toValue(C.jit_insn_is_finite(f.c, value1.c))
}

func (f *Function) IsInf(value1 *Value) *Value {
	return toValue(C.jit_insn_is_inf(f.c, value1.c))
}

func (f *Function) Abs(value1 *Value) *Value {
	return toValue(C.jit_insn_abs(f.c, value1.c))
}

func (f *Function) Min(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_min(f.c, value1.c, value2.c))
}

func (f *Function) Max(value1, value2 *Value) *Value {
	return toValue(C.jit_insn_max(f.c, value1.c, value2.c))
}

func (f *Function) Sign(value1 *Value) *Value {
	return toValue(C.jit_insn_sign(f.c, value1.c))
}

func (f *Function) Branch(label *Label) bool {
	return int(C.jit_insn_branch(f.c, &label.c)) == 1
}

func (f *Function) BranchIf(value *Value, label *Label) bool {
	return int(C.jit_insn_branch_if(f.c, value.c, &label.c)) == 1
}

func (f *Function) BranchIfNot(value *Value, label *Label) bool {
	return int(C.jit_insn_branch_if_not(f.c, value.c, &label.c)) == 1
}

func (f *Function) JumpTable(value *Value, labels Labels) bool {
	return int(C.jit_insn_jump_table(f.c, value.c, labels.c(), C.uint(len(labels)))) == 1
}

func (f *Function) AddressOf(value1 *Value) *Value {
	return toValue(C.jit_insn_address_of(f.c, value1.c))
}

func (f *Function) AddressOfLabel(label *Label) *Value {
	return toValue(C.jit_insn_address_of_label(f.c, &label.c))
}

func (f *Function) Convert(value *Value, typ *Type, overflowCheck int) *Value {
	return toValue(C.jit_insn_convert(f.c, value.c, typ.c, C.int(overflowCheck)))
}

func (f *Function) Call(name string, fn *Function, args Values) *Value {
	return toValue(C.jit_insn_call(f.c, C.CString(name), fn.c, nil, args.c(), C.uint(len(args)), C.JIT_CALL_NOTHROW))
}

func (f *Function) CallIndirect(fn *Function, value *Value, signature *Type, args Values) *Value {
	return toValue(C.jit_insn_call_indirect(f.c, value.c, signature.c, args.c(), C.uint(len(args)), C.JIT_CALL_NOTHROW))
}

func (f *Function) CallNestedIndirect(fn *Function, value *Value, parentFrame *Value, signature *Type, args Values) *Value {
	return toValue(C.jit_insn_call_nested_indirect(f.c, value.c, parentFrame.c, signature.c, args.c(), C.uint(len(args)), C.JIT_CALL_NOTHROW))
}

func (f *Function) CallIndirectVtable(fn *Function, value *Value, signature *Type, args Values) *Value {
	return toValue(C.jit_insn_call_indirect_vtable(f.c, value.c, signature.c, args.c(), C.uint(len(args)), C.JIT_CALL_NOTHROW))
}

func (f *Function) CallNative(fn *Function, name string, nativeFunc unsafe.Pointer, signature *Type, args Values) *Value {
	return toValue(C.jit_insn_call_native(f.c, C.CString(name), nativeFunc, signature.c, args.c(), C.uint(len(args)), C.JIT_CALL_NOTHROW))
}

//func (f *Function) CallIntrinsic(fn *Function, name string, intrinsicFunc unsafe.Pointer, desc *IntrinsicDescriptor, arg1, arg2 *Value) *Value {
//	return toValue(C.jit_insn_call_intrinsic(f.c, C.CString(name), intrinsicFunc, desc.c, arg1.c, arg2.c))
//}

func (f *Function) IncomingReg(value *Value, reg int) int {
	return int(C.jit_insn_incoming_reg(f.c, value.c, C.int(reg)))
}

func (f *Function) Return(value *Value) int {
	return int(C.jit_insn_return(f.c, value.c))
}

func (f *Function) ReturnPtr(value *Value, typ *Type) int {
	return int(C.jit_insn_return_ptr(f.c, value.c, typ.c))
}

func (f *Function) DefaultReturn() int {
	return int(C.jit_insn_default_return(f.c))
}

func (f *Function) Memcpy(dest, src, size *Value) int {
	return int(C.jit_insn_memcpy(f.c, dest.c, src.c, size.c))
}

func (f *Function) Memmove(dest, src, size *Value) int {
	return int(C.jit_insn_memmove(f.c, dest.c, src.c, size.c))
}

func (f *Function) Memset(dest, src, size *Value) int {
	return int(C.jit_insn_memset(f.c, dest.c, src.c, size.c))
}

func (f *Function) Alloca(size *Value) *Value {
	return toValue(C.jit_insn_alloca(f.c, size.c))
}

func (f *Function) MoveBlocksToStart(fromLabel *Label, toLabel *Label) int {
	return int(C.jit_insn_move_blocks_to_start(f.c, fromLabel.c, toLabel.c))
}

func (f *Function) MoveBlocksToEnd(fromLabel *Label, toLabel *Label) int {
	return int(C.jit_insn_move_blocks_to_end(f.c, fromLabel.c, toLabel.c))
}

func (f *Function) MarkOffset(offset int) int {
	return int(C.jit_insn_mark_offset(f.c, C.int(offset)))
}

func (f *Function) MarkBreakpoint(data1, data2 int) int {
	return int(C.jit_insn_mark_breakpoint(f.c, C.jit_nint(data1), C.jit_nint(data2)))
}

func (f *Function) MarkBreakpointVariable(data1, data2 *Value) int {
	return int(C.jit_insn_mark_breakpoint_variable(f.c, data1.c, data2.c))
}

func (f *Function) Abandon() {
	C.jit_function_abandon(f.c)
}

func (f *Function) Context() *Context {
	return toContext(C.jit_function_get_context(f.c))
}

func (f *Function) Signature() *Type {
	return toType(C.jit_function_get_signature(f.c))
}

func (f *Function) Meta(typ int) unsafe.Pointer {
	return C.jit_function_get_meta(f.c, C.int(typ))
}

func (f *Function) FreeMeta(typ int) {
	C.jit_function_free_meta(f.c, C.int(typ))
}

func (f *Function) Entry() *Block {
	return toBlock(C.jit_function_get_entry(f.c))
}

func (f *Function) Current() *Block {
	return toBlock(C.jit_function_get_current(f.c))
}

func (f *Function) NestedParent() *Function {
	return toFunction(C.jit_function_get_nested_parent(f.c))
}

func (f *Function) SetParentFrame(parentFrame *Value) {
	C.jit_function_set_parent_frame(f.c, parentFrame.c)
}

func (f *Function) Compile() bool {
	return int(C.jit_function_compile(f.c)) == 1
}

func (f *Function) IsCompiled() bool {
	return int(C.jit_function_is_compiled(f.c)) == 1
}

func (f *Function) SetRecompilable() {
	C.jit_function_set_recompilable(f.c)
}

func (f *Function) ClearRecompilable() {
	C.jit_function_clear_recompilable(f.c)
}

func (f *Function) IsRecompilable() bool {
	return int(C.jit_function_is_recompilable(f.c)) == 1
}

func (f *Function) SetupEntry(entryPoint unsafe.Pointer) {
	C.jit_function_setup_entry(f.c, entryPoint)
}

func (f *Function) ToClosure() unsafe.Pointer {
	return C.jit_function_to_closure(f.c)
}

func (f *Function) ToVtablePointer() unsafe.Pointer {
	return C.jit_function_to_vtable_pointer(f.c)
}

type interfaceHeader struct {
	typ unsafe.Pointer
	ptr unsafe.Pointer
}

func (f *Function) Apply(args []interface{}) interface{} {
	params := make([]unsafe.Pointer, len(args))
	for idx, arg := range args {
		header := (*interfaceHeader)(unsafe.Pointer(&arg))
		params[idx] = header.ptr
	}
	cparams := (*unsafe.Pointer)(unsafe.Pointer((*reflect.SliceHeader)(unsafe.Pointer(&params)).Data))
	var result int
	C.jit_function_apply(f.c, cparams, unsafe.Pointer(&result))
	return result
}

func (f *Function) SetOptimizationLevel(level uint) {
	C.jit_function_set_optimization_level(f.c, C.uint(level))
}

func (f *Function) OptimizationLevel() uint {
	return uint(C.jit_function_get_optimization_level(f.c))
}

func MaxOptimizationLevel() uint {
	return uint(C.jit_function_get_max_optimization_level())
}

func (f *Function) ReserveLabel() *Label {
	return toLabel(C.jit_function_reserve_label(f.c))
}

func (f *Function) LabelsEqual(label *Label, label2 *Label) bool {
	return int(C.jit_function_labels_equal(f.c, label.c, label2.c)) == 1
}

func (f *Function) Optimize() bool {
	return int(C.jit_optimize(f.c)) == 1
}

func (f *Function) CreateValue(typ *Type) *Value {
	return toValue(C.jit_value_create(f.c, typ.c))
}

func (f *Function) CreatePtrValue(ptr unsafe.Pointer) *Value {
	return toValue(C.jit_value_create_nint_constant(f.c, C.jit_type_void_ptr, C.jit_nint(uintptr(ptr))))
}

func (f *Function) CreateNintConstant(typ *Type, constValue int) *Value {
	return toValue(C.jit_value_create_nint_constant(f.c, typ.c, C.jit_nint(constValue)))
}

func (f *Function) CreateLongConstant(typ *Type, constValue int64) *Value {
	return toValue(C.jit_value_create_long_constant(f.c, typ.c, C.jit_long(constValue)))
}

func (f *Function) CreateFloat32Constant(typ *Type, constValue float32) *Value {
	return toValue(C.jit_value_create_float32_constant(f.c, typ.c, C.jit_float32(constValue)))
}

func (f *Function) CreateFloat64Constant(typ *Type, constValue float64) *Value {
	return toValue(C.jit_value_create_float64_constant(f.c, typ.c, C.jit_float64(constValue)))
}

func (f *Function) CreateIntValue(value int) *Value {
	return f.CreateNintConstant(TypeGoInt, value)
}

func (f *Function) Param(param uint) *Value {
	return toValue(C.jit_value_get_param(f.c, C.uint(param)))
}

func (f *Function) StructPointer() *Value {
	return toValue(C.jit_value_get_struct_pointer(f.c))
}

func (f *Function) ValueRef(value *Value) {
	C.jit_value_ref(f.c, value.c)
}

func (f *Function) NextBlock(next *Block) *Block {
	return toBlock(C.jit_block_next(f.c, next.c))
}

func (f *Function) PreviousBlock(previous *Block) *Block {
	return toBlock(C.jit_block_previous(f.c, previous.c))
}

func (f *Function) BlockFromLabel(label *Label) *Block {
	return toBlock(C.jit_block_from_label(f.c, label.c))
}

func (f *Function) IsDeadCurrentBlock() bool {
	return int(C.jit_block_current_is_dead(f.c)) == 1
}

func (f *Function) Dump(name string, w io.Writer) error {
	tmpfile, err := ioutil.TempFile("", "")
	if err != nil {
		return err
	}
	defer os.Remove(tmpfile.Name())
	file := C.fdopen(C.int(tmpfile.Fd()), C.CString("w"))
	C.jit_dump_function(file, f.c, C.CString(name))
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

func (f *Function) DumpValue(value *Value, prefix string, w io.Writer) error {
	tmpfile, err := ioutil.TempFile("", "")
	if err != nil {
		return err
	}
	defer os.Remove(tmpfile.Name())
	file := C.fdopen(C.int(tmpfile.Fd()), C.CString("w"))
	C.jit_dump_value(file, f.c, value.c, C.CString(prefix))
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

func (f *Function) DumpInstruction(insn *Instruction, w io.Writer) error {
	tmpfile, err := ioutil.TempFile("", "")
	if err != nil {
		return err
	}
	defer os.Remove(tmpfile.Name())
	file := C.fdopen(C.int(tmpfile.Fd()), C.CString("w"))
	C.jit_dump_insn(file, f.c, insn.c)
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

func (f *Function) GoCall(fn interface{}, args []*Value) ([]*Value, error) {
	typ := reflect.TypeOf(fn)
	if typ.Kind() != reflect.Func {
		return nil, errors.New("invalid type")
	}
	fargs := []*Type{}
	for i := 0; i < typ.NumIn(); i++ {
		fargs = append(fargs, ReflectTypeToType(typ.In(i)))
	}
	rtypes := []*Type{}
	for i := 0; i < typ.NumOut(); i++ {
		rtypes = append(rtypes, ReflectTypeToType(typ.Out(i)))
	}
	rtype := TypeVoid
	if len(rtypes) > 0 {
		rtype = CreateStruct(rtypes, 0)
	}
	sig := CreateSignature(fargs, rtype)
	defer sig.Free()

	p := (*interfaceHeader)(unsafe.Pointer(&fn)).ptr
	entryPoint := *(*unsafe.Pointer)(p)
	ctxt := f.call_cgo_wait_runtime_init_done()
	crossargs := append(args, ctxt)
	rvalues := f.call_crosscall2(fargs, rtypes, entryPoint, crossargs)
	return rvalues, nil
}

func (f *Function) call_cgo_wait_runtime_init_done() *Value {
	fn := f.cgo_wait_runtime_init_done
	return f.Call("_cgo_wait_runtime_init_done", fn, nil)
}

//go:linkname cgo_runtime_cgocallback runtime.cgocallback
func cgo_runtime_cgocallback(unsafe.Pointer, unsafe.Pointer, uintptr, uintptr)

func (f *Function) call_crosscall2(argtypes []*Type, rtypes []*Type, entryPoint unsafe.Pointer, args []*Value) []*Value {
	crosscall2 := f.crosscall2

	fields := []*Type{}
	fields = append(fields, argtypes...)
	fields = append(fields, rtypes...)
	fields = append(fields, TypeVoidPtr)
	argtype := CreateStruct(fields, 0)
	fargs := f.CreateValue(argtype)

	wrapper := func(a unsafe.Pointer, n int32, ctxt uintptr) {
		addr := *(*unsafe.Pointer)(unsafe.Pointer(uintptr(a) + uintptr(n) - uintptr(ptrsize)))
		cgo_runtime_cgocallback(addr, a, uintptr(n), ctxt)
	}
	wrapperptr := **(**unsafe.Pointer)(unsafe.Pointer(&wrapper))

	ep := f.CreatePtrValue(entryPoint)
	argsref := f.AddressOf(fargs)
	offset := 0
	for i := 0; i < len(argtypes); i++ {
		f.StoreRelative(argsref, offset, args[i])
		offset += int(argtypes[i].Size())
	}
	f.StoreRelative(argsref, int(argtype.Size()-ptrsize), ep)
	fp := f.CreatePtrValue(wrapperptr)
	size := f.CreateNintConstant(TypeInt, int(argtype.Size()))
	f.Call("crosscall2", crosscall2, []*Value{fp, argsref, size, args[len(args)-1]})

	rvalues := []*Value{}
	for i := 0; i < len(rtypes); i++ {
		rvalues = append(rvalues, f.LoadRelative(argsref, offset, rtypes[i]))
		offset += int(rtypes[i].Size())
	}
	return rvalues
}
