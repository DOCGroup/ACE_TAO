eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

$tao_root = $ENV{TAO_ROOT};
# This is a Perl script that runs the Logger client and server

unshift @INC, '../../../../bin';
require Process;

# amount of delay between running the servers
$sleeptime = 3;

# error register
$status = 0;

# Starts the Logging Service
sub service
{
  my $args = "";
  my $prog = $EXEPREFIX."$tao_root/orbsvcs/Logging_Service/Logging_Service"
      .$EXE_EXT;
  $SV = Process::Create ($prog, $args);
}

# Starts the test client
sub test
{
  my $args = "";
  my $prog = $EXEPREFIX."Logging_Test".$EXE_EXT;

  $CL = Process::Create ($prog, $args);
}

# Start the service
service ();

# Give the service time to settle
sleep $sleeptime;

# Start the client (which exits automatically)
test ();

# Give the client time to log and exit
if ($CL->TimedWait (60) == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

# Kill the service
$SV->Terminate (); if ($SV->TimedWait (5) == -1) {
  print STDERR "ERROR: couldn't shutdown the service nicely\n";
  $status = 1;
  $SV->Kill (); $SV->TimedWait (1);
}

exit $status;
