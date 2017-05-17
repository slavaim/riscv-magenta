/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <arch/riscv/thread_state.h>

__BEGIN_CDECLS

static inline void __switch_to_aux(struct riscv_thread_state *prev,
                                   struct riscv_thread_state *next)
{
}

extern void __switch_to(struct riscv_thread_state *,
                        struct riscv_thread_state *);

#define riscv_switch_to(prev, next)   \
do { \
    struct riscv_thread_state *__prev = (prev);     \
    struct riscv_thread_state *__next = (next);     \
    __switch_to_aux(__prev, __next);    \
    __switch_to(__prev, __next); \
} while (0)

__END_CDECLS