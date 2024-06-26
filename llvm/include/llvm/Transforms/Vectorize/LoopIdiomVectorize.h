//===----------LoopIdiomVectorize.h -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TRANSFORMS_VECTORIZE_LOOPIDIOMVECTORIZE_H
#define LLVM_LIB_TRANSFORMS_VECTORIZE_LOOPIDIOMVECTORIZE_H

#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"

namespace llvm {
struct LoopIdiomVectorizePass : PassInfoMixin<LoopIdiomVectorizePass> {
  PreservedAnalyses run(Loop &L, LoopAnalysisManager &AM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &U);
};
} // namespace llvm
#endif // LLVM_LIB_TRANSFORMS_VECTORIZE_LOOPIDIOMVECTORIZE_H
