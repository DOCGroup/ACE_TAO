eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

# Make pretty look thing
print STDERR "\n";
my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client  = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $iorbase1 = "server.ior";
my $iorbase2 = "server2.ior";
my $iorbase3 = "server3.ior";

my $server1_iorfile = $server1->LocalFile ($iorbase1);
my $server2_iorfile = $server2->LocalFile ($iorbase2);
my $server3_iorfile = $server3->LocalFile ($iorbase3);
my $client_iorfile = $client->LocalFile ($iorbase3);

$server1->DeleteFile ($iorbase1);
$server2->DeleteFile ($iorbase2);
$server3->DeleteFile ($iorbase3);
$client->DeleteFile ($iorbase3);

$SV1 = $server1->CreateProcess ("server", "-o $server1_iorfile");
$SV2 = $server2->CreateProcess ("server", "-o $server2_iorfile -f file://$server1_iorfile");
$SV3 = $server3->CreateProcess ("server", "-o $server3_iorfile -f file://$server2_iorfile");
$CL  = $client->CreateProcess ("client",  "-s 3 -k file://$client_iorfile");

$status = 0;

$server1_status = $SV1->Spawn ();

if ($server1_status != 0) {
    print STDERR "ERROR: server returned $server1_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($iorbase1,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$SV2->Spawn ();

if ($server2->WaitForFileTimed ($iorbase2,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$SV3->Spawn ();

if ($server3->WaitForFileTimed ($iorbase3,
                               $server3->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server3_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV3->Kill (); $SV3->TimedWait (1);
    exit 1;
}

$client_status  = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());
$server1_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());
$server2_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());
$server3_status = $SV3->WaitKill ($server3->ProcessStopWaitInterval());

$server1->DeleteFile ($iorbase1);
$server2->DeleteFile ($iorbase2);
$server3->DeleteFile ($iorbase3);
$client->DeleteFile ($iorbase3);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

if ($server1_status != 0) {
    print STDERR "ERROR: server 1 returned $server1_status\n";
    $status = 1;
}

if ($server2_status != 0) {
    print STDERR "ERROR: server 2 returned $server2_status\n";
    $status = 1;
}

if ($server3_status != 0) {
    print STDERR "ERROR: server 3 returned $server3_status\n";
    $status = 1;
}

exit $status;
