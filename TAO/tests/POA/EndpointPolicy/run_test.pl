eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server_shared = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server_multi  = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client_good = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client_bad  = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $goodiorbase = "good.ior";
my $badiorbase = "bad.ior";

my $server_shared_good_iorfile = $server_shared->LocalFile ($goodiorbase);
my $server_multi_good_iorfile = $server_shared->LocalFile ($goodiorbase);
my $server_shared_bad_iorfile = $server_multi->LocalFile ($badiorbase);
my $server_multi_bad_iorfile = $server_multi->LocalFile ($badiorbase);
my $client_good_iorfile = $client_good->LocalFile ($goodiorbase);
my $client_bad_iorfile = $client_bad->LocalFile ($badiorbase);

$server_shared->DeleteFile($goodiorbase);
$server_shared->DeleteFile($badiorbase);
$server_multi->DeleteFile($goodiorbase);
$server_multi->DeleteFile($badiorbase);
$client_good->DeleteFile($goodiorbase);
$client_bad->DeleteFile($badiorbase);

$port = 12345;

$sharedSV = $server_shared->CreateProcess ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -g $server_shared_good_iorfile -b $server_shared_bad_iorfile -p $port");
$multiSV = $server_multi->CreateProcess ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -g $server_multi_good_iorfile -b $server_multi_bad_iorfile -p $port");

$goodCL = $client_good->CreateProcess ("client", "-k file://$client_good_iorfile");
$badCL = $client_bad->CreateProcess ("client", "-b -k file://$client_bad_iorfile");

print "Starting server using shared profiles\n";

$server_status_shared = $sharedSV->Spawn ();

if ($server_status_shared != 0) {
    print STDERR "ERROR: server returned $server_status_shared\n";
    exit 1;
}

if ($server_shared->WaitForFileTimed ($goodiorbase,
                               $server_shared->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_shared_good_iorfile>\n";
    $sharedSV->Kill (); $sharedSV->TimedWait (1);
    exit 1;
}

$client_status_bad = $badCL->SpawnWaitKill ($client_bad->ProcessStartWaitInterval());

if ($client_status_bad != 0) {
    print STDERR "ERROR: bad client returned $client_status_bad\n";
    $status = 1;
}

$client_good_status = $goodCL->SpawnWaitKill ($client_good->ProcessStartWaitInterval());

if ($client_good_status != 0) {
    print STDERR "ERROR: good client returned $client_good_status\n";
    $status = 1;
}

$server_status_shared = $sharedSV->WaitKill ($server_shared->ProcessStopWaitInterval());

if ($server_status_shared != 0) {
    print STDERR "ERROR: server [single profile per IOR] returned $server_status_shared\n";
    $status = 1;
}

$server_shared->DeleteFile($goodiorbase);
$server_shared->DeleteFile($badiorbase);
$server_multi->DeleteFile($goodiorbase);
$server_multi->DeleteFile($badiorbase);
$client_good->DeleteFile($goodiorbase);
$client_bad->DeleteFile($badiorbase);

print "Starting server using multiple profiles\n";

$server_status_multi = $multiSV->Spawn ();

if ($server_multi->WaitForFileTimed ($goodiorbase,
                               $server_multi->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_multi_good_iorfile>\n";
    $multiSV->Kill (); $multiSV->TimedWait (1);
    exit 1;
}

$client_bad_status = $badCL->SpawnWaitKill ($client_bad->ProcessStartWaitInterval());

if ($client_bad_status != 0) {
    print STDERR "ERROR: bad client returned $client_bad_status\n";
    $status = 1;
}

$client_good_status = $goodCL->SpawnWaitKill ($client_good->ProcessStartWaitInterval());

if ($client_good_status != 0) {
    print STDERR "ERROR: good client returned $client_good_status\n";
    $status = 1;
}

$server_multi_status = $multiSV->WaitKill ($server_multi->ProcessStopWaitInterval());

if ($server_multi_status != 0) {
    print STDERR "ERROR: server [multiple profiles per IOR] returned $server_multi_status\n";
    $status = 1;
}

$server_shared->DeleteFile($goodiorbase);
$server_shared->DeleteFile($badiorbase);
$server_multi->DeleteFile($goodiorbase);
$server_multi->DeleteFile($badiorbase);
$client_good->DeleteFile($goodiorbase);
$client_bad->DeleteFile($badiorbase);

exit $status;
