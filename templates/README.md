<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# General Templates

This folder contains general templates that can be used throughout the entire repository.

The settings.json file is required to set up VS-Code for debugging. Substitute the placeholders
with the actual paths to `arm-none-eabi-gdb` and `arm-none-eabi-gcc` from the ARM Toolchain, as well as the path of `JLinkGDBServerCL`. The configuration also contains settings for the C/C++ extension, which is required for IntelliSense.

Copy this file into your `.vscode` folder in your workspace.
