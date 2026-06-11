# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2024 NXP
#
# Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#

mcux_set_variable(board tqma93xxca-mba93xxca)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX9352)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX93/${device}/variable.cmake)
