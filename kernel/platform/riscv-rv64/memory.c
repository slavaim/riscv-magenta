// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <assert.h>
#include <err.h>
#include <kernel/vm.h>
#include <kernel/vm/page.h>
#include <arch/riscv/setup.h>
#include <arch/riscv/page.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/sections.h>
#include <platform/riscv/memory.h>

/* statically allocate an array of pmm_arena_info_ts to be filled in at boot time */
#define PMM_ARENAS 4
static pmm_arena_info_t mem_arenas[PMM_ARENAS];

void boot_alloc_reserve(uintptr_t phys, size_t _len);

//
// reserves the address space occupied by the page table allocated
// by the bootloader which allocates pages just after the kernel
// image so the range is consecutive in the direct mapped virtual
// address range
//
static void reserve_boot_page_table(
    pte_t *table, uint depth, bool wire_in_arena)
{
    unsigned long i;
    uint next_depth = depth + 1;
    struct list_node list = LIST_INITIAL_VALUE(list);

    if (!table && 0 == depth)
    {
        assert(kernel_init_pgd);
        table = (pte_t *)kernel_init_pgd;
    }

    assert(table);

    //
    // reserve the page table's page
    //
    // - adjust boot allocator to remove
    // the page from it to not accidently
    // allocate it
    // - mark the page in arena as wired
    //
    // this mechanism are not the best choice but 
    // this is the only choice that exist in the current
    // kernel source base, take a look at
    // platform_preserve_ramdisk that uses the same trick
    //
    boot_alloc_reserve(__pa(table), PAGE_SIZE);

    //
    // if wire is true then the arena has been initialized and
    // pages can be wired
    //
    if (wire_in_arena) {

        if (1 == pmm_alloc_range(__pa(table), 1, &list)) {

            //
            // mark all of the pages we allocated as WIRED
            //
            vm_page_t *p;
            list_for_every_entry(&list, p, vm_page_t, free.node) {
                p->state = VM_PAGE_STATE_WIRED;
            }
        } else {
            panic("unable to reserve a boot page table\n");
        }
    } // end if (wire_in_arena)

    //
    // scan the ptes and reserve the next level page tables
    //
    for (i = 0; i < PTRS_PER_PTE; i++)
    {
        if (!pte_leaf(table[i]))
        {
            //
            // the current RV64 architecture has a 3 level page table
            //
            assert(next_depth < 3);
            reserve_boot_page_table(pfn_to_virt(pte_pfn(table[i])),
                                    next_depth, wire_in_arena);
        }
    }
}

static void reserve_boot_pages(bool wire_in_arena)
{
    ASSERT(kernel_init_pgd);
    reserve_boot_page_table((pte_t *)kernel_init_pgd, 0, wire_in_arena);
}

static int setup_system_arena(pmm_arena_info_t *mem_arena)
{
    mem_arena->name = "memory";
    mem_arena->base = PFN_PHYS(min_low_pfn);
    mem_arenas->size = PFN_PHYS(max_low_pfn - min_low_pfn);
    mem_arenas->priority = 1;
    mem_arenas->flags = PMM_ARENA_FLAG_KMAP;
    return 0;
}

void platform_mem_init(void)
{
    int error;
    int arena_count = 0;

    error = setup_memory_info();
    if (error)
        panic("setup_memory_info");

    setup_kernel_init_pgd();

    //
    // reserve page tables in the boot allocator but
    // do not wire in arena as the arena has not been
    // initialized and the boot allocator is required
    // for arena initialization, so the boot allocator
    // should at first become aware about the boot page
    // tables to not overwrite them
    //
    reserve_boot_pages(false);

    error = setup_system_arena(&mem_arenas[arena_count++]);
    if (error)
        panic("setup_system_arena");

    for (int i = 0; i < arena_count; i++)
        pmm_add_arena(&mem_arenas[i]);

    //
    // after the main arena has been set the
    // boot page tables can be wired
    //
    reserve_boot_pages(true);

}