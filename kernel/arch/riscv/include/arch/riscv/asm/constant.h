/* const.h: Macros for dealing with constants.  */
/*2017 Modfied for Magenta by Slava Imameev*/

#pragma once

/* Some constant macros are used in both assembler and
 * C code.  Therefore we cannot annotate them always with
 * 'UL' and other type specifiers unilaterally.  We
 * use the following macros to deal with this.
 *
 * Similarly, _AT() will cast an expression with a type in C, but
 * leave it unchanged in asm.
 */

/*
the Linux kernel uses __ASSEMBLY__ which had been introduced before
they knew about GCC __ASSEMBLER__ definition, to print out
GCC assembler definitions run the command

$ echo > empty.S; gcc -dM -E empty.S

*/
#if defined(__ASSEMBLER__) || defined(ASSEMBLY)
#define __ASSEMBLY__ 1
#define _AC(X,Y)	X
#define _AT(T,X)	X
#else
#define __AC(X,Y)	(X##Y)
#define _AC(X,Y)	__AC(X,Y)
#define _AT(T,X)	((T)(X))
#endif

#define _BITUL(x)	(_AC(1,UL) << (x))
#define _BITULL(x)	(_AC(1,ULL) << (x))

#ifndef CONFIG_64BIT
    #error "only 64 bit RISCV is supported for the current code base"
#endif