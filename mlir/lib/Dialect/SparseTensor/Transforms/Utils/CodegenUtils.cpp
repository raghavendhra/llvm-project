//===- CodegenUtils.cpp - Utilities for generating MLIR -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CodegenUtils.h"

#include "mlir/Dialect/Bufferization/IR/Bufferization.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"
#include "mlir/Dialect/Linalg/Utils/Utils.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/IR/Types.h"
#include "mlir/IR/Value.h"
#include <optional>

using namespace mlir;
using namespace mlir::sparse_tensor;

//===----------------------------------------------------------------------===//
// ExecutionEngine/SparseTensorUtils helper functions.
//===----------------------------------------------------------------------===//

OverheadType mlir::sparse_tensor::overheadTypeEncoding(unsigned width) {
  switch (width) {
  case 64:
    return OverheadType::kU64;
  case 32:
    return OverheadType::kU32;
  case 16:
    return OverheadType::kU16;
  case 8:
    return OverheadType::kU8;
  case 0:
    return OverheadType::kIndex;
  }
  llvm_unreachable("Unsupported overhead bitwidth");
}

OverheadType mlir::sparse_tensor::overheadTypeEncoding(Type tp) {
  if (tp.isIndex())
    return OverheadType::kIndex;
  if (auto intTp = dyn_cast<IntegerType>(tp))
    return overheadTypeEncoding(intTp.getWidth());
  llvm_unreachable("Unknown overhead type");
}

Type mlir::sparse_tensor::getOverheadType(Builder &builder, OverheadType ot) {
  switch (ot) {
  case OverheadType::kIndex:
    return builder.getIndexType();
  case OverheadType::kU64:
    return builder.getIntegerType(64);
  case OverheadType::kU32:
    return builder.getIntegerType(32);
  case OverheadType::kU16:
    return builder.getIntegerType(16);
  case OverheadType::kU8:
    return builder.getIntegerType(8);
  }
  llvm_unreachable("Unknown OverheadType");
}

OverheadType
mlir::sparse_tensor::posTypeEncoding(SparseTensorEncodingAttr enc) {
  return overheadTypeEncoding(enc.getPosWidth());
}

OverheadType
mlir::sparse_tensor::crdTypeEncoding(SparseTensorEncodingAttr enc) {
  return overheadTypeEncoding(enc.getCrdWidth());
}

// TODO: we ought to add some `static_assert` tests to ensure that the
// `STEA::get{Pos,Crd}Type` methods agree with `getOverheadType(builder,
// {pos,crd}OverheadTypeEncoding(enc))`

// TODO: Adjust the naming convention for the constructors of
// `OverheadType` so we can use the `MLIR_SPARSETENSOR_FOREVERY_O` x-macro
// here instead of `MLIR_SPARSETENSOR_FOREVERY_FIXED_O`; to further reduce
// the possibility of typo bugs or things getting out of sync.
StringRef mlir::sparse_tensor::overheadTypeFunctionSuffix(OverheadType ot) {
  switch (ot) {
  case OverheadType::kIndex:
    return "0";
#define CASE(ONAME, O)                                                         \
  case OverheadType::kU##ONAME:                                                \
    return #ONAME;
    MLIR_SPARSETENSOR_FOREVERY_FIXED_O(CASE)
#undef CASE
  }
  llvm_unreachable("Unknown OverheadType");
}

StringRef mlir::sparse_tensor::overheadTypeFunctionSuffix(Type tp) {
  return overheadTypeFunctionSuffix(overheadTypeEncoding(tp));
}

