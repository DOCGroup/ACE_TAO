#
# $Id$
#
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;
require Uniqueid;

$iorfile = "theior";
$sleeptime = 10;

$SV = Process::Create ("server".$Process::EXE_EXT, 
		       " -ORBobjrefstyle url");

sleep ($sleeptime);

$status  = system ("client".$Process::EXE_EXT.
		   " -x ");

$SV->Wait ();

# @@ Capture any errors from the server too.
exit $status;
