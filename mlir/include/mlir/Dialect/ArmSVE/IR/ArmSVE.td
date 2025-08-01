//===-- ArmSVE.td - ArmSVE dialect operation definitions ---*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the basic operations for the ArmSVE dialect.
//
//===----------------------------------------------------------------------===//

#ifndef ARMSVE_OPS
#define ARMSVE_OPS

include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/Dialect/LLVMIR/LLVMOpBase.td"

//===----------------------------------------------------------------------===//
// ArmSVE dialect definition
//===----------------------------------------------------------------------===//

def ArmSVE_Dialect : Dialect {
  let name = "arm_sve";
  let cppNamespace = "::mlir::arm_sve";
  let summary = "Basic dialect to target Arm SVE architectures";
  let description = [{
    This dialect contains the definitions necessary to target specific Arm SVE
    scalable vector operations.
  }];

  let dependentDialects = ["vector::VectorDialect"];
}

//===----------------------------------------------------------------------===//
// ArmSVE type definitions
//===----------------------------------------------------------------------===//

def SVBool : ScalableVectorOfRankAndLengthAndType<
  [1], [16], [I1]>
{
  let summary = "vector<[16]xi1>";
}

def SVEPredicate : ScalableVectorOfRankAndLengthAndType<
  [1], [16, 8, 4, 2, 1], [I1]>
{
  let summary = "vector<[1]xi1>, vector<[2]xi1>, vector<[4]xi1>, vector<[8]xi1>, or vector<[16]xi1>";
}

// Generalizations of SVBool and SVEPredicate to ranks >= 1.
// These are masks with a single trailing scalable dimension.
def SVBoolMask : VectorWithTrailingDimScalableOfSizeAndType<
  [16], [I1]>;
def SVEPredicateMask : VectorWithTrailingDimScalableOfSizeAndType<
  [16, 8, 4, 2, 1], [I1]>;

// A constraint for a 1-D scalable vector of `length`.
class Scalable1DVectorOfLength<int length, list<Type> elementTypes> : ShapedContainerType<
  elementTypes, And<[IsVectorOfShape<[length]>, IsVectorTypeWithAnyDimScalablePred]>,
  "a 1-D scalable vector with length " # length,
  "::mlir::VectorType">;

def SVEVector : AnyTypeOf<[
  Scalable1DVectorOfLength<2, [I64, F64]>,
  Scalable1DVectorOfLength<4, [I32, F32]>,
  Scalable1DVectorOfLength<8, [I16, F16, BF16]>,
  Scalable1DVectorOfLength<16, [I8]>],
  "an SVE vector with element size <= 64-bit">;

//===----------------------------------------------------------------------===//
// ArmSVE op definitions
//===----------------------------------------------------------------------===//

class ArmSVE_Op<string mnemonic, list<Trait> traits = []> :
  Op<ArmSVE_Dialect, mnemonic, traits> {}

class ArmSVE_IntrOp<string mnemonic,
                    list<Trait> traits = [],
                    list<int> overloadedOperands = [],
                    list<int> overloadedResults = [],
                    int numResults = 1,
                    list<int> immArgPositions = [],
                    list<string> immArgAttrNames = []> :
  LLVM_IntrOpBase</*Dialect dialect=*/ArmSVE_Dialect,
                  /*string opName=*/"intr." # mnemonic,
                  /*string enumName=*/"aarch64_sve_" # !subst(".", "_", mnemonic),
                  /*list<int> overloadedResults=*/overloadedResults,
                  /*list<int> overloadedOperands=*/overloadedOperands,
                  /*list<Trait> traits=*/traits,
                  /*int numResults=*/numResults,
                  /*bit requiresAccessGroup=*/0,
                  /*bit requiresAliasAnalysis=*/0,
                  /*bit requiresFastmath=*/0,
                  /*bit requiresArgAndResultAttrs=*/0,
                  /*bit requiresOpBundles=*/0,
                  /*list<int> immArgPositions=*/immArgPositions,
                  /*list<string> immArgAttrNames=*/immArgAttrNames>;

