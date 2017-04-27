/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#include <arch/riscv/asm/csr.h>
/*
struct task_struct;

static inline struct task_struct *get_current(void)
{
	register struct task_struct * tp asm("tp");
	return tp;
}
*/
#define current (get_current())