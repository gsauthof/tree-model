#!/bin/bash

set -eux

build="$HOME"/build/tree-model
src="$PWD"/..

docker stop cxx-devel

docker create --name cxx-runtime \
  -v "$src":/srv/src:ro,Z \
  -v "$build":/srv/build:Z \
  "$docker_img_b"

docker start cxx-runtime

sleep 4

docker ps

