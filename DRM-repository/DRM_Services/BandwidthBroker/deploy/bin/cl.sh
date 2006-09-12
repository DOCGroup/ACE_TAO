export temp1=`mktemp ptp.XXXXXXX`

mysql -h $DBHOST -u arms -parms > $temp1  <<END
use ARMSBB;
select aPtpID from Link where id=$1;
END
export ptp=`tail -1 $temp1`
bin/cptp.sh $ptp $2
mysql -h $DBHOST -u arms -parms > $temp1  <<END
use ARMSBB;
select zPtpID from Link where id=$1;
END
export ptp=`tail -1 $temp1`
bin/cptp.sh $ptp $2

rm $temp1
