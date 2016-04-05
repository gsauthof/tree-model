#!/bin/bash

set -eux

build="$HOME"/build/build/tree-model
work_dir="$HOME"/deploy
: ${run_tag:=""}

function list_sums()
{
  find "$1" -type f -print0 | xargs -0 sha256sum  | sort -k2
}

function mk_version()
{
  date --utc '+%Y.%m.%d.%H.%M' | sed 's/\.0/./g'
}
version=$(mk_version)
# the path already encodes the version (and arch), thus, no need
# to encode it in the filename as well
# but we do it anyway to automatically get unique file names
# on 'save-as-is' downloads
archive=tree-model_w64-$version.zip

# cf. https://docs.travis-ci.com/user/deployment/bintray#Descriptor-file-example
#
# There are basically 2 choices when supporting multiple architectures
# like windows 32/64 bit etc.:
# 1. create one repository for each architecture
# 2. just create one repository and encode the architecture in the
#   uploadPattern - e.g. as base directory
#
# Currently, we implement the first one.
function mk_descriptor()
{
  cat > "$1" <<EOF
  {
    "package": {
      "name": "edtior",
      "repo": "tree-model-w64",
      "subject": "gsauthof",
      "licenses": ["LGPL-3.0"],
      "vcs_url": "https://github.com/gsauthof/tree-model.git",
      "website_url": "https://github.com/gsauthof/tree-model",
      "public_download_numbers": true,
      "public_stats": false
    }
    "version": {
      "name": "$version",
      "desc": "tree-model version build on travis ci",
      "released": "$(date -I)",
      "vcs_tag": "master",
      "gpgSign": true
    },

    "files": [
      { "includePattern": "$work_dir/(.*\\\\.zip)",
        "uploadPattern": "\$1" }
    ],

    "publish": true
  }
EOF
#      { "includePattern": "build/bin(.*)*/(.*\\\\.gem)",
#        "excludePattern": ".*/do-not-deploy/.*",
#        "uploadPattern": "gems/\$2" },
#      { "includePattern": "build/docs/(.*)",
#        "uploadPattern": "docs/\$1"}
}

function prepare_mingw()
{
  mkdir -p "$work_dir"
  cd "$work_dir"

  rsync -r "$build"/deploy/ tree-model/
  list_sums tree-model
  zip -r9 "$archive" tree-model
  sha256sum "$archive"
  mk_descriptor bintray_w64.json
}

function prepare()
{
  :
}

prepare"$run_tag"
