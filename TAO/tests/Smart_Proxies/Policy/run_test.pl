eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile1base = "test1.ior";
$iorfile1 = PerlACE::LocalFile ("$iorfile1base");
$iorfile2 = PerlACE::LocalFile ("test2.ior");

unlink $iorfile1;
unlink $iorfile2;

if (PerlACE::is_vxworks_test()) {
$SV1 = new PerlACE::ProcessVX ("server", "-o $iorfile1base");
}
else {
$SV1 = new PerlACE::Process ("server", "-o $iorfile1");
}
$SV2 = new PerlACE::Process ("server", "-o $iorfile2");
$CL  = new PerlACE::Process ("client", "-i file://$iorfile1 -j file://$iorfile2");

print STDERR "\nrunning Smart Proxy test consisting of the client and two servers with <one-shot> policy\n\n";

$SV1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile1,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV1->Kill ();
    exit 1;
}

$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV1->Kill ();
    $SV2->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV1->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server 1 returned $server\n";
    $status = 1;
}

unlink $iorfile1;

$server = $SV2->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server 2 returned $server\n";
    $status = 1;
}

unlink $iorfile2;

print STDERR "\nrunning Smart Proxy test consisting of the client and two servers with <one-shot> policy disabled\n\n";

$SV1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile1,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV1->Kill ();
    exit 1;
}

$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV1->Kill ();
    $SV2->Kill ();
    exit 1;
}

$CL->Arguments ("-f0 -i file://$iorfile1 -j file://$iorfile2");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV1->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server 1 returned $server\n";
    $status = 1;
}

unlink $iorfile1;

$server = $SV2->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server 2 returned $server\n";
    $status = 1;
}

unlink $iorfile2;

exit $status;
