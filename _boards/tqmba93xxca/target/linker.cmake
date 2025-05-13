# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

mcux_add_armgcc_linker_script(
    TARGETS lpa_ram debug release
    BASE_PATH ${CMAKE_CURRENT_LIST_DIR}
    LINKER MIMX9352_cm33_lpa_ram.ld
)