PrimaryType mlir::sparse_tensor::primaryTypeEncoding(Type elemTp) {
  if (elemTp.isF64())
    return PrimaryType::kF64;
  if (elemTp.isF32())
    return PrimaryType::kF32;
  if (elemTp.isF16())
    return PrimaryType::kF16;
  if (elemTp.isBF16())
    return PrimaryType::kBF16;
  if (elemTp.isInteger(64))
    return PrimaryType::kI64;
  if (elemTp.isInteger(32))
    return PrimaryType::kI32;
  if (elemTp.isInteger(16))
    return PrimaryType::kI16;
  if (elemTp.isInteger(8))
    return PrimaryType::kI8;
  if (auto complexTp = dyn_cast<ComplexType>(elemTp)) {
    auto complexEltTp = complexTp.getElementType();
    if (complexEltTp.isF64())
      return PrimaryType::kC64;
    if (complexEltTp.isF32())
      return PrimaryType::kC32;
  }
  llvm_unreachable("Unknown primary type");
}

StringRef mlir::sparse_tensor::primaryTypeFunctionSuffix(PrimaryType pt) {
  switch (pt) {
#define CASE(VNAME, V)                                                         \
  case PrimaryType::k##VNAME:                                                  \
    return #VNAME;
    MLIR_SPARSETENSOR_FOREVERY_V(CASE)
#undef CASE
  }
  llvm_unreachable("Unknown PrimaryType");
}

StringRef mlir::sparse_tensor::primaryTypeFunctionSuffix(Type elemTp) {
  return primaryTypeFunctionSuffix(primaryTypeEncoding(elemTp));
}

//===----------------------------------------------------------------------===//
// Misc code generators.
//===----------------------------------------------------------------------===//

Value sparse_tensor::genCast(OpBuilder &builder, Location loc, Value value,
                             Type dstTp) {
  const Type srcTp = value.getType();
  if (srcTp == dstTp)
    return value;

  // int <=> index
  if (isa<IndexType>(srcTp) || isa<IndexType>(dstTp))
    return arith::IndexCastOp::create(builder, loc, dstTp, value);

  const auto srcIntTp = dyn_cast_or_null<IntegerType>(srcTp);
  const bool isUnsignedCast = srcIntTp ? srcIntTp.isUnsigned() : false;
  return mlir::convertScalarToDtype(builder, loc, value, dstTp, isUnsignedCast);
}

Value sparse_tensor::genScalarToTensor(OpBuilder &builder, Location loc,
                                       Value elem, Type dstTp) {
  if (auto rtp = dyn_cast<RankedTensorType>(dstTp)) {
    // Scalars can only be converted to 0-ranked tensors.
    assert(rtp.getRank() == 0);
    elem = sparse_tensor::genCast(builder, loc, elem, rtp.getElementType());
    return tensor::FromElementsOp::create(builder, loc, rtp, elem);
  }
  return sparse_tensor::genCast(builder, loc, elem, dstTp);
}

Value sparse_tensor::genIndexLoad(OpBuilder &builder, Location loc, Value mem,
                                  ValueRange s) {
  Value load = memref::LoadOp::create(builder, loc, mem, s);
  if (!isa<IndexType>(load.getType())) {
    if (load.getType().getIntOrFloatBitWidth() < 64)
      load = arith::ExtUIOp::create(builder, loc, builder.getI64Type(), load);
    load =
        arith::IndexCastOp::create(builder, loc, builder.getIndexType(), load);
  }
  return load;
}

mlir::TypedAttr mlir::sparse_tensor::getOneAttr(Builder &builder, Type tp) {
  if (isa<FloatType>(tp))
    return builder.getFloatAttr(tp, 1.0);
  if (isa<IndexType>(tp))
    return builder.getIndexAttr(1);
  if (auto intTp = dyn_cast<IntegerType>(tp))
    return builder.getIntegerAttr(tp, APInt(intTp.getWidth(), 1));
  if (isa<RankedTensorType, VectorType>(tp)) {
    auto shapedTp = cast<ShapedType>(tp);
    if (auto one = getOneAttr(builder, shapedTp.getElementType()))
      return DenseElementsAttr::get(shapedTp, one);
  }
  llvm_unreachable("Unsupported attribute type");
}

