#! /bin/sh
#

. parameters

for n in 2; do
  for c in 2 4 6 8 10 12; do

  date
  echo rtcorba $c $n

  ./driver -ORBSvcConf ec.dispatching_rtcorba.conf -r -d -h 10000 -l 10000 -i $ITERATIONS -c $c -n $n > ec_dispatching.rtcorba.${c}.${n}.txt 2>&1

  date
  echo threaded $c $n

  ./driver -ORBSvcConf ec.dispatching_threaded.conf -d -h 10000 -l 10000 -i $ITERATIONS -c $c -n $n > ec_dispatching.threaded.${c}.${n}.txt 2>&1

  date
  echo reactive $c $n

  ./driver -ORBSvcConf ec.dispatching_reactive.conf -d -h 10000 -l 10000 -i $ITERATIONS -c $c -n $n > ec_dispatching.reactive.${c}.${n}.txt 2>&1

  done
done
