#!/bin/bash

set -eux

: ${run_tag:=""}

docker exec cxx-runtime \
  /srv/src/tree-model/ci/docker/run"$run_tag".sh \
  /srv/src/tree-model \
  /srv/build/tree-model

