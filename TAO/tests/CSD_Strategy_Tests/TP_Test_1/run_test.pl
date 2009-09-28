eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

PerlACE::add_lib_path ('../TP_Foo_A/.');
PerlACE::add_lib_path ('../TP_Common/.');

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

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

if (PerlACE::waitforfile_timed ($iorbase,
                        $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorbase>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

for ($i = 0; $i < $num_clients; $i++) {

    $CLS[$i] = $client->CreateProcess ("client_main", "-i file://$client_iorfile");

    $CLS[$i]->Spawn ();
}

for ($i = 0; $i < $num_clients; $i++) {

    $client = $CLS[$i]->WaitKill ($client->ProcessStopWaitInterval());

    if ($client != 0) {
        print STDERR "ERROR: client $i returned $client\n";
        $status = 1;
    }
}

$server = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
