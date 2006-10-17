#!/bin/sh

if [ $# -eq 1 ] && [ $1 == "clean" ] ; then
    for i in 1 2 3 4 5 6 7 8 ; 
    do 
      echo "Cleaning router 10.0.$i.$i" 
      ssh 10.0.$i.$i sudo /sbin/iptables -t mangle -F PREROUTING  
    done
    exit 0
fi

for i in 1 2 3 4 5 6 7 8 ; 
do 
  echo "Showing router 10.0.$i.$i" 
  ssh 10.0.$i.$i sudo /sbin/iptables -t mangle -L PREROUTING  
done

