#
# $Id$
#

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

$server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$status = 0;
$iorbase = "server.ior";
$server_iorfile = $server->LocalFile($iorbase);
$client_iorfile = $client->LocalFile($iorbase);

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBDebuglevel $debug_level -ORBid AMI_Forward_server -o $server_iorfile");
$CL = $client->CreateProcess ("client", "-ORBdebuglevel $debug_level -ORBid AMI_Forward_client -k file://$client_iorfile");

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());
$server_status = $SV->Kill ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
