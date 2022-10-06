#!/bin/bash -eu

cd "$1"

ctest -C Debug -T test
lcov --rc lcov_branch_coverage=1 --capture --directory . \
	--output-file coverage.info
lcov --rc lcov_branch_coverage=1 --remove coverage.info '/usr/*' \
	--output-file coverage.info
lcov --rc lcov_branch_coverage=1 --list coverage.info

mv coverage.info ..
