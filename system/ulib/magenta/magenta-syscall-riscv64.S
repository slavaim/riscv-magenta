// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Magenta expects some magic in the high bits. Encapsulate that here,
// so that callers do not have to set it manually.
.macro magenta_syscall num, name, caller
    addi sp, sp, -8
    sd   t0, 0(sp)
    li   t0, (0x00ff00ff00000000 | \num)
    scall
    // This symbol at the return address identifies this as an approved call site.
    .hidden CODE_SYSRET_\name\()_VIA_\caller
    CODE_SYSRET_\name\()_VIA_\caller\():
    ld   t0, 0(sp)
    addi sp, sp, 8
.endm