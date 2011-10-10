#!/bin/sh
# $Id$

usage() {
	cat >&2 << EOF
usage: $0 <first> <second>
compares ACE config.h files (using cpp)
e.g.: $0 ACE_wrappers/ace/config-linux.h objdir/ace/config.h
EOF
}

report_to() {
    g++ -E -DACE_HAS_LINUX_NPTL -dM -I"$ACE_ROOT" $1 | \
      grep '^#define ACE' | grep -v '^#define ACE_CONFIG' | \
      sed -e 's/^#define \+\([A-Z0-9_]\+\) *$/#define \1 1/g' | \
      sort >> $2
}

if test "a$ACE_ROOT" = "a"; then
    echo "$0: please define ACE_ROOT" >&2
    exit 1
fi

if test "$#" -ne 2; then
    usage
    exit 2
fi

ONE_TMP=`mktemp -t ace_cmp_one.XXXXXXXXXX` || exit 1
TWO_TMP=`mktemp -t ace_cmp_two.XXXXXXXXXX` || exit 1

trap "rm $ONE_TMP $TWO_TMP" EXIT

report_to $1 $ONE_TMP
report_to $2 $TWO_TMP

diff -u $ONE_TMP $TWO_TMP

