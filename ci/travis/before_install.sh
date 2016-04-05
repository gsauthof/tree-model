#!/bin/bash

set -eux

build="$HOME"/build/tree-model/..
src="$PWD"/..

mkdir -p $build
chmod 770 "$HOME"/build
# The compile-user inside the docker image has uid 1000, where ubuntu,
# by default uses 1001 - thus, we have to make it world-readable.
# Because of that it is put into a 770 parent
chmod 777 "$build"

# Travis' Ubuntu probably doesn't come with SELinux enabled, but
# shouldn't hurt
docker create --name cxx-devel \
  -v "$src":/srv/src:ro,Z \
  -v "$build":/srv/build:Z \
  "$docker_img_a"

docker start cxx-devel

sleep 4

docker ps

