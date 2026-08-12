#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

"""
Converted from the Bash script and extended:

  - Collect all `variable.cmake` files under ../_boards
  - Find board names from each `variable.cmake` via regex
    (mcux_set_variable for `board`)
  - Add a sanity-check assertion that at least one board was found
  - Find every `prj.conf` inside `<board>/examples/**/`
  - Print the same summaries the Bash script printed
  - cd into $MCUXSDK_ROOT and check that `west build --help` executes.
"""

from __future__ import annotations

import os
import re
import sys
import subprocess
from pathlib import Path
import argparse
import yaml

# ------------------------------------------------------------
# 0.  Definitions
# ------------------------------------------------------------

CWD = Path.cwd()

# Regex that tolerates indentation and optional space after "(".
BOARD_REGEX = re.compile(r"""^\s*mcux_set_variable\(\s*board\s+([^\s\)]+)""")

# ------------------------------------------------------------
# 1.  Class
# ------------------------------------------------------------

class Board:
    """Represents a board with its name and path."""

    def __init__(self, name: str, path: Path):
        self.name = name
        self.path = path
        self.supported_cores: list[str] = []
        self.supported_examples: list[dict] = []
        self.load_from_yaml()
        self.find_examples()

    # ---------------------- metadata helpers ---------------------- #
    def load_from_yaml(self):
        """Load board configuration from *board.yml* if present."""
        yaml_file = self.path / "board.yml"
        if yaml_file.is_file():
            with yaml_file.open() as fh:
                data = yaml.safe_load(fh)
            if data and isinstance(data, dict):
                cores = data.get("board.cores")
                for core in cores:
                    if isinstance(core, dict) and "id" in core and "support" in core:
                        if core.get("support", True):
                            self.supported_cores.append(core.get("core_id", core["id"]))
                    else:
                        print(f"Warning: Invalid core definition in {yaml_file}", file=sys.stderr)

    def find_examples(self):
        """Populate *supported_examples* with dirs containing `prj.conf`."""
        examples_dir = self.path / "examples"
        if examples_dir.is_dir():
            for prj_file in examples_dir.glob("**/prj.conf"):
                example_yaml = prj_file.parent / "example.yml"
                example_dict = {"path": "", "build_configuration": {}}
                if example_yaml.is_file():
                    with example_yaml.open() as fh:
                        example_data = yaml.safe_load(fh)
                    if example_data and isinstance(example_data, dict):
                        build_type = example_data.get("example.support", {}).get("build_type", [])
                        if isinstance(build_type, list) and build_type:
                            example_dict["path"] = prj_file.parent
                            example_dict["build_configuration"] = build_type
                            self.supported_examples.append(example_dict)

    # ---------------------- dunders ---------------------- #
    def __repr__(self):
        return f"Board(name={self.name}, path={self.path})"

# ------------------------------------------------------------
# 2.  Helpers
# ------------------------------------------------------------

def strip_quotes(s: str):
    """Remove symmetrical single or double quotes surrounding *s* (windows safe)."""
    if not s:
        return None
    if len(s) >= 2 and s[0] == s[-1] and s[0] in "'\"":
        return s[1:-1]
    return s

def get_board_name(file: Path) -> str:
    """Extract board name from *variable.cmake* using BOARD_REGEX.

    Returns a list because, in theory, a file might define several boards.
    """

    for line in file.read_text().splitlines():
        m = BOARD_REGEX.match(line)
        if m:
            return m.group(1)
    return None

def build_loop(targets:list[Board],
               workdir: Path,
               boarddir: Path,
               generator: str,
               build_configs: list):
    """
    Build all boards with their examples and cores.
    This function is called at the end of the script.
    """
    for target in targets:
        for core in target.supported_cores:
            for example in target.supported_examples:
                for build_configuration in example["build_configuration"]:
                    for config in build_configs:
                        if config in build_configuration:
                            config = f'{config}'
                            example_workdir = Path(workdir) / str(target.name) / f"{example['path'].name}_{core}" / str(config)

                            # disable exception, check for returncode instead.
                            res = subprocess.run(
                                [
                                    "west", "build",
                                    "--pristine",
                                    "--board", target.name,
                                    str(example["path"]),
                                    "--build-dir", str(example_workdir),
                                    f"-DCUSTOM_BOARD_ROOT={boarddir}",
                                    f"-Dcore_id={core}",
                                    f"--config={config}",
                                    f"-G{generator}",
                                ],
                                text=True,
                                check=False
                            )

                            if res.returncode == 0:
                                print(f"[BUILD_PROCESS] ✅  Build successful for {example_workdir}")
                            else:
                                print(f"[BUILD_PROCESS] ❌ Build **failed** for {example_workdir}")
                                return -1
    return 0

