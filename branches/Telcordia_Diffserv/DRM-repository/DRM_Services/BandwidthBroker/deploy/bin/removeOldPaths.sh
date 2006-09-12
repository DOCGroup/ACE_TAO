mysql -h pool2db -u arms -parms <<MYSQLEND
use ARMSBB;
delete from L3Hops where isOldPath = 1;
MYSQLEND
