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

my $n_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "ns.ior";
my $sleeptime = 6;
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
my $n_service_iorfile = $n_service->LocalFile($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$n_service->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level ".
                                        "-ORBInitRef NameService=file://$server_iorfile");
$CL = $client->CreateProcess ("client", "-ORBInitRef NameService=file://$client_iorfile");
$NS = $n_service->CreateProcess ("../../Naming_Service/tao_cosnaming", "-o $n_service_iorfile");

print STDERR "Starting tao_cosnaming\n";

$n_service_status = $NS->Spawn ();

if ($n_service->WaitForFileTimed ($iorbase,
                                  $n_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$n_service_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($n_service->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$n_service_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($server->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$server_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Server\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

sleep $sleeptime;

print STDERR "Starting Client\n";

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$n_service_status = $NS->TerminateWaitKill ($n_service->ProcessStopWaitInterval());

if ($n_service_status != 0) {
    print STDERR "ERROR: n_server returned $n_service_status\n";
    $status = 1;
}

$n_service->DeleteFile($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
