// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <assert.h>
#include <err.h>
#include <kernel/vm.h>
#include <arch/riscv/setup.h>
#include <arch/riscv/page.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/sections.h>
#include <memory.h>

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
static void reserve_boot_page_table(pte_t *table, uint depth)
{
    unsigned long i;
    uint next_depth = depth + 1;

    if (!table && 0 == depth)
    {
        assert(kernel_init_pgd);
        table = (pte_t *)kernel_init_pgd;
    }

    assert(table);

    //
    // reserve the page table's page
    //
    boot_alloc_reserve(__pa(table), PAGE_SIZE);

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
            reserve_boot_page_table(pfn_to_virt(pte_pfn(table[i])), next_depth);
        }
    }
}

static void reserve_boot_pages(void)
{
    ASSERT(kernel_init_pgd);
    reserve_boot_page_table((pte_t *)kernel_init_pgd, 0);
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
    reserve_boot_pages();

    error = setup_system_arena(&mem_arenas[arena_count++]);
    if (error)
        panic("setup_system_arena");

    for (int i = 0; i < arena_count; i++)
        pmm_add_arena(&mem_arenas[i]);
}