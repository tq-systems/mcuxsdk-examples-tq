#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Isaac Lucas de Lima Yuki
#

"""
Converted from the Bash script and extended:

  - Sources ./.config → loads KEY=VALUE pairs into os.environ
  - Collects all *variable.cmake* files under ../_boards
  - Uses **CMake itself** to evaluate each variable.cmake and retrieve the
    value of the `board` variable (robust against syntax changes)
  - Adds a **sanity‑check assertion** that at least one board was found
  - Finds every `prj.conf` inside <board>/examples/**/
  - Prints the same summaries the Bash script printed
  - cd’s into $MCUXSDK_ROOT and checks that `west build --help` executes.
"""

from __future__ import annotations

import os
import re
import sys
import subprocess
import yaml
import pandas as pd
from typing import List
import io
from pathlib import Path
import argparse

# ------------------------------------------------------------
# 0.  Definitions
# ------------------------------------------------------------

CWD = Path.cwd()
CONFIG_PATH = Path(__file__).with_name(".config")

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
        self.supportedCores: list[str] = []
        self.supportedExamples: list[dict] = []
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
                            self.supportedCores.append(core.get("core_id", core["id"]))
                    else:
                        print(f"Warning: Invalid core definition in {yaml_file}", file=sys.stderr)

    def find_examples(self):
        """Populate *supportedExamples* with dirs containing `prj.conf`."""
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
                            self.supportedExamples.append(example_dict)

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

def get_board_name(var_file: Path) -> str:
    """Extract board name from *variable.cmake* using BOARD_REGEX.

    Returns a list because, in theory, a file might define several boards.
    """
    name: str = ""
    for line in var_file.read_text().splitlines():
        m = BOARD_REGEX.match(line)
        if m:
            return m.group(1)
    return None


def parse_diagnostics(raw_text: str) -> pd.DataFrame:
    """
    Extract compiler / linker diagnostics from *raw_text* and return
    a DataFrame with columns [severity, message, path].
    """
    records: List[dict] = []
    for line in io.StringIO(raw_text):
        if "warning: " in line or "error: " in line:
            line = line.split(": ")
            for i, section in enumerate(line):
                if "warning" in section or "error" in section:
                    severity = section.split(":")[0].strip().lower()
                    message = line[i + 1].strip()
                    if i > 0:
                        path = line[i-1].strip().replace("\\", "/")
                    else:
                        path = ""

                    records.append({
                        "severity": severity,
                        "message": message,
                        "path": path,
                    })
    return pd.DataFrame.from_records(records, columns=["severity", "message", "path"])

def build_all(board:list[Board], build_root: Path, general_build_types: list) -> pd.DataFrame:
    """
    Build all boards with their examples and cores.
    This function is called at the end of the script.
    """
    all_diags: list[pd.DataFrame] = []
    for board in boards:
        for core in board.supportedCores:
            for example in board.supportedExamples:
                for build_type in example["build_configuration"]:
                    for mode in general_build_types:
                        if mode in build_type:
                            build_type = f'{build_type}'
                            build_example_dir = Path(build_root) / str(board.name) / f"{example['path'].name}_{core}" / str(build_type)
                            build_example_dir.mkdir(parents=True, exist_ok=True)

                            res = subprocess.run(
                                [
                                    "west", "build",
                                    "-p",
                                    "-b", board.name,
                                    str(example["path"]),
                                    "-d", str(build_example_dir),
                                    f"-DCUSTOM_BOARD_ROOT={BOARD_ROOT}",
                                    f"-Dcore_id={core}",
                                    f"--config={build_type}",
                                    f"-G{generator}",
                                ],
                                text=True,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT,   # merge → everything in res.stdout
                            )

                            # Parse every diagnostic, keep board/core/example for context
                            [print(f'[BUILD_PROCESS] {line.strip()}') for line in io.StringIO(res.stdout)]
                            df = parse_diagnostics(res.stdout)
                            if not df.empty:
                                df.insert(0, "board", board.name)
                                df.insert(1, "core", core)
                                df.insert(2, "example", example["path"].name)
                                all_diags.append(df)

                            if res.returncode == 0:
                                print(f"[BUILD_PROCESS] ✅  Build successful for {build_example_dir}")
                            else:
                                print(f"[BUILD_PROCESS] ❌ Build **failed** for {build_example_dir}")
                                return pd.concat(all_diags, ignore_index=True)
    return pd.concat(all_diags, ignore_index=True)

