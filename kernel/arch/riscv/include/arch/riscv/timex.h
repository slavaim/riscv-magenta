/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <lib/lib.h>

__BEGIN_CDECLS

typedef unsigned long cycles_t;

static inline cycles_t get_ticks(void)
{
#if __riscv_xlen >= 64
	cycles_t n;
	__asm__ __volatile__ (
		"rdtime %0"
		: "=r" (n));
	return n;
#else
    #error "RV32 is not supported in the current release"
	u32 lo, hi, tmp;
	__asm__ __volatile__ (
		"1:\n"
		"rdtimeh %0\n"
		"rdtime %1\n"
		"rdtimeh %2\n"
		"bne %0, %2, 1b"
		: "=&r" (hi), "=&r" (lo), "=&r" (tmp));
	return ((u64)hi << 32) | lo;
#endif
}

static inline int read_current_timer(unsigned long *timer_val)
{
	*timer_val = get_ticks();
	return 0;
}

__END_CDECLS

