// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/ops.h>
#include <debug.h>
#include <lib/lib.h>
#include <arch/riscv/page.h>

void arch_disable_cache(uint flags)
{
    PANIC_UNIMPLEMENTED;
}

void arch_enable_cache(uint flags)
{
    PANIC_UNIMPLEMENTED;
}

void arch_clean_cache_range(addr_t start, size_t len)
{
    PANIC_UNIMPLEMENTED;
}

void arch_clean_invalidate_cache_range(addr_t start, size_t len)
{
    PANIC_UNIMPLEMENTED;
}

void arch_invalidate_cache_range(addr_t start, size_t len)
{
    PANIC_UNIMPLEMENTED;
}

void arch_sync_cache_range(addr_t start, size_t len)
{
    PANIC_UNIMPLEMENTED;
}

/* Used to suspend work on a CPU until it is further shutdown.
 * This will only be invoked with interrupts disabled.  This function
 * must not re-enter the scheduler.
 * flush_done should be signaled after state is flushed. */
void arch_flush_state_and_halt(event_t *flush_done)
{
    PANIC_UNIMPLEMENTED;
}

void arch_idle(void)
{
    __asm__ volatile("wfi");
}

/* arch optimized version of a page zero routine against a page aligned buffer */
void arch_zero_page(void* page)
{
    arch_memset(page, 0, PAGE_SIZE);
}
