#!/bin/bash

set -eux

src_dir="$1"
build_dir="$2"

"$build_dir/ci/docker/run.sh" "$src_dir" "$build_dir/build-cov"

cd "$build_dir"/build-cov


lcov --capture --directory ./ --output-file lcov_raw.info
lcov --remove lcov_raw.info \
  '/c++/*' \
  '/qt5/*' \
  'Catch/*' \
  'QtAwesome/*' \
  'boost/*' \
  'build-cov/*' \
  'cppformat/*' \
  'libgrammar/*' \
  'libixxx*' \
  'libxfsx/*' \
  'libxxxml/*' \
  -o lcov.info

# 'boost/*' '/c++/*' 'build-cov/*' 'libgrammar/*' 'cppformat/*' 'libixxx*' 'libxxxml/*' 'libxfsx/*' '/qt5/*' 'Catch/*' 'QtAwesome/*' 
