<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# Porting Custom Examples from NXPs MCUSDK

This guide shows how to port demos for EVK boards from NXP's MCUXSDK into TQ-Systems `mcuxsdk-examples-tq` for TQ-Systems boards.

## Choosing a Demo

Most demos consist of a board specific part that can be found at `/mcuxsdk/examples/_boards/<board>` and a generic part at `/mcuxsdk/examples/`.

The generic part usually contains the following files:
  - readme.md: contains an overview of the demo and supported boards
  - CMakeLists.txt: necessary configs for cmake
  - example.yml: contains build configurations of the demo
  - prj.conf: config file for west, containing specific flags
  - <demo>.c: contains the demos code, will get included by the CMakeLists.txt

The board specific part contains configurations such as pinmuxing, hardware initialisation or linkerfiles.

## Porting a Demo

To port a demo, create a board specific directory for the demo in `_boards/<board>/examples/<path>`. The path should match the path from the MCUXSDK repository path. (e.g. `/mcuxsdk/examples/demo_apps/hello_world/` should be created in `/mcuxsdk-examples-tq/_boards/<board>/examples/demo_apps/hello_world`).

Copy the `prj.conf` with its content into the demo's directory. 

Copy the `example.yml` into the demo's directory and set the content to:

```
example.support:
  build_type:
    - "debug"
    - "flexspi_nor_debug"
    - "flexspi_nor_sdram_debug"
    - "release"
    - "flexspi_nor_release"
    - "flexspi_nor_sdram_release"
```

Note: Some demos may require a specific build configuration and will not work with others. For an overview of the given build configurations, see [list of build configurations](./_boards/tqma117xl-mba117xl/README.md#building-demos-for-tqma117xl)

Copy the `CMakeLists.txt` from the example into the demo's directory and adjust the settings in this file. Make sure to include the source files from the generic demo sources and the board specific sources using the `mcux_add_source` and the header files using the `mcux_add_include` CMake extension from the MCUXSDK.

Some board specific demo source files from the MCUXSDK might need to be adjusted to the TQ-Systems board. This is usaually the case for pin muxing, clock configuration and interface initialisation.
In this case, the adjusted source file can be put into the custom demo directory and included via `mcux_add_source` in `CMakeLists.txt`.

## Building and Running the Demo

After creating the custom demo, have a look at the board specific READMEs, that explain how to build and debug a demo.
