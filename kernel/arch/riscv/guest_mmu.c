// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT


#include <arch/guest_mmu.h>

/* initialize per address space */
status_t guest_mmu_init_paspace(guest_paspace_t* paspace, size_t size) __NONNULL((1))
{
    return ERR_NOT_SUPPORTED;
}

status_t guest_mmu_destroy_paspace(guest_paspace_t* paspace) __NONNULL((1))
{
    return ERR_NOT_SUPPORTED;
}

/* routines to map/unmap/update permissions/query mappings per address space */
status_t guest_mmu_map(guest_paspace_t* paspace, vaddr_t vaddr, paddr_t paddr, size_t count, uint mmu_flags, size_t* mapped) __NONNULL((1))
{
    return ERR_NOT_SUPPORTED;
}

status_t guest_mmu_unmap(guest_paspace_t* paspace, vaddr_t vaddr, size_t count, size_t* unmapped) __NONNULL((1))
{
    return ERR_NOT_SUPPORTED;
}

status_t guest_mmu_protect(guest_paspace_t* paspace, vaddr_t vaddr, size_t count, uint mmu_flags) __NONNULL((1))
{
    return ERR_NOT_SUPPORTED;
}

status_t guest_mmu_query(guest_paspace_t* paspace, vaddr_t vaddr, paddr_t* paddr, uint* mmu_flags) __NONNULL((1))
{
    return ERR_NOT_SUPPORTED;
}