Value mlir::sparse_tensor::genIsNonzero(OpBuilder &builder, mlir::Location loc,
                                        Value v) {
  Type tp = v.getType();
  Value zero = constantZero(builder, loc, tp);
  if (isa<FloatType>(tp))
    return arith::CmpFOp::create(builder, loc, arith::CmpFPredicate::UNE, v,
                                 zero);
  if (tp.isIntOrIndex())
    return arith::CmpIOp::create(builder, loc, arith::CmpIPredicate::ne, v,
                                 zero);
  if (isa<ComplexType>(tp))
    return complex::NotEqualOp::create(builder, loc, v, zero);
  llvm_unreachable("Non-numeric type");
}

void mlir::sparse_tensor::genReshapeDstShape(
    OpBuilder &builder, Location loc, SmallVectorImpl<Value> &dstShape,
    ArrayRef<Value> srcShape, ArrayRef<Size> staticDstShape,
    ArrayRef<ReassociationIndices> reassociation) {
  // Collapse shape.
  if (reassociation.size() < srcShape.size()) {
    unsigned start = 0;
    for (const auto &map : llvm::enumerate(reassociation)) {
      auto dstDim = constantIndex(builder, loc, 1);
      for (unsigned i = start; i < start + map.value().size(); i++) {
        dstDim = arith::MulIOp::create(builder, loc, dstDim, srcShape[i]);
      }
      dstShape.push_back(dstDim);
      start = start + map.value().size();
    }
    assert(start == srcShape.size());
    return;
  }

  // Expand shape.
  assert(reassociation.size() == srcShape.size());
  unsigned start = 0;
  // Expand the i-th dimension in srcShape.
  for (unsigned i = 0, size = srcShape.size(); i < size; i++) {
    const auto &map = reassociation[i];
    auto srcDim = srcShape[i];
    // Iterate through dimensions expanded from the i-th dimension.
    for (unsigned j = start; j < start + map.size(); j++) {
      // There can be only one dynamic sized dimension among dimensions
      // expanded from the i-th dimension in srcShape.
      // For example, if srcDim = 8, then the expanded shape could be <2x?x2>,
      // but not <2x?x?>.
      if (staticDstShape[j] == ShapedType::kDynamic) {
        // The expanded dimension has dynamic size. We compute the dimension
        // by dividing srcDim by the product of the static dimensions.
        Size product = 1;
        for (unsigned k = start; k < start + map.size(); k++) {
          if (staticDstShape[k] != ShapedType::kDynamic) {
            product *= staticDstShape[k];
          }
        }
        // Compute the dynamic dimension size.
        Value productVal = constantIndex(builder, loc, product);
        Value dynamicSize =
            arith::DivUIOp::create(builder, loc, srcDim, productVal);
        dstShape.push_back(dynamicSize);
      } else {
        // The expanded dimension is statically known.
        dstShape.push_back(constantIndex(builder, loc, staticDstShape[j]));
      }
    }
    start = start + map.size();
  }
  assert(start == staticDstShape.size());
}

void mlir::sparse_tensor::reshapeCvs(
    OpBuilder &builder, Location loc,
    ArrayRef<ReassociationIndices> reassociation, // NOLINT
    ValueRange srcSizes, ValueRange srcCvs,       // NOLINT
    ValueRange dstSizes, SmallVectorImpl<Value> &dstCvs) {
  const unsigned srcRank = srcSizes.size();
  const unsigned dstRank = dstSizes.size();
  assert(srcRank == srcCvs.size() && "Source rank mismatch");
  const bool isCollapse = srcRank > dstRank;
  const ValueRange sizes = isCollapse ? srcSizes : dstSizes;
  // Iterate over reassociation map.
  unsigned i = 0;
  unsigned start = 0;
  for (const auto &map : llvm::enumerate(reassociation)) {
    // Prepare strides information in dimension slice.
    Value linear = constantIndex(builder, loc, 1);
    for (unsigned j = start, end = start + map.value().size(); j < end; j++) {
      linear = arith::MulIOp::create(builder, loc, linear, sizes[j]);
    }
    // Start expansion.
    Value val;
    if (!isCollapse)
      val = srcCvs[i];
    // Iterate over dimension slice.
    for (unsigned j = start, end = start + map.value().size(); j < end; j++) {
      linear = arith::DivUIOp::create(builder, loc, linear, sizes[j]);
      if (isCollapse) {
        const Value mul =
            arith::MulIOp::create(builder, loc, srcCvs[j], linear);
        val = val ? arith::AddIOp::create(builder, loc, val, mul) : mul;
      } else {
        const Value old = val;
        val = arith::DivUIOp::create(builder, loc, val, linear);
        assert(dstCvs.size() == j);
        dstCvs.push_back(val);
        val = arith::RemUIOp::create(builder, loc, old, linear);
      }
    }
    // Finalize collapse.
    if (isCollapse) {
      assert(dstCvs.size() == i);
      dstCvs.push_back(val);
    }
    start += map.value().size();
    i++;
  }
  assert(dstCvs.size() == dstRank);
}

