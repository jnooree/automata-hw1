#!/bin/bash

. scripts/common_functions.sh

echo >&2 "Running builder..."

for inp in test/*.q1; do
	if "bin/builder${1}" <"$inp" >>"${inp%.q1}.q2"; then
		success "$inp"
	else
		failure "$inp"
	fi
done
