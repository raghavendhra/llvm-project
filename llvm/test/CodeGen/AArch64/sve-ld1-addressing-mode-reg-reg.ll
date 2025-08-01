; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=aarch64-linux-gnu -mattr=+sve,+bf16 < %s | FileCheck %s --check-prefixes=CHECK,CHECK-LE
; RUN: llc -mtriple=aarch64_be-linux-gnu -mattr=+sve,+bf16 < %s | FileCheck %s --check-prefixes=CHECK,CHECK-BE

; LD1B

define <vscale x 16 x i8> @ld1_nxv16i8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv16i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b
; CHECK-NEXT:    ld1b { z0.b }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 16 x i8>, ptr %ptr
  ret <vscale x 16 x i8> %val
}

define <vscale x 8 x i16> @ld1_nxv16i8_bitcast_to_i16(ptr %addr, i64 %off) {
; CHECK-LE-LABEL: ld1_nxv16i8_bitcast_to_i16:
; CHECK-LE:       // %bb.0:
; CHECK-LE-NEXT:    ptrue p0.b
; CHECK-LE-NEXT:    ld1b { z0.b }, p0/z, [x0, x1]
; CHECK-LE-NEXT:    ret
;
; CHECK-BE-LABEL: ld1_nxv16i8_bitcast_to_i16:
; CHECK-BE:       // %bb.0:
; CHECK-BE-NEXT:    ptrue p0.h
; CHECK-BE-NEXT:    add x8, x0, x1
; CHECK-BE-NEXT:    ld1h { z0.h }, p0/z, [x8]
; CHECK-BE-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 8 x i16>, ptr %ptr
  ret <vscale x 8 x i16> %val
}

define <vscale x 4 x i32> @ld1_nxv16i8_bitcast_to_i32(ptr %addr, i64 %off) {
; CHECK-LE-LABEL: ld1_nxv16i8_bitcast_to_i32:
; CHECK-LE:       // %bb.0:
; CHECK-LE-NEXT:    ptrue p0.b
; CHECK-LE-NEXT:    ld1b { z0.b }, p0/z, [x0, x1]
; CHECK-LE-NEXT:    ret
;
; CHECK-BE-LABEL: ld1_nxv16i8_bitcast_to_i32:
; CHECK-BE:       // %bb.0:
; CHECK-BE-NEXT:    ptrue p0.s
; CHECK-BE-NEXT:    add x8, x0, x1
; CHECK-BE-NEXT:    ld1w { z0.s }, p0/z, [x8]
; CHECK-BE-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x i32>, ptr %ptr
  ret <vscale x 4 x i32> %val
}

define <vscale x 2 x i64> @ld1_nxv16i8_bitcast_to_i64(ptr %addr, i64 %off) {
; CHECK-LE-LABEL: ld1_nxv16i8_bitcast_to_i64:
; CHECK-LE:       // %bb.0:
; CHECK-LE-NEXT:    ptrue p0.b
; CHECK-LE-NEXT:    ld1b { z0.b }, p0/z, [x0, x1]
; CHECK-LE-NEXT:    ret
;
; CHECK-BE-LABEL: ld1_nxv16i8_bitcast_to_i64:
; CHECK-BE:       // %bb.0:
; CHECK-BE-NEXT:    ptrue p0.d
; CHECK-BE-NEXT:    add x8, x0, x1
; CHECK-BE-NEXT:    ld1d { z0.d }, p0/z, [x8]
; CHECK-BE-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i64>, ptr %ptr
  ret <vscale x 2 x i64> %val
}

define <vscale x 8 x i16> @ld1_nxv8i16_zext8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv8i16_zext8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h
; CHECK-NEXT:    ld1b { z0.h }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 8 x i8>, ptr %ptr
  %zext = zext <vscale x 8 x i8> %val to <vscale x 8 x i16>
  ret <vscale x 8 x i16> %zext
}

