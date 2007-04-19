eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("obj.ior");
unlink $iorfile;

$lm_ior = "lm.ior";
unlink $lm_ior;
$ns_ior = "ns.ior";
unlink $ns_ior;

$status = 0;

$init_ref = "-ORBInitRef LoadManager=file://lm.ior -n 1 -ORBInitRef NameService=file://ns.ior -s LeastLoaded -r 9000 -c 10300 -d 0.1 ";
$init_ref_backup = "-ORBInitRef LoadManager=file://lm.ior -n 2 -ORBInitRef NameService=file://ns.ior -s LeastLoaded -r 9000 -c 10300 -d 0.1 ";

$init_client_ref = "-k file://$iorfile -i 5 -n 1 -ORBInitRef NameService=file://ns.ior ";
$init_client_ref_backup = "-k file://$iorfile -i 5 -n 2 -ORBInitRef NameService=file://ns.ior ";

$LM = new PerlACE::Process ("../../LoadBalancer/LoadManager", "-o lm.ior");
$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service", "-o ns.ior");
$SV = new PerlACE::Process ("server", $init_ref);
$SR = new PerlACE::Process ("server", $init_ref_backup);
$CL = new PerlACE::Process ("client", $init_client_ref);
$CR = new PerlACE::Process ("client", $init_client_ref_backup);

$LM->Spawn ();

if (PerlACE::waitforfile_timed ("lm.ior", $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file LoadManager IOR: lm.ior\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$NS->Spawn ();

if (PerlACE::waitforfile_timed ("ns.ior", $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file NameService IOR: ns.ior\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find server file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$SR->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find server file <$iorfile>\n";
    $SR->Kill (); $SR->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (100);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$clientbackup = $CR->SpawnWaitKill (100);

if ($clientbackup != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$load_manager = $LM->TerminateWaitKill (10);

if ($load_manager != 0) {
    print STDERR "ERROR: LoadManager returned $load_manager\n";
    $status = 1;
}

$name_service = $NS->TerminateWaitKill (10);

if ($name_service != 0) {
    print STDERR "ERROR: NameService returned $name_service\n";
    $status = 1;
}

unlink $iorfile;
unlink $lm_ior;
unlink $ns_ior;

exit $status;
