eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $svc_conf = 'svc.conf';

# copy the configuation files
if ($server->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($server->PutFile ($svc_conf.'.xml') == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf.'.xml').">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf.'.xml') == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf.'.xml').">\n";
    exit 1;
}

$number_of_clients = 4;
$status = 0;

@configurations =
    ({
        file => "ior_1",
        description => "Invoking methods on servant in default thread pool",
     },{
        file => "ior_2",
        description => "Invoking methods on servant in first RT thread pool (without lanes)",
     },{
        file => "ior_3",
        description => "Invoking methods on servant in second RT thread pool (with lanes)",
     },
     );

for $test (@configurations) {
    $server->DeleteFile ($test->{file});
    $client->DeleteFile ($test->{file});
}

sub run_clients
{
    my @parms = @_;
    $arg = $parms[0];
    $clients = $parms[1];


    for ($i = 0; $i < $clients; $i++) {
        $CL[$i] = $client->CreateProcess ("client", $arg);
        $CL[$i]->Spawn ();
    }

    for ($i = 0; $i < $clients; $i++) {
        $client_status = $CL[$i]->WaitKill ($client->ProcessStopWaitInterval () + 120);
        if ($client_status != 0) {
            print STDERR "ERROR: client returned $client_status\n";
            $status = 1;
            goto kill_server;
        }
    }
    print STDERR "Finished running clients";
}

$SV = $server->CreateProcess ("server");

$SV->Spawn ();

for $test (@configurations) {
    if ($server->WaitForFileTimed ($test->{file},
                               $server->ProcessStartWaitInterval()) == -1) {
        $server_status = $SV->TimedWait (1);
        if ($server_status == 2) {
            # Mark as no longer running to avoid errors on exit.
            $SV->{RUNNING} = 0;
            exit $status;
        }
        else {
            print STDERR "ERROR: cannot find ior file: $test->{file}\n";
            $status = 1;
            goto kill_server;
        }
    }
}

for $test (@configurations) {
    print STDERR "\n*************************************************************\n";
    print STDERR "$test->{description}\n";
    print STDERR "*************************************************************\n\n";

    $iorfile = $client->LocalFile ($test->{file});
    run_clients ("-k file://$iorfile", $number_of_clients);
    print STDERR "Prepare next cycle";
}

print STDERR "\n************************\n";
print STDERR "Shutting down the server\n";
print STDERR "************************\n\n";

$client_iorfile = $client->LocalFile ($configurations[0]->{file});
run_clients ("-k file://$client_iorfile -i 0 -x", 1);

kill_server:

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval () + $number_of_clients * 100);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

for $test (@configurations) {
    $client->DeleteFile ($test->{file});
    $server->DeleteFile ($test->{file});
}

exit $status
