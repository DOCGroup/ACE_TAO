#!/bin/sh
# $Id$

export TREE_ROOT=$HOME/ACE/latest
export ACE_ROOT=$TREE_ROOT/DOC_ROOT/ACE
export TAO_ROOT=$TREE_ROOT/DOC_ROOT/TAO
export CIAO_ROOT=$TREE_ROOT/DOC_ROOT/CIAO
export DANCE_ROOT=$TREE_ROOT/DOC_ROOT/DAnCE
mkdir -p $TREE_ROOT
cd $TREE_ROOT
clone --depth 1 http://:@git.remedy.nl/git/atcd.git DOC_ROOT
cd $ACE_ROOT/bin
rm *Tests.txt
rm *TestRev.txt
rm *Ignore.txt
rm *Builds.txt
rm cleanbuildresults.txt

./diff-builds-and-group-fixed-tests-only.sh
./cleanbuilds.sh

MAILTO="devo-group@list.isis.vanderbilt.edu"
MAIL="mail -S smtp=mail.remedy.nl"
MAILFROM="jwillemsen@remedy.nl"

MAIL_ATTACHMENTS=
for fn in `ls *Tests.txt`; do
   MAIL_ATTACHMENTS=$MAIL_ATTACHMENTS+"-a $fn "
done
for fn in `ls *NoTestRev.txt`; do
   MAIL_ATTACHMENTS=$MAIL_ATTACHMENTS+"-a $fn "
done
CURRENTDATE=`date -u +%Y_%m_%d`
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

$MAIL -r $MAILFROM -s "ACE/TAO/CIAO/DAnCE test statistics for $CURRENTDATE" $MAILTO < $mailfile

rm -f $mailfile

mailfile="/tmp/rsmailfile"
{
   echo "Sending failing tests for " $CURRENTDATE
   echo
   cat cleanbuildresults.txt
} > $mailfile

$MAIL -r $MAILFROM -s "ACE/TAO/CIAO/DAnCE failing tests for $CURRENTDATE" $MAILTO < $mailfile

rm -f $mailfile
