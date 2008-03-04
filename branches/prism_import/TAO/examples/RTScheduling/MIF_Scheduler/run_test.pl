eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('..');

# amount of delay between running the servers

$sleeptime = 2;
$status = 0;

$nsior = PerlACE::LocalFile ("ns.ior");

unlink $nsior;

$NS = new PerlACE::Process ("../../../orbsvcs/Naming_Service/Naming_Service", "-o $nsior");
$TS1 = new PerlACE::Process ("test", "-ORBInitRef NameService=file://$nsior -ORBSvcConf svc.conf.server");
$TS2 = new PerlACE::Process ("test", "-ORBInitRef NameService=file://$nsior -ORBSvcConf svc.conf.client");
$ST = new PerlACE::Process ("Starter", "-ORBInitRef NameService=file://$nsior");

print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

print STDERR "Starting Test 1\n";

$TS1->Spawn ();

sleep 5;

print STDERR "Starting Test 2\n";

$TS2->Spawn ();

sleep 5;

$starter = $ST->SpawnWaitKill (200);

if ($starter!= 0) {
    print STDERR "ERROR:  starter returned $starter\n";
    $status = 1;
}

sleep 200;

$test1 = $TS1->TerminateWaitKill (5);

if ($test1 != 0) {
    print STDERR "ERROR: receiver returned $test1\n";
    $status = 1;
}

$test2 = $TS2->TerminateWaitKill (5);

if ($test2!= 0) {
    print STDERR "ERROR: test 1 returned $test2\n";
    $status = 1;
}


$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;

exit $status;









