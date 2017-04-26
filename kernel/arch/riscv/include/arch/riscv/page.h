// Copyright 2017 The Fuchsia Authors
// Some code has been borrowed from the Linux kernel which is under GPLv2 license.
// 2017 Modified for Magenta by Slava Imameev.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <arch/riscv/asm/constant.h>
#include <arch/riscv/pfn.h>

#define PAGE_SHIFT	(12)
#define PAGE_SIZE_SHIFT PAGE_SHIFT
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE - 1))

__BEGIN_CDECLS

/*
 * Use struct definitions to apply C type checking
 */

/* Page Global Directory entry */
typedef struct {
	unsigned long pgd;
} pgd_t;

/* Page Table entry */
typedef struct {
	unsigned long pte;
} pte_t;

typedef struct {
	unsigned long pgprot;
} pgprot_t;

typedef struct page *pgtable_t;

#define pte_val(x)	((x).pte)
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) })
#define __pgd(x)	((pgd_t) { (x) })
#define __pgprot(x)	((pgprot_t) { (x) })

extern unsigned long va_pa_offset;
extern unsigned long pfn_base;

//
// min_low_pfn points to the start of DRAM
// and max_low_pfn to the end of DRAM
//
extern unsigned long max_low_pfn;
extern unsigned long min_low_pfn;

extern unsigned long max_mapnr;

//
// __pa converts virtual to physical address for the direct mapped area
// __va converts physical to virtual address for the direct mapped area 
//
#define __pa(x)		((unsigned long)(x) - va_pa_offset)
#define __va(x)		((void *)((unsigned long) (x) + va_pa_offset))

#define phys_to_pfn(phys)	(PFN_DOWN(phys))
#define pfn_to_phys(pfn)	(PFN_PHYS(pfn))

#define virt_to_pfn(vaddr)	(phys_to_pfn(__pa(vaddr)))
#define pfn_to_virt(pfn)	(__va(pfn_to_phys(pfn)))

#define page_to_phys(page)	(pfn_to_phys(page_to_pfn(page)))
#define page_to_bus(page)	(page_to_phys(page))

#define pfn_valid(pfn)		(((pfn) >= pfn_base) && (((pfn)-pfn_base) < max_mapnr))

__END_CDECLS