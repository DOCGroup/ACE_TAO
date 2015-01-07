#! /bin/sh
#

LOW_PRIORITY="1 5 10 15 20 15 30 35 40 45 50"
#" 60 70 80 90 100 110 120 130 140 150 160"

#IOR=/project/amras/coryan/IOR/roundtrip.ior
IOR=test.ior

for c in $LOW_PRIORITY; do
#   date
#   echo reactive $c
#
#   /bin/rm -f $IOR
#   ssh celegorm "cd /project/celegorm/coryan/EC_Roundtrip ; ./server -ORBSvcConf ec.dispatching_reactive.conf -o $IOR" >server.log 2>&1 </dev/null &
#   while [ ! -f $IOR ]; do
#     sleep 1
#   done
#   ./client -d -h 1000 -l 2000 -i 500 -n $c -k file://$IOR > ec_dispatching_reactive.$c.txt 2>&1
#   wait
#
#   date
#   echo threaded $c
#
#   /bin/rm -f $IOR
#   ssh celegorm "cd /project/celegorm/coryan/EC_Roundtrip ; ./server -ORBSvcConf ec.dispatching_threaded.conf -o $IOR" >server.log 2>&1 </dev/null &
#   while [ ! -f $IOR ]; do
#     sleep 1
#   done
#   ./client -d -h 1000 -l 2000 -i 500 -n $c -k file://$IOR > ec_dispatching_threaded.$c.txt 2>&1
#   wait

  date
  echo rtcorba $c

  /bin/rm -f $IOR
#  ssh celegorm "cd /project/celegorm/coryan/EC_Roundtrip ; ./server -ORBSvcConf ec.dispatching_rtcorba.conf -r -o $IOR" >server.log 2>&1 </dev/null &
  ./server -ORBSvcConf ec.dispatching_rtcorba.conf -r -o $IOR" >server.log 2>&1 </dev/null &
  while [ ! -f $IOR ]; do
    sleep 1
  done
  ./client -ORBSvcConf ec.dispatching_rtcorba.conf -r -d -h 1000 -l 2000 -i 500 -n $c -k file://$IOR > ec_dispatching_rtcorba.$c.txt 2>&1
  wait

done
