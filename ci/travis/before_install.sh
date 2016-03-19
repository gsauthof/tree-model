#!/bin/bash

set -eux

build=/var/tmp/build
src="$PWD"/..

mkdir -p $build

# Travis' Ubuntu probably doesn't come with SELinux enabled, but
# shouldn't hurt
docker create --name cxx-devel \
  -v "$src":/srv/src:ro,Z \
  -v "$build":/srv/build:Z \
  gsauthof/fedora-cxx-devel:23

docker start cxx-devel

sleep 4

docker ps

