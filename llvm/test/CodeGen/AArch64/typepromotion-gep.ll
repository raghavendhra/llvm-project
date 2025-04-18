; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 5
; RUN: opt -S -p typepromotion %s -o - | FileCheck %s

target triple = "arm64-apple-macosx15.0.0"

define i4 @gep_offset_signedness(ptr %ptr, i8 %offset, i1 %cond) {
; CHECK-LABEL: define i4 @gep_offset_signedness(
; CHECK-SAME: ptr [[PTR:%.*]], i8 [[OFFSET:%.*]], i1 [[COND:%.*]]) {
; CHECK-NEXT:  [[ENTRY:.*]]:
; CHECK-NEXT:    [[UNUSED_TRUNC:%.*]] = trunc i8 [[OFFSET]] to i4
; CHECK-NEXT:    [[PTR_IDX:%.*]] = getelementptr i8, ptr [[PTR]], i8 [[OFFSET]]
; CHECK-NEXT:    [[COND_2:%.*]] = icmp uge ptr [[PTR_IDX]], [[PTR]]
; CHECK-NEXT:    br i1 [[COND_2]], label %[[RETURN:.*]], label %[[ELSE:.*]]
; CHECK:       [[RETURN]]:
; CHECK-NEXT:    [[RET_VAL:%.*]] = phi i4 [ 0, %[[ELSE_RET:.*]] ], [ 1, %[[ENTRY]] ], [ 0, %[[ELSE]] ]
; CHECK-NEXT:    ret i4 [[RET_VAL]]
; CHECK:       [[ELSE]]:
; CHECK-NEXT:    [[ADD:%.*]] = add nuw i8 0, 0
; CHECK-NEXT:    [[COND_3:%.*]] = icmp ult i8 [[ADD]], [[OFFSET]]
; CHECK-NEXT:    br i1 [[COND]], label %[[RETURN]], label %[[ELSE_RET]]
; CHECK:       [[ELSE_RET]]:
; CHECK-NEXT:    br label %[[RETURN]]
;
entry:
  %unused_trunc = trunc i8 %offset to i4
  %ptr.idx = getelementptr i8, ptr %ptr, i8 %offset
  %cond.2 = icmp uge ptr %ptr.idx, %ptr
  br i1 %cond.2, label %return, label %else

return:                                              ; preds = %else.ret, %else, %entry
  %ret.val = phi i4 [ 0, %else.ret ], [ 1, %entry ], [ 0, %else ]
  ret i4 %ret.val

else:                                                ; preds = %entry
  %add = add nuw i8 0, 0
  %cond.3 = icmp ult i8 %add, %offset
  br i1 %cond, label %return, label %else.ret

else.ret:                                            ; preds = %else
  br label %return
}
