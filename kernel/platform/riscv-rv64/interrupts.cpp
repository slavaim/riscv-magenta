// Copyright 2016 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <sys/types.h>
#include <dev/interrupt.h>
#include <debug.h>

void shutdown_interrupts(void) {
    PANIC_UNIMPLEMENTED;
}

status_t mask_interrupt(unsigned int vector) {
    PANIC_UNIMPLEMENTED;
}

status_t unmask_interrupt(unsigned int vector) {
    PANIC_UNIMPLEMENTED;
}

unsigned int remap_interrupt(unsigned int vector) {
    PANIC_UNIMPLEMENTED;
}

bool is_valid_interrupt(unsigned int vector, uint32_t flags) {
    PANIC_UNIMPLEMENTED;
}

void register_int_handler(unsigned int vector, int_handler handler, void *arg) {
    PANIC_UNIMPLEMENTED;
}
