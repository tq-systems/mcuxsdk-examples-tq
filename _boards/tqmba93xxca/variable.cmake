# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#
# Copyright 2024 NXP
#

mcux_set_variable(board tqmba93xxca)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX9352)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX93/${device}/variable.cmake)
