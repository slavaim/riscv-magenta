#ifndef __riscv_float_abi_soft

.global sqrtf
.type   sqrtf, %function
sqrtf:
        fsqrt.s fa0, fa0
        ret

#endif