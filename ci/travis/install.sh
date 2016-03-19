#!/usr/bin/bash

set -eux

docker exec cxx-devel \
  /srv/src/tree-model/ci/docker/build.sh \
  /srv/src/tree-model \
  /srv/build/tree-model

