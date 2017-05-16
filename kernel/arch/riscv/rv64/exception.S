/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#include <asm.h>
#include <arch/riscv/asm/asm.h>
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/asm/constant.h>
#include <arch/riscv/asm/asm-offsets.h>
#include <arch/riscv/asm/thread_info.h>

	/* Exception vector table */
.section ".rodata"
DATA(excp_vect_table)
	PTR do_trap_insn_misaligned
	PTR do_page_fault
	PTR do_trap_insn_illegal
	PTR do_trap_unknown
	PTR do_trap_unknown
	PTR do_page_fault
	PTR do_trap_amo_misaligned
	PTR do_page_fault
	PTR 0 /* handle_syscall */
	PTR do_trap_break
excp_vect_table_end:
END(excp_vect_table)

    /*a call frame to facilitate with debugging*/
    .macro SET_GDB_FRAME
    addi	sp, sp, -2*SZREG /* allocate the frame */
    REG_S	s0, 0(sp)        /* get the frame pointer at the exception moment */
    csrr    s0, sepc         /* get the exception PC */
    REG_S	s0, SZREG(sp)    /* set the exception PC as $ra for the frame */
    addi	s0, sp, 2*SZREG  /* set s0 to the current frame pointer */
    .endm

    .macro DEL_GDB_FRAME
    REG_L	s0, 0(sp)        /* restore the caller frame pointer */
    addi	sp, sp, 2*SZREG  /* restore the stack pointer */
    .endm

	.text
	.altmacro
	.macro SAVE_ALL
	LOCAL _restore_kernel_sp
	LOCAL _save_context

	/* If coming from userspace, preserve the user stack pointer and load
	   the kernel stack pointer.  If we came from the kernel, sscratch
	   will contain 0, and we should continue on the current stack. */
	csrrw sp, sscratch, sp
	bnez sp, _save_context

_restore_kernel_sp:
	csrr sp, sscratch
_save_context:
    SET_GDB_FRAME
	addi  sp,  sp, -(PT_SIZE)
	REG_S x1,  PT_RA(sp)
	REG_S x3,  PT_GP(sp)
	REG_S x4,  PT_TP(sp)
	REG_S x5,  PT_T0(sp)
	REG_S x6,  PT_T1(sp)
	REG_S x7,  PT_T2(sp)
	REG_S x8,  PT_S0(sp)
	REG_S x9,  PT_S1(sp)
	REG_S x10, PT_A0(sp)
	REG_S x11, PT_A1(sp)
	REG_S x12, PT_A2(sp)
	REG_S x13, PT_A3(sp)
	REG_S x14, PT_A4(sp)
	REG_S x15, PT_A5(sp)
	REG_S x16, PT_A6(sp)
	REG_S x17, PT_A7(sp)
	REG_S x18, PT_S2(sp)
	REG_S x19, PT_S3(sp)
	REG_S x20, PT_S4(sp)
	REG_S x21, PT_S5(sp)
	REG_S x22, PT_S6(sp)
	REG_S x23, PT_S7(sp)
	REG_S x24, PT_S8(sp)
	REG_S x25, PT_S9(sp)
	REG_S x26, PT_S10(sp)
	REG_S x27, PT_S11(sp)
	REG_S x28, PT_T3(sp)
	REG_S x29, PT_T4(sp)
	REG_S x30, PT_T5(sp)
	REG_S x31, PT_T6(sp)

	/* Disable FPU to detect illegal usage of
	   floating point in kernel space */
	li t0, SR_FS

    /* s0 is used as a frame pointer, do not 
       overwrite it */
	csrr  s1, sscratch
	csrrc s2, sstatus, t0
	csrr  s3, sepc
	csrr  s4, sbadaddr
	csrr  s5, scause
    REG_S s1, PT_SP(sp)
	REG_S s2, PT_SSTATUS(sp)
	REG_S s3, PT_SEPC(sp)
	REG_S s4, PT_SBADADDR(sp)
	REG_S s5, PT_SCAUSE(sp)
	.endm

	.macro RESTORE_ALL
	REG_L a0, PT_SSTATUS(sp)
	REG_L a2, PT_SEPC(sp)
	csrw sstatus, a0
	csrw sepc, a2

	REG_L x1,  PT_RA(sp)
	REG_L x3,  PT_GP(sp)
	REG_L x4,  PT_TP(sp)
	REG_L x5,  PT_T0(sp)
	REG_L x6,  PT_T1(sp)
	REG_L x7,  PT_T2(sp)
	REG_L x8,  PT_S0(sp)
	REG_L x9,  PT_S1(sp)
	REG_L x10, PT_A0(sp)
	REG_L x11, PT_A1(sp)
	REG_L x12, PT_A2(sp)
	REG_L x13, PT_A3(sp)
	REG_L x14, PT_A4(sp)
	REG_L x15, PT_A5(sp)
	REG_L x16, PT_A6(sp)
	REG_L x17, PT_A7(sp)
	REG_L x18, PT_S2(sp)
	REG_L x19, PT_S3(sp)
	REG_L x20, PT_S4(sp)
	REG_L x21, PT_S5(sp)
	REG_L x22, PT_S6(sp)
	REG_L x23, PT_S7(sp)
	REG_L x24, PT_S8(sp)
	REG_L x25, PT_S9(sp)
	REG_L x26, PT_S10(sp)
	REG_L x27, PT_S11(sp)
	REG_L x28, PT_T3(sp)
	REG_L x29, PT_T4(sp)
	REG_L x30, PT_T5(sp)
	REG_L x31, PT_T6(sp)

	REG_L x2,  PT_SP(sp)

    DEL_GDB_FRAME
	.endm

    .macro PANIC
    li a0, 0
    li a1, 0
    la a2, asm_panic_string
    call _panic
    .endm

