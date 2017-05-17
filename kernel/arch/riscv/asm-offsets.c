/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#define _ASM_OFFSETS_ 1

//
// this file is not compiled to any object file,
// the file is used to generate asm-offsets.h
// with  offsets for assembler sources because
// gnu assembler doesn't understand C structure
// definition
//
#include <arch/riscv/asm/kbuild.h>
#include <arch/riscv/pt_regs.h>
#include <arch/riscv/thread_info.h>
#include <arch/riscv/thread_state.h>

void asm_offsets(void)
{
	OFFSET(THREAD_RA, riscv_thread_state, ra);
	OFFSET(THREAD_SP, riscv_thread_state, sp);
	OFFSET(THREAD_S0, riscv_thread_state, s[0]);
	OFFSET(THREAD_S1, riscv_thread_state, s[1]);
	OFFSET(THREAD_S2, riscv_thread_state, s[2]);
	OFFSET(THREAD_S3, riscv_thread_state, s[3]);
	OFFSET(THREAD_S4, riscv_thread_state, s[4]);
	OFFSET(THREAD_S5, riscv_thread_state, s[5]);
	OFFSET(THREAD_S6, riscv_thread_state, s[6]);
	OFFSET(THREAD_S7, riscv_thread_state, s[7]);
	OFFSET(THREAD_S8, riscv_thread_state, s[8]);
	OFFSET(THREAD_S9, riscv_thread_state, s[9]);
	OFFSET(THREAD_S10, riscv_thread_state, s[10]);
	OFFSET(THREAD_S11, riscv_thread_state, s[11]);
	OFFSET(THREAD_SP, riscv_thread_state, sp);
    
    OFFSET(THREAD_TH, riscv_thread_state, thread);

	OFFSET(THREAD_F0,  riscv_thread_state, fstate.f[0]);
	OFFSET(THREAD_F1,  riscv_thread_state, fstate.f[1]);
	OFFSET(THREAD_F2,  riscv_thread_state, fstate.f[2]);
	OFFSET(THREAD_F3,  riscv_thread_state, fstate.f[3]);
	OFFSET(THREAD_F4,  riscv_thread_state, fstate.f[4]);
	OFFSET(THREAD_F5,  riscv_thread_state, fstate.f[5]);
	OFFSET(THREAD_F6,  riscv_thread_state, fstate.f[6]);
	OFFSET(THREAD_F7,  riscv_thread_state, fstate.f[7]);
	OFFSET(THREAD_F8,  riscv_thread_state, fstate.f[8]);
	OFFSET(THREAD_F9,  riscv_thread_state, fstate.f[9]);
	OFFSET(THREAD_F10, riscv_thread_state, fstate.f[10]);
	OFFSET(THREAD_F11, riscv_thread_state, fstate.f[11]);
	OFFSET(THREAD_F12, riscv_thread_state, fstate.f[12]);
	OFFSET(THREAD_F13, riscv_thread_state, fstate.f[13]);
	OFFSET(THREAD_F14, riscv_thread_state, fstate.f[14]);
	OFFSET(THREAD_F15, riscv_thread_state, fstate.f[15]);
	OFFSET(THREAD_F16, riscv_thread_state, fstate.f[16]);
	OFFSET(THREAD_F17, riscv_thread_state, fstate.f[17]);
	OFFSET(THREAD_F18, riscv_thread_state, fstate.f[18]);
	OFFSET(THREAD_F19, riscv_thread_state, fstate.f[19]);
	OFFSET(THREAD_F20, riscv_thread_state, fstate.f[20]);
	OFFSET(THREAD_F21, riscv_thread_state, fstate.f[21]);
	OFFSET(THREAD_F22, riscv_thread_state, fstate.f[22]);
	OFFSET(THREAD_F23, riscv_thread_state, fstate.f[23]);
	OFFSET(THREAD_F24, riscv_thread_state, fstate.f[24]);
	OFFSET(THREAD_F25, riscv_thread_state, fstate.f[25]);
	OFFSET(THREAD_F26, riscv_thread_state, fstate.f[26]);
	OFFSET(THREAD_F27, riscv_thread_state, fstate.f[27]);
	OFFSET(THREAD_F28, riscv_thread_state, fstate.f[28]);
	OFFSET(THREAD_F29, riscv_thread_state, fstate.f[29]);
	OFFSET(THREAD_F30, riscv_thread_state, fstate.f[30]);
	OFFSET(THREAD_F31, riscv_thread_state, fstate.f[31]);
	OFFSET(THREAD_FCSR, riscv_thread_state, fstate.fcsr);

	OFFSET(TI_THREAD, thread_info, thread);
	OFFSET(TI_FLAGS, thread_info, flags);
	OFFSET(TI_CPU, thread_info, cpu);

	DEFINE(PT_SIZE, sizeof(struct pt_regs));
	OFFSET(PT_SEPC, pt_regs, sepc);
	OFFSET(PT_RA, pt_regs, ra);
	OFFSET(PT_FP, pt_regs, s0);
	OFFSET(PT_S0, pt_regs, s0);
	OFFSET(PT_S1, pt_regs, s1);
	OFFSET(PT_S2, pt_regs, s2);
	OFFSET(PT_S3, pt_regs, s3);
	OFFSET(PT_S4, pt_regs, s4);
	OFFSET(PT_S5, pt_regs, s5);
	OFFSET(PT_S6, pt_regs, s6);
	OFFSET(PT_S7, pt_regs, s7);
	OFFSET(PT_S8, pt_regs, s8);
	OFFSET(PT_S9, pt_regs, s9);
	OFFSET(PT_S10, pt_regs, s10);
	OFFSET(PT_S11, pt_regs, s11);
	OFFSET(PT_SP, pt_regs, sp);
	OFFSET(PT_TP, pt_regs, tp);
	OFFSET(PT_A0, pt_regs, a0);
	OFFSET(PT_A1, pt_regs, a1);
	OFFSET(PT_A2, pt_regs, a2);
	OFFSET(PT_A3, pt_regs, a3);
	OFFSET(PT_A4, pt_regs, a4);
	OFFSET(PT_A5, pt_regs, a5);
	OFFSET(PT_A6, pt_regs, a6);
	OFFSET(PT_A7, pt_regs, a7);
	OFFSET(PT_T0, pt_regs, t0);
	OFFSET(PT_T1, pt_regs, t1);
	OFFSET(PT_T2, pt_regs, t2);
	OFFSET(PT_T3, pt_regs, t3);
	OFFSET(PT_T4, pt_regs, t4);
	OFFSET(PT_T5, pt_regs, t5);
	OFFSET(PT_T6, pt_regs, t6);
	OFFSET(PT_GP, pt_regs, gp);
	OFFSET(PT_SSTATUS, pt_regs, sstatus);
	OFFSET(PT_SBADADDR, pt_regs, sbadaddr);
	OFFSET(PT_SCAUSE, pt_regs, scause);
}
