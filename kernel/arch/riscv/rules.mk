# Copyright 2016 The Fuchsia Authors
# Copyright 2017 Slava Imameev
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT

LOCAL_DIR := $(GET_LOCAL_DIR)

ifeq ($(SUBARCH),riscv-rv64)
    SUBARCH_DIR := $(LOCAL_DIR)/rv64
	GLOBAL_CFLAGS += -mabi=lp64
	GLOBAL_CPPFLAGS += -mabi=lp64
	GLOBAL_LDFLAGS += -melf64lriscv
else
    $(error The 32 bit RISC-V is not supported yet)
	SUBARCH_DIR := $(LOCAL_DIR)/rv32
	GLOBAL_CFLAGS += -mabi=ilp32
	GLOBAL_CPPFLAGS += -mabi=ilp32
	GLOBAL_LDFLAGS += -melf32lriscv
endif

SUBARCH_BUILDDIR := $(call TOBUILDDIR,$(SUBARCH_DIR))

$(info LINKER_SCRIPT = $(LINKER_SCRIPT))

MEMBASE ?= 0
KERNEL_BASE ?= 0xffffffff80000000
KERNEL_SIZE ?= 0x40000000 # 1GB
KERNEL_LOAD_OFFSET ?= 0x00100000
HEADER_LOAD_OFFSET ?= 0x00010000
PHYS_HEADER_LOAD_OFFSET ?= 0x00002000
KERNEL_ASPACE_BASE ?= 0xffffff8000000000UL # -512GB
KERNEL_ASPACE_SIZE ?= 0x0000008000000000UL
USER_ASPACE_BASE   ?= 0x0000000001000000UL # 16MB
# We set the top of user address space to be (1 << 47) - 4k.  See
# docs/magenta/sysret_problem.md for why we subtract 4k here.
# Subtracting USER_ASPACE_BASE from that value gives the value for
# USER_ASPACE_SIZE below.
USER_ASPACE_SIZE   ?= 0x00007ffffefff000UL

KERNEL_DEFINES += \
	ARCH_$(SUBARCH)=1 \
	MEMBASE=$(MEMBASE) \
	KERNEL_BASE=$(KERNEL_BASE) \
	KERNEL_SIZE=$(KERNEL_SIZE) \
	KERNEL_LOAD_OFFSET=$(KERNEL_LOAD_OFFSET) \
	PHYS_HEADER_LOAD_OFFSET=$(PHYS_HEADER_LOAD_OFFSET)

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

MODULE := $(LOCAL_DIR)

MODULE_SRCS += \
	$(LOCAL_DIR)/debugger.c \
	$(LOCAL_DIR)/guest_mmu.c \
	$(LOCAL_DIR)/hypervisor.cpp \
	$(LOCAL_DIR)/mmu.c \
	$(LOCAL_DIR)/mp.c \
	$(LOCAL_DIR)/ops.c \
	$(LOCAL_DIR)/thread.c \
	$(LOCAL_DIR)/user_copy.c

LINKER_SCRIPT += $(SUBARCH_BUILDDIR)/kernel.ld

# potentially generated files that should be cleaned out with clean make rule
GENERATED += $(SUBARCH_BUILDDIR)/kernel.ld

# rules for generating the linker scripts
$(SUBARCH_BUILDDIR)/kernel.ld: $(SUBARCH_DIR)/kernel.ld $(wildcard arch/*.ld)
	@echo generating $@
	@$(MKDIR)
	$(NOECHO)sed "s/%MEMBASE%/$(MEMBASE)/;s/%MEMSIZE%/$(MEMSIZE)/;s/%KERNEL_BASE%/$(KERNEL_BASE)/;s/%KERNEL_LOAD_OFFSET%/$(KERNEL_LOAD_OFFSET)/;s/%HEADER_LOAD_OFFSET%/$(HEADER_LOAD_OFFSET)/;s/%PHYS_HEADER_LOAD_OFFSET%/$(PHYS_HEADER_LOAD_OFFSET)/;" < $< > $@.tmp
	@$(call TESTANDREPLACEFILE,$@.tmp,$@)

# force a rebuild every time in case something changes
$(SUBARCH_BUILDDIR)/kernel.ld: FORCE

include $(LOCAL_DIR)/toolchain.mk

include make/module.mk