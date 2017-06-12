// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <assert.h>
#include <err.h>
#include <string.h>
#include <kernel/vm.h>
#include <kernel/vm/page.h>
#include <arch/riscv/setup.h>
#include <arch/riscv/page.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/sections.h>
#include <platform/riscv/memory.h>


//
// BBL(Berkeley Boot Loader) creates two VA regions
//  - SDRAM map at KERNEL_BASE
//  - SBI map at the top of the VA space
//
struct mmu_initial_mapping mmu_initial_mappings[] = {

    //
    // A range of memory mapped where the kernel lives.
    //
    // This mapping is used for identical physical to 
    // virtual mapping in the [ KERNEL_BASE, KERNEL_BASE+mem_size )
    // range. The size valie is set by setup_memory_info.
    //
    // This range will be reserved in a call to
    // vm_init_postheap, this mapping is common for all
    // RISC-V platforms and not platform specific.
    //
    // FYI a call stack when this VA region is being reserved
    //
    // #0  vm_init_postheap (level=327680) at kernel/kernel/vm/vm.cpp:139
    // #1  0xffffffff80004838 in lk_init_level (required_flag=LK_INIT_FLAG_PRIMARY_CPU, start_level=262144, stop_level=393215) at kernel/top/init.c:86
    // #2  0xffffffff800048dc in lk_primary_cpu_init_level (start_level=262144, stop_level=393215) at kernel/include/lk/init.h:51
    // #3  0xffffffff80004a24 in lk_main () at kernel/top/main.c:91
    // #4  0xffffffff80001528 in _riscv_start () at kernel/arch/riscv/rv64/start.S:42
    //
    {
        // the default DRAM base and size, changed to real value on boot
        // by a query to SBI
        .phys = 0x0,
        .virt = KERNEL_BASE,
        .size = -KERNEL_BASE,
        .flags = 0,
        .name = "SDRAM"
    },

    //
    // an SBI map at the top of the VA
    //
    {
        .phys = (-1),
        .virt = (-1),
        .size = 0,
        .flags = 0,
        .name = "SBI"
    },

    /* null entry to terminate the list */
    {}
};

/* statically allocate an array of pmm_arena_info_ts to be filled in at boot time */
#define PMM_ARENAS 4
static pmm_arena_info_t mem_arenas[PMM_ARENAS];

void boot_alloc_reserve(uintptr_t phys, size_t _len);

typedef void (*pte_function)(
    pte_t *table, vaddr_t va_mapped, vaddr_t page_size);

static void wire_in_arena(
    pte_t *table, vaddr_t va_mapped, vaddr_t page_size)
{
    struct list_node list = LIST_INITIAL_VALUE(list);

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
}

static void reserve_pte_page(
    pte_t *table, vaddr_t va_mapped, vaddr_t page_size)
{
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
}

static void add_SBI_page_initial_mapping(
    pte_t *table, vaddr_t va_mapped, vaddr_t page_size)
{
    unsigned long i;
    vaddr_t   DRAM_va_start = mmu_initial_mappings[0].virt;
    vaddr_t   DRAM_va_end   = mmu_initial_mappings[0].virt +
                              mmu_initial_mappings[0].size;

    assert(PAGE_SIZE <= page_size && page_size <= PGD_PAGE_SIZE);

    //
    // page tables allocations are accounted in SDRAM mapping,
    // so there is no need to account for table's va only
    // the table entries must be processed
    //

    for (i = 0; i < PTRS_PER_PTE; i++)
    {
        paddr_t pa;
        vaddr_t va;

        if (!pte_valid_va2pa(table[i]))
            continue;

        pa = (paddr_t)pte_to_phys(table[i]);
        //
        // __va and pfn_to_virt can't be used here
        // as SBI is mapped at the top of the VA space
        // outside the identical mapping range
        //
        va = get_canonical_va(va_mapped + i*page_size);
        assert(va >= DRAM_va_start);
        if (va < DRAM_va_end)
            continue;

        //
        // an SBI VA page has been detected
        //
        mmu_initial_mappings[1].size += page_size;

        if (va < mmu_initial_mappings[1].virt)
            mmu_initial_mappings[1].virt = va;

        if (pa < mmu_initial_mappings[1].phys)
            mmu_initial_mappings[1].phys = pa;
    }
}

