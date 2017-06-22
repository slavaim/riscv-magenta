#ifndef __riscv_float_abi_soft

.global sqrt
.type   sqrt, %function
sqrt:
        fsqrt.d fa0, fa0
        ret

#endif