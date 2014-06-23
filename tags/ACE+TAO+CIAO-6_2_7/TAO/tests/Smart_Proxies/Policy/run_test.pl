eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $status = 0;
my $debug_level = '0';

foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase1 = "server1.ior";
my $iorbase2 = "server2.ior";
my $server1_iorfile = $server1->LocalFile ($iorbase1);
my $server2_iorfile = $server2->LocalFile ($iorbase2);
my $client_iorfile1 = $client->LocalFile ($iorbase1);
my $client_iorfile2 = $client->LocalFile ($iorbase2);
$server1->DeleteFile($iorbase1);
$server2->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

my $SV1 = $server1->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server1_iorfile");
my $SV2 = $server2->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server2_iorfile");
my $CL = $client->CreateProcess ("client",
                                 "-i file://$client_iorfile1 ".
                                 "-j file://$client_iorfile2");

print STDERR "\nrunning Smart Proxy test consisting of the client and ".
    "two servers with <one-shot> policy\n\n";

my $server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($iorbase1,
                                $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
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

if ($server1->GetFile ($iorbase1) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase1) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($server2->GetFile ($iorbase2) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase2) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

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

$server1->DeleteFile($iorbase1);
$server2->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

print STDERR "\nrunning Smart Proxy test consisting of the client and ".
    "two servers with <one-shot> policy disabled\n\n";

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($iorbase1,
                                $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
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

if ($server1->GetFile ($iorbase1) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase1) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($server2->GetFile ($iorbase2) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase2) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$CL->Arguments ("-f 0 -i file://$client_iorfile1 ".
                "-j file://$client_iorfile2");


$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

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

$server1->DeleteFile($iorbase1);
$server2->DeleteFile($iorbase2);
$client->DeleteFile($iorbase1);
$client->DeleteFile($iorbase2);

exit $status;
