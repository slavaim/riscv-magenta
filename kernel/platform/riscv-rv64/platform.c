// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <kernel/vm.h>
#include <platform.h>
#include <magenta/atomic.h>

#include <lib/debuglog.h>

#include <platform/riscv/memory.h>

#include <arch/riscv/mmu.h>
#include <arch/arch_ops.h>

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

static void halt_other_cpus(void) {

    // TO_DO_RISCV
}

static volatile int panic_started;

void platform_panic_start(void) {

    arch_disable_ints();

    if (atomic_swap(&panic_started, 1) == 0) {
#if WITH_LIB_DEBUGLOG
        dlog_bluescreen_init();
#endif
    }

    halt_other_cpus();
}

static void reboot(void) {

    // TO_DO_RISCV

    arch_disable_ints();

    for (;;) {
        cpu_relax();
    }
}

bool halt_on_panic = false;

void platform_halt(
        platform_halt_action suggested_action,
        platform_halt_reason reason)
{
    printf("platform_halt suggested_action %ud reason %ud\n", suggested_action, reason);

    arch_disable_ints();

    switch (suggested_action) {
        case HALT_ACTION_SHUTDOWN:
            printf("Power off failed, halting\n");
            break;
        case HALT_ACTION_REBOOT:
            printf("Rebooting...\n");
            reboot();
            printf("Reboot failed, halting\n");
            break;
        case HALT_ACTION_HALT:
            printf("Halting...\n");
            halt_other_cpus();
            break;
    }

#if WITH_LIB_DEBUGLOG
#if WITH_PANIC_BACKTRACE
    thread_print_backtrace(get_current_thread(), __GET_FRAME(0));
#endif
    dlog_bluescreen_halt();
#endif

    if (!halt_on_panic) {
        printf("Rebooting...\n");
        reboot();
    }

    printf("Halted\n");

#if ENABLE_PANIC_SHELL
    panic_shell_start();
#endif

    for (;;) {
        cpu_relax();
    }
}

size_t platform_stow_crashlog(void* log, size_t len) {
    return 0;
}
