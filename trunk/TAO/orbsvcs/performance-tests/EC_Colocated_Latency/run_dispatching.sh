#! /bin/sh
#
# $Id$
#

LOW_PRIORITY="1 5 10 15 20 25 30 35"

for c in $LOW_PRIORITY; do
  date
  echo $c
  ./driver -ORBSvcConf ec.dispatching_reactive.conf   -d -h 2000 -l 1000 -v 500 -w 100 -i 20000 -n $c > ec_dispatching_reactive.$c.txt 2>&1
  ./driver -ORBSvcConf ec.dispatching_threaded.conf   -d -h 2000 -l 1000 -v 500 -w 100 -i 20000 -n $c > ec_dispatching_threaded.$c.txt 2>&1
  ./driver -ORBSvcConf ec.dispatching_rtcorba.conf -r -d -h 2000 -l 1000 -v 500 -w 100 -i 20000 -n $c > ec_dispatching_rtcorba.$c.txt 2>&1
done
