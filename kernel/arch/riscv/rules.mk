# Copyright 2016 The Fuchsia Authors
# Copyright 2017 Slava Imameev
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT


LOCAL_DIR := $(GET_LOCAL_DIR)

ifeq ($(SUBARCH),riscv-rv64)

    BITS_PER_LONG = 64
    SUBARCH_DIR := $(LOCAL_DIR)/rv64

	# no floating point instructions (f+d|q)
	MARCH = rv64im
	MABI = lp64

	GLOBAL_LDFLAGS += --verbose -melf64lriscv
	GLOBAL_LDFLAGS_TAIL += -L/work/risc-v/riscv-tools/build-rv64im-lp64/lib/gcc/riscv64-unknown-linux-gnu/6.1.0  -lgcc
else
    $(error The 32 bit RISC-V is not supported yet)
	BITS_PER_LONG = 32
	SUBARCH_DIR := $(LOCAL_DIR)/rv32

	# no floating point instructions (f+d|q)
	MARCH = rv32im
	MABI = ilp32

	GLOBAL_LDFLAGS += -melf32lriscv
endif

# atomic CPU is required for the current code base
RV_ATOMIC = a

# KERNEL_COMPILEFLAGS += -Wall

ifeq ($(CONFIG_RVC),y)
	RVC = c
endif

GLOBAL_CFLAGS += -mabi=$(MABI)
GLOBAL_CPPFLAGS += -mabi=$(MABI)
KERNEL_COMPILEFLAGS += -mabi=$(MABI)

# TO_DO_RISCV define applications flags with fd to allow floating point operations
#GLOBAL_CFLAGS += -march=$(MARCH)$(RV_ATOMIC)fd$(RVC)
#GLOBAL_CPPFLAGS += -march=$(MARCH)$(RV_ATOMIC)fd$(RVC)

# disable floating point instructions in the kernel, no f and d|q set
KERNEL_COMPILEFLAGS += -march=$(MARCH)$(RV_ATOMIC)$(KBUILD_RVC)
KERNEL_COMPILEFLAGS += -mno-save-restore

$(info KERNEL_COMPILEFLAGS = $(KERNEL_COMPILEFLAGS))

SUBARCH_BUILDDIR := $(call TOBUILDDIR,$(SUBARCH_DIR))

$(info LINKER_SCRIPT = $(LINKER_SCRIPT))

MEMBASE ?= 0
KERNEL_BASE ?= 0xffffffff80000000
KERNEL_ASPACE_BASE ?= 0xffffff8000000000UL # -512GB
KERNEL_ASPACE_SIZE ?= 0x0000008000000000UL
USER_ASPACE_BASE   ?= 0x0000000001000000UL # 16MB
# We set the top of user address space to be (1 << 47) - 4k.  See
# docs/magenta/sysret_problem.md for why we subtract 4k here.
# Subtracting USER_ASPACE_BASE from that value gives the value for
# USER_ASPACE_SIZE below.
USER_ASPACE_SIZE   ?= 0x00007ffffefff000

# The following two definitions are not required for RISC-V
# but checked on kernel compilation TO_DO_RISCV
KERNEL_LOAD_OFFSET ?= 0
PHYS_HEADER_LOAD_OFFSET ?= 0

KERNEL_DEFINES += \
	ARCH_$(SUBARCH)=1 \
	MEMBASE=$(MEMBASE) \
	KERNEL_BASE=$(KERNEL_BASE) \
	KERNEL_LOAD_OFFSET=$(KERNEL_LOAD_OFFSET) \
	PHYS_HEADER_LOAD_OFFSET=$(PHYS_HEADER_LOAD_OFFSET) \
	BITS_PER_LONG=$(BITS_PER_LONG) \

# debug checkss
KERNEL_DEFINES += \
	LK_DEBUGLEVEL=2

ifeq ($(SUBARCH),riscv-rv64)
	KERNEL_DEFINES += \
		CONFIG_64BIT=1
endif

GLOBAL_DEFINES += \
	KERNEL_BASE=$(KERNEL_BASE) \
    KERNEL_ASPACE_BASE=$(KERNEL_ASPACE_BASE) \
    KERNEL_ASPACE_SIZE=$(KERNEL_ASPACE_SIZE) \
    USER_ASPACE_BASE=$(USER_ASPACE_BASE) \
    USER_ASPACE_SIZE=$(USER_ASPACE_SIZE)

WITH_SMP ?= 1
SMP_MAX_CPUS ?= 8
$(info SMP_MAX_CPUS = $(SMP_MAX_CPUS))

