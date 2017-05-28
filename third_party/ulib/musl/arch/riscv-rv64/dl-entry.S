// Copyright 2017 Slava Imameev
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "asm.h"

// Call the C _dl_start, which returns a dl_start_return_t containing the
// user entry point and its argument.  Then jump to that entry point with
// the argument in the first argument register, pushing a zero return
// address and clearing the frame pointer register so the user entry point
// is the base of the call stack.

.hidden _start
ENTRY(_start)
    call _dl_start
    //
    // typedef struct {
    // void* arg;   returned in a0
    // void* entry; returned in a1
    // } dl_start_return_t;
    //
    tail a1
END(_start)
