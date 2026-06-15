# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_sdram_debug flexspi_nor_sdram_release
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    LINKER MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.ld
)

mcux_add_armgcc_linker_script(
    TARGETS flexspi_nor_debug flexspi_nor_release
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    LINKER MIMXRT1176xxxxx_cm7_flexspi_nor.ld
)
