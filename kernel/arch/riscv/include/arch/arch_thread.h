// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2015-2016 Travis Geiselbrecht
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#ifndef ASSEMBLY

__BEGIN_CDECLS

struct arch_thread {
    vaddr_t sp;

    /* if non-NULL, address to return to on data fault */
    void *data_fault_resume;
};

__END_CDECLS

#endif // ASSEMBLY