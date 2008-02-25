eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile1base = "server1.ior";
$iorfile1 = PerlACE::LocalFile ("$iorfile1base");
$iorfile2 = PerlACE::LocalFile ("server2.ior");

unlink $iorfile1;
unlink $iorfile2;

$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV1 = new PerlACE::ProcessVX ("server", "-o $iorfile1base");
}
else {
    $SV1 = new PerlACE::Process ("server", "-o $iorfile1");
}
$SV2 = new PerlACE::Process ("server", "-o $iorfile2 -r");
$CL = new PerlACE::Process ("client", " -k file://$iorfile1 -m file://$iorfile2");

$SV1->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile1,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$SV2->Spawn ();
if (PerlACE::waitforfile_timed ($iorfile2,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server1 = $SV1->WaitKill (10);

$server2 = $SV2->WaitKill (10);
if ($server2 != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}
unlink $iorfile1;
unlink $iorfile2;

exit $status;
