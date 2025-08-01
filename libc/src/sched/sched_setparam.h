//===-- Implementation header for sched_setparam ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SCHED_SCHED_SETPARAM_H
#define LLVM_LIBC_SRC_SCHED_SCHED_SETPARAM_H

#include "src/__support/macros/config.h"

#include "hdr/types/pid_t.h"
#include "hdr/types/struct_sched_param.h"

namespace LIBC_NAMESPACE_DECL {

int sched_setparam(pid_t tid, const struct sched_param *param);

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SCHED_SCHED_SETPARAM_H
