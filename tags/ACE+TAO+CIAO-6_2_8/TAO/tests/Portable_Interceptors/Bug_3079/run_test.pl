eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client  = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $iorbase1 = "test1.ior";
my $iorbase2 = "test2.ior";
my $iorbase3 = "test3.ior";

my $server1_iorfile = $server1->LocalFile ($iorbase1);
my $server2_iorfile = $server2->LocalFile ($iorbase2);
my $server3_iorfile = $server3->LocalFile ($iorbase3);
my $client_iorfile1 = $client->LocalFile ($iorbase1);
my $client_iorfile2 = $client->LocalFile ($iorbase2);
my $client_iorfile3 = $client->LocalFile ($iorbase3);

$server1->DeleteFile ($iorbase1);
$server2->DeleteFile ($iorbase2);
$server3->DeleteFile ($iorbase3);
$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);
$client->DeleteFile ($iorbase3);

$SV1 = $server1->CreateProcess ("server", "-o $server1_iorfile -c 1 -n 1 -s 1");
$SV2 = $server2->CreateProcess ("server", "-o $server2_iorfile -n 2 -s 2");
$SV3 = $server3->CreateProcess ("server", "-o $server3_iorfile -n 3 -s 3");
$CL  = $client->CreateProcess ("client",
                    "-p file://$client_iorfile1 -f file://$client_iorfile2 -s file://$client_iorfile3");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::Bug_3079 test\n";

$SV1->Spawn ();
$SV2->Spawn ();
$SV3->Spawn ();

if ($server1->WaitForFileTimed ($iorbase1,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->WaitForFileTimed ($iorbase2,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($server3->WaitForFileTimed ($iorbase3,
                               $server3->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server3_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV3->Kill (); $SV3->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server1_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

# The first server will crash by design and in such instance it will
# return the magic number 1. The test must not interpret it as an
# error.

if ($server1_status != 0) {
    if ($server1_status != 1) {
        print STDERR "ERROR: server1 returned $server1_status\n";
        $status = 1;
    }
    else {
        print STDERR "INFO: server1 commited suicide as expected\n";
    }
}

$server2_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

# The second server will crash by design and in such instance it will
# return the magic number 1. The test must not interpret it as an
# error.
if ($server2_status != 0) {
    if ($server2_status != 1) {
        print STDERR "ERROR: server2 returned $server2_status\n";
        $status = 1;
    }
    else {
        print STDERR "INFO: server2 commited suicide as expected\n";
    }
}

$server3_status = $SV3->WaitKill ($server3->ProcessStopWaitInterval());

if ($server3_status != 0) {
    print STDERR "ERROR: server3 returned $server3_status\n";
    $status = 1;
}

$server1->DeleteFile ($iorbase1);
$server2->DeleteFile ($iorbase2);
$server3->DeleteFile ($iorbase3);
$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);
$client->DeleteFile ($iorbase3);

exit $status;
