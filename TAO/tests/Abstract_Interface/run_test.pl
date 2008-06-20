eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
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

$server = PerlACE::TestTarget::create_target (1);
$client = PerlACE::TestTarget::create_target (2);
if (!defined $server || !defined $client) {
    exit 1;
}

$iorfilebase = "test.ior";
$server_iorfile = $server->LocalFile ("$iorfilebase");
$client_iorfile = $client->LocalFile ("$iorfilebase");
$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

if (PerlACE::is_vxworks_test()) {
  $SV = new PerlACE::ProcessVX ("server",
                              "-ORBDottedDecimalAddresses 1 "
                            . " -o $iorfilebase");
}
else {
  $SV = $server->CreateProcess ("server",
                                "-ORBDottedDecimalAddresses 1 "
                                . " -o $server_iorfile");
}

$SV->Spawn ();

if ($server->WaitForFileTimed ($server_iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

if ($server->GetFile ($server_iorfile, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 
if ($client->PutFile ($iorbase, $client_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$CL = $client->CreateProcess ("client",
                              " -k file://$client_iorfile "
                              . " $debug "
                              . " $which_test");

$client_status = $CL->SpawnWaitKill (60);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill (20);

$server->DeleteFile($server_iorfile);
$client->DeleteFile($client_iorfile);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

exit $status;

