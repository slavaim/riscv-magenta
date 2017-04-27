/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#ifndef __ASSEMBLY__

/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
#ifndef barrier
    #define barrier() __asm__ __volatile__("": : :"memory")
#endif /*barrier*/

static const float zero = 0.0;

static inline void cpu_relax(void)
{
	int dummy;
	/* In lieu of a halt instruction, induce a long-latency stall. */
	__asm__ __volatile__ ("div %0, %0, zero" : "=r" (dummy));
	barrier();
}

static inline void wait_for_interrupt(void)
{
	__asm__ __volatile__ ("wfi");
}

#endif /*__ASSEMBLY__*/
