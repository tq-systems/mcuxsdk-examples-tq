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
trap 'error_abort $LINENO' ERR

# Internal variables and initializations.
readonly PROGRAM=$(basename "$0")
readonly VERSION=0.1

VERBOSE=0

# RETURN VALUES/EXIT STATUS CODES
readonly E_BAD_OPTION=254
readonly E_UNKNOWN=255

function error_abort () {
	echo "error at $1"
}

function debug() {
	[ ${VERBOSE} -eq 1 ] && echo "$@"
	return 0
}

function error () {
	echo "$1" >&2
}

function exit_error () {
	error "$2"
	exit "$1"
}

function usage () {
	cat <<- END
	Usage:

	${PROGRAM} [--format <tar|tgz>] [--prefix <prefix>] [--build-dir <build_dir>] [--verbose] [--dry-run]

	This script creates archive files for each object (directory or file) in the specified build directory.
	It includes .bin and .elf files in the archives.

	Options:
	  --format <tar|tgz|zip>   Specify the archive format (default: tar)
	  --prefix <prefix>        Prefix to add to each file in the archive (default: none)
	  --build-dir <build_dir>  Specify the build directory (required)
	  --verbose, -v            Enable verbose output
	  --dry-run, -n            Perform a trial run with no changes made
	  --version                Show program version
	  --output-folder, -o      Specify the output folder for the archives relatively (default: current directory)
	  --help, --usage, -?      Show this help message

	END
}

function version () {
    echo "${PROGRAM} version ${VERSION}"
    return 0
}

function main () {
    PWD="$(pwd)"
    local -r PWD
    # default output folder is current directory
    local OUT_PATH="${PWD}"
    local DRY_RUN=0
    local FORMAT=tar
    local PREFIX=""
    local BUILD_DIR=""

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
        exit_error "1" "Error: BUILD_DIR is not set. Please specify the build directory." 
    fi

    if [ ! -d "${BUILD_DIR}" ]; then
        exit_error "1" "Error: BUILD_DIR (${BUILD_DIR}) does not exist."
    fi

    # Validate or create the OUTPUT_FOLDER
    if [ ! -d "${OUT_PATH}" ]; then
        debug "Output folder (${OUT_PATH}) does not exist. Creating it..."
        mkdir -p "${OUT_PATH}" || {
            exit_error "1" "Error: Failed to create output folder (${OUT_PATH})."
        }
    fi

    OUT_DIR=$(basename "${OUT_PATH}")
    local -r OUT_DIR

    for object in "${BUILD_DIR}"/*; do
        object_name=$(basename "${object}")
        files_to_archive=()
        if [ -d "${object}" ] && [ "${object_name}" != "${OUT_DIR}" ]; then
            archive_name="${OUT_PATH}/${PREFIX}${object_name}.${FORMAT}"

            debug "Creating archive for ${object_name}..."

            # Find .bin and .elf files and add them to the archive
            files_to_archive=($(find "${object}" -type f \( -name "*.bin" -o -name "*.elf" \)))
            if [ ${#files_to_archive[@]} -eq 0 ]; then
                echo "No .bin or .elf files found for ${object_name}. Skipping..."
                continue
            fi

            if [ ${DRY_RUN} -eq 1 ]; then
                echo "Dry run mode: would create archive ${archive_name} with files:"
                echo "${files_to_archive[@]}"
                continue
            
            else
                debug "${files_to_archive[@]}"
                debug "prefix: ${PREFIX}"
                if [ -n "${PREFIX}" ]; then
                    # ensure single trailing slash inside the archive
                    PFX="${PREFIX%/}/"
                fi
                # Create the archive
                if ! tar --create --file="${archive_name}" --auto-compress --transform="s|^${PFX}/||" "${files_to_archive[@]}"; then
                    exit_error "1" "Error: Failed to create archive for ${object_name}."
                fi
                echo "Archive created: ${archive_name}"
            fi
        fi
    done

    echo "All archives created successfully in ${PWD}/${OUT_DIR}"

    return 0
}

main "$@"
