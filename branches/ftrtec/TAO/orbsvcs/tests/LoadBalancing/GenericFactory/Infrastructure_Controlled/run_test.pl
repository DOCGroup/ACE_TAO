eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("obj.ior");
unlink $iorfile;

$lm_ior = "lm.ior";
unlink $lm_ior;

$status = 0;

$init_ref = "-ORBInitRef LoadManager=file://lm.ior";

$LM = new PerlACE::Process ("../../../../LoadBalancer/LoadManager", "-o lm.ior");
$SV = new PerlACE::Process ("server", $init_ref);
$CL = new PerlACE::Process ("client", " -k file://$iorfile");

$LM->Spawn ();

if (PerlACE::waitforfile_timed ("lm.ior", 5) == -1) {
    print STDERR "ERROR: cannot find file LoadManager IOR: lm.ior\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find server file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (100);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$load_manager = $LM->TerminateWaitKill (10);

if ($load_manager != 0) {
    print STDERR "ERROR: LoadManager returned $load_manager\n";
    $status = 1;
}

unlink $iorfile;
unlink $lm_ior;

exit $status;
