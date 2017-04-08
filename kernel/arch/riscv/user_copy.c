// Copyright 2016 The Fuchsia Authors
// Copyright 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include <magenta/errors.h>
#include <arch/user_copy.h>
#include <debug.h>

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
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
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
    PANIC_UNIMPLEMENTED;
    return ERR_NOT_SUPPORTED;
}

