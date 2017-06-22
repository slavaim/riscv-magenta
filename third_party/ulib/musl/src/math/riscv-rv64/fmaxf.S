#ifndef __riscv_float_abi_soft

.global fmaxf
.type   fmaxf, %function
fmaxf:
        fmax.s fa0, fa0, fa1
        ret

#endif
