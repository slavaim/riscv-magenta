# RISC-V port for Magenta kernel

This is an initiative to port Magenta kernel to RISC-V architecture. The canonical Google's Magenta kernel supports x86-64 and arm64 architectures only.

This repository will be periodically merged with the canonical Magenta repository to keep it up-to-date.  

Milestones:
  - 20 May 2017 the kernel is running in QEMU and switching between kernel threads
  - 02 June 2017 some syscall facilities are implemented
  - 14 June 2017 some pagefault facilities are implemented
  - 21 June 2017 user mode entry
  - 23 June 2017 libc (musl) was ported to RISC-V
  
Below is a RISC-V kernel log output on load in QEMU
```
[1266874889.709] 00000.00000> Available physical memory: 2032dMB
[1266874889.709] 00000.00000> 
welcome to lk/MP

[1266874889.709] 00000.00000> INIT: cpu 0, calling hook 0xffffffff8006e2d4 (global_prng_seed) at level 0x30000, flags 0x1
[1266874889.709] 00000.00000> WARNING: System has insufficient randomness.  It is completely unsafe to use this system for any cryptographic applications.
[1266874889.709] 00000.00000> INIT: cpu 0, calling hook 0xffffffff800386b0 (elf_build_id) at level 0x3fffe, flags 0x1
[1266874889.709] 00000.00000> INIT: cpu 0, calling hook 0xffffffff800385f0 (version) at level 0x3ffff, flags 0x1
[1266874889.709] 00000.00000> version:
[1266874889.709] 00000.00000> 	arch:     RISCV
[1266874889.709] 00000.00000> 	platform: RISCV_RV64
[1266874889.709] 00000.00000> 	target:   QEMU_RISCV_RV64
[1266874889.709] 00000.00000> 	project:  MAGENTA_QEMU_RISCV_RV64
[1266874889.709] 00000.00000> 	buildid:  GIT_0FBB50D3A8FC1D242E1D7A2921674579C9192D66_DIRTY_LOCAL
[1266874889.709] 00000.00000> 	ELF build ID: be2909891fc4cce74084f5fa3f14d6e98903d3bb
[1266874889.709] 00000.00000> INIT: cpu 0, calling hook 0xffffffff80042f0c (vm_preheap) at level 0x3ffff, flags 0x1
[1266874889.709] 00000.00000> initializing heap
[1266874889.709] 00000.00000> INIT: cpu 0, calling hook 0xffffffff8004305c (vm) at level 0x50000, flags 0x1
[1266874889.709] 00000.00000> VM: reserving kernel region [ffffffff80000000, ffffffff800f7000) flags 0x28 name 'kernel_code'
[1266874889.709] 00000.00000> VM: reserving kernel region [ffffffff800f7000, ffffffff80139000) flags 0x8 name 'kernel_rodata'
[1266874889.709] 00000.00000> VM: reserving kernel region [ffffffff80139000, ffffffff8013c000) flags 0x18 name 'kernel_data'
[1266874889.709] 00000.00000> VM: reserving kernel region [ffffffff8013c000, ffffffff80166000) flags 0x18 name 'kernel_bss'
[1266874889.709] 00000.00000> VM: reserving kernel region [ffffffff8016a000, ffffffff8114b000) flags 0x18 name 'kernel_bootalloc'
[1266874889.709] 00000.00000> INIT: cpu 0, calling hook 0xffffffff8000197c (timer) at level 0x50003, flags 0x1
[00003.966] 00000.00000> initializing mp
[00003.966] 00000.00000> initializing threads
[00003.967] 00000.00000> initializing timers
[00003.967] 00000.00000> INIT: cpu 0, calling hook 0xffffffff800132d4 (debuglog) at level 0x6ffff, flags 0x1
[00003.970] 00000.00000> INIT: cpu 0, calling hook 0xffffffff8006e448 (global_prng_thread_safe) at level 0x6ffff, flags 0x1
[00003.971] 00000.00000> creating bootstrap completion thread
[00004.057] 00000.00000> top of bootstrap2()
[00004.058] 00000.00000> INIT: cpu 0, calling hook 0xffffffff80070c9c (dpc) at level 0x70000, flags 0x1
[00004.064] 00000.00000> INIT: cpu 0, calling hook 0xffffffff8009158c (magenta) at level 0x70000, flags 0x1
[00004.078] 00000.00000> initializing platform
[00004.078] 00000.00000> initializing target
[00004.078] 00000.00000> calling apps_init()
[00004.078] 00000.00000> INIT: cpu 0, calling hook 0xffffffff80014464 (ktrace) at level 0xaffff, flags 0x1
[00005.091] 00000.00000> ktrace: buffer at 0xffffffc000e01000 (33554432 bytes)
[00005.094] 00000.00000> INIT: cpu 0, calling hook 0xffffffff800384bc (userboot) at level 0xaffff, flags 0x1
[00005.161] 00000.00000> userboot: userboot rodata       0 @ [0x292f043000,0x292f045000)
[00005.163] 00000.00000> userboot: userboot code    0x2000 @ [0x292f045000,0x292f05a000)
[00005.165] 00000.00000> userboot: vdso/full rodata       0 @ [0x292f05a000,0x292f05f000)
[00005.167] 00000.00000> userboot: vdso/full code    0x5000 @ [0x292f05f000,0x292f061000)
[00005.193] 00000.00000> userboot: entry point             @ 0x292f046da0
[00005.293] 00000.00000> starting app shell
] [00005.296] 00000.00000> entering main console loop
```
An example of a kernel console seeion

