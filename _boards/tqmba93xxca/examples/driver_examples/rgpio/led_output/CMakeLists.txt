# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rgpio_led_output LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board}/examples/driver_examples/rgpio/led_output)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/rgpio/led_output/rgpio_led_output.c
            drivers/rgpio/fsl_rgpio.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES drivers/rgpio
             examples/driver_examples/rgpio/led_output
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
