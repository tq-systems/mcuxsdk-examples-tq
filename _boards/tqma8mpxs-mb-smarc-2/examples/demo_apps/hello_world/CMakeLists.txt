# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(hello_world LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board}/examples/demo_apps/hello_world)

message(STATUS "Project: ${PROJECT_NAME} for board: ${board}")

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/hello_world/hello_world.c
)


mcux_add_include(
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    INCLUDES .
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