```
help
] [00013.062] 00000.00000> command list:
[00013.063] 00000.00000> 	ut              : Run unittests
[00013.064] 00000.00000> 	rng32           : Generate and print a random 32 bit unsigned integer using the HW RNG
[00013.065] 00000.00000> 	rng             : Generate and print N random bytes using the HW RNG
[00013.066] 00000.00000> 	mx              : magenta diagnostics
[00013.067] 00000.00000> 	heap            : heap debug commands
[00013.068] 00000.00000> 	gfx             : gfx commands
[00013.069] 00000.00000> 	help            : this list
[00013.071] 00000.00000> 	test            : test the command processor
[00013.072] 00000.00000> 	history         : command history
[00013.073] 00000.00000> 	vmm             : virtual memory manager
[00013.074] 00000.00000> 	vm_object       : vm object debug commands
[00013.075] 00000.00000> 	vm              : vm commands
[00013.076] 00000.00000> 	pmm             : physical memory manager
[00013.077] 00000.00000> 	vm_page         : vm_page debug commands
[00013.078] 00000.00000> 	mem_test        : test memory
[00013.079] 00000.00000> 	printf_tests    : test printf
[00013.079] 00000.00000> 	thread_tests    : test the scheduler
[00013.080] 00000.00000> 	clock_tests     : test clocks
[00013.081] 00000.00000> 	sleep_tests     : tests sleep
[00013.081] 00000.00000> 	bench           : miscellaneous benchmarks
[00013.082] 00000.00000> 	fibo            : threaded fibonacci
[00013.083] 00000.00000> 	spinner         : create a spinning thread
[00013.084] 00000.00000> 	sync_ipi_tests  : test synchronous IPIs
[00013.085] 00000.00000> 	timer_tests     : tests timers
[00013.085] 00000.00000> 	cache_tests     : test/bench the cpu cache
[00013.086] 00000.00000> 	reboot          : soft reset
[00013.087] 00000.00000> 	poweroff        : powerdown
[00013.088] 00000.00000> 	version         : print version
[00013.089] 00000.00000> 	thread          : list kernel threads with options
[00013.090] 00000.00000> 	threadstats     : thread level statistics
[00013.091] 00000.00000> 	threadload      : toggle thread load display
[00013.092] 00000.00000> 	kill            : kill a thread
vmm aspaces
[00027.013] 00000.00000> as 0xffffffff80160e10 [0xffffffc000000000 0xffffffffffffffff] sz 0x4000000000 fl 0x1 ref 17 'kernel'
[00027.020] 00000.00000>   vmar 0xffffffff80161040 [0xffffffc000000000 0xffffffffffffffff] sz 0x4000000000 ref 1 'root'
[00027.022] 00000.00000>     vmar 0xffffffff81156a30 [0xffffffc000000000 0xffffffc000e00fff] sz 0xe01000 ref 2 'arena:handles'
[00027.023] 00000.00000>       map 0xffffffff81156af0 [0xffffffc000000000 0xffffffc0003fffff] sz 0x400000 mmufl 0x18 vmo 0xffffffff81156920/k0 off 0 pages 0 ref 2 'arena:handles'
[00027.028] 00000.00000>         vmo 0xffffffff81156920/k0 size 0xe00000 pages 4 ref 2 parent k0
[00027.030] 00000.00000>       map 0xffffffff81156b90 [0xffffffc000401000 0xffffffc000e00fff] sz 0xa00000 mmufl 0x18 vmo 0xffffffff81156920/k0 off 0x400000 pages 4 ref 2 'arena:handles'
[00027.055] 00000.00000>         vmo 0xffffffff81156920/k0 size 0xe00000 pages 4 ref 2 parent k0
[00027.057] 00000.00000>     map 0xffffffff8116ee90 [0xffffffc000e01000 0xffffffc002e00fff] sz 0x2000000 mmufl 0x18 vmo 0xffffffff81156d80/k0 off 0 pages 8192 ref 1 ''
[00027.074] 00000.00000>       vmo 0xffffffff81156d80/k0 size 0x2000000 pages 8192 ref 1 parent k0
[00027.075] 00000.00000>     vmar 0xffffffff811711b0 [0xffffffc002e01000 0xffffffc002e04fff] sz 0x4000 ref 2 'kstack_vmar'
[00027.076] 00000.00000>       map 0xffffffff81171270 [0xffffffc002e02000 0xffffffc002e03fff] sz 0x2000 mmufl 0x18 vmo 0xffffffff811710a0/k0 off 0 pages 2 ref 2 ''
[00027.078] 00000.00000>         vmo 0xffffffff811710a0/k0 size 0x2000 pages 2 ref 1 parent k0
[00027.079] 00000.00000>     map 0xffffffff8114c130 [0xffffffff80000000 0xffffffff800f6fff] sz 0xf7000 mmufl 0x28 vmo 0xffffffff8114c020/k0 off 0 pages 0 ref 1 ''
[00027.081] 00000.00000>       vmo 0xffffffff8114c020/k0 size 0 pages 0 ref 1 parent k0
[00027.083] 00000.00000>     map 0xffffffff8114c2e0 [0xffffffff800f7000 0xffffffff80138fff] sz 0x42000 mmufl 0x8 vmo 0xffffffff8114c1d0/k0 off 0 pages 0 ref 1 ''
[00027.084] 00000.00000>       vmo 0xffffffff8114c1d0/k0 size 0 pages 0 ref 1 parent k0
[00027.085] 00000.00000>     map 0xffffffff8114c490 [0xffffffff80139000 0xffffffff8013bfff] sz 0x3000 mmufl 0x18 vmo 0xffffffff8114c380/k0 off 0 pages 0 ref 1 ''
[00027.087] 00000.00000>       vmo 0xffffffff8114c380/k0 size 0 pages 0 ref 1 parent k0
[00027.088] 00000.00000>     map 0xffffffff8114c640 [0xffffffff8013c000 0xffffffff80165fff] ] sz 0x2a000 mmufl 0x18 vmo 0xffffffff8114c530/k0 off 0 pages 0 ref 1 ''
[00027.090] 00000.00000>       vmo 0xffffffff8114c530/k0 size 0 pages 0 ref 1 parent k0
[00027.091] 00000.00000>     map 0xffffffff8114c9a0 [0xffffffff80166000 0xffffffff80169fff] sz 0x4000 mmufl 0x18 vmo 0xffffffff8114c890/k0 off 0 pages 0 ref 1 ''
[00027.093] 00000.00000>       vmo 0xffffffff8114c890/k0 size 0 pages 0 ref 1 parent k0
[00027.094] 00000.00000>     map 0xffffffff8114c7f0 [0xffffffff8016a000 0xffffffff8114afff] sz 0xfe1000 mmufl 0x18 vmo 0xffffffff8114c6e0/k0 off 0 pages 0 ref 1 ''
[00027.096] 00000.00000>       vmo 0xffffffff8114c6e0/k0 size 0 pages 0 ref 1 parent k0
[00027.097] 00000.00000>     map 0xffffffff8114cb50 [0xffffffff8114b000 0xfffffffffeffffff] sz 0x7deb5000 mmufl 0x18 vmo 0xffffffff8114ca40/k0 off 0 pages 0 ref 1 ''
[00027.099] 00000.00000>       vmo 0xffffffff8114ca40/k0 size 0 pages 0 ref 1 parent k0
[00027.100] 00000.00000>     map 0xffffffff8114cd00 [0xffffffffffff5000 0xffffffffffffffff] sz 0xb000 mmufl 0x18 vmo 0xffffffff8114cbf0/k0 off 0 pages 0 ref 1 ''
[00027.102] 00000.00000>       vmo 0xffffffff8114cbf0/k0 size 0 pages 0 ref 1 parent k0
[00027.166] 00000.00000> as 0xffffffff8116f970 [0x1000000 0x3fffffefff] sz 0x3ffefff000 fl 0 ref 8 'proc:1029'
[00027.167] 00000.00000>   vmar 0xffffffff8116fb20 [0x1000000 0x3fffffefff] sz 0x3ffefff000 ref 2 'root'
[00027.168] 00000.00000>     map 0xffffffff81170a70 [0x1cc2f46000 0x1cc2f85fff] sz 0x40000 mmufl 0x1c vmo 0xffffffff8116ef30/k1026 off 0 pages 1 ref 1 'userboot-initial-stack'
[00027.170] 00000.00000>       vmo 0xffffffff8116ef30/k1026 size 0x40000 pages 1 ref 2 parent k0
[00027.171] 00000.00000>     vmar 0xffffffff811706f8 [0x292f043000 0x292f060fff] sz 0x1e000 ref 1 'useralloc'
[00027.172] 00000.00000>       map 0xffffffff811707f0 [0x292f043000 0x292f044fff] sz 0x2000 mmufl 0xc vmo 0xffffffff811702f0/k1034 off 0 pages 2 ref 1 'userboot'
[00027.174] 00000.00000>         vmo 0xffffffff811702f0/k1034 size 0x17000 pages 23 ref 3 parent k0
[00027.175] 00000.00000>       map 0xffffffff81170890 [0x292f045000 0x292f059fff] sz 0x15000 mmufl 0x2c vmo 0xffffffff811702f0/k1034 off 0x2000 pages 21 ref 1 'userboot'
[00027.177] 00000.00000>         vmo 0xffffffff811702f0/k1034 size 0x17000 pages 23 ref 3 parent k0
[00027.178] 00000.00000>       map 0xffffffff81170930 [0x292f05a000 0x292f05efff] sz 0x5000 mmufl 0xc vmo 0xffffffff8116fc60/k1031 off 0 pages 5 ref 1 'vdso/full'
[00027.180] 00000.00000>         vmo 0xffffffff8116fc60/k1031 size 0x7000 pages 7 ref 6 parent k0
[00027.181] 00000.00000>       map 0xffffffff811709d0 [0x292f05f000 0x292f060fff] sz 0x2000 mmufl 0x2c vmo 0xffffffff8116fc60/k1031 off 0x5000 pages 2 ref 2 'vdso/full'
[00027.183] 00000.00000>         vmo 0xffffffff8116fc60/k1031 size 0x7000 pages 7 ref 6 parent k0
```
  
