#if defined(__x86_64__)
#include "x86_64/fcntl.h"
#elif defined(__aarch64__)
#include "aarch64/fcntl.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "riscv64/fcntl.h"
#else
#error Unsupported architecture!
#endif
