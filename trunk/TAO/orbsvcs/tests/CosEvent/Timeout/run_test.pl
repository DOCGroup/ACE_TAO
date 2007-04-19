eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;
# $Id$

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$nsiorfile = PerlACE::LocalFile ("ns.ior");
$esiorfile = PerlACE::LocalFile ("es.ior");
$arg_ns_ref = "-ORBInitRef NameService=file://$nsiorfile";

$status = 0;

unlink $nsiorfile;
unlink $esiorfile;

# start Naming Service

$NameService = "$PerlACE::TAO_ROOT/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $nsiorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$nsiorfile>\n";
    $NS->Kill();
    exit 1;
}

# start Event Service
$EventService = "$PerlACE::TAO_ROOT/orbsvcs/CosEvent_Service/CosEvent_Service";
$ES = new PerlACE::Process($EventService, "-ORBSvcConf cosevent.conf "
                                          ."-b -o $esiorfile $arg_ns_ref");
$ES->Spawn();
if (PerlACE::waitforfile_timed ($esiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$esiorfile>\n";
    $ES->Kill();
    unlink $nsiorfile;
    exit 1;
}


$S = new PerlACE::Process("TimeoutTest", "-supplier -consumer -hang "
                          . $arg_ns_ref);
$ret = $S->SpawnWaitKill(30);
if ($ret != 0) {
    print STDERR "ERROR: application returned $ret\n";
    $status = 1;
}

$ret = $ES->Kill();
if ($ret != 0) {
    print STDERR "ERROR: event channel returned $ret\n";
    $status = 1;
}

$ret = $NS->Kill();
if ($ret != 0) {
    print STDERR "ERROR: name service returned $ret\n";
    $status = 1;
}

unlink $nsiorfile;
unlink $esiorfile;

exit $status;