FlatSymbolRefAttr mlir::sparse_tensor::getFunc(ModuleOp module, StringRef name,
                                               TypeRange resultType,
                                               ValueRange operands,
                                               EmitCInterface emitCInterface) {
  MLIRContext *context = module.getContext();
  auto result = SymbolRefAttr::get(context, name);
  auto func = module.lookupSymbol<func::FuncOp>(result.getAttr());
  if (!func) {
    OpBuilder moduleBuilder(module.getBodyRegion());
    func = func::FuncOp::create(
        moduleBuilder, module.getLoc(), name,
        FunctionType::get(context, operands.getTypes(), resultType));
    func.setPrivate();
    if (static_cast<bool>(emitCInterface))
      func->setAttr(LLVM::LLVMDialect::getEmitCWrapperAttrName(),
                    UnitAttr::get(context));
  }
  return result;
}

func::CallOp mlir::sparse_tensor::createFuncCall(
    OpBuilder &builder, Location loc, StringRef name, TypeRange resultType,
    ValueRange operands, EmitCInterface emitCInterface) {
  auto module = builder.getBlock()->getParentOp()->getParentOfType<ModuleOp>();
  FlatSymbolRefAttr fn =
      getFunc(module, name, resultType, operands, emitCInterface);
  return func::CallOp::create(builder, loc, resultType, fn, operands);
}

Type mlir::sparse_tensor::getOpaquePointerType(MLIRContext *ctx) {
  return LLVM::LLVMPointerType::get(ctx);
}

Type mlir::sparse_tensor::getOpaquePointerType(Builder &builder) {
  return getOpaquePointerType(builder.getContext());
}

Value mlir::sparse_tensor::genAlloca(OpBuilder &builder, Location loc,
                                     unsigned sz, Type tp, bool staticShape) {
  if (staticShape) {
    auto memTp = MemRefType::get({sz}, tp);
    return memref::AllocaOp::create(builder, loc, memTp);
  }
  return genAlloca(builder, loc, constantIndex(builder, loc, sz), tp);
}

Value mlir::sparse_tensor::genAlloca(OpBuilder &builder, Location loc, Value sz,
                                     Type tp) {
  auto memTp = MemRefType::get({ShapedType::kDynamic}, tp);
  return memref::AllocaOp::create(builder, loc, memTp, ValueRange{sz});
}

Value mlir::sparse_tensor::genAllocaScalar(OpBuilder &builder, Location loc,
                                           Type tp) {
  return memref::AllocaOp::create(builder, loc, MemRefType::get({}, tp));
}

Value mlir::sparse_tensor::allocaBuffer(OpBuilder &builder, Location loc,
                                        ValueRange values) {
  const unsigned sz = values.size();
  assert(sz >= 1);
  Value buffer = genAlloca(builder, loc, sz, values[0].getType());
  for (unsigned i = 0; i < sz; i++) {
    Value idx = constantIndex(builder, loc, i);
    memref::StoreOp::create(builder, loc, values[i], buffer, idx);
  }
  return buffer;
}

