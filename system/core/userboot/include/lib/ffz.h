/*
The code has been borrowed from the Linux kernel which is under GPLv2 license.
*/
#ifndef _ASM_GENERIC_BITOPS_FFZ_H_
#define _ASM_GENERIC_BITOPS_FFZ_H_

#include <magenta/compiler.h>
#include <lib/lib.h>

/*
 * ffz - find first zero in word.
 * @word: The word to search
 *
 * Undefined if no zero exists, so code should check against ~0UL first.
 */
#define ffz(x)  __ffs(~(x))

#endif /* _ASM_GENERIC_BITOPS_FFZ_H_ */
