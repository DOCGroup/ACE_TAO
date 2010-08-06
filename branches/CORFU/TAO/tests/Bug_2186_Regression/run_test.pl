eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$port = PerlACE::uniqueid () + 10001;

$serveriorfile = PerlACE::LocalFile ("server.ior");
unlink $serveriorfile;
$clientiorfile = PerlACE::LocalFile ("client.ior");
unlink $clientiorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBSvcConf server.conf");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBSvcConf server.conf");
}
$CL = new PerlACE::Process ("client", "-ORBEndpoint iiop://:$port");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($serveriorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$serveriorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: First client spawn returned $client. Indicates a problem other than a regression.\n";
    $status = 1;
}

if ($client == 0) {
    $client = $CL->SpawnWaitKill (300);

    if ($client != 0) {
        print STDERR "ERROR: REGRESSION - Second client spawn returned $client\n";
        $status = 1;
    }
}

$server = $SV->TerminateWaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $serveriorfile;
unlink $clientiorfile;

exit $status;
