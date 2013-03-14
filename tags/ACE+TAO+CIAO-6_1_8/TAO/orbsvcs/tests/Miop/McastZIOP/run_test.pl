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

my $server = PerlACE::TestTarget::create_target (1) || die "Create server failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create client failed\n";

my $iorfile = "server.ior";

#Files which used by server
my $server_iorfile = $server->LocalFile ($iorfile);
$server->DeleteFile($iorfile);

#Files which used by server2
my $client_iorfile = $client->LocalFile ($iorfile);
$client->DeleteFile($iorfile);

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_iorfile");

$CL = $client->CreateProcess ("client",
                              "-ORBAllowZIOPNoServerPolicies 1 " .
                              "-ORBdebuglevel $debug_level " .
                              "-k file://$iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sub KillServers{
    $SV->Kill (); $SV->TimedWait (1);
}

if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    KillServers ();
    exit 1;
}

if ($server->GetFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    KillServers ();
    exit 1;
}
if ($client->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    KillServers ();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 60);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}


$server->DeleteFile($iorfile);
$client->DeleteFile($iorfile);

exit $status;