Value mlir::sparse_tensor::allocDenseTensor(OpBuilder &builder, Location loc,
                                            RankedTensorType tensorTp,
                                            ValueRange sizes) {
  Type elemTp = tensorTp.getElementType();
  auto shape = tensorTp.getShape();
  auto memTp = MemRefType::get(shape, elemTp);
  SmallVector<Value> dynamicSizes;
  for (unsigned i = 0, rank = tensorTp.getRank(); i < rank; i++) {
    if (shape[i] == ShapedType::kDynamic)
      dynamicSizes.push_back(sizes[i]);
  }
  Value mem = memref::AllocOp::create(builder, loc, memTp, dynamicSizes);
  Value zero = constantZero(builder, loc, elemTp);
  linalg::FillOp::create(builder, loc, ValueRange{zero}, ValueRange{mem});
  return mem;
}

void mlir::sparse_tensor::deallocDenseTensor(OpBuilder &builder, Location loc,
                                             Value buffer) {
  memref::DeallocOp::create(builder, loc, buffer);
}

void mlir::sparse_tensor::sizesFromSrc(OpBuilder &builder,
                                       SmallVectorImpl<Value> &sizes,
                                       Location loc, Value src) {
  const Dimension dimRank = getSparseTensorType(src).getDimRank();
  for (Dimension d = 0; d < dimRank; d++)
    sizes.push_back(linalg::createOrFoldDimOp(builder, loc, src, d));
}

Operation *mlir::sparse_tensor::getTop(Operation *op) {
  for (; isa<scf::ForOp>(op->getParentOp()) ||
         isa<scf::WhileOp>(op->getParentOp()) ||
         isa<scf::ParallelOp>(op->getParentOp()) ||
         isa<scf::IfOp>(op->getParentOp());
       op = op->getParentOp())
    ;
  return op;
}

void sparse_tensor::foreachInSparseConstant(
    OpBuilder &builder, Location loc, SparseElementsAttr attr, AffineMap order,
    function_ref<void(ArrayRef<Value>, Value)> callback) {
  if (!order)
    order = builder.getMultiDimIdentityMap(attr.getType().getRank());

  auto stt = SparseTensorType(getRankedTensorType(attr));
  const Dimension dimRank = stt.getDimRank();
  const auto coordinates = attr.getIndices().getValues<IntegerAttr>();
  const auto values = attr.getValues().getValues<Attribute>();

  // This is like the `Element<V>` class in the runtime library, but for
  // MLIR attributes.  In the future we may want to move this out into
  // a proper class definition to help improve code legibility (e.g.,
  // `first` -> `coords`, `second` -> `value`) as well as being able
  // to factor out analogues of `ElementLT<V>` for the sort below, etc.
  using ElementAttr = std::pair<SmallVector<IntegerAttr>, Attribute>;

  // Construct the COO from the SparseElementsAttr.
  SmallVector<ElementAttr> elems;
  for (size_t i = 0, nse = values.size(); i < nse; i++) {
    elems.emplace_back();
    elems.back().second = values[i];
    auto &coords = elems.back().first;
    coords.reserve(dimRank);
    for (Dimension d = 0; d < dimRank; d++)
      coords.push_back(coordinates[i * dimRank + d]);
  }

  // Sorts the sparse element attribute based on coordinates.
  llvm::sort(elems, [order](const ElementAttr &lhs, const ElementAttr &rhs) {
    if (std::addressof(lhs) == std::addressof(rhs))
      return false;

    auto lhsCoords = llvm::map_to_vector(
        lhs.first, [](IntegerAttr i) { return i.getInt(); });
    auto rhsCoords = llvm::map_to_vector(
        rhs.first, [](IntegerAttr i) { return i.getInt(); });

    SmallVector<int64_t, 4> lhsLvlCrds = order.compose(lhsCoords);
    SmallVector<int64_t, 4> rhsLvlCrds = order.compose(rhsCoords);
    // Sort the element based on the lvl coordinates.
    for (Level l = 0; l < order.getNumResults(); l++) {
      if (lhsLvlCrds[l] == rhsLvlCrds[l])
        continue;
      return lhsLvlCrds[l] < rhsLvlCrds[l];
    }
    llvm_unreachable("no equal coordinate in sparse element attr");
  });

  SmallVector<Value> cvs;
  cvs.reserve(dimRank);
  for (size_t i = 0, nse = values.size(); i < nse; i++) {
    // Remap coordinates.
    cvs.clear();
    for (Dimension d = 0; d < dimRank; d++) {
      auto crd = elems[i].first[d].getInt();
      cvs.push_back(arith::ConstantIndexOp::create(builder, loc, crd));
    }
    // Remap value.
    Value val;
    if (isa<ComplexType>(attr.getElementType())) {
      auto valAttr = cast<ArrayAttr>(elems[i].second);
      val = complex::ConstantOp::create(builder, loc, attr.getElementType(),
                                        valAttr);
    } else {
      auto valAttr = cast<TypedAttr>(elems[i].second);
      val = arith::ConstantOp::create(builder, loc, valAttr);
    }
    assert(val);
    callback(cvs, val);
  }
}

