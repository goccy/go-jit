package jit

import (
	"io"
	"unsafe"

	"github.com/goccy/go-jit/internal/ccall"
)

type Function struct {
	*ccall.Function
}

func toFunction(c *ccall.Function) *Function {
	return &Function{c}
}

func (f *Function) Label(label *Label) bool {
	return f.Function.Label(label.Label)
}

func (f *Function) LabelRight(label *Label) bool {
	return f.Function.LabelRight(label.Label)
}

func (f *Function) NewBlock(label *Label) bool {
	return f.Function.NewBlock(label.Label)
}

func (f *Function) Load(value *Value) *Value {
	return toValue(f.Function.Load(value.Value))
}

func (f *Function) Dup(value *Value) *Value {
	return toValue(f.Function.Dup(value.Value))
}

func (f *Function) Store(dest, value *Value) bool {
	return f.Function.Store(dest.Value, value.Value)
}

func (f *Function) LoadRelative(value *Value, offset int, typ *Type) *Value {
	return toValue(f.Function.LoadRelative(value.Value, offset, typ.Type))
}

func (f *Function) StoreRelative(dest *Value, offset int, value *Value) bool {
	return f.Function.StoreRelative(dest.Value, offset, value.Value)
}

func (f *Function) AddRelative(value *Value, offset int) *Value {
	return toValue(f.Function.AddRelative(value.Value, offset))
}

func (f *Function) LoadElem(baseAddr, index *Value, elemType *Type) *Value {
	return toValue(f.Function.LoadElem(baseAddr.Value, index.Value, elemType.Type))
}

func (f *Function) LoadElemAddress(baseAddr, index *Value, elemType *Type) *Value {
	return toValue(f.Function.LoadElemAddress(baseAddr.Value, index.Value, elemType.Type))
}

func (f *Function) StoreElem(baseAddr, index, value *Value) bool {
	return f.Function.StoreElem(baseAddr.Value, index.Value, value.Value)
}

func (f *Function) CheckNull(value *Value) bool {
	return f.Function.CheckNull(value.Value)
}

func (f *Function) Nop() bool {
	return f.Function.Nop()
}

func (f *Function) Add(value1, value2 *Value) *Value {
	return toValue(f.Function.Add(value1.Value, value2.Value))
}

func (f *Function) AddOvf(value1, value2 *Value) *Value {
	return toValue(f.Function.AddOvf(value1.Value, value2.Value))
}

func (f *Function) Sub(value1, value2 *Value) *Value {
	return toValue(f.Function.Sub(value1.Value, value2.Value))
}

func (f *Function) SubOvf(value1, value2 *Value) *Value {
	return toValue(f.Function.SubOvf(value1.Value, value2.Value))
}

func (f *Function) Mul(value1, value2 *Value) *Value {
	return toValue(f.Function.Mul(value1.Value, value2.Value))
}

func (f *Function) MulOvf(value1, value2 *Value) *Value {
	return toValue(f.Function.MulOvf(value1.Value, value2.Value))
}

func (f *Function) Div(value1, value2 *Value) *Value {
	return toValue(f.Function.Div(value2.Value, value2.Value))
}

func (f *Function) Rem(value1, value2 *Value) *Value {
	return toValue(f.Function.Rem(value1.Value, value2.Value))
}

func (f *Function) RemIEEE(value1, value2 *Value) *Value {
	return toValue(f.Function.RemIEEE(value1.Value, value2.Value))
}

func (f *Function) Neg(value1 *Value) *Value {
	return toValue(f.Function.Neg(value1.Value))
}

func (f *Function) And(value1, value2 *Value) *Value {
	return toValue(f.Function.And(value1.Value, value2.Value))
}

func (f *Function) Or(value1, value2 *Value) *Value {
	return toValue(f.Function.Or(value1.Value, value2.Value))
}

func (f *Function) Xor(value1, value2 *Value) *Value {
	return toValue(f.Function.Xor(value1.Value, value2.Value))
}

func (f *Function) Not(value1 *Value) *Value {
	return toValue(f.Function.Not(value1.Value))
}

func (f *Function) Shl(value1, value2 *Value) *Value {
	return toValue(f.Function.Shl(value1.Value, value2.Value))
}

func (f *Function) Shr(value1, value2 *Value) *Value {
	return toValue(f.Function.Shr(value1.Value, value2.Value))
}

func (f *Function) Ushr(value1, value2 *Value) *Value {
	return toValue(f.Function.Ushr(value1.Value, value2.Value))
}

