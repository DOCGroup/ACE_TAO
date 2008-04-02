eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the Logger client and server

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

# amount of delay between running the servers
$sleeptime = 15;

if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("Logging_Service");
}
else {
  $SV = new PerlACE::Process ("Logging_Service");
}
$CL = new PerlACE::Process ("Logging_Test");

# Start the service
$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

# Give the service time to settle
sleep $sleeptime;

# Start the client 
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: test returned $client\n";
    $status = 1;
}

# Kill the service
$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: service returned $server\n";
    $status = 1;
}

exit $status;
