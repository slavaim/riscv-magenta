/*
Copyright 2017 Slava Imameev.
*/

#pragma once

#ifndef _ASM_OFFSETS_

#include <stdint.h>
#include <arch/riscv/asm/thread_info.h>

#endif

struct thread;

/*
 * low level task data that exception.S needs immediate access to
 * - it is advisably that this struct fits entirely inside of
 *   one cache line
 * - this struct pointer is saved in the supervisor/kernel $tp
 *   as we can't resides it at the bottom of the supervisor stack
 *   as stacks in Magenta are not page aligned so the Linux
 *   trick with zeroing $sp low order bits to access thread_info
 *   doesn't work here
 * - if the members of this struct changes, the assembly constants
 *   in asm-offsets.c must be updated accordingly
 */
typedef struct thread_info {
    unsigned long       e_sp;       /* exception $sp, a temporary
                                       storage for $sp of the
                                       interrupted code, valid in
                                       the region with disabled
                                       interrupts */
    unsigned long       k_sp;       /* kernel mode $sp, saves the
                                       kernel stack when returning
                                       into user mode, valid only
                                       when entering into the exception
                                       handler from the user mode */
    struct thread* 	    thread;		/* main thread structure */
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


