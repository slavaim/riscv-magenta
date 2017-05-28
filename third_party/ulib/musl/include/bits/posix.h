#if defined(__x86_64__)
#include "x86_64/posix.h"
#elif defined(__aarch64__)
#include "aarch64/posix.h"
#elif defined(__riscv) && __riscv_xlen == 64

#else
#error Unsupported architecture!
#endif
