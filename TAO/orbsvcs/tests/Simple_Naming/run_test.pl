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

$nsmport = 10000 + $uid;

sub name_server
{
  my $args = "-ORBnameserviceport $nsmport";
  my $prog = "..$DIR_SEPARATOR..$DIR_SEPARATOR"."Naming_Service".$DIR_SEPARATOR.
             "Naming_Service".$Process::EXE_EXT;

  $NS = Process::Create ($prog, $args);
}

sub client
{
  my $args = "-ORBnameserviceport $nsmport";
  my $prog = "client".$Process::EXE_EXT;
    
  system ($prog." ".$args);
}

name_server ();
sleep $sleeptime;
client ();

$NS->Kill ();

# @@ Capture any exit status from the processes.
exit 0;