.section .text
FUNCTION(handle_exception)
    /*
      registers usage
        s0 - a frame pointer set by SET_EXCPT_FRAME
        sp - pt_regs structure allocated on the stack
    */
	SAVE_ALL

	/* Set sscratch register to 0, so that if a recursive exception
	   occurs, the exception vector knows it came from the kernel */
	csrw sscratch, x0

	/* Compute the address of current thread_info */
	li tp, ~(ARCH_DEFAULT_STACK_SIZE-1)
	and tp, tp, sp

	/* Set the current thread pointer */
	REG_L tp, TI_THREAD(tp)

1:	la ra, ret_from_exception
	/* MSB of cause differentiates between
	   interrupts and exceptions */
	bge s4, zero, 1f

	/* Handle interrupts */
	slli a0, s4, 1
	srli a0, a0, 1
	move a1, sp /* pt_regs */
	tail do_IRQ
1:
	/* Handle syscalls */
	li t0, EXC_SYSCALL
	beq s4, t0, handle_syscall

	/* Handle other exceptions */
	slli t0, s4, LGPTR
	la t1, excp_vect_table
	la t2, excp_vect_table_end
	move a0, sp /* pt_regs */
	add t0, t1, t0
	/* Check if exception code lies within bounds */
	bgeu t0, t2, 1f
	REG_L t0, 0(t0)
	jr t0
1:
	tail do_trap_unknown

handle_syscall:
	/* Advance SEPC to avoid executing the original
	   scall instruction on sret */
	addi s2, s2, 0x4
	REG_S s2, PT_SEPC(sp)
	/* System calls run with interrupts enabled */
	csrs sstatus, SR_IE
check_syscall_nr:
    /* TO_DO_RISCV */
    PANIC

ret_from_syscall:
	/* Set user a0 to kernel a0 */
	REG_S a0, PT_A0(sp)
	/* TO_DO_RISCV */
    PANIC

ret_from_exception:
	REG_L s1, PT_SSTATUS(sp)
	csrc sstatus, SR_IE
	andi s1, s1, SR_PS
	bnez s1, restore_all

resume_userspace:
	/* Interrupts must be disabled here */
	/* Save unwound kernel stack pointer in sscratch */
	addi s1, sp, PT_SIZE
	csrw sscratch, s1
restore_all:
	RESTORE_ALL
	sret
END(handle_exception)

#if 0
.section .text
FUNCTION(ret_from_fork)
	la ra, ret_from_exception
	tail schedule_tail
END(ret_from_fork)

.section .text
FUNCTION(ret_from_kernel_thread)
	call schedule_tail
	/* Call fn(arg) */
	la ra, ret_from_exception
	move a0, s1
	jr s0
END(ret_from_kernel_thread)


/*
 * Integer register context switch
 * The callee-saved registers must be saved and restored.
 * 
 *   a0: previous task_struct (must be preserved across the switch)
 *   a1: next task_struct
 */
