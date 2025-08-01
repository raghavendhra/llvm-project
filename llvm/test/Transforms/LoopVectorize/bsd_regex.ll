; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -passes=loop-vectorize,dce,instcombine -force-vector-width=2 -force-vector-interleave=2 < %s | FileCheck %s

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"

;PR 15830.

; When scalarizing stores we need to preserve the original order.
; Make sure that we are extracting in the correct order (0101, and not 0011).

define i32 @foo(ptr nocapture %A) {
; CHECK-LABEL: @foo(
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br i1 false, label [[SCALAR_PH:%.*]], label [[VECTOR_PH:%.*]]
; CHECK:       vector.ph:
; CHECK-NEXT:    br label [[VECTOR_BODY:%.*]]
; CHECK:       vector.body:
; CHECK-NEXT:    [[INDEX:%.*]] = phi i64 [ 0, [[VECTOR_PH]] ], [ [[INDEX_NEXT:%.*]], [[VECTOR_BODY]] ]
; CHECK-NEXT:    [[VEC_IND:%.*]] = phi <2 x i64> [ <i64 0, i64 1>, [[VECTOR_PH]] ], [ [[VEC_IND_NEXT:%.*]], [[VECTOR_BODY]] ]
; CHECK-NEXT:    [[TMP0:%.*]] = shl nsw <2 x i64> [[VEC_IND]], splat (i64 2)
; CHECK-NEXT:    [[STEP_ADD:%.*]] = shl <2 x i64> [[VEC_IND]], splat (i64 2)
; CHECK-NEXT:    [[TMP1:%.*]] = add <2 x i64> [[STEP_ADD]], splat (i64 8)
; CHECK-NEXT:    [[TMP2:%.*]] = extractelement <2 x i64> [[TMP0]], i64 0
; CHECK-NEXT:    [[TMP3:%.*]] = getelementptr inbounds i32, ptr [[A:%.*]], i64 [[TMP2]]
; CHECK-NEXT:    [[TMP4:%.*]] = extractelement <2 x i64> [[TMP0]], i64 1
; CHECK-NEXT:    [[TMP5:%.*]] = getelementptr inbounds i32, ptr [[A]], i64 [[TMP4]]
; CHECK-NEXT:    [[TMP6:%.*]] = extractelement <2 x i64> [[TMP1]], i64 0
; CHECK-NEXT:    [[TMP7:%.*]] = getelementptr inbounds i32, ptr [[A]], i64 [[TMP6]]
; CHECK-NEXT:    [[TMP8:%.*]] = extractelement <2 x i64> [[TMP1]], i64 1
; CHECK-NEXT:    [[TMP9:%.*]] = getelementptr inbounds i32, ptr [[A]], i64 [[TMP8]]
; CHECK-NEXT:    store i32 4, ptr [[TMP3]], align 4
; CHECK-NEXT:    store i32 4, ptr [[TMP5]], align 4
; CHECK-NEXT:    store i32 4, ptr [[TMP7]], align 4
; CHECK-NEXT:    store i32 4, ptr [[TMP9]], align 4
; CHECK-NEXT:    [[INDEX_NEXT]] = add nuw i64 [[INDEX]], 4
; CHECK-NEXT:    [[VEC_IND_NEXT]] = add <2 x i64> [[VEC_IND]], splat (i64 4)
; CHECK-NEXT:    [[TMP10:%.*]] = icmp eq i64 [[INDEX_NEXT]], 10000
; CHECK-NEXT:    br i1 [[TMP10]], label [[MIDDLE_BLOCK:%.*]], label [[VECTOR_BODY]], !llvm.loop [[LOOP0:![0-9]+]]
; CHECK:       middle.block:
; CHECK-NEXT:    br label [[FOR_END:%.*]]
; CHECK:       scalar.ph:
; CHECK-NEXT:    br label [[FOR_BODY:%.*]]
; CHECK:       for.body:
; CHECK-NEXT:    br i1 poison, label [[FOR_END]], label [[FOR_BODY]], !llvm.loop [[LOOP3:![0-9]+]]
; CHECK:       for.end:
; CHECK-NEXT:    ret i32 undef
;
entry:
  br label %for.body

for.body:
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %for.body ]
  %0 = shl nsw i64 %indvars.iv, 2
  %arrayidx = getelementptr inbounds i32, ptr %A, i64 %0
  store i32 4, ptr %arrayidx, align 4
  %indvars.iv.next = add i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next to i32
  %exitcond = icmp eq i32 %lftr.wideiv, 10000
  br i1 %exitcond, label %for.end, label %for.body

for.end:
  ret i32 undef
}


