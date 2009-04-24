eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$status = 0;

$iiop_port = 27532;
$tp_iiop_port = 27533;

$continuous = ($^O eq 'hpux');

$extra_server_args = ($continuous ? "-ORBSvcConf continuous$PerlACE::svcconf_ext " : '') .
                     "-d 1 -ORBEndpoint iiop://1.2\@:$iiop_port -ORBLaneEndpoint 2:0 iiop://1.2\@:$tp_iiop_port";

@iorfiles =
    (
     "not_used_ior_1",
     "not_used_ior_2",
     "persistent_ior",
     "transient_ior",
     "tp_persistent_ior",
     );

@configurations =
    ({
         iorfiles => [ "persistent_ior", "tp_persistent_ior", "transient_ior" ],
         server => "-a ". $server->LocalFile ("tp_persistent_ior") ." -p ". 
                          $server->LocalFile ("persistent_ior") ." -t ".
                          $server->LocalFile ("transient_ior") ." $extra_server_args",
         clients => [ "-k file://".$client->LocalFile("tp_persistent_ior"),
                      "-k file://".$client->LocalFile("persistent_ior"),
                      "-k file://".$client->LocalFile("transient_ior")." -x" ],
     }, {
         iorfiles => [ "not_used_ior_1", "not_used_ior_2", "transient_ior" ],
         server => "-a ". $server->LocalFile ("not_used_ior_1") ." -p ". 
                          $server->LocalFile ("not_used_ior_2") ." -t ".
                          $server->LocalFile ("transient_ior") ." $extra_server_args",
         clients => [ "-k file://".$client->LocalFile("tp_persistent_ior"),
                      "-k file://".$client->LocalFile("persistent_ior"),
                      "-k file://".$client->LocalFile("transient_ior")." -x" ],
     },
     );

sub run_client
{
    print "\nRunning client with the following args: @_\n\n";

    $CL = $client->CreateProcess ("client", @_);

    $CL->Spawn ();

    $client_status = $CL->WaitKill ($client->ProcessStopWaitInterval ()+200);

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
        zap_server (1);
    }
}

sub run_server
{
    my @parms = @_;
    my $args = $parms[0];
    my $iorfiles = $parms[1];

    print "\nRunning server with the following args: $args\n\n";

    $SV = $server->CreateProcess ("server", $args);
    $SV->Spawn ();

    for $file (@$iorfiles) {
        if ($server->WaitForFileTimed ($file,
                                      $server->ProcessStartWaitInterval ()) == -1) {
            print STDERR "ERROR: cannot find ior file: $file\n";
            $status = 1;
            zap_server (1);
        }
    }
}

sub zap_server
{
    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    if ($_[0]) {
        for $file (@iorfiles) {
            $server->DeleteFile ($file);
        }

        exit $status;
    }
}

for $file (@iorfiles) {
    $server->DeleteFile ($file);
    $client->DeleteFile ($file);
}

for $test (@configurations) {
    print STDERR "\n******************************************************\n";

    run_server ($test->{server}, $test->{iorfiles});

    my $clients = $test->{clients};
    for $args (@$clients) {
        run_client ($args);
    }

    zap_server (0);
}

for $file (@iorfiles) {
    $server->DeleteFile ($file);
    $client->DeleteFile ($file);
}

exit $status;
