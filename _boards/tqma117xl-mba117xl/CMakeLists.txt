# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2024 NXP
#
# Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

include(${CMAKE_CURRENT_LIST_DIR}/common/include.cmake)
include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/target/flags.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/RT1xxx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/setting.cmake OPTIONAL)
include(${CMAKE_CURRENT_LIST_DIR}/../_common/common_include.cmake)
