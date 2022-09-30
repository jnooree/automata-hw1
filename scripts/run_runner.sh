#!/bin/bash

. scripts/common_functions.sh

echo >&2 "Running runner..."

for inp in test/*.q2; do
	out="${inp%.q2}.o2"
	"bin/Q2${1}" <"$inp" >"$out"

	if diff --ignore-all-space "$out" "${inp%.q2}.a2" &>/dev/null; then
		success "$inp"
	else
		failure "$inp"
	fi
done