# always set this to something
KERNEL_DEFINES += \
	WITH_SMP=$(WITH_SMP) \
	SMP_MAX_CPUS=$(SMP_MAX_CPUS) \

$(info LOCAL_DIR = $(LOCAL_DIR))

# try to find the toolchain
include $(LOCAL_DIR)/toolchain.mk

$(info ARCH = $(ARCH))

TOOLCHAIN_PREFIX := $(ARCH_$(ARCH)_TOOLCHAIN_PREFIX)

$(info TOOLCHAIN_PREFIX = $(TOOLCHAIN_PREFIX))

MODULE := $(LOCAL_DIR)


#####
# Generate asm-offsets.h
#

DIR_INCLUDE_GENERATED = include
asm-offsets-s := $(DIR_INCLUDE_GENERATED)/generated/asm-offsets.S
asm-offsets-h := $(DIR_INCLUDE_GENERATED)/generated/asm-offsets.h

# LOCAL_DIR is changed by make so the last one will be used for
# bash scripts, we need a local copy
DIR_ASM_OFFSET := $(LOCAL_DIR)

# generate the assembler file
$(SUBARCH_BUILDDIR)/$(asm-offsets-s): FORCE
	@echo generating $@ from $(DIR_ASM_OFFSET)/asm-offsets.c
	@mkdir -p $(dir $@)
	@pwd
	@$(CC) -S $(KERNEL_COMPILEFLAGS) $(GLOBAL_CFLAGS) \
	       -I$(DIR_ASM_OFFSET)/include \
		   $(DIR_ASM_OFFSET)/asm-offsets.c -o $@

# parse the assembler file to extract the offsets
$(SUBARCH_BUILDDIR)/$(asm-offsets-h): $(SUBARCH_BUILDDIR)/$(asm-offsets-s) FORCE
	@echo generating $@
	@$(MKDIR)
	@pwd
	$(call filechk,offsets,__ASM_OFFSETS_H__)

# add generated files in the list of files to remove on cleanup
GENERATED += $(SUBARCH_BUILDDIR)/$(asm-offsets-h) $(SUBARCH_BUILDDIR)/$(asm-offsets-s)

# make the module dependent on the header file generation
MODULE_SRCDEPS += $(SUBARCH_BUILDDIR)/$(asm-offsets-h)

# add the include directory in the search path
KERNEL_COMPILEFLAGS += -I$(SUBARCH_BUILDDIR)/$(DIR_INCLUDE_GENERATED)

#
# end of asm-offsets.h generation
#####

MODULE_SRCS += \
	$(SUBARCH_DIR)/start.S \
	$(SUBARCH_DIR)/sbi.S \
\
	$(LOCAL_DIR)/setup.c \
	$(LOCAL_DIR)/arch.c \
	$(LOCAL_DIR)/debugger.c \
	$(LOCAL_DIR)/guest_mmu.c \
	$(LOCAL_DIR)/hypervisor.cpp \
	$(LOCAL_DIR)/mmu.cpp \
	$(LOCAL_DIR)/mp.c \
	$(LOCAL_DIR)/ops.c \
	$(LOCAL_DIR)/thread.c \
	$(LOCAL_DIR)/user_copy.c \
	$(LOCAL_DIR)/page.c \
	$(LOCAL_DIR)/pgtable.c \
	$(LOCAL_DIR)/exceptions.c \
	$(LOCAL_DIR)/trap.c \
	$(LOCAL_DIR)/lib/memset.S \
	#$(LOCAL_DIR)/lib/clz_ctz.c \
	#$(SUBARCH_DIR)/exception.S \

$(info MODULE_SRCS = $(MODULE_SRCS))

LINKER_SCRIPT += $(SUBARCH_BUILDDIR)/kernel.ld

$(info LINKER_SCRIPT = $(LINKER_SCRIPT))

# potentially generated files that should be cleaned out with clean make rule
GENERATED += $(SUBARCH_BUILDDIR)/kernel.ld

# rules for generating the linker scripts
$(SUBARCH_BUILDDIR)/kernel.ld: $(SUBARCH_DIR)/kernel.ld $(wildcard arch/*.ld)
	@echo generating $@
	@$(MKDIR)
	$(NOECHO)sed "s/%MEMBASE%/$(MEMBASE)/;s/%MEMSIZE%/$(MEMSIZE)/;s/%KERNEL_BASE%/$(KERNEL_BASE)/;" < $< > $@.tmp
	@$(call TESTANDREPLACEFILE,$@.tmp,$@)

# force a rebuild every time in case something changes
$(SUBARCH_BUILDDIR)/kernel.ld: FORCE

include $(LOCAL_DIR)/toolchain.mk
include make/module.mk

