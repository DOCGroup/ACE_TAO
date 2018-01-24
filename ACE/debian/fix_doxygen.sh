#!/bin/sh
#
# This script regenerates changes on the Doxygen templates.
#
silent_nothing_to_do() {
    test $? -eq 2 || false
}

set -e
export QUILT_PATCHES=debian/patches

quilt pop -a || silent_nothing_to_do
quilt delete reduce-doxygen-doc.diff
quilt new reduce-doxygen-doc.diff

for f in $(find $ACE_ROOT -name '*.doxygen')
do
    QUILT_PATCHES=debian/patches quilt add $f
    sed -e 's#COLLABORATION_GRAPH    = YES#COLLABORATION_GRAPH    = NO#' \
        -e 's#SEARCHENGINE           = YES#SEARCHENGINE           = NO#' \
        -e 's#DOT_GRAPH_MAX_NODES    = 50#DOT_GRAPH_MAX_NODES     = 15#' \
< $f > $f.new

    mv $f.new $f
done

quilt refresh --no-timestamps --no-index
