#!/bin/bash

set -eux

docker exec cxx-runtime \
  /srv/src/tree-model/ci/docker/run.sh \
  /srv/src/tree-model \
  /srv/build/tree-model

