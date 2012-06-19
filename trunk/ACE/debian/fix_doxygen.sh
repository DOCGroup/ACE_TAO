#!/bin/sh
#
# This script regenerates changes on the Doxygen templates.
# $Id$
#
set -e
export QUILT_PATCHES=debian/patches

#quilt pop -a
#quilt delete reduce-doxygen-doc.diff
quilt new reduce-doxygen-doc.diff

for f in $(find $ACE_ROOT -name '*.doxygen')
do
    QUILT_PATCHES=debian/patches quilt add $f
    sed -e 's#COLLABORATION_GRAPH   = YES#COLLABORATION_GRAPH   = NO#' \
        -e 's#INCLUDE_GRAPH         = YES#INCLUDE_GRAPH         = NO#' \
        -e 's#INCLUDED_BY_GRAPH     = YES#INCLUDED_BY_GRAPH     = NO#' \
        -e 's#SEARCHENGINE          = YES#SEARCHENGINE          = NO#' \
        -e 's#DOT_MULTI_TARGETS     = YES#DOT_MULTI_TARGETS     = YES \
DOT_GRAPH_MAX_NODES   = 15 \
DOT_IMAGE_FORMAT      = png#' < $f > $f.new

    mv $f.new $f
done

quilt refresh --no-timestamps --no-index
