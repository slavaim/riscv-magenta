//
// Copyright (c) 2017 Slava Imameev
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
#include <arch/riscv/faults.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/ptrace.h>
#include <arch/riscv/asm/linkage.h>
#include <arch/riscv/asm/asm-offsets.h>

#if WITH_LIB_MAGENTA
#include <lib/user_copy.h>
#include <magenta/exception.h>
#endif


/*
 * This routine handles page faults.  It determines the address and the
 * problem, and then passes it off to one of the appropriate routines.
 */
asmlinkage void do_page_fault(struct pt_regs *regs)
{
    vaddr_t addr = regs->sbadaddr;

    if (addr >= KERNEL_ASPACE_BASE && !user_mode(regs)) {

        //
        // Try  to fix the current process page table
        // with the entries from the kernel table,
        // if there is no entry in the master tables
        // try to page in data
        //

        pgd_t *pgd, *pgd_k;
        pud_t *pud, *pud_k;
        pmd_t *pmd, *pmd_k;
        pte_t *pte_k;
        int index;

        //
        // Synchronize this task's top level page-table
        // with the 'reference' page table.
        //
        index = pgd_index(addr);
        pgd = (pgd_t *)pfn_to_virt(csr_read(sptbr)) + index;
        pgd_k = kernel_init_pgd + index;

        if (!pgd_present(*pgd_k))
            goto page_in;
        set_pgd(pgd, *pgd_k);

        pud = pud_offset(pgd, addr);
        pud_k = pud_offset(pgd_k, addr);
        if (!pud_present(*pud_k))
            goto page_in;

        //
        // Since the kernel is global, it is unnecessary
		// to copy individual PTEs
        //
		pmd = pmd_offset(pud, addr);
		pmd_k = pmd_offset(pud_k, addr);
		if (!pmd_present(*pmd_k))
			goto page_in;
		set_pmd(pmd, *pmd_k);

		//
        // Make sure the actual PTE exists as well to
		// catch kernel area accesses to non-mapped
		// addresses. If we don't do this, this will just
		// silently loop forever.
		//
		pte_k = pte_offset_kernel(pmd_k, addr);
		if (!pte_present(*pte_k))
			goto page_in;

        //
        // the page table has been fixed
        //
		return;
    }

page_in:

	//
    // If interrupts were enabled in the parent context?
    //
	if (0x0 == (regs->sstatus & SR_PIE)) {
        //
        // a page fault when interrupts
        // are disabled that can't be fixed
        // with copying from kernel_init_pgd
        //
        panic("%s: page fault in non-paged area with interrupts disabled", __func__);
    }

    //
    // Call the high level page fault handler.
    // Enable interrupts to allow process scheduling
    // when reading a page.
    //
    arch_enable_ints();
    {

        //
        // convert the previous state and casue to page fault flags
        //
        uint flags = (user_mode(regs)) ? VMM_PF_FLAG_USER : 0;

        switch (regs->scause) {
        case EXC_INST_ACCESS:
            flags |= VMM_PF_FLAG_INSTRUCTION;
            break;
        case EXC_LOAD_ACCESS:
            break;
        case EXC_STORE_ACCESS:
            flags |= VMM_PF_FLAG_WRITE;
            break;
        default:
            panic("%s: unhandled cause %lu", __func__, regs->scause);
        }

        DEBUG_ASSERT(!arch_in_int_handler());

        status_t pf_err = vmm_page_fault_handler(addr, flags);
        if (pf_err != NO_ERROR) {

            //
            // if the high level page fault handler can't deal with it,
            // resort to trying to recover first, before bailing
            //

            //
            // Check if a resume address is specified, and just return to it if so
            //
            thread_t *current_thread = get_current_thread();
            if (unlikely(current_thread->arch.data_fault_resume))
                regs->sepc = (uintptr_t)current_thread->arch.data_fault_resume;
        }
    }
    arch_disable_ints();
    return;
}

#if WITH_LIB_MAGENTA
void arch_fill_in_suspension_context(mx_exception_report_t *report)
{
    mx_exception_context_t *mx_context = &report->context;

    mx_context->arch_id = ARCH_ID_RISCV_64;
}
#endif