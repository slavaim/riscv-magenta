#if defined(__x86_64__)
#include "x86_64/endian.h"
#elif defined(__aarch64__)
#include "aarch64/endian.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "riscv64/endian.h"
#else
#error Unsupported architecture!
#endif
