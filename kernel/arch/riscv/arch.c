// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2009 Corey Tabaka
// Copyright (c) 2015 Intel Corporation
// Copyright (c) 2016 Travis Geiselbrecht
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <assert.h>
#include <magenta/compiler.h>
#include <arch.h>
#include <arch/riscv/page.h>
#include <debug.h>
#include <err.h>

/* early stack, put in the data as bss is being zeroed by memset that requires the stack */
__SECTION(".data") uint8_t _kstack[PAGE_SIZE] __ALIGNED(16);

void arch_early_init(void)
{
    PANIC_UNIMPLEMENTED;
}

void arch_init(void)
{
    PANIC_UNIMPLEMENTED;
}
