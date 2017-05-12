// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <assert.h>
#include <magenta/compiler.h>
#include <arch.h>
#include <arch/riscv/page.h>
#include <arch/riscv/thread_info.h>
#include <debug.h>
#include <err.h>

/* early stack, put in the data section as the bss section 
   is being zeroed by memset that requires the stack */
__SECTION(".data") union thread_union init_thread_union __ALIGNED(16);
__SECTION(".data") char asm_panic_string[] = {'A','S','M'};

void arch_early_init(void)
{
}

void arch_init(void)
{
    PANIC_UNIMPLEMENTED;
}
