#!/bin/bash

set -eux

build=/var/tmp/build

mkdir -p $build

docker create --name cxx-devel \
  -v $PWD:/srv/src:ro,Z \
  -v $build:/srv/build:Z \
  gsauthof/fedora-cxx-devel:23

docker start cxx-devel

sleep 4

docker ps

