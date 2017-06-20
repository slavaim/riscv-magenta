// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <string.h> // for memset
#include <magenta/errors.h>
#include <arch/mmu.h>
#include <kernel/vm.h>
#include <kernel/vm/pmm.h>
#include <debug.h>
#include <assert.h>
#include <lib/lib.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/pgalloc.h>
#include <arch/riscv/pgtable-walk.h>
#include <arch/riscv/tlbflush.h>
#include <arch/ops.h>

#define min(a, b)   (a) < (b) ? a : b

//
// if the page table entry doesn point to a leaf
// it doesn't have protection bits set
//
pgprot_t TABLE_PROT = {.pgprot = 0};

//
// allocates a page table of any level - pgd, pud, pmt, pte
// a counterpart is free_pt
//
static
inline
void* allocate_pt(paddr_t* _pa)
{
    void* va;
    va = pmm_alloc_kpage(_pa, NULL);
    //
    // check that the allocationn has been 
    // made from the direct mapped region
    // and this region is correctly mapped
    // by __pa, __va, vaddr_to_paddr, paddr_to_kvaddr
    //
    DEBUG_ASSERT(va && __pa(va) == *_pa && __va(*_pa) == va);
    DEBUG_ASSERT(va && vaddr_to_paddr(va) == (paddr_t)__pa(va));
    DEBUG_ASSERT(va && paddr_to_kvaddr(*_pa) == __va(*_pa));

    return va;
}

