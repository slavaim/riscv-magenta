/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <magenta/compiler.h>
#include <arch/riscv/asm/linkage.h>

__BEGIN_CDECLS

void trap_init(void);

__END_CDECLS