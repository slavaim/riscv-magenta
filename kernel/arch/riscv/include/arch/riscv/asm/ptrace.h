/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <magenta/compiler.h>
#include <arch/riscv/asm/constant.h>
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/asm/pt_regs.h>

#ifndef __ASSEMBLY__

__BEGIN_CDECLS

#ifdef CONFIG_64BIT
#define REG_FMT "%016lx"
#else
#define REG_FMT "%08lx"
#endif

#define user_mode(regs) (((regs)->sstatus & SR_PS) == 0)


/* Helpers for working with the instruction pointer */
#define GET_IP(regs) ((regs)->sepc)
#define SET_IP(regs, val) (GET_IP(regs) = (val))

static inline unsigned long instruction_pointer(struct pt_regs *regs)
{
	return GET_IP(regs);
}
static inline void instruction_pointer_set(struct pt_regs *regs,
                                           unsigned long val)
{
	SET_IP(regs, val);
}

#define profile_pc(regs) instruction_pointer(regs)

/* Helpers for working with the user stack pointer */
#define GET_USP(regs) ((regs)->sp)
#define SET_USP(regs, val) (GET_USP(regs) = (val))

static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return GET_USP(regs);
}
static inline void user_stack_pointer_set(struct pt_regs *regs,
                                          unsigned long val)
{
	SET_USP(regs, val);
}

/* Helpers for working with the frame pointer */
#define GET_FP(regs) ((regs)->s0)
#define SET_FP(regs, val) (GET_FP(regs) = (val))

static inline unsigned long frame_pointer(struct pt_regs *regs)
{
	return GET_FP(regs);
}
static inline void frame_pointer_set(struct pt_regs *regs,
                                     unsigned long val)
{
	SET_FP(regs, val);
}

__END_CDECLS

#endif /* __ASSEMBLY__ */

