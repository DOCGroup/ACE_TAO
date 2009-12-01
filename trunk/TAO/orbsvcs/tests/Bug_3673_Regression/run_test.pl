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

my $iorbase = "ns.ior";
my $shutdown_ior = "shutdown.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $server_sdiorfile = $server->LocalFile ($shutdown_ior);
my $client_sdiorfile = $client->LocalFile ($shutdown_ior);
$server->DeleteFile($iorbase);
$server->DeleteFile($shutdown_ior);
$client->DeleteFile($shutdown_ior);

$SV = $server->CreateProcess ("nsmain", "-ORBdebuglevel $debug_level -o $server_iorfile -s $server_sdiorfile");
$CL = $client->CreateProcess ("client", "-k file://$client_sdiorfile");
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

if ($server->GetFile ($shutdown_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_sdiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($shutdown_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_sdiorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

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
$server->DeleteFile($shutdown_ior);
$client->DeleteFile($shutdown_ior);

exit $status;
