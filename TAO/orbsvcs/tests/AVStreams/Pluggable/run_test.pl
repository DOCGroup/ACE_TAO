eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require Process;
require ACEutils;
use Cwd;

# amount of delay between running the servers

$sleeptime = 6;
$status = 0;
$cwd = getcwd();
local $nsior = "$cwd$DIR_SEPARATOR" . "ns.ior";

ACE::checkForTarget($cwd);

$name_server_prog = $EXEPREFIX."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."..".$DIR_SEPARATOR."Naming_Service".$DIR_SEPARATOR."Naming_Service".$EXE_EXT;
$pluggable_server= $EXEPREFIX.".".$DIR_SEPARATOR."server".$EXE_EXT;
$pluggable_ftp = $EXEPREFIX.".".$DIR_SEPARATOR."ftp".$EXE_EXT;


# variables for parameters

sub name_server
{
    my $args = "-o $nsior";
    print ("\n$name_server_prog $args\n");
    unlink $nsior;
    $NS = Process::Create ($name_server_prog, $args);
    if (ACE::waitforfile_timed ($nsior, 8) == -1) {
      print STDERR "ERROR: cannot find naming service IOR file\n";
      $NS->Kill (); $NS->TimedWait (1);
      exit 1;
    }
}

sub server
{
    my $args = " -ORBInitRef NameService=file://$nsior"." -f test";
    print ("\nPluggable Server: $args\n");
    $SV = Process::Create ($pluggable_server, $args);
}


sub ftp
{
    my $args = " -ORBInitRef NameService=file://$nsior"." -f Makefile";
    print ("\nPluggable Ftp: $args\n");
    $CL = Process::Create ($pluggable_ftp, $args);
}

name_server ();

server ();

sleep $sleeptime;

ftp ();

if ($CL->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $CL->Kill (); $CL->TimedWait (1);
}

$SV->Terminate (); if ($SV->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate server\n";
  $SV->Kill (); $SV->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}



$NS->Terminate (); if ($NS->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate naming service\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}


exit $status;
