eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

print STDERR "\n\n==== Running CPU Load Monitor test\n";


$lm_ior = "lm.ior";
unlink $lm_ior;

$status = 0;

$init_ref = "-ORBInitRef LoadManager=file://" . $lm_ior;

$location = "MyLocation";

$LM = new PerlACE::Process ("../../../../LoadBalancer/LoadManager",
			    "-o " . $lm_ior);

$MON = new PerlACE::Process ("../../../../LoadBalancer/LoadMonitor",
			     "-l " . $location
			     . " -t CPU -s PULL "
			     . $init_ref);

$CL = new PerlACE::Process ("client",
			    $init_ref
			    . " -l " . $location);

$LM->Spawn ();

if (PerlACE::waitforfile_timed ($lm_ior, 8) == -1) {
    print STDERR "ERROR: cannot find file LoadManager IOR: " . $lm_ior ."\n";
    exit 1;
}

$MON->Spawn ();

$client = $CL->SpawnWaitKill (100);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$load_monitor = $MON->TerminateWaitKill (10);
$load_manager = $LM->TerminateWaitKill (10);

if ($load_manager != 0 || $load_monitor != 0) {
    print STDERR "Error terminating LoadManager or LoadMonitor.\n";
    $status = 1;
}

unlink $lm_ior;

exit $status;
