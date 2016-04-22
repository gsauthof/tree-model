#!/bin/bash

set -eux


if [[ ! "$CMAKE_BUILD_TYPE" =~ "Coverage" ]]; then
  exit 0
fi


function run_osx()
{
  cd build
  bash <(curl -s https://codecov.io/bash) || echo "Codecov.io reporting failed"
}

function run_linux()
{
  docker exec cxx-runtime \
    env CI=$CI TRAVIS=$TRAVIS TRAVIS_BRANCH=$TRAVIS_BRANCH \
    TRAVIS_COMMIT=$TRAVIS_COMMIT TRAVIS_JOB_NUMBER=$TRAVIS_JOB_NUMBER \
    TRAVIS_PULL_REQUEST=$TRAVIS_PULL_REQUEST TRAVIS_JOB_ID=$TRAVIS_JOB_ID \
    TRAVIS_REPO_SLUG=$TRAVIS_REPO_SLUG \
    /srv/src/tree-model/ci/docker/upload_coverage.sh \
    /srv/src/tree-model \
    /srv/build/tree-model
}

run_$TRAVIS_OS_NAME
