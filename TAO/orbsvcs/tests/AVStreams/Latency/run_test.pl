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

ACE::checkForTarget($cwd);

$latency_ping= $EXEPREFIX.".".$DIR_SEPARATOR."ping".$EXE_EXT;
$latency_pong = $EXEPREFIX.".".$DIR_SEPARATOR."pong".$EXE_EXT;
$latency_control = $EXEPREFIX.".".$DIR_SEPARATOR."control".$EXE_EXT;


# variables for parameters

sub ping
{
    my $args = " -o ping_ior";
    print ("\nPing: $args\n");
    $SV = Process::Create ($latency_ping, $args);
}


sub pong
{
    my $args = " -o pong_ior";
    print ("\nPong: $args\n");
    $CL = Process::Create ($latency_pong, $args);
}                                                     


sub control
{
    my $args = " -f file://ping_ior -g file://pong_ior";
    print ("\nControl: $args\n");
    $CO = Process::Create ($latency_control, $args);
}

ping ();

sleep $sleeptime;

pong ();

sleep $sleeptime;

control ();


if ($CL->TimedWait (200) == -1) {
  print STDERR "ERROR: client timedout\n";
  $status = 1;
  $CL->Kill (); $CL->TimedWait (1);
}

$SV->Terminate (); if ($SV->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate server\n";
  $SV->Kill (); $SV->TimedWait (1);
  $CO->Kill (); $CO->TimedWait (1);
  exit 1;
}

exit $status;