class ArmSVE_IntrBinaryOverloadedOp<string mnemonic,
                                    list<Trait> traits = []>:
  ArmSVE_IntrOp<mnemonic, traits,
    /*overloadedOperands=*/[], /*overloadedResults=*/[0]>;

class ScalableMaskedFOp<string mnemonic, string op_description,
                        list<Trait> traits = []> :
  ArmSVE_Op<mnemonic, !listconcat(traits,
                       [AllTypesMatch<["src1", "src2", "res"]>,
                        TypesMatchWith<
                          "mask has i1 element type and same shape as operands",
                          "src1", "mask", "getI1SameShape($_self)">])> {
  let summary = "masked " # op_description # " for scalable vectors of floats";
  let description = [{
    The `arm_sve.}] # mnemonic # [{` operation takes one scalable vector mask
    and two scalable vector operands, and perform floating point }] #
    op_description # [{ on active lanes. Inactive lanes will keep the value of
    the first operand.}];
  let arguments = (ins
          ScalableVectorOfAnyRank<[I1]>:$mask,
          ScalableVectorOfAnyRank<[AnyFloat]>:$src1,
          ScalableVectorOfAnyRank<[AnyFloat]>:$src2
  );
  let results = (outs ScalableVectorOfAnyRank<[AnyFloat]>:$res);
  let assemblyFormat =
    "$mask `,` $src1 `,` $src2 attr-dict `:` type($mask) `,` type($res)";
}

class ScalableMaskedIOp<string mnemonic, string op_description,
                        list<Trait> traits = []> :
  ArmSVE_Op<mnemonic, !listconcat(traits,
                       [AllTypesMatch<["src1", "src2", "res"]>,
                        TypesMatchWith<
                          "mask has i1 element type and same shape as operands",
                          "src1", "mask", "getI1SameShape($_self)">])> {
  let summary = "masked " # op_description # " for scalable vectors of integers";
  let description = [{
    The `arm_sve.}] # mnemonic # [{` operation takes one scalable vector mask
    and two scalable vector operands, and perform integer }] #
    op_description # [{ on active lanes. Inactive lanes will keep the value of
    the first operand.}];
  let arguments = (ins
          ScalableVectorOfAnyRank<[I1]>:$mask,
          ScalableVectorOfAnyRank<[I8, I16, I32, I64]>:$src1,
          ScalableVectorOfAnyRank<[I8, I16, I32, I64]>:$src2
  );
  let results = (outs ScalableVectorOfAnyRank<[I8, I16, I32, I64]>:$res);
  let assemblyFormat =
    "$mask `,` $src1 `,` $src2 attr-dict `:` type($mask) `,` type($res)";
}

def SdotOp : ArmSVE_Op<"sdot", [Pure,
                                AllTypesMatch<["src1", "src2"]>,
                                AllTypesMatch<["acc", "dst"]>]> {
  let summary = "Vector-vector dot product and accumulate op";
  let description = [{
    SDOT: Signed integer addition of dot product.

    This function maps to the SDOT instruction, and it takes signless integer
    operands that the operation interprets as signed. It partitions the second
    and third vector inputs into groups of four elements. They calculate the dot
    product of each group (without loss of precision) and then add each result
    to the overlapping element of the first vector input.

    Source:
    https://developer.arm.com/documentation/100987/0000
  }];
  // Supports either:
  //   (vector<16xi8>, vector<16xi8>) -> (vector<4xi32>)
  //   (vector<8xi16>. vector<8xi16>) -> (vector<2xi64>)
  let arguments = (ins
          ScalableVectorOfLengthAndType<[4, 2], [I32, I64]>:$acc,
          ScalableVectorOfLengthAndType<[16, 8], [I8, I16]>:$src1,
          ScalableVectorOfLengthAndType<[16, 8], [I8, I16]>:$src2
  );
  let results = (outs ScalableVectorOfLengthAndType<[4, 2], [I32, I64]>:$dst);
  let assemblyFormat =
    "$acc `,` $src1 `,` $src2 attr-dict `:` type($src1) `to` type($dst)";
}

