// Copyright 2017 Slava Imameev.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT
//

#pragma once

#include <arch/riscv/asm/constant.h>
#include <arch/riscv/asm/page.h>
#include <arch/riscv/asm/pgtable.h>

#if __riscv_xlen == 64

    #define VA_SHIFT (VA_BITS-1)

    #define KERNEL_VA_BITS_MASK (_AC(-1,UL) << VA_SHIFT)    /* [63:38] bits are set */

    #define KERNEL_ASPACE_BASE  KERNEL_VA_BITS_MASK   /* 0xffffffC000000000UL # -256GB */
    #define KERNEL_ASPACE_SIZE  (_AC(1,UL) << VA_SHIFT)   /* 0x0000004000000000UL # 256GB */

    #define USER_ASPACE_BASE    0x0000000001000000UL /* 16MB */

    /* We set the top of user address space to be (1 << 38) - 4k.  See
       docs/magenta/sysret_problem.md for why we subtract 4k here.
       Subtracting USER_ASPACE_BASE from that value gives the value for
       USER_ASPACE_SIZE below */
    #define USER_ASPACE_SIZE ((_AC(1,UL) << VA_SHIFT) - 0x1000)

    //
    // the membase is supposed to be a DRAM base but it is not known until
    // boot, the real base is retrieved by a call to SBI
    //
    #define MEMBASE   (_AC(0,UL))

#else
    # error "32 bit RISC-V is not supported"
#endif