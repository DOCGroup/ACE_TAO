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

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client");

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

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\nFlushing based on implicit message counts\n\n";

$CL->Arguments ("-k file://$client_iorfile -c 5 -b -1 -f -1 -t -1");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\nFlushing based on implicit message bytes\n\n";

$CL->Arguments ("-k file://$client_iorfile -b 250 -c -1 -f -1 -t -1");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\nFlushing based on implicit timeout\n\n";

$CL->Arguments ("-k file://$client_iorfile -t 5000 -b -1 -c -1 -f -1");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "\nExplicit queue flushing (and server shutdown)\n\n";

$CL->Arguments ("-k file://$client_iorfile -f 5 -b -1 -c 100 -t -1 -x");

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

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
