#!/bin/bash -eu

. scripts/common_functions.sh

echo >&2 "Running Q2${2}..."

for inp in "$1"/*.q2; do
	out="${inp%.q2}.o2"
	timeout 10s "bin/Q2${2}" <"$inp" >"$out" || true

	if diff -w "$out" "${inp%.q2}.a2" &>/dev/null; then
		success "$inp"
	else
		failure "$inp"
	fi
done
