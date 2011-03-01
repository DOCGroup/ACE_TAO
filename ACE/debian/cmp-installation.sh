#!/bin/sh
# $Id$
#
# This script compares autoconf-based installation against regular builds
# provided files.
#
# This script should be run after `make install DESTDIR=debian/tmp' has been
# run on autoconf-based build.
#

set -e

P1=ACE_wrappers
P2=debian/tmp/usr/include

# Find files
# ff <from>
ff() {
  find "$1" -name '*.h' -o -name '*.inl' -o -name '*.hpp' -o -name '*.ipp'
}

# Remove path with examples, tests and tutorial from STDIN
drop() {
  grep -E -v '(examples|tests|tutorials)/' 
}

# Removed unused/unshipped stuff
unused() {
  grep -E -v '^/(ASNMP|CIAO|TAO_IDL|ace/QoS|apps|contrib|utils)'
}

F1=`mktemp -t ace_regular.XXXXXXXXXX`
F2=`mktemp -t ace_autoconf.XXXXXXXXXX`
trap "rm $F1 $F2" 0

ff $P1 | sed -e "s@$P1@@" -e "s@/TAO@@" -e "s@/protocols@@" -e "s@^/orbsvcs@@" | drop | unused | sort >> $F1
ff $P2 | sed -e "s@$P2@@" | sort >> $F2
diff -u $F1 $F2
