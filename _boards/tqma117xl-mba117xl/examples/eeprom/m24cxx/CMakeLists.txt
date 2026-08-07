# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Mohammad Tawsif Ahmad Ansari
#

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(eeprom_m24c64 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board}/examples/eeprom)

include(${SdkRootDirPath}/CMakeLists.txt)

message(STATUS "Project: ${PROJECT_NAME} for board: ${board}")

mcux_add_source(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    SOURCES eeprom.c
            eeprom_m24cxx.c
)

mcux_add_include(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    INCLUDES .
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
