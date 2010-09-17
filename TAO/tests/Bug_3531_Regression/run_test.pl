eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use POSIX "sys_wait_h";

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# constants
my $verbose = 1;
my $server_threads = 2;
my $num_clients = 5;
my $iorbase = "server.ior";
my $shutdown = "server_terminated";

# create test targets
my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my @clients = ();
for($i = 0; $i < $num_clients; $i++) {
    $clients[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target $i+1 failed\n";
}

# clean resources before start
my $server_iorfile = $server->LocalFile ($iorbase);
my $server_shutdownfile = $server->LocalFile ($shutdown);
$server->DeleteFile($iorbase);
$server->DeleteFile($shutdown);
my @clients_iorfile = ();
for($i = 0; $i < $num_clients; $i++) {
    $clients_iorfile[$i] = $clients[$i]->LocalFile ($iorbase);
    $clients[$i]->DeleteFile ($iorbase);
}

# create processes
$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level ".
                                        "-o $server_iorfile -n $server_threads ".
                                        "-x $server_shutdownfile");

my @CLS = ();
for($i = 0; $i < $num_clients - 1; $i++) {
    $CLS[$i] = $clients[$i]->CreateProcess ("client", "-k file://$clients_iorfile[$i]");
}
$CLS[$num_clients - 1] = $clients[$i]->CreateProcess ("client",
                                    "-k file://$clients_iorfile[$num_clients - 1] -x");

# start server and wait for file
print STDERR $SV->CommandLine(), "\n" if $verbose;
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

# deliver ior file to all consumers
if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

for($i = 0; $i < $num_clients; $i++) {
    if ($clients[$i]->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: client $i cannot set file <$clients_iorfile[$i]>\n";
        $status = 1; $i = $num_clients;
    }
}

if ($status != 0) {
    $SV->Kill (); $SV->TimedWait (1);
    exit $status;
}

# spawn clients in the loop and one in separated way
my $broken_number = 0;
for($i = 0; $i < $num_clients - 1; $i++) {
    print STDERR $CLS[$i]->CommandLine(), "\n" if $verbose;
    my $client_status = $CLS[$i]->Spawn();
    if ($client_status != 0) {
        print STDERR "ERROR: client $i Spawn returned $client_status\n";
        $broken_number = $i; $i = $num_clients;  $status = 1;
    }
}

if ($status != 0) {
    for($i = 0; $i < $broken_number; $i++) {
        $CLS[$i]->Kill (); $CLS[$i]->TimedWait (1);
    }

    $SV->Kill (); $SV->TimedWait (1);
    exit $status;
}

for($i = 0; $i < $num_clients - 1; $i++) {
    my $client_status = $CLS[$i]->WaitKill ($clients[$i]->ProcessStartWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client $i WaitKill returned $client_status\n";
        $broken_number = $i; $i = $num_clients; $status = 1;
    }
}

if ($status != 0) {
    for($i = $broken_number; $i < $num_clients - 1; $i++) {
        $CLS[$i]->Kill (); $CLS[$i]->TimedWait (1);
    }

    $SV->Kill (); $SV->TimedWait (1);
    exit $status;
}

#Server should still be running at this point, now start the client that asks
#the server to shut itself down

$client_status = $CLS[$num_clients - 1]->SpawnWaitKill ($clients[$num_clients - 1]->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client $num_clients - 1 returned $client_status\n";
    $status = 1;
}

#Server should now be shutdown
if ($server->WaitForFileTimed ($shutdown,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_shutdownfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$server->DeleteFile($shutdown);
for($i = 0; $i < $num_clients; $i++) {
    $clients[$i]->DeleteFile ($iorbase);
}

exit $status;
