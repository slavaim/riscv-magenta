// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <magenta/errors.h>
#include <arch/debugger.h>
#include <debug.h>

struct thread;

uint arch_num_regsets(void)
{
    PANIC_UNIMPLEMENTED;
    return 0;
}

status_t arch_get_regset(struct thread *thread, uint regset, void* regs, uint* buf_size)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}

// |priv| = true -> allow setting privileged values, otherwise leave them unchanged
// This is useful for, for example, flags registers that consist of both
// privileged and unprivileged fields.
status_t arch_set_regset(struct thread *thread, uint regset, const void* regs, uint buf_size, bool priv)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}
