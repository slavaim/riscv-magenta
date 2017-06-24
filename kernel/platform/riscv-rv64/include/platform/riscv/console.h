// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <arch/riscv/irqreturn.h>
#include <lib/cbuf.h>

__BEGIN_CDECLS

extern cbuf_t console_input_buf;

void sbi_console_init(void);
irqreturn_t sbi_console_isr(void);

__END_CDECLS