	.file	"asm-offsets.c"
	.option nopic
	.text
	.align	2
	.globl	asm_offsets
	.type	asm_offsets, @function
asm_offsets:
	add	sp,sp,-16
	sd	s0,8(sp)
	add	s0,sp,16
#APP
# 67 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_SIZE 280 sizeof(struct pt_regs)
# 0 "" 2
# 68 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_SEPC 0 offsetof(struct pt_regs, sepc)
# 0 "" 2
# 69 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_RA 8 offsetof(struct pt_regs, ra)
# 0 "" 2
# 70 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_FP 64 offsetof(struct pt_regs, s0)
# 0 "" 2
# 71 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S0 64 offsetof(struct pt_regs, s0)
# 0 "" 2
# 72 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S1 72 offsetof(struct pt_regs, s1)
# 0 "" 2
# 73 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S2 144 offsetof(struct pt_regs, s2)
# 0 "" 2
# 74 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S3 152 offsetof(struct pt_regs, s3)
# 0 "" 2
# 75 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S4 160 offsetof(struct pt_regs, s4)
# 0 "" 2
# 76 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S5 168 offsetof(struct pt_regs, s5)
# 0 "" 2
# 77 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S6 176 offsetof(struct pt_regs, s6)
# 0 "" 2
# 78 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S7 184 offsetof(struct pt_regs, s7)
# 0 "" 2
# 79 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S8 192 offsetof(struct pt_regs, s8)
# 0 "" 2
# 80 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S9 200 offsetof(struct pt_regs, s9)
# 0 "" 2
# 81 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S10 208 offsetof(struct pt_regs, s10)
# 0 "" 2
# 82 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_S11 216 offsetof(struct pt_regs, s11)
# 0 "" 2
# 83 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_SP 16 offsetof(struct pt_regs, sp)
# 0 "" 2
# 84 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_TP 32 offsetof(struct pt_regs, tp)
# 0 "" 2
# 85 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A0 80 offsetof(struct pt_regs, a0)
# 0 "" 2
# 86 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A1 88 offsetof(struct pt_regs, a1)
# 0 "" 2
# 87 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A2 96 offsetof(struct pt_regs, a2)
# 0 "" 2
# 88 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A3 104 offsetof(struct pt_regs, a3)
# 0 "" 2
# 89 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A4 112 offsetof(struct pt_regs, a4)
# 0 "" 2
# 90 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A5 120 offsetof(struct pt_regs, a5)
# 0 "" 2
# 91 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A6 128 offsetof(struct pt_regs, a6)
# 0 "" 2
# 92 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_A7 136 offsetof(struct pt_regs, a7)
# 0 "" 2
# 93 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T0 40 offsetof(struct pt_regs, t0)
# 0 "" 2
# 94 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T1 48 offsetof(struct pt_regs, t1)
# 0 "" 2
# 95 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T2 56 offsetof(struct pt_regs, t2)
# 0 "" 2
# 96 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T3 224 offsetof(struct pt_regs, t3)
# 0 "" 2
# 97 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T4 232 offsetof(struct pt_regs, t4)
# 0 "" 2
# 98 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T5 240 offsetof(struct pt_regs, t5)
# 0 "" 2
# 99 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_T6 248 offsetof(struct pt_regs, t6)
# 0 "" 2
# 100 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_GP 24 offsetof(struct pt_regs, gp)
# 0 "" 2
# 101 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_SSTATUS 256 offsetof(struct pt_regs, sstatus)
# 0 "" 2
# 102 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_SBADADDR 264 offsetof(struct pt_regs, sbadaddr)
# 0 "" 2
# 103 "kernel/arch/riscv/asm-offsets.c" 1
	
->PT_SCAUSE 272 offsetof(struct pt_regs, scause)
# 0 "" 2
#NO_APP
	nop
	ld	s0,8(sp)
	add	sp,sp,16
	jr	ra
	.size	asm_offsets, .-asm_offsets
	.ident	"GCC: (GNU) 6.1.0"
