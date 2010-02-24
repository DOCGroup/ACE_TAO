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

$hostname = $server->HostName ();
$port_iiop = $server->RandomPort ();
$port_diop = $server->RandomPort ();


$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBListenEndpoints iiop://:$port_iiop;diop://:$port_diop");

$CL = $client->CreateProcess ("client",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef UDPTest=corbaloc:diop:$hostname:$port_diop/UDPTest " .
                              "-ORBDefaultInitRef corbaloc:iiop:$hostname:$port_iiop");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sleep($server->ProcessStartWaitInterval() / 5);

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 75);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;
