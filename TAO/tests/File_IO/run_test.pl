eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$server_debug_level = '0';
$client_debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-sdebug') {
        $server_debug_level = '10';
    }
    if ($i eq '-cdebug') {
        $client_debug_level = '10';
    }
}


my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $client_file = "test";
my $server_file = "big.txt";

my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);

my $client_test_file = $client->LocalFile ($client_file);
my $server_test_file = $server->LocalFile ($server_file);

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$server->DeleteFile($server_file);
$client->DeleteFile($client_file);


$SV = $server->CreateProcess ("server", "-ORBdebuglevel $server_debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client", "-ORBdebuglevel $client_debug_level -k $client_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
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

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 105);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$server->DeleteFile($server_file);
$client->DeleteFile($client_file);

exit $status;
