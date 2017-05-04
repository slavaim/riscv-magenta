/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#include <arch/riscv/spinlock_types.h>
#include <arch/riscv/processor.h>

__BEGIN_CDECLS

/*
 * Simple spin lock operations.  These provide no fairness guarantees.
 * Use the ‘&’ constraint modifier (see Modifiers) on all output operands
 * that must not overlap an input. Otherwise, GCC may allocate the output
 * operand in the same register as an unrelated input operand, on the
 * assumption that the assembler code consumes its inputs before producing
 * outputs. This assumption may be false if the assembler code actually
 * consists of more than one instruction.
 */

#define arch_spin_lock_flags_raw(lock, flags) arch_spin_lock(lock)
#define arch_spin_is_locked_raw(x)	(*(x) != 0)
#define arch_spin_unlock_wait_raw(x) \
		do { cpu_relax(); } while (*(x))

static inline void arch_spin_unlock_raw(arch_spinlock_t *lock)
{
	__asm__ __volatile__ (
		"amoswap.w.rl x0, x0, %0"
		: "+A" (*lock)
		:: "memory");
}

static inline int arch_spin_trylock_raw(arch_spinlock_t *lock)
{
	int tmp = 1, busy;

	__asm__ __volatile__ (
		"amoswap.w.aq %0, %2, %1"
		: "=&r" (busy), "+A" (*lock)
		: "r" (tmp)
		: "memory");

	return !busy;
}

static inline void arch_spin_lock_raw(arch_spinlock_t *lock)
{
	while (1) {
		if (arch_spin_is_locked_raw(lock))
			continue;

		if (arch_spin_trylock_raw(lock))
			break;
	}
}

/***********************************************************/

static inline int arch_read_can_lock(arch_rwlock_t *lock)
{
	return *lock >= 0;
}

static inline int arch_write_can_lock(arch_rwlock_t *lock)
{
	return *lock == 0;
}

static inline void arch_read_lock(arch_rwlock_t *lock)
{
	int tmp;

	__asm__ __volatile__(
		"1:	lr.w	%1, 0(%0)\n"
		"	bltz	%1, 1b\n"
		"	addi	%1, %1, 1\n"
		"	sc.w.aq	%1, %1, 0(%0)\n"
		"	bnez	%1, 1b\n"
		: "+&r" (lock), "=&r" (tmp)
		:: "memory");
}

static inline void arch_write_lock(arch_rwlock_t *lock)
{
	int tmp;

	__asm__ __volatile__(
		"1:	lr.w	%1, 0(%0)\n"
		"	bnez	%1, 1b\n"
		"	li	%1, -1\n"
		"	sc.w.aq	%1, %1, 0(%0)\n"
		"	bnez	%1, 1b\n"
		: "+&r" (lock), "=&r" (tmp)
		:: "memory");
}

static inline int arch_read_trylock(arch_rwlock_t * lock)
{
	int busy;

	__asm__ __volatile__(
		"1:	lr.w	%1, 0(%0)\n"
		"	bltz	%1, 1f\n"
		"	addi	%1, %1, 1\n"
		"	sc.w.aq	%1, %1, 0(%0)\n"
		"	bnez	%1, 1b\n"
		"1:\n"
		: "+&r" (lock), "=&r" (busy)
		:: "memory");
	
	return !busy;
}

static inline int arch_write_trylock(arch_rwlock_t * lock)
{
	int busy;

	__asm__ __volatile__(
		"1:	lr.w	%1, 0(%0)\n"
		"	bnez	%1, 1f\n"
		"	li	%1, -1\n"
		"	sc.w.aq	%1, %1, 0(%0)\n"
		"	bnez	%1, 1b\n"
		"1:\n"
		: "+&r" (lock), "=&r" (busy)
		:: "memory");
	
	return !busy;
}

static inline void arch_read_unlock(arch_rwlock_t * lock)
{
	__asm__ __volatile__(
		"amoadd.w.rl x0, %1, 0(%0)"
		: "+&r" (lock)
		: "r" (-1)
		: "memory");
}

static inline void arch_write_unlock(arch_rwlock_t * lock)
{
	__asm__ __volatile__ (
		"amoswap.w.rl x0, x0, 0(%0)"
		: "=&r" (lock)
		:: "memory");
}

#define arch_read_lock_flags(lock, flags) arch_read_lock(lock)
#define arch_write_lock_flags(lock, flags) arch_write_lock(lock)

__END_CDECLS

