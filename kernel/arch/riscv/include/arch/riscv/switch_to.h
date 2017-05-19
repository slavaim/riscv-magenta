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
    //
    // TO DO save fp registers if a thread uses fp operations
    //
}

//
// __switch_to return the "last" ( i.e. the old "prev" )
// in $a0 register as a side effect of registers restoring
// 
extern
struct riscv_thread_state *
 __switch_to(struct riscv_thread_state *,
             struct riscv_thread_state *);

static inline void riscv_switch_to(struct riscv_thread_state * prev,
                                   struct riscv_thread_state * next)
{
    __switch_to_aux(prev, next);
    __switch_to(prev, next); 
}

__END_CDECLS