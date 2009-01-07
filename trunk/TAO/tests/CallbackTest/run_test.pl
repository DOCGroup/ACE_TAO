eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target(2) || die "Create target 2 failed\n";

$iorbase = "server.ior";
$server_iorfile = $server->LocalFile ("$iorbase");
$client_iorfile = $client->LocalFile ("$iorbase");
$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);
$status = 0;

$SV = $server->CreateProcess ("server", "-o $server_iorfile");
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile");

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

$client_status = $CL->SpawnWaitKill (60);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client_status == -1) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill (60);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->GetStderrLog();

$server->DeleteFile ($iorbase);
$client->DeleteFile($iorbase);

exit $status;
