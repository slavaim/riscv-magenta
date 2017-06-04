/*
Some definitions have been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <magenta/compiler.h>
#include <sys/types.h>
#include <arch/riscv/asm/page.h>

__BEGIN_CDECLS

/*
 * RV32Sv32 page table entry:
 * | 31 10 | 9             7 | 6 | 5 | 4  1 | 0
 *    PFN    reserved for SW   D   R   TYPE   V
 *
 * RV64Sv39 / RV64Sv48 page table entry:
 * | 63           48 | 47 10 | 9             7 | 6 | 5 | 4  1 | 0
 *   reserved for HW    PFN    reserved for SW   D   R   TYPE   V
 */

#define _PAGE_PRESENT   (1 << 0)
#define _PAGE_READ      (1 << 1)    /* Readable */
#define _PAGE_WRITE     (1 << 2)    /* Writable */
#define _PAGE_EXEC      (1 << 3)    /* Executable */
#define _PAGE_USER      (1 << 4)    /* User */
#define _PAGE_GLOBAL    (1 << 5)    /* Global */
#define _PAGE_ACCESSED  (1 << 6)    /* Set by hardware on any access */
#define _PAGE_DIRTY     (1 << 7)    /* Set by hardware on any write */
#define _PAGE_SOFT      (1 << 8)    /* Reserved for software */

#define _PAGE_SPECIAL   _PAGE_SOFT
#define _PAGE_TABLE     _PAGE_PRESENT

#define _PAGE_PFN_SHIFT 10

/* Set of bits to preserve across pte_modify() */
#define _PAGE_CHG_MASK  (~(unsigned long)(_PAGE_PRESENT | _PAGE_READ |	\
					  _PAGE_WRITE | _PAGE_EXEC |	\
					  _PAGE_USER | _PAGE_GLOBAL))

/* Set of bits to preserve across page access modification */
#define _PAGE_CHG_ACCESS_MASK  (~(unsigned long)(_PAGE_READ |	\
					  _PAGE_WRITE | _PAGE_EXEC | _PAGE_USER ))

/* Advertise support for _PAGE_SPECIAL */
#define __HAVE_ARCH_PTE_SPECIAL

#if __riscv_xlen == 64

	static_assert(PAGE_SIZE == 4096, "Unsupported page size");

	# define VA_BITS 39
	# define IS_UPPER_VA(va) (0x0 != ((va) & 1UL<<(VA_BITS-1)))
	# define UPPER_VA_BITS (~((1UL<<(VA_BITS-1)) - 1))  /*[63-38] bits are set*/

	//
	// converts a VA_BITS wide VA to a canonical 64 bit VA
	//
	static inline vaddr_t get_canonical_va(vaddr_t va) {
		if (IS_UPPER_VA(va))
			va = va | UPPER_VA_BITS;
		return va;
	}

	# define PGLEVEL_BITS 9
	# define PMD_PAGE_SIZE ((uintptr_t)(PAGE_SIZE << PGLEVEL_BITS))
	# define PGD_PAGE_SIZE (PMD_PAGE_SIZE << PGLEVEL_BITS)
	# define VA_SIZE_RANGE (PGD_PAGE_SIZE << PGLEVEL_BITS)

#else
	# error "32 bit RISC-V is not supported"
#endif

__END_CDECLS

