export TREE_ROOT=/home/build/ACE/latest
export ACE_ROOT=$TREE_ROOT/ACE_wrappers
export TAO_ROOT=$ACE_ROOT/TAO
export CIAO_ROOT=$TAO_ROOT/CIAO
cd $TREE_ROOT
svn up
cd $ACE_ROOT/bin
rm *Tests.txt
rm *TestRev.txt
./diff-builds-and-group-fixed-tests-only.sh

MAILTO="devo-group@list.isis.vanderbilt.edu"
MAIL="/usr/bin/mail -S smtp=10.2.0.3"

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
   cat *Tests.txt
   echo
   echo "Sending without revision number"
   cat *NoTestRev.txt
} > $mailfile

$MAIL -s "ACE/TAO/CIAO test statistics for $CURRENTDATE" $MAILTO < $mailfile

rm -f $mailfile

