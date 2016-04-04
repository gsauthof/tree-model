#!/bin/bash

set -eux

docker exec cxx-devel \
  env CMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" \
  /srv/src/tree-model/ci/docker/build.sh \
  /srv/src/tree-model \
  /srv/build/tree-model

