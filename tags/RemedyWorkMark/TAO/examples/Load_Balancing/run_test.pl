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

my $balancer = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "server.ior";
my $balancer_iorfile = $balancer->LocalFile ($iorbase);
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$balancer->DeleteFile($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$LB = $balancer->CreateProcess ("load_balancer", "-ORBdebuglevel $debug_level -o $balancer_iorfile");
$SV = $server->CreateProcess ("server", "-i file://$server_iorfile");
$CL = $client->CreateProcess ("client", "-i file://$client_iorfile -n 10");

print STDERR "\n    Starting Load Balancing Server and Identity Server \n\n";

# Run the load balancing server.
$balancer_status = $LB->Spawn ();

if ($balancer_status != 0) {
    print STDERR "ERROR: balancer returned $balancer_status\n";
    exit 1;
}

if ($balancer->WaitForFileTimed ($iorbase,
                                 $balancer->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$balancer_iorfile>\n";
    $LB->Kill (); $LB->TimedWait (1);
    exit 1;
}

if ($balancer->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$balancer_iorfile>\n";
    $LB->Kill (); $LB->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$server_iorfile>\n";
    $LB->Kill (); $LB->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $LB->Kill (); $LB->TimedWait (1);
    exit 1;
}

# Run the identity server, which registers its objects with the load
# balancing server.
$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $LB->Kill (); $LB->TimedWait (1);
    exit 1;
}

sleep ($server->ProcessStartWaitInterval());

# Run tests, i.e., run client with different command line options.

print STDERR "\n     Client using Round Robin Object Group (10 iterations): \n\n";
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $LB->Kill (); $LB->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\n     Client using Random Object Group (10 iterations): \n\n";
$CL->Arguments ("-r " . $CL->Arguments ());
$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $LB->Kill (); $LB->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$balancer_status = $LB->TerminateWaitKill ($balancer->ProcessStopWaitInterval());

if ($balancer_status != 0) {
    print STDERR "ERROR: server returned $balancer_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$balancer->DeleteFile($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
