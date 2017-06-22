#ifndef __riscv_float_abi_soft

.global fminf
.type   fminf, %function
fminf:
        fmin.s fa0, fa0, fa1
        ret

#endif