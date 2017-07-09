// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <magenta/errors.h>
#include <arch/user_copy.h>
#include <kernel/thread.h>
#include <kernel/vm.h>

#include <debug.h>

extern status_t _riscv_copy_user(void *dst,
                                 const void *src,
                                 size_t len,
                                 void **fault_return);

/*
 * @brief Copy data from userspace into kernelspace
 *
 * This function validates that usermode has access to src before copying the
 * data.
 *
 * @param dst The destination buffer.
 * @param src The source buffer.
 * @param len The number of bytes to copy.
 *
 * @return NO_ERROR on success
 */
status_t arch_copy_from_user(void *dst, const void *src, size_t len)
{
    if (!is_user_address_range((vaddr_t)src, len)) {
        return MX_ERR_INVALID_ARGS;
    }

    thread_t *thr = get_current_thread();
    status_t status = _riscv_copy_user(dst,
                                       src,
                                       len,
                                       &thr->arch.data_fault_resume);
    return status;
}

/*
 * @brief Copy data from kernelspace into userspace
 *
 * This function validates that usermode has access to dst before copying the
 * data.
 *
 * @param dst The destination buffer.
 * @param src The source buffer.
 * @param len The number of bytes to copy.
 *
 * @return NO_ERROR on success
 */
status_t arch_copy_to_user(void *dst, const void *src, size_t len)
{
    if (!is_user_address_range((vaddr_t)dst, len)) {
        return MX_ERR_INVALID_ARGS;
    }

    thread_t *thr = get_current_thread();
    status_t status = _riscv_copy_user(dst,
                                       src,
                                       len,
                                       &thr->arch.data_fault_resume);
    return status;
}

