#if defined(__x86_64__)
#include "x86_64/limits.h"
#elif defined(__aarch64__)
#include "aarch64/limits.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "riscv64/limits.h"
#else
#error Unsupported architecture!
#endif
