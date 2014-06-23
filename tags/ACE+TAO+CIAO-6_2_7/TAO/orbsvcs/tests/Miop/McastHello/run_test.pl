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
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $ior1file = "server1.ior";
my $ior2file = "server2.ior.unused";

#Files which used by server1
my $server1_ior1file = $server1->LocalFile ($ior1file);
$server1->DeleteFile($ior1file);

#Files which used by server2
my $server2_ior2file = $server2->LocalFile ($ior2file);
$server2->DeleteFile($ior2file);

#Files which used by server2
my $client_ior1file = $client->LocalFile ($ior1file);
$client->DeleteFile($ior1file);

$SV1 = $server1->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server1_ior1file");

$SV2 = $server2->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server2_ior2file");

$CL = $client->CreateProcess ("client",
                              "-k file://$ior1file");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sub KillServers{
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
}

if ($server1->WaitForFileTimed ($ior1file,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_ior1file>\n";
    KillServers ();
    exit 1;
}

if ($server2->WaitForFileTimed ($ior2file,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_ior2file>\n";
    KillServers ();
    exit 1;
}

if ($server1->GetFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_ior1file>\n";
    KillServers ();
    exit 1;
}
if ($client->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$client_ior1file>\n";
    KillServers ();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile($ior1file);
$server2->DeleteFile($ior2file);
$client->DeleteFile($ior1file);

exit $status;
