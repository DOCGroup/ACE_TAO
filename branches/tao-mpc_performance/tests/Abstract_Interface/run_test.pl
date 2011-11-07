eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$debug = "";
$which_test = "";
$status = 0;

foreach $i (@ARGV) {
    if ($i eq "-d") {
        $debug = $i;
    }
    else {
        $which_test = $i;
    }
}

my $server = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$iorbase = "test.ior";
$server_iorfile = $server->LocalFile ("$iorbase");
$client_iorfile = $client->LocalFile ("$iorbase");
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server",
                              "-ORBDottedDecimalAddresses 1 "
                              . " -o $server_iorfile");

$SV->Spawn ();

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

$CL = $client->CreateProcess ("client",
                              " -k file://$client_iorfile "
                              . " $debug "
                              . " $which_test");

$client_status = $CL->SpawnWaitKill ($server->ProcessStartWaitInterval());

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













