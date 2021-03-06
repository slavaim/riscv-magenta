#if defined(__x86_64__)
#include "generic/sem.h"
#elif defined(__aarch64__)
#include "aarch64/sem.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "generic/sem.h"
#else
#error Unsupported architecture!
#endif
