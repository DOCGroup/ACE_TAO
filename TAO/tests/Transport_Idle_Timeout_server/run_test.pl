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

# Server-only expiry: Y=2, X=2. Reuse across scans, then observe idle closure.
my $server_args =
    "-ORBSvcConf server.conf -ORBdebuglevel $debug_level -ORBVerboseLogging 1 " .
    "-o $server_iorfile";

my $client_args =
    "-ORBSvcConf client.conf -ORBdebuglevel $cdebug_level -ORBVerboseLogging 1 " .
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
    $client->ProcessStartWaitInterval () + 20);

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
