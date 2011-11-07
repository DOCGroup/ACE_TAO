eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$sleeptime = 8;

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server3 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$port = $server1->RandomPort () + 10001; # This can't be 10000 on Chorus 4.0

$naming_ior = "NameService.ior";

$server1_naming_ior = $server1->LocalFile ($naming_ior);
$server2_naming_ior = $server2->LocalFile ($naming_ior);
$server3_naming_ior = $server3->LocalFile ($naming_ior);
$client_naming_ior = $client->LocalFile ($naming_ior);

$server1->DeleteFile ($naming_ior);
$server2->DeleteFile ($naming_ior);
$server3->DeleteFile ($naming_ior);
$client->DeleteFile ($naming_ior);


$SV1 = $server1->CreateProcess ("../../Naming_Service/tao_cosnaming",
                                "-ORBNameServicePort $port " .
                                "-o $server1_naming_ior");

$SV2 = $server2->CreateProcess ("../../Event_Service/tao_rtevent",
                                "-ORBInitRef NameService=file://$server2_naming_ior");

$SV3 = $server3->CreateProcess ("ECM_Supplier",
                                "-ORBInitRef NameService=file://$server3_naming_ior");

$CL = $client->CreateProcess ("ECM_Consumer",
                                "-ORBInitRef NameService=file://$client_naming_ior");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($server1->WaitForFileTimed ($naming_ior,
                               $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server1_naming_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server1->GetFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_naming_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server2_naming_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($server3->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server3_naming_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_naming_ior>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

sleep $sleeptime;

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

sleep $sleeptime;

$server_status = $SV3->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    $SV3->Kill (); $SV3->TimedWait (1);
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval() + 45);
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV3->WaitKill ($server3->ProcessStopWaitInterval() + 45);
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_status = $SV2->TerminateWaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}


$server_status = $SV1->TerminateWaitKill ($server1->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server1->DeleteFile ($naming_ior);
$server2->DeleteFile ($naming_ior);
$server3->DeleteFile ($naming_ior);
$client->DeleteFile ($naming_ior);

exit $status;
