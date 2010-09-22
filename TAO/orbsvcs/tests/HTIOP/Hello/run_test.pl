eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ns_service = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $host = $ns_service->HostName();
my $server_port = 8088;
my $name_port = 8087;

my $srv_conf = "outside.conf";
my $srv_ior = "server.ior";
my $ns_ior = "ns.ior";

my $ns_service_srv_conf = $ns_service->LocalFile($srv_conf);
my $server_srv_conf = $server->LocalFile($srv_conf);
my $client_srv_ior = $client->LocalFile ($srv_ior);
my $ns_service_ns_ior = $ns_service->LocalFile($ns_ior);
my $server_ns_ior = $ns_service->LocalFile($ns_ior);
my $server_srv_ior = $server->LocalFile ($srv_ior);

$ns_service->DeleteFile($ns_ior);
$server->DeleteFile($ns_ior);
$server->DeleteFile($srv_ior);
$client->DeleteFile($srv_ior);

$client_param_ior = "file://$client_srv_ior";
foreach $i (@ARGV) {
    if ($i eq '-corbaloc') {
        $client_param_ior = "corbaloc:htiop:$host:$server_port/HelloObj";
    }
    elsif ($i eq '-corbaname') {
        $client_param_ior = "corbaname:htiop:$host:$name_port#HelloObj";
#        $use_ns = 1;
    }
    elsif ($i eq '-debug') {
        $debug_level = '10';
    }
}

$NS = $ns_service->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                                  "-ORBSvcConf $ns_service_srv_conf -ORBEndpoint \"iiop://;".
                                  "htiop://$host:$name_port\" -o $ns_service_ns_ior");

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_srv_ior ".
                                        "-ORBInitRef NameService=file://$server_ns_ior ".
                                        "-ORBSvcConf $server_srv_conf ".
                                        "-ORBEndpoint htiop://$host:$server_port");

$CL = $client->CreateProcess ("client", "-k $client_param_ior");

$NS->IgnoreExeSubDir ();

$server_status = $NS->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: naming service returned $server_status\n";
    exit 1;
}

if ($ns_service->WaitForFileTimed ($ns_ior,
                                   $ns_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_service_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns_service->GetFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_service_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($server->PutFile ($ns_ior) == -1) {
    print STDERR "ERROR: cannot set file <$server_ns_ior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "Waiting for server to start\n";
$server_status = $SV->Spawn ();
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($srv_ior,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_srv_ior>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($srv_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_srv_ior>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($srv_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_srv_ior>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "Running Client\n";
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$NS->Kill ();
$NS->TimedWait (1);

$ns_service->DeleteFile($ns_ior);
$server->DeleteFile($ns_ior);
$server->DeleteFile($srv_ior);
$client->DeleteFile($srv_ior);

exit $status;
