//Copyright 2017 Slava Imameev.
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT
//

#pragma once

#include <arch/riscv/asm/page.h>

#if __riscv_xlen == 64

    # define NM_PAGE_TABLES (3)
	# define PGLEVEL_BITS (9)
	# define PMD_PAGE_SIZE ((uintptr_t)(PAGE_SIZE << PGLEVEL_BITS))
	# define PGD_PAGE_SIZE (PMD_PAGE_SIZE << PGLEVEL_BITS)
	# define VA_SIZE_RANGE (PGD_PAGE_SIZE << PGLEVEL_BITS)

	# define VA_BITS (NM_PAGE_TABLES*PGLEVEL_BITS + PAGE_SHIFT) /* 39 bits */
	# define IS_UPPER_VA(va) (0x0 != ((va) & 1UL<<(VA_BITS-1)))
	# define UPPER_VA_BITS_MASK ((~0UL)<<(VA_BITS-1))    /* [63:38] bits are set */

    # define KERNEL_ASPACE_BASE  UPPER_VA_BITS_MASK   /* 0xffffffC000000000UL # -256GB */
    # define KERNEL_ASPACE_SIZE  (1UL<<(VA_BITS-1))   /* 0x0000004000000000UL # 256GB */


    # define USER_ASPACE_BASE    0x0000000001000000UL /* 16MB */

    /* We set the top of user address space to be (1 << 38) - 4k.  See
       docs/magenta/sysret_problem.md for why we subtract 4k here.
       Subtracting USER_ASPACE_BASE from that value gives the value for
       USER_ASPACE_SIZE below */
    # define USER_ASPACE_SIZE ((1UL<<(VA_BITS-1)) - 0x1000)

#else
	# error "32 bit RISC-V is not supported"
#endif