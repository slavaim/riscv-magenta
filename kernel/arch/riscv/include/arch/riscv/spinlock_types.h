/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

typedef volatile unsigned int arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	0

typedef volatile int arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED		0