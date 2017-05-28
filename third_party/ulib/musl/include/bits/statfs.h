#if defined(__x86_64__)
#include "generic/statfs.h"
#elif defined(__aarch64__)
#include "generic/statfs.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "generic/statfs.h"
#else
#error Unsupported architecture!
#endif
