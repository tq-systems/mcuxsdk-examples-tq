# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2024 NXP
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Mohammad Tawsif Ahmad Ansari
#

board_runner_args(jlink "--device=MIMXRT1176XXXA_M7")

include(${SdkRootDirPath}/cmake/extension/runner/jlink.board.cmake)
