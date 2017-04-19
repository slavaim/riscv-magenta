// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2015 Travis Geiselbrecht
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <magenta/compiler.h>
#include <stdbool.h>
#include <debug.h>

__BEGIN_CDECLS
#define SPIN_LOCK_INITIAL_VALUE (0)

typedef unsigned long spin_lock_t;

typedef uint spin_lock_saved_state_t;
typedef uint spin_lock_save_flags_t;

#if WITH_SMP
static inline void arch_spin_lock_init(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;
    *lock = SPIN_LOCK_INITIAL_VALUE;
}

static inline bool arch_spin_lock_held(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;
    return *lock != 0;
}

void arch_spin_lock(spin_lock_t *lock);
int arch_spin_trylock(spin_lock_t *lock);
void arch_spin_unlock(spin_lock_t *lock);
#else
/* simple implementation of spinlocks for no smp support */
static inline void arch_spin_lock_init(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;
    *lock = SPIN_LOCK_INITIAL_VALUE;
}

static inline bool arch_spin_lock_held(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;
    return *lock != 0;
}

static inline void arch_spin_lock(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;
    *lock = 1;
}

static inline int arch_spin_trylock(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;

    if (*lock)
        return 1;

    *lock = 1;
    return 0;
}

static inline void arch_spin_unlock(spin_lock_t *lock)
{
    PANIC_UNIMPLEMENTED;

    *lock = 0;
}
#endif // WITH_SMP

/* flags are unused on x86 */
#define ARCH_DEFAULT_SPIN_LOCK_FLAG_INTERRUPTS  0

static inline void
arch_interrupt_save(spin_lock_saved_state_t *statep, spin_lock_save_flags_t flags)
{
    PANIC_UNIMPLEMENTED;
}

static inline void
arch_interrupt_restore(spin_lock_saved_state_t old_state, spin_lock_save_flags_t flags)
{
    PANIC_UNIMPLEMENTED;
}

__END_CDECLS