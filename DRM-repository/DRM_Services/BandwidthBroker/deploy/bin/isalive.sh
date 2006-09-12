#!/bin/bash
export temp=`mktemp ping.XXXXXXX`
mysql -h $DBHOST -u arms -parms > $temp  <<MYSQLEND
use ARMSBB;
select managedIPAddr from Router where name = "$1";
MYSQLEND
ipaddr=`tail -1 $temp`
ping -c 1 $ipaddr | grep loss | awk '/packet/ {printf ("%s\n", $4)}' 
rm -r $temp
