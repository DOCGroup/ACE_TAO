#!/bin/sh
#
# $Id$
#

for s in 0 1 5 10 15 20 25; do
  echo ================ WORK: $s 1>&2
  for w in 1 2 ; do
    echo ================ THREADS: $w 1>&2
    ./Queue_Based_Workers/workers -m 100000 -b 100000 -s $s -w $w
    ./Leader_Follower/leader_follower -m 100000 -b 100000 -s $s -w $w
  done
done
