# Copyright 2016 The Fuchsia Authors
# Copyright 2017 Slava Imameev
#
# Use of this source code is governed by a MIT-style
# license that can be found in the LICENSE file or at
# https://opensource.org/licenses/MIT

# Project file to build magenta + user space on top of qemu
# emulating riscv 64bit

SUBARCH := riscv-rv64
MODULES +=

#include project/virtual/test.mk
#include project/virtual/user.mk
include project/target/qemu-riscv-rv64.mk