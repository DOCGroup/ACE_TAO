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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client1 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client2 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client3 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $client4 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $client5 = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $client6 = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";
my $client7 = PerlACE::TestTarget::create_target (8) || die "Create target 8 failed\n";
my $client8 = PerlACE::TestTarget::create_target (9) || die "Create target 9 failed\n";
my $client9 = PerlACE::TestTarget::create_target (10) || die "Create target 10 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client3_iorfile = $client3->LocalFile ($iorbase);
my $client4_iorfile = $client4->LocalFile ($iorbase);
my $client5_iorfile = $client5->LocalFile ($iorbase);
my $client6_iorfile = $client6->LocalFile ($iorbase);
my $client7_iorfile = $client7->LocalFile ($iorbase);
my $client8_iorfile = $client8->LocalFile ($iorbase);
my $client9_iorfile = $client9->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);
$client4->DeleteFile($iorbase);
$client5->DeleteFile($iorbase);
$client6->DeleteFile($iorbase);
$client7->DeleteFile($iorbase);
$client8->DeleteFile($iorbase);
$client9->DeleteFile($iorbase);

my $hostname = $server->HostName ();

$SV = $server->CreateProcess ("MessengerServer",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBListenEndpoints iiop://$hostname " .
                              "-o $server_iorfile");
$CL1 = $client1->CreateProcess ("MessengerClient", "-k file://$client1_iorfile");
$CL2 = $client2->CreateProcess ("MessengerClient", "-k file://$client2_iorfile");
$CL3 = $client3->CreateProcess ("MessengerClient", "-k file://$client3_iorfile");
$CL4 = $client4->CreateProcess ("MessengerClient", "-k file://$client4_iorfile");
$CL5 = $client5->CreateProcess ("MessengerClient", "-k file://$client5_iorfile");
$CL6 = $client6->CreateProcess ("MessengerClient", "-k file://$client6_iorfile");
$CL7 = $client7->CreateProcess ("MessengerClient", "-k file://$client7_iorfile");
$CL8 = $client8->CreateProcess ("MessengerClient", "-k file://$client8_iorfile");
$CL9 = $client9->CreateProcess ("MessengerClient", "-k file://$client9_iorfile");

print STDOUT "Starting MessengerServer\n";
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client1->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client1_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client2->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client2_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client3->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client3_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client4->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client4_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client5->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client5_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client6->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client6_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client7->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client7_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client8->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client8_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client9->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client9_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDOUT "\n\nStarting 9 MessengerClients.\n";
print STDOUT "The server should use different threads to handle requests.\n\n";

$client_status = $CL1->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL2->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    exit 1;
}

$client_status = $CL3->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    exit 1;
}

$client_status = $CL4->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    exit 1;
}

$client_status = $CL5->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    $CL4->Kill (); $CL4->TimedWait (1);
    exit 1;
}

$client_status = $CL6->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    $CL4->Kill (); $CL4->TimedWait (1);
    $CL5->Kill (); $CL5->TimedWait (1);
    exit 1;
}

$client_status = $CL7->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    $CL4->Kill (); $CL4->TimedWait (1);
    $CL5->Kill (); $CL5->TimedWait (1);
    $CL6->Kill (); $CL6->TimedWait (1);
    exit 1;
}

$client_status = $CL8->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    $CL4->Kill (); $CL4->TimedWait (1);
    $CL5->Kill (); $CL5->TimedWait (1);
    $CL6->Kill (); $CL6->TimedWait (1);
    $CL7->Kill (); $CL7->TimedWait (1);
    exit 1;
}

$client_status = $CL9->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    $CL2->Kill (); $CL2->TimedWait (1);
    $CL3->Kill (); $CL3->TimedWait (1);
    $CL4->Kill (); $CL4->TimedWait (1);
    $CL5->Kill (); $CL5->TimedWait (1);
    $CL6->Kill (); $CL6->TimedWait (1);
    $CL7->Kill (); $CL7->TimedWait (1);
    $CL8->Kill (); $CL8->TimedWait (1);
    exit 1;
}

$client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL3->WaitKill ($client3->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL4->WaitKill ($client4->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL5->WaitKill ($client5->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL6->WaitKill ($client6->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL7->WaitKill ($client7->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL8->WaitKill ($client8->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL9->WaitKill ($client9->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$SV->Kill ();

$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);
$client4->DeleteFile($iorbase);
$client5->DeleteFile($iorbase);
$client6->DeleteFile($iorbase);
$client7->DeleteFile($iorbase);
$client8->DeleteFile($iorbase);
$client9->DeleteFile($iorbase);

exit $status;
