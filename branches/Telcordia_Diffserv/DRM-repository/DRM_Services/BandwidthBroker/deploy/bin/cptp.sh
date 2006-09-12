export temp=`mktemp eth.XXXXXXX`
export ifconfig=`mktemp ifc.XXXXXXX`

if test "$2" == "down";
then

mysql -h $DBHOST -u arms -parms > $temp  <<END
use ARMSBB;
select name from L3Interface where id=$1;
END
awk '/eth/ {printf ("sudo ifconfig %s down \n", $1)}' $temp > $ifconfig 
cat $ifconfig
mysql -h $DBHOST -u arms -parms > $temp  <<END
use ARMSBB;
update PTP set operationalState="$2" where id=$1;
select name from Router where id in (select routerID from L3Interface where id=$1);
END
export router=`tail -1 $temp`.bb-l2.arms.emulab.net
echo $router
ssh -t $router < $ifconfig

fi;
if test "$2" == "up";
then
mysql -h $DBHOST -u arms -parms > $temp  <<END
use ARMSBB;
select name,ipAddress,ipMask from L3Interface where id=$1;
END
awk '/eth/ {printf ("sudo ifconfig %s %s netmask %s up\n", $1, $2, $3)}' $temp > $ifconfig 
cat $ifconfig
mysql -h $DBHOST -u arms -parms > $temp  <<END
use ARMSBB;
update PTP set operationalState="$2" where id=$1;
select name from Router where id in (select routerID from L3Interface where id=$1);
END
export router=`tail -1 $temp`.bb-l2.arms.emulab.net
echo $router
ssh -t $router < $ifconfig
fi

rm $temp
rm $ifconfig