func (f *Function) Sshr(value1, value2 *Value) *Value {
	return toValue(f.Function.Sshr(value1.Value, value2.Value))
}

func (f *Function) Eq(value1, value2 *Value) *Value {
	return toValue(f.Function.Eq(value1.Value, value2.Value))
}

func (f *Function) Ne(value1, value2 *Value) *Value {
	return toValue(f.Function.Ne(value1.Value, value2.Value))
}

func (f *Function) Lt(value1, value2 *Value) *Value {
	return toValue(f.Function.Lt(value1.Value, value2.Value))
}

func (f *Function) Le(value1, value2 *Value) *Value {
	return toValue(f.Function.Le(value1.Value, value2.Value))
}

func (f *Function) Gt(value1, value2 *Value) *Value {
	return toValue(f.Function.Gt(value1.Value, value2.Value))
}

func (f *Function) Ge(value1, value2 *Value) *Value {
	return toValue(f.Function.Ge(value1.Value, value2.Value))
}

func (f *Function) Cmpl(value1, value2 *Value) *Value {
	return toValue(f.Function.Cmpl(value1.Value, value2.Value))
}

func (f *Function) Cmpg(value1, value2 *Value) *Value {
	return toValue(f.Function.Cmpg(value1.Value, value2.Value))
}

func (f *Function) ToBool(value1 *Value) *Value {
	return toValue(f.Function.ToBool(value1.Value))
}

func (f *Function) ToNotBool(value1 *Value) *Value {
	return toValue(f.Function.ToNotBool(value1.Value))
}

func (f *Function) Acos(value1 *Value) *Value {
	return toValue(f.Function.Acos(value1.Value))
}

func (f *Function) Asin(value1 *Value) *Value {
	return toValue(f.Function.Asin(value1.Value))
}

func (f *Function) Atan(value1 *Value) *Value {
	return toValue(f.Function.Atan(value1.Value))
}

func (f *Function) Atan2(value1, value2 *Value) *Value {
	return toValue(f.Function.Atan2(value1.Value, value2.Value))
}

func (f *Function) Ceil(value1 *Value) *Value {
	return toValue(f.Function.Ceil(value1.Value))
}

func (f *Function) Cos(value1 *Value) *Value {
	return toValue(f.Function.Cos(value1.Value))
}

func (f *Function) Cosh(value1 *Value) *Value {
	return toValue(f.Function.Cosh(value1.Value))
}

func (f *Function) Exp(value1 *Value) *Value {
	return toValue(f.Function.Exp(value1.Value))
}

func (f *Function) Floor(value1 *Value) *Value {
	return toValue(f.Function.Floor(value1.Value))
}

func (f *Function) Log(value1 *Value) *Value {
	return toValue(f.Function.Log(value1.Value))
}

func (f *Function) Log10(value1 *Value) *Value {
	return toValue(f.Function.Log10(value1.Value))
}

func (f *Function) Pow(value1, value2 *Value) *Value {
	return toValue(f.Function.Pow(value1.Value, value2.Value))
}

func (f *Function) Rint(value1 *Value) *Value {
	return toValue(f.Function.Rint(value1.Value))
}

func (f *Function) Round(value1 *Value) *Value {
	return toValue(f.Function.Round(value1.Value))
}

func (f *Function) Sin(value1 *Value) *Value {
	return toValue(f.Function.Sin(value1.Value))
}

func (f *Function) Sinh(value1 *Value) *Value {
	return toValue(f.Function.Sinh(value1.Value))
}

func (f *Function) Sqrt(value1 *Value) *Value {
	return toValue(f.Function.Sqrt(value1.Value))
}

func (f *Function) Tan(value1 *Value) *Value {
	return toValue(f.Function.Tan(value1.Value))
}

func (f *Function) Tanh(value1 *Value) *Value {
	return toValue(f.Function.Tanh(value1.Value))
}

func (f *Function) Trunc(value1 *Value) *Value {
	return toValue(f.Function.Trunc(value1.Value))
}

func (f *Function) IsNan(value1 *Value) *Value {
	return toValue(f.Function.IsNan(value1.Value))
}

func (f *Function) IsFinite(value1 *Value) *Value {
	return toValue(f.Function.IsFinite(value1.Value))
}

func (f *Function) IsInf(value1 *Value) *Value {
	return toValue(f.Function.IsInf(value1.Value))
}

func (f *Function) Abs(value1 *Value) *Value {
	return toValue(f.Function.Abs(value1.Value))
}

func (f *Function) Min(value1, value2 *Value) *Value {
	return toValue(f.Function.Min(value1.Value, value2.Value))
}

