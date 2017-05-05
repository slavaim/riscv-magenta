/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <arch/riscv/asm/constant.h>

__BEGIN_CDECLS

#define PGDIR_SHIFT     30
/* Size of region mapped by a page global directory */
#define PGDIR_SIZE      (_AC(1,UL) << PGDIR_SHIFT)
#define PGDIR_MASK      (~(PGDIR_SIZE - 1))

#define PMD_SHIFT       21
/* Size of region mapped by a page middle directory */
#define PMD_SIZE        (_AC(1,UL) << PMD_SHIFT)
#define PMD_MASK        (~(PMD_SIZE - 1))

#define PTE_SHIFT       12
/* Size of region mapped by a page table directory */
#define PTE_SIZE        (_AC(1,UL) << PTE_SHIFT)
#define PTE_MASK        (~(PTE_SIZE - 1))

/* Page Middle Directory entry */
typedef struct {
	unsigned long pmd;
} pmd_t;

#define pmd_val(x)      ((x).pmd)
#define __pmd(x)        ((pmd_t) { (x) })

#define PTRS_PER_PMD    (PAGE_SIZE / sizeof(pmd_t))

static inline int pud_present(pud_t pud)
{
	return (pud_val(pud) & _PAGE_PRESENT);
}

static inline int pud_none(pud_t pud)
{
	return (pud_val(pud) == 0);
}

static inline int pud_bad(pud_t pud)
{
	return !pud_present(pud);
}

static inline void set_pud(pud_t *pudp, pud_t pud)
{
	*pudp = pud;
}

static inline void pud_clear(pud_t *pudp)
{
	set_pud(pudp, __pud(0));
}

static inline unsigned long pud_page_vaddr(pud_t pud)
{
	return (unsigned long)pfn_to_virt(pud_val(pud) >> _PAGE_PFN_SHIFT);
}

#define pmd_index(addr) (((addr) >> PMD_SHIFT) & (PTRS_PER_PMD - 1))

static inline pmd_t *pmd_offset(pud_t *pud, unsigned long addr)
{
	return (pmd_t *)pud_page_vaddr(*pud) + pmd_index(addr);
}

#define pmd_ERROR(e) \
	panic("%s:%d: bad pmd %016lx.\n", __FILE__, __LINE__, pmd_val(e))

__END_CDECLS

