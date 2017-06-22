#ifndef __riscv_float_abi_soft

.global fabsf
.type   fabsf, %function
fabsf:
        fabs.s fa0, fa0
        ret

#endif