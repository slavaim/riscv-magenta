#if defined(__x86_64__)
#include "x86_64/socket.h"
#elif defined(__aarch64__)
#include "aarch64/socket.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "riscv64/socket.h"
#else
#error Unsupported architecture!
#endif
