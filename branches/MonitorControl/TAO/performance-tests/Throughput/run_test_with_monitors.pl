eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");
$mc_iorfile = PerlACE::LocalFile ("monitor.ior");
$svc_conf = PerlACE::LocalFile ("svc$PerlACE::svcconf_ext");

print STDERR "================ Throughput test\n";

unlink $iorfile;
unlink $mc_iorfile;

$SV = new PerlACE::Process ("server", " -ORBSvcConf $svc_conf");

$CL = new PerlACE::Process ("client",
                            " -ORBSvcConf $svc_conf -ORBNoDelay 1"
                            . " -x -m");
                            
$MCL = new PerlACE::Process ("monitor_client");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL->Spawn ();

if (PerlACE::waitforfile_timed ($mc_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find file <$mc_iorfile>\n";
    $CL->Kill ();
    exit 1;
}

#sleep (2);

$monitor_client = $MCL->SpawnWaitKill (60);

if ($monitor_client != 0) {
    print STDERR "ERROR: monitor client returned $monitor_client\n";
    $status = 1;
}

$client = $CL->WaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client kill returned $client\n";
    $status = 1;
}

$server = $SV->Kill ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $mc_iorfile;

exit $status;
