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
$port = $server->RandomPort ();

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level " .
                              "-ORBEndpoint iiop://$hostname:$port");

$CL = $client->CreateProcess ("client", "-h $hostname -p $port");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

$client_status = $CL->Spawn ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    exit 1;
}

sleep($client->ProcessStartWaitInterval() / 3);

print STDERR "Kill server\n";

$SV->Kill ();

sleep($client->ProcessStartWaitInterval() / 3);


print STDERR "Restart server\n";

$server_status = $SV->Spawn ();

sleep($server->ProcessStartWaitInterval() + 15);

exit $status;
