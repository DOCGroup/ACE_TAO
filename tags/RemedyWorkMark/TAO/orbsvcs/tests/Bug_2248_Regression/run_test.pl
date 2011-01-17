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

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client  = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $file1_ior  = "file1.ior";
my $file2_ior  = "file2.ior";
my $iogr10_ior = "iogr_10.ior";
my $iogr11_ior = "iogr_11.ior";

my $server1_file1_ior = $server1->LocalFile ($file1_ior);
my $server1_iogr11_ior= $server1->LocalFile ($iogr11_ior);
my $server2_file2_ior = $server2->LocalFile ($file2_ior);
my $client_file1_ior  = $client->LocalFile ($file1_ior);
my $client_file2_ior  = $client->LocalFile ($file2_ior);
my $client_iogr10_ior = $client->LocalFile ($iogr10_ior);
my $client_iogr11_ior = $client->LocalFile ($iogr11_ior);

$server1->DeleteFile ($file1_ior);
$server1->DeleteFile ($iogr11_ior);
$server2->DeleteFile ($file2_ior);
$client->DeleteFile ($file1_ior);
$client->DeleteFile ($file2_ior);
$client->DeleteFile ($iogr10_ior);
$client->DeleteFile ($iogr11_ior);

$SV1 = $server1->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server1_file1_ior ".
                                          "-f file://$server1_iogr11_ior -k KEY1");

$SV2 = $server2->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server2_file2_ior -k KEY2");

$CL = $client->CreateProcess ("client", "-a file://$client_file1_ior -k KEY1 ".
                                        "-b file://$client_file2_ior -l KEY2 ".
                                        "-g $client_iogr10_ior -h $client_iogr11_ior");

print STDERR "Starting Server 1\n";

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server 1 returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($file1_ior,
                                $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_file1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($file1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_file1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($file1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_file1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print STDERR "Starting Server 2\n";

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->WaitForFileTimed ($file2_ior,
                                $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_file2_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($file2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_file2_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($file2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_file2_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print STDERR "Starting Client\n";
$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

if ($client->WaitForFileTimed ($iogr11_ior,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_iogr11_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

if ($client->GetFile ($iogr11_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$client_iogr11_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

if ($server1->PutFile ($iogr11_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server1_iogr11_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval() + 15);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

if ($server_status < 0) {
    print STDERR "ERROR: server 1 returned $server_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status < 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile ($file1_ior);
$server1->DeleteFile ($iogr11_ior);
$server2->DeleteFile ($file2_ior);
$client->DeleteFile ($file1_ior);
$client->DeleteFile ($file2_ior);
$client->DeleteFile ($iogr10_ior);
$client->DeleteFile ($iogr11_ior);

exit $status
