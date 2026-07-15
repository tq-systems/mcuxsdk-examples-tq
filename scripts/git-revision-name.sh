#!/bin/bash -
# SPDX-License-Identifier: GPL-2.0-or-later
#
# Copyright (C) 2014-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
# D-82229 Seefeld, Germany.
# Author: Markus Niebel
#
# Description: A utility to get a pretty string for
#              current revision of your project.
###############################################################################

set -e
set -C # noclobber

# TRAP SIGNALS
trap 'error_abort $LINENO' ERR

function error_abort () {
	echo "error at $1" >&2
}

function error_exit () {
	echo "ERROR: $1" >&2
	exit 1
}

# Internal variables and initializations.
readonly SCRIPTNAME="${0}"

usage() {
	cat <<END
Usage:

${SCRIPTNAME} [--help] [--no-verify] [git rev]

[-h|--help]: print this message
[--no-verify]: do not check if given git rev is at HEAD

Print a pretty version string of HEAD or git rev
The pretty version string can be either
 - the exact tag if tag was given as parameter.
 - the last tag matching the commit in HEAD if git rev is commit or
   branch equal to HEAD
 - <last reachable tag>-<commits since>-g<12 digits of SHA>
 - if no tag at HEAD or no tag can be found: 'git-g<<12 digits of SHA>''
END
}


 # Create a place to store our work's progress
function main () {
	local GIT_DESCRIPTION=""
	local PREFERRED_HEAD=""
	local PREFERRED_TAG=""
	local IS_GIT_TAG="0"
	local GITHEAD=""
	local GITREV=""
	local VERIFY="1"
	local STAMP="git-unknown"

	local pos_params=""

	while (( "$#" )); do
		case "$1" in
		-h|--help )
			usage
			return 0
			;;
		-n|--no-verify )
			VERIFY="0"
			shift
			;;
		--*=|-*) # unsupported flags
			usage
			error_exit "Unsupported flag $1"
			;;
		*) # preserve positional arguments
			pos_params="$pos_params $1"
			shift
			;;
		esac
	done

	# set positional arguments in their proper place
	eval set -- "$pos_params"
	if [ -n "${1}" ]; then
		GITREV="${1}"
	fi

	# commit hash for head
	GITHEAD="$(git rev-parse --verify HEAD)"

	# if an optional revision was given, check if it is
	# - a valid git object
	# - a tag or
	# - a branch or
	# - a commit
	# If not --no-verify given, check if this is equal to the checked out revision
	# if no parameter given, assume checked out head.
	# prepare tp determine the pretty version string on that base using PREFERRED_HEAD
	# and GIT_DESCRIPTION
	if [ -n "${GITREV}" ]; then
		local PREFERRED_HEAD_COMMIT=""
		local PREFERRED_TAG_COMMIT=""
		local PREFERRED_REV=""

		PREFERRED_REV="$(git rev-parse --verify "${GITREV}" 2>/dev/null || true)"
		# the supplied parameter is not a git object
		if [ -z "${PREFERRED_REV}" ]; then
			error_exit "${GITREV} is not a git object"
		fi

		# check if given parameter denotes a tag
		PREFERRED_TAG_COMMIT="$( (git show-ref --tags --dereference "${GITREV}" 2>/dev/null || true) | awk 'END { print $1 }')"
		# check if given parameter denotes a head
		PREFERRED_HEAD_COMMIT="$( (git show-ref --heads --dereference "${GITREV}" 2>/dev/null || true) | awk 'END { print $1 }')"

		if [ -n "${PREFERRED_TAG_COMMIT}" ]; then
			if [ "${VERIFY}" -ne "0" ] && [ "${PREFERRED_TAG_COMMIT}" != "${GITHEAD}" ]; then
				error_exit "tag ${GITREV} is not at HEAD"
			fi
			PREFERRED_TAG="${GITREV}"
			GITHEAD="${PREFERRED_TAG_COMMIT}"
			# <last reachable tag>-<commits since>-g<12 cipher short hash>
			GIT_DESCRIPTION=$(git describe --abbrev=12 "${PREFERRED_TAG}" 2>/dev/null || true)
		elif [ -n "${PREFERRED_HEAD_COMMIT}" ]; then
			if [ "${VERIFY}" -ne "0" ] && [ "${PREFERRED_HEAD_COMMIT}" != "${GITHEAD}" ]; then
				error_exit "head ${GITREV} is not at HEAD"
			fi
			PREFERRED_HEAD="${GITREV}"
			GITHEAD="${PREFERRED_HEAD_COMMIT}"
			# <last reachable tag>-<commits since>-g<12 cipher short hash>
			GIT_DESCRIPTION=$(git describe --abbrev=12 "${PREFERRED_HEAD}" 2>/dev/null || true)
		else
			if [ "${VERIFY}" -ne "0" ] && [ "${PREFERRED_REV}" != "${GITHEAD}" ]; then
				error_exit "commit ${GITREV} is not at HEAD"
			fi
			# <last reachable tag>-<commits since>-g<12 cipher short hash>
			GIT_DESCRIPTION=$(git describe --abbrev=12 "${PREFERRED_REV}" 2>/dev/null || true)
			GITHEAD="${PREFERRED_REV}"
		fi
	else
		# <last reachable tag>-<commits since>-g<12 cipher short hash>
		GIT_DESCRIPTION="$(git describe --abbrev=12 2>/dev/null || true)"
	fi

	# go further if no parameter was given or parameter is valid

	# a valid tag was supplied
	if [ -n "${PREFERRED_TAG}" ]; then
		GIT_DESCRIPTION="${PREFERRED_TAG}"
		IS_GIT_TAG="1"
	# there is a tag at the current HEAD
	elif git show-ref --quiet --tags "${GIT_DESCRIPTION}" 2>/dev/null; then
		IS_GIT_TAG="1"
	fi
	# OK, checked out version is tag given by parameter or if no parameter
	# there is a tag for the checked out commit
	if [ "${IS_GIT_TAG}" -gt "0" ]; then
		STAMP="${GIT_DESCRIPTION}"
	# There is a tag down the commit stack, use it and make a pretty version of the form
	# <last reachable tag>-<commits since>-g<12 cipher short hash> where <commits since> have
	# 5 digits always filled with zeros
	elif [ -n "${GIT_DESCRIPTION}" ]; then
		STAMP="$(echo "${GIT_DESCRIPTION}" | awk -F- '{ OFS = "-"; if (NF > 1) $(NF-1) = sprintf("%05d", $(NF-1)); print $0 }')"
	# no tag at all: first 12 digits of commit in strings 'git-g<SHA>'
	else
		STAMP="git-g${GITHEAD:0:12}"
	fi

	echo "${STAMP}"
}

main "$@"
