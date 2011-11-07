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

$server->AddLibPath ('../TP_Foo_A/.');
$server->AddLibPath ('../TP_Foo_B/.');
$server->AddLibPath ('../TP_Foo_C/.');
$server->AddLibPath ('../TP_Common/.');

my $iorbase = "server.ior";
my $num_clients = 40;

my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server_main", "-ORBdebuglevel $debug_level ".
                                             "-o $server_iorfile -n $num_clients");

@clients = ();
@CLS = ();
@clients_iorfile = ();

for ($i = 0; $i < $num_clients; $i++) {
    $clients[$i] = PerlACE::TestTarget::create_target ($i + 2) || die "Create target " . ($i + 2) . " failed\n";

    $clients[$i]->AddLibPath ('../TP_Foo_A/.');
    $clients[$i]->AddLibPath ('../TP_Foo_B/.');
    $clients[$i]->AddLibPath ('../TP_Foo_C/.');
    $clients[$i]->AddLibPath ('../TP_Common/.');

    $clients_iorfile[$i] = $clients[$i]->LocalFile ($iorbase);
    $clients[$i]->DeleteFile($iorbase);

    $CLS[$i] = $clients[$i]->CreateProcess ("client_main", "-k file://$clients_iorfile[$i]");
}

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


for ($i = 0; $i < $num_clients; $i++) {
    if ($clients[$i]->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$clients_iorfile[$i]>\n";
        $i = $num_clients; $status = 1;
    }
}

if ($status != 0) {
    $SV->Kill (); $SV->TimedWait (1);
    exit $status;
}

my $broken_number = 0;
for ($i = 0; $i < $num_clients; $i++) {
    $client_status = $CLS[$i]->Spawn ();

    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
        $broken_number = $i; $i = $num_clients; $status = 1;
    }
}

if ($status != 0) {
    for ($i = 0; $i < $broken_number; $i++) {
        $CLS[$i] = Kill ();
    }

    $SV->Kill (); $SV->TimedWait (1);
    exit $status;
}

for ($i = 0; $i < $num_clients; $i++) {
    $client_status = $CLS[$i]->WaitKill ($clients[$i]->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
        $status = 1;
    }
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 45);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
for ($i = 0; $i < $num_clients; $i++) {
    $clients[$i]->DeleteFile($iorbase);
}

exit $status;
