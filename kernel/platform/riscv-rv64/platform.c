// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <kernel/vm.h>
#include <arch/riscv/mmu.h>
#include <platform.h>
#include <platform/riscv/memory.h>

#include <dev/display.h>
#include <dev/hw_rng.h>

#include <debug.h>

void platform_early_init(void)
{
    /* initialize physical memory arenas */
    platform_mem_init();
}

int platform_dgetc(char *c, bool wait)
{
    PANIC_UNIMPLEMENTED;
}

/* currently doesn't exist on test platform */
size_t hw_rng_get_entropy(void* buf, size_t len, bool block) {
    // TO_DO_RISCV
    return 0;
}

/* no built in framebuffer */
status_t display_get_info(struct display_info *info) {
    return ERR_NOT_FOUND;
}
