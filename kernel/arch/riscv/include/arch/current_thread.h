// Copyright 2017 The Fuchsia Authors
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <stdint.h>
#include <magenta/compiler.h>
#include <arch/riscv/current.h>
#include <debug.h>

__BEGIN_CDECLS

static inline struct thread *get_current_thread(void)
{
    return get_current();
}

static inline void set_current_thread(struct thread *t)
{
    set_current(t);
}

__END_CDECLS