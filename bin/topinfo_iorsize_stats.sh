#!/bin/sh
#
# $Id$
#

if [ $# -lt 4 ]; then
  echo "Usage: $0 [ROOT] [DEST] [USER] [OPTIMIZED]"
  exit 0
fi

ROOT=$1
DEST=$2
US=$3
OPT=$4

DATE=`date +%Y/%m/%d-%H:%M`
cd $ROOT
ACE_ROOT=$ROOT
export ACE_ROOT
LD_LIBRARY_PATH=$ACE_ROOT/ace
export LD_LIBRARY_PATH
PATH=/usr/bin:$PATH
export PATH
cd TAO/performance-tests/Memory/IORsize

# start the server. If OPT == 1 then start the optimized version, else
# the non-optimized version

if test $OPT == 1
    then ./server -ORBSvcConf server.conf &
    else ./server &
fi

s_id=$!;

# Just sleep for 2 seconds.
sleep 2;
# Check whether the server has started 
file="test.ior"
if test -f $file
    then
    # start the client
    ./client &
    c_id=$!;
    # Wait till all the invocations are done
    sleep 30;
    # Get the size once the client has made sufficient invocations. 
    s_invocations=`top -p $s_id -n 1 -b | grep $US| awk '{print $5}'`;

    if test $OPT == 1
        then 
        echo $DATE $s_invocations >> $DEST/source/server_opt_ior_size.txt
        else
        echo $DATE $s_invocations >> $DEST/source/server_ior_size.txt
    fi

    # Get teh size of the client after all the invocations
    c_invocations=`top -p $c_id -n 1 -b | grep $US| awk '{print $5}'`;
    echo $DATE $c_invocations >> $DEST/source/client_ior_size.txt

    # Kill the server and client. We will look at better ways of doing
    # this later. 
    kill -9 $c_id;
    kill -9 $s_id;
    rm -f $file
else
    echo $file doesnt exist
fi


cd $DEST/source

FILES="server server_opt client"
for i in $FILES ; do
/usr/bin/tac ${i}_ior_size.txt > $DEST/data/${i}_ior_size.txt
/usr/bin/tail -5 ${i}_ior_size.txt > $DEST/data/LAST_${i}_ior_size.txt
$ROOT/bin/generate_topinfo_charts.sh ${i}_ior_size.txt $DEST/images/${i}_ior_size.png ${i}_ior_size.txt
done
