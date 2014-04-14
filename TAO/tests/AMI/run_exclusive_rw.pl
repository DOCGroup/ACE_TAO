eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$client_conf_base = "exclusive_rw$PerlACE::svcconf_ext";
$client_conf = $client->LocalFile ($client_conf_base);
if ($client->PutFile ($client_conf_base) == -1) {
    print STDERR "ERROR: cannot set file <$client_conf>\n";
    exit 1;
}

$debug = 0;
$iterations = 5;
$threads = 5;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug = 10;
    }
}

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $server_dbg = "-ORBdebuglevel $debug -ORBLogFile server.log" if ($debug > 0);
my $client_dbg = "-ORBdebuglevel $debug -ORBLogFile client.log" if ($debug > 0);

$SV = $server->CreateProcess ("server", "$server_dbg -o $server_iorfile");

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
    print STDERR "ERROR: cannot get file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL = $client->CreateProcess ("client",
                              "-ORBsvcconf $client_conf $client_dbg"
                              . " -k file://$client_iorfile "
                              . " -n $threads -i $iterations -d -x");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

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
