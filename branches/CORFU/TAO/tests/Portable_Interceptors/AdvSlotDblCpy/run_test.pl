eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
my $server = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-o $server_iorfile");
$CL = $client->CreateProcess ("client", "file://$client_iorfile");

print STDERR "\n\n==== Running advanced slot double copy test\n";

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorbase>\n";
    $SV->Kill ();
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill (60);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill (5);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
