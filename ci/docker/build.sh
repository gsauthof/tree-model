#!/usr/bin/bash

set -x
set -e
set -u

: ${CMAKE_BUILD_TYPE:=Sanitize}
# -j1 : be nice about CI environments with low memory ...
: ${jobs:=3}
: ${targets:=ut ut_select_open ut_gui main}
: ${run_tag:=""}

src_dir="$1"
build_dir="$2"

# already created by before_install.sh
#mkdir -p "$build_dir"
cd "$build_dir"


function build()
{
  cmake -G Ninja -D CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" "$src_dir"
  ninja-build -j"$jobs" $targets
}

function build_mingw()
{
  mingw64-cmake -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" "$src_dir"
  mingw64-make -j"$jobs" $targets
}

build"$run_tag"
