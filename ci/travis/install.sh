#!/bin/bash

set -eux

: ${run_tag:=""}

docker exec cxx-devel \
  env \
  CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
  run_tag="$run_tag" \
  /srv/src/tree-model/ci/docker/build.sh \
  /srv/src/tree-model \
  /srv/build/tree-model

