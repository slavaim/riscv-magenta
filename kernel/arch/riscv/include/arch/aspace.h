// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2015-2016 Travis Geiselbrecht
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <magenta/compiler.h>
#include <list.h>

__BEGIN_CDECLS

#define ARCH_ASPACE_MAGIC 0x41524153 // ARAS

struct arch_aspace {
    /* magic value for use-after-free detection */
    uint32_t magic;

    uint16_t asid;

    /* pointer to the translation table */
    paddr_t tt_phys;

    uint flags;

    /* range of address space */
    vaddr_t base;
    size_t size;
};

__END_CDECLS

