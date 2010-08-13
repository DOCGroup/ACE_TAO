eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$srv_threads = '4';
$cli_threads = '30';

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$srvdownfile = "server_terminated";
$server_srvdownfile = $server->LocalFile ($srvdownfile);

$SV = $server->CreateProcess ("server",
                              "-o $server_iorfile " .
                              "-n $srv_threads");

$CL = $client->CreateProcess ("client", "-k file://$client_iorfile -n $cli_threads");

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

$CL->Spawn ();
#Client will not terminate on its own, kill it after 30 seconds
$CL->TimedWait ($client->ProcessStartWaitInterval() + 15);
$CL->Kill ();

print "***Client has been killed***\n";

#Server should still be running at this point, now start the client that asks
#the server to shut itself down

$CL->Arguments ("-k file://$client_iorfile -x");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

#Server should now be shutdown
if ($server->WaitForFileTimed ($srvdownfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_srvdownfile>".
        " -- server has not shut down cleanly.\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$status_server = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($status_server != 0) {
    print STDERR "ERROR: server returned $status_server\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);
$server->DeleteFile ($srvdownfile);

exit $status;
