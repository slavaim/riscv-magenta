/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#ifndef _ASM_OFFSETS_

#include <stdint.h>
#include <arch/riscv/asm/thread_info.h>

#endif

struct thread;

/*
 * low level task data that entry.S needs immediate access to
 * - this struct should fit entirely inside of one cache line
 * - this struct resides at the bottom of the supervisor stack
 * - if the members of this struct changes, the assembly constants
 *   in asm-offsets.c must be updated accordingly
 */
typedef struct thread_info {
    unsigned long       e_sp;       /* exception $sp */
    unsigned long       k_sp;       /* kernel mode $sp */
    struct thread* 	    thread;		/* main task structure */
    unsigned long		flags;		/* low level flags */
    unsigned int        cpu;		/* current CPU */
} thread_info_t;

#ifndef _ASM_OFFSETS_

__BEGIN_CDECLS

union thread_union {
	unsigned long stack[ARCH_DEFAULT_STACK_SIZE/sizeof(long)];
};

#define init_stack		(init_thread_union.stack)

static inline thread_info_t *current_thread_info(void)
{
    register thread_info_t* ti;
     __asm__ __volatile__ (
         "move %0, tp"
         :"=r"(ti)
         ::);
    return ti;
}

__END_CDECLS

#endif // _ASM_OFFSETS_


