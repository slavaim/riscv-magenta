/*
Some code has been borrowed from the Linux kernel which is under GPLv2 license.
2017 Modified for Magenta by Slava Imameev.
*/

#pragma once

#ifdef __cplusplus
    #define CPP_ASMLINKAGE extern "C"
#else
    #define CPP_ASMLINKAGE
#endif

#ifndef asmlinkage
    #define asmlinkage CPP_ASMLINKAGE
#endif