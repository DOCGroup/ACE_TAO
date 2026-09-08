eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$cdebug_level = '0';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-cdebug') {
      $cdebug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1)
    || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2)
    || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

# The timeout is intentionally short. The first ping opens the transport
# and is followed 1.5 seconds later by the second ping. Without the fix,
# the server-side timer started when the transport was opened expires after
# 2 seconds and the second request fails. With the fix, receiving the first
# request restarts the server-side timer and the second request succeeds.
my $server_args =
    "-ORBSvcConf svc.conf -ORBdebuglevel $debug_level -ORBVerboseLogging 1 " .
    "-o $server_iorfile";

my $client_args =
    "-ORBdebuglevel $cdebug_level -ORBVerboseLogging 1 " .
    "-k file://$client_iorfile";

$SV = $server->CreateProcess ("server", $server_args);
$CL = $client->CreateProcess ("client", $client_args);

$server_status = $SV->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed (
        $iorbase, $server->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
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

$client_status = $CL->SpawnWaitKill (
    $client->ProcessStartWaitInterval () + 10);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill (
    $server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

exit $status;
