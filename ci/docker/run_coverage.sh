#!/bin/bash

set -eux

src_dir="$1"
build_dir="$2"

# for coverage, we are doing in-source-builds, but still, the run
# script just needs to access test data from the src dir
exec "$build_dir/ci/docker/run.sh" "$@"
