#!/bin/bash -e

mkdir -p build
cd build

cmake "$@" ..
cmake --build . -j"$(nproc)"
