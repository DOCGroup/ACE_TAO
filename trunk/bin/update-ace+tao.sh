#!/bin/sh
# Update ACE/TAO source tree to the latest numeric-assigned version.
# $Id$

# Define helper function to extract version number into tag format
d='\([0-9]*\)'; version() sed -ne "s,.*$1 version $d\.$d\.$d.*,$1-\1_\2_\3,p" \
                               -e "s,.*$1 version $d\.$d.*,$1-\1_\2,p"

# If ccvs found, use it instead of plain cvs since it knows about SOCKS5.
if [ ! -z `which ccvs` ]; then cvs () ccvs $*; fi

old_ace_version=`version <VERSION ACE`
old_tao_version=`version <TAO/VERSION TAO`

# Abort with message if no values in variables
if [ -z $old_ace_version ]; then echo No existing ACE version; exit 1; fi
if [ -z $old_tao_version ]; then echo No existing TAO version; exit 1; fi

cvs update -A VERSION TAO/VERSION

ace_version=`version <VERSION ACE`
tao_version=`version <TAO/VERSION TAO`

# Abort with message if no values in variables
if [ -z $ace_version ]; then echo No ACE version after update; exit 1; fi
if [ -z $tao_version ]; then echo No TAO version after update; exit 1; fi

echo Old software version tags: $old_ace_version $old_tao_version 
echo New software version tags: $ace_version $tao_version 

# Conserve net bandwidth if no change was observed
if [ $old_ace_version != $ace_version ]; then
  cvs -q update -Pd -r $ace_version `ls|egrep -v '^(\.|\.\.|TAO|CVS)$'`
fi
if [ $old_tao_version != $tao_version ]; then
  cvs -q update -Pd -r $tao_version TAO
fi

