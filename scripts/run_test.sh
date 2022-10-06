#!/bin/bash -eu

template="${1-test/test.in}"
suffix="${2-.debug}"

testdir="$(dirname "$template")"
workdir="$(dirname "$template")/generated"

rm -rf "$workdir"
scripts/generate_test.py "$template"
cp -t "$workdir" "$testdir"/*.{q,a}*

scripts/run_builder.sh "$workdir" "$suffix"
scripts/run_runner.sh "$workdir" "$suffix"
