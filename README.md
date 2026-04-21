# MCUXpresso SDK: TQ-Systems Board Support

This guide contains instructions for configuring the build system, building targets, and executing the demonstration
applications included in this repository, specifically designed for TQ-developed boards using the i.MX MCU family from NXP.

## Table of Contents

[[_TOC_]]

## Getting Started

### Requirements

__NOTE__: The versions provided are the ones with which the build system and its artifacts were tested.

- Arm GNU Toolchain: arm-gnu-toolchain-13.3.rel1-[host system]-arm-none-eabi
- NXP's [MCUXSDK-Manifest](https://github.com/nxp-mcuxpresso/mcuxsdk-manifests): v26.03.00
- CMake: v3.30.0^
- West: v1.2.0^
- Python: v3.10^
- Ninja v1.12.1^
- For Debug only:
  - [Segger J-link](https://www.segger.com/downloads/jlink/): v8.66l

#### VS-Code Extensions

- For Debug only:
  - Cortex-Debug (from marus25): v1.12.0^
    - ID: `marus25.cortex-debug`

### Preparation

- Python3 installation (needed for the west repo tool and helper scripts).
- Install [Arm GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain).
  Use the .exe file for installation or unpack the archive to the desired path.
- Install [CMake](https://cmake.org/download/) and ensure that `CMake` is added to the system path.
- Set the environment variable `ARMGCC_DIR` pointing to the toolchain installation dir:
  - VS-Code / Codium: use the "cmake.environment" option under [settings.json](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-settings.md).
  - Command Line: Create a variable, e.g. `export ARMGCC_DIR=</path/to/toolchain>`.
- Debugging only: Install [Segger J-link](https://www.segger.com/downloads/jlink/) (Version used: `8.66`).
- Follow the installation instructions at:
[Getting Started with SDK - Detailed Installation Instructions](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/installation.html#installation)
- Create a workspace directory and clone this repository into the workspace.
- Initialize the west workspace and create a python venv.

  Under Linux the `scripts/prepare.sh` script can be used, or run the following commands manually:

  ```bash
  mkdir <workspace>
  cd <workspace>
  git clone https://github.com/tq-systems/mcuxsdk-examples-tq.git
  west init --local "<workspace>/mcuxsdk-examples-tq"
  west update
  west config commands.allow_extensions true
  python3 -m venv <workspace>/.venv
  source <workspace>/.venv/bin/activate
  pip install -r "<workspace>/mcuxsdk/scripts/requirements.txt" -c "<workspace>/mcuxsdk-examples-tq/scripts/constraints.txt"
  pip install -r "<workspace>/mcuxsdk-examples-tq/scripts/requirements.txt"
  deactivate
  ```

**IMPORTANT NOTES**:

- `west` will clone the MCUXpresso SDK into the `<workspace>/mcuxpresso-sdk` folder per default.
  The location can be adjusted using the `path` attribute in `west.yml`.
- `west.yml` configuration file in this repository is a reduced version of the entire MCUXpresso SDK
  to speed up cloning.
- Manual installation of MCUXpresso SDK and using the examples in this repository separately is
  out of scope of this documentation.
- See the [NXP MCUXpresso SDK Documentation](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/introduction/README.html)
  for SDK documentation.

### Setting Up VS-Code

- Open Visual Studio from the applications folder.
- Please refer to the [example templates](./templates/) when setting up VS-Code. You should have at least the same
`.json-files` and `.yaml-files` in your [.vscode folder](./.vscode) by the end of this section.
- Install the [CMake tools extension by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools).
  - Ctrl+P: `ext install ms-vscode.cmake-tools` or search for the extension under the menu extensions on the left bar.
  - Adjust [settings.json](https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-settings.md)
    in the [.vscode](./.vscode) folder accordingly to your environment. You can follow the provided **settings.json**
    under "<_board_>/templates".
- For Debug only: Install [cortex-debug](https://github.com/Marus/cortex-debug/wiki)
  extension from marus25.
  - either use `Ctrl+P` and `ext install marus25.cortex-debug`
  - or search for the extension under the menu extensions on the left bar
  - Set up the desired configurations for debug on VS-Code. You will need two files:
    - one to configure the debug settings for VS-Code.
      - You can follow the provided **settings.json** under "<_board_>/templates".
    - the other to set up the debugger configuration.
       - You can follow the provided **launch.json** under "<_board_>/templates".

> [!TIP]
> Here is an entire [guide](https://wiki.segger.com/J-Link_Visual_Studio_Code)
> for setting up VS-Code in order to use J-Link Segger as a debugger with VS-Code.

> [!NOTE]
> Please refer, if existing, to the README of your board for further details on setting up the debugger.
> Some boards may require special settings. The README should be placed under: `examples/README.md`.

## Building

To build examples, use the `west build` command inside the root directory of this repository.
The following commands can be used under Linux to work in a workspace that was set up as
[documented](#preparation):

```bash
cd <workspace>
source ".venv/bin/activate"
west build _boards/<board>/<app_location> --board <board> \
-Dcore_id=<core> --build-dir build \
-DCUSTOM_BOARD_ROOT="<workspace>/mcuxsdk-examples-tq>/_boards" \
--config=<configuration> --pristine
```

**IMPORTANT NOTES**:

- The `CUSTOM_BOARD_ROOT` parameter must be used to tell the SDK where to find boards and examples.
- If you installed the MCUXpresso SDK manually, all `west` commands must run from within the SDK directory.
- The `--config` parameter is optional. If not provided, the default configuration will be used.
  Available configurations can be found in the `examples.yml` file of the respective board.
- The `core_id` must be set if your board has multiple cores. This can be recognized by the presence of core folders
  in the board directory. If your board has only one core, this parameter might not be required.
  Example core IDs are: `cm4`, `cm7`, `cm33`.

## Loading Targets

Before running a target, make **boot instructions of your board**.

### Loading a Target via GDB Server

If you prefer to boot without VS-Code, follow the instructions for debugging within the **board's README**.

### Loading a Target with VS-Code

  You can follow the instructions for [debugging with VS-Code](#debugging-with-vs-code).

## Debugging

### Debugging with VS-Code

The repository utilizes `JLinkGDBServerCL` alongside the appropriate hardware for debugging in VS-Code. To initiate
debugging, you need to configure the [debug tool](#setting-up-vs-code). Once configured, select the debug tool from the
left menu bar and choose the desired debug configuration specified within the [launch.json](/templates/launch.json).
To begin debugging, either click on `start debugging` or press `F5`.

> [!NOTE]
> Please refer, if existing, to the README of your board for further details on setting up the debugger.
> Some boards may require special settings. The README should be placed under: `examples/README.md`.

## Applications

The list of all applications can be found in the [board](./examples/README.md#applications) README file.

## Build System

### Structure

This repository is structured almost like the examples folder inside the mcuxsdk repository. The main difference is that
this repository includes the source and header files for the examples from a CMakesLists.txt file located inside a dedicated
examples folder for each board inside this repository.

Besides the boards folder this repository provides as well scripts to help with the setup of the environment and CI.

It also provides access to an TQ specific submodule for including some light weight API for handling specific devices.

The following table provides an overview of the top-level folders in this repository and their purposes:

| Folder Name       | Description                                                                 |
|-------------------|-----------------------------------------------------------------------------|
| `_boards`         | Contains dedicated folders for all boards, which mimics the examples folder/_boards folder from the mcuxsdk repository     |
| `_boards/_common`         | Shared resources and configurations used across multiple boards.           |
| `_boards/<board>/templates`         | Some templates for IDEs (e.g.VS-Code settings)         |
| `scripts`         | Helper scripts for setting up the environment, building, and CI workflows. |
| `tq-api`           | Contains lightweight APIs for handling specific devices.             |

### Programming Apps

This guide provides instructions and hints on structuring the build system within the context of MCUXpresso SDKs and the
build system structure within this repository when creating new apps or boards.

The build system is built using west. West uses CMake and Kconfig to manage the build system.

To create a new application, you must create a new folder for it inside the examples directory of the board and create
a `CMakeLists.txt` file located in `../examples/<app_name>`.

> [!TIP]
> You can use any of the CMakeLists.txt files from the existing demo applications as a template.
> Be also aware that depending on your app requirements you must choose the correct components or even add new ones
> to the board via the KConfig tool.

Compiler and linker flags are set in the `flags.cmake` file in the `<board>/target` directory. However, if an app
requires special flags you can set it inside the `CMakeLists.txt` of the application directory. You can use e.g.
`mcux_add_armgcc_configuration` for adding flags for the arm compiler.

When programming, use the linker scripts provided by TQ. You can switch between the different linker scripts by using the
`--config` parameter in the `west build` command. The available configurations can be found in the "examples.yml" file
of the respective board. The linker scripts are located in the `<board>/target` directory.

You can switch the location of the board files (pinmux, clock, board, hardware_init.c, and linker scripts) by using the
KConfig options provided in the `<board>/_common/Kconfig` file. You can choose between having the files in a common folder
(`_common`) or in the project folder (`<app_name>`). The KConfig options are set in the `prj.conf` file of the board.

> [!TIP]
> Kconfig includes `prj.conf` files recursively. This means that the last `prj.conf` file found will override the
> previous ones. The order of inclusion is:
>
> 1. `<board>/prj.conf`
> 2. `<board>/<core_id>/prj.conf` (if core_id is set)
> 3. `<board>/<app>/prj.conf`
>
> You can set any `prj.conf` files inside an application folder to override the default `prj.conf` file of the board.

For learning about NXP's API, the examples under `mcuxsdk/examples` can be helpful.

## License

Except where otherwise noted, all files within this repository are licensed under the following terms, excluding
auto-generated files:

SPDX-License-Identifier: BSD-3-Clause

All documentation is licensed under
SPDX-License-Identifier: CC-BY-4.0 OR BSD-3-Clause

License Text see
[BSD-3-Clause](./LICENSE.md)
[CC-BY-4.0 (Creative Commons Attribution 4.0 International Public License)](./COPYING.CC-BY-4.0).

Copyright (c) 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,  
D-82229 Seefeld, Germany.
Author: Isaac L. L. Yuki

## Support Wiki

For more information, please refer to our [support wiki](https://support.tq-group.com/).
