eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$num_clients = 40;
$num_orb_threads = 4;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my @clients = ();
for ($i = 0; $i < $num_clients; $i++) {
    $clients[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target $i+1 failed\n";
}

my $iorbase = "server.ior";

my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$SV = $server->CreateProcess ("server_main", "-ORBdebuglevel $debug_level ".
                                             "-o $server_iorfile ".
                                             "-n $num_clients  -t $num_orb_threads");

my @CLS = ();
my @clients_iorfile = ();
for ($i = 0; $i < $num_clients; $i++) {
    $clients_iorfile[$i] = $clients[$i]->LocalFile ($iorbase);
    $clients[$i]->DeleteFile($iorbase);
    $CLS[$i] = $clients[$i]->CreateProcess ("client_main", "-i file://$clients_iorfile[$i]");
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
        print STDERR "ERROR: client $i cannot set file <$clients_iorfile[$i]>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

for ($i = 0; $i < $num_clients; $i++) {
    my $client_status = $CLS[$i]->Spawn();
    if ($client_status != 0) {
        print STDERR "ERROR: client $i Spawn returned $client_status\n";
        $status = 1;
    }
}

for ($i = 0; $i < $num_clients; $i++) {
    my $client_status = $CLS[$i]->WaitKill ($clients[$i]->ProcessStartWaitInterval() + 45);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
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
