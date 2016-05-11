#!/bin/bash

set -eux

export docker_img_a=gsauthof/fedora-cxx-devel:23
export docker_img_b=gsauthof/fedora-cxx-runtime:23
export run_tag="_coverage"
export CMAKE_BUILD_TYPE=Coverage

target="$1"


targets="before_install install before_script script"

for i in $targets; do

  ./ci/travis/$i.sh

  if [[ $i == $target ]]; then
    break
  fi
done
