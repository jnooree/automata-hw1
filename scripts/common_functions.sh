# shellcheck shell=bash

function _do_announce() {
	tput setaf "$1"
	echo >&2 "[$2] $(basename "$3")"
	tput sgr0
}

function failure() {
	_do_announce 1 FAILURE "$1"
}

function success() {
	_do_announce 2 SUCCESS "$1"
}
