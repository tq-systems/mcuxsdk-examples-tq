# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

mcux_add_armgcc_configuration(
    TARGETS sdram_debug              sdram_release
            flexspi_nor_sdram_debug  flexspi_nor_sdram_release
            sdram_txt_debug          sdram_txt_release
    CC "-DSKIP_SYSCLK_INIT=1 -D__STARTUP_INITIALIZE_RAMFUNCTION -DDATA_SECTION_IS_CACHEABLE=1 -DXIP_BOOT_HEADER_DCD_ENABLE=1"
    CX "-DSKIP_SYSCLK_INIT=1 -D__STARTUP_INITIALIZE_RAMFUNCTION -DDATA_SECTION_IS_CACHEABLE=1 -DXIP_BOOT_HEADER_DCD_ENABLE=1"
)

mcux_add_armgcc_configuration(
    CC "-DSKIP_DCDC_ADJUSTMENT=1"
    CX "-DSKIP_DCDC_ADJUSTMENT=1"
)

mcux_add_armgcc_configuration(
    TARGETS flexspi_nor_debug        flexspi_nor_release
            flexspi_nor_sdram_debug  flexspi_nor_sdram_release
    LD "-Xlinker --defsym=__use_flash64MB__=1"
)
