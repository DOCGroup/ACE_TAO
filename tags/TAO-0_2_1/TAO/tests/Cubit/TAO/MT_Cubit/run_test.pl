# $Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../../bin';
require Process;

$server_port = 0;
$iorfile = "theior";
$sleeptime = 3;

$SV = Process::Create ("server".$Process::EXE_EXT, 
                       " -ORBport ".$server_port.
		       " -ORBobjrefstyle url".
		       " -s -f $iorfile");

sleep $sleeptime;

$status = system ("client".$Process::EXE_EXT.
		  " -x -s -f $iorfile");


# @@ TODO change to Wait() once the -x option works.
$SV->Kill (); $SV->Wait ();

unlink $iorfile;

# @@ Capture any errors from the server too.
exit $status;
