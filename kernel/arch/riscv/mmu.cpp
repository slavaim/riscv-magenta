// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <magenta/errors.h>
#include <arch/mmu.h>
#include <kernel/vm.h>
#include <debug.h>
#include <assert.h>
#include <arch/riscv/pgtable.h>
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
    }

    return NO_ERROR;
}

status_t arch_mmu_destroy_aspace(arch_aspace_t* aspace)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
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

status_t arch_mmu_protect(arch_aspace_t* aspace, vaddr_t vaddr, size_t count, uint mmu_flags)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}

status_t arch_mmu_query(arch_aspace_t* aspace, vaddr_t vaddr, paddr_t* paddr, uint* mmu_flags)
{
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
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
