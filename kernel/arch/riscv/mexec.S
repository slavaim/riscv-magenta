// Copyright 2017 The Fuchsia Authors
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <asm.h>

.section .text
FUNCTION(mexec_asm)
    // Unimplemented for now, emit a loop
    1: nop
        j 1b
DATA(mexec_asm_end)