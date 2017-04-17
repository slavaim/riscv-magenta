// Copyright 2017 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <arch/riscv/asm/constant.h>

#define PAGE_SHIFT	(12)
#define PAGE_SIZE_SHIFT PAGE_SHIFT
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE - 1))