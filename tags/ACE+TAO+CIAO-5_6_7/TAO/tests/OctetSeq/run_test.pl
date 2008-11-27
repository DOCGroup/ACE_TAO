eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o test.ior");
    $T = new PerlACE::ProcessVX ("OctetSeq", "-n 32 -l 8192 -h 8192 -s 1 -q");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
    $T = new PerlACE::Process ("OctetSeq", "-n 32 -l 8192 -h 8192 -s 1 -q");
}
$CL = new PerlACE::Process ("client", "-i 5000 -k file://$iorfile");

print STDERR "\n\n==== Octet sequence passing test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (120);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (25);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "\n\n==== Octet sequence performance test\n";

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
