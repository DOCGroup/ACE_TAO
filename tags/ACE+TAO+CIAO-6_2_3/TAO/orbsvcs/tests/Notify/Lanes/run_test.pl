eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

PerlACE::check_privilege_group();

$status = 0;
$debug_level = '0';

$experiment_timeout = 50;
$startup_timeout = 45;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$client1->AddLibPath ("../lib");
$client2->AddLibPath ("../lib");

my $ior1file = "naming.ior";
my $ior2file = "notify.ior";
my $client_ior = "supplier.ior";

# Config files for targets
my $server2_conf = $server2->LocalFile ("notify.conf");
my $client1_conf = $client1->LocalFile ("supplier.conf");
my $client2_conf = $client2->LocalFile ("consumer.conf");

# Files which used by server1
my $server1_ior1file = $server1->LocalFile ($ior1file);
$server1->DeleteFile($ior1file);

# Files which used by server2
my $server2_ior2file = $server2->LocalFile ($ior2file);
$server2->DeleteFile($ior2file);
my $server2_ior1file = $server2->LocalFile ($ior1file);
$server2->DeleteFile($ior1file);



# Files which used by client1
my $client1_ior1file = $client1->LocalFile ($ior1file);
my $client1_client_ior = $client1->LocalFile ($client_ior);
$client1->DeleteFile($ior1file);
$client1->DeleteFile($client_ior);

# Files which used by client2
my $client2_ior1file = $client2->LocalFile ($ior1file);
my $client2_client_ior = $client2->LocalFile ($client_ior);
$client2->DeleteFile($ior1file);
$client2->DeleteFile($client_ior);

$SV1 = $server1->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                              "-o $server1_ior1file");

$SV2 = $server2->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                                "-ORBInitRef NameService=file://$server2_ior1file " .
                                "-IORoutput $server2_ior2file " .
                                "-ORBSvcConf $server2_conf");

$CL1 = $client1->CreateProcess ("../Driver/Notify_Tests_Driver",
                                "-ORBdebuglevel $debug_level " .
                                "-ORBInitRef NameService=file://$client1_ior1file " .
                                "-IORoutput $client1_client_ior " .
                                "-ORBSvcConf $client1_conf");

$CL2 = $client2->CreateProcess ("../Driver/Notify_Tests_Driver",
                                "-ORBdebuglevel $debug_level " .
                                "-ORBInitRef NameService=file://$client2_ior1file " .
                                "-IORinput $client2_client_ior " .
                                "-ORBSvcConf $client2_conf");

# Naming start
$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server1->WaitForFileTimed ($ior1file,
                               $server1->ProcessStartWaitInterval() + $startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$server1_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($server1->GetFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server1_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($server2->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$server2_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($client1->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$client1_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}
if ($client2->PutFile ($ior1file) == -1) {
    print STDERR "ERROR: cannot set file <$client2_ior1file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$args = $SV2->Arguments ();
print STDERR "Running Notification with arguments: $args\n";

# Notification start
$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->WaitForFileTimed ($ior2file,
                               $server2->ProcessStartWaitInterval() + $startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$server2_ior2file>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$args = $SV2->Arguments ();
print STDERR "Running Supplier with arguments: $args\n";

# Supplier start

$client_status = $CL1->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned client_status\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

if ($client1->WaitForFileTimed ($client_ior,
                               $client1->ProcessStartWaitInterval() + $startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$client1_client_ior>\n";
    $CL1->Kill (); $CL1->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client1->GetFile ($client_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$client1_client_ior>\n";
    $CL1->Kill (); $CL1->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}
if ($client2->PutFile ($client_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client2_client_ior>\n";
    $CL1->Kill (); $CL1->TimedWait (1);
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
    exit 1;
}

$args = $CL2->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";

# Consumer start
$client_status = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + $experiment_timeout);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$CL1->Kill ();
$SV2->Kill ();
$SV1->Kill ();


$client2->DeleteFile($ior1file);
$client2->DeleteFile($client_ior);
$server2->DeleteFile($ior2file);
$server2->DeleteFile($ior1file);
$client1->DeleteFile($ior1file);
$client1->DeleteFile($client_ior);


exit $status;
