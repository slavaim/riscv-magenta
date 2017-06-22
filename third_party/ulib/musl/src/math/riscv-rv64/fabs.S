#ifndef __riscv_float_abi_soft

.global fabs
.type   fabs, %function
fabs:
        fabs.d fa0, fa0
        ret

#endif