#!/bin/bash -eu

scripts/cleanup.sh test &>/dev/null

scripts/parenthesize.py test/build.in
scripts/prepare_test.py test/test.input.in test/test.output.in

suffix="${1:-}"
scripts/run_builder.sh "$suffix"
scripts/run_runner.sh "$suffix"
