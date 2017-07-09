// Copyright 2016 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <asm.h>
#include <err.h>
#include <arch/riscv/asm/asm.h>

# status_t _riscv_copy_user(void *dst, const void *src, size_t len, void **fault_return)
# $a0 - dst, a user address if copied to user, else kernel address
# $a1 - src, a user address if copied from user, else kernel address
# $a2 - len
# $a3 - fault_return
FUNCTION(_riscv_copy_user)
    # Setup data fault return
    la      t0, .Lfault_to_user
    REG_S   t0, 0(a3)

    # exit if len is zero, an unlikely case
    beqz    a2, .Lcleanup_to_user

    # Perform the memcpy, the strange instructions arrangemens
    # is to reduce pipeline stalls because of data dependencies
    # by putting dependent isntructions as far apart as possible
.Lcopy_byte_to_user:
    lb      t0, 0(a1)
    addi    a0, a0, 1   # calculate the next dst byte address
    addi    a2, a2, -1  # decrement bytes left to copy
    addi    a1, a1, 1   # calculate the next src byte address
    sb      t0, -1(a0)  # copy to dst, (-1) as a0 has been incremented
    bnez    a2, .Lcopy_byte_to_user
0:
    li      a0, MX_OK
    j       .Lcleanup_to_user
.Lfault_to_user:
    li      a0, MX_ERR_INVALID_ARGS
.Lcleanup_to_user:
    # Reset data fault return
    REG_S   x0, 0(a3)
    ret
END_FUNCTION(_riscv_copy_user)