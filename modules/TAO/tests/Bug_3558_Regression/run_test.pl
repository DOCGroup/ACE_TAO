eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase1 = "server1.ior";
my $iorbase2 = "server2.ior";
my $server1_iorfile = $server->LocalFile ($iorbase1);
my $server2_iorfile = $server->LocalFile ($iorbase2);
my $client1_iorfile = $client->LocalFile ($iorbase1);
my $client2_iorfile = $client->LocalFile ($iorbase2);

$server->DeleteFile($iorbase1);
$server->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

$SV1 = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server1_iorfile");
$SV2 = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server2_iorfile");
$CL = $client->CreateProcess ("client", "-k file://$server1_iorfile -l file://$server2_iorfile -d $debug_level");

$server1_status = $SV1->Spawn ();

if ($server1_status != 0) {
    print STDERR "ERROR: server returned $server1_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase1,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server2_status = $SV2->Spawn ();

if ($server2_status != 0) {
    print STDERR "ERROR: server returned $server2_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server1_status = $SV1->WaitKill ($server->ProcessStopWaitInterval());

if ($server1_status != 0) {
    print STDERR "ERROR: server returned $server1_status\n";
    $status = 1;
}

$server2_status = $SV2->WaitKill ($server->ProcessStopWaitInterval());

if ($server2_status != 0) {
    print STDERR "ERROR: server returned $server2_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase1);
$server->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

exit $status;
