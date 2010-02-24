eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

$iterations = '10';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "test.ior";
my $mc_iorbase = "monitor.ior";

my $server1_iorfile1 = $server1->LocalFile ($iorbase);
my $client_iorfile1 = $client->LocalFile ($iorbase);

my $server2_iorfile2 = $server2->LocalFile ($mc_iorbase);
my $client_iorfile2 = $client->LocalFile ($mc_iorbase);

$server1->DeleteFile($iorbase);
$server2->DeleteFile($mc_iorbase);
$client->DeleteFile($iorbase);
$client->DeleteFile($mc_iorbase);

$SV1 = $server1->CreateProcess ("server",
                                "-ORBdebuglevel $debug_level " .
                                "-o $server1_iorfile1");

$SV2 = $server2->CreateProcess ("../../../utils/monitor/monitor_client",
                                "-k \"file://$server2_iorfile2\" " .
                                "-p \"Output CDR Buffer\" " .
                                "-p \"OS/Processor/CPULoad\" " .
                                "-p \"OS/Network/BytesSent\" " .
                                "-i $iterations");

$CL = $client->CreateProcess ("client", "-k file://$client_iorfile1 -m $client_iorfile2");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}
if ($server1->WaitForFileTimed ($iorbase,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

# Start client
$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}
if ($client->WaitForFileTimed ($mc_iorbase,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

if ($client->GetFile ($mc_iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$client_iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

if ($server2->PutFile ($mc_iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$server2_iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

sleep (2);

$server_status = $SV2->SpawnWaitKill ($server2->ProcessStartWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$client_status = $CL->Kill ($client->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile($iorbase);
$server2->DeleteFile($mc_iorbase);
$client->DeleteFile($iorbase);
$client->DeleteFile($mc_iorbase);

exit $status;
