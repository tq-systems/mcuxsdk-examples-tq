#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2025 TQ-Systems GmbH
# Author: Isaac L. L. Yuki
#
# Description: Create per-subdirectory archives from a given build directory.
#              Each archive contains:
#                - all files under that subdirectory (recursively), and
#                - all files in the top level of the build directory (duplicated into each archive).
#
# Examples:
#   cd "$GIT_DIR_TO_ARCHIVE"; create_archives.sh --build-dir ./build --format tar.gz -o ./dist
#
###############################################################################

set -e
set -C  # noclobber

# TRAP SIGNALS
trap 'cleanup' EXIT
trap 'error_abort $LINENO' ERR

# Use newline+tab (no splitting on spaces)
OLD_IFS=$IFS
IFS=$'\n\t'

# Internal variables and initializations.
readonly PROGRAM=$(basename "$0")
readonly VERSION=0.1

readonly PWD="$(pwd)"
OUT_PATH="$PWD"   # default output folder is current directory
VERBOSE=0
DRY_RUN=0

FORMAT=tar            # tar | tgz | tar.gz
PREFIX=""

# RETURN VALUES/EXIT STATUS CODES
readonly E_BAD_OPTION=254
readonly E_UNKNOWN=255

function cleanup () {
	rm -f ${TMPFILE}
	rm -f ${TOARCHIVE}
	IFS="${OLD_IFS}"
	return 0
}

function error_abort () {
	echo "error at $1"
	cleanup
}

function debug() {
	[ ${VERBOSE} -eq 1 ] && echo $@
	return 0
}

function error () {
	echo $1 >&2
}

function exit_error () {
	error "$2"
	exit $1
}

function usage () {
    echo "Usage: $PROGRAM [--format <tar|tgz>] [--prefix <prefix>] [--build-dir <build_dir>] [--verbose] [--dry-run]"

    echo "This script creates archive files for each object (directory or file) in the specified build directory."
    echo "It includes .bin, .elf, and .csv files in the archives."
    echo "Options:"
    echo "  --format <tar|tgz|zip>       Specify the archive format (default: tar)"
    echo "  --prefix <prefix>        Prefix to add to each file in the archive (default: none)"
    echo "  --build-dir <build_dir>  Specify the build directory (required)"
    echo "  --verbose, -v            Enable verbose output"
    echo "  --dry-run, -n            Perform a trial run with no changes made"
    echo "  --version                Show program version"
    echo "  --prefix <prefix>        Specify a prefix to add to each file in the archive (default: none)"
    echo "  --output-folder, -o      Specify the output folder for the archives relatively (default: current directory)"
    echo "  --help, --usage, -?      Show this help message"

    return 0
}

function version () {
    echo "${PROGRAM} version ${VERSION}"
    return 0
}

function rm_file () {
	if [ -f ${1} ]; then
		rm -f ${1}
	fi
}

function main () {
    # Process command-line arguments.
    while test $# -gt 0; do
        case $1 in
        --format )
            shift
            FORMAT="$1"
            shift
            ;;

        --prefix )
            shift
            PREFIX="$1"
            shift
            ;;

        --build-dir | -d )
            shift
            BUILD_DIR="$1"
            shift
            ;;

        --version )
            version
            exit
            ;;

        --verbose | -v )
            shift
            VERBOSE=1
            ;;

        --dry-run | -n )
            shift
            DRY_RUN=1
            ;;

        --prefix | -p )
            shift
            PREFIX="$1"
            shift
            ;;

        --output_path | -o )
            shift
            OUT_PATH="$1"
            shift
            ;;

        -? | --usage | --help )
            usage
            exit
            ;;

        -* )
            echo "Unrecognized option: $1" >&2
            usage
            exit $E_BAD_OPTION
            ;;

        * )
            break
            ;;
        esac
    done

    # Validate parameters; error early, error often.
    if [ -z "${BUILD_DIR}" ]; then
        echo "Error: BUILD_DIR is not set. Please specify the build directory."
        exit 1
    fi

    if [ ! -d "${BUILD_DIR}" ]; then
        echo "Error: BUILD_DIR (${BUILD_DIR}) does not exist."
        exit 1
    fi

    # Validate or create the OUTPUT_FOLDER
    if [ ! -d "${OUT_PATH}" ]; then
        debug "Output folder (${OUT_PATH}) does not exist. Creating it..."
        mkdir -p "${OUT_PATH}" || {
            echo "Error: Failed to create output folder (${OUT_PATH})."
            exit 1
        }
    fi

    readonly OUT_DIR=$(basename "${OUT_PATH}")

    files=($(find "${BUILD_DIR}" -maxdepth 1 -type f -name "*.*"))

    for object in "${BUILD_DIR}"/*; do
        object_name=$(basename "${object}")
        files_to_archive=()
        if [ -d "${object}" ] && [ "${object_name}" != "${OUT_DIR}" ]; then
            archive_name="${OUT_PATH}/${PREFIX}${object_name}.${FORMAT}"

            debug "Creating archive for ${object_name}..."

            # Find .bin and .elf files and add them to the archive
            files_to_archive=($(find "${object}" -type f \( -name "*.bin" -o -name "*.elf" \)))
            files_to_archive+=("${files[@]}")

            if [ ${#files_to_archive[@]} -eq 0 ]; then
                echo "No .bin, .elf, or .csv files found for ${object_name}. Skipping..."
                continue
            fi


            if [ ${DRY_RUN} -eq 1 ]; then
                echo "Dry run mode: would create archive ${archive_name} with files:"
                echo ${files_to_archive[@]}
                continue
            
            else
                debug ${files_to_archive[@]}
                debug "prefix: ${PREFIX}"
                if [ -n "${PREFIX}" ]; then
                    # ensure single trailing slash inside the archive
                    PFX="${PREFIX%/}/"
                fi
                # Create the archive
                if ! tar --create --file="${archive_name}" --auto-compress --transform="s|^${PFX}/||" "${files_to_archive[@]}"; then
                    echo "Error: Failed to create archive for ${object_name}."
                    exit 1
                fi
                echo "Archive created: ${archive_name}"
            fi
        fi
    done

    echo "All archives created successfully in ${PWD}/${OUT_DIR}"

    return 0
}

main "$@"
