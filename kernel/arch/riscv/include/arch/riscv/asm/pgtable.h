// Copyright 2017 Slava Imameev.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT
//

#pragma once

#include <arch/riscv/asm/constant.h>
#include <arch/riscv/asm/page.h>

#if __riscv_xlen == 64

/*
 * RV64Sv39
 *
 * RV64Sv39 page table entry:
 * | 63           39 | 38 10 | 9             7 | 6 | 5 | 4  1 | 0
 *   reserved for HW    PFN    reserved for SW   D   R   TYPE   V
 */

    #define NM_PAGE_TABLES  (3)
    #define PGLEVEL_BITS    (9)

    #define PTE_PAGE_SIZE   PAGE_SIZE
    #define PMD_PAGE_SIZE   (PAGE_SIZE << PGLEVEL_BITS)
    #define PUD_PAGE_SIZE   PGD_PAGE_SIZE /* no pud, pud == pgd */
    #define PGD_PAGE_SIZE   (PMD_PAGE_SIZE << PGLEVEL_BITS)

    #define VA_BITS ((NM_PAGE_TABLES * PGLEVEL_BITS) + PAGE_SHIFT) /* 39 bits */

    #define PGDIR_SHIFT     (VA_BITS - PGLEVEL_BITS) /* 30 bits */
    /* Size of region mapped by a page global directory */
    #define PGDIR_SIZE      (_AC(1,UL) << PGDIR_SHIFT)
    #define PGDIR_MASK      (~(PGDIR_SIZE - 1))

    /* no pud, pud == pgd, see pgtable-nopud.h */

    #define PMD_SHIFT       (PGDIR_SHIFT - PGLEVEL_BITS) /* 21 bits */
    /* Size of region mapped by a page middle directory */
    #define PMD_SIZE        (_AC(1,UL) << PMD_SHIFT)
    #define PMD_MASK        (~(PMD_SIZE - 1))

    #define PTE_SHIFT       PAGE_SHIFT /* 12 bits */
    /* Size of region mapped by a page table directory */
    #define PTE_SIZE        (_AC(1,UL) << PTE_SHIFT)
    #define PTE_MASK        (~(PTE_SIZE - 1))

#else
    # error "32 bit RISC-V is not supported"
#endif