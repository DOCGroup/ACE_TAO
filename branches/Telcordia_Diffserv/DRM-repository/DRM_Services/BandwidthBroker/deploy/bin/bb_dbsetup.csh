#!/bin/csh

echo Please ignore any \"Can\'t drop database\" errors in the following.
sudo /usr/local/mysql/bin/mysql -u root <<ENDSQL
create database if not exists ARMSBB;
grant all privileges on ARMSBB.* to arms@localhost identified by 'arms';
grant all privileges on ARMSBB.* to arms@"%" identified by 'arms';
exit
ENDSQL

/usr/local/mysql/bin/mysql -u arms -parms < ${BB_DB_INIT_SQL}
/usr/local/mysql/bin/mysql -u arms -parms < ${BB_DATA_SQL}

echo \nDone. Press any key to quit  

