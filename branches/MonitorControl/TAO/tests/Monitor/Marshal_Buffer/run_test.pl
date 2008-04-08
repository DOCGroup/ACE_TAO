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

unlink $iorfile;
unlink $mc_iorfile;

if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("server");
}
else {
  $SV = new PerlACE::Process ("server");
}
$CL = new PerlACE::Process ("client");
$MCL = new PerlACE::Process ("../../../utils/monitor/monitor_client", "-k file://monitor.ior -p \"Output CDR Buffer\" -p \"CPULoad\" -p \"BytesSent\" -i 10");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->Spawn ();

if (PerlACE::waitforfile_timed ($mc_iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find file <$mc_iorfile>\n";
    $CL->Kill ();
    exit 1;
}

sleep (2);

$monitor_client = $MCL->SpawnWaitKill (60);

if ($monitor_client != 0) {
    print STDERR "ERROR: monitor client returned $monitor_client\n";
    $status = 1;
}

$client = $CL->Kill ();

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
