# Copyright 2016 The Fuchsia Authors
# Copyright 2017 Slava Imameev
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT

# riscv64 toolchain
ifndef ARCH_riscv_TOOLCHAIN_INCLUDED
ARCH_riscv_TOOLCHAIN_INCLUDED := 1

ifndef ARCH_riscv_TOOLCHAIN_PREFIX
ARCH_riscv_TOOLCHAIN_PREFIX := riscv64-unknown-elf-
endif
FOUNDTOOL=$(shell which $(ARCH_riscv_TOOLCHAIN_PREFIX)gcc)

ifeq ($(FOUNDTOOL),)
$(error cannot find toolchain, please set ARCH_riscv_TOOLCHAIN_PREFIX or add it to your path)
endif

endif # ifndef ARCH_riscv_TOOLCHAIN_INCLUDED

# Clang
ifeq ($(call TOBOOL,$(USE_CLANG)),true)
FOUNDTOOL=$(shell which $(CLANG_TOOLCHAIN_PREFIX)clang)
ifeq ($(FOUNDTOOL),)
$(error cannot find toolchain, please set CLANG_TOOLCHAIN_PREFIX or add it to your path)
endif
endif # USE_CLANG==true