define <vscale x 4 x i32> @ld1_nxv4i32_zext8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4i32_zext8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1b { z0.s }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x i8>, ptr %ptr
  %zext = zext <vscale x 4 x i8> %val to <vscale x 4 x i32>
  ret <vscale x 4 x i32> %zext
}

define <vscale x 2 x i64> @ld1_nxv2i64_zext8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64_zext8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1b { z0.d }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i8>, ptr %ptr
  %zext = zext <vscale x 2 x i8> %val to <vscale x 2 x i64>
  ret <vscale x 2 x i64> %zext
}

define <vscale x 8 x i16> @ld1_nxv8i16_sext8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv8i16_sext8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h
; CHECK-NEXT:    ld1sb { z0.h }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 8 x i8>, ptr %ptr
  %sext = sext <vscale x 8 x i8> %val to <vscale x 8 x i16>
  ret <vscale x 8 x i16> %sext
}

define <vscale x 4 x i32> @ld1_nxv4i32_sext8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4i32_sext8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1sb { z0.s }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x i8>, ptr %ptr
  %sext = sext <vscale x 4 x i8> %val to <vscale x 4 x i32>
  ret <vscale x 4 x i32> %sext
}

define <vscale x 2 x i64> @ld1_nxv2i64_sext8(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64_sext8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1sb { z0.d }, p0/z, [x0, x1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i8, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i8>, ptr %ptr
  %sext = sext <vscale x 2 x i8> %val to <vscale x 2 x i64>
  ret <vscale x 2 x i64> %sext
}

; LD1H

define <vscale x 8 x i16> @ld1_nxv8i16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv8i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i16, ptr %addr, i64 %off
  %val = load volatile <vscale x 8 x i16>, ptr %ptr
  ret <vscale x 8 x i16> %val
}

define <vscale x 4 x i32> @ld1_nxv4i32_zext16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4i32_zext16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1h { z0.s }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i16, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x i16>, ptr %ptr
  %zext = zext <vscale x 4 x i16> %val to <vscale x 4 x i32>
  ret <vscale x 4 x i32> %zext
}

define <vscale x 2 x i64> @ld1_nxv2i64_zext16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64_zext16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1h { z0.d }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i16, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i16>, ptr %ptr
  %zext = zext <vscale x 2 x i16> %val to <vscale x 2 x i64>
  ret <vscale x 2 x i64> %zext
}

define <vscale x 4 x i32> @ld1_nxv4i32_sext16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4i32_sext16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1sh { z0.s }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i16, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x i16>, ptr %ptr
  %sext = sext <vscale x 4 x i16> %val to <vscale x 4 x i32>
  ret <vscale x 4 x i32> %sext
}

define <vscale x 2 x i64> @ld1_nxv2i64_sext16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64_sext16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1sh { z0.d }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i16, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i16>, ptr %ptr
  %sext = sext <vscale x 2 x i16> %val to <vscale x 2 x i64>
  ret <vscale x 2 x i64> %sext
}

define <vscale x 8 x half> @ld1_nxv8f16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv8f16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds half, ptr %addr, i64 %off
  %val = load volatile <vscale x 8 x half>, ptr %ptr
  ret <vscale x 8 x half> %val
}

define <vscale x 8 x bfloat> @ld1_nxv8bf16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv8bf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds bfloat, ptr %addr, i64 %off
  %val = load volatile <vscale x 8 x bfloat>, ptr %ptr
  ret <vscale x 8 x bfloat> %val
}

define <vscale x 4 x half> @ld1_nxv4f16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4f16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1h { z0.s }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds half, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x half>, ptr %ptr
  ret <vscale x 4 x half> %val
}

define <vscale x 4 x bfloat> @ld1_nxv4bf16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4bf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1h { z0.s }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds bfloat, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x bfloat>, ptr %ptr
  ret <vscale x 4 x bfloat> %val
}

define <vscale x 2 x half> @ld1_nxv2f16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2f16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1h { z0.d }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds half, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x half>, ptr %ptr
  ret <vscale x 2 x half> %val
}

