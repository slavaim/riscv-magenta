#if defined(__x86_64__)
#include "x86_64/mman.h"
#elif defined(__aarch64__)
#include "generic/mman.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "riscv64/mman.h"
#else
#error Unsupported architecture!
#endif
