#!/bin/sh


#export DBHOST=pool2db
STR="BBClient"
REF=`java TimeStamp initial`
if [ "$RES" == "1" ]
then
  RES=`bin/isalive.sh access2`
fi

if [ "$RES" == "1" ]
then
   FINISHED=0
   bin/bbtestmead.sh i=$STR$REF  $*  &
  

   while [[ `bin/isalive.sh access2` == "1" ]]
   do
	HUNG=`pgrep -f $STR$REF | wc -l`
	if [ "$HUNG" == "0" ] 
	then
	  echo "exiting"
	   #exit;
	 FINISHED=1 
	 break
	 fi

   done
   echo "while done"


fi


if [ "$FINISHED" != 1 ]
then
  RES=0
   pkill -f $STR$REF
   RES2=`bin/isalive.sh access5`
    if [ "$RES2" == "1" ]
    then
        echo "before backup call"
	bin/bbtestmead.sh i=$STR$REF n=AdmissionControlBackup  $* 
        echo "backup call done"
    fi
fi
#fi

