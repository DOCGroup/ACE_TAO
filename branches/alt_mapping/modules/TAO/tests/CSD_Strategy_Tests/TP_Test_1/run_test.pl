eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$server->AddLibPath ('../TP_Foo_A/.');
$client->AddLibPath ('../TP_Foo_A/.');
$server->AddLibPath ('../TP_Common/.');
$client->AddLibPath ('../TP_Common/.');

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$status = 0;

$num_clients=40;

$SV = $server->CreateProcess ("server_main", "-o $server_iorfile -n $num_clients");

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

for ($i = 0; $i < $num_clients; $i++) {

    $CLS[$i] = $client->CreateProcess ("client_main", "-i file://$client_iorfile");

    $CLS[$i]->Spawn ();
}

for ($i = 0; $i < $num_clients; $i++) {

    $client_status = $CLS[$i]->WaitKill ($client->ProcessStopWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
        $status = 1;
    }
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
