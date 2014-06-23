eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

# Amount of delay (in microseconds) that the sink-server delays a client reply.
$delay_time = 11000;

# Number of calls a client makes to the AMH server
$iterations = 100;

$conf = $PerlACE::svcconf_ext;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";


# File used to pass AMH server ior to its clients.
my $iorbase = "amh.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $server_conf = $server->LocalFile ("st_server" . $conf);

$SV = $server->CreateProcess ("st_server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBsvcconf $server_conf " .
                              "-o $server_iorfile " .
                              "-s $delay_time");

$CL = $client->CreateProcess ("client",
                              "-k file://$client_iorfile " .
                              "-n $iterations");


print STDERR "\n    Starting AMH Sink Server with $delay_time microseconds delayed response: \n\n";

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

# Run client.
print STDERR "\n     Client making $iterations calls to server: \n";
$evt_per_sec = 1000/$delay_time/1000;
print STDERR "\n(Expect less than $evt_per_sec Events/sec) \n\n";

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 60);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
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
