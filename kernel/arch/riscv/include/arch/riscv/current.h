/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#include <arch/riscv/asm/csr.h>

#pragma once

__BEGIN_CDECLS

struct thread;

static inline struct thread *get_current(void)
{
	register struct thread * t;// asm("tp");

    __asm__ __volatile__ (
        "mv %0, tp"
        :"=r"(t)
        :
        :);

	return t;
}

static inline void set_current(struct thread* t)
{
    __asm__ __volatile__ (
        "mv tp, %0"
        :
        :"r"(t)
        :);
}

__END_CDECLS