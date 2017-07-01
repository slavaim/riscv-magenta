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

static unsigned long           timebase; // ticks in one second

//
// lk_time_t is in nanoseconds
//

lk_time_t current_time(void)
{
    return (get_ticks() * LK_SEC(1)) / timebase;
}

#if PLATFORM_HAS_DYNAMIC_TIMER
status_t platform_set_oneshot_timer(lk_time_t deadline)
{
    DEBUG_ASSERT(arch_ints_disabled());
    
    sbi_set_timer((deadline * timebase) / LK_SEC(1));

    return MX_OK; // no error
}
#endif // #if PLATFORM_HAS_DYNAMIC_TIMER

enum handler_return riscv_timer_interrupt(void)
{
    DEBUG_ASSERT(arch_ints_disabled());
    return timer_tick(current_time());
}

static void platform_init_timer(uint level)
{
	timebase = sbi_timebase();

	/* Enable timer interrupts. */
	csr_set(sie, SIE_STIE);
}

void platform_stop_timer(void)
{
    //
    // TO_DO_RISCV implement it
    //
}

uint64_t ticks_per_second(void)
{
    return timebase;
}

LK_INIT_HOOK(timer, &platform_init_timer, LK_INIT_LEVEL_VM + 3);