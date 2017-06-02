// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <arch/riscv/pt_regs.h>

__BEGIN_CDECLS

void riscv_syscall(struct pt_regs*  regs);

struct arch_exception_context {
    struct pt_regs* frame;
};

__END_CDECLS