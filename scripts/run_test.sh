#!/bin/bash -eu

template="${1-test/test.in}"
suffix="${2-.debug}"

scripts/cleanup.sh "$(dirname "$template")" &>/dev/null
scripts/preprocess.py "$template"

scripts/run_builder.sh "$suffix"
scripts/run_runner.sh "$suffix"