def SmmlaOp : ArmSVE_Op<"smmla", [Pure,
                                  AllTypesMatch<["src1", "src2"]>,
                                  AllTypesMatch<["acc", "dst"]>]> {
  let summary = "Matrix-matrix multiply and accumulate op";
  let description = [{
    SMMLA: Signed integer matrix multiply-accumulate.

    This function maps to the SMMLA instruction, and it takes signless integer
    operands that the operation interprets as signed. It partitions the inputs
    into 128-bit quadwords, with the first input containing a row-by-row 2×2
    matrix of 32-bit integers, the second input containing a row-by-row 2×8
    matrix of 8-bit integers, and the third input containing a column-by-column
    8×2 matrix of 8-bit integers. For each quadword, they multiply the second
    input matrix by the third input matrix using natural arithmetic and then add
    the result to the first input using modular arithmetic.

    Source:
    https://developer.arm.com/documentation/100987/0000
  }];
  // Supports (vector<16xi8>, vector<16xi8>) -> (vector<4xi32>)
  let arguments = (ins
          ScalableVectorOfLengthAndType<[4], [I32]>:$acc,
          ScalableVectorOfLengthAndType<[16], [I8]>:$src1,
          ScalableVectorOfLengthAndType<[16], [I8]>:$src2
  );
  let results = (outs ScalableVectorOfLengthAndType<[4], [I32]>:$dst);
  let assemblyFormat =
    "$acc `,` $src1 `,` $src2 attr-dict `:` type($src1) `to` type($dst)";
}

def UdotOp : ArmSVE_Op<"udot", [Pure,
                                AllTypesMatch<["src1", "src2"]>,
                                AllTypesMatch<["acc", "dst"]>]> {
  let summary = "Vector-vector dot product and accumulate op";
  let description = [{
    UDOT: Unsigned integer addition of dot product.

    This function maps to the UDOT instruction, and it takes signless integer
    operands that the operation interprets as unsigned. It partitions the second
    and third vector inputs into groups of four elements. They calculate the dot
    product of each group (without loss of precision) and then add each result
    to the overlapping element of the first vector input.

    Source:
    https://developer.arm.com/documentation/100987/0000
  }];
  // Supports either:
  //   (vector<16xi8>, vector<16xi8>) -> (vector<4xi32>)
  //   (vector<8xi16>. vector<8xi16>) -> (vector<2xi64>)
  let arguments = (ins
          ScalableVectorOfLengthAndType<[4, 2], [I32, I64]>:$acc,
          ScalableVectorOfLengthAndType<[16, 8], [I8, I16]>:$src1,
          ScalableVectorOfLengthAndType<[16, 8], [I8, I16]>:$src2
  );
  let results = (outs ScalableVectorOfLengthAndType<[4, 2], [I32, I64]>:$dst);
  let assemblyFormat =
    "$acc `,` $src1 `,` $src2 attr-dict `:` type($src1) `to` type($dst)";
}

