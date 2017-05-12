// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <sys/types.h>

#include <magenta/errors.h>
#include <lk/init.h>
#include <platform.h>
#include <sys/types.h>
#include <platform/timer.h>
#include <arch/arch_ops.h>
#include <arch/riscv/timex.h>
#include <arch/riscv/sbi.h>
#include <platform/riscv/timer.h>
#include <debug.h>
#include <assert.h>

static platform_timer_callback t_callback[SMP_MAX_CPUS] = {NULL};
static void*                   callback_arg[SMP_MAX_CPUS] = {NULL};
static unsigned long           timebase; // ticks in one seconds

//
// lk_time_t is in nanoseconds
//

lk_time_t current_time(void)
{
    return (get_ticks() * LK_SEC(1)) / timebase;
}

status_t platform_set_periodic_timer(platform_timer_callback callback, void *arg, lk_time_t interval)
{
    PANIC_UNIMPLEMENTED;
}

#if PLATFORM_HAS_DYNAMIC_TIMER
status_t platform_set_oneshot_timer(platform_timer_callback callback,
                                    void *arg, lk_time_t deadline)
{
    uint cpu = arch_curr_cpu_num();

    DEBUG_ASSERT(arch_ints_disabled());

    t_callback[cpu] = callback;
    callback_arg[cpu] = arg;
    
    sbi_set_timer((deadline * timebase) / LK_SEC(1));

    return NO_ERROR;
}
#endif // #if PLATFORM_HAS_DYNAMIC_TIMER

enum handler_return riscv_timer_interrupt(void)
{
    enum handler_return  ret = INT_NO_RESCHEDULE;
    uint                 cpu = arch_curr_cpu_num();
    lk_time_t            time = current_time();

    DEBUG_ASSERT(arch_ints_disabled());

    if (t_callback[cpu])
        ret = t_callback[cpu](callback_arg[cpu], time);

    return ret;
}

static void platform_init_timer(uint level)
{
	timebase = sbi_timebase();

	/* Enable timer interrupts. */
	csr_set(sie, SIE_STIE);
}

// LK_INIT_HOOK(timer, &platform_init_timer, LK_INIT_LEVEL_VM + 3);