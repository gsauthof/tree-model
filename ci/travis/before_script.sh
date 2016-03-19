#!/bin/bash

set -eux

build=/var/tmp/build

docker stop cxx-devel

docker create --name cxx-runtime \
  -v $PWD/src:/srv/src:ro,Z \
  -v $build:/srv/build:Z \
  gsauthof/fedora-cxx-runtime:23

docker start cxx-runtime

sleep 4

docker ps

