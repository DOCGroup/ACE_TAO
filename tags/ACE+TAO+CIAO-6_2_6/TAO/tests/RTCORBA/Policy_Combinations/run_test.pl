eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

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

@iorfiles =
    (
     "root",
     "child",
     "default_pool_no_bands_client_propagated",
     "default_pool_no_bands_server_declared",
     "no_lanes_no_bands_client_propagated",
     "no_lanes_no_bands_server_declared",
     "lanes_no_bands_client_propagated",
     "lanes_no_bands_server_declared",
     "default_pool_bands_client_propagated",
     "default_pool_bands_server_declared",
     "no_lanes_bands_client_propagated",
     "no_lanes_bands_server_declared",
     "lanes_bands_client_propagated",
     "lanes_bands_server_declared",
     );

sub run_client
{
    $CL = $client->CreateProcess ("client", @_);

    $CL->Spawn ();

    $client_status = $CL->WaitKill ($client->ProcessStartWaitInterval ());

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
        goto kill_server;
    }
}

for $file (@iorfiles) {
    $server->DeleteFile ($file);
}

$SV = $server->CreateProcess ("server");

$SV->Spawn ();

for $file (@iorfiles) {
    $server_iorfile = $server->LocalFile ($file);
    $client_iorfile = $server->LocalFile ($file);
    if ($server->WaitForFileTimed ($file,
                                   $server->ProcessStartWaitInterval()) == -1) {
        $server_status = $SV->TimedWait (1);
        if ($server_status == 2) {
            # Mark as no longer running to avoid errors on exit.
            $SV->{RUNNING} = 0;
            exit $status;
        }
        else {
            print STDERR "ERROR: cannot find ior file: $server_iorfile\n";
            $status = 1;
            goto kill_server;
        }
    }

    print STDERR "\n******************************************************\n";
    print STDERR "Invoking methods on servant in $file poa\n";
    print STDERR "******************************************************\n\n";

    run_client ("-k file://$client_iorfile");
}

print STDERR "\n**************************\n";
print STDERR "Shutting down the server\n";
print STDERR "**************************\n\n";

$ior_file = $client->LocalFile ($iorfiles[0]);
run_client ("-k file://$ior_file -i 0 -x");

kill_server:
    print STDERR "Killing server...\n";
    $server_status = $SV->Kill ($server->ProcessStopWaitInterval ());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    for $file (@iorfiles) {
        $server->DeleteFile ($file);
    }

exit $status;