SmallVector<Value> sparse_tensor::loadAll(OpBuilder &builder, Location loc,
                                          size_t size, Value mem,
                                          size_t offsetIdx, Value offsetVal) {
#ifndef NDEBUG
  const auto memTp = cast<MemRefType>(mem.getType());
  assert(memTp.getRank() == 1);
  const Size memSh = memTp.getDimSize(0);
  assert(ShapedType::isDynamic(memSh) || memSh >= static_cast<Size>(size));
  assert(offsetIdx == 0 || offsetIdx < size);
#endif // NDEBUG
  SmallVector<Value> vs;
  vs.reserve(size);
  for (unsigned i = 0; i < size; i++) {
    Value v = memref::LoadOp::create(builder, loc, mem,
                                     constantIndex(builder, loc, i));
    if (i == offsetIdx && offsetVal)
      v = arith::AddIOp::create(builder, loc, v, offsetVal);
    vs.push_back(v);
  }
  return vs;
}

void sparse_tensor::storeAll(OpBuilder &builder, Location loc, Value mem,
                             ValueRange vs, size_t offsetIdx, Value offsetVal) {
#ifndef NDEBUG
  const size_t vsize = vs.size();
  const auto memTp = cast<MemRefType>(mem.getType());
  assert(memTp.getRank() == 1);
  const Size memSh = memTp.getDimSize(0);
  assert(ShapedType::isDynamic(memSh) || memSh >= static_cast<Size>(vsize));
  assert(offsetIdx == 0 || offsetIdx < vsize);
#endif // NDEBUG
  for (const auto &v : llvm::enumerate(vs)) {
    const Value w =
        (offsetIdx == v.index() && offsetVal)
            ? arith::AddIOp::create(builder, loc, v.value(), offsetVal)
            : v.value();
    memref::StoreOp::create(builder, loc, w, mem,
                            constantIndex(builder, loc, v.index()));
  }
}

TypedValue<BaseMemRefType>
sparse_tensor::genToMemref(OpBuilder &builder, Location loc, Value tensor) {
  auto tTp = llvm::cast<TensorType>(tensor.getType());
  auto mTp = MemRefType::get(tTp.getShape(), tTp.getElementType());
  return cast<TypedValue<BaseMemRefType>>(
      bufferization::ToBufferOp::create(builder, loc, mTp, tensor).getResult());
}

Value sparse_tensor::createOrFoldSliceOffsetOp(OpBuilder &builder, Location loc,
                                               Value tensor, Dimension dim) {
  auto enc = getSparseTensorEncoding(tensor.getType());
  assert(enc && enc.isSlice());
  std::optional<unsigned> offset = enc.getStaticDimSliceOffset(dim);
  if (offset.has_value())
    return constantIndex(builder, loc, *offset);
  return ToSliceOffsetOp::create(builder, loc, tensor, APInt(64, dim));
}

