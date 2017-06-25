// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <stddef.h>
#include <debug.h>
#include <magenta/errors.h>
#include <arch/riscv/sbi.h>
#include <platform/riscv/console.h>

cbuf_t      console_input_buf;
static char raw_console_input_buf[128];

static char         dputs_buffer[256];
static size_t       dputs_buffer_len = 0;
static spin_lock_t  dputs_buffer_lock;

void sbi_console_init(void)
{
    spin_lock_init(&dputs_buffer_lock);

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

static void platform_flush_dputs_buffer(void)
{
    //
    // a caller should hold dputs_buffer_lock
    //
    char* str = dputs_buffer;

    while (dputs_buffer_len > 0) {
        char c = *str++;
        dputs_buffer_len--;
        
        if (c == '\n') {
            sbi_console_putchar('\r');
        }
        sbi_console_putchar(c);
    }
}

void platform_dputs(const char* str, size_t len)
{
    spin_lock(&dputs_buffer_lock);
    { // start of the lock
        while (len-- > 0) {
            char c = *str++;
            dputs_buffer[dputs_buffer_len++] = c;

            assert(dputs_buffer_len <= sizeof(dputs_buffer));

            if ('\0' == c || '\n' == c || dputs_buffer_len == sizeof(dputs_buffer)){
                platform_flush_dputs_buffer();
            }
        }
    } // end of the lock
    spin_unlock(&dputs_buffer_lock);
}

int platform_dgetc(char *c, bool wait)
{
    return (int)cbuf_read_char(&console_input_buf, c, wait);
}

void platform_pputc(char c)
{
    sbi_console_putchar(c);
}

int platform_pgetc(char *c, bool wait)
{
     *c = sbi_console_getchar();
     return MX_OK;
}