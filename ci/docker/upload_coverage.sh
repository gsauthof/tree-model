#!/usr/bin/bash

set -eux

src_dir="$1"
build_dir="$2"

cd "$build_dir"

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
