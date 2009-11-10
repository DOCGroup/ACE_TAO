eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfilebase = "server.ior";
$iorfile = PerlACE::LocalFile ("$iorfilebase");
unlink $iorfile;

# Test A: object exists (_non_existent() returns false)

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorfilebase");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
}
$CL = new PerlACE::Process ("client", "-k file://$iorfile");

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

$client = $CL->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);
$SV->Kill ();
unlink $iorfile;
if ($client != 2) {
    print STDERR "ERROR: client returned $client in test A, expected 2\n";
    # (see comment about result value in client.cpp)
    exit 1;
}

# Test B: object does not exist (_non_existent() returns true)

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorfilebase -r");
}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile -r");
}

$SV->Spawn ();
if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);
$SV->Kill ();
if ($client != 3) {
    print STDERR "ERROR: client returned $client in test B, expected 3\n";
    # (see comment about result value in client.cpp)
    unlink $iorfile;
    exit 1;
}

# Test C: server does not run (_non_existent() throws TRANSIENT)

# This test was failing on win32 without this sleep.
sleep 1;

$client = $CL->SpawnWaitKill ($PerlACE::wait_interval_for_process_creation);
unlink $iorfile;
if ($client != 5) {
    print STDERR "ERROR: client returned $client in test C, expected 5\n";
    # (see comment about result value in client.cpp)
    exit 1;
}

exit 0;
