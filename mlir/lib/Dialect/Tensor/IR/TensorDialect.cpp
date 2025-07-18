//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Bufferization/IR/BufferizableOpInterface.h"
#include "mlir/Dialect/Complex/IR/Complex.h"
#include "mlir/Dialect/Tensor/IR/Tensor.h"
#include "mlir/Dialect/Transform/Interfaces/TransformInterfaces.h"
#include "mlir/Interfaces/RuntimeVerifiableOpInterface.h"
#include "mlir/Interfaces/SubsetOpInterface.h"
#include "mlir/Transforms/InliningUtils.h"

using namespace mlir;
using namespace mlir::tensor;

#include "mlir/Dialect/Tensor/IR/TensorOpsDialect.cpp.inc"

//===----------------------------------------------------------------------===//
// TensorDialect Dialect Interfaces
//===----------------------------------------------------------------------===//

namespace {
struct TensorInlinerInterface : public DialectInlinerInterface {
  using DialectInlinerInterface::DialectInlinerInterface;
  bool isLegalToInline(Region *dest, Region *src, bool wouldBeCloned,
                       IRMapping &valueMapping) const final {
    return true;
  }
  bool isLegalToInline(Operation *, Region *, bool wouldBeCloned,
                       IRMapping &) const final {
    return true;
  }
};
} // namespace

//===----------------------------------------------------------------------===//
// TensorDialect Methods
//===----------------------------------------------------------------------===//

void TensorDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "mlir/Dialect/Tensor/IR/TensorOps.cpp.inc"
      >();
  addInterfaces<TensorInlinerInterface>();
  declarePromisedInterfaces<
      bufferization::BufferizableOpInterface, CastOp, CollapseShapeOp, ConcatOp,
      DimOp, EmptyOp, ExpandShapeOp, ExtractSliceOp, ExtractOp, FromElementsOp,
      GenerateOp, InsertOp, InsertSliceOp, PadOp, ParallelInsertSliceOp, RankOp,
      ReshapeOp, SplatOp>();
  declarePromisedInterfaces<transform::FindPayloadReplacementOpInterface,
                            CollapseShapeOp, ExpandShapeOp, ExtractSliceOp,
                            InsertSliceOp, ReshapeOp>();
  declarePromisedInterfaces<ReifyRankedShapedTypeOpInterface, ExpandShapeOp,
                            CollapseShapeOp, PadOp>();
  declarePromisedInterfaces<RuntimeVerifiableOpInterface, CastOp, DimOp,
                            ExtractOp, InsertOp, ExtractSliceOp>();
  declarePromisedInterfaces<SubsetOpInterface, ExtractSliceOp, InsertSliceOp,
                            ParallelInsertSliceOp>();
  declarePromisedInterfaces<SubsetInsertionOpInterface, InsertSliceOp,
                            ParallelInsertSliceOp>();
  declarePromisedInterface<SubsetExtractionOpInterface, ExtractSliceOp>();
  declarePromisedInterfaces<TilingInterface, PadOp>();
  declarePromisedInterfaces<ValueBoundsOpInterface, CastOp, DimOp, EmptyOp,
                            ExtractSliceOp, PadOp, RankOp>();
}
