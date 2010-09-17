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

# used in server
$iorbaseA = "iorA.ior";
$iorbaseB = "iorB.ior";

my $server_iorfileA = $server->LocalFile ($iorbaseA);
my $server_iorfileB = $server->LocalFile ($iorbaseB);
my $client_iorfileA = $client->LocalFile ($iorbaseA);
my $client_iorfileB = $client->LocalFile ($iorbaseB);
$server->DeleteFile($iorbaseA);
$server->DeleteFile($iorbaseB);
$client->DeleteFile($iorbaseA);
$client->DeleteFile($iorbaseB);

$SV = $server->CreateProcess ("server", "-ORBDebuglevel $debug_level  -a $server_iorfileA  -b $server_iorfileB");

$CL = $client->CreateProcess ("client", "-a file://$client_iorfileA -b file://$client_iorfileB");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbaseA,
                        $server->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfileA>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbaseA);
$server->DeleteFile($iorbaseB);
$client->DeleteFile($iorbaseA);
$client->DeleteFile($iorbaseB);

exit $status;