def UmmlaOp : ArmSVE_Op<"ummla", [Pure,
                                  AllTypesMatch<["src1", "src2"]>,
                                  AllTypesMatch<["acc", "dst"]>]> {
  let summary = "Matrix-matrix multiply and accumulate op";
  let description = [{
    UMMLA: Unsigned integer matrix multiply-accumulate.

    This function maps to the UMMLA instruction, and it takes signless integer
    operands that the operation interprets as unsigned. It partitions the inputs
    into 128-bit quadwords, with the first input containing a row-by-row 2×2
    matrix of 32-bit integers, the second input containing a row-by-row 2×8
    matrix of 8-bit integers, and the third input containing a column-by-column
    8×2 matrix of 8-bit integers. For each quadword, they multiply the second
    input matrix by the third input matrix using natural arithmetic and then add
    the result to the first input using modular arithmetic.

    Source:
    https://developer.arm.com/documentation/100987/0000
  }];
  // Supports (vector<16xi8>, vector<16xi8>) -> (vector<4xi32>)
  let arguments = (ins
          ScalableVectorOfLengthAndType<[4], [I32]>:$acc,
          ScalableVectorOfLengthAndType<[16], [I8]>:$src1,
          ScalableVectorOfLengthAndType<[16], [I8]>:$src2
  );
  let results = (outs ScalableVectorOfLengthAndType<[4], [I32]>:$dst);
  let assemblyFormat =
    "$acc `,` $src1 `,` $src2 attr-dict `:` type($src1) `to` type($dst)";
}

def UsmmlaOp : ArmSVE_Op<"usmmla", [Pure,
                                    AllTypesMatch<["src1", "src2"]>,
                                    AllTypesMatch<["acc", "dst"]>]> {
  let summary = "Matrix-matrix multiply and accumulate op";
  let description = [{
    USMMLA: Unsigned by signed integer matrix multiply-accumulate.

    The unsigned by signed integer matrix multiply-accumulate operation
    multiplies the 2×8 matrix of unsigned 8-bit integer values held
    the first source vector by the 8×2 matrix of signed 8-bit integer
    values in the second source vector. The resulting 2×2 widened 32-bit
    integer matrix product is then added to the 32-bit integer matrix
    accumulator.

    Source:
    https://developer.arm.com/documentation/100987/0000
  }];
  // Supports (vector<16xi8>, vector<16xi8>) -> (vector<4xi32>)
  let arguments = (ins
          ScalableVectorOfLengthAndType<[4], [I32]>:$acc,
          ScalableVectorOfLengthAndType<[16], [I8]>:$src1,
          ScalableVectorOfLengthAndType<[16], [I8]>:$src2
  );
  let results = (outs ScalableVectorOfLengthAndType<[4], [I32]>:$dst);
  let assemblyFormat =
    "$acc `,` $src1 `,` $src2 attr-dict `:` type($src1) `to` type($dst)";
}

def BfmmlaOp : ArmSVE_IntrOp<"bfmmla", [Pure,
                                        AllTypesMatch<["src1", "src2"]>,
                                        AllTypesMatch<["acc", "res"]>,
                                        ]> {
  let summary = "BFloat16 matrix multiply-accumulate";
  let description = [{
    BFMMLA: BFloat16 matrix multiply-accumulate into 2×2 matrices";

    This operation multiplies the 2x4 BFloat16 matrix held in each 128-bit
    segment of the first source vector by the 4x2 BFloat16 matrix in the
    corresponding segment of the second source vector, then accumulates
    this intermediate result with the 2x2 Float32 matrix in the corresponding
    segment of the accumulator vector, yielding the final 2x2 Float32
    segment of the result.

    Source:
    https://developer.arm.com/documentation/100987/0000
  }];
  // Supports (vector<[8]xbf16>, vector<[8]xbf16>) -> (vector<[4]xf32>)
  let arguments = (ins
          ScalableVectorOfLengthAndType<[4], [F32]>:$acc,
          ScalableVectorOfLengthAndType<[8], [BF16]>:$src1,
          ScalableVectorOfLengthAndType<[8], [BF16]>:$src2
  );
  let results = (outs ScalableVectorOfLengthAndType<[4], [F32]>:$res);
  let assemblyFormat =
    "$acc `,` $src1 `,` $src2 attr-dict `:` type($src1) `to` type($res)";
}

class SvboolTypeConstraint<string lhsArg, string rhsArg> : TypesMatchWith<
      "expected corresponding svbool type widened to [16]xi1",
      lhsArg, rhsArg,
      "VectorType(VectorType::Builder(::llvm::cast<VectorType>($_self)).setDim(::llvm::cast<VectorType>($_self).getRank() - 1, 16))">;

