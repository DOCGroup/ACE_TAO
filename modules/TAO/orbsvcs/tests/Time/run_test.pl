eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$server_ior   = PerlACE::LocalFile ("server_ior");
$clerk_ior    = PerlACE::LocalFile ("clerk_ior");

$status = 0;

# Make sure the files are gone, so we can wait on them.

unlink $server_ior, $clerk_ior;

$SV = new PerlACE::Process ("../../Time_Service/Time_Service_Server", "-o $server_ior");
$CK = new PerlACE::Process ("../../Time_Service/Time_Service_Clerk", "-f $server_ior -o $clerk_ior -t 2");
$CL = new PerlACE::Process ("client", "-f $clerk_ior");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($server_ior, 15) == -1) {
    print STDERR "ERROR: cannot find file <$server_ior>\n";
    $SV->Kill ();
    exit 1;
}

$CK->Spawn ();

if (PerlACE::waitforfile_timed ($clerk_ior, 15) == -1) {
    print STDERR "ERROR: cannot find file <$clerk_ior>\n";
    $SV->Kill ();
    $CK->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$clerk = $CK->TerminateWaitKill (5);

if ($clerk != 0) {
    print STDERR "ERROR: clerk returned $clerk\n";
    $status = 1;
}

unlink $server_ior, $clerk_ior;

exit $status;
