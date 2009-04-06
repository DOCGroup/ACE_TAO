eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$server_static_threads = 1;
$server_dynamic_threads = 0;

$status = 0;

$continuous = ($^O eq 'hpux');
$common_args = ($continuous ? "-ORBSvcConf continuous$PerlACE::svcconf_ext" : '');

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";


@configurations =
    ({
        file => "ior_2",
        args => "-b 0",
        description => "Invoking methods on servant in thread lanes without bands",
     },{
        file => "ior_2",
        args => "-b 1",
        description => "Invoking methods on servant in thread lanes with bands",
     },{
        file => "ior_1",
        args => "-b 0",
        description => "Invoking methods on servant in thread pool without bands",
     },{
        file => "ior_1",
        args => "-b 1",
        description => "Invoking methods on servant in thread pool with bands",
     },);

sub run_test
{
    for $test (@configurations) {
        $server->DeleteFile ($test->{file});
    }

    my @parms = @_;
    $arg = $parms[0];

    $SV = $server->CreateProcess ("server", "$common_args -s $server_static_threads -d $server_dynamic_threads");
    
    $server_status = $SV->Spawn ();
    if ($server_status == -1) {
        exit $server_status;
    }

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
        print $test->{file}."\n";
    }

    $CL[$i] = $client->CreateProcess ("client", "$common_args $arg");
    $CL[$i]->Spawn ();

    $client_status = $CL[$i]->WaitKill ($client->ProcessStartWaitInterval (60));
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
        goto kill_server;
    }

kill_server:

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval (120));

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    for $test (@configurations) {
        $server->DeleteFile ($test->{file});
    }
}

for $test (@configurations) {
    print STDERR "\n*************************************************************\n";
    print STDERR "$test->{description}\n";
    print STDERR "*************************************************************\n\n";

    my $file = $server->LocalFile($test->{file});
    run_test ("-k file://$file $test->{args}");
}

exit $status

