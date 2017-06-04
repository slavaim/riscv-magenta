/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/
#pragma once

#include <magenta/compiler.h>
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/thread_info.h>

__BEGIN_CDECLS

struct thread;

static inline struct thread *get_current(void)
{
	register struct thread_info* ti;// asm("tp");

    __asm__ __volatile__ (
        "mv %0, tp"
        :"=r"(ti)
        :
        :);

    assert(ti);
	return ti->thread;
}

static inline void set_current(struct thread* t)
{
    register struct thread_info* ti = &t->arch.ti;
    assert(t);
    
    __asm__ __volatile__ (
        "mv tp, %0"
        :
        :"r"(ti)
        :);
}

__END_CDECLS