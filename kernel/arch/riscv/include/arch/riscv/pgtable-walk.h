// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <magenta/compiler.h>
#include <arch/riscv/pgtable.h>

__BEGIN_CDECLS

static inline paddr_t follow_huge_pmd_to_phys(pmd_t pmd, vaddr_t vaddr)
{
    assert(pmd_present(pmd));
	assert(pmd_huge(pmd));
	return pmd_to_phys(pmd) + ((vaddr & ~PMD_MASK));
}

static inline paddr_t follow_pte_to_phys(pte_t pte, vaddr_t vaddr)
{
    assert(pte_present(pte));
	return pte_to_phys(pte) + ((vaddr & ~PTE_MASK));
}

__END_CDECLS