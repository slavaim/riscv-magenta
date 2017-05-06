// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <string.h>
#include <magenta/errors.h>
#include <arch/mmu.h>
#include <kernel/vm.h>
#include <debug.h>
#include <assert.h>
#include <lib/lib.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/pgtable-walk.h>
#include <arch/ops.h>

/* initialize per address space */
status_t arch_mmu_init_aspace(arch_aspace_t* aspace, vaddr_t base, size_t size, uint flags)
{

    DEBUG_ASSERT(aspace);
    DEBUG_ASSERT(aspace->magic != ARCH_ASPACE_MAGIC);

    /* validate that the base + size is sane and doesn't wrap */
    DEBUG_ASSERT(size > PAGE_SIZE);
    DEBUG_ASSERT(base + size - 1 > base);

    aspace->magic = ARCH_ASPACE_MAGIC;
    aspace->flags = flags;
    if (flags & ARCH_ASPACE_FLAG_KERNEL) {

        aspace->base = base;
        aspace->size = size;
        aspace->pt_virt = kernel_init_pgd;
        aspace->pt_phys = __pa(aspace->pt_virt);
        //assert(vaddr_to_paddr(aspace->pt_virt) == __pa(aspace->pt_virt));
    } else {
        //DEBUG_ASSERT(base >= 0);
        DEBUG_ASSERT(base + size <= TASK_SIZE );

        aspace->base = base;
        aspace->size = size;

        paddr_t pa;
        pgd_t* va = static_cast<pgd_t*>(pmm_alloc_kpage(&pa, NULL));
        if (!va)
            return ERR_NO_MEMORY;

        aspace->pt_virt = va;
        aspace->pt_phys = pa;

        /* zero the top level translation table */
        /* XXX remove when PMM starts returning pre-zeroed pages */
        arch_zero_page(aspace->pt_virt);

        /* zero out the user space portion of it */
        memset(aspace->pt_virt, 0, sizeof(pgd_t) * USER_PTRS_PER_PGD);

        /* copy the kernel portion of it from the master kernel pt */
        memcpy(aspace->pt_virt + USER_PTRS_PER_PGD, &kernel_init_pgd[USER_PTRS_PER_PGD],
               sizeof(pgd_t) * KERNEL_PTRS_PER_PGD);
    }

    return NO_ERROR;
}

status_t arch_mmu_destroy_aspace(arch_aspace_t* aspace)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}

/* test the vaddr against the address space's range */
static bool is_valid_vaddr(arch_aspace_t* aspace, vaddr_t vaddr) {
    return (vaddr >= aspace->base && vaddr <= aspace->base + aspace->size - 1);
}

/* routines to map/unmap/update permissions/query mappings per address space */
status_t arch_mmu_map(arch_aspace_t* aspace, vaddr_t vaddr, paddr_t paddr, size_t count, uint mmu_flags, size_t* mapped)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}

status_t arch_mmu_unmap(arch_aspace_t* aspace, vaddr_t vaddr, size_t count, size_t* unmapped)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}

static inline void set_pte_mmu_flags(pte_t pte, uint mmu_flags)
{
    unsigned long pgprot = 0;

    if (mmu_flags & ARCH_MMU_FLAG_PERM_WRITE)
        pgprot |= _PAGE_WRITE;
    
    if (mmu_flags & ARCH_MMU_FLAG_PERM_READ)
        pgprot |= _PAGE_READ;

    if (mmu_flags & ARCH_MMU_FLAG_PERM_EXECUTE)
        pgprot |= _PAGE_EXEC;

    if (mmu_flags & ARCH_MMU_FLAG_PERM_USER)
        pgprot |= _PAGE_USER;

    pte_modify_access(pte, __pgprot(pgprot));
}

static inline uint get_pte_mmu_flags(pte_t pte)
{
    uint mmu_flags = 0;

    if (pte_write(pte))
        mmu_flags |= ARCH_MMU_FLAG_PERM_WRITE;
    
    if (pte_read(pte))
        mmu_flags |= ARCH_MMU_FLAG_PERM_READ;

    if (pte_exec(pte))
        mmu_flags |= ARCH_MMU_FLAG_PERM_EXECUTE;

    if (pte_user(pte))
        mmu_flags |= ARCH_MMU_FLAG_PERM_USER;

    return mmu_flags;
}

static inline uint get_pdu_mmu_flags(pud_t pdu)
{
    pte_t pte = {.pte = pud_val(pdu) };

    return get_pte_mmu_flags(pte);
}

static inline uint get_pmd_mmu_flags(pmd_t pmd)
{
    pte_t pte = {.pte = pmd_val(pmd) };

    return get_pte_mmu_flags(pte);
}

