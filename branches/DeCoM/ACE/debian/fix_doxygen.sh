#!/bin/sh
#
# This script regenerates changes on the Doxygen templates.
#

rm -Rf ACE_wrappers.orig
cp -a ACE_wrappers ACE_wrappers.orig

for f in $(find ACE_wrappers -name '*.doxygen')
do
    sed -e 's#GENERATE_HTMLHELP     = YES#GENERATE_HTMLHELP     = NO#' \
        -e 's#COLLABORATION_GRAPH   = YES#COLLABORATION_GRAPH   = NO#' \
        -e 's#INCLUDE_GRAPH         = YES#INCLUDE_GRAPH         = NO#' \
        -e 's#INCLUDED_BY_GRAPH     = YES#INCLUDED_BY_GRAPH     = NO#' \
        -e 's#SEARCHENGINE          = YES#SEARCHENGINE          = NO#' \
        -e 's#DOT_MULTI_TARGETS     = YES#DOT_MULTI_TARGETS     = YES \
DOT_GRAPH_MAX_NODES   = 15 \
DOT_IMAGE_FORMAT      = png#' < $f > $f.new

    mv $f.new $f
done

cat > debian/patches/49-reduce-doxygen-doc.dpatch <<EOF
#! /bin/sh /usr/share/dpatch/dpatch-run
## 49-reduce-doxygen-doc.dpatch by Marek Brudka <mbrudka@aster.pl>
##
## DP: Enforce doxygen to generate smaller doxumentation

@DPATCH@
EOF

diff -Nru ACE_wrappers.orig ACE_wrappers >> debian/patches/49-reduce-doxygen-doc.dpatch
