#!/bin/sh
#
# $Id$
#

for s in 0 1 2 3 4 5; do
  echo WORK: $s
  for w in 1 2 3 4 8; do
    echo THREADS: $w
    ./Queue_Based_Workers/workers -m 100000 -b 100000 -s $s -w $w
    ./Leader_Follower/leader_follower -m 100000 -b 100000 -s $s -w $w
  done
done
