// Copyright 2016 The Fuchsia Authors
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

void platform_dputs(const char* str, size_t len)
{
    while (len-- > 0) {
        char c = *str++;
        /*if (c == '\n') {
            sbi_console_putchar('\r');
        }*/
        sbi_console_putchar(c);
    }
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