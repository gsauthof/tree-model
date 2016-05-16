#!/usr/bin/bash

set -eux

src_dir="$1"
build_dir="$2"

: ${run_tag:=""}

cd "$build_dir"

function upload_coverage()
{
  # out-of-src builds doesn't seem to be supported with
  # codecov.io as of 2016-05, thus trying in-src builds
  # for now
  bash <(curl -s https://codecov.io/bash)
}


function upload()
{
  # workaround coverage.cmake being identified as coverage report
  # workaround script only searching in the source directory for gcov reports
  #
  # cf. https://github.com/codecov/codecov-bash/pull/29
  bash <(curl -s https://codecov.io/bash \
    | sed '/-not -name .*coverage.txt/a -not -name '"'"'*.cmake'"'"' \\' \
    | sed 's/search_in="$search_in $git_root"/'\
'search_in="$search_in $proj_root"/' ) \
    \
    -R "$src_dir" -p "$build_dir" \
    \
    || echo "Codecov.io reporting failed"
}

function upload_lcov()
{
  # codecov.io report detection just yields too many false positives
  bash <(curl -s https://codecov.io/bash) -X gcov -f build-cov/lcov.info
}


upload"$run_tag"
