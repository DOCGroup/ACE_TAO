#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../../bin';
require Process;

$client_port = 0;
$server_port = 0;
$iorfile = "theior";
$sleeptime = 5;

$SV = Process::Create ("..$DIR_SEPARATOR"
		       ."IDL_Cubit".$DIR_SEPARATOR.
		       "server".$Process::EXE_EXT,
                       " -ORBport ".$server_port.
		       " -ORBobjrefstyle url".
		       " -o $iorfile");

sleep $sleeptime;

$status = system ("client".$Process::EXE_EXT.
		  " -ORBport $client_port".
		  " -f $iorfile -x");

# @@ TODO change to Wait() once the -x option works.
$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
