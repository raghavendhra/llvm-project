; RUN: not opt -passes='print<dxil-root-signature>' %s -S -o - 2>&1 | FileCheck %s

target triple = "dxil-unknown-shadermodel6.0-compute"

; CHECK: error: Invalid Root Signature Element
; CHECK-NOT: Root Signature Definitions


define void @main() #0 {
entry:
  ret void
}

attributes #0 = { "hlsl.numthreads"="1,1,1" "hlsl.shader"="compute" }


!dx.rootsignatures = !{!2} ; list of function/root signature pairs
!2 = !{ ptr @main, !3, i32 2 } ; function, root signature
!3 = !{ !4 } ; list of root signature elements
!4 = !{ !"NOTRootFlags", i32 1 } ; 1 = allow_input_assembler_input_layout
