#!/bin/bash
# ****************************************************************************
# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright (c) 2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author:    Isaac Lucas de Lima Yuki
#
# Description: Script for setting up the build system and building targets.
# ******************************************************************************

set -e
set -C # noclobber

trap 'error_abort $LINENO' ERR
function error_abort () {
	echo "error at $1"
}

SCRIPT="$(basename "${0}")"
PROJECT_PATH="$(dirname "$(readlink -f "$0")")/.."
SCRIPT_PATH="$(dirname "$(readlink -f "${0}")")"
readonly SCRIPT
readonly PROJECT_PATH
readonly SCRIPT_PATH
readonly MCUXSDK_DIR="mcuxsdk"

main() {
	local BOARD=all
	local BUILD_DIR=build

	# Parse command-line options
	while test $# -gt 0; do
		case $1 in
			--board )
				shift
				BOARD="${1}"
				shift
				;;
			--build-dir )
				shift
				BUILD_DIR="${1}"
				shift
				;;
			--help | -h )
				echo "Usage: $SCRIPT [-board <board|all>]"
				exit 0
				;;
			-* | * )
				echo "Unrecognized option: $1" >&2
				usage
				exit 1
				;;
		esac
	done

	# Ensure this script is run from the project directory
	if [ -z "${MCUXSDK_ROOT}" ]; then
		MCUXSDK_ROOT="${PROJECT_PATH}/.."
		echo "-- MCUXSDK_ROOT is set to default: ${MCUXSDK_ROOT}"
	fi

	if [ -n "${WEST}" ]; then
		echo "-- WEST is defined as: ${WEST}"
	else
		WEST=$(which west) || true
		if [ -z "${WEST}" ] || ! ${WEST} --version > /dev/null 2>&1; then
			echo "Error: 'west' command not found or not working. Please install west and ensure it is in your PATH."
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

	echo "-- Starting Build Setup..."
	echo "-- Project Path: ${PROJECT_PATH}"
	echo "-- Script Path: ${SCRIPT_PATH}"
	echo "-- MCUXSDK_ROOT: $MCUXSDK_ROOT"
	echo "-- MCUXSDK_DIR: $MCUXSDK_DIR"

	cd "${SCRIPT_PATH}"

	echo "-- Creating virtual environment..."

	# start virtual environment
	if [ -d "${MCUXSDK_ROOT}/.venv" ]; then
		source "${MCUXSDK_ROOT}/.venv/bin/activate"
		echo "$VIRTUAL_ENV"
	else
		echo "Error: no venv found."
		exit 1
	fi

	echo "-- Virtual environment created and activated."

	python "${SCRIPT_PATH}/build_all.py" \
		--mcuxsdk_root "${MCUXSDK_ROOT}/${MCUXSDK_DIR}" \
		--build_root "${PROJECT_PATH}/${BUILD_DIR}" \
		--generator "${GENERATOR}" \
		--board "${BOARD}"

	echo "Build completed ..."

	return 0
}

main "$@"
