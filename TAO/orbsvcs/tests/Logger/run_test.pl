# $Id$
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

$tao_root = $ENV{TAO_ROOT};
# This is a Perl script that runs the Logger client and server

unshift @INC, '../../../../bin';
require Process;

# amount of delay between running the servers
$sleeptime = 3;

# Starts the Logging Service
sub service
{
  my $args = "";
  my $prog = $EXEPREFIX."$tao_root/orbsvcs/Logging_Service/Logging_Service"
      .$Process::EXE_EXT;    
  $SV = Process::Create ($prog, $args);
}

# Starts the test client
sub test
{
  my $args = "";
  my $prog = $EXEPREFIX."Logging_Test".$Process::EXE_EXT;
    
  system ("$prog $args");
}

# Start the service
service ();

# Give the service time to settle
sleep $sleeptime;

# Start the client (which exits automatically)
test ();

# Give the client time to log and exit
sleep 3;

# Kill the service
$SV->Kill ();
