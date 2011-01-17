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

my $server_a = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server_b = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $ior1file = "servera.ior";
my $ior2file = "serverb.ior";

#Files which used by server_a
my $server_a_ior1file = $server_a->LocalFile ($ior1file);
$server_a->DeleteFile ($ior1file);

#Files which used by server_b
my $server_b_ior2file = $server_b->LocalFile ($ior2file);
$server_b->DeleteFile ($ior2file);

#Files which used by client
my $client_ior1file = $client->LocalFile ($ior1file);
my $client_ior2file = $client->LocalFile ($ior2file);
$client->DeleteFile($ior1file);
$client->DeleteFile($ior2file);

$SV1 = $server_a->CreateProcess ("server_A",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_a_ior1file ");

$SV2 = $server_b->CreateProcess ("server_B",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_b_ior2file ");

$CL = $client->CreateProcess ("initiator",
                              "-ORBdebuglevel $debug_level " .
                              "-f $client_ior1file " .
                              "-g $client_ior2file ");

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

if ($server_a->WaitForFileTimed ($ior1file,
                               $server_a->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_a_ior1file>\n";
    KillServers();
    exit 1;
}

if ($server_b->WaitForFileTimed ($ior2file,
                               $server_b->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_b_ior2file>\n";
    KillServers();
    exit 1;
}


if ($server_a->GetFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_a_ior1file>\n";
    KillServers();
    exit 1;
}
if ($client->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$client_ior1file>\n";
    KillServers();
    exit 1;
}

if ($server_b->GetFile ($ior2file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_b_ior2file>\n";
    KillServers();
    exit 1;
}
if ($client->PutFile ($ior2file) == -1) {
    print STDERR "ERROR: cannot set file <$client_ior2file>\n";
    KillServers();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV1->WaitKill ($server_a->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server_b->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_a->DeleteFile ($ior1file);
$server_b->DeleteFile ($ior2file);
$client->DeleteFile($ior1file);
$client->DeleteFile($ior2file);

exit $status;