//
// reserves the address space occupied by the page table allocated
// by the bootloader which allocates pages just after the kernel
// image so the range is consecutive in the direct mapped virtual
// address range
//
static void traverse_boot_page_table(
    pte_t *table,      // page table
    vaddr_t va_mapped, // va mapped by table[0]
    uint depth,
    pte_function ptef)
{
    unsigned long i;
    uint    next_depth = depth + 1;
    paddr_t page_size  = PGD_PAGE_SIZE >> (PGLEVEL_BITS*depth);

    assert(page_size >= PAGE_SIZE);

    if (!table && 0 == depth){
        
        assert(kernel_init_pgd);
        table = (pte_t *)kernel_init_pgd;
    }

    assert(table);

    //
    // apply a function to the pte
    //
    ptef(table, va_mapped, page_size);

    //
    // scan the ptes and process the next level page tables
    //
    for (i = 0; i < PTRS_PER_PTE; i++)
    {
        if (!pte_leaf(table[i]))
        {
            //
            // the RV64 architecture has a NM_PAGE_TABLES level page table
            //
            assert(next_depth < NM_PAGE_TABLES);
            traverse_boot_page_table(pfn_to_virt(pte_pfn(table[i])),
                                     va_mapped + i*page_size,
                                     next_depth,
                                     ptef);
        }
    }
}

static void traverse_boot_pages(pte_function ptef)
{
    ASSERT(kernel_init_pgd);
    traverse_boot_page_table((pte_t *)kernel_init_pgd, 0, 0, ptef);
}

static int setup_system_arena(pmm_arena_info_t *mem_arena)
{
    #define ARENA_NAME "SDRAM"
    #define _min(a, b)   ((a) < (b) ? a : b)
    memcpy(mem_arena->name, ARENA_NAME, _min(sizeof(ARENA_NAME), sizeof(mem_arena->name)));
    mem_arena->base = PFN_PHYS(min_low_pfn);
    mem_arenas->size = PFN_PHYS(max_low_pfn - min_low_pfn);
    mem_arenas->priority = 1;
    mem_arenas->flags = PMM_ARENA_FLAG_KMAP;
    return 0;
    #undef _min
    #undef ARENA_NAME
}

void platform_mem_init(void)
{
    int error;
    int arena_count = 0;
    memory_block_info*  mbi;

    mbi = setup_memory_info();
    if (!mbi)
        panic("setup_memory_info");

	//
	// Fix the initial mappings for physical address,
    // this will be an area for direct(identical) mapping 
    // which paddr_to_kvaddr uses to map pages from SDRAM
    // arena.
    // The same mapping can be done with the help of
    // va_pa_offset value, see __pa and __va macros.
	//
	mmu_initial_mappings[0].phys = mbi->base;
	mmu_initial_mappings[0].virt = KERNEL_BASE;
	mmu_initial_mappings[0].size = mbi->size;

    setup_kernel_init_pgd();

    //
    // set SBI initial mapping
    //
    traverse_boot_pages(add_SBI_page_initial_mapping);

    //
    // reserve page tables in the boot allocator but
    // do not wire in arena as the arena has not been
    // initialized and the boot allocator is required
    // for arena initialization, so the boot allocator
    // should at first become aware about the boot page
    // tables to not overwrite them
    //
    traverse_boot_pages(reserve_pte_page);

    error = setup_system_arena(&mem_arenas[arena_count++]);
    if (error)
        panic("setup_system_arena");

    assert(arena_count <= PMM_ARENAS);
    for (int i = 0; i < arena_count; i++)
        pmm_add_arena(&mem_arenas[i]);

    //
    // after the main arena has been set the
    // boot page tables can be wired
    //
    traverse_boot_pages(wire_in_arena);

}