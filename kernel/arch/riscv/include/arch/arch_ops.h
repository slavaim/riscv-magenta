// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#ifndef ASSEMBLY

#include <stdbool.h>

__BEGIN_CDECLS

/* fast routines that most arches will implement inline */
static inline void arch_enable_ints(void)
{
    return;
}

static inline void arch_disable_ints(void)
{
    return;
}

static inline bool arch_ints_disabled(void)
{
    return false;
}

static inline bool arch_in_int_handler(void)
{
    return false;
}

static inline uint32_t arch_cycle_count(void)
{
    return 0;
}

static inline uint arch_curr_cpu_num(void)
{
    return 0;
}

static inline uint arch_max_num_cpus(void)
{
    return 0;
}

/* function to call when an event happens that may trigger the exit from
 * a spinloop */
static inline void arch_spinloop_signal(void)
{
    return;
}

__END_CDECLS

#endif // ASSEMBLY