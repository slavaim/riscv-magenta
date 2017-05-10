//
// Copyright 2017 Slava Imameev.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT


#include <asm.h>
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/asm/thread-info.h>

.section .text.boot
FUNCTION(_riscv_start)

	/* Disable FPU to detect illegal usage of
	   floating point in kernel space */
	li t0, SR_FS
	csrc sstatus, t0

#ifdef CONFIG_RV_PUM
	/* Prevent inadvertent access to user memory */
	li t0, SR_PUM
	csrs sstatus, t0
#endif

	/* See if we're the main hart */
	call sbi_hart_id
	bnez a0, .Lsecondary_start

	/* we are in the main core/hart */

	/* Initialize the stack pointer,
	   a stack is required for function calls */
	la sp, init_thread_union + THREAD_STACK_SIZE

	/* Clear the .bss segment */
	la a0, __bss_start
	li a1, 0
	la a2, __bss_end
	sub a2, a2, a0
	call memset

	/* Setup supervisor trap vector */
	call trap_init

	tail lk_main

.Lsecondary_start:
/*TO_DO_RISCV secondary CPU start code*/
.Lsecondary_park:
	/* We lack SMP support or have too many harts, so park this hart */
	wfi
	j .Lsecondary_park
END(_riscv_start)