def main():
    parser = argparse.ArgumentParser(description="MCUXSDK Build Script")
    parser.add_argument("--mcuxsdk_root", type=str, help="Path to MCUXSDK root directory")
    parser.add_argument("--build_root", type=str, help="Path to build output directory")
    parser.add_argument("--build_types",
                        nargs='*',
                        default="all",
                        help="List of build types to use (default: all, options: debug, release, all)")
    parser.add_argument("--generator",
                        type=str,
                        help="choose build generator e.g. Ninja, Unix Makefiles",
                        default="Ninja")
    parser.add_argument("--board",
                        type=str,
                        help="Specify one board to build, if not set all boards will be built",
                        default="all")
    args = parser.parse_args()
    board_root = None
    build_types = None

    if args.build_root is not None:
        build_root = strip_quotes(args.build_root)
    else:
        sys.exit("Error: --build_root is missing.")

    if args.mcuxsdk_root is not None:
        mcux_root =  strip_quotes(args.mcuxsdk_root)
    else:
        sys.exit("Error: --mcuxsdk_root is missing.")

    if args.build_types == "all":
        build_types = ["debug", "release"]
    elif args.build_types == "debug":
        build_types = ["debug"]
    elif args.build_types == "release":
        build_types = ["release"]
    else:
        sys.exit("Error: --build_types is not valid.")

    if args.generator is not None:
        generator = strip_quotes(args.generator)
    else:
        sys.exit("Error: --generator is missing.")

    if args.board is not None or not args.board == "all":
        board = args.board
    else:
        sys.exit("Error: --board is missing.")

# ------------------------------------------------------------
# 4.  Locate every variable.cmake and grab the board name via CMake
# ------------------------------------------------------------

    if "scripts" not in CWD.parts:
        board_root = CWD / "_boards"
    else:
        board_root = CWD / ".." / "_boards"

    if not board_root.is_dir():
        sys.exit(
            f"Error: The board root directory '{board_root}' does not exist. "
            "Please ensure you are in the correct directory."
        )

    boards: list[Board] = []

    for var_file in board_root.rglob("variable.cmake"):
        board_name = get_board_name(var_file)
        if board_name:
            boards.append(Board(board_name, var_file.parent))
            print(f"Found board: {boards[-1]}")

    if board is not None:
        if board in [b.name for b in boards]:
            boards = [b for b in boards if b.name == board]

# ---------------------- sanity check ---------------------- #
    assert boards, (
        f"No board names found under {board_root}. "
        "Verify that variable.cmake files exist and that CMake is able to parse them."
    )

# ------------------------------------------------------------
# 5.  Summaries
# ------------------------------------------------------------
    print(f"\nBoards found ({len(boards)}): {' '.join(b.name for b in boards)}")

    example_dirs = [e for b in boards for e in b.supported_examples]
    print(f"Examples with prj.conf found ({len(example_dirs)}):")
    for ex in example_dirs:
        print(f"  {ex}")

# ------------------------------------------------------------
# 6.  Change to $MCUXSDK_ROOT and verify west is available
# ------------------------------------------------------------

    try:
        os.chdir(mcux_root)
    except FileNotFoundError:
        sys.exit(f"Error: MCUXSDK_ROOT directory '{mcux_root}' does not exist.")

    # disable exception, check for returncode instead.
    result = subprocess.run(
        ["west", "--version"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        check=False
    )

    if result.returncode != 0:
        sys.exit(
            "West build command not found. "
            "Please ensure you have the MCUX SDK (and West) installed."
        )

    print("\nEnvironment check passed – ready to build.")

# ------------------------------------------------------------
# 7. Build all boards
# ------------------------------------------------------------

    build_result = build_loop(boards, build_root, board_root, generator, build_types)

# ------------------------------------------------------------
# 6.  Combine and save / show diagnostics
# ------------------------------------------------------------

    if build_result != 0:
        sys.exit(
            "build command error, terminate "
        )

if __name__ == "__main__":
    main()
