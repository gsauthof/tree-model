#!/usr/bin/bash

set -x
set -e
set -u

# clean the environment, we don't want that stuff like DBUS*, XDG_*, DESKTOP_SESSION etc. is leaked
[ -v HOME ] && exec -c "$0" "$@"

mkdir -p fake_home

export HOME=$PWD/fake_home

src_dir="$1"
build_dir="$2"

cd "$build_dir"

export TEST_IN="$src_dir"/test/in

r=0

./ut && true
: $((r+=$?))

export ASN1_PATH="$src_dir"/libgrammar/test/in/asn1:"$src_dir"/libxfsx/config

export DISPLAY=:1
# there are some false positives
# TODO, use a fine grained ignore file
export ASAN_OPTIONS=detect_leaks=false
# otherwise some tests fail
# perhaps because of different focus policy
export DESKTOP_SESSION=gnome

timeout 5m ./ut_select_open && true
: $((r+=$?))
timeout 5m ./ut_gui && true
: $((r+=$?))

exit $((r>127?127:r))
