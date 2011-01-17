eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Getopt::Std;

$status = 0;

#get incoming arguments
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

local ($opt_i);

if (!getopts ('i:')) {
    print "Usage: run_test.pl [-i iterations]\n";
    exit 1;
}

my $iorbase = "server.ior";
my $concurrent_clients = 30;
my $iterations = 40;

if (defined $opt_i) {
    $iterations = $opt_i;
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my @clients = ();
for ($i = 0; $i < $concurrent_clients + 1; $i++) {
    $clients[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target $i+1 failed\n";
}


my $server_iorfile = $server->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");

my @CLS = ();
my @clients_iorfile = ();
for ($i = 0; $i < $concurrent_clients + 1; $i++) {
    $clients_iorfile[$i] = $clients[$i]->LocalFile ($iorbase);
    $clients[$i]->DeleteFile ($iorbase);
    $CLS[$i] = $clients[$i]->CreateProcess ("client", "-k file://$clients_iorfile[$i]");
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

for ($i = 0; $i < $concurrent_clients + 1; $i++) {
    if ($clients[$i]->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: client $i cannot set file <$clients_iorfile[$i]>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
}

$count = 0;
for ($i = 0; $i != $iterations; $i++) {
    # First spawn all the processes
    for ($j = 0; $j < $concurrent_clients; $j++) {
        $CLS[$j]->Spawn ();
        $count++;
    }
    # Now wait for each one
    for ($j = 0; $j < $concurrent_clients; $j++) {
        my $client_status = $CLS[$j]->WaitKill ($clients[$j]->ProcessStartWaitInterval());
        if ($client_status != 0) {
            print STDERR "ERROR: client $j returned $client_status in iteration $i\n";
            $status = 1;
        }
    }
    if ($count % 100 == 0) {
        print STDERR "Iteration $i has created $count clients\n";
    }
}


my $CL = $clients[$concurrent_clients]->CreateProcess ("client",
                         "-k file://$clients_iorfile[$concurrent_clients] -x");
my $client_status = $CL->SpawnWaitKill ($clients[$concurrent_clients]->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client $concurrent_clients returned $client_status during test shutdown\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
for ($i = 0; $i < $concurrent_clients + 1; $i++) {
    $clients[$i]->DeleteFile ($iorbase);
}

exit $status;
