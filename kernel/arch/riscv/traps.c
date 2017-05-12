// Some code has been borrowed from the Linux kernel which is under GPLv2 license.
// 2017 Modified for Magenta by Slava Imameev
//

#include <asm.h>
#include <arch/riscv/asm/linkage.h>
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/asm/pt_regs.h>
#include <arch/riscv/trap.h>
#include <debug.h>

extern asmlinkage void handle_exception(void);
asmlinkage void do_trap_unknown(struct pt_regs *regs);
asmlinkage void do_trap_amo_misaligned(struct pt_regs *regs);
asmlinkage void do_trap_insn_misaligned(struct pt_regs *regs);
asmlinkage void do_trap_insn_illegal(struct pt_regs *regs);
asmlinkage void do_trap_break(struct pt_regs *regs);

#define SIGILL 4
#define SIGBUS 7

#define __SI_FAULT	0

/*
 * SIGILL si_codes
 */
#define ILL_ILLOPC	(__SI_FAULT|1)	/* illegal opcode */
#define ILL_ILLOPN	(__SI_FAULT|2)	/* illegal operand */
#define ILL_ILLADR	(__SI_FAULT|3)	/* illegal addressing mode */
#define ILL_ILLTRP	(__SI_FAULT|4)	/* illegal trap */
#define ILL_PRVOPC	(__SI_FAULT|5)	/* privileged opcode */
#define ILL_PRVREG	(__SI_FAULT|6)	/* privileged register */
#define ILL_COPROC	(__SI_FAULT|7)	/* coprocessor error */
#define ILL_BADSTK	(__SI_FAULT|8)	/* internal stack error */
#define NSIGILL		8

/*
 * SIGBUS si_codes
 */
#define BUS_ADRALN	(__SI_FAULT|1)	/* invalid address alignment */
#define BUS_ADRERR	(__SI_FAULT|2)	/* non-existent physical address */
#define BUS_OBJERR	(__SI_FAULT|3)	/* object specific hardware error */
/* hardware memory error consumed on a machine check: action required */
#define BUS_MCEERR_AR	(__SI_FAULT|4)
/* hardware memory error detected in process but not consumed: action optional*/
#define BUS_MCEERR_AO	(__SI_FAULT|5)
#define NSIGBUS		5

static void do_trap_error(struct pt_regs *regs, int signo, int code,
	unsigned long addr, const char *str)
{
	PANIC_UNIMPLEMENTED;
}

asmlinkage void do_trap_unknown(struct pt_regs *regs)
{
	do_trap_error(regs, SIGILL, ILL_ILLTRP, regs->sepc, "Oops - unknown exception");
}

asmlinkage void do_trap_amo_misaligned(struct pt_regs *regs)
{
	do_trap_error(regs, SIGBUS, BUS_ADRALN, regs->sepc, "Oops - AMO address misaligned");
}

asmlinkage void do_trap_insn_misaligned(struct pt_regs *regs)
{
	do_trap_error(regs, SIGBUS, BUS_ADRALN, regs->sepc, "Oops - instruction address misaligned");
}

asmlinkage void do_trap_insn_illegal(struct pt_regs *regs)
{
	do_trap_error(regs, SIGILL, ILL_ILLOPC, regs->sepc, "Oops - illegal instruction");
}

asmlinkage void do_trap_break(struct pt_regs *regs)
{
	PANIC_UNIMPLEMENTED;
}

void trap_init(void)
{
	/* Set sup0 scratch register to 0, indicating to exception vector
	   that we are presently executing in the kernel */
	csr_write(sscratch, 0);
	/* Set the exception vector address */
	//csr_write(stvec, &handle_exception);
}