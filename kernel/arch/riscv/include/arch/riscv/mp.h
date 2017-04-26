// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2016 Travis Geiselbrecht
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <stdint.h>
#include <magenta/compiler.h>
#include <debug.h>

__BEGIN_CDECLS

static inline struct thread *get_current_thread(void)
{
    PANIC_UNIMPLEMENTED;
    return NULL;
}

static inline void set_current_thread(struct thread *t)
{
    PANIC_UNIMPLEMENTED;
}

__END_CDECLS