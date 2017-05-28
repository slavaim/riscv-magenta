#if defined(__x86_64__)
#include "generic/resource.h"
#elif defined(__aarch64__)
#include "generic/resource.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "generic/resource.h"
#else
#error Unsupported architecture!
#endif