func (f *Function) Max(value1, value2 *Value) *Value {
	return toValue(f.Function.Max(value1.Value, value2.Value))
}

func (f *Function) Sign(value1 *Value) *Value {
	return toValue(f.Function.Sign(value1.Value))
}

func (f *Function) Branch(label *Label) bool {
	return f.Function.Branch(label.Label)
}

func (f *Function) BranchIf(value *Value, label *Label) bool {
	return f.Function.BranchIf(value.Value, label.Label)
}

func (f *Function) BranchIfNot(value *Value, label *Label) bool {
	return f.Function.BranchIfNot(value.Value, label.Label)
}

func (f *Function) JumpTable(value *Value, labels Labels) bool {
	return f.Function.JumpTable(value.Value, labels.raw())
}

func (f *Function) AddressOf(value1 *Value) *Value {
	return toValue(f.Function.AddressOf(value1.Value))
}

func (f *Function) AddressOfLabel(label *Label) *Value {
	return toValue(f.Function.AddressOfLabel(label.Label))
}

func (f *Function) Convert(value *Value, typ *Type, overflowCheck int) *Value {
	return toValue(f.Function.Convert(value.Value, typ.Type, overflowCheck))
}

func (f *Function) Call(name string, fn *Function, args Values) *Value {
	return toValue(f.Function.Call(name, fn.Function, args.raw()))
}

func (f *Function) CallIndirect(fn *Function, value *Value, signature *Type, args Values) *Value {
	return toValue(f.Function.CallIndirect(fn.Function, value.Value, signature.Type, args.raw()))
}

func (f *Function) CallNestedIndirect(fn *Function, value *Value, parentFrame *Value, signature *Type, args Values) *Value {
	return toValue(f.Function.CallNestedIndirect(fn.Function, value.Value, parentFrame.Value, signature.Type, args.raw()))
}

func (f *Function) CallIndirectVtable(fn *Function, value *Value, signature *Type, args Values) *Value {
	return toValue(f.Function.CallIndirectVtable(fn.Function, value.Value, signature.Type, args.raw()))
}

func (f *Function) CallNative(fn *Function, name string, nativeFunc unsafe.Pointer, signature *Type, args Values) *Value {
	return toValue(f.Function.CallNative(fn.Function, name, nativeFunc, signature.Type, args.raw()))
}

//func (f *Function) CallIntrinsic(fn *Function, name string, intrinsicFunc unsafe.Pointer, desc *IntrinsicDescriptor, arg1, arg2 *Value) *Value {
//	return toValue(C.jit_insn_call_intrinsic(f.c, C.CString(name), intrinsicFunc, desc.c, arg1.c, arg2.c))
//}

func (f *Function) IncomingReg(value *Value, reg int) int {
	return f.Function.IncomingReg(value.Value, reg)
}

func (f *Function) Return(value *Value) int {
	return f.Function.Return(value.Value)
}

func (f *Function) ReturnPtr(value *Value, typ *Type) int {
	return f.Function.ReturnPtr(value.Value, typ.Type)
}

func (f *Function) DefaultReturn() int {
	return f.Function.DefaultReturn()
}

func (f *Function) Memcpy(dest, src, size *Value) int {
	return f.Function.Memcpy(dest.Value, src.Value, size.Value)
}

func (f *Function) Memmove(dest, src, size *Value) int {
	return f.Function.Memmove(dest.Value, src.Value, size.Value)
}

func (f *Function) Memset(dest, src, size *Value) int {
	return f.Function.Memset(dest.Value, src.Value, size.Value)
}

func (f *Function) Alloca(size *Value) *Value {
	return toValue(f.Function.Alloca(size.Value))
}

func (f *Function) MoveBlocksToStart(fromLabel *Label, toLabel *Label) int {
	return f.Function.MoveBlocksToStart(fromLabel.Label, toLabel.Label)
}

func (f *Function) MoveBlocksToEnd(fromLabel *Label, toLabel *Label) int {
	return f.Function.MoveBlocksToEnd(fromLabel.Label, toLabel.Label)
}

func (f *Function) MarkOffset(offset int) int {
	return f.Function.MarkOffset(offset)
}

func (f *Function) MarkBreakpoint(data1, data2 int) int {
	return f.Function.MarkBreakpoint(data1, data2)
}

func (f *Function) MarkBreakpointVariable(data1, data2 *Value) int {
	return f.Function.MarkBreakpointVariable(data1.Value, data2.Value)
}

func (f *Function) Abandon() {
	f.Function.Abandon()
}

