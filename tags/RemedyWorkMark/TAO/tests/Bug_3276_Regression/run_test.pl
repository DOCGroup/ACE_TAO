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

my $controlbase = "control.ior";
my $server_controlfile = $server->LocalFile ($controlbase);
my $client_controlfile = $client->LocalFile ($controlbase);
$server->DeleteFile($controlbase);
$client->DeleteFile($controlbase);
my $proxybase = "proxy.ior";
my $server_proxyfile = $server->LocalFile ($proxybase);
my $client_proxyfile = $client->LocalFile ($proxybase);
$server->DeleteFile($proxybase);
$client->DeleteFile($proxybase);

$SV = $server->CreateProcess ("manager", "-ORBdebuglevel $debug_level -c $server_controlfile -p $server_proxyfile");
$CL = $client->CreateProcess ("client", "-c file://$client_controlfile -p file://$client_proxyfile");
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($controlbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_controlfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($controlbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_controlfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($controlbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_controlfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($controlbase);
$client->DeleteFile($controlbase);
$server->DeleteFile($proxybase);
$client->DeleteFile($proxybase);

exit $status;
