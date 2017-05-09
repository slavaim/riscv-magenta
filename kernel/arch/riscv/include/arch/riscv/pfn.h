/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <stdint.h>
#include <magenta/compiler.h>
#include <arch/riscv/asm/constant.h>
#include <arch/riscv/page.h>

__BEGIN_CDECLS

#ifdef CONFIG_64BIT
    typedef uint64_t phys_addr_t;
#else
    typedef uint32_t phys_addr_t;
#endif

#define PFN_ALIGN(x)	(((unsigned long)(x) + (PAGE_SIZE - 1)) & PAGE_MASK)
#define PFN_UP(x)	(((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
#define PFN_DOWN(x)	((x) >> PAGE_SHIFT)
#define PFN_PHYS(x)	((phys_addr_t)(x) << PAGE_SHIFT)
#define PHYS_PFN(x)	((unsigned long)((x) >> PAGE_SHIFT))

__END_CDECLS
