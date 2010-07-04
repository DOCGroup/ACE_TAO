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
my @clients = ();
$clients[0] = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
$clients[1] = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my @clients_iorfile = ();
$clients_iorfile[0] = $clients[0]->LocalFile ($iorbase);
$clients_iorfile[1] = $clients[1]->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$clients[0]->DeleteFile($iorbase);
$clients[1]->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
my @CLS = ();
$CLS[0] = $clients[0]->CreateProcess ("client", "-k file://$clients_iorfile[0]");
$CLS[1] = $clients[1]->CreateProcess ("client", "-k file://$clients_iorfile[1] -x");
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

if ($clients[0]->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$clients_iorfile[0]>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($clients[1]->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$clients_iorfile[1]>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

for ($i = 0; $i != 30; $i++) {
    $client_status = $CLS[0]->SpawnWaitKill ($clients[0]->ProcessStartWaitInterval() + 285);

    if ($client_status != 0) {
        print STDERR "ERROR: client 0 returned $client_status\n";
        $status = 1;
    }
}

$client_status = $CLS[1]->SpawnWaitKill ($clients[1]->ProcessStartWaitInterval() + 285);

if ($client_status != 0) {
    print STDERR "ERROR: client 1 returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$clients[0]->DeleteFile($iorbase);
$clients[1]->DeleteFile($iorbase);

exit $status;
