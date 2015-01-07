#!/bin/sh

if test -z $1; then CURRENTDATE=`date -u +%Y_%m_%d`; else CURRENTDATE=$1; fi
if test -z $2; then PREFIX=`date -u +%Y%m%d%a`; else PREFIX=$2; fi
if test -z $3; then MAILTO="devo-group@list.isis.vanderbilt.edu"; else MAILTO=$3; fi

cd $ACE_ROOT/bin

./diff-builds-and-group-fixed-tests-only.sh $CURRENTDATE $PREFIX
./cleanbuilds.sh $CURRENTDATE

MAIL="mail -S smtp=mail.remedy.nl"
MAILFROM="jwillemsen@remedy.nl"

MAIL_ATTACHMENTS=
for fn in `ls *Tests.txt`; do
   MAIL_ATTACHMENTS=$MAIL_ATTACHMENTS+"-a $fn "
done
for fn in `ls *NoTestRev.txt`; do
   MAIL_ATTACHMENTS=$MAIL_ATTACHMENTS+"-a $fn "
done
mailfile="/tmp/rsmailfile"
{
   echo "Sending test statistics for" $CURRENTDATE
   echo
   cat *NoTestRev.txt
   echo
   echo "Sending with revision number"
   cat *Tests.txt
   echo
   echo "Sending results per build"
   cat *Builds.txt
} > $mailfile

$MAIL -v -r $MAILFROM -s "ACE/TAO/CIAO/DAnCE test statistics for $CURRENTDATE" $MAILTO < $mailfile

rm -f $mailfile

mailfile="/tmp/rsmailfile"
{
   echo "Sending failing tests for " $CURRENTDATE
   echo
   cat cleanbuildresults.txt
} > $mailfile

$MAIL -v -r $MAILFROM -s "ACE/TAO/CIAO/DAnCE failing tests for $CURRENTDATE" $MAILTO < $mailfile

rm -f $mailfile
