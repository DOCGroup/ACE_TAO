eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $goodiorbase = "good.ior";
my $badiorbase = "bad.ior";
my $svcconf = "multi_prot.conf";

my $server_svcconf_file = $server->LocalFile ($svcconf);
my $server_good_iorfile = $server->LocalFile ($goodiorbase);
my $server_bad_iorfile = $server->LocalFile ($badiorbase);
my $client_good_iorfile = $client->LocalFile ($goodiorbase);
my $client_bad_iorfile = $client->LocalFile ($badiorbase);

$server->DeleteFile($goodiorbase);
$server->DeleteFile($badiorbase);
$client->DeleteFile($goodiorbase);
$client->DeleteFile($badiorbase);

$port = 12345;

$sharedSV = $server->CreateProcess ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 1 -g $server_good_iorfile -b $server_bad_iorfile -p $port -c $server_svcconf_file");
$goodCL = $client->CreateProcess ("client", "-k file://$client_good_iorfile");
$badCL  = $client->CreateProcess ("client", "-b -k file://$client_bad_iorfile");

print "Starting server using shared profiles\n";

$server_status_shared = $sharedSV->Spawn ();

if ($server_status_shared != 0) {
    print STDERR "ERROR: server returned $server_status_shared\n";
    exit 1;
}

if ($server->WaitForFileTimed ($goodiorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_good_iorfile>\n";
    $sharedSV->Kill (); $sharedSV->TimedWait (1);
    exit 1;
}

$client_status_bad = $badCL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status_bad != 0) {
    print STDERR "ERROR: bad client returned $client_status_bad\n";
    $status = 1;
}

$client_good_status = $goodCL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_good_status != 0) {
    print STDERR "ERROR: good client returned $client_good_status\n";
    $status = 1;
}

$server_status_shared = $sharedSV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status_shared != 0) {
    print STDERR "ERROR: server [single profile per IOR] returned $server_status_shared\n";
    $status = 1;
}

$server->DeleteFile($goodiorbase);
$server->DeleteFile($badiorbase);
$client->DeleteFile($goodiorbase);
$client->DeleteFile($badiorbase);

print "Starting server using multiple profiles\n";

$multiSV = $server->CreateProcess ("server", "-ORBDottedDecimalAddresses 0 -ORBUseSharedProfile 0 -g $server_good_iorfile -b $server_bad_iorfile -p $port");

$server_status_multi = $multiSV->Spawn ();

if ($server_status_multi != 0) {
    print STDERR "ERROR: server returned $server_status_multi\n";
    exit 1;
}

if ($server->WaitForFileTimed ($goodiorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_good_iorfile>\n";
    $multiSV->Kill (); $multiSV->TimedWait (1);
    exit 1;
}

$client_bad_status = $badCL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_bad_status != 0) {
    print STDERR "ERROR: bad client returned $client_bad_status\n";
    $status = 1;
}

$client_good_status = $goodCL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_good_status != 0) {
    print STDERR "ERROR: good client returned $client_good_status\n";
    $status = 1;
}

$server_multi_status = $multiSV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_multi_status != 0) {
    print STDERR "ERROR: server [multiple profiles per IOR] returned $server_multi_status\n";
    $status = 1;
}

$server->DeleteFile($goodiorbase);
$server->DeleteFile($badiorbase);
$client->DeleteFile($goodiorbase);
$client->DeleteFile($badiorbase);

exit $status;
