// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2013 Travis Geiselbrecht
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <debug.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <platform/debug.h>

int putchar(int c) {
    platform_dputc(c);
    return c;
}

int puts(const char *str) {
    int len = strlen(str);
    platform_dputs(str, len);
    return len;
}

int getchar(void)
{
    char c;
    int err = platform_dgetc(&c, true);
    if (err < 0) {
        return err;
    } else {
        return c;
    }
}

extern int __printf_output_func(const char *str, size_t len, void *state);

int _printf(const char *fmt, ...)
{
    va_list ap;
    int err;

#if ARCH_RISCV
    // TO_DO_RISCV , not implemented
    /*
#3  0xffffffff8002e460 in __kernel_serial_write (str=0xffffffff800301d0 "Available physical memory: %ludMB\n", len=27) at kernel/lib/io/console.c:45
#4  0xffffffff8002e594 in __kernel_stdout_write (str=0xffffffff800301d0 "Available physical memory: %ludMB\n", len=27) at kernel/lib/io/console.c:82
#5  0xffffffff8002e5d0 in __printf_output_func (s=0xffffffff800301d0 "Available physical memory: %ludMB\n", len=27, state=0x0) at kernel/lib/io/console.c:150
#6  0xffffffff8002c048 in _printf_engine (out=0xffffffff8002e5a8 <__printf_output_func>, state=0x0, fmt=0xffffffff800301ec "ludMB\n", ap=0xffffffff8003bf88 <_kstack+3896>) at kernel/lib/libc/printf.c:435
#7  0xffffffff8002cb28 in _printf (fmt=0xffffffff800301d0 "Available physical memory: %ludMB\n") at kernel/lib/libc/stdio.c:45
    */
    return 0;
#endif

    va_start(ap, fmt);
    err = _printf_engine(__printf_output_func, NULL, fmt, ap);
    va_end(ap);

    return err;
}


int _vprintf(const char *fmt, va_list ap)
{
    return _printf_engine(__printf_output_func, NULL, fmt, ap);
}