static
inline
void free_pt(void* va, size_t count)
{
    pmm_free_kpages(va, count);
}

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
    } else {
        DEBUG_ASSERT(base + size <= USER_TASK_SIZE );

        aspace->base = base;
        aspace->size = size;

        aspace->pt_virt = static_cast<pgd_t*>(allocate_pt(&aspace->pt_phys));
        if (!aspace->pt_virt)
            return ERR_NO_MEMORY;

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

/* convert user level mmu flags to pte flags */
static
pgprot_t
mmu_flags_to_pte_attr(uint flags) {

    pgprot_t prot;
    prot.pgprot = _PAGE_PRESENT;

    switch (flags & ARCH_MMU_FLAG_CACHE_MASK) {
    case ARCH_MMU_FLAG_CACHED:
        break;
    case ARCH_MMU_FLAG_WRITE_COMBINING:
    case ARCH_MMU_FLAG_UNCACHED:
        break;
    case ARCH_MMU_FLAG_UNCACHED_DEVICE:
        break;
    default:
        /* invalid user-supplied flag */
        DEBUG_ASSERT(!"mmu_flags_to_pte_attr 1");
        break;
    }

    DEBUG_ASSERT(flags & (ARCH_MMU_FLAG_PERM_WRITE |
                          ARCH_MMU_FLAG_PERM_READ |
                          ARCH_MMU_FLAG_PERM_USER |
                          ARCH_MMU_FLAG_PERM_EXECUTE));

    if (flags & ARCH_MMU_FLAG_PERM_WRITE)
        prot.pgprot |= _PAGE_WRITE;

    if (flags & ARCH_MMU_FLAG_PERM_READ)
        prot.pgprot |= _PAGE_READ;

    if (flags & ARCH_MMU_FLAG_PERM_USER)
        prot.pgprot |= _PAGE_USER;

    if (flags & ARCH_MMU_FLAG_PERM_EXECUTE)
        prot.pgprot |= _PAGE_EXEC;

    if (flags & ARCH_MMU_FLAG_NS) {
    }

    return prot;
}

static
ssize_t
riscv64_mmu_map(
    arch_aspace_t* aspace,
    vaddr_t        vaddr_in,
    paddr_t        paddr_in,
    size_t         size_in,
    pgprot_t       page_prot,
    pgd_t*         top_table
    )
{
    vaddr_t  vaddr = vaddr_in;
    paddr_t  paddr = paddr_in;
    size_t   size  = size_in;
    size_t   chunk_size;
    size_t   mapped_size;

    if ((vaddr & PAGE_MASK_LOW) ||
        (paddr & PAGE_MASK_LOW) ||
        (size & PAGE_MASK_LOW)) {
        return ERR_INVALID_ARGS;
    }

    mapped_size = 0;
    chunk_size = PAGE_SIZE;

    while (size) {

        //
        // get a PGD entry
        //
        pgd_t *pgd = pgd_offset(top_table, vaddr);
        DEBUG_ASSERT(!pgd_bad(*pgd));
        if (unlikely(pgd_bad(*pgd)))
            break;
        
        DEBUG_ASSERT(pgd_none(*pgd) || pgd_present(*pgd));
        if (pgd_none(*pgd)) {

            DEBUG_ASSERT(!"we should not be here as pud == pgd, see pgtable-nopud.h");

            unsigned long pud_p;
            pud_t* pud_v = static_cast<pud_t*>(allocate_pt(&pud_p));
            if (unlikely(!pud_v))
                break;

            arch_zero_page(pud_v);
            pgd_populate(aspace, pgd, phys_to_pfn(pud_p), TABLE_PROT);
        }

        //
        // get a PUD entry
        //
        pud_t *pud = pud_offset(pgd, vaddr);
        DEBUG_ASSERT(!pud_huge(*pud));
        if (pud_huge(*pud))
            panic("pdu_huge(*pud) in riscv64_mmu_map");

        DEBUG_ASSERT(pud_none(*pud) || pud_present(*pud));
        if (pud_none(*pud)) {

            unsigned long  pmd_p;
            pmd_t* pmd_v = static_cast<pmd_t*>(allocate_pt(&pmd_p));
            if (unlikely(!pmd_v))
                break;

            arch_zero_page(pmd_v);
            pud_populate(aspace, pud, phys_to_pfn(pmd_p), TABLE_PROT);
        }

        //
        // get a PMD entry
        //
        pmd_t *pmd = pmd_offset(pud, vaddr);
        DEBUG_ASSERT(!pmd_huge(*pmd));
        if (pmd_huge(*pmd))
            panic("pmd_huge(*pmd) in riscv64_mmu_map");

        DEBUG_ASSERT(pmd_none(*pmd) || pmd_present(*pmd));
        if (pmd_none(*pmd)) {

            unsigned long  pte_p;
            pte_t* pte_v = static_cast<pte_t*>(allocate_pt(&pte_p));
            if (unlikely(!pte_v))
                break;

            arch_zero_page(pte_v);
            pmd_populate(aspace, pmd, phys_to_pfn(pte_p), TABLE_PROT);
        }

        //
        // get a PTE entry
        //
        pte_t *pte = pte_offset(pmd, vaddr);
        DEBUG_ASSERT(pte_none(*pte));
        if (!pte_none(*pte) || pte_present(*pte))
            break; // oops the entry is mapped

        //
        // set the pte to finalize vaddr to paddr mapping
        //
        pte_populate(aspace, pte, phys_to_pfn(paddr), page_prot);

        //
        // move to the next page
        //
        vaddr += chunk_size;
        paddr += chunk_size;
        size  -= chunk_size;
        mapped_size += chunk_size;
    } // end while

    return mapped_size;
}

/* routines to map/unmap/update permissions/query mappings per address space */
status_t
arch_mmu_map(
    arch_aspace_t* aspace,
    vaddr_t vaddr,
    paddr_t paddr,
    size_t count,
    uint mmu_flags,
    size_t* mapped
    )
{
    DEBUG_ASSERT(aspace);
    DEBUG_ASSERT(aspace->magic == ARCH_ASPACE_MAGIC);
    DEBUG_ASSERT(aspace->pt_virt);

    DEBUG_ASSERT(is_valid_vaddr(aspace, vaddr));
    if (!is_valid_vaddr(aspace, vaddr))
        return ERR_OUT_OF_RANGE;

    if (!(mmu_flags & ARCH_MMU_FLAG_PERM_READ))
        return ERR_INVALID_ARGS;

    /* paddr and vaddr must be aligned */
    DEBUG_ASSERT(IS_PAGE_ALIGNED(vaddr));
    DEBUG_ASSERT(IS_PAGE_ALIGNED(paddr));
    if (!IS_PAGE_ALIGNED(vaddr) || !IS_PAGE_ALIGNED(paddr))
        return ERR_INVALID_ARGS;

    if (count == 0)
        return NO_ERROR;

    ssize_t  bmp; // stands for BytesMaPped
    if (aspace->flags & ARCH_ASPACE_FLAG_KERNEL) {

        bmp = riscv64_mmu_map(aspace,
                              vaddr,
                              paddr,
                              count * PAGE_SIZE,
                              mmu_flags_to_pte_attr(mmu_flags),
                              kernel_init_pgd);
    } else {
        DEBUG_ASSERT(aspace->pt_virt != kernel_init_pgd);
        DEBUG_ASSERT((vaddr + count * PAGE_SIZE) <= USER_ASPACE_MAXADDR);

        bmp = riscv64_mmu_map(aspace,
                              vaddr,
                              paddr,
                              count * PAGE_SIZE,
                              mmu_flags_to_pte_attr(mmu_flags),
                              aspace->pt_virt);
    }

    DEBUG_ASSERT(0 == (bmp % PAGE_SIZE));

    if (mapped) {
        *mapped = (bmp > 0) ? (bmp / PAGE_SIZE) : 0u;
        DEBUG_ASSERT(*mapped <= count);
    }

    return (bmp < 0) ? (status_t)bmp : NO_ERROR;
}

static
ssize_t
riscv64_mmu_unmap(
    vaddr_t vaddr_in,
    size_t  size_in,
    pgd_t*  top_table
)
{
    vaddr_t  vaddr = vaddr_in;
    size_t   size  = size_in;
    size_t   chunk_size;
    size_t   unmapped_size;

    if ((vaddr & PAGE_MASK_LOW) ||
        (size & PAGE_MASK_LOW)) {
        return ERR_INVALID_ARGS;
    }

    unmapped_size = 0;
    chunk_size = PAGE_SIZE;

    while (size) {

        //
        // get a PGD entry
        //
        pgd_t *pgd = pgd_offset(top_table, vaddr);
        DEBUG_ASSERT(!pgd_bad(*pgd));
        if (unlikely(pgd_bad(*pgd)))
            break;
        
        DEBUG_ASSERT(pgd_present(*pgd));
        if (pgd_present(*pgd)) {

            //
            // get a PUD entry
            //
            pud_t *pud = pud_offset(pgd, vaddr);
            DEBUG_ASSERT(!pud_huge(*pud));
            if (pud_huge(*pud))
                panic("pdu_huge(*pud) in riscv64_mmu_map");

            if (pud_present(*pud)) {

                //
                // get a PMD entry
                //
                pmd_t *pmd = pmd_offset(pud, vaddr);
                DEBUG_ASSERT(!pmd_huge(*pmd));
                if (pmd_huge(*pmd))
                    panic("pmd_huge(*pmd) in riscv64_mmu_map");

                if (pmd_present(*pmd)) {

                    //
                    // get a PTE entry
                    //
                    pte_t *pte = pte_offset(pmd, vaddr);

                    //
                    // invalidate the pte
                    //
                    if (pte_present(*pte))
                        pte_set_none(pte);
                    
                    chunk_size = PAGE_SIZE;

                } else {
                     chunk_size = min(size, PMD_PAGE_SIZE);
                }
            } else {
                 chunk_size = min(size, PUD_PAGE_SIZE);
            }
        } else {
            chunk_size = min(size, PGD_PAGE_SIZE);
        }

        //
        // move to the next page
        //
        vaddr += chunk_size;
        size  -= chunk_size;
        unmapped_size += chunk_size;
    } // end while

    //
    // Free the page tables, for the kernel keep
    // the page tables. In case of the kernel at least
    // PMD ( or PUD if exist ) should be keept
    // as it is not possible to invalidate PGD
    // entries for all processes
    //
    if (kernel_init_pgd != top_table && unmapped_size > 0) {

        //
        // an exhaustive search is a very ugly solution,
        // the better one is to keep the number of valid PTEs
        // in the corresponding vm_page_t , TO_DO_RISCV
        //
        PANIC_UNIMPLEMENTED;
    }

    return unmapped_size;
}

status_t
arch_mmu_unmap(
    arch_aspace_t* aspace,
    vaddr_t vaddr,
    size_t count,
    size_t* unmapped
    )
{
    DEBUG_ASSERT(aspace);
    DEBUG_ASSERT(aspace->magic == ARCH_ASPACE_MAGIC);
    DEBUG_ASSERT(aspace->pt_virt);

    DEBUG_ASSERT(is_valid_vaddr(aspace, vaddr));

    if (!is_valid_vaddr(aspace, vaddr))
        return ERR_OUT_OF_RANGE;

    DEBUG_ASSERT(IS_PAGE_ALIGNED(vaddr));
    if (!IS_PAGE_ALIGNED(vaddr))
        return ERR_INVALID_ARGS;

    ssize_t ret;
    if (aspace->flags & ARCH_ASPACE_FLAG_KERNEL) {
        ret = riscv64_mmu_unmap(vaddr,
                                count * PAGE_SIZE,
                                kernel_init_pgd);
    } else {
        ret = riscv64_mmu_unmap(vaddr,
                                count * PAGE_SIZE,
                                aspace->pt_virt);
    }

    if (unmapped) {
        *unmapped = (ret > 0) ? (ret / PAGE_SIZE) : 0u;
        DEBUG_ASSERT(*unmapped <= count);
    }

    return (ret < 0) ? (status_t)ret : 0;
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
    // check for overflow
    //
    if (start > end)
        return ERR_INVALID_ARGS;

    //
    // check that the range doesn't span from
    // lower(user) to upper(kernel) half
    //
    if (start < USER_ASPACE_MAXADDR && end > USER_ASPACE_MAXADDR)
        return ERR_INVALID_ARGS;

    //
    // use the kernel page table for kernel space
    // as a reference table
    //
    pgd_t*  pt_virt;
    if (start > USER_ASPACE_MAXADDR)
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
            if (vaddr >= KERNEL_ASPACE_BASE)
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
    if (likely(old_aspace != aspace)) {

        if (aspace) {
		    csr_write(sptbr, virt_to_pfn(aspace->pt_virt));
        } else {
            csr_write(sptbr, virt_to_pfn(kernel_init_pgd));
        }

        local_flush_tlb_all();
	}
}

void arch_disable_mmu(void)
{
    PANIC_UNIMPLEMENTED;
}
