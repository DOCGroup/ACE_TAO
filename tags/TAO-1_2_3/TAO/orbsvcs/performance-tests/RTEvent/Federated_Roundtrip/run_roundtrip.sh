#! /bin/sh
#
# $Id$
#

. parameters

for c in $NCONSUMERS; do

  date
  echo $t $c $n

  /bin/rm -f $IOR
# ssh $REMOTE_HOST "cd $REMOTE_LOCATION ; ./server -ORBSvcConf ec.locking_${t}.conf -r -o $IOR" >server.log 2>&1 </dev/null &
  ./server -o $IOR -i $ITERATIONS -p 2 >server.$c.txt 2>&1 </dev/null &
  while [ ! -f $IOR ]; do
    sleep 1
  done
  ./client -ORBSvcConf roundtrip.conf -k file://$IOR -d -z -h 10000 -l 10000 -i $ITERATIONS -c $c > ec_roundtrip.$c.01.txt 2>&1 </dev/null &

  ./client -ORBSvcConf roundtrip.conf -k file://$IOR -d -z -h 10000 -l 10000 -i $ITERATIONS -c $c > ec_roundtrip.$c.02.txt 2>&1 </dev/null &

  wait
  wait
  wait
done
