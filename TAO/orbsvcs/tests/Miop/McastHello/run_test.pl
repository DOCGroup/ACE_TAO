eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
$iorfile_server2 = PerlACE::LocalFile ("server.ior.unused");
unlink $iorfile;
unlink $iorfile_server2;
$status = 0;

$SV1 = new PerlACE::Process ("server", "-o $iorfile");
$SV2 = new PerlACE::Process ("server", "-o $iorfile_server2 ");
$CL = new PerlACE::Process ("client", " -k file://$iorfile ");

$SV1->Spawn ();
$SV2->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if (PerlACE::waitforfile_timed ($iorfile_server2, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_server2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV1->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$server = $SV2->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;
unlink $iorfile_server2;

exit $status;
