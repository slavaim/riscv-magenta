// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <arch/riscv/asm/linkage.h>
#include <arch/riscv/asm/pt_regs.h>
#include <arch/riscv/asm/irq.h>

__BEGIN_CDECLS

bool arch_cpu_in_int_handler(uint cpu);
asmlinkage void do_IRQ(unsigned int cause, struct pt_regs *regs);

__END_CDECLS
