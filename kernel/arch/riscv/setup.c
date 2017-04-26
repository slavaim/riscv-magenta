// Copyright 2017 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Some code has been borrowed from the Linux kernel which is under GPLv2 license.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <stdint.h>
#include <magenta/errors.h>
#include <arch/mmu.h>
#include <kernel/vm.h>
#include <arch/riscv/asm/sbi.h>
#include <arch/riscv/page.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/pfn.h>
#include <arch/riscv/setup.h>
#include <debug.h>

//
// a memory info reported by a bootloader,
// a global variable for a debug purposes
//
static memory_block_info setup_block_info;

int setup_memory_info(void)
{
    unsigned long error = 0;

	// check that this function was not called in the past
	ASSERT(0 == max_low_pfn);

	error = sbi_query_memory(0, &setup_block_info);
	ASSERT(!error);
	ASSERT((setup_block_info.base & ~PMD_MASK) == 0);
	ASSERT((setup_block_info.size & ~PMD_MASK) == 0);
	printf("Available physical memory: %ludMB\n", setup_block_info.size >> 20);
    if (error)
        panic("sbi_query_memory");

    //
	// The kernel image is mapped at VA=KERNEL_BASE and PA=info.base
    //
	va_pa_offset = KERNEL_BASE - setup_block_info.base;
	pfn_base = min_low_pfn = PFN_DOWN(setup_block_info.base);
	max_low_pfn = PFN_DOWN(setup_block_info.base + setup_block_info.size);
	max_mapnr = PFN_DOWN(setup_block_info.size);

    return error;
}

void setup_kernel_init_pgd(void)
{
    kernel_init_pgd = get_current_sptbr_va();
    if (!kernel_init_pgd)
        panic("get_current_sptbr_va");
}

