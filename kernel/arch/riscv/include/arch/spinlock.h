// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2015 Travis Geiselbrecht
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <magenta/compiler.h>
#include <arch/riscv/irqflags.h>
#include <arch/riscv/spinlock_raw.h>
#include <stdbool.h>
#include <debug.h>

__BEGIN_CDECLS

#define SPIN_LOCK_INITIAL_VALUE __ARCH_SPIN_LOCK_UNLOCKED

typedef unsigned long spin_lock_saved_state_t;
typedef unsigned long spin_lock_save_flags_t;

typedef arch_spinlock_t spin_lock_t;

static inline void arch_spin_lock_init(spin_lock_t *lock)
{
    *(arch_spinlock_t*)lock = __ARCH_SPIN_LOCK_UNLOCKED;
}

static inline bool arch_spin_lock_held(spin_lock_t *lock)
{
    return *(arch_spinlock_t*)lock != 0;
}

static inline void arch_spin_lock(spin_lock_t *lock)
{
    arch_spin_lock_raw((arch_spinlock_t*)lock);
}

static inline int arch_spin_trylock(spin_lock_t *lock)
{
    return arch_spin_trylock_raw((arch_spinlock_t*)lock);
}

static inline void arch_spin_unlock(spin_lock_t *lock)
{
    arch_spin_unlock((arch_spinlock_t*)lock);
}

/* flags are unused on risc-v */
#define ARCH_DEFAULT_SPIN_LOCK_FLAG_INTERRUPTS  0

static inline void
arch_interrupt_save(spin_lock_saved_state_t *statep, spin_lock_save_flags_t flags)
{
    *statep = arch_local_irq_save();
}

static inline void
arch_interrupt_restore(spin_lock_saved_state_t old_state, spin_lock_save_flags_t flags)
{
    arch_local_irq_restore(old_state);
}

__END_CDECLS