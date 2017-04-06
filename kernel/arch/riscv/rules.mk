# Copyright 2016 The Fuchsia Authors
# Copyright 2017 Slava Imameev
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT

LOCAL_DIR := $(GET_LOCAL_DIR)

SMP_MAX_CPUS ?= 8
$(info SMP_MAX_CPUS = $(SMP_MAX_CPUS))

# always set this to something
KERNEL_DEFINES += \
	SMP_MAX_CPUS=$(SMP_MAX_CPUS)

$(info LOCAL_DIR = $(LOCAL_DIR))

# try to find the toolchain
include $(LOCAL_DIR)/toolchain.mk

$(info ARCH = $(ARCH))

TOOLCHAIN_PREFIX := $(ARCH_$(ARCH)_TOOLCHAIN_PREFIX)

$(info TOOLCHAIN_PREFIX = $(TOOLCHAIN_PREFIX))