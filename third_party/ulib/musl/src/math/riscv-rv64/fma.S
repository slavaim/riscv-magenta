#ifndef __riscv_float_abi_soft

.global fma
.type   fma, %function
fma:
        fmadd.d fa0, fa0, fa1, fa2
        ret

#endif