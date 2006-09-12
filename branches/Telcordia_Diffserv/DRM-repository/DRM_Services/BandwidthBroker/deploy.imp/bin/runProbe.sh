#!/bin/sh

DISPLAY=${DISPLAY:-":0.0"}
export DISPLAY

GUI=
if test "X$1" = "X-G"
then
    echo "X display is $DISPLAY"
    xset -q > /dev/null 2>&1 || ( echo "Could not contact X server" ; exit 1 )
    GUI=-G
fi

#export IMPDB=$1
#export DBHOST=`ssh $IMPDB /proj/arms/etc/imp/find_control_interface_ip`
export DBHOST=$1
echo "DBHOST=$DBHOST"
if test "$DBHOST" = ""
then
 echo "Error in setting DBHOST"
 exit 1
fi

#export GUI=-G
export LOCALHOST=`hostname`

java imp.ImpServer $GUI $LOCALHOST $DBHOST
