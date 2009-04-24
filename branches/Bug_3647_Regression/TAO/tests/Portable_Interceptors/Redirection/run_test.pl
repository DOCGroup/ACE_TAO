eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
#
# $Id$


use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (1) || die "Create target 2 failed\n";
my $client  = PerlACE::TestTarget::create_target (2) || die "Create target 3 failed\n";

my $iorbase1 = "test.ior";
my $iorbase2 = "test2.ior";

my $server1_iorfile = $server1->LocalFile ($iorbase1);
my $server2_iorfile = $server2->LocalFile ($iorbase2);
my $client1_iorfile  = $client->LocalFile ($iorbase1);
my $client2_iorfile  = $client->LocalFile ($iorbase2);

$server1->DeleteFile($iorbase1);
$server1->DeleteFile($iorbase2);
$server2->DeleteFile($iorbase1);
$server2->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

$SV1 = $server1->CreateProcess ("server", "-o $server1_iorfile -c 1 -n 1");
$SV2 = $server2->CreateProcess ("server", "-o $server2_iorfile -n 2");
$CL  = $client->CreateProcess ("client", "-p file://$client1_iorfile -s file://$client2_iorfile");

$status = 0;

print STDERR "\n\n==== Running PortableInterceptor::Redirection test\n";

$SV1->Spawn ();

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
    $SV2->Kill (); $SV2->TimedWait (1);
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

if ($server2_status != 0) {
    print STDERR "ERROR: server2 returned $server2_status\n";
    $status = 1;
}

$server1->DeleteFile($iorbase1);
$server1->DeleteFile($iorbase2);
$server2->DeleteFile($iorbase1);
$server2->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

exit $status;