define <vscale x 2 x bfloat> @ld1_nxv2bf16(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2bf16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1h { z0.d }, p0/z, [x0, x1, lsl #1]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds bfloat, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x bfloat>, ptr %ptr
  ret <vscale x 2 x bfloat> %val
}

; Ensure we don't lose the free shift when using indexed addressing.
define <vscale x 2 x bfloat> @ld1_nxv2bf16_double_shift(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2bf16_double_shift:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    lsr x8, x1, #6
; CHECK-NEXT:    ld1h { z0.d }, p0/z, [x0, x8, lsl #1]
; CHECK-NEXT:    ret
  %off2 = lshr i64 %off, 6
  %ptr = getelementptr inbounds bfloat, ptr %addr, i64 %off2
  %val = load volatile <vscale x 2 x bfloat>, ptr %ptr
  ret <vscale x 2 x bfloat> %val
}

; LD1W

define <vscale x 4 x i32> @ld1_nxv4i32(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0, x1, lsl #2]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i32, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x i32>, ptr %ptr
  ret <vscale x 4 x i32> %val
}

define <vscale x 2 x i64> @ld1_nxv2i64_zext32(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64_zext32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1w { z0.d }, p0/z, [x0, x1, lsl #2]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i32, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i32>, ptr %ptr
  %zext = zext <vscale x 2 x i32> %val to <vscale x 2 x i64>
  ret <vscale x 2 x i64> %zext
}

define <vscale x 2 x i64> @ld1_nxv2i64_sext32(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64_sext32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1sw { z0.d }, p0/z, [x0, x1, lsl #2]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i32, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i32>, ptr %ptr
  %sext = sext <vscale x 2 x i32> %val to <vscale x 2 x i64>
  ret <vscale x 2 x i64> %sext
}

define <vscale x 4 x float> @ld1_nxv4f32(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv4f32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0, x1, lsl #2]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds float, ptr %addr, i64 %off
  %val = load volatile <vscale x 4 x float>, ptr %ptr
  ret <vscale x 4 x float> %val
}

define <vscale x 2 x float> @ld1_nxv2f32(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2f32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1w { z0.d }, p0/z, [x0, x1, lsl #2]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds float, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x float>, ptr %ptr
  ret <vscale x 2 x float> %val
}

; Ensure we don't lose the free shift when using indexed addressing.
define <vscale x 2 x float> @ld1_nxv2f32_double_shift(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2f32_double_shift:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    lsr x8, x1, #6
; CHECK-NEXT:    ld1w { z0.d }, p0/z, [x0, x8, lsl #2]
; CHECK-NEXT:    ret
  %off2 = lshr i64 %off, 6
  %ptr = getelementptr inbounds float, ptr %addr, i64 %off2
  %val = load volatile <vscale x 2 x float>, ptr %ptr
  ret <vscale x 2 x float> %val
}

; LD1D

define <vscale x 2 x i64> @ld1_nxv2i64(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0, x1, lsl #3]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds i64, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x i64>, ptr %ptr
  ret <vscale x 2 x i64> %val
}

define <vscale x 2 x double> @ld1_nxv2f64(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2f64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0, x1, lsl #3]
; CHECK-NEXT:    ret
  %ptr = getelementptr inbounds double, ptr %addr, i64 %off
  %val = load volatile <vscale x 2 x double>, ptr %ptr
  ret <vscale x 2 x double> %val
}

; Ensure we don't lose the free shift when using indexed addressing.
define <vscale x 2 x double> @ld1_nxv2f64_double_shift(ptr %addr, i64 %off) {
; CHECK-LABEL: ld1_nxv2f64_double_shift:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d
; CHECK-NEXT:    lsr x8, x1, #6
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0, x8, lsl #3]
; CHECK-NEXT:    ret
  %off2 = lshr i64 %off, 6
  %ptr = getelementptr inbounds double, ptr %addr, i64 %off2
  %val = load volatile <vscale x 2 x double>, ptr %ptr
  ret <vscale x 2 x double> %val
}
