eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

# amount of delay between running the servers

$sleeptime = 6;
$status = 0;

# variables for parameters

$nsiorbase = "ns.ior";
$nsior = PerlACE::LocalFile ("$nsiorbase");

unlink $nsior;

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service", "-o $nsior");
if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("server", "-ORBInitRef NameService=file://$nsiorbase");
}
else {
  $SV = new PerlACE::Process ("server", "-ORBInitRef NameService=file://$nsior");
}
$CL = new PerlACE::Process ("client", "-ORBInitRef NameService=file://$nsior");

print STDERR "Starting Naming_Service\n";
$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

print STDERR "Starting Server\n";
$SV->Spawn ();

sleep $sleeptime;

print STDERR "Starting Client\n";


$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (15);

if ($nserver != 0) {
    print STDERR "ERROR: name server returned $nserver\n";
    $status = 1;
}

exit $status;
