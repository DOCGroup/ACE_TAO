#!/pkg/gnu/bin/perl -I../../../../../bin
#
# If your perl installation isn't in /pkg/gnu/bin/perl,
# please make the change accordingly

use Process;

$client_port = 0;
$server_port = 0;
$iorfile = "cubit_ior";

$SV = Process::Create ("server".$Process::EXE_EXT, 
                       "-s -ORBport ".$server_port." -o ".$iorfile." -ORBobjrefstyle url");

sleep (2);

system ("client".$Process::EXE_EXT." -x -s -f ".$iorfile." -ORBport ".$client_port );

$SV->Kill ();


if ($^O eq "MSWin32")
{
  system ("del ".$iorfile);
}
else
{
  system ("rm ".$iorfile);
}