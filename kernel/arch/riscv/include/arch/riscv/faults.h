//
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/riscv/asm/linkage.h>
#include <arch/riscv/asm/pt_regs.h>

__BEGIN_CDECLS

asmlinkage void do_page_fault(struct pt_regs *regs);

__END_CDECLS
