// Copyright 2017 The Fuchsia Authors
// Copyright (c) 2014 Travis Geiselbrecht
// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <stdio.h>
#include <debug.h>
#include <bits.h>
#include <inttypes.h>
#include <trace.h>
#include <kernel/thread.h>
#include <platform.h>
#include <arch/riscv/asm/asm-offsets.h>

#if WITH_LIB_MAGENTA
#include <lib/user_copy.h>
#include <magenta/exception.h>
#endif

bool riscv_in_int_handler[SMP_MAX_CPUS];