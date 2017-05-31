// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "syscall-entry.h"
#include "magenta-syscall-riscv64.S"

.text
.cfi_sections .eh_frame, .debug_frame

.macro m_syscall name, num, with_aliases
syscall_entry_begin \name
    magenta_syscall \num, \name, \name
    ret
syscall_entry_end \name \with_aliases
.endm

// for a while we will use a file generated for arm64,
// to change this the tools/sysgen utility should be
// modified to support riscv
#include <magenta/syscalls-arm64.S>

#include "syscalls-stubs.S"