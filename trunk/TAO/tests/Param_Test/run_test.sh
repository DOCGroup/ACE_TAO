#!/bin/sh
# $Id$

PORT=30001
IORFILE=theior
INVOCATION=sii
start_server()
{
  ./server -ORBobjrefstyle url -ORBport $PORT -o $IORFILE 2>&1 | sed -e 's/^/SERVER: /' &
  server_pid=`ps | grep server | grep -v grep | awk '{print $1;}'`
  sleep 2	# Give the server a chance to start up
}

# Restart the server if we get SIGCHLD, i.e., the server died.
trap "start_server" SIGCHLD
start_server
trap "kill $server_pid; rm -f $IORFILE" 0 1 2 3 15

TYPES="short ubstring fixed_struct strseq var_struct nested_struct struct_seq objref"
for type in $TYPES
do
  echo "BEGIN Testing type $type"
  ./client -f $IORFILE  -i $INVOCATION -t $type 2>&1 | sed -e "s/^/CLIENT($type): /"
  echo "END   Testing type $type"
done 
