/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <arch/riscv/uapi/ptrace.h>

struct thread;

/* CPU-specific state of a thread */
typedef struct riscv_thread_state {
	/* Callee-saved registers */
	unsigned long ra;
	unsigned long sp;	/* Kernel mode stack */
	unsigned long s[12];	/* s[0]: frame pointer */
	struct user_fpregs_struct fstate;
	struct thread* thread; /* a back pointer to a thread */
} riscv_thread_state_t;