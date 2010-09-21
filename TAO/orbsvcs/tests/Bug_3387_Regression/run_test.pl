eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

#$Id$

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $nt_service = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server     = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client     = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $consumer1  = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $consumer2  = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

my $consumer_orb_port = 12345;
my $endpoint = "-ORBEndPoint iiop://localhost:$consumer_orb_port";

my $svc_conf  = "NotSvc.conf";
my $ns_ior    = "ns.ior";
my $nt_ior    = "notify.ior";
my $msngr_ior = "Messenger.ior";

my $nt_svc_conf = $nt_service->LocalFile ($svc_conf);

my $nm_ns_ior = $nm_service->LocalFile ($ns_ior);
my $nt_ns_ior = $nt_service->LocalFile ($ns_ior);
my $server_ns_ior = $server->LocalFile ($ns_ior);
my $client_ns_ior = $client->LocalFile ($ns_ior);
my $consumer1_ns_ior = $consumer1->LocalFile ($ns_ior);
my $consumer2_ns_ior = $consumer2->LocalFile ($ns_ior);
my $nt_nt_ior = $nt_service->LocalFile ($nt_ior);
my $server_msngr_ior = $server->LocalFile ($msngr_ior);
$nm_service->DeleteFile ($ns_ior);
$nt_service->DeleteFile ($ns_ior);
$server->DeleteFile ($ns_ior);
$client->DeleteFile ($ns_ior);
$consumer1->DeleteFile ($ns_ior);
$consumer2->DeleteFile ($ns_ior);
$nt_service->DeleteFile ($nt_ior);
$server->DeleteFile ($msngr_ior);

$NMS = $nm_service->CreateProcess("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                                  "-ORBdebuglevel $debug_level -o $nm_ns_ior");
$NFS = $nt_service->CreateProcess("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification",
                                  "-ORBdebuglevel $debug_level -ORBInitRef NameService=file://$nt_ns_ior ".
                                  "-IORoutput $nt_nt_ior -UseSeparateDispatchingORB 1 ".
                                  "-ORBSvcConf $nt_svc_conf");
$SV  = $server->CreateProcess("MessengerServer", "-ORBInitRef NameService=file://$server_ns_ior");
$MC1 = $consumer1->CreateProcess("MessengerConsumer", "-ORBInitRef NameService=file://$consumer1_ns_ior ".
                                                      "$endpoint -p 1");
$MC2 = $consumer2->CreateProcess("MessengerConsumer", "-ORBInitRef NameService=file://$consumer2_ns_ior ".
                                                      "$endpoint -p 2");
$CL  = $client->CreateProcess("MessengerClient", "-ORBInitRef NameService=file://$client_ns_ior");

# start Naming Service
$result = $NMS->Spawn();

if ($result != 0) {
    print STDERR "ERROR: naming service returned $result\n";
    exit 1;
}

if ($nm_service->WaitForFileTimed ($ns_ior,
                                  $nm_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file $nm_ns_ior\n";
    $NMS->Kill(); $NMS->TimedWait (1);
    exit 1;
}

if ($nm_service->GetFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nm_ns_ior>\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

if ($nt_service->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot set file <$nt_ns_ior>\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

if ($server->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server_ns_ior>\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_ns_ior>\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

if ($consumer1->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot set file <$consumer1_ns_ior>\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

if ($consumer2->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot set file <$consumer2_ns_ior>\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

# start Notification Service
$result = $NFS->Spawn();

if ($result != 0) {
    print STDERR "ERROR: notify service returned $result\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    exit 1;
}

# the ior file is only used to wait for the service to start
if ($nt_service->WaitForFileTimed ($nt_ior,
                                  $nt_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file $nt_nt_ior\n";
    $NMS->Kill (); $NMS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# start MessengerServer
$result = $SV->Spawn();

if ($result != 0) {
    $NMS->Kill (); $NMS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# Wait for the MessengerServer
if ($server->WaitForFileTimed ($msngr_ior, $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: Timed out waiting for $server_msngr_ior\n";
    $SV->Kill(); $SV->TimedWait (1);
    $NMS->Kill (); $NMS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# start first MessengerConsumer
$result = $MC1->Spawn();
if ($result != 0) {
    print STDERR "ERROR: Consumer 1 returned $result\n";
    $status = 1;
}

# start MessengerClient
$result = $CL->Spawn();
if ($result != 0) {
    print STDERR "ERROR: Client returned $result\n";
    $status = 1;
}

# wait for first MessengerConsumer to end
$result = $MC1->WaitKill ($consumer1->ProcessStopWaitInterval());
if ($result != 0) {
    print STDERR "ERROR: Consumer 1 returned $result\n";
    $status = 1;
}

# start second MessengerConsumer
$result = $MC2->SpawnWaitKill ($consumer2->ProcessStartWaitInterval());
if ($result != 0) {
    print STDERR "ERROR: Consumer 2 returned $result\n";
    $status = 1;
}

$CL->Kill(); $CL->TimedWait (1);
$SV->Kill(); $SV->TimedWait (1);
$NMS->Kill (); $NMS->TimedWait (1);
$NFS->Kill (); $NFS->TimedWait (1);

$nm_service->DeleteFile ($ns_ior);
$nt_service->DeleteFile ($ns_ior);
$server->DeleteFile ($ns_ior);
$client->DeleteFile ($ns_ior);
$consumer1->DeleteFile ($ns_ior);
$consumer2->DeleteFile ($ns_ior);
$nt_service->DeleteFile ($nt_ior);
$server->DeleteFile ($msngr_ior);

exit $status;
