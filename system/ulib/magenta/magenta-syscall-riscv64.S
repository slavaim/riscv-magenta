// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Magenta expects some magic in the high bits. Encapsulate that here,
// so that callers do not have to set it manually.
.macro magenta_syscall num
    addi sp, sp, -8
    sd   t0, 0(sp)
    li   t0, (0x00ff00ff00000000 | \num)
    scall
    ld   t0, 0(sp)
    addi sp, sp, 8
.endm