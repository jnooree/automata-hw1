# shellcheck shell=bash

if ! command tput init &>/dev/null; then
	function tput() {
		return
	}
fi

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

if [[ -z "${2-}" ]]; then
	function with_timeout() {
		timeout 10s "$@"
	}
else
	function with_timeout() {
		"$@"
	}
fi

function run_tests() {
	SHELL="$(type -p bash)"
	export SHELL
	export -f with_timeout

	echo >&2 "Running $1..."
	parallel -j"$(nproc)" --bar \
		with_timeout "bin/$1" '<{}' "${4}{.}.${5}" ::: "$2"/*."${3}"
}

function check_results() {
	local total=0 ok=0 nok=0

	for out in "$1"/*.o"${2}"; do
		total=$((total + 1))
		if diff -w "$out" "${out%.o"${2}"}.a${2}" &>/dev/null; then
			ok=$((ok + 1))
		else
			nok=$((nok + 1))
			failure "$out"
		fi
	done

	echo >&2 "Q${2}: $ok/$total tests passed."
	return "$nok"
}
