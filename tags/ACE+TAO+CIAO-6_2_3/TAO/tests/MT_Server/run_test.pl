eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$threads = '8';
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

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client1_iorfile = $client1->LocalFile ($iorbase);
my $client2_iorfile = $client2->LocalFile ($iorbase);
my $client3_iorfile = $client3->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

my $svcconf = "server$PerlACE::svcconf_ext";
my $server_svcfile = $server->LocalFile ($svcconf);

# Copy the configuration file to the target.
if ($server->PutFile ($svcconf) == -1) {
    print STDERR "ERROR: cannot set file <$server_svcfile>\n";
    exit 1;
}

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level ".
                              "-ORBSvcConf $server_svcfile ".
                              "-n $threads ".
                              "-o $server_iorfile");
$CL1 = $client1->CreateProcess ("client",
                                "-i 100 -k file://$client1_iorfile");
$CL2 = $client2->CreateProcess ("client",
                                "-i 100 -k file://$client2_iorfile");
$CL3 = $client3->CreateProcess ("client",
                                "-i 100 -k file://$client3_iorfile");

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

$client_status = $CL1->WaitKill ($client1->ProcessStopWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client 1 returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->WaitKill ($client2->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client 2 returned $client_status\n";
    $status = 1;
}

$client_status = $CL3->WaitKill ($client3->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client 3 returned $client_status\n";
    $status = 1;
}

$CL1->Arguments ("-i 100 -x -k file://$client1_iorfile");

$client_status = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 135);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client1->DeleteFile($iorbase);
$client2->DeleteFile($iorbase);
$client3->DeleteFile($iorbase);

exit $status;
