#ifndef _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_
#define _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_

#include <magenta/compiler.h>
#include <lib/lib.h>

/*
 * Compile time versions of __arch_hweightN()
 */
#define __const_hweight8(w)		\
	((unsigned int)			\
	 ((!!((w) & (1ULL << 0))) +	\
	  (!!((w) & (1ULL << 1))) +	\
	  (!!((w) & (1ULL << 2))) +	\
	  (!!((w) & (1ULL << 3))) +	\
	  (!!((w) & (1ULL << 4))) +	\
	  (!!((w) & (1ULL << 5))) +	\
	  (!!((w) & (1ULL << 6))) +	\
	  (!!((w) & (1ULL << 7)))))

#define __const_hweight16(w) (__const_hweight8(w)  + __const_hweight8((w)  >> 8 ))
#define __const_hweight32(w) (__const_hweight16(w) + __const_hweight16((w) >> 16))
#define __const_hweight64(w) (__const_hweight32(w) + __const_hweight32((w) >> 32))

/*
 * Generic interface.
 */
#define hweight8(w)  __const_hweight8(w)
#define hweight16(w) __const_hweight16(w)
#define hweight32(w) __const_hweight32(w)
#define hweight64(w) __const_hweight64(w)

#endif /* _ASM_GENERIC_BITOPS_CONST_HWEIGHT_H_ */
