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

my($prog_server) = 'MonitorManager';
my($prog_client) = 'MonitorClient';

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$SV = $server->CreateProcess ($prog_server, "-ORBdebuglevel $debug_level");
$CL = $client->CreateProcess ($prog_client);

$status_server = $SV->Spawn();


sleep($server->ProcessStartWaitInterval()/3);

$status_client = $CL->SpawnWaitKill ($server->ProcessStartWaitInterval() + 10);

if ($status_client != 0) {
    print STDERR "ERROR: $prog_server returned $status_server\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;
