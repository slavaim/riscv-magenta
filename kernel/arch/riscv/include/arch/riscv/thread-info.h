/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <stdint.h>
#include <kernel/thread.h>
#include <arch/riscv/asm/thread-info.h>

/*
 * low level task data that entry.S needs immediate access to
 * - this struct should fit entirely inside of one cache line
 * - this struct resides at the bottom of the supervisor stack
 * - if the members of this struct changes, the assembly constants
 *   in asm-offsets.c must be updated accordingly
 */
typedef struct thread_info {
    thread_t* 	        thread;		/* main task structure */
    unsigned long		flags;		/* low level flags */
    uint                cpu;		/* current CPU */
} thread_info_t;

/*
 * macros/functions for gaining access to the thread information structure
 *
 * preempt_count needs to be 1 initially, until the scheduler is functional.
 */
#define INIT_THREAD_INFO(thread)			\
{						\
    .task		= &thread,			\
    .flags		= 0,			\
    .cpu		= 0,			\
}

union thread_union {
	thread_info_t thread_info;
	unsigned long stack[THREAD_STACK_SIZE/sizeof(long)];
};

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

/*
 * Pointer to the thread_info struct of the current process
 * Assumes that the kernel mode stack (thread_union) is THREAD_SIZE-aligned
 */
static inline thread_info_t *current_thread_info(void)
{
    register unsigned long sp;
     __asm__ __volatile__ (
         "move %0, sp"
         :"=r"(sp)
         ::);
    return (thread_info_t *)(sp & ~(THREAD_STACK_SIZE - 1));
}



