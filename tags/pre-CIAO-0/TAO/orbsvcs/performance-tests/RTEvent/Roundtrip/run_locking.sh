#! /bin/sh
#
# $Id$
#

LOW_PRIORITY="1 2 4 6 8 10 12 14 16 18 20 22 24 26 28 30"

#IOR=/project/amras/coryan/IOR/roundtrip.ior
IOR=test.ior

for c in $LOW_PRIORITY; do
  for t in copy_on_write copy_on_read delayed immediate; do
    date
    echo $t $c

    /bin/rm -f $IOR
#   ssh celegorm "cd /project/celegorm/coryan/EC_Roundtrip ; ./server -ORBSvcConf ec.dispatching_rtcorba.conf -r -o $IOR" >server.log 2>&1 </dev/null &
    ./server -ORBSvcConf ec.locking_${t}.conf -o $IOR >server.log 2>&1 </dev/null &
    while [ ! -f $IOR ]; do
      sleep 1
    done
    ./client -d -h 10000 -l 10000 -i 25000 -n $c -k file://$IOR > ec_locking_${t}.$c.txt 2>&1
    wait
  done
done