Some notes on Magents RISC-V porting can be found here https://github.com/slavaim/riscv-notes/tree/master/magenta 

Below is the original Magenta's README.md  

# Magenta

Magenta is the core platform that powers the Fuchsia OS.  Magenta is
composed of a microkernel (source in kernel/...) as well as a small
set of userspace services, drivers, and libraries (source in system/...)
necessary for the system to boot, talk to hardware, load userspace
processes and run them, etc.  Fuchsia builds a much larger OS on top
of this foundation.

The canonical Magenta Git repository is located
at: https://fuchsia.googlesource.com/magenta

A read-only mirror of the code is present
at: https://github.com/fuchsia-mirror/magenta

The Magenta Kernel provides syscalls to manage processes, threads,
virtual memory, inter-process communication, waiting on object state
changes, and locking (via futexes).

Currently there are some temporary syscalls that have been used for early
bringup work, which will be going away in the future as the long term
syscall API/ABI surface is finalized.  The expectation is that there will
be 10s, not 100s of syscalls.

Magenta syscalls are generally non-blocking.  The wait (one, many, set)
family of syscalls, ioport reads, and thread sleep being the notable
exceptions.

This page is a non-comprehensive index of the magenta documentation.

+ [Getting Started](docs/getting_started.md)
+ [Contributing Patches](docs/contributing.md)
+ [Testing](docs/testing.md)
+ [Hacking notes](docs/hacking.md)
+ [Memory usage analysis tools](docs/memory.md)
+ [Relationship with LK](docs/mg_and_lk.md)
+ [Kernel Objects](docs/kernel_objects.md)
+ [Process Objects](docs/objects/process.md)
+ [Thread Objects](docs/objects/thread.md)
+ [Handles](docs/handles.md)
+ [System Calls](docs/syscalls.md)
