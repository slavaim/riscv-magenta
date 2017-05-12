//
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <stdio.h>
#include <debug.h>
#include <bits.h>
#include <inttypes.h>
#include <trace.h>
#include <kernel/thread.h>
#include <platform.h>
#include <arch/riscv/irq.h>
#include <arch/riscv/asm/linkage.h>
#include <arch/riscv/asm/asm-offsets.h>
#include <arch/riscv/asm/pt_regs.h>
#include <platform/riscv/timer.h>

#if WITH_LIB_MAGENTA
#include <lib/user_copy.h>
#include <magenta/exception.h>
#endif

typedef struct plic_context {
	volatile int priority_threshold;
	volatile int claim;
} plic_context_t;

//static plic_context_t  cpu_plic_context[SMP_MAX_CPUS];
//static int             cpu_irq_in_progress[SMP_MAX_CPUS];
static bool            cpu_in_int_handler[SMP_MAX_CPUS];
static struct pt_regs* cpu_pt_regs[SMP_MAX_CPUS];

bool arch_cpu_in_int_handler(uint cpu)
{
    assert(cpu < SMP_MAX_CPUS);
    return cpu_in_int_handler[cpu];
}

static void arch_set_in_int_handler(bool in_irq)
{
    uint cpu = arch_curr_cpu_num();
    cpu_in_int_handler[cpu] = in_irq;
}

static void riscv_software_interrupt(void)
{
	PANIC_UNIMPLEMENTED;
}

static void plic_interrupt(void)
{
    PANIC_UNIMPLEMENTED;
}

static struct pt_regs* set_irq_regs(struct pt_regs* new)
{
    uint               cpu = arch_curr_cpu_num();
    struct pt_regs*    old = cpu_pt_regs[cpu];

    cpu_pt_regs[cpu] = new;
    return old;
}

asmlinkage void do_IRQ(unsigned int cause, struct pt_regs *regs)
{
    enum handler_return ret = INT_NO_RESCHEDULE;
	struct pt_regs *old_regs = set_irq_regs(regs);
	arch_set_in_int_handler(true);

	/* There are three classes of interrupt: timer, software, and
	   external devices.  We dispatch between them here.  External
	   device interrupts use the generic IRQ mechanisms. */
	switch (cause) {
		case INTERRUPT_CAUSE_TIMER:
			ret = riscv_timer_interrupt();
			break;
		case INTERRUPT_CAUSE_SOFTWARE:
			riscv_software_interrupt();
			break;
		case INTERRUPT_CAUSE_EXTERNAL:
			plic_interrupt();
			break;
		default:
			assert(!"invalid cause for do_IRQ");
	}

	arch_set_in_int_handler(false);
	set_irq_regs(old_regs);

    if (ret != INT_NO_RESCHEDULE)
        thread_preempt(true);
}
