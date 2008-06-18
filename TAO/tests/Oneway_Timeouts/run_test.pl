eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
my $ACE_ROOT = $ENV{ACE_ROOT};

if (!defined $ACE_ROOT) {
    print "Error: ACE_ROOT not defined.\n";
    return 1;
}

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::Copy;

use strict;

my $srv_ior = PerlACE::LocalFile ("server.ior");
my $CLI = new PerlACE::Process ("client");
my $SRV = new PerlACE::Process ("server");
if (PerlACE::is_vxworks_test()) {
    $SRV = new PerlACE::ProcessVX ("server");
}
my $SRV_PORT = PerlACE::random_port();
my $SRV_ARGS = "-orbendpoint iiop://:$SRV_PORT";

sub test_timeouts
{
    print "test_timeouts 1 testing...\n";
    $CLI->Arguments("-force_timeout -connect_timeout 200");
    my $ret = $CLI->SpawnWaitKill(15);
    if ($ret != 0) {
        return $ret;
    }
    print "test_timeouts 1 passed...\n";
    print "test_timeouts 2 testing...\n";
    # request timeout should override connect timeout
    $CLI->Arguments("-force_timeout -request_timeout 100 -connect_timeout 200");
    my $ret = $CLI->SpawnWaitKill(15);
    if ($ret != 0) {
        return $ret;
    }
    print "test_timeouts 2 passed...\n";
    print "test_timeouts 3 testing...\n";
    $CLI->Arguments("-use_twoway -force_timeout -connect_timeout 200");
    my $ret = $CLI->SpawnWaitKill(15);
    if ($ret != 0) {
        return $ret;
    }
    print "test_timeouts 3 passed...\n";
    print "test_timeouts 4 testing...\n";
    # request timeout should override connect timeout
    $CLI->Arguments("-use_twoway -force_timeout -request_timeout 200 -connect_timeout 1000");
    my $ret = $CLI->SpawnWaitKill(15);
    if ($ret != 0) {
        return $ret;
    }
    print "test_timeouts 4 passed...\n";
    print "test_timeouts 5 testing...\n";
    # request_timeout ignored for other sync_scopes
    $CLI->Arguments("-sync none -force_timeout -request_timeout 100 -connect_timeout 200 -max_request_time 30");
    my $ret = $CLI->SpawnWaitKill(15);
    if ($ret != 0) {
        return $ret;
    }
    print "test_timeouts 5 passed...\n";
    print "test_timeouts 6 testing...\n";
    $CLI->Arguments("-sync delayed -force_timeout -request_timeout 100 -connect_timeout 200 -max_request_time 30");
    my $ret = $CLI->SpawnWaitKill(15);
    if ($ret != 0) {
        return $ret;
    }
    print "test_timeouts 6 passed...\n";
    return $ret;
}

sub test_buffering
{
    print "test_buffering 1 testing...\n";
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_min 350");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync none -max_request_time 30");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffering 1 passed...\n";
    print "test_buffering 2 testing...\n";
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_min 350");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync delayed -max_request_time 30");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffering 2 passed...\n";
    print "test_buffering 3 testing...\n";
    # Using sleep() instead of orb->run() for the interval
    # should cause all requests to be sent at once.
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_max 50");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync none -max_request_time 30 -use_sleep -run_orb_time 500");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffering 3 passed...\n";
    print "test_buffering 4 testing...\n";
    # Even delayed buffering will work this way, because the
    # connection won't be established until the orb is run.
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_max 50");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync delayed -max_request_time 30 -use_sleep -run_orb_time 500");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffering 4 passed...\n";
    print "test_buffering 5 testing...\n";
    # However, if we connect first, then delayed buffering will
    # cause the data to be sent right away
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_min 350");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync delayed -max_request_time 30 -use_sleep -run_orb_time 500 -force_connect");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffering 5 passed...\n";
    print "test_buffering 6 testing...\n";
    # Forcing the connection won't help sync_none, because it always buffers
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_max 50");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync none -max_request_time 30 -use_sleep -run_orb_time 500 -force_connect");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffering 6 passed...\n";
    return 0;
}

# Set a buffer count trigger and a request timeout so that a
# predictable number will be discarded.
sub test_buffer_count_timeout
{
    print "test_buffer_count_timeout testing...\n";
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 2");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync none -buffer_count 5 -max_request_time 30 -request_timeout 10");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffer_count_timeout passed...\n";
    return 0;
}

sub test_buffer_bytes_timeout
{
    print "test_buffer_bytes_timeout testing...\n";
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 3");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    $CLI->Arguments("-sync none -buffer_bytes 200 -max_request_time 30 -request_timeout 10");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffer_bytes_timeout passed...\n";
    return 0;
}

sub test_buffer_timeout
{
    print "test_buffer_timeout 1 testing...\n";
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_max 50 -first_min 1000");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CLI->Arguments("-sync none -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffer_timeout 1 passed...\n";
    print "test_buffer_timeout 2 testing...\n";
    # delayed buffering should behave as above, because it will start out buffering
    # due to the connection not being established.  However, the when the
    # actual connection is made, it will flush the queued messages which
    # may take up to 550ms.
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_max 550 -first_min 990");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CLI->Arguments("-sync delayed -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffer_timeout 2 passed...\n";
    print "test_buffer_timeout 3 testing...\n";
    # delayed buffering will ignore constraints if the connection is forced
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 10 -elapsed_min 450 -first_max 50");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CLI->Arguments("-sync delayed -force_connect -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_buffer_timeout 3 passed...\n";
    return 0;
}

# test sending one request with buffering timeout constraint.
sub test_one_request
{
    print "test_one_request testing...\n";
    unlink $srv_ior;
    $SRV->Arguments("$SRV_ARGS -expected 1 -first_min 1000");
    if ($SRV->Spawn() != 0) {
        return 1;
    }
    if (PerlACE::waitforfile_timed ($srv_ior, $PerlACE::wait_interval_for_process_creation) != 0) {
        print STDERR "Error: IOR not found.\n";
        return 1;
    }
    # Must use run_orb_time so that the timer will fire, and to prevent sending the
    # test_done twoway request which would flush the queue.
    $CLI->Arguments("-sync none -buffer_timeout 1000 -max_request_time 30 -run_orb_time 1500 -num_requests 1");
    if ($CLI->SpawnWaitKill(15) != 0) {
        print STDERR "Error: Client failed.\n";
        return 1;
    }
    if ($SRV->WaitKill(5) != 0) {
        print STDERR "Error: Server failed.\n";
        return 1;
    }
    print "test_one_request passed...\n";
    return 0;
}

sub run_test
{
    my $ret = shift;
    my $name = shift;
    if ($ret != 0) {
        print "Subtest $name ERROR: $ret\n";
    }
    return $ret;
}

unlink $srv_ior;

my $ret = 0;

$ret |= run_test(test_timeouts(), 'test_timeouts');
$ret |= run_test(test_buffering(), 'test_buffering');
$ret |= run_test(test_buffer_count_timeout() ,'test_buffer_count_timeout');
$ret |= run_test(test_buffer_bytes_timeout(), 'test_buffer_bytes_timeout');
$ret |= run_test(test_buffer_timeout(), 'test_buffer_timeout');
$ret |= run_test(test_one_request(), 'test_one_request');

# Regardless of the return value, ensure that the processes
# are terminated before exiting
$CLI->Kill();
$SRV->Kill();

unlink $srv_ior;
exit $ret;
