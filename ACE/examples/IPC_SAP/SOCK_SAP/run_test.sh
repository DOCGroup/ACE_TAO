#! /bin/sh
#
# Spawns CPP-inserver-fancy and CPP-inclient executables on a single host.

usage="usage: $0 #client_threads"

user=`whoami`
iterations=1000

if [ $# -ne 1 ]; then
  echo $usage;
  exit 1
fi
threads=$1;


########
######## Enable signal handler.
########
trap 'kill -1 $server_pid; ' 0 1 2 15


########
######## Start CPP-inserver-fancy and save its pid.
########
./CPP-inserver-fancy > \
  ${tmp}server.log 2>&1 &
server_pid=$!

sleep 2;

########
######## Start CPP-inclient.
########
./CPP-inclient -2 -T 100000 -m 69 -t $threads -i 100  > ${tmp}client-${threads}.log 2>&1
