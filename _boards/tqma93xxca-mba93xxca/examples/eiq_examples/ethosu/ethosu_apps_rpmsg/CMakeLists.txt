# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Mohammad Tawsif Ahmad Ansari

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ethosu_apps_rpmsg LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board}/examples/eiq_examples/ethosu/ethosu_apps_rpmsg)

message(STATUS "Project: ${PROJECT_NAME} for board: ${board}")

include(${SdkRootDirPath}/CMakeLists.txt)

include(${CUSTOM_BOARD_ROOT}/${board}/examples/eiq_examples/ethosu/ethosu_apps_rpmsg/reconfig.cmake OPTIONAL)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/ethosu/ethosu_apps_rpmsg/ethosu_apps_rpmsg.cpp
            examples/eiq_examples/ethosu/ethosu_apps_rpmsg/ethosu_core_interface.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/eiq_examples/ethosu/ethosu_apps_rpmsg
)

mcux_add_armgcc_configuration(
    CX "-Wno-maybe-uninitialized\
        -Wno-deprecated-declarations"
)

mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1 \
             __multicore__=1 \
            "
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
