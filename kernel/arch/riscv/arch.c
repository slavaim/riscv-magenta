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

#include <arch/current_thread.h>

#include <debug.h>
#include <trace.h>
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
}

extern void riscv64_uspace_entry(
    unsigned long arg1, unsigned long arg2, unsigned long pc,
    unsigned long sp, unsigned long kernel_stack) __NO_RETURN;

/* switch to user mode, set the user stack pointer to user_stack_top, put the svc stack pointer to the top of the kernel stack */
void arch_enter_uspace(uintptr_t pc, uintptr_t sp, uintptr_t arg1, uintptr_t arg2) {

    thread_t *ct = get_current_thread();

    /*
     * Set up a default state to get into 64bit user space:
     *  - all interrupts enabled
     *  - $sepc, $sp, $sstatus set to user mode values
     */

    arch_disable_ints();

    riscv64_uspace_entry(arg1, arg2, pc, sp, ct->stack_top);
    
    __UNREACHABLE;
}