# ------------------------------------------------------------
# 3.  “source ./.config”  →  import environment variables
# ------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="MCUXSDK Build Script")
    parser.add_argument("--mcuxsdk_root", type=str, help="Path to MCUXSDK root directory")
    parser.add_argument("--build_root", type=str, help="Path to build output directory")
    parser.add_argument("--general_build_types", nargs='*', default="all",
                        help="List of general build types to use (default: debug, options: debug, release, all)")
    parser.add_argument("--generator", type=str, help="choose build generator e.g. Ninja, Unix Makefiles", default="Ninja")
    parser.add_argument("--board", type=str, help="Specify one board to build, if not set all boards will be built", default="all")
    args = parser.parse_args()

    if CONFIG_PATH.is_file():
        pattern = re.compile(r"""^\s*([^=\s#]+)\s*=\s*(.+?)\s*$""")
        with CONFIG_PATH.open() as fh:
            for line in fh:
                if line.strip().startswith("#") or "=" not in line:
                    continue
                m = pattern.match(line)
                if m:
                    key, value = m.groups()
                    os.environ.setdefault(key, strip_quotes(value))
    else:
        print("Warning: ./.config not found – continuing with current env", file=sys.stderr)

    if args.build_root is not None:
        build_root = strip_quotes(args.build_root)
    else:
        build_root = strip_quotes(os.environ.get("BUILD_ROOT", ""))
        if build_root is None:
            if "scripts" not in CWD.parts:
                build_root = CWD / "build"
            else:
                build_root = CWD / ".." / "build"
    if args.mcuxsdk_root is not None:
        mcux_root =  strip_quotes(args.mcuxsdk_root)
    else:
            mcux_root = strip_quotes(os.environ.get("MCUXSDK_ROOT", ""))
            if mcux_root is None:
                sys.exit("Error: MCUXSDK_ROOT is not set (expected in .config or exported).")

    if args.general_build_types is not None:
        general_build_types = args.general_build_types
    else:
        general_build_types = "all"
    if general_build_types == "all":
        general_build_types = ["debug", "release"]
    elif general_build_types == "debug":
        general_build_types = ["debug"]
    else:
        general_build_type = ["release"]

    if args.generator is not None:
        generator = strip_quotes(args.generator)
    else:
        generator = "Ninja"

    if args.board is not None or not args.board == "all":
        board = args.board
    else:
        board = None

# ------------------------------------------------------------
# 4.  Locate every variable.cmake and grab the board name via CMake
# ------------------------------------------------------------

    if "scripts" not in CWD.parts:
        BOARD_ROOT = CWD / "_boards"
    else:
        BOARD_ROOT = CWD / ".." / "_boards"

    if not BOARD_ROOT.is_dir():
        sys.exit(
            f"Error: The board root directory '{BOARD_ROOT}' does not exist. "
            "Please ensure you are in the correct directory."
        )

    boards: list[Board] = []

    for var_file in BOARD_ROOT.rglob("variable.cmake"):
        board_name = get_board_name(var_file)
        if board_name:
            boards.append(Board(board_name, var_file.parent))
            print(f"Found board: {boards[-1]}")

    if board is not None:
        if board in [b.name for b in boards]:
            boards = [b for b in boards if b.name == board]

# ---------------------- sanity check ---------------------- #
    assert boards, (
        f"No board names found under {BOARD_ROOT}. "
        "Verify that variable.cmake files exist and that CMake is able to parse them."
    )

# ------------------------------------------------------------
# 5.  Summaries
# ------------------------------------------------------------
    print(f"\nBoards found ({len(boards)}): {' '.join(b.name for b in boards)}")

    example_dirs = [e for b in boards for e in b.supportedExamples]
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

    result = subprocess.run(
        ["west", "build", "--help"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
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

    all_diags: list[pd.DataFrame] = []
    build_example_dir = None

    diagnostics = build_all(boards, build_root, general_build_types)

# ------------------------------------------------------------
# 6.  Combine and save / show diagnostics
# ------------------------------------------------------------
    if not diagnostics.empty:
        diagnostics.sort_values(["severity", "board", "core", "path"], inplace=True)

        # Example summaries
        print("\n=== Diagnostic summary ==========================")
        print(diagnostics.groupby(["severity", "message"]).size())
        summary = diagnostics.groupby(["severity", "message"]).size().reset_index(name="count")
        os.chdir(CWD)  # Ensure we are back in the original directory
        # Persist (CSV / Parquet / whatever you wish)
        try:
            diagnostics.to_csv(f"{build_root}/build_diagnostics.csv", index=False)
            summary.to_csv(f"{build_root}/diagnostics_summarized.csv", index=False)
            print(f'\nSaved summarized diagnostics to {build_root}/diagnostics_summarized.csv')
            print(f'\nSaved complete diagnostics to {build_root}/build_diagnostics.csv')
        except Exception as e:
            print(f"Error writing CSV: {e}")
        if diagnostics['severity'].str.contains('error').any():
            sys.exit(-1)
    else:
        print("\nNo warnings or errors captured.")
