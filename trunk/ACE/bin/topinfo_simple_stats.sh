#!/bin/sh
#
# $Id$
#


if [ $# -lt 2 ]; then
  echo "Usage: $0 [DEST] [USER]"
  exit 0
fi

DEST=$1
US=$2

DATE=`date +%Y/%m/%d-%H:%M`
cd $ACE_ROOT
LD_LIBRARY_PATH=$ACE_ROOT/ace
export LD_LIBRARY_PATH
PATH=/usr/bin:/bin:$PATH
export PATH
cd $TAO_ROOT/performance-tests/Memory/Single_Threaded
# start the server
./server &
s_id=$!;
# Just sleep for 2 seconds.
sleep 2;
# Check whether the server has started
file="test.ior"
if test -f $file
    then
    # Just get the size as soon the server is started, ie. the vanilla
    # server.
    s_up=`cat /proc/$s_id/status | grep VmRSS | awk '{print $2}'`;

    # Write it a file
    echo $DATE $s_up >> $DEST/source/st_start_size.txt
    # start the client
    ./client &
    c_id=$!;
    # Wait till all the invocations are done
    sleep 10;
    # Get the size once the client has made sufficient invocations.
    s_invocations=`cat /proc/$s_id/status | grep VmRSS| awk '{print $2}'`;

    echo $DATE $s_invocations >> $DEST/source/st_after_invoke_size.txt

    # Get teh size of the client after all the invocations
    c_invocations=`cat /proc/$c_id/status | grep VmRSS | awk '{print $2}'`;
    echo $DATE $c_invocations >> $DEST/source/st_client_size.txt

    # Kill the server and client. We will look at better ways of doing
    # this later.
    kill -9 $c_id;

    # Just sleep for the server to release memory etc.
    sleep 5;

    # Get the size once the client is killed or crashed
    s_client_death=`cat /proc/$s_id/status | grep VmRSS | awk '{print $2}'`;
    echo $DATE $s_client_death  >> $DEST/source/st_after_peer_death_size.txt
    kill -9 $s_id;
    rm -f $file
else
    echo $file doesnt exist
fi


cd $DEST/source

FILES="start after_invoke client after_peer_death"
for i in $FILES ; do
/usr/bin/tac st_${i}_size.txt > $DEST/data/st_${i}_size.txt
/usr/bin/tail -5 st_${i}_size.txt > $DEST/data/LAST_st_${i}_size.txt
$ACE_ROOT/bin/generate_topinfo_charts.sh st_${i}_size.txt $DEST/images/st_${i}_size.png st_${i}_size.txt
done
