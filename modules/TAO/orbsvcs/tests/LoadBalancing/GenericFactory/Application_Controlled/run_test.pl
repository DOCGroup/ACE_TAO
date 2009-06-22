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
$CL = new PerlACE::Process ("client", " -k file://$iorfile");

print STDERR "\n\n======== Running Application Controlled Membership Test================\n";
print STDERR "\n";

print STDERR "This test uses the Random Load Balancing strategy in the Cygnus Load Balancer\n";

print STDERR "6 servers are created and added into a Object Group\n";

print STDERR "When the client makes an invocation, Random load balancing strategy\n";
print STDERR "selects one of the servers and then the client makes 5 invocations on\n";
print STDERR "the server. When done, the application has to delete the object from the\n";
print STDERR "object group. This is called the application controlled membership of\n";
print STDERR "object group.\n";
print STDERR "\n";

$LM->Spawn ();

if (PerlACE::waitforfile_timed ("lm.ior", $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file LoadManager IOR: lm.ior\n";
    $LM->Kill (); $LM->TimedWait (1);
    exit 1;
}

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find server file <$iorfile>\n";
    $LM->Kill ();
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (100);

if ($client != 0) {
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

unlink $iorfile;
unlink $lm_ior;

exit $status;
