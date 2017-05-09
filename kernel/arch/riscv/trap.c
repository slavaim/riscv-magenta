// Some code has been borrowed from the Linux kernel which is under GPLv2 license.
// 2017 Modified for Magenta by Slava Imameev
//

#include <asm.h>
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/trap.h>
#include <debug.h>

extern asmlinkage void handle_exception(void);

void trap_init(void)
{
	/* Set sup0 scratch register to 0, indicating to exception vector
	   that we are presently executing in the kernel */
	csr_write(sscratch, 0);
	/* Set the exception vector address */
	//csr_write(stvec, &handle_exception);
}