eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 2 failed\n";

$port1 = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0
$port2 = PerlACE::uniqueid () + 10002;  # This can't be 10000 for Chorus 4.0

my $iorbase1 = "server1.ior";
my $iorbase2 = "server2.ior";
my $shutdown_iorbase1 = "server1_shutdown.ior";
my $shutdown_iorbase2 = "server2_shutdown.ior";
my $server_iorfile1 = $server1->LocalFile ($iorbase1);
my $server_iorfile2 = $server2->LocalFile ($iorbase2);
my $shutdown_server_iorfile1 = PerlACE::LocalFile ($shutdown_iorbase1);
my $shutdown_server_iorfile2 = PerlACE::LocalFile ($shutdown_iorbase2);

$server1->DeleteFile($iorbase1);
$server1->DeleteFile($shutdown_iorbase1);
$server2->DeleteFile($iorbase2);
$server2->DeleteFile($shutdown_iorbase2);

$SV1 = $server1->CreateProcess ("server", "-ORBEndpoint iiop://localhost:$port1 -o $server_iorfile1 -s $shutdown_server_iorfile1");
$SV2 = $server2->CreateProcess ("server", "-ORBEndpoint iiop://localhost:$port2 -o $server_iorfile2 -s $shutdown_server_iorfile2");
$CL = $client->CreateProcess ("client", "-t 30");

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server 1 returned $server_status\n";
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($iorbase1,
                                $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($server1->WaitForFileTimed ($shutdown_iorbase1,
                                $server1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$shutdown_server_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($server2->WaitForFileTimed ($iorbase2,
                                $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile1>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($server2->WaitForFileTimed ($shutdown_iorbase2,
                                $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$shutdown_server_iorfile2>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}


$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}


# The client requests server1 shutdown during 3th request.
sleep 15;

# To be safe, we kill service 1.
$server_status = $SV1->Kill ();
if ($server_status != 0) {
  print STDERR "ERROR: kill server 1 returned $server_status\n";
  $SV2->Kill (); $SV2->TimedWait (1);
  $CL->Kill (); $CL->TimedWait (1);
  exit 1;
}

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
  print STDERR "ERROR: restart server 1 returned $server_status\n";
  $SV2->Kill (); $SV2->TimedWait (1);
  $CL->Kill (); $CL->TimedWait (1);
  exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());
if ($server_status != 0) {
    print STDERR "ERROR: server 1  returned $server_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());
if ($server_status != 0) {
    print STDERR "ERROR: server 2 returned $server_status\n";
    $status = 1;
}


$server1->GetStderrLog();
$server2->GetStderrLog();
$client->GetStderrLog();

$server1->DeleteFile($server_iorfile1);
$server1->DeleteFile($shutdown_server_iorfile1);
$server2->DeleteFile($server_iorfile2);
$server2->DeleteFile($shutdown_server_iorfile2);

exit $status;
