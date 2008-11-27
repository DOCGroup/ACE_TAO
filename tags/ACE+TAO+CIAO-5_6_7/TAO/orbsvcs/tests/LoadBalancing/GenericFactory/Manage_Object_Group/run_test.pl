eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$lm_ior = "lm.ior";
unlink $lm_ior;

$status = 0;

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$lm_conf = PerlACE::LocalFile ("windows$PerlACE::svcconf_ext");

$init_ref = "-ORBInitRef LoadManager=file://lm.ior";

$LM = new PerlACE::Process ("../../../../LoadBalancer/LoadManager",
                            "-o lm.ior" . ($^O eq 'MSWin32' ?
                                           " -ORBSvcConf $lm_conf" : ''));
$SV = new PerlACE::Process ("server", $init_ref);

print STDERR "\n\n======== Running Manage ObjectGroup Membership Test================\n";
print STDERR "\n";

print STDERR "This test will check the add_member () and remove_member () methods of the\n";
print STDERR "ObjectGroupManager\n";
print STDERR "\n";

$LM->Spawn ();

if (PerlACE::waitforfile_timed ("lm.ior", $PerlACE::wait_interval_for_process_creation) == -1) {
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
