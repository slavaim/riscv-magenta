#ifndef __riscv_float_abi_soft

.global fmax
.type   fmax, %function
fmax:
        fmax.d fa0, fa0, fa1
        ret

#endif