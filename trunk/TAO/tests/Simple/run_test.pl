#$Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../bin';
require Process;
require Uniqueid;

$nsport = 20000 + uniqueid ();
# doesn't actually use ns yet
$client_port = 0;
$server_port = 0;
$iorfile = "theior";
$sleeptime = 3;

$SV = Process::Create ("server".$Process::EXE_EXT, 
                       " -ORBport $server_port -o $iorfile ".
		       " -ORBobjrefstyle url");

sleep ($sleeptime);

$status  = system ("client".$Process::EXE_EXT.
		   " -f $iorfile -x -ORBport $client_port");

if ($^O eq "MSWin32")
{
  system ("del ".$iorfile);
}
else
{
  system ("rm ".$iorfile);
}

$SV->Wait ();

exit $status;
