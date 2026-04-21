# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/remoteproc.h
)

mcux_add_source(
    BASE_PATH ${CUSTOM_BOARD_ROOT}
    SOURCES ${board}/FreeRTOSConfigBoard.h
            ${board}/rsc_table.c
            ${board}/rsc_table.h
            ${board}/srtm_config.h
            ${board}/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h
            ${board}/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/rpmsg_config.h
)

mcux_add_include(
    BASE_PATH ${CUSTOM_BOARD_ROOT}
    INCLUDES ${board}
             ${board}/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux
             ${board}/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
)
