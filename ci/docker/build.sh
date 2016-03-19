#!/usr/bin/bash

set -x
set -e
set -u

: ${cmake_build_type:=Sanitize}
# -j1 : be nice about CI environments with low memory ...
: ${jobs:=3}
: ${targets:=ut ut_select_open ut_gui main}


src_dir="$1"
build_dir="$2"

mkdir -p "$build_dir"
cd "$build_dir"

cmake -G Ninja -D CMAKE_BUILD_TYPE="$cmake_build_type" "$src_dir"
ninja-build -j"$jobs" $targets
