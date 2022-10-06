#!/bin/bash -eu

. scripts/common_functions.sh

run_tests "Q1$2" "$1" q1 '>>' q2