func (f *Function) Context() *Context {
	return toContext(f.Function.Context())
}

func (f *Function) Signature() *Type {
	return toType(f.Function.Signature())
}

func (f *Function) Meta(typ int) unsafe.Pointer {
	return f.Function.Meta(typ)
}

func (f *Function) FreeMeta(typ int) {
	f.Function.FreeMeta(typ)
}

func (f *Function) Entry() *Block {
	return toBlock(f.Function.Entry())
}

func (f *Function) Current() *Block {
	return toBlock(f.Function.Current())
}

func (f *Function) NestedParent() *Function {
	return toFunction(f.Function.NestedParent())
}

func (f *Function) SetParentFrame(parentFrame *Value) {
	f.Function.SetParentFrame(parentFrame.Value)
}

func (f *Function) Compile() bool {
	return f.Function.Compile()
}

func (f *Function) IsCompiled() bool {
	return f.Function.IsCompiled()
}

func (f *Function) SetRecompilable() {
	f.Function.SetRecompilable()
}

func (f *Function) ClearRecompilable() {
	f.Function.ClearRecompilable()
}

func (f *Function) IsRecompilable() bool {
	return f.Function.IsRecompilable()
}

func (f *Function) SetupEntry(entryPoint unsafe.Pointer) {
	f.Function.SetupEntry(entryPoint)
}

func (f *Function) ToClosure() unsafe.Pointer {
	return f.Function.ToClosure()
}

func (f *Function) ToVtablePointer() unsafe.Pointer {
	return f.Function.ToVtablePointer()
}

func (f *Function) Run(args ...interface{}) interface{} {
	return f.Function.Apply(args)
}

func (f *Function) SetOptimizationLevel(level uint) {
	f.Function.SetOptimizationLevel(level)
}

func (f *Function) OptimizationLevel() uint {
	return f.Function.OptimizationLevel()
}

func MaxOptimizationLevel() uint {
	return ccall.MaxOptimizationLevel()
}

func (f *Function) ReserveLabel() *Label {
	return toLabel(f.Function.ReserveLabel())
}

func (f *Function) LabelsEqual(label *Label, label2 *Label) bool {
	return f.Function.LabelsEqual(label.Label, label2.Label)
}

func (f *Function) Optimize() bool {
	return f.Function.Optimize()
}

func (f *Function) CreateValue(typ *Type) *Value {
	return toValue(f.Function.CreateValue(typ.Type))
}

func (f *Function) CreatePtrValue(ptr unsafe.Pointer) *Value {
	return toValue(f.Function.CreatePtrValue(ptr))
}

func (f *Function) CreateIntValue(value int) *Value {
	return toValue(f.Function.CreateNintConstant(ccall.TypeGoInt, value))
}

func (f *Function) CreateFloat32Value(constValue float32) *Value {
	return toValue(f.Function.CreateFloat32Constant(ccall.TypeFloat32, constValue))
}

func (f *Function) CreateFloat64Value(constValue float64) *Value {
	return toValue(f.Function.CreateFloat64Constant(ccall.TypeFloat64, constValue))
}

func (f *Function) Param(param uint) *Value {
	return toValue(f.Function.Param(param))
}

func (f *Function) StructPointer() *Value {
	return toValue(f.Function.StructPointer())
}

func (f *Function) ValueRef(value *Value) {
	f.Function.ValueRef(value.Value)
}

func (f *Function) NextBlock(block *Block) *Block {
	return toBlock(f.Function.NextBlock(block.Block))
}

func (f *Function) PreviousBlock(block *Block) *Block {
	return toBlock(f.Function.PreviousBlock(block.Block))
}

func (f *Function) BlockFromLabel(label *Label) *Block {
	return toBlock(f.Function.BlockFromLabel(label.Label))
}

func (f *Function) IsDeadCurrentBlock() bool {
	return f.Function.IsDeadCurrentBlock()
}

func (f *Function) Dump(name string, w io.Writer) error {
	return f.Function.Dump(name, w)
}

func (f *Function) DumpValue(value *Value, prefix string, w io.Writer) error {
	return f.Function.DumpValue(value.Value, prefix, w)
}

func (f *Function) DumpInstruction(insn *Instruction, w io.Writer) error {
	return f.Function.DumpInstruction(insn.Instruction, w)
}

func (f *Function) GoCall(fn interface{}, args Values) ([]*Value, error) {
	values, err := f.Function.GoCall(fn, args.raw())
	if err != nil {
		return nil, err
	}
	return toValues(values), nil
}
