#! /bin/sh
#
# $Id$
#

LOW_PRIORITY="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28"
#"1 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32"

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
  ./client -ORBSvcConf realtime.conf -r -d -h 10000 -l 10000 -i 20000 -n $c -k file://$IOR > rtcorba.$c.txt 2>&1
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
  ./client -d -h 10000 -l 10000 -i 20000 -n $c -k file://$IOR > reactive.$c.txt 2>&1
  sleep 5
  wait

done
