#if defined(__x86_64__)
#include "generic/termios.h"
#elif defined(__aarch64__)
#include "generic/termios.h"
#elif defined(__riscv) && __riscv_xlen == 64
#include "generic/termios.h"
#else
#error Unsupported architecture!
#endif