def ConvertFromSvboolOp : ArmSVE_Op<"convert_from_svbool",
                                    [Pure,
                                     SvboolTypeConstraint<"result", "source">]> {
  let summary = "Convert a svbool type to a SVE predicate type";
  let description = [{
    Converts svbool types (`vector<[16]xi1>` or vectors of that type, e.g.
    `vector<2x3x[16]xi1>`) to SVE predicate types. Note: Only the trailing
    dimension can be scalable.

    Example 1: Convert a 1-D svbool mask to a SVE predicate.
    ```mlir
    %source = vector.load %memref[%c0] : memref<?xi1>, vector<[16]xi1>
    %result = arm_sve.convert_from_svbool %source : vector<[4]xi1>
    ```

    Example 2: Convert a 2-D svbool mask to a mask of SVE predicates.
    ```mlir
    %source = vector.load %memref[%c0, %c0] : memref<2x?xi1>, vector<2x[16]xi1>
    %result = arm_sve.convert_from_svbool %source : vector<2x[8]xi1>
    ```

    ---

    A `svbool` is the smallest SVE predicate type that has a in-memory
    representation (and maps to a full predicate register). In MLIR `svbool` is
    represented as `vector<[16]xi1>`. Smaller SVE predicate types
    (`vector<[1|2|4|8]xi1>`) must be stored as a `svbool` then converted back to
    the original predicate type after loading.
  }];
  let arguments = (ins SVBoolMask:$source);
  let results = (outs SVEPredicateMask:$result);
  let assemblyFormat = "$source attr-dict `:` type($result)";
}

def ConvertToSvboolOp : ArmSVE_Op<"convert_to_svbool",
                                  [Pure,
                                   SvboolTypeConstraint<"source", "result">]> {
  let summary = "Convert a SVE predicate type to a svbool type";
  let description = [{
    Converts SVE predicate types (or vectors of predicate types, e.g.
    `vector<4x[4]xi1>`) to svbool types. Note: Only the trailing dimension can
    be scalable.

    Example 1: Convert a 1-D SVE predicate to a svbool mask.
    ```mlir
    %source = vector.create_mask %dim_size : vector<[4]xi1>
    %result = arm_sve.convert_to_svbool %source : vector<[4]xi1>
    // => Results in vector<[16]xi1>
    ```

    Example 2: Convert a 2-D mask of SVE predicates to a svbool mask.
    ```mlir
    %source = vector.create_mask %c2, %dim_size : vector<2x[2]xi1>
    %result = arm_sve.convert_to_svbool %source : vector<2x[2]xi1>
    // => Results in vector<2x[16]xi1>
    ```

    ---

    A `svbool` is the smallest SVE predicate type that has a in-memory
    representation (and maps to a full predicate register). In MLIR `svbool` is
    represented as `vector<[16]xi1>`. Smaller SVE predicate types
    (`vector<[1|2|4|8]xi1>`) must be converted to a `svbool` before they can be
    stored.
  }];
  let arguments = (ins SVEPredicateMask:$source);
  let results = (outs SVBoolMask:$result);
  let assemblyFormat = "$source attr-dict `:` type($source)";
}

// Inputs valid for the multi-vector zips (not including the 128-bit element zipqs)
def ZipInputVectorType : AnyTypeOf<[
  Scalable1DVectorOfLength<2, [I64, F64]>,
  Scalable1DVectorOfLength<4, [I32, F32]>,
  Scalable1DVectorOfLength<8, [I16, F16, BF16]>,
  Scalable1DVectorOfLength<16, [I8]>],
  "an SVE vector with element size <= 64-bit">;

