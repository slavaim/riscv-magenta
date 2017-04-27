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
#include <memory.h>

/* statically allocate an array of pmm_arena_info_ts to be filled in at boot time */
#define PMM_ARENAS 4
static pmm_arena_info_t mem_arenas[PMM_ARENAS];

static void reserve_boot_page_table(void)
{
	unsigned long i;
    pte_t*   table = (pte_t*)kernel_init_pgd;

    assert(table);

	pmm_alloc_range(__pa(table), 1, NULL);

	for (i = 0; i < PTRS_PER_PTE; i++) {
		if (pte_present(table[i]) && !pte_huge(table[i]))
			pmm_alloc_range(PFN_PHYS(pte_pfn(table[i])), 1, NULL);
	}
}

static int setup_system_arena(pmm_arena_info_t* mem_arena)
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
    int arena_count= 0;

    error = setup_memory_info();
    if(error)
        panic("setup_memory_info");

    setup_kernel_init_pgd();

    error = setup_system_arena(&mem_arenas[arena_count++]);
    if (error)
        panic("setup_system_arena");

    for (int i = 0; i < arena_count; i++)
        pmm_add_arena(&mem_arenas[i]);

    reserve_boot_page_table();
}