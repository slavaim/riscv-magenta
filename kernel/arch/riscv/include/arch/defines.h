// Copyright (c) 2017 Slava Imameev
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

//
// the file is used for assembler code so it should
// include only arch/riscv/asm files outside
// 
//

#include <arch/riscv/asm/defines.h>
#include <arch/riscv/asm/page.h>
#include <arch/riscv/asm/va-space-layout.h>

//
// the Linux kernel uses __ASSEMBLY__ which had been introduced
// before they knew about GCC __ASSEMBLER__ definition, Magenta
// sometimes use ASSEMBLY definitions, to print out GCC assembler
// definitions run the command
//
// $ echo > empty.S; gcc -dM -E empty.S
//

#if !defined(__ASSEMBLER__) && !defined(ASSEMBLY) && !defined(__ASSEMBLY__)
// C and C++ definitions go here
#endif
