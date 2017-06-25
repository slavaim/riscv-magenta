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
