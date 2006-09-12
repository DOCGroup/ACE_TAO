export temp=`mktemp ro.XXXXXXX`
export ifconfig=`mktemp ifc.XXXXXXX`
export sql=`mktemp msql.XXXXXXX`

ssh -T coresw1 > $temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.1\.0/ {printf ("sudo ifconfig %s down \n", $8)}' $temp > $ifconfig 
awk '/192\.168\.1\.0/ {printf ("sudo ifconfig %s 192.168.1.1 netmask 255.255.255.0 up\n", $8)}' $temp > bin/repair
echo "use ARMSBB;" > bin/repair.db
echo "update PTP set operationalState=\"up\" where id=1;" >> bin/repair.db
echo "update PTP set operationalState=\"up\" where id=6;" >> bin/repair.db

ssh -T coresw1 < $ifconfig
echo "use ARMSBB;" > $sql
echo "update PTP set operationalState=\"down\" where id=1;" >> $sql
echo "update PTP set operationalState=\"down\" where id=6;" >> $sql
mysql -h pool2db -u arms -parms < $sql

rm $temp
rm $ifconfig
rm $sql
