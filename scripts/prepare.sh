#!/bin/bash
# ****************************************************************************
# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author:    Isaac Lucas de Lima Yuki
#
# Description: Script for setting up the mcuxsdk.
# ******************************************************************************

set -e
set -C # noclobber

trap 'error_abort $LINENO' ERR
function error_abort () {
	echo "error at $1"
}

SCRIPT="$(basename "${0}")"
PROJECT_PATH="$(dirname "$(readlink -f "$0")")/.."
readonly SCRIPT
readonly PROJECT_PATH
readonly MCUXSDK_DIR="mcuxsdk"

function usage () {
	cat <<- END
	Usage:

	${SCRIPT} [--force] [--help|-h]

	This script prepares the workspace for the MCUXSDK examples for TQ-Systems GmbH boards

	Options:
	  --force          force creation of .venv for SDK usage, not to be called from inside a .venv
	  --help, -h       Show this help message
	END
}

# Script for Automating the Build Process
main() {
	# Ensure this script is run from the project directory
	if [ -z "${MCUXSDK_ROOT}" ]; then
		MCUXSDK_ROOT="${PROJECT_PATH}/.."
		echo "-- MCUXSDK_ROOT is set to default: ${MCUXSDK_ROOT}"
	fi

	# Parse command-line options
	while test $# -gt 0; do
		case $1 in
			--force )
				if [ "${VIRTUAL_ENV}" ]; then
					echo "Error: called with '--force' inside virtual env"
					exit 1
				else
					rm -rf "${MCUXSDK_ROOT}/.venv"
				fi
				shift
				;;
			--help | -h )
				usage
				exit 0
				;;
			* )
				echo "Unrecognized option: $1" >&2
				usage
				exit 1
				;;
		esac
	done

	echo "-- Starting MCUXSDK setup..."

	if [ -n "${WEST}" ]; then
		echo "-- WEST is defined as: ${WEST}"
	else
		WEST=$(which west) || true
		if [ -z "${WEST}" ] || ! ${WEST} --version > /dev/null 2>&1; then
			echo "Error: 'west' command not found or not working. Please install west and ensure it is in your PATH."
			exit 1
		fi
	fi

	if [ -n "${PYTHON}" ]; then
		echo "-- PYTHON is defined as: ${PYTHON}"
	else
		PYTHON=$(which python3) || true
		if [ -z "${PYTHON}" ] || ! ${PYTHON} --version > /dev/null 2>&1; then
			echo "Error: Python3 is not installed or not found in PATH."
			exit 1
		fi
	fi

	if [ -n "${GENERATOR}" ]; then
		echo "-- GENERATOR is defined as: ${GENERATOR}"
	else
		GENERATOR="Ninja"
		echo "-- Ninja is set as default Generator"
		NINJA=$(which ninja) || true
		if [ -z "${NINJA}" ] || ! ${NINJA} --version > /dev/null 2>&1; then
			echo "Error: Ninja is not installed or not found in PATH."
			exit 1
		fi
	fi

	if [ -n "${ARMGCC_DIR}" ]; then
		echo "-- ARMGCC_DIR is defined as: ${ARMGCC_DIR}"
		if [ -x "${ARMGCC_DIR}/bin/arm-none-eabi-gcc" ]; then
			echo "-- ARM GCC toolchain found at: ${ARMGCC_DIR}/bin/arm-none-eabi-gcc"
		else
			echo "Error: ARM GCC toolchain not found at ${ARMGCC_DIR}/bin/arm-none-eabi-gcc."
			exit 1
		fi
	else
		echo "Error: ARMGCC_DIR is not defined. Please set the path to your ARM GCC toolchain."
		exit 1
	fi

	PYTHON_VERSION_REQUIRED_MAJOR=3
	PYTHON_VERSION_REQUIRED_MINOR=10

	PYTHON_VERSION=$(${PYTHON} -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')")
	PYTHON_VERSION_MAJOR=$(echo "$PYTHON_VERSION" | cut -d. -f1)
	PYTHON_VERSION_MINOR=$(echo "$PYTHON_VERSION" | cut -d. -f2)

	if [ "$PYTHON_VERSION_MAJOR" -lt "$PYTHON_VERSION_REQUIRED_MAJOR" ] || \
	{ [ "$PYTHON_VERSION_MAJOR" -eq "$PYTHON_VERSION_REQUIRED_MAJOR" ] && [ "$PYTHON_VERSION_MINOR" -lt "$PYTHON_VERSION_REQUIRED_MINOR" ]; }; then
		echo "Error: Python >= ${PYTHON_VERSION_REQUIRED_MAJOR}.${PYTHON_VERSION_REQUIRED_MINOR} required, found ${PYTHON_VERSION}"
		exit 1
	fi

	PIP=$(which pip || which pip3 || true)
	if [ -z "${PIP}" ] || ! ${PIP} --version >/dev/null 2>&1; then
		echo "Error: pip is not installed or not found in PATH."
		exit 1
	fi

	# --- pip version check (>= MAJOR.MINOR) ---
	PIP_VERSION_REQUIRED_MAJOR=22
	PIP_VERSION_REQUIRED_MINOR=0

	PIP_VERSION=$(${PIP} --version | awk '{print $2}')   # e.g., 24.2.1
	PIP_VERSION_MAJOR=$(echo "$PIP_VERSION" | cut -d. -f1)
	PIP_VERSION_MINOR=$(echo "$PIP_VERSION" | cut -d. -f2)

	if [ "$PIP_VERSION_MAJOR" -lt "$PIP_VERSION_REQUIRED_MAJOR" ] || \
	{ [ "$PIP_VERSION_MAJOR" -eq "$PIP_VERSION_REQUIRED_MAJOR" ] && [ "$PIP_VERSION_MINOR" -lt "$PIP_VERSION_REQUIRED_MINOR" ]; }; then
		echo "Error: pip >= ${PIP_VERSION_REQUIRED_MAJOR}.${PIP_VERSION_REQUIRED_MINOR} required, found ${PIP_VERSION}"
		exit 1
	fi

	if [ -d "${MCUXSDK_ROOT}/${MCUXSDK_DIR}" ]; then
		echo "-- Directory ${MCUXSDK_DIR} already exists. Skipping clone."
	else
		${WEST} init --local "${PROJECT_PATH}"
		${WEST} config commands.allow_extensions true
	fi

	${WEST} update

	if [ "${VIRTUAL_ENV}" ]; then
		echo "-- Virtual environment already activated."
	else
		if [ ! -d "${MCUXSDK_ROOT}/.venv" ]; then
			echo "-- Creating virtual environment..."
			${PYTHON} -m venv "${MCUXSDK_ROOT}/.venv"
		fi
		echo "-- Sourcing virtual environment..."
		. "${MCUXSDK_ROOT}/.venv/bin/activate"
		echo "$VIRTUAL_ENV"
		echo "-- Virtual environment created and activated."
	fi
	# pip should be also there in .venv but not as ${PIP}
	pip install -r "${MCUXSDK_ROOT}/${MCUXSDK_DIR}/scripts/requirements.txt" -c "${PROJECT_PATH}/scripts/constraints.txt"

	if [ -f "${PROJECT_PATH}/scripts/requirements.txt" ]; then
		pip install -r "${PROJECT_PATH}/scripts/requirements.txt"
	else
		echo "No requirements.txt found, skipping Python dependencies installation."
		exit 1
	fi

	deactivate

	echo "-- MCUXSDK setup completed successfully."

	exit 0

}

main "$@"
