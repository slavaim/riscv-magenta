// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/thread.h>
#include <kernel/thread.h>
#include <arch/riscv/current.h>
#include <arch/riscv/thread_info.h>
#include <arch/riscv/switch_to.h>
#include <debug.h>
#include <inttypes.h>
#include <string.h> // for memset

static inline void init_thread_info(struct thread * t, thread_info_t*  ti)
{
    memset(ti, 0, sizeof(*ti));

    ti->thread = t;

    //
    // set a bogus cpu as the thread is not running, 
    // the actual cpu number is updated on each
    // context switch
    //
    ti->cpu = (-1);
}

void arch_thread_initialize(struct thread * t, vaddr_t entry_point)
{
    thread_info_t*  ti;

    //
    // make sure the top of the stack is 16 byte aligned for 
    // the stringest alignment and ABI compliance
    //
    t->stack_top = ROUNDDOWN(((vaddr_t)t->stack + t->stack_size), 16);

    //
    // init the thread registers
    //
    t->arch.state.ra = entry_point;
    t->arch.state.sp = t->stack_top;
    //t->arch.state.sstatus = SR_PIE /* User mode, irqs on */ | SR_FS_INITIAL;

    //
    // let gdb know that there is no stack frame above
    //
    t->arch.state.s[0] = 0;

    //
    // set the thread info
    //
    ti = &t->arch.ti;
    init_thread_info(t, ti);

    //
    // set the back pointer, see __switch_to for details
    //
    t->arch.state.ti = ti;
}

void arch_thread_construct_first(thread_t *t)
{
    //*(int*)0x0 = 1; // page fault test TTTTTTTTTTTTTTTTTT

    //
    // initialize the boot thread's thread_info structure
    //
    thread_info_t* ti = &t->arch.ti;
    init_thread_info(t, ti);

    //
    // we always start at 0 heart
    //
    ti->cpu = 0;
    assert( (uint)sbi_hart_id() == ti->cpu );

    //
    // set the thread pointer, see __switch_to for details
    //
    t->arch.state.ti = ti;
}

void arch_context_switch(struct thread *oldthread, struct thread *newthread)
{
    //
    // set the cpu number, look at arch_curr_cpu_num that uses it
    //
    if (newthread->arch.ti.cpu != oldthread->arch.ti.cpu)
        newthread->arch.ti.cpu = oldthread->arch.ti.cpu;

    assert( newthread->arch.ti.cpu < SMP_MAX_CPUS );
    
    //
    // swap registers
    //
    riscv_switch_to(&oldthread->arch.state, &newthread->arch.state);
}

void arch_dump_thread(thread_t *t)
{
    if (t->state != THREAD_RUNNING) {
        dprintf(INFO, "\tarch: ");
        dprintf(INFO, "sp %#" PRIxPTR "\n", t->arch.state.sp);
    }
}

