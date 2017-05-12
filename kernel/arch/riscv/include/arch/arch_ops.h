// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#ifndef ASSEMBLY

#include <stdbool.h>
#include <debug.h>
#include <arch/riscv/mp.h>
#include <arch/riscv/irqflags.h>
#include <arch/riscv/sbi.h>
#include <arch/riscv/irq.h>

__BEGIN_CDECLS

//
// some forward declarations
//
static uint arch_curr_cpu_num(void);
extern bool arch_cpu_in_int_handler(uint cpu);

/* fast routines that most arches will implement inline */
static inline void arch_enable_ints(void)
{
    arch_local_irq_enable();
}

static inline void arch_disable_ints(void)
{
    arch_local_irq_disable();
}

static inline bool arch_ints_disabled(void)
{
    return arch_irqs_disabled();
}

static inline bool arch_in_int_handler(void)
{
    return arch_cpu_in_int_handler(arch_curr_cpu_num());
}

static inline uint64_t arch_cycle_count(void)
{
    PANIC_UNIMPLEMENTED;
    return 0;
}

static inline uint arch_curr_cpu_num(void)
{
    //
    // TO_DO_RISCV return the actual index for SMP
    // instead of a hart ID which might not be consecuitive
    //
    return (uint)sbi_hart_id();
}

static inline uint arch_max_num_cpus(void)
{
    // TO_DO_RISCV , return the actual number when mp is implemented
    return 1;
}

static inline void arch_spinloop_pause(void)
{
    PANIC_UNIMPLEMENTED;
}

/* function to call when an event happens that may trigger the exit from
 * a spinloop */
static inline void arch_spinloop_signal(void)
{
    PANIC_UNIMPLEMENTED;
}

#define mb()        do{ PANIC_UNIMPLEMENTED; }while(0);
#define wmb()       do{ PANIC_UNIMPLEMENTED; }while(0);
#define rmb()       do{ PANIC_UNIMPLEMENTED; }while(0);

#define smp_mb()    mb()
#define smp_wmb()   wmb()
#define smp_rmb()   rmb()

__END_CDECLS

#endif // ASSEMBLY