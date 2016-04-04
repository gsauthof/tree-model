#!/usr/bin/bash

set -eux


src_dir="$1"
build_dir="$2"
deploy_dir="$build_dir"/deploy
temp_dir="$build_dir"/temp
# how to build such a list:
# http://stackoverflow.com/questions/29678352/how-to-use-qts-windeployqt-in-linux-fedora/36388450#36388450
dll_lst="$src_dir"/ci/mingw/dll.lst

cd "$build_dir"


mkdir -p "$deploy_dir"

function deploy_binaries()
{
  # note that cmake puts the binaries/libraries into
  # CMAKE_RUNTIME_OUTPUT_DIRECTORY which might be different for
  # each target/subproject. For example, it defaults to
  # CMAKE_CURRENT_BINARY_DIR and cppformat set it to:
  # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  #
  # TODO probably makes sense to set it in each CMakeLists.txt

  test -f "$dll_lst"
  mkdir -p "$deploy_dir"/{imageformats,platforms}
  for i in imageformats platforms ; do
    grep "/plugins/$i" "$dll_lst" | xargs -r cp -t "$deploy_dir"/$i
  done
  grep -v '/plugins/' "$dll_lst" | xargs -r cp -t "$deploy_dir"
}

function deploy_config()
{
  mkdir "$deploy_dir"/config
  cp "$src_dir"/libgrammar/test/in/asn1/tap_3_12_strip.asn1 \
     "$src_dir"/libgrammar/grammar/xml/*.zsv \
     "$src_dir"/libxfsx/config/detector.json \
     "$deploy_dir"/config
  mv "$deploy_dir"/config/tap_3_12_strip.asn1 "$deploy_dir"/config/tap_3_12.asn1
}

function deploy_examples()
{
  mkdir "$deploy_dir"/example
  cp "$src_dir"/libxfsx/test/in/tap_3_12_valid.ber \
     "$src_dir"/test/in/tap_3_12_small.xml \
     "$deploy_dir"/example
}

deploy_binaries
deploy_config
deploy_examples

rsync -r "$deploy_dir"/ "$temp_dir"

cd "$temp_dir"

r=0

mkdir home

env -i TEST_IN="$src_dir"/test/in \
       ASN1_PATH="$PWD"/config \
      HOME="$PWD"/home \
      TERM=vt100 \
      wineconsole --backend=curses ./ut && true
: $((r+=$?))

exit $((r>127?127:r))

