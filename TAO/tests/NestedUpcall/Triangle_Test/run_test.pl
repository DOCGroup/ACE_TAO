eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$ior1file = PerlACE::LocalFile ("servera.ior");
$ior2file = PerlACE::LocalFile ("serverb.ior");

#  Make sure these aren't hanging around
unlink $ior1file;
unlink $ior2file;

if (PerlACE::is_vxworks_test()) {
    $SV1 = new PerlACE::ProcessVX ("server_A", "-o servera.ior");
}
else {
    $SV1 = new PerlACE::Process ("server_A", "-o $ior1file");
}
$SV2 = new PerlACE::Process ("server_B", "-o $ior2file");
$CL = new PerlACE::Process ("initiator", "-f $ior1file -g $ior2file");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if (PerlACE::waitforfile_timed ($ior1file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior1file>\n";
    $SV1->Kill ();
    $SV2->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($ior2file,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ior2file>\n";
    $SV2->Kill ();
    $SV1->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

$server1 = $SV1->TerminateWaitKill (5);
$server2 = $SV2->TerminateWaitKill (5);

# Clean up
unlink $ior1file;
unlink $ior2file;

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server1 != 0) {
    print STDERR "ERROR: server 1 returned $server1\n";
    $status = 1;
}

if ($server2 != 0) {
    print STDERR "ERROR: server 2 returned $server2\n";
    $status = 1;
}

exit $status;
