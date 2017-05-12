// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/thread.h>
#include <kernel/thread.h>
#include <arch/riscv/current.h>
#include <arch/riscv/thread_info.h>
#include <debug.h>

void arch_thread_initialize(struct thread * t, vaddr_t entry_point)
{
    //
    // init a thread_info on the stack
    //
    thread_info_t*  ti = (thread_info_t*)t->stack;

    ti->thread = t;
    ti->cpu = 0;
}

void arch_context_switch(struct thread *oldthread, struct thread *newthread)
{
    PANIC_UNIMPLEMENTED;
}

void arch_thread_construct_first(thread_t *t)
{
    //
    // initialize the boot thread's thread_info structure
    //
    thread_info_t* ti = current_thread_info();

    ti->thread = t;
    ti->cpu = arch_curr_cpu_num();

    //
    // the boot hart must have ID equal to 0
    //
    assert( 0 == ti->cpu );
}

