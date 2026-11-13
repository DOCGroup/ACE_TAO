#!/bin/sh

set -eu

for s in 0 25 50 75 100; do
  for w in 1 2 3 4 5 6; do
    printf '%s\n' "================ WORK: $s" >&2
    printf '%s\n' "================ THREADS: $w" >&2
    ./Queue_Based_Workers/workers -m 100000 -b 100000 -s "$s" -w "$w"
    ./Leader_Follower/leader_follower -m 100000 -b 100000 -s "$s" -w "$w"
  done
done
