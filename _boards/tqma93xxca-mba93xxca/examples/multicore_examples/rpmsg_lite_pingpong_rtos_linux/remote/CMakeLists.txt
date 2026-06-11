# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(rpmsg_lite_pingpong_rtos_linux_remote LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board}/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote)

message(STATUS "Project: ${PROJECT_NAME} for board: ${board}")

include(${SdkRootDirPath}/CMakeLists.txt)

include(${CUSTOM_BOARD_ROOT}/${board}/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/multicore_examples/rpmsg_lite_pingpong_rtos/secondary/main.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/multicore_examples/rpmsg_lite_pingpong_rtos/secondary
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400 \
             __use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
