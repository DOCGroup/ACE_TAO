#!/bin/bash
t2=`mktemp t2.XXXXXXX`

echo use ARMSBB\; > $t2
echo select @r1:=id from Router where name=\"$1\"\; >> $t2
echo select @r2:=id from Router where name=\"$2\"\; >> $t2
echo select @pid:=id from L3Path where ingressID=@r1 and egressID=@r2 \; >> $t2
echo delete from L3Hops where l3PathID=@pid AND isOldPath=1 \; >> $t2
echo update L3Hops set isOldPath=1 where l3PathID=@pid AND isOldPath=0 \; >> $t2

if [ "$3" != "" -a "$4" != ""  ]; then
   echo select @l3i:=id from L3Interface where ipAddress=\"$3\"\; >> $t2
   echo insert into L3Hops \(l3InterfaceID, l3PathID, hopNumber, isOldPath\) values \(@l3i, @pid, 1, 0 \)\; >> $t2
fi;
if [ "$4" != ""  -a "$5" != "" ]; then
   echo select @l3i:=id from L3Interface where ipAddress=\"$4\"\; >> $t2
   echo insert into L3Hops \(l3InterfaceID, l3PathID, hopNumber, isOldPath\) values \(@l3i, @pid, 2, 0\)\; >> $t2
fi;
if [ "$5" != ""  -a "$6" != "" ]; then
   echo select @l3i:=id from L3Interface where ipAddress=\"$5\"\; >> $t2
   echo insert into L3Hops \(l3InterfaceID, l3PathID, hopNumber, isOldPath\) values \(@l3i, @pid, 3, 0\)\; >>$t2
fi;
if [ "$6" != "" -a "$7" != "" ]; then
   echo select @l3i:=id from L3Interface where ipAddress=\"$6\"\; >> $t2
   echo insert into L3Hops \(l3InterfaceID, l3PathID, hopNumber, isOldPath\) values \(@l3i, @pid, 4, 0\)\; >>$t2
fi;
if [ "$7" != "" -a "$8" != "" ]; then
   echo select @l3i:=id from L3Interface where ipAddress=\"$7\"\; >> $t2
   echo insert into L3Hops \(l3InterfaceID, l3PathID, hopNumber, isOldPath\) values \(@l3i, @pid, 5, 0\)\; >>$t2
fi;
if [ "$8" != "" -a "$9" != "" ]; then
   echo select @l3i:=id from L3Interface where ipAddress=\"$8\"\; >> $t2
   echo insert into L3Hops \(l3InterfaceID, l3PathID, hopNumber, isOldPath\) values \(@l3i, @pid, 6, 0\)\; >>$t2
fi;

cat $t2
mysql -h $DBHOST -u arms -parms <$t2
#mysql -h $DBHOST -u arms -parms <bin/fixtr;
rm $t2