.section .text
FUNCTION(__switch_to)
	/* Save context into prev->thread */
	REG_S ra,  THREAD_RA(a0)
	REG_S sp,  THREAD_SP(a0)
	REG_S s0,  THREAD_S0(a0)
	REG_S s1,  THREAD_S1(a0)
	REG_S s2,  THREAD_S2(a0)
	REG_S s3,  THREAD_S3(a0)
	REG_S s4,  THREAD_S4(a0)
	REG_S s5,  THREAD_S5(a0)
	REG_S s6,  THREAD_S6(a0)
	REG_S s7,  THREAD_S7(a0)
	REG_S s8,  THREAD_S8(a0)
	REG_S s9,  THREAD_S9(a0)
	REG_S s10, THREAD_S10(a0)
	REG_S s11, THREAD_S11(a0)
	/* Restore context from next->thread */
	REG_L ra,  THREAD_RA(a1)
	REG_L sp,  THREAD_SP(a1)
	REG_L s0,  THREAD_S0(a1)
	REG_L s1,  THREAD_S1(a1)
	REG_L s2,  THREAD_S2(a1)
	REG_L s3,  THREAD_S3(a1)
	REG_L s4,  THREAD_S4(a1)
	REG_L s5,  THREAD_S5(a1)
	REG_L s6,  THREAD_S6(a1)
	REG_L s7,  THREAD_S7(a1)
	REG_L s8,  THREAD_S8(a1)
	REG_L s9,  THREAD_S9(a1)
	REG_L s10, THREAD_S10(a1)
	REG_L s11, THREAD_S11(a1)
	mv tp, a1 /* Next current pointer */
	ret
END(__switch_to)

.section .text
FUNCTION(__fstate_save)
	li t1, SR_FS
	csrs sstatus, t1
	frcsr t0
	fsd f0,  THREAD_F0(a0)
	fsd f1,  THREAD_F1(a0)
	fsd f2,  THREAD_F2(a0)
	fsd f3,  THREAD_F3(a0)
	fsd f4,  THREAD_F4(a0)
	fsd f5,  THREAD_F5(a0)
	fsd f6,  THREAD_F6(a0)
	fsd f7,  THREAD_F7(a0)
	fsd f8,  THREAD_F8(a0)
	fsd f9,  THREAD_F9(a0)
	fsd f10, THREAD_F10(a0)
	fsd f11, THREAD_F11(a0)
	fsd f12, THREAD_F12(a0)
	fsd f13, THREAD_F13(a0)
	fsd f14, THREAD_F14(a0)
	fsd f15, THREAD_F15(a0)
	fsd f16, THREAD_F16(a0)
	fsd f17, THREAD_F17(a0)
	fsd f18, THREAD_F18(a0)
	fsd f19, THREAD_F19(a0)
	fsd f20, THREAD_F20(a0)
	fsd f21, THREAD_F21(a0)
	fsd f22, THREAD_F22(a0)
	fsd f23, THREAD_F23(a0)
	fsd f24, THREAD_F24(a0)
	fsd f25, THREAD_F25(a0)
	fsd f26, THREAD_F26(a0)
	fsd f27, THREAD_F27(a0)
	fsd f28, THREAD_F28(a0)
	fsd f29, THREAD_F29(a0)
	fsd f30, THREAD_F30(a0)
	fsd f31, THREAD_F31(a0)
	sw t0, THREAD_FCSR(a0)
	csrc sstatus, t1
	ret
END(__fstate_save)

.section .text
FUNCTION(__fstate_restore)
	li t1, SR_FS
	lw t0, THREAD_FCSR(a0)
	csrs sstatus, t1
	fld f0,  THREAD_F0(a0)
	fld f1,  THREAD_F1(a0)
	fld f2,  THREAD_F2(a0)
	fld f3,  THREAD_F3(a0)
	fld f4,  THREAD_F4(a0)
	fld f5,  THREAD_F5(a0)
	fld f6,  THREAD_F6(a0)
	fld f7,  THREAD_F7(a0)
	fld f8,  THREAD_F8(a0)
	fld f9,  THREAD_F9(a0)
	fld f10, THREAD_F10(a0)
	fld f11, THREAD_F11(a0)
	fld f12, THREAD_F12(a0)
	fld f13, THREAD_F13(a0)
	fld f14, THREAD_F14(a0)
	fld f15, THREAD_F15(a0)
	fld f16, THREAD_F16(a0)
	fld f17, THREAD_F17(a0)
	fld f18, THREAD_F18(a0)
	fld f19, THREAD_F19(a0)
	fld f20, THREAD_F20(a0)
	fld f21, THREAD_F21(a0)
	fld f22, THREAD_F22(a0)
	fld f23, THREAD_F23(a0)
	fld f24, THREAD_F24(a0)
	fld f25, THREAD_F25(a0)
	fld f26, THREAD_F26(a0)
	fld f27, THREAD_F27(a0)
	fld f28, THREAD_F28(a0)
	fld f29, THREAD_F29(a0)
	fld f30, THREAD_F30(a0)
	fld f31, THREAD_F31(a0)
	fscsr t0
	csrc sstatus, t1
	ret
END(__fstate_restore)

#endif // 0
