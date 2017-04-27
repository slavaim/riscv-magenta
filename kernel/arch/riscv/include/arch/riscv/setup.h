// Copyright 2017 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Some code has been borrowed from the Linux kernel which is under GPLv2 license.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <stdint.h>

__BEGIN_CDECLS

int setup_memory_info(void);
void setup_kernel_init_pgd(void);

__END_CDECLS