def ZipX2Op : ArmSVE_Op<"zip.x2", [Pure,
                                   AllTypesMatch<["sourceV1", "sourceV2",
                                                  "resultV1", "resultV2"]>]> {
  let summary = "Multi-vector two-way zip op";

  let description = [{
    This operation interleaves elements from two input SVE vectors, returning
    two new SVE vectors (`resultV1` and `resultV2`), which contain the low and
    high halves of the result respectively.

    Example:
    ```mlir
    // sourceV1 = [ A1, A2, A3, ... An ]
    // sourceV2 = [ B1, B2, B3, ... Bn ]
    // (resultV1, resultV2) = [ A1, B1, A2, B2, A3, B3, ... An, Bn ]
    %resultV1, %resultV2 = arm_sve.zip.x2 %sourceV1, %sourceV2 : vector<[16]xi8>
    ```

    Note: This requires SME 2 (`+sme2` in LLVM target features)

    [Source](https://developer.arm.com/documentation/ddi0602/2023-12/SME-Instructions/ZIP--two-registers---Interleave-elements-from-two-vectors-?lang=en)
  }];

  let arguments = (ins ZipInputVectorType:$sourceV1,
                       ZipInputVectorType:$sourceV2);

  let results = (outs ZipInputVectorType:$resultV1,
                      ZipInputVectorType:$resultV2);

  let builders = [
    OpBuilder<(ins "Value":$v1, "Value":$v2), [{
      build($_builder, $_state, v1.getType(), v1.getType(), v1, v2);
  }]>];

  let assemblyFormat = "$sourceV1 `,` $sourceV2 attr-dict `:` type($sourceV1)";

  let extraClassDeclaration = [{
    VectorType getVectorType() {
      return ::llvm::cast<VectorType>(getSourceV1().getType());
    }
  }];
}

def ZipX4Op
  : ArmSVE_Op<"zip.x4",
              [Pure,
               AllTypesMatch<["sourceV1", "sourceV2", "sourceV3", "sourceV4",
                              "resultV1", "resultV2", "resultV3", "resultV4"]>]> {
  let summary = "Multi-vector four-way zip op";

  let description = [{
    This operation interleaves elements from four input SVE vectors, returning
    four new SVE vectors, each of which contain a quarter of the result. The
    first quarter will be in `resultV1`, second in `resultV2`, third in
    `resultV3`, and fourth in `resultV4`.

    ```mlir
    // sourceV1 = [ A1, A2, ... An ]
    // sourceV2 = [ B1, B2, ... Bn ]
    // sourceV3 = [ C1, C2, ... Cn ]
    // sourceV4 = [ D1, D2, ... Dn ]
    // (resultV1, resultV2, resultV3, resultV4)
    //   = [ A1, B1, C1, D1, A2, B2, C2, D2, ... An, Bn, Cn, Dn ]
    %resultV1, %resultV2, %resultV3, %resultV4 = arm_sve.zip.x4
      %sourceV1, %sourceV2, %sourceV3, %sourceV4 : vector<[16]xi8>
    ```

    **Warning:** The result of this op is undefined for 64-bit elements on
    hardware with less than 256-bit vectors!

    Note: This requires SME 2 (`+sme2` in LLVM target features)

    [Source](https://developer.arm.com/documentation/ddi0602/2023-12/SME-Instructions/ZIP--four-registers---Interleave-elements-from-four-vectors-?lang=en)
  }];

  let arguments = (ins ZipInputVectorType:$sourceV1,
                       ZipInputVectorType:$sourceV2,
                       ZipInputVectorType:$sourceV3,
                       ZipInputVectorType:$sourceV4);

  let results = (outs ZipInputVectorType:$resultV1,
                      ZipInputVectorType:$resultV2,
                      ZipInputVectorType:$resultV3,
                      ZipInputVectorType:$resultV4);

  let builders = [
    OpBuilder<(ins "Value":$v1, "Value":$v2, "Value":$v3, "Value":$v4), [{
      build($_builder, $_state,
        v1.getType(), v1.getType(),
        v1.getType(), v1.getType(),
        v1, v2, v3, v4);
  }]>];

  let assemblyFormat = [{
    $sourceV1 `,` $sourceV2 `,` $sourceV3 `,` $sourceV4 attr-dict
      `:` type($sourceV1)
  }];

  let extraClassDeclaration = [{
    VectorType getVectorType() {
      return ::llvm::cast<VectorType>(getSourceV1().getType());
    }
  }];
}

def PselOp : ArmSVE_Op<"psel", [Pure, AllTypesMatch<["p1", "result"]>]> {
  let summary = "Predicate select";

  let description = [{
    This operation returns the input predicate `p1` or an all-false predicate
    based on the bit at `p2[index]`. Informally, the semantics are:
    ```
    if p2[index % num_elements(p2)] == 1:
      return p1 : type(p1)
    return all-false : type(p1)
    ```

    Example:
    ```mlir
    // Note: p1 and p2 can have different sizes.
    %pd = arm_sve.psel %p1, %p2[%index] : vector<[4]xi1>, vector<[8]xi1>
    ```

    Note: This requires SME or SVE2.1 (`+sme` or `+sve2p1` in LLVM target features).
  }];

  let arguments = (ins SVEPredicate:$p1, SVEPredicate:$p2, Index:$index);
  let results = (outs SVEPredicate:$result);

  let builders = [
    OpBuilder<(ins "Value":$p1, "Value":$p2, "Value":$index), [{
      build($_builder, $_state, p1.getType(), p1, p2, index);
  }]>];

  let assemblyFormat = [{
    $p1 `,` $p2 `[` $index `]` attr-dict `:` type($p1) `,` type($p2)
  }];
}

def ScalableMaskedAddIOp : ScalableMaskedIOp<"masked.addi", "addition",
                                             [Commutative]>;

def ScalableMaskedAddFOp : ScalableMaskedFOp<"masked.addf", "addition",
                            [Commutative]>;

def ScalableMaskedSubIOp : ScalableMaskedIOp<"masked.subi", "subtraction">;

def ScalableMaskedSubFOp : ScalableMaskedFOp<"masked.subf", "subtraction">;

def ScalableMaskedMulIOp : ScalableMaskedIOp<"masked.muli", "multiplication",
                            [Commutative]>;

def ScalableMaskedMulFOp : ScalableMaskedFOp<"masked.mulf", "multiplication",
                            [Commutative]>;

def ScalableMaskedSDivIOp : ScalableMaskedIOp<"masked.divi_signed",
                                              "signed division">;

def ScalableMaskedUDivIOp : ScalableMaskedIOp<"masked.divi_unsigned",
                                              "unsigned division">;

def ScalableMaskedDivFOp : ScalableMaskedFOp<"masked.divf", "division">;

def DupQLaneOp : ArmSVE_Op<"dupq_lane", [Pure, AllTypesMatch<["src", "dst"]>]> {
  let summary = "Broadcast indexed 128-bit segment to vector";

  let description = [{
    This operation fills each 128-bit segment of a vector with the elements
    from the indexed 128-bit segment of the source vector. If the VL is
    128 bits the operation is a NOP. If the index exceeds the number of
    128-bit segments in a vector the result is an all-zeroes vector.

    Example:
    ```mlir
    // VL == 256
    // %X = [A B C D x x x x]
    %Y = arm_sve.dupq_lane %X[0] : vector<[4]xi32>
    // Y = [A B C D A B C D]

    // %U = [x x x x x x x x A B C D E F G H]
    %V = arm_sve.dupq_lane %U[1] : vector<[8]xf16>
    // %V = [A B C D E F H A B C D E F H]
    ```

    Note: The semantics of the operation match those of the `svdupq_lane` instrinsics.
    [Source](https://developer.arm.com/architectures/instruction-sets/intrinsics/#q=svdupq_lane)
  }];

  let arguments = (ins SVEVector:$src,
                       I64Attr:$lane);
  let results = (outs SVEVector:$dst);

  let builders = [
    OpBuilder<(ins "Value":$src, "int64_t":$lane), [{
      build($_builder, $_state, src.getType(), src, lane);
    }]>];

  let assemblyFormat = [{
    $src `[` $lane `]` attr-dict `:` type($dst)
  }];
}

def UmmlaIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"ummla">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def SmmlaIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"smmla">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def UsmmlaIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"usmmla">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def SdotIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"sdot">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def UdotIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"udot">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedAddIIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"add">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedAddFIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"fadd">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedMulIIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"mul">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedMulFIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"fmul">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedSubIIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"sub">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedSubFIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"fsub">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedSDivIIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"sdiv">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedUDivIIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"udiv">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ScalableMaskedDivFIntrOp :
  ArmSVE_IntrBinaryOverloadedOp<"fdiv">,
  Arguments<(ins AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank, AnyScalableVectorOfAnyRank)>;

def ConvertFromSvboolIntrOp :
  ArmSVE_IntrOp<"convert.from.svbool",
    [TypeIs<"res", SVEPredicate>],
    /*overloadedOperands=*/[],
    /*overloadedResults=*/[0]>,
  Arguments<(ins SVBool:$svbool)>;

def ConvertToSvboolIntrOp :
  ArmSVE_IntrOp<"convert.to.svbool",
    [TypeIs<"res", SVBool>],
    /*overloadedOperands=*/[0],
    /*overloadedResults=*/[]>,
    Arguments<(ins SVEPredicate:$mask)>;

// Note: This multi-vector intrinsic requires SME2.
def ZipX2IntrOp : ArmSVE_IntrOp<"zip.x2",
    /*traits=*/[],
    /*overloadedOperands=*/[0],
    /*overloadedResults=*/[],
    /*numResults=*/2>,
    Arguments<(ins Arg<AnyScalableVectorOfAnyRank, "v1">:$v1,
                   Arg<AnyScalableVectorOfAnyRank, "v2">:$v2)>;

// Note: This multi-vector intrinsic requires SME2.
def ZipX4IntrOp : ArmSVE_IntrOp<"zip.x4",
    /*traits=*/[],
    /*overloadedOperands=*/[0],
    /*overloadedResults=*/[],
    /*numResults=*/4>,
    Arguments<(ins Arg<AnyScalableVectorOfAnyRank, "v1">:$v1,
                   Arg<AnyScalableVectorOfAnyRank, "v2">:$v2,
                   Arg<AnyScalableVectorOfAnyRank, "v3">:$v3,
                   Arg<AnyScalableVectorOfAnyRank, "v3">:$v4)>;

// Note: This intrinsic requires SME or SVE2.1.
def PselIntrOp : ArmSVE_IntrOp<"psel",
  /*traits=*/[Pure, TypeIs<"res", SVBool>],
  /*overloadedOperands=*/[1]>,
  Arguments<(ins Arg<SVBool, "p1">:$p1,
                 Arg<SVEPredicate, "p2">:$p2,
                 Arg<I32, "index">:$index)>;

def WhileLTIntrOp :
  ArmSVE_IntrOp<"whilelt",
    [TypeIs<"res", SVEPredicate>, Pure],
    /*overloadedOperands=*/[0],
    /*overloadedResults=*/[0]>,
  Arguments<(ins I64:$base, I64:$n)>;

def DupQLaneIntrOp : ArmSVE_IntrOp<"dupq_lane",
    /*traits=*/[],
    /*overloadedOperands=*/[0],
    /*overloadedResults=*/[],
    /*numResults=*/1,
    /*immArgPositions*/[1],
    /*immArgAttrNames*/["lane"]>,
    Arguments<(ins Arg<ScalableVectorOfRank<[1]>, "v">:$v,
                   Arg<I64Attr, "lane">:$lane)>;

#endif // ARMSVE_OPS
