#! /bin/sh
#

LOW_PRIORITY="1 4 8 12 16 20 24 28 32"

ITERATIONS=10000

#IOR=/project/amras/coryan/IOR/roundtrip.ior
IOR=test.ior

for c in $LOW_PRIORITY; do
  date
  echo rtcorba $c

  /bin/rm -f $IOR
#  ssh celegorm "cd /project/celegorm/coryan/RTCORBA_Baseline ; ./server -ORBSvcConf realtime.conf -r -n $c -o $IOR" >server.log 2>&1 </dev/null &
  ./server -ORBSvcConf realtime.conf -r -n $c -o $IOR >server.log 2>&1 </dev/null &
  while [ ! -f $IOR ]; do
    sleep 1
  done
  ./client -ORBSvcConf realtime.conf -g -r -d -h 0 -l 0 -w 9000 -i $ITERATIONS -n $c -k file://$IOR > rtcorba.$c.txt 2>&1
  sleep 5
  wait

  date
  echo reactive $c

  /bin/rm -f $IOR
#  ssh celegorm "cd /project/celegorm/coryan/RTCORBA_Baseline ; ./server -n $c -o $IOR" >server.log 2>&1 </dev/null &
  ./server -n $c -o $IOR >server.log 2>&1 </dev/null &
  while [ ! -f $IOR ]; do
    sleep 1
  done
  ./client -g -d -h 0 -l 0 -w 9000 -i $ITERATIONS -n $c -k file://$IOR > reactive.$c.txt 2>&1
  sleep 5
  wait

done
