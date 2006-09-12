#!/bin/csh

echo use ARMSBB > t2

ssh -T access1 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.12\.0/ {printf ("update PTP set name=\"%s\" where id = 1;\nupdate L3Interface set name=\"%s\" where id = 1;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.13\.0/ {printf ("update PTP set name=\"%s\" where id = 2;\nupdate L3Interface set name=\"%s\" where id = 2;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.14\.0/ {printf ("update PTP set name=\"%s\" where id = 3;\nupdate L3Interface set name=\"%s\" where id = 3;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.17\.0/ {printf ("update PTP set name=\"%s\" where id = 4;\nupdate L3Interface set name=\"%s\" where id = 4;\n", $8, $8)}' temp >> t2 

#ssh -T access1 > temp <<ENDROUTE
#/sbin/ifconfig
#ENDROUTE
#awk '/155\.98/ {printf ("update Router set managedIPAddr=\"%s\" where name = \"access1\";\n", substr ($2, 6))}' temp >> t2 
#awk '/155\.98/ {printf ("%s user password enablepw Linux\n", substr ($2, 6))}' temp > routerAuth.deleteme 
#


ssh -T access2 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.12\.0/ {printf ("update PTP set name=\"%s\" where id = 5;\nupdate L3Interface set name=\"%s\" where id = 5;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.23\.0/ {printf ("update PTP set name=\"%s\" where id = 6;\nupdate L3Interface set name=\"%s\" where id = 6;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.29\.0/ {printf ("update PTP set name=\"%s\" where id = 7;\nupdate L3Interface set name=\"%s\" where id = 7;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.120\.0/ {printf ("update PTP set name=\"%s\" where id = 8;\nupdate L3Interface set name=\"%s\" where id = 8;\n", $8, $8)}' temp >> t2 



#ssh -T access2 > temp <<ENDROUTE
#/sbin/ifconfig
#ENDROUTE
#awk '/155\.98/ {printf ("update Router set managedIPAddr=\"%s\" where name = \"access2\";\n", substr ($2, 6))}' temp >> t2 
#awk '/155\.98/ {printf ("%s user password enablepw Linux\n", substr ($2, 6))}' temp >> routerAuth.deleteme 
#

ssh -T access3 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.13\.0/ {printf ("update PTP set name=\"%s\" where id = 9;\nupdate L3Interface set name=\"%s\" where id = 9;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.23\.0/ {printf ("update PTP set name=\"%s\" where id = 10;\nupdate L3Interface set name=\"%s\" where id = 10;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.35\.0/ {printf ("update PTP set name=\"%s\" where id = 11;\nupdate L3Interface set name=\"%s\" where id = 11;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.130\.0/ {printf ("update PTP set name=\"%s\" where id = 12;\nupdate L3Interface set name=\"%s\" where id = 12;\n", $8, $8)}' temp >> t2 




#ssh -T access3 > temp <<ENDROUTE
#/sbin/ifconfig
#ENDROUTE
#awk '/155\.98/ {printf ("update Router set managedIPAddr=\"%s\" where name = \"access3\";\n", substr ($2, 6))}' temp >> t2 
#awk '/155\.98/ {printf ("%s user password enablepw Linux\n", substr ($2, 6))}' temp >> routerAuth.deleteme 
#

ssh -T access4 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.14\.0/ {printf ("update PTP set name=\"%s\" where id = 13;\nupdate L3Interface set name=\"%s\" where id = 13;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.45\.0/ {printf ("update PTP set name=\"%s\" where id = 14;\nupdate L3Interface set name=\"%s\" where id = 14;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.46\.0/ {printf ("update PTP set name=\"%s\" where id = 15;\nupdate L3Interface set name=\"%s\" where id = 15;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.47\.0/ {printf ("update PTP set name=\"%s\" where id = 16;\nupdate L3Interface set name=\"%s\" where id = 16;\n", $8, $8)}' temp >> t2 


#ssh -T access4 > temp <<ENDROUTE
#/sbin/ifconfig
#ENDROUTE
#awk '/155\.98/ {printf ("update Router set managedIPAddr=\"%s\" where name = \"access4\";\n", substr ($2, 6))}' temp >> t2 
#awk '/155\.98/ {printf ("%s user password enablepw Linux\n", substr ($2, 6))}' temp >> routerAuth.deleteme 
#