Value sparse_tensor::createOrFoldSliceStrideOp(OpBuilder &builder, Location loc,
                                               Value tensor, Dimension dim) {
  auto enc = getSparseTensorEncoding(tensor.getType());
  assert(enc && enc.isSlice());
  std::optional<unsigned> stride = enc.getStaticDimSliceStride(dim);
  if (stride.has_value())
    return constantIndex(builder, loc, *stride);
  return ToSliceStrideOp::create(builder, loc, tensor, APInt(64, dim));
}

Value sparse_tensor::genReader(OpBuilder &builder, Location loc,
                               SparseTensorType stt, Value tensor,
                               /*out*/ SmallVectorImpl<Value> &dimSizesValues,
                               /*out*/ Value &dimSizesBuffer) {
  // Construct the dimension **shapes** buffer. The buffer contains the static
  // size per dimension, or otherwise a zero for a dynamic size.
  Dimension dimRank = stt.getDimRank();
  dimSizesValues.clear();
  dimSizesValues.reserve(dimRank);
  for (const Size sz : stt.getDimShape()) {
    const auto s = ShapedType::isDynamic(sz) ? 0 : sz;
    dimSizesValues.push_back(constantIndex(builder, loc, s));
  }
  Value dimShapesBuffer = allocaBuffer(builder, loc, dimSizesValues);
  // Create the `CheckedSparseTensorReader`. This reader performs a
  // consistency check on the static sizes, but accepts any size
  // of each dimension with a dynamic size.
  Type opaqueTp = getOpaquePointerType(builder);
  Type eltTp = stt.getElementType();
  Value valTp = constantPrimaryTypeEncoding(builder, loc, eltTp);
  Value reader =
      createFuncCall(builder, loc, "createCheckedSparseTensorReader", opaqueTp,
                     {tensor, dimShapesBuffer, valTp}, EmitCInterface::On)
          .getResult(0);
  // For static shapes, the shape buffer can be used right away. For dynamic
  // shapes, use the information from the reader to construct a buffer that
  // supplies the actual size for each dynamic dimension.
  dimSizesBuffer = dimShapesBuffer;
  if (stt.hasDynamicDimShape()) {
    Type indexTp = builder.getIndexType();
    auto memTp = MemRefType::get({ShapedType::kDynamic}, indexTp);
    dimSizesBuffer =
        createFuncCall(builder, loc, "getSparseTensorReaderDimSizes", memTp,
                       reader, EmitCInterface::On)
            .getResult(0);
    // Also convert the dim shapes values into dim sizes values, just in case
    // subsequent clients need the values (DCE will remove unused).
    for (Dimension d = 0; d < dimRank; d++) {
      if (stt.isDynamicDim(d))
        dimSizesValues[d] = memref::LoadOp::create(
            builder, loc, dimSizesBuffer, constantIndex(builder, loc, d));
    }
  }
  return reader;
}

