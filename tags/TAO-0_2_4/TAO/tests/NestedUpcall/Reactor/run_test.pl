#$Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require Process;
require Uniqueid;

$nsport = 20000 + uniqueid ();
$client_port = 0;
$server_port = 0;
$iorfile = "theior";
$sleeptime = 3;

$SV = Process::Create ("server".$Process::EXE_EXT, 
                       " -ORBport ".$server_port.
		       " -ORBnameserviceport ".$nsport.
		       " -ORBobjrefstyle url");

sleep ($sleeptime);

$status  = system ("client".$Process::EXE_EXT.
		   " -x -ORBnameserviceport ".$nsport.
		   " -ORBport ".$client_port );

$SV->Wait ();

# @@ Capture any errors from the server too.
exit $status;
