# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

mcux_set_variable(board tqma117xl-mba117xl)

if (NOT DEFINED device)
    mcux_set_variable(device MIMXRT1176)
endif()

include(${SdkRootDirPath}/devices/RT/RT1170/${device}/variable.cmake)
