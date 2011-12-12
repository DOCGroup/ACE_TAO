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
my $client_iorbase = "client.ior";

my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $client_iorbase_file = $client->LocalFile ($client_iorbase);
$client->DeleteFile($client_iorbase);

my $server_conf_file = "server.conf";
my $server_conf = $server->LocalFile ($server_conf_file);


# copy the configuation file
if ($server->PutFile ($server_conf_file) == -1) {
    print STDERR "ERROR: cannot set file <$server_conf>\n";
    $SV->Kill (); $SV->TimedWait (1);
    return 1;
}

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBSvcConf $server_conf " .
                              "-o $server_iorfile");

$port = $client->RandomPort ();

$CL = $client->CreateProcess ("client",
                              "-ORBEndpoint iiop://:$port " .
                              "-k file://$client_iorfile " .
                              "-o $client_iorbase_file");

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

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: First client spawn returned $client_status. Indicates a problem other than a regression.\n";
    $status = 1;
}

if ($client_status == 0){
    $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

    if ($client_status != 0) {
        print STDERR "ERROR: REGRESSION - Second client spawn returned $client\n";
        $status = 1;
    }
}


$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$client->DeleteFile($client_iorbase);

exit $status;
