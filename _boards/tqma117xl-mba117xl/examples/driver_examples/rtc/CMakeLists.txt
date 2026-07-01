# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Mohammad Tawsif Ahmad Ansari
#

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rtc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/snvs/snvs_hp_rtc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/snvs_hp/snvs_hp_rtc/snvs_hp_rtc.c
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/rtc/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_include(INCLUDES .)
