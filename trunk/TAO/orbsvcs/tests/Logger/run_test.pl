# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# This is a Perl script that runs the client and all the other servers that
# are needed

unshift @INC, '../../../../bin';
require Process;

# amount of delay between running the servers
$sleeptime = 3;

# Get the userid (or ip on NT)
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

# variables for parameters

$svport = 0; 

# other variables

$n = 1;
$leave = 1;
$ior = 0;
$done = "";
$debug = "";
$cm = "";
$sm = "";
$other = "";
$c_conf = "client.conf";
$s_conf = "server.conf";

sub server
{
    my $args = "";
    my $prog = "server".$Process::EXE_EXT;
    
    $SV = Process::Create ($prog, $args);
}

sub client
{
    my $args = "";
    my $prog = "client".$Process::EXE_EXT;
    
    $CL = Process::Create ($prog, $args);
}

server ();

sleep $sleeptime;

client ();

sleep $sleeptime;
if ($leave)
{
  $SV->Kill ();
  $NS->Kill ();
  $CL->Kill ();
}
