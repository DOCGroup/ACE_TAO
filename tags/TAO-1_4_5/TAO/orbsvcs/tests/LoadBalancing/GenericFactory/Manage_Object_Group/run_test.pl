eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../bin";
use PerlACE::Run_Test;

$lm_ior = "lm.ior";
unlink $lm_ior;

$status = 0;

$init_ref = "-ORBInitRef LoadManager=file://lm.ior";

$LM = new PerlACE::Process ("../../../../LoadBalancer/LoadManager", "-o lm.ior");
$SV = new PerlACE::Process ("server", $init_ref);

print STDERR "\n\n======== Running Manage ObjectGroup Membership Test================\n";
print STDERR "\n";

print STDERR "This test will check the add_member () and remove_member () methods of the\n";
print STDERR "ObjectGroupManager\n";
print STDERR "\n";

$LM->Spawn ();

if (PerlACE::waitforfile_timed ("lm.ior", 5) == -1) {
    print STDERR "ERROR: cannot find file LoadManager IOR: lm.ior\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$SV->Spawn ();

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

unlink $lm_ior;

exit $status;
