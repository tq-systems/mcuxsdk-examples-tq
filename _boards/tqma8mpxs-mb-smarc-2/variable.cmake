# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2024 NXP
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#

mcux_set_variable(board tqma8mpxs-mb-smarc-2)

if (NOT DEFINED device)
    mcux_set_variable(device MIMX8ML8)
endif()

include(${SdkRootDirPath}/devices/i.MX/i.MX8MP/${device}/variable.cmake)
