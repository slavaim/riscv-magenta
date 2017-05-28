//
// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

    .text
    .globl _start
    .type _start, %function
_start:
    // Called _start(argument) with argument in %a0.
    // Tail-call __libc_start_main(argument, &main).
    
    la a1, main // set the second argument
    tail __libc_start_main