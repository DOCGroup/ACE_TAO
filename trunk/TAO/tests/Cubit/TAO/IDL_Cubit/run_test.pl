#!/pkg/gnu/bin/perl -I../../../../../bin
#
# If your perl installation isn't in /pkg/gnu/bin/perl,
# please make the change accordingly

use Process;

if ($^O eq "MSWin32")
{
  system ("ipconfig | find \"Address\">ipnum");

  open (IPNUM, "ipnum");

  read (IPNUM, $line, 80);

  ($junk, $ip1, $ip2, $ip3, $ip4) = split (/: (\d+)\.(\d+)\.(\d+)\.(\d+)/, $line);

  close IPNUM;

  system ("del /q ipnum");

  $uid = $ip4;
}
else
{
  $uid = getpwnam (getlogin ());
}

$nsport = 20000 + $uid;
$client_port = 0;
$server_port = 0;
$iorfile = "cubit_ior";

$SV = Process::Create ("server".$Process::EXE_EXT, 
                       "-ORBport ".$server_port." -ORBnameserviceport ".$nsport." -ORBobjrefstyle url");

sleep (2);

system ("client".$Process::EXE_EXT." -x -ORBnameserviceport ".$nsport." -ORBport ".$client_port );