ssh -T access5 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.35\.0/ {printf ("update PTP set name=\"%s\" where id = 17;\nupdate L3Interface set name=\"%s\" where id = 17;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.45\.0/ {printf ("update PTP set name=\"%s\" where id = 18;\nupdate L3Interface set name=\"%s\" where id = 18;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.56\.0/ {printf ("update PTP set name=\"%s\" where id = 19;\nupdate L3Interface set name=\"%s\" where id = 19;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.150\.0/ {printf ("update PTP set name=\"%s\" where id = 20;\nupdate L3Interface set name=\"%s\" where id = 20;\n", $8, $8)}' temp >> t2 

#ssh -T access5 > temp <<ENDROUTE
#/sbin/ifconfig
#ENDROUTE
#awk '/155\.98/ {printf ("update Router set managedIPAddr=\"%s\" where name = \"access5\";\n", substr ($2, 6))}' temp >> t2 
#awk '/155\.98/ {printf ("%s user password enablepw Linux\n", substr ($2, 6))}' temp >> routerAuth.deleteme 

ssh -T access6 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.46\.0/ {printf ("update PTP set name=\"%s\" where id = 21;\nupdate L3Interface set name=\"%s\" where id = 21;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.56\.0/ {printf ("update PTP set name=\"%s\" where id = 22;\nupdate L3Interface set name=\"%s\" where id = 22;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.68\.0/ {printf ("update PTP set name=\"%s\" where id = 23;\nupdate L3Interface set name=\"%s\" where id = 23;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.160\.0/ {printf ("update PTP set name=\"%s\" where id = 24;\nupdate L3Interface set name=\"%s\" where id = 24;\n", $8, $8)}' temp >> t2 


ssh -T access7 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.17\.0/ {printf ("update PTP set name=\"%s\" where id = 25;\nupdate L3Interface set name=\"%s\" where id = 25;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.47\.0/ {printf ("update PTP set name=\"%s\" where id = 26;\nupdate L3Interface set name=\"%s\" where id = 26;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.78\.0/ {printf ("update PTP set name=\"%s\" where id = 27;\nupdate L3Interface set name=\"%s\" where id = 27;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.79\.0/ {printf ("update PTP set name=\"%s\" where id = 28;\nupdate L3Interface set name=\"%s\" where id = 28;\n", $8, $8)}' temp >> t2 


ssh -T access8 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.68\.0/ {printf ("update PTP set name=\"%s\" where id = 29;\nupdate L3Interface set name=\"%s\" where id = 29;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.78\.0/ {printf ("update PTP set name=\"%s\" where id = 30;\nupdate L3Interface set name=\"%s\" where id = 30;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.89\.0/ {printf ("update PTP set name=\"%s\" where id = 31;\nupdate L3Interface set name=\"%s\" where id = 31;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.180\.0/ {printf ("update PTP set name=\"%s\" where id = 32;\nupdate L3Interface set name=\"%s\" where id = 32;\n", $8, $8)}' temp >> t2 

ssh -T access9 > temp <<ENDROUTE
/sbin/route
ENDROUTE
 
awk '/192\.168\.29\.0/ {printf ("update PTP set name=\"%s\" where id = 33;\nupdate L3Interface set name=\"%s\" where id = 33;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.79\.0/ {printf ("update PTP set name=\"%s\" where id = 34;\nupdate L3Interface set name=\"%s\" where id = 34;\n", $8, $8)} ' temp >> t2 
awk '/192\.168\.89\.0/ {printf ("update PTP set name=\"%s\" where id = 35;\nupdate L3Interface set name=\"%s\" where id = 35;\n", $8, $8)}' temp >> t2 
awk '/192\.168\.190\.0/ {printf ("update PTP set name=\"%s\" where id = 36;\nupdate L3Interface set name=\"%s\" where id = 36;\n", $8, $8)}' temp >> t2 

#ssh -T access6 > temp <<ENDROUTE
#/sbin/ifconfig
#ENDROUTE
#awk '/155\.98/ {printf ("update Router set managedIPAddr=\"%s\" where name = \"access6\";\n", substr ($2, 6))}' temp >> t2 
#awk '/155\.98/ {printf ("%s user password enablepw Linux\n", substr ($2, 6))}' temp >> routerAuth.deleteme 

#echo java EncryptStuff routerAuth.deleteme cfg/BandwidthBroker/routerAuth 
#/usr/java/j2sdk1.4.2_04/bin/java EncryptStuff routerAuth.deleteme cfg/BandwidthBroker/routerAuth <<PWD
#darparms
#
#PWD

#cp t2 manIPupdate.sql

#echo mysql -h $DBHOST -u arms -parms 
/usr/local/bin/mysql -h $DBHOST -u arms -parms < t2
#rm temp
rm t2
echo \nDone. Press any key to quit  

