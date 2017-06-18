/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/
#pragma once

#include <debug.h>
#include <arch/riscv/asm/csr.h>

/* Flush entire local TLB */
static inline void local_flush_tlb_all(void)
{
	__asm__ __volatile__ ("sfence.vm");
}

/* Flush one page from local TLB */
static inline void local_flush_tlb_page(unsigned long addr)
{
	__asm__ __volatile__ ("sfence.vm %0" : : "r" (addr));
}

#if !WITH_SMP

#define flush_tlb_all() local_flush_tlb_all()
#define flush_tlb_page(vma, addr) local_flush_tlb_page(addr)
#define flush_tlb_range(vma, start, end) local_flush_tlb_all()

#else /* CONFIG_SMP */

#include <arch/riscv/sbi.h>

#define flush_tlb_all() sbi_remote_sfence_vm(0, 0)
#define flush_tlb_page(vma, addr) flush_tlb_range(vma, (addr), (addr) + 1)
#define flush_tlb_range(vma, start, end) \
	sbi_remote_sfence_vm_range(0, 0, (start), (end) - (start))

#endif /* CONFIG_SMP */

/* Flush the TLB entries of the specified mm context */
static inline void flush_tlb_mm(struct mm_struct *mm)
{
	flush_tlb_all();
}

/* Flush a range of kernel pages */
static inline void flush_tlb_kernel_range(unsigned long start,
	unsigned long end)
{
	flush_tlb_all();
}


