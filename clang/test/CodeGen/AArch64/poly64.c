// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --version 5
// RUN: %clang_cc1 -triple arm64-none-linux-gnu -target-feature +neon -flax-vector-conversions=none\
// RUN:  -ffp-contract=fast -disable-O0-optnone -emit-llvm -o - %s | opt -S -passes=mem2reg,sroa \
// RUN:  | FileCheck %s

// REQUIRES: aarch64-registered-target || arm-registered-target

#include <arm_neon.h>

// CHECK-LABEL: define dso_local <1 x i64> @test_vceq_p64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[CMP_I:%.*]] = icmp eq <1 x i64> [[A]], [[B]]
// CHECK-NEXT:    [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK-NEXT:    ret <1 x i64> [[SEXT_I]]
//
uint64x1_t test_vceq_p64(poly64x1_t a, poly64x1_t b) {
  return vceq_p64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vceqq_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[CMP_I:%.*]] = icmp eq <2 x i64> [[A]], [[B]]
// CHECK-NEXT:    [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK-NEXT:    ret <2 x i64> [[SEXT_I]]
//
uint64x2_t test_vceqq_p64(poly64x2_t a, poly64x2_t b) {
  return vceqq_p64(a, b);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vtst_p64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[B]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    [[TMP4:%.*]] = and <1 x i64> [[TMP2]], [[TMP3]]
// CHECK-NEXT:    [[TMP5:%.*]] = icmp ne <1 x i64> [[TMP4]], zeroinitializer
// CHECK-NEXT:    [[VTST_I:%.*]] = sext <1 x i1> [[TMP5]] to <1 x i64>
// CHECK-NEXT:    ret <1 x i64> [[VTST_I]]
//
uint64x1_t test_vtst_p64(poly64x1_t a, poly64x1_t b) {
  return vtst_p64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vtstq_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[B]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    [[TMP4:%.*]] = and <2 x i64> [[TMP2]], [[TMP3]]
// CHECK-NEXT:    [[TMP5:%.*]] = icmp ne <2 x i64> [[TMP4]], zeroinitializer
// CHECK-NEXT:    [[VTST_I:%.*]] = sext <2 x i1> [[TMP5]] to <2 x i64>
// CHECK-NEXT:    ret <2 x i64> [[VTST_I]]
//
uint64x2_t test_vtstq_p64(poly64x2_t a, poly64x2_t b) {
  return vtstq_p64(a, b);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vbsl_p64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]], <1 x i64> noundef [[C:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[B]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <1 x i64> [[C]] to <8 x i8>
// CHECK-NEXT:    [[VBSL_I:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[VBSL1_I:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    [[VBSL2_I:%.*]] = bitcast <8 x i8> [[TMP2]] to <1 x i64>
// CHECK-NEXT:    [[VBSL3_I:%.*]] = and <1 x i64> [[VBSL_I]], [[VBSL1_I]]
// CHECK-NEXT:    [[TMP3:%.*]] = xor <1 x i64> [[VBSL_I]], splat (i64 -1)
// CHECK-NEXT:    [[VBSL4_I:%.*]] = and <1 x i64> [[TMP3]], [[VBSL2_I]]
// CHECK-NEXT:    [[VBSL5_I:%.*]] = or <1 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK-NEXT:    ret <1 x i64> [[VBSL5_I]]
//
poly64x1_t test_vbsl_p64(poly64x1_t a, poly64x1_t b, poly64x1_t c) {
  return vbsl_p64(a, b, c);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vbslq_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]], <2 x i64> noundef [[C:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[B]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <2 x i64> [[C]] to <16 x i8>
// CHECK-NEXT:    [[VBSL_I:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[VBSL1_I:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    [[VBSL2_I:%.*]] = bitcast <16 x i8> [[TMP2]] to <2 x i64>
// CHECK-NEXT:    [[VBSL3_I:%.*]] = and <2 x i64> [[VBSL_I]], [[VBSL1_I]]
// CHECK-NEXT:    [[TMP3:%.*]] = xor <2 x i64> [[VBSL_I]], splat (i64 -1)
// CHECK-NEXT:    [[VBSL4_I:%.*]] = and <2 x i64> [[TMP3]], [[VBSL2_I]]
// CHECK-NEXT:    [[VBSL5_I:%.*]] = or <2 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK-NEXT:    ret <2 x i64> [[VBSL5_I]]
//
poly64x2_t test_vbslq_p64(poly64x2_t a, poly64x2_t b, poly64x2_t c) {
  return vbslq_p64(a, b, c);
}

// CHECK-LABEL: define dso_local i64 @test_vget_lane_p64(
// CHECK-SAME: <1 x i64> noundef [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <1 x i64> [[V]], i32 0
// CHECK-NEXT:    ret i64 [[VGET_LANE]]
//
poly64_t test_vget_lane_p64(poly64x1_t v) {
  return vget_lane_p64(v, 0);
}

// CHECK-LABEL: define dso_local i64 @test_vgetq_lane_p64(
// CHECK-SAME: <2 x i64> noundef [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <2 x i64> [[V]], i32 1
// CHECK-NEXT:    ret i64 [[VGETQ_LANE]]
//
poly64_t test_vgetq_lane_p64(poly64x2_t v) {
  return vgetq_lane_p64(v, 1);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vset_lane_p64(
// CHECK-SAME: i64 noundef [[A:%.*]], <1 x i64> noundef [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VSET_LANE:%.*]] = insertelement <1 x i64> [[V]], i64 [[A]], i32 0
// CHECK-NEXT:    ret <1 x i64> [[VSET_LANE]]
//
poly64x1_t test_vset_lane_p64(poly64_t a, poly64x1_t v) {
  return vset_lane_p64(a, v, 0);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vsetq_lane_p64(
// CHECK-SAME: i64 noundef [[A:%.*]], <2 x i64> noundef [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VSET_LANE:%.*]] = insertelement <2 x i64> [[V]], i64 [[A]], i32 1
// CHECK-NEXT:    ret <2 x i64> [[VSET_LANE]]
//
poly64x2_t test_vsetq_lane_p64(poly64_t a, poly64x2_t v) {
  return vsetq_lane_p64(a, v, 1);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vcopy_lane_p64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <1 x i64> [[B]], i32 0
// CHECK-NEXT:    [[VSET_LANE:%.*]] = insertelement <1 x i64> [[A]], i64 [[VGET_LANE]], i32 0
// CHECK-NEXT:    ret <1 x i64> [[VSET_LANE]]
//
poly64x1_t test_vcopy_lane_p64(poly64x1_t a, poly64x1_t b) {
  return vcopy_lane_p64(a, 0, b, 0);

}

// CHECK-LABEL: define dso_local <2 x i64> @test_vcopyq_lane_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGET_LANE:%.*]] = extractelement <1 x i64> [[B]], i32 0
// CHECK-NEXT:    [[VSET_LANE:%.*]] = insertelement <2 x i64> [[A]], i64 [[VGET_LANE]], i32 1
// CHECK-NEXT:    ret <2 x i64> [[VSET_LANE]]
//
poly64x2_t test_vcopyq_lane_p64(poly64x2_t a, poly64x1_t b) {
  return vcopyq_lane_p64(a, 1, b, 0);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vcopyq_laneq_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VGETQ_LANE:%.*]] = extractelement <2 x i64> [[B]], i32 1
// CHECK-NEXT:    [[VSET_LANE:%.*]] = insertelement <2 x i64> [[A]], i64 [[VGETQ_LANE]], i32 1
// CHECK-NEXT:    ret <2 x i64> [[VSET_LANE]]
//
poly64x2_t test_vcopyq_laneq_p64(poly64x2_t a, poly64x2_t b) {
  return vcopyq_laneq_p64(a, 1, b, 1);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vcreate_p64(
// CHECK-SAME: i64 noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast i64 [[A]] to <1 x i64>
// CHECK-NEXT:    ret <1 x i64> [[TMP0]]
//
poly64x1_t test_vcreate_p64(uint64_t a) {
  return vcreate_p64(a);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vdup_n_p64(
// CHECK-SAME: i64 noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VECINIT_I:%.*]] = insertelement <1 x i64> poison, i64 [[A]], i32 0
// CHECK-NEXT:    ret <1 x i64> [[VECINIT_I]]
//
poly64x1_t test_vdup_n_p64(poly64_t a) {
  return vdup_n_p64(a);
}
// CHECK-LABEL: define dso_local <2 x i64> @test_vdupq_n_p64(
// CHECK-SAME: i64 noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VECINIT_I:%.*]] = insertelement <2 x i64> poison, i64 [[A]], i32 0
// CHECK-NEXT:    [[VECINIT1_I:%.*]] = insertelement <2 x i64> [[VECINIT_I]], i64 [[A]], i32 1
// CHECK-NEXT:    ret <2 x i64> [[VECINIT1_I]]
//
poly64x2_t test_vdupq_n_p64(poly64_t a) {
  return vdupq_n_p64(a);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vmov_n_p64(
// CHECK-SAME: i64 noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VECINIT_I:%.*]] = insertelement <1 x i64> poison, i64 [[A]], i32 0
// CHECK-NEXT:    ret <1 x i64> [[VECINIT_I]]
//
poly64x1_t test_vmov_n_p64(poly64_t a) {
  return vmov_n_p64(a);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vmovq_n_p64(
// CHECK-SAME: i64 noundef [[A:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VECINIT_I:%.*]] = insertelement <2 x i64> poison, i64 [[A]], i32 0
// CHECK-NEXT:    [[VECINIT1_I:%.*]] = insertelement <2 x i64> [[VECINIT_I]], i64 [[A]], i32 1
// CHECK-NEXT:    ret <2 x i64> [[VECINIT1_I]]
//
poly64x2_t test_vmovq_n_p64(poly64_t a) {
  return vmovq_n_p64(a);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vdup_lane_p64(
// CHECK-SAME: <1 x i64> noundef [[VEC:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[VEC]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <1 x i64> [[TMP1]], <1 x i64> [[TMP1]], <1 x i32> zeroinitializer
// CHECK-NEXT:    ret <1 x i64> [[LANE]]
//
poly64x1_t test_vdup_lane_p64(poly64x1_t vec) {
  return vdup_lane_p64(vec, 0);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vdupq_lane_p64(
// CHECK-SAME: <1 x i64> noundef [[VEC:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[VEC]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <1 x i64> [[TMP1]], <1 x i64> [[TMP1]], <2 x i32> zeroinitializer
// CHECK-NEXT:    ret <2 x i64> [[LANE]]
//
poly64x2_t test_vdupq_lane_p64(poly64x1_t vec) {
  return vdupq_lane_p64(vec, 0);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vdupq_laneq_p64(
// CHECK-SAME: <2 x i64> noundef [[VEC:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[VEC]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[LANE:%.*]] = shufflevector <2 x i64> [[TMP1]], <2 x i64> [[TMP1]], <2 x i32> <i32 1, i32 1>
// CHECK-NEXT:    ret <2 x i64> [[LANE]]
//
poly64x2_t test_vdupq_laneq_p64(poly64x2_t vec) {
  return vdupq_laneq_p64(vec, 1);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vcombine_p64(
// CHECK-SAME: <1 x i64> noundef [[LOW:%.*]], <1 x i64> noundef [[HIGH:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <1 x i64> [[LOW]], <1 x i64> [[HIGH]], <2 x i32> <i32 0, i32 1>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vcombine_p64(poly64x1_t low, poly64x1_t high) {
  return vcombine_p64(low, high);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vld1_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = load <1 x i64>, ptr [[PTR]], align 8
// CHECK-NEXT:    ret <1 x i64> [[TMP0]]
//
poly64x1_t test_vld1_p64(poly64_t const * ptr) {
  return vld1_p64(ptr);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vld1q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = load <2 x i64>, ptr [[PTR]], align 8
// CHECK-NEXT:    ret <2 x i64> [[TMP0]]
//
poly64x2_t test_vld1q_p64(poly64_t const * ptr) {
  return vld1q_p64(ptr);
}

// CHECK-LABEL: define dso_local void @test_vst1_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], <1 x i64> noundef [[VAL:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[VAL]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    store <1 x i64> [[TMP1]], ptr [[PTR]], align 8
// CHECK-NEXT:    ret void
//
void test_vst1_p64(poly64_t * ptr, poly64x1_t val) {
  return vst1_p64(ptr, val);
}

// CHECK-LABEL: define dso_local void @test_vst1q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], <2 x i64> noundef [[VAL:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[VAL]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    store <2 x i64> [[TMP1]], ptr [[PTR]], align 8
// CHECK-NEXT:    ret void
//
void test_vst1q_p64(poly64_t * ptr, poly64x2_t val) {
  return vst1q_p64(ptr, val);
}

// CHECK-LABEL: define dso_local %struct.poly64x1x2_t @test_vld2_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VLD2:%.*]] = call { <1 x i64>, <1 x i64> } @llvm.aarch64.neon.ld2.v1i64.p0(ptr [[PTR]])
// CHECK-NEXT:    [[VLD2_FCA_0_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64> } [[VLD2]], 0
// CHECK-NEXT:    [[VLD2_FCA_1_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64> } [[VLD2]], 1
// CHECK-NEXT:    [[DOTFCA_0_0_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X2_T:%.*]] poison, <1 x i64> [[VLD2_FCA_0_EXTRACT]], 0, 0
// CHECK-NEXT:    [[DOTFCA_0_1_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X2_T]] [[DOTFCA_0_0_INSERT]], <1 x i64> [[VLD2_FCA_1_EXTRACT]], 0, 1
// CHECK-NEXT:    ret [[STRUCT_POLY64X1X2_T]] [[DOTFCA_0_1_INSERT]]
//
poly64x1x2_t test_vld2_p64(poly64_t const * ptr) {
  return vld2_p64(ptr);
}

// CHECK-LABEL: define dso_local %struct.poly64x2x2_t @test_vld2q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VLD2:%.*]] = call { <2 x i64>, <2 x i64> } @llvm.aarch64.neon.ld2.v2i64.p0(ptr [[PTR]])
// CHECK-NEXT:    [[VLD2_FCA_0_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64> } [[VLD2]], 0
// CHECK-NEXT:    [[VLD2_FCA_1_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64> } [[VLD2]], 1
// CHECK-NEXT:    [[DOTFCA_0_0_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X2_T:%.*]] poison, <2 x i64> [[VLD2_FCA_0_EXTRACT]], 0, 0
// CHECK-NEXT:    [[DOTFCA_0_1_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X2_T]] [[DOTFCA_0_0_INSERT]], <2 x i64> [[VLD2_FCA_1_EXTRACT]], 0, 1
// CHECK-NEXT:    ret [[STRUCT_POLY64X2X2_T]] [[DOTFCA_0_1_INSERT]]
//
poly64x2x2_t test_vld2q_p64(poly64_t const * ptr) {
  return vld2q_p64(ptr);
}

// CHECK-LABEL: define dso_local %struct.poly64x1x3_t @test_vld3_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VLD3:%.*]] = call { <1 x i64>, <1 x i64>, <1 x i64> } @llvm.aarch64.neon.ld3.v1i64.p0(ptr [[PTR]])
// CHECK-NEXT:    [[VLD3_FCA_0_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64> } [[VLD3]], 0
// CHECK-NEXT:    [[VLD3_FCA_1_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64> } [[VLD3]], 1
// CHECK-NEXT:    [[VLD3_FCA_2_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64> } [[VLD3]], 2
// CHECK-NEXT:    [[DOTFCA_0_0_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X3_T:%.*]] poison, <1 x i64> [[VLD3_FCA_0_EXTRACT]], 0, 0
// CHECK-NEXT:    [[DOTFCA_0_1_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X3_T]] [[DOTFCA_0_0_INSERT]], <1 x i64> [[VLD3_FCA_1_EXTRACT]], 0, 1
// CHECK-NEXT:    [[DOTFCA_0_2_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X3_T]] [[DOTFCA_0_1_INSERT]], <1 x i64> [[VLD3_FCA_2_EXTRACT]], 0, 2
// CHECK-NEXT:    ret [[STRUCT_POLY64X1X3_T]] [[DOTFCA_0_2_INSERT]]
//
poly64x1x3_t test_vld3_p64(poly64_t const * ptr) {
  return vld3_p64(ptr);
}

// CHECK-LABEL: define dso_local %struct.poly64x2x3_t @test_vld3q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VLD3:%.*]] = call { <2 x i64>, <2 x i64>, <2 x i64> } @llvm.aarch64.neon.ld3.v2i64.p0(ptr [[PTR]])
// CHECK-NEXT:    [[VLD3_FCA_0_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64> } [[VLD3]], 0
// CHECK-NEXT:    [[VLD3_FCA_1_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64> } [[VLD3]], 1
// CHECK-NEXT:    [[VLD3_FCA_2_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64> } [[VLD3]], 2
// CHECK-NEXT:    [[DOTFCA_0_0_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X3_T:%.*]] poison, <2 x i64> [[VLD3_FCA_0_EXTRACT]], 0, 0
// CHECK-NEXT:    [[DOTFCA_0_1_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X3_T]] [[DOTFCA_0_0_INSERT]], <2 x i64> [[VLD3_FCA_1_EXTRACT]], 0, 1
// CHECK-NEXT:    [[DOTFCA_0_2_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X3_T]] [[DOTFCA_0_1_INSERT]], <2 x i64> [[VLD3_FCA_2_EXTRACT]], 0, 2
// CHECK-NEXT:    ret [[STRUCT_POLY64X2X3_T]] [[DOTFCA_0_2_INSERT]]
//
poly64x2x3_t test_vld3q_p64(poly64_t const * ptr) {
  return vld3q_p64(ptr);
}

// CHECK-LABEL: define dso_local %struct.poly64x1x4_t @test_vld4_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VLD4:%.*]] = call { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64> } @llvm.aarch64.neon.ld4.v1i64.p0(ptr [[PTR]])
// CHECK-NEXT:    [[VLD4_FCA_0_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64> } [[VLD4]], 0
// CHECK-NEXT:    [[VLD4_FCA_1_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64> } [[VLD4]], 1
// CHECK-NEXT:    [[VLD4_FCA_2_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64> } [[VLD4]], 2
// CHECK-NEXT:    [[VLD4_FCA_3_EXTRACT:%.*]] = extractvalue { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64> } [[VLD4]], 3
// CHECK-NEXT:    [[DOTFCA_0_0_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X4_T:%.*]] poison, <1 x i64> [[VLD4_FCA_0_EXTRACT]], 0, 0
// CHECK-NEXT:    [[DOTFCA_0_1_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X4_T]] [[DOTFCA_0_0_INSERT]], <1 x i64> [[VLD4_FCA_1_EXTRACT]], 0, 1
// CHECK-NEXT:    [[DOTFCA_0_2_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X4_T]] [[DOTFCA_0_1_INSERT]], <1 x i64> [[VLD4_FCA_2_EXTRACT]], 0, 2
// CHECK-NEXT:    [[DOTFCA_0_3_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X1X4_T]] [[DOTFCA_0_2_INSERT]], <1 x i64> [[VLD4_FCA_3_EXTRACT]], 0, 3
// CHECK-NEXT:    ret [[STRUCT_POLY64X1X4_T]] [[DOTFCA_0_3_INSERT]]
//
poly64x1x4_t test_vld4_p64(poly64_t const * ptr) {
  return vld4_p64(ptr);
}

// CHECK-LABEL: define dso_local %struct.poly64x2x4_t @test_vld4q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VLD4:%.*]] = call { <2 x i64>, <2 x i64>, <2 x i64>, <2 x i64> } @llvm.aarch64.neon.ld4.v2i64.p0(ptr [[PTR]])
// CHECK-NEXT:    [[VLD4_FCA_0_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64>, <2 x i64> } [[VLD4]], 0
// CHECK-NEXT:    [[VLD4_FCA_1_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64>, <2 x i64> } [[VLD4]], 1
// CHECK-NEXT:    [[VLD4_FCA_2_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64>, <2 x i64> } [[VLD4]], 2
// CHECK-NEXT:    [[VLD4_FCA_3_EXTRACT:%.*]] = extractvalue { <2 x i64>, <2 x i64>, <2 x i64>, <2 x i64> } [[VLD4]], 3
// CHECK-NEXT:    [[DOTFCA_0_0_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X4_T:%.*]] poison, <2 x i64> [[VLD4_FCA_0_EXTRACT]], 0, 0
// CHECK-NEXT:    [[DOTFCA_0_1_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X4_T]] [[DOTFCA_0_0_INSERT]], <2 x i64> [[VLD4_FCA_1_EXTRACT]], 0, 1
// CHECK-NEXT:    [[DOTFCA_0_2_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X4_T]] [[DOTFCA_0_1_INSERT]], <2 x i64> [[VLD4_FCA_2_EXTRACT]], 0, 2
// CHECK-NEXT:    [[DOTFCA_0_3_INSERT:%.*]] = insertvalue [[STRUCT_POLY64X2X4_T]] [[DOTFCA_0_2_INSERT]], <2 x i64> [[VLD4_FCA_3_EXTRACT]], 0, 3
// CHECK-NEXT:    ret [[STRUCT_POLY64X2X4_T]] [[DOTFCA_0_3_INSERT]]
//
poly64x2x4_t test_vld4q_p64(poly64_t const * ptr) {
  return vld4q_p64(ptr);
}

// CHECK-LABEL: define dso_local void @test_vst2_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], [2 x <1 x i64>] alignstack(8) [[VAL_COERCE:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VAL_COERCE_FCA_0_EXTRACT:%.*]] = extractvalue [2 x <1 x i64>] [[VAL_COERCE]], 0
// CHECK-NEXT:    [[VAL_COERCE_FCA_1_EXTRACT:%.*]] = extractvalue [2 x <1 x i64>] [[VAL_COERCE]], 1
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_0_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_1_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    call void @llvm.aarch64.neon.st2.v1i64.p0(<1 x i64> [[TMP2]], <1 x i64> [[TMP3]], ptr [[PTR]])
// CHECK-NEXT:    ret void
//
void test_vst2_p64(poly64_t * ptr, poly64x1x2_t val) {
  return vst2_p64(ptr, val);
}

// CHECK-LABEL: define dso_local void @test_vst2q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], [2 x <2 x i64>] alignstack(16) [[VAL_COERCE:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VAL_COERCE_FCA_0_EXTRACT:%.*]] = extractvalue [2 x <2 x i64>] [[VAL_COERCE]], 0
// CHECK-NEXT:    [[VAL_COERCE_FCA_1_EXTRACT:%.*]] = extractvalue [2 x <2 x i64>] [[VAL_COERCE]], 1
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_0_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_1_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    call void @llvm.aarch64.neon.st2.v2i64.p0(<2 x i64> [[TMP2]], <2 x i64> [[TMP3]], ptr [[PTR]])
// CHECK-NEXT:    ret void
//
void test_vst2q_p64(poly64_t * ptr, poly64x2x2_t val) {
  return vst2q_p64(ptr, val);
}

// CHECK-LABEL: define dso_local void @test_vst3_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], [3 x <1 x i64>] alignstack(8) [[VAL_COERCE:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VAL_COERCE_FCA_0_EXTRACT:%.*]] = extractvalue [3 x <1 x i64>] [[VAL_COERCE]], 0
// CHECK-NEXT:    [[VAL_COERCE_FCA_1_EXTRACT:%.*]] = extractvalue [3 x <1 x i64>] [[VAL_COERCE]], 1
// CHECK-NEXT:    [[VAL_COERCE_FCA_2_EXTRACT:%.*]] = extractvalue [3 x <1 x i64>] [[VAL_COERCE]], 2
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_0_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_1_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_2_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    [[TMP5:%.*]] = bitcast <8 x i8> [[TMP2]] to <1 x i64>
// CHECK-NEXT:    call void @llvm.aarch64.neon.st3.v1i64.p0(<1 x i64> [[TMP3]], <1 x i64> [[TMP4]], <1 x i64> [[TMP5]], ptr [[PTR]])
// CHECK-NEXT:    ret void
//
void test_vst3_p64(poly64_t * ptr, poly64x1x3_t val) {
  return vst3_p64(ptr, val);
}

// CHECK-LABEL: define dso_local void @test_vst3q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], [3 x <2 x i64>] alignstack(16) [[VAL_COERCE:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VAL_COERCE_FCA_0_EXTRACT:%.*]] = extractvalue [3 x <2 x i64>] [[VAL_COERCE]], 0
// CHECK-NEXT:    [[VAL_COERCE_FCA_1_EXTRACT:%.*]] = extractvalue [3 x <2 x i64>] [[VAL_COERCE]], 1
// CHECK-NEXT:    [[VAL_COERCE_FCA_2_EXTRACT:%.*]] = extractvalue [3 x <2 x i64>] [[VAL_COERCE]], 2
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_0_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_1_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_2_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    [[TMP5:%.*]] = bitcast <16 x i8> [[TMP2]] to <2 x i64>
// CHECK-NEXT:    call void @llvm.aarch64.neon.st3.v2i64.p0(<2 x i64> [[TMP3]], <2 x i64> [[TMP4]], <2 x i64> [[TMP5]], ptr [[PTR]])
// CHECK-NEXT:    ret void
//
void test_vst3q_p64(poly64_t * ptr, poly64x2x3_t val) {
  return vst3q_p64(ptr, val);
}

// CHECK-LABEL: define dso_local void @test_vst4_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], [4 x <1 x i64>] alignstack(8) [[VAL_COERCE:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VAL_COERCE_FCA_0_EXTRACT:%.*]] = extractvalue [4 x <1 x i64>] [[VAL_COERCE]], 0
// CHECK-NEXT:    [[VAL_COERCE_FCA_1_EXTRACT:%.*]] = extractvalue [4 x <1 x i64>] [[VAL_COERCE]], 1
// CHECK-NEXT:    [[VAL_COERCE_FCA_2_EXTRACT:%.*]] = extractvalue [4 x <1 x i64>] [[VAL_COERCE]], 2
// CHECK-NEXT:    [[VAL_COERCE_FCA_3_EXTRACT:%.*]] = extractvalue [4 x <1 x i64>] [[VAL_COERCE]], 3
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_0_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_1_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_2_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <1 x i64> [[VAL_COERCE_FCA_3_EXTRACT]] to <8 x i8>
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[TMP5:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    [[TMP6:%.*]] = bitcast <8 x i8> [[TMP2]] to <1 x i64>
// CHECK-NEXT:    [[TMP7:%.*]] = bitcast <8 x i8> [[TMP3]] to <1 x i64>
// CHECK-NEXT:    call void @llvm.aarch64.neon.st4.v1i64.p0(<1 x i64> [[TMP4]], <1 x i64> [[TMP5]], <1 x i64> [[TMP6]], <1 x i64> [[TMP7]], ptr [[PTR]])
// CHECK-NEXT:    ret void
//
void test_vst4_p64(poly64_t * ptr, poly64x1x4_t val) {
  return vst4_p64(ptr, val);
}

// CHECK-LABEL: define dso_local void @test_vst4q_p64(
// CHECK-SAME: ptr noundef [[PTR:%.*]], [4 x <2 x i64>] alignstack(16) [[VAL_COERCE:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[VAL_COERCE_FCA_0_EXTRACT:%.*]] = extractvalue [4 x <2 x i64>] [[VAL_COERCE]], 0
// CHECK-NEXT:    [[VAL_COERCE_FCA_1_EXTRACT:%.*]] = extractvalue [4 x <2 x i64>] [[VAL_COERCE]], 1
// CHECK-NEXT:    [[VAL_COERCE_FCA_2_EXTRACT:%.*]] = extractvalue [4 x <2 x i64>] [[VAL_COERCE]], 2
// CHECK-NEXT:    [[VAL_COERCE_FCA_3_EXTRACT:%.*]] = extractvalue [4 x <2 x i64>] [[VAL_COERCE]], 3
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_0_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_1_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_2_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <2 x i64> [[VAL_COERCE_FCA_3_EXTRACT]] to <16 x i8>
// CHECK-NEXT:    [[TMP4:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[TMP5:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    [[TMP6:%.*]] = bitcast <16 x i8> [[TMP2]] to <2 x i64>
// CHECK-NEXT:    [[TMP7:%.*]] = bitcast <16 x i8> [[TMP3]] to <2 x i64>
// CHECK-NEXT:    call void @llvm.aarch64.neon.st4.v2i64.p0(<2 x i64> [[TMP4]], <2 x i64> [[TMP5]], <2 x i64> [[TMP6]], <2 x i64> [[TMP7]], ptr [[PTR]])
// CHECK-NEXT:    ret void
//
void test_vst4q_p64(poly64_t * ptr, poly64x2x4_t val) {
  return vst4q_p64(ptr, val);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vext_p64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[B]] to <8 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    [[VEXT:%.*]] = shufflevector <1 x i64> [[TMP2]], <1 x i64> [[TMP3]], <1 x i32> zeroinitializer
// CHECK-NEXT:    ret <1 x i64> [[VEXT]]
//
poly64x1_t test_vext_p64(poly64x1_t a, poly64x1_t b) {
  return vext_u64(a, b, 0);

}

// CHECK-LABEL: define dso_local <2 x i64> @test_vextq_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[B]] to <16 x i8>
// CHECK-NEXT:    [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    [[VEXT:%.*]] = shufflevector <2 x i64> [[TMP2]], <2 x i64> [[TMP3]], <2 x i32> <i32 1, i32 2>
// CHECK-NEXT:    ret <2 x i64> [[VEXT]]
//
poly64x2_t test_vextq_p64(poly64x2_t a, poly64x2_t b) {
  return vextq_p64(a, b, 1);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vzip1q_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> [[A]], <2 x i64> [[B]], <2 x i32> <i32 0, i32 2>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vzip1q_p64(poly64x2_t a, poly64x2_t b) {
  return vzip1q_p64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vzip2q_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> [[A]], <2 x i64> [[B]], <2 x i32> <i32 1, i32 3>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vzip2q_p64(poly64x2_t a, poly64x2_t b) {
  return vzip2q_u64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vuzp1q_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> [[A]], <2 x i64> [[B]], <2 x i32> <i32 0, i32 2>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vuzp1q_p64(poly64x2_t a, poly64x2_t b) {
  return vuzp1q_p64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vuzp2q_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> [[A]], <2 x i64> [[B]], <2 x i32> <i32 1, i32 3>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vuzp2q_p64(poly64x2_t a, poly64x2_t b) {
  return vuzp2q_u64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vtrn1q_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> [[A]], <2 x i64> [[B]], <2 x i32> <i32 0, i32 2>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vtrn1q_p64(poly64x2_t a, poly64x2_t b) {
  return vtrn1q_p64(a, b);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vtrn2q_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> [[A]], <2 x i64> [[B]], <2 x i32> <i32 1, i32 3>
// CHECK-NEXT:    ret <2 x i64> [[SHUFFLE_I]]
//
poly64x2_t test_vtrn2q_p64(poly64x2_t a, poly64x2_t b) {
  return vtrn2q_u64(a, b);
}

// CHECK-LABEL: define dso_local <1 x i64> @test_vsri_n_p64(
// CHECK-SAME: <1 x i64> noundef [[A:%.*]], <1 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <1 x i64> [[A]] to <8 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <1 x i64> [[B]] to <8 x i8>
// CHECK-NEXT:    [[VSRI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK-NEXT:    [[VSRI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK-NEXT:    [[VSRI_N2:%.*]] = call <1 x i64> @llvm.aarch64.neon.vsri.v1i64(<1 x i64> [[VSRI_N]], <1 x i64> [[VSRI_N1]], i32 33)
// CHECK-NEXT:    ret <1 x i64> [[VSRI_N2]]
//
poly64x1_t test_vsri_n_p64(poly64x1_t a, poly64x1_t b) {
  return vsri_n_p64(a, b, 33);
}

// CHECK-LABEL: define dso_local <2 x i64> @test_vsriq_n_p64(
// CHECK-SAME: <2 x i64> noundef [[A:%.*]], <2 x i64> noundef [[B:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    [[TMP0:%.*]] = bitcast <2 x i64> [[A]] to <16 x i8>
// CHECK-NEXT:    [[TMP1:%.*]] = bitcast <2 x i64> [[B]] to <16 x i8>
// CHECK-NEXT:    [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK-NEXT:    [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK-NEXT:    [[VSRI_N2:%.*]] = call <2 x i64> @llvm.aarch64.neon.vsri.v2i64(<2 x i64> [[VSRI_N]], <2 x i64> [[VSRI_N1]], i32 64)
// CHECK-NEXT:    ret <2 x i64> [[VSRI_N2]]
//
poly64x2_t test_vsriq_n_p64(poly64x2_t a, poly64x2_t b) {
  return vsriq_n_p64(a, b, 64);
}
