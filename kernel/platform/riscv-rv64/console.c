// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/riscv/sbi.h>
#include <platform/riscv/console.h>

cbuf_t      console_input_buf;
static char raw_console_input_buf[128];

void sbi_console_init(void)
{
    cbuf_initialize_etc(&console_input_buf,
                        sizeof(raw_console_input_buf),
                        raw_console_input_buf);

	//
    // Poll the console once, which will trigger future interrupts
    //
	sbi_console_isr();
}

irqreturn_t sbi_console_isr(void)
{
	int ch = sbi_console_getchar();
	if (ch < 0)
		return IRQ_NONE;

	cbuf_write_char(&console_input_buf, ch, false);

	return IRQ_HANDLED;
}