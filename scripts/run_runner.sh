#!/bin/bash -eu

. scripts/common_functions.sh

run_tests "Q2$2" "$1" q2 '>' o2
check_results "$1" 2
