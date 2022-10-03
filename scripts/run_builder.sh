#!/bin/bash -eu

. scripts/common_functions.sh

echo >&2 "Running Q1${2}..."

for inp in "$1"/*.q1; do
	if timeout 10s "bin/Q1${2}" <"$inp" >>"${inp%.q1}.q2"; then
		success "$inp"
	else
		failure "$inp"
	fi
done
