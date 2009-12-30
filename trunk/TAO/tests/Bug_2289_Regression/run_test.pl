eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$dotted_decimal_addresses = '1';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# We want the server to run on a fixed port
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBEndpoint iiop://:$port " .
                              "-o $server_iorfile " .
                              "-ORBDottedDecimalAddresses $dotted_decimal_addresses ");

$hostname = $server->HostName();

$CL = $client->CreateProcess ("client",
                              "-k corbaloc::$hostname:$port/collocated_ior_bound_in_remote_iortable " .
                              "-ORBCollocationStrategy thru_poa " .
                              "-ORBDottedDecimalAddresses $dotted_decimal_addresses ");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: Bug 2289 Regression failed. Client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());


if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