status_t arch_mmu_protect(arch_aspace_t* aspace, vaddr_t vaddr, size_t count, uint mmu_flags)
{
    DEBUG_ASSERT(aspace);
    DEBUG_ASSERT(aspace->magic == ARCH_ASPACE_MAGIC);

    if (count == 0)
        return NO_ERROR; // nothing to do

    //
    // the end is the last byte of the range instead of a usual
    // "first byte after the last range's byte" this is done
    // to prevent overflowing of the end value when a range
    // is at the end of the CPU address range. That means
    // that the range is [start, end] instead of [start, end)
    // if expressed in mathematical terms.
    //
    vaddr_t start = vaddr;
    vaddr_t end = vaddr + ((count * PAGE_SIZE) - 1);

    if (!is_valid_vaddr(aspace, start) ||
        !is_valid_vaddr(aspace, end))
        return ERR_INVALID_ARGS;

    if (!IS_PAGE_ALIGNED(start))
        return ERR_INVALID_ARGS;

    if (!(mmu_flags & ARCH_MMU_FLAG_PERM_READ))
        return ERR_INVALID_ARGS;

    //
    // check that the range doesn't span from
    // lower(user) to upper(kernel) half
    //
    if (start < HIGHEST_UA && end > HIGHEST_UA)
        return ERR_INVALID_ARGS;

    //
    // check for overflow
    //
    if (start > end)
        return ERR_INVALID_ARGS;

    //
    // use the kernel page table for kernel space
    // as a reference table
    //
    pgd_t*  pt_virt;
    if (start > HIGHEST_UA)
        pt_virt = kernel_init_pgd;
    else
        pt_virt = aspace->pt_virt;
    
    for (size_t i = 0; i < count; ++i) {

        vaddr_t  pva = start + (i * PAGE_SIZE);

        pgd_t *pgd = pgd_offset(pt_virt, pva);
        if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
            continue;

        //
        // no pud in the current release so pud == pgd ,
        // nevertehless the pud code is present
        //
        pud_t *pud = pud_offset(pgd, pva);
        if (pud_none(*pud) || unlikely(pud_bad(*pud)))
            continue;

        if (pud_huge(*pud)) {

            panic("huge pud, we should not be here as pud is not used");
            continue;
        }

        pmd_t *pmd = pmd_offset(pud, pva);
        if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
            continue;

        if (pmd_huge(*pmd)) {

            //
            // in case of a kernel space carry on,
            // we need to divide huge pmd on ptes and
            // apply new flags to each pte individually
            // TO_DO_RISCV
            //
            if (vaddr > HIGHEST_UA)
                continue;

            //
            // huge pmd is not used for user space range
            // 
            panic("huge pmd for user space");
            continue;
        }

        pte_t *pte = pte_offset(pmd, pva);
        if (pte_none(*pte) || !pte_present(*pte))
            continue;

        //
        // modify the pte
        //
        pte_t new_pte = *pte;
        set_pte_mmu_flags(new_pte, mmu_flags);
        *pte = new_pte;
    } // end for

    return NO_ERROR;
}

status_t arch_mmu_query(arch_aspace_t *aspace, vaddr_t vaddr, paddr_t *p_paddr, uint *p_mmu_flags)
{
    if (!is_valid_vaddr(aspace, vaddr))
        return ERR_INVALID_ARGS;

    pgd_t *pgd = pgd_offset(aspace->pt_virt, vaddr);
    if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
        return ERR_NOT_FOUND;

    //
    // no pud in the current release so pud == pgd ,
    // nevertehless the pud code is present
    //
    pud_t *pud = pud_offset(pgd, vaddr);
    if (pud_none(*pud) || unlikely(pud_bad(*pud)))
        return ERR_NOT_FOUND;

    if (pud_huge(*pud)) {

        assert(!"we should not be here as PUD is not used");
        paddr_t paddr = follow_huge_pud_to_phys(*pud, vaddr);
        if (paddr) {

            if (p_paddr)
                *p_paddr = paddr;

            if (p_mmu_flags)
                *p_mmu_flags = get_pdu_mmu_flags(*pud);

            return NO_ERROR;
        }

        return ERR_NOT_FOUND;
    }

    pmd_t *pmd = pmd_offset(pud, vaddr);
    if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
        return ERR_NOT_FOUND;

    if (pmd_huge(*pmd)) {

        paddr_t paddr = follow_huge_pmd_to_phys(*pmd, vaddr);
        if (paddr) {

            if (p_paddr)
                *p_paddr = paddr;

            if (p_mmu_flags)
                *p_mmu_flags = get_pmd_mmu_flags(*pmd);

            return NO_ERROR;
        }

        return ERR_NOT_FOUND;
    }

    pte_t *pte = pte_offset(pmd, vaddr);
    if (pte_none(*pte) || !pte_present(*pte))
        return ERR_NOT_FOUND;

    paddr_t paddr = follow_pte_to_phys(*pte, vaddr);
    if (paddr) {

        if (p_paddr)
            *p_paddr = paddr;

        if (p_mmu_flags)
            *p_mmu_flags = get_pte_mmu_flags(*pte);

        return NO_ERROR;
    }

    return ERR_NOT_FOUND;
}

/*
vaddr_t arch_mmu_pick_spot(const arch_aspace_t* aspace,
                           vaddr_t base, uint prev_region_mmu_flags,
                           vaddr_t end, uint next_region_mmu_flags,
                           vaddr_t align, size_t size, uint mmu_flags) __NONNULL((1));
                           */

/* load a new user address space context.
 * aspace argument NULL should unload user space.
 */
void arch_mmu_context_switch(arch_aspace_t* old_aspace, arch_aspace_t* aspace)
{
    PANIC_UNIMPLEMENTED;
}

void arch_disable_mmu(void)
{
    PANIC_UNIMPLEMENTED;
}
