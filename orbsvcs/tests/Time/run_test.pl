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
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $clerk = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $srv_ior = "server.ior";
my $clk_ior = "clerk.ior";

my $server_srv_ior = $server->LocalFile ($srv_ior);
my $clerk_srv_ior  = $clerk->LocalFile($srv_ior);
my $clerk_clk_ior  = $clerk->LocalFile($clk_ior);
my $client_clk_ior = $client->LocalFile ($clk_ior);

$server->DeleteFile($srv_ior);
$clerk->DeleteFile($srv_ior);
$clerk->DeleteFile($clk_ior);
$client->DeleteFile($clk_ior);

$SV = $server->CreateProcess ("../../Time_Service/tao_costime_server", "-o $server_srv_ior");
$CK = $server->CreateProcess ("../../Time_Service/tao_costime_clerk",
                              "-f $clerk_srv_ior -o $clerk_clk_ior -t 2");
$CL = $client->CreateProcess ("client", "-f $client_clk_ior");

$process_status = $SV->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($srv_ior,
                                $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_srv_ior>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($srv_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_srv_ior>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($clerk->PutFile ($srv_ior) == -1) {
    print STDERR "ERROR: cannot set file <$clerk_srv_ior>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$process_status = $CK->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: clerk returned $process_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($clerk->WaitForFileTimed ($clk_ior,
                              $clerk->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$clerk_clk_ior>\n";
    $CK->Kill (); $CK->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($clerk->GetFile ($clk_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$clerk_clk_ior>\n";
    $CK->Kill (); $CK->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($clk_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_clk_ior>\n";
    $CK->Kill (); $CK->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$process_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());
if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    $status = 1;
}

$process_status = $CK->TerminateWaitKill ($clerk->ProcessStopWaitInterval());
if ($process_status != 0) {
    print STDERR "ERROR: clerk returned $process_status\n";
    $status = 1;
}

$server->DeleteFile($srv_ior);
$clerk->DeleteFile($srv_ior);
$clerk->DeleteFile($clk_ior);
$client->DeleteFile($clk_ior);

exit $status;