Value sparse_tensor::genMapBuffers(
    OpBuilder &builder, Location loc, SparseTensorType stt,
    ArrayRef<Value> dimSizesValues, Value dimSizesBuffer,
    /*out*/ SmallVectorImpl<Value> &lvlSizesValues,
    /*out*/ Value &dim2lvlBuffer,
    /*out*/ Value &lvl2dimBuffer) {
  const Dimension dimRank = stt.getDimRank();
  const Level lvlRank = stt.getLvlRank();
  lvlSizesValues.clear();
  lvlSizesValues.reserve(lvlRank);
  // For an identity mapping, the dim2lvl and lvl2dim mappings are
  // identical as are dimSizes and lvlSizes, so buffers are reused
  // as much as possible.
  if (stt.isIdentity()) {
    assert(dimRank == lvlRank);
    SmallVector<Value> iotaValues;
    iotaValues.reserve(lvlRank);
    for (Level l = 0; l < lvlRank; l++) {
      iotaValues.push_back(constantIndex(builder, loc, l));
      lvlSizesValues.push_back(dimSizesValues[l]);
    }
    dim2lvlBuffer = lvl2dimBuffer = allocaBuffer(builder, loc, iotaValues);
    return dimSizesBuffer; // now lvlSizesBuffer
  }
  // Otherwise, some code needs to be generated to set up the buffers.
  // This code deals with permutations as well as non-permutations that
  // arise from rank changing blocking.
  const auto dimToLvl = stt.getDimToLvl();
  const auto lvlToDim = stt.getLvlToDim();
  SmallVector<Value> dim2lvlValues(lvlRank); // for each lvl, expr in dim vars
  SmallVector<Value> lvl2dimValues(dimRank); // for each dim, expr in lvl vars
  // Generate dim2lvl.
  assert(lvlRank == dimToLvl.getNumResults());
  for (Level l = 0; l < lvlRank; l++) {
    AffineExpr exp = dimToLvl.getResult(l);
    // We expect:
    //    (1) l = d
    //    (2) l = d / c
    //    (3) l = d % c
    Dimension d = 0;
    uint64_t cf = 0, cm = 0;
    switch (exp.getKind()) {
    case AffineExprKind::DimId: {
      d = cast<AffineDimExpr>(exp).getPosition();
      break;
    }
    case AffineExprKind::FloorDiv: {
      auto floor = cast<AffineBinaryOpExpr>(exp);
      d = cast<AffineDimExpr>(floor.getLHS()).getPosition();
      cf = cast<AffineConstantExpr>(floor.getRHS()).getValue();
      break;
    }
    case AffineExprKind::Mod: {
      auto mod = cast<AffineBinaryOpExpr>(exp);
      d = cast<AffineDimExpr>(mod.getLHS()).getPosition();
      cm = cast<AffineConstantExpr>(mod.getRHS()).getValue();
      break;
    }
    default:
      llvm::report_fatal_error("unsupported dim2lvl in sparse tensor type");
    }
    dim2lvlValues[l] = constantIndex(builder, loc, encodeDim(d, cf, cm));
    // Compute the level sizes.
    //    (1) l = d        : size(d)
    //    (2) l = d / c    : size(d) / c
    //    (3) l = d % c    : c
    Value lvlSz;
    if (cm == 0) {
      lvlSz = dimSizesValues[d];
      if (cf != 0)
        lvlSz = arith::DivUIOp::create(builder, loc, lvlSz,
                                       constantIndex(builder, loc, cf));
    } else {
      lvlSz = constantIndex(builder, loc, cm);
    }
    lvlSizesValues.push_back(lvlSz);
  }
  // Generate lvl2dim.
  assert(dimRank == lvlToDim.getNumResults());
  for (Dimension d = 0; d < dimRank; d++) {
    AffineExpr exp = lvlToDim.getResult(d);
    // We expect:
    //    (1) d = l
    //    (2) d = l' * c + l
    Level l = 0, ll = 0;
    uint64_t c = 0;
    switch (exp.getKind()) {
    case AffineExprKind::DimId: {
      l = cast<AffineDimExpr>(exp).getPosition();
      break;
    }
    case AffineExprKind::Add: {
      // Always mul on lhs, symbol/constant on rhs.
      auto add = cast<AffineBinaryOpExpr>(exp);
      assert(add.getLHS().getKind() == AffineExprKind::Mul);
      auto mul = cast<AffineBinaryOpExpr>(add.getLHS());
      ll = cast<AffineDimExpr>(mul.getLHS()).getPosition();
      c = cast<AffineConstantExpr>(mul.getRHS()).getValue();
      l = cast<AffineDimExpr>(add.getRHS()).getPosition();
      break;
    }
    default:
      llvm::report_fatal_error("unsupported lvl2dim in sparse tensor type");
    }
    lvl2dimValues[d] = constantIndex(builder, loc, encodeLvl(l, c, ll));
  }
  // Return buffers.
  dim2lvlBuffer = allocaBuffer(builder, loc, dim2lvlValues);
  lvl2dimBuffer = allocaBuffer(builder, loc, lvl2dimValues);
  return allocaBuffer(builder, loc, lvlSizesValues); // lvlSizesBuffer
}
