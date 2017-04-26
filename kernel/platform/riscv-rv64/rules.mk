# Copyright 2016 The Fuchsia Authors
# Copyright 2017 Slava Imameev
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

ARCH := riscv

WITH_SMP ?= 1
SMP_MAX_CPUS ?= 8

LK_HEAP_IMPLEMENTATION ?= cmpctmalloc

# TEST TTTTTTT
KERNEL_INCLUDES += kernel/arch/riscv/include
$(info KERNEL_INCLUDES=$(KERNEL_INCLUDES))
# END TEST TTTTTTT

MODULE_SRCS += \
    $(LOCAL_DIR)/debug.c \
    $(LOCAL_DIR)/timer.c \
    $(LOCAL_DIR)/platform.c \
    $(LOCAL_DIR)/memory.c \

include make/module.mk
