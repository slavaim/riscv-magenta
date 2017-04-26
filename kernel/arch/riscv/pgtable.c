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
#include <arch/riscv/asm/csr.h>
#include <arch/riscv/page.h>
#include <arch/riscv/pgtable.h>
#include <arch/riscv/pfn.h>
#include <debug.h>

//
// an offset from physical to virtual addresses for direct mapped area
//
unsigned long va_pa_offset;

//
// a page frame number for the KERNEL_BASE virtual address
//
unsigned long pfn_base;

//
// a page frame number for the KERNEL_BASE+mem_size virtual address
//
unsigned long max_low_pfn;

//
// a page frame number for the KERNEL_BASE virtual address
//
unsigned long min_low_pfn;

//
// a maximum number of pfns
//
unsigned long max_mapnr;

//
// the kernel pgd
//
pgd_t* kernel_init_pgd;

pgd_t* get_current_sptbr_va(void)
{
    return pfn_to_virt(csr_read(sptbr));
}


