#!/bin/sh
# $Id$

uid=`id | cut -c5-20 | cut -f1 -d"("`
port=`expr 30001 + $uid`
iorfile=theior
invocation=sii

if [ `uname -s` = 'SunOS' ]; then
  #### SunOS sh's trap doesn't understand symbolic names for signals.
  sigchld=18
else
  sigchld='SIGCHLD'
  # Don't do this on SunOS.  It goes into an endless loop.
  trap "start_server" $sigchld
fi

start_server()
{
  ./server -ORBobjrefstyle url -ORBport $port -o $iorfile 2>&1 | \
    sed -e 's/^/SERVER: /' &
  server_pid=`ps | grep server | grep -v grep | awk '{print $1;}'`
  sleep 2	# Give the server a chance to start up
}

# Restart the server if we get SIGCHLD, i.e., the server died.
start_server
trap "kill $server_pid; rm -f $iorfile" 0 1 2 3 15

types="short ubstring fixed_struct strseq var_struct nested_struct struct_seq objref"
for type in $types
do
  echo "BEGIN Testing type $type"
  ./client -f $iorfile  -i $invocation -t $type 2>&1 | \
    sed -e "s/^/CLIENT($type): /"
  echo "END   Testing type $type"
done 
