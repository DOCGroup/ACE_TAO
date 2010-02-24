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
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV_PORT = $server->RandomPort();
$SV_ARGS = "-ORBListenEndpoints iiop://:$SV_PORT -server_ior $server_iorfile";
$CL_ARGS = "-server_ior file://$client_iorfile";

$SV = $server->CreateProcess ("server");
$CL = $client->CreateProcess ("client");

sub run_only_client
{
    my $ret = $CL->SpawnWaitKill($client->ProcessStartWaitInterval());
    if ($ret != 0) {
        print STDERR "ERROR: client returned $ret\n";
        exit 1;
    }
}

sub run_client_n_server
{
    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);
    my $ret = $SV->Spawn();
    if ($ret != 0) {
        print STDERR "ERROR: server returned $ret\n";
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
    if ($client->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    $ret = $CL->SpawnWaitKill($client->ProcessStartWaitInterval());
    if ($ret != 0) {
        print STDERR "ERROR: client returned $ret\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }
    $ret = $SV->WaitKill ($server->ProcessStopWaitInterval());
    if ($ret != 0) {
        print STDERR "ERROR: server returned $ret\n";
        exit 1;
    }
    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);
}

sub test_timeouts
{
    print "test_timeouts 1 testing...\n";
    $CL->Arguments("-force_timeout -connect_timeout 200");
    run_only_client;
    print "test_timeouts 1 passed...\n";
    print "test_timeouts 2 testing...\n";
    # request timeout should override connect timeout
    $CL->Arguments("-force_timeout -request_timeout 100 -connect_timeout 200");
    run_only_client;
    print "test_timeouts 2 passed...\n";
    print "test_timeouts 3 testing...\n";
    $CL->Arguments("-use_twoway -force_timeout -connect_timeout 200");
    run_only_client;
    print "test_timeouts 3 passed...\n";
    print "test_timeouts 4 testing...\n";
    # request timeout should override connect timeout
    $CL->Arguments("-use_twoway -force_timeout -request_timeout 200 -connect_timeout 1000");
    run_only_client;
    print "test_timeouts 4 passed...\n";
    print "test_timeouts 5 testing...\n";
    # request_timeout ignored for other sync_scopes
    $CL->Arguments("-sync none -force_timeout -request_timeout 100 -connect_timeout 200 -max_request_time 30");
    run_only_client;
    print "test_timeouts 5 passed...\n";
    print "test_timeouts 6 testing...\n";
    $CL->Arguments("-sync delayed -force_timeout -request_timeout 100 -connect_timeout 200 -max_request_time 30");
    run_only_client;
    print "test_timeouts 6 passed...\n";
    return 0;
}

sub test_buffering
{
    print "test_buffering 1 testing...\n";
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_min 350");
    $CL->Arguments("$CL_ARGS -sync none -max_request_time 30");
    run_client_n_server;
    print "test_buffering 1 passed...\n";
    print "test_buffering 2 testing...\n";
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_min 350");
    $CL->Arguments("$CL_ARGS -sync delayed -max_request_time 30");
    run_client_n_server;
    print "test_buffering 2 passed...\n";
    print "test_buffering 3 testing...\n";
    # Using sleep() instead of orb->run() for the interval
    # should cause all requests to be sent at once.
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_max 50");
    $CL->Arguments("$CL_ARGS -sync none -max_request_time 30 -use_sleep -run_orb_time 500");
    run_client_n_server;
    print "test_buffering 3 passed...\n";
    print "test_buffering 4 testing...\n";
    # Even delayed buffering will work this way, because the
    # connection won't be established until the orb is run.
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_max 50");
    $CL->Arguments("$CL_ARGS -sync delayed -max_request_time 30 -use_sleep -run_orb_time 500 -make_request_queued");
    run_client_n_server;
    print "test_buffering 4 passed...\n";
    print "test_buffering 5 testing...\n";
    # However, if we connect first, then delayed buffering will
    # cause the data to be sent right away
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_min 350");
    $CL->Arguments("$CL_ARGS -sync delayed -max_request_time 30 -use_sleep -run_orb_time 500 -force_connect");
    run_client_n_server;
    print "test_buffering 5 passed...\n";
    print "test_buffering 6 testing...\n";
    # Forcing the connection won't help sync_none, because it always buffers
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_max 50");
    $CL->Arguments("$CL_ARGS -sync none -max_request_time 30 -use_sleep -run_orb_time 500 -force_connect");
    run_client_n_server;
    print "test_buffering 6 passed...\n";
    return 0;
}

# Set a buffer count trigger and a request timeout so that a
# predictable number will be discarded.
sub test_buffer_count_timeout
{
    print "test_buffer_count_timeout testing...\n";
    $SV->Arguments("$SV_ARGS -expected 2");
    $CL->Arguments("$CL_ARGS -sync none -buffer_count 5 -max_request_time 30 -request_timeout 10");
    run_client_n_server;
    print "test_buffer_count_timeout passed...\n";
    return 0;
}

sub test_buffer_bytes_timeout
{
    print "test_buffer_bytes_timeout testing...\n";
    $SV->Arguments("$SV_ARGS -expected 3");
    $CL->Arguments("$CL_ARGS -sync none -buffer_bytes 200 -max_request_time 30 -request_timeout 10");
    run_client_n_server;
    print "test_buffer_bytes_timeout passed...\n";
    return 0;
}

sub test_buffer_timeout
{
    print "test_buffer_timeout 1 testing...\n";
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_max 50 -first_min 1000");
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CL->Arguments("$CL_ARGS -sync none -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500");
    run_client_n_server;
    print "test_buffer_timeout 1 passed...\n";
    print "test_buffer_timeout 2 testing...\n";
    # delayed buffering should behave as above, because it will start out buffering
    # due to the connection not being established.  However, the when the
    # actual connection is made, it will flush the queued messages which
    # may take up to 550ms.
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_max 550 -first_min 990");
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CL->Arguments("$CL_ARGS -sync delayed -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500 -make_request_queued");
    run_client_n_server;
    print "test_buffer_timeout 2 passed...\n";
    print "test_buffer_timeout 3 testing...\n";
    # delayed buffering will ignore constraints if the connection is forced
    $SV->Arguments("$SV_ARGS -expected 10 -elapsed_min 450 -first_max 50");
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CL->Arguments("$CL_ARGS -sync delayed -force_connect -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500");
    run_client_n_server;
    print "test_buffer_timeout 3 passed...\n";
    return 0;
}

# test sending one request with buffering timeout constraint.
sub test_one_request
{
    print "test_one_request testing...\n";
    $SV->Arguments("$SV_ARGS -expected 1 -first_min 1000");
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CL->Arguments("$CL_ARGS -sync none -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500 -num_requests 1");
    run_client_n_server;
    print "test_one_request passed...\n";
    return 0;
}

$status |= test_timeouts();
$status |= test_buffering();
$status |= test_buffer_count_timeout();
$status |= test_buffer_bytes_timeout();
$status |= test_buffer_timeout();
$status |= test_one_request();

# Regardless of the return value, ensure that the processes
# are terminated before exiting

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());
if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$client_status = $CL->Kill ();
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
