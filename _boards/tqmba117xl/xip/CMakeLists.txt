# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

if (CONFIG_MCUX_COMPONENT_board.boot_header)

        message(STATUS "Adding boot header configuration for ${board} board")
        
        mcux_add_source(
        SOURCES ${board}_flexspi_nor_config.c
                ${board}_flexspi_nor_config.h
        )

        mcux_add_include(
        INCLUDES .
        )

        mcux_add_source(
        SOURCES dcd.c
                dcd.h
        )

        mcux_add_configuration(
                TARGETS flexspi_nor_debug        flexspi_nor_release
                        flexspi_nor_sdram_debug  flexspi_nor_sdram_release
                CC "-DXIP_EXTERNAL_FLASH=1 -DFLASH_TARGET"
        )

        mcux_add_configuration(
                TARGETS flexspi_nor_sdram_debug  flexspi_nor_sdram_release
                CC "-DXIP_BOOT_HEADER_DCD_ENABLE=1"
        )
endif()
