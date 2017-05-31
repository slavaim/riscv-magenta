// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "syscall-entry.h"
#include "magenta-syscall-riscv64.S"
#include <magenta/mx-syscall-numbers.h>

.text

.cfi_sections .eh_frame, .debug_frame

// (value_ptr: a0, count: a1, handle: a2)
syscall_entry_begin mx_futex_wake_handle_close_thread_exit

    // Save the handle argument
    addi sp, sp, -8
    sd   a2, 0(sp)

    magenta_syscall MX_SYS_futex_wake, mx_futex_wake, mx_futex_wake_handle_close_thread_exit
    bnez a0, .Lfutex_wake_fail

    // Now the stack might be gone and we can never return!

    ld a0, 0(sp)
    magenta_syscall MX_SYS_handle_close, mx_thread_exit, mx_futex_wake_handle_close_thread_exit
    bnez a0, .Lhandle_close_fail

    magenta_syscall MX_SYS_thread_exit

    // It should be impossible to get here.
.Lthread_exit_returned:
    j .Lthread_exit_returned

.Lfutex_wake_fail:
    addi sp, sp, 8
    ret

.Lhandle_close_fail:
    j .Lhandle_close_fail


syscall_entry_end mx_futex_wake_handle_close_thread_exit