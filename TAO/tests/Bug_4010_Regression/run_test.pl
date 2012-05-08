eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$cdebug_level = '0';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-cdebug') {
      $cdebug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $client_conf_file = "client.conf";
my $client_conf = $client->LocalFile ($client_conf_file);

# copy the configuation file
if ($client->PutFile ($client_conf_file) == -1) {
    print STDERR "ERROR: cannot set file <$client_conf>\n";
    exit 1;
}

$port = $server->RandomPort ();
$iterations = 4;
$loop_time = $server->ProcessStartWaitInterval() + $server->ProcessStopWaitInterval() + 2;

$SV1 = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile -ORBListenEndpoints iiop://:${port}");
$SV2 = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile -ORBListenEndpoints iiop://:${port}");
$CL = $client->CreateProcess ("client", "-ORBdebuglevel $cdebug_level -ORBSvcConf $client_conf -k file://$client_iorfile -t $loop_time -i $iterations");

print $SV1->CommandLine() . "\n\n";
print $CL->CommandLine() . "\n\n";

$server_status = $SV1->Spawn ();

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

$client_status = $CL->Spawn ();

$server_status = $SV1->WaitKill ($loop_time  + $server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

sleep 10;
print "\nSpawning second server\n\n";
$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1; 
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

$server_status = $SV2->WaitKill ( (2 * $loop_time) + $server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;

