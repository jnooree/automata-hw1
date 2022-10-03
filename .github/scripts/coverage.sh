#!/bin/bash -eu

cd "$1"

ctest -C Debug -T test
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
lcov --list coverage.info

mv coverage.info ..
