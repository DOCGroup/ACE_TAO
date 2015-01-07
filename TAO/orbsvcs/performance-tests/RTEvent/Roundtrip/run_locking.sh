#! /bin/sh
#

. parameters

for n in $NTHREADS; do
  for c in $NCONSUMERS; do
    for t in $LOCKING_TYPES; do
      date
      echo $t $c $n

      /bin/rm -f $IOR
#     ssh $REMOTE_HOST "cd $REMOTE_LOCATION ; ./server -ORBSvcConf ec.locking_${t}.conf -r -o $IOR" >server.log 2>&1 </dev/null &
      ./server -ORBSvcConf ec.locking_${t}.conf -o $IOR -n $n >server.log 2>&1 </dev/null &
      while [ ! -f $IOR ]; do
        sleep 1
      done
      ./client -d -u -h 10000 -l 10000 -i $ITERATIONS -c $c -n $n -k file://$IOR > ec_locking.${t}.$c.txt 2>&1

      wait
    done
  done
done
