// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "syscall-entry.h"
#include "magenta-syscall-riscv64.S"
#include <magenta/mx-syscall-numbers.h>

.text

.cfi_sections .eh_frame, .debug_frame

// (vmar_handle: a0, addr: a1, len: a2, handle: a3)
syscall_entry_begin mx_vmar_unmap_handle_close_thread_exit

    // Save the handle argument
    addi sp, sp, -8
    sd   a3, 0(sp)

    magenta_syscall MX_SYS_vmar_unmap
    bnez a0, .Lvmar_unmap_fail

    // Now the stack is gone and we can never return!

    ld a0, 0(sp)
    magenta_syscall MX_SYS_handle_close
    bnez a0, .Lhandle_close_fail

    magenta_syscall MX_SYS_thread_exit

    // It should be impossible to get here.
.Lthread_exit_returned:
    j .Lthread_exit_returned

.Lvmar_unmap_fail:
    addi sp, sp, 8
    ret

.Lhandle_close_fail:
    j .Lhandle_close_fail

syscall_entry_end mx_vmar_unmap_handle_close_thread_exit