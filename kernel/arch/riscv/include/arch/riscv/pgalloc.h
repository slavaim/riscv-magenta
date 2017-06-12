/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <arch/riscv/pgtable.h>

#ifndef __PAGETABLE_PUD_FOLDED
static inline void pgd_populate(arch_aspace_t* aspace,
	pgd_t *pgd, unsigned long pfn, pgprot_t prot)
{
	set_pgd(pmd, __pgd((pfn << _PAGE_PFN_SHIFT) | prot.pgprot | _PAGE_TABLE));
}
#endif // __PAGETABLE_PUD_FOLDED

static inline void pud_populate(arch_aspace_t* aspace,
    pud_t *pud, unsigned long pfn, pgprot_t prot)
{
	set_pud(pud, __pud((pfn << _PAGE_PFN_SHIFT) | prot.pgprot | _PAGE_TABLE));
}

static inline void pmd_populate(arch_aspace_t* aspace,
	pmd_t *pmd, unsigned long pfn, pgprot_t prot)
{
	set_pmd(pmd, __pmd((pfn << _PAGE_PFN_SHIFT) | prot.pgprot | _PAGE_TABLE));
}

static inline void pte_populate(arch_aspace_t* aspace,
	pte_t *pte, unsigned long pfn, pgprot_t prot)
{
	set_pte(pte, __pte((pfn << _PAGE_PFN_SHIFT) | prot.pgprot | _PAGE_PRESENT));
}


