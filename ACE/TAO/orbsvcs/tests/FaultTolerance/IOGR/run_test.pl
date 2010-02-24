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
my $manager = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $file1_ior = "file1.ior";
my $file2_ior = "file2.ior";
my $out_ior = "output.ior";

my $server1_file1_ior = $server1->LocalFile($file1_ior);
my $server2_file2_ior = $server2->LocalFile($file2_ior);
my $manager_file1_ior = $manager->LocalFile($file1_ior);
my $manager_file2_ior = $manager->LocalFile($file2_ior);
my $manager_out_ior = $manager->LocalFile($out_ior);

$server1->DeleteFile($file1_ior);
$server2->DeleteFile($file2_ior);
$manager->DeleteFile($file1_ior);
$manager->DeleteFile($file2_ior);
$manager->DeleteFile($out_ior);

$SV1 = $server1->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server1_file1_ior");
$SV2 = $server2->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server2_file2_ior");
$MN = $manager->CreateProcess ("Manager", "-a file://$manager_file1_ior -b file://$manager_file2_ior ".
                                          "-c $manager_out_ior");

print STDERR "Starting Server\n";

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

if ($manager->PutFile ($file1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$manager_file1_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print STDERR "Starting Server\n";

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

if ($server2->GetFile ($file2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_file2_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($manager->PutFile ($file2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$manager_file2_ior>\n";
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

print STDERR "Starting Manager\n";

$manager_status = $MN->SpawnWaitKill ($manager->ProcessStartWaitInterval() + 45);

if ($manager_status != 0) {
    print STDERR "ERROR: manager returned $manager_status\n";
    $status = 1;
}

$server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server 1 returned $server_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile($file1_ior);
$server2->DeleteFile($file2_ior);
$manager->DeleteFile($file1_ior);
$manager->DeleteFile($file2_ior);
$manager->DeleteFile($out_ior);

exit $status;
