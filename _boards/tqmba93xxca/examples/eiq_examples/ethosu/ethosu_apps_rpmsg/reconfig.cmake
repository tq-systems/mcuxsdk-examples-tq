# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Mohammad Tawsif Ahmad Ansari

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
            ${board}/examples/eiq_examples/ethosu/ethosu_apps_rpmsg/FreeRTOSConfig.h
            ${board}/examples/eiq_examples/ethosu/ethosu_apps_rpmsg/rpmsg_config.h
)

mcux_add_include(
    BASE_PATH ${CUSTOM_BOARD_ROOT}
    INCLUDES ${board}
             ${board}/examples/eiq_examples/ethosu/ethosu_apps_rpmsg
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/multicore/remoteproc
)

mcux_add_armgcc_configuration(
    CC "-fno-strict-aliasing\
       -Wno-unused-function"
    LD "-Xlinker --no-wchar-size-warning"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
    CX "-O1"
)

mcux_add_macro(
    CC "-DARM_MATH_CM33\
       -D__FPU_PRESENT=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1\
       -DETHOSU_ARCH=u65\
       -DETHOSU65=1"
    CX "-DARM_MATH_CM33\
       -D__FPU_PRESENT=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1\
       -DETHOSU_ARCH=u65\
       -DETHOSU65=1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
    CX "-O0"
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/i.MX/i.MX93/MIMX9352/gcc/MIMX9352xxxxM_ram.ld
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${CUSTOM_BOARD_ROOT}
    TARGETS debug release
    LINKER ${board}/target/MIMX9352_cm33_lpa_ram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS debug release
    LINKER MIMX9352_cm33_ram.ld
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000\
        -Xlinker --defsym=__heap_size__=0x2000"
)
