// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <arch/thread.h>
#include <kernel/thread.h>
#include <debug.h>

void arch_thread_initialize(struct thread * t, vaddr_t entry_point)
{
    PANIC_UNIMPLEMENTED;
}

void arch_context_switch(struct thread *oldthread, struct thread *newthread)
{
    PANIC_UNIMPLEMENTED;
}
