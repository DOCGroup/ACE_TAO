eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

###############################################################################
my $ACE_ROOT = $ENV{ACE_ROOT};

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;
use File::Copy;

use strict;
use POSIX "sys_wait_h";

my $cwd = getcwd();

if (!defined $ACE_ROOT) {
    print "Error: ACE_ROOT not defined.\n";
    return 1;
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

my $host = $test->HostName ();
my $port = 9999;
# this matches the orbendpoint used in start_all.conf
my $imr_corbaloc = "corbaloc::localhost:$port";
my $imr_initref  = "-orbinitref ImplRepoService=$imr_corbaloc/ImplRepoService";

my $combined_ior = "combined.ior";

my $imr_start_file    = $test->LocalFile ("imr_status");
my $server_start_file = $test->LocalFile ("server_status");
my $start_conf        = $test->LocalFile ("start_all.conf");
my $test_combined_ior = $test->LocalFile ($combined_ior);

my $cs_initref   = "-orbinitref ServiceConfig=file://$test_combined_ior";
my $test_initref = "-orbinitref Test=$imr_corbaloc";

my $COMB        = $test->CreateProcess ("combined_service");
my $SERV        = $test->CreateProcess ("test_server");
my $IMRUTIL     = $test->CreateProcess ("$ACE_ROOT/bin/tao_imr");
my $CLI         = $test->CreateProcess ("test");

my $test_server = $SERV->Executable ();

my @outputs = ();
my @output_files = ();

for (my $i = 0; $i < 4; $i++) {
    $outputs[$i] = "run_test_$i.log";
    $test->DeleteFile ($outputs[$i]);
    $output_files[$i] = $test->LocalFile ($outputs[$i]);
}

my $id_comb_log = 0;
my $id_serv_log = 1;
my $id_imru_log = 2;
my $id_cli_log = 3;

my $status = 0;

###############################################################################

sub print_output
{
    my $output_file = $_[0];
    open (TEST_OUTPUT, "< $output_file" ) or return -1;
    while (<TEST_OUTPUT>) {
        print;
    }
    close (TEST_OUTPUT);
}

sub cleanup
{
    my $output_file = $_[0];
    # Retrieve spawned process id from the log file.
    my @pids = ();

    if (! open (TEST_OUTPUT, "<$output_file")) {
        return 0;
    }

    my $i = 0;

    while (<TEST_OUTPUT>) {
        if (m/ImR Activator: Successfully started/) {
            chomp $_;
            my @words = split (/=/, $_);
            push(@pids, $words[1]);
            ++ $i;
        }
    }
    close (TEST_OUTPUT);

    # Kill the MessengerService processes spawed by activator if they are still running.
    my $size = @pids;

    for (my $i = 0; $i < $size; ++$i) {
        if (kill ('KILL', $pids[$i]) == 1) {
            print STDERR "ERROR: Killed the test server (pid=$pids[$i]) " .
                         "that was still running after IMR shutdown. \n";
            $status = 1;
        }
    }
}

sub do_test
{
    $test->DeleteFile ($combined_ior);

    my $start_time = time();
    print STDERR "Starting comb -f $start_conf\n";
    # First we start all the servers, including the test server
    $COMB->Arguments ("-ORBLogFile $output_files[$id_comb_log] -f $start_conf");
    my $ret = $COMB->Spawn ();
    if ($ret != 0) {
        print "ERROR : spawning combined service.\n";
        return $ret;
    }

    ## Wait a little bit for everything to get started
    sleep(10);
    print STDERR "Starting test_server -orbuseimr 1 $imr_initref\n";

    $SERV->Arguments ("-ORBLogFile $output_files[$id_serv_log] -orbuseimr 1 $imr_initref");
    $ret = $SERV->Spawn ();
    if ($ret != 0) {
        print "ERROR : spawning test server.\n";
        return $ret;
    }

    ## Wait a little bit for everything to get started
    sleep(10);
    print STDERR "Starting client\n";

    # The client should pass the simple test
    $CLI->Arguments ("-ORBLogFile $output_files[$id_cli_log] $test_initref/TestObject1");
    $ret = $CLI->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : spawning test client 1.\n";
        return $ret;
    }

    # The client should pass the simple test again, this time with obj #2
    $CLI->Arguments ("-ORBLogFile $output_files[$id_cli_log] $test_initref/TestObject2");
    $ret = $CLI->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : spawning test client 2.\n";
        return $ret;
    }
    print STDERR "Starting imr\n";

    # The server was autoregistered without any start information. We
    # need to update the registration with a command line so that the
    # activator can be used to re-launch it.
    $IMRUTIL->Arguments ("-ORBLogFile $output_files[$id_imru_log] $imr_initref ".
                         "update TestObject1 -l $host -c ".
                         "\"$test_server -orbuseimr 1 $imr_initref\"");
    $ret = $IMRUTIL->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : Updating TestObject1 cmdline.\n";
        return $ret;
    }

    # The new command line should now be registered, but only for TestObject1
    $IMRUTIL->Arguments ("-ORBLogFile $output_files[$id_imru_log] $imr_initref list -v");
    $ret = $IMRUTIL->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : Listing ImR Servers.\n";
        return $ret;
    }

    # Now we can kill the server.
    $IMRUTIL->Arguments ("-ORBLogFile $output_files[$id_imru_log] $imr_initref shutdown TestObject1");
    $ret = $IMRUTIL->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : Shutting down test server.\n";
        return $ret;
    }

    # Both TestObject1 and TestObject2 should now show up as "not running"
    $IMRUTIL->Arguments ("-ORBLogFile $output_files[$id_imru_log] $imr_initref list -v");
    $ret = $IMRUTIL->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : Listing ImR Servers.\n";
        return $ret;
    }
    print STDERR "Starting client\n";

    # The client should pass the simple test again, because the Activator will
    # restart test_server
    $CLI->Arguments ("-ORBLogFile $output_files[$id_cli_log] $test_initref/TestObject1");
    $ret = $CLI->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : spawning test client 3.\n";
        return $ret;
    }

    # The client should pass the simple test again with obj #2, because both
    # are started by the server
    $CLI->Arguments ("-ORBLogFile $output_files[$id_cli_log] $test_initref/TestObject2");
    $ret = $CLI->SpawnWaitKill ($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : spawning test client 4.\n";
        return $ret;
    }

    # We have to kill the server, since $SERV most likely does
    # not refer to the actual process anymore since a new server
    # was started by the ImR Activator.
    $IMRUTIL->Arguments ("-ORBLogFile $output_files[$id_imru_log] $imr_initref shutdown TestObject1");
    $ret = $IMRUTIL->SpawnWaitKill($test->ProcessStartWaitInterval());
    if ($ret != 0) {
        print "ERROR : Shutting down test server.\n";
        return $ret;
    }

    $ret = $COMB->TerminateWaitKill ($test->ProcessStopWaitInterval());
    if ($ret != 0) {
        print "ERROR : Terminating combined service.\n";
        return $ret;
    }

    $ret = $SERV->TerminateWaitKill ($test->ProcessStopWaitInterval());
    if ($ret != 0) {
        print "ERROR : Terminating test server.\n";
        return $ret;
    }

    $test->DeleteFile ($combined_ior);

    return $ret;
}

$status  = 0;

if (do_test () == -1) {
    $status = 1;
}

for (my $i = 0; $i < 4; $i++) {
    cleanup ($output_files[$i]);
    print_output ($output_files[$i]);
    $test->DeleteFile ($outputs[$i]);
}

# Regardless of the return value, ensure that the processes
# are terminated before exiting
$CLI->Kill();
$COMB->Kill();
$SERV->Kill();

exit $status;
