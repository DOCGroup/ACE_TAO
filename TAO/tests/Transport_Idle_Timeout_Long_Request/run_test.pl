eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
my $server = PerlACE::TestTarget::create_target (1)
    || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2)
    || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
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

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

my $SV = $server->CreateProcess (
    "server",
    "-ORBSvcConf svc.conf -o $server_iorfile -ORBdebuglevel $debug_level -ORBVerboseLogging 1");

my $CL = $client->CreateProcess (
    "client",
    "-ORBSvcConf svc.conf -k file://$client_iorfile -ORBdebuglevel $cdebug_level -ORBVerboseLogging 1");

if ($SV->Spawn () != 0) {
    print STDERR "ERROR: server failed to start\n";
    exit 1;
}

if ($server->WaitForFileTimed (
        $iorbase, $server->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1 ||
    $client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot transfer IOR\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

my $client_status =
    $CL->SpawnWaitKill ($client->ProcessStartWaitInterval () + 15);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

my $server_status =
    $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

exit $status;
