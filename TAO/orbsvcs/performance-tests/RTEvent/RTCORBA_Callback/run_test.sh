#! /bin/sh
#

. parameters

for n in $NTHREADS; do
  date
  echo normal $n

  /bin/rm -f $IOR
# ssh $REMOTE_HOST "cd $REMOTE_LOCATION ;./server -o $IOR -n $n" >server.log 2>&1 </dev/null &
  ./server -o $IOR -n $n >server.log 2>&1 </dev/null &
  while [ ! -f $IOR ]; do
    sleep 1
  done
  ./client -d -h 10000 -l 10000 -i $ITERATIONS -n $n -k file://$IOR > normal.$n.txt 2>&1

  wait

  date
  echo realtime $n
  /bin/rm -f $IOR
# ssh $REMOTE_HOST "cd $REMOTE_LOCATION ; ./server -ORBSvcConf realtime.conf -r -o $IOR -n $n" >server.log 2>&1 </dev/null &
  ./server -ORBSvcConf realtime.conf -r -o $IOR -n $n >server.log 2>&1 </dev/null &
  while [ ! -f $IOR ]; do
    sleep 1
  done
  ./client -ORBSvcConf realtime.conf -r -d -h 10000 -l 10000 -i $ITERATIONS -n $n -k file://$IOR > realtime.$n.txt 2>&1

  wait

done
