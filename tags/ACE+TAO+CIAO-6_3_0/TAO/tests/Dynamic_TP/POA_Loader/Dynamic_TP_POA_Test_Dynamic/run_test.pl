eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
$iorbase = "server.ior";
$server_iorfile = $server->LocalFile ($iorbase);
$client_iorfile = $client->LocalFile ($iorbase);
$deletelogs = 1;

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $status = 0;

sub count_strings
{
   my $log_file = shift;
   my $find_str = shift;
   open my $fh, '<', $log_file or die $!;
   my $cnt=0;
   while (<$fh>)
   {
      $f = quotemeta($find_str);
      $cnt += $_ =~ /$f/;
   }
    print STDERR "INFO: Count - [$cnt]\n";
    close $fh;
    return($cnt);
}

sub run_test
{
    my $test_num = shift;
    my $test_name = "";
    my $find_this = "";
    my $expected_cnt = 0;
    my $status = 0;
    my $num_clients = 1;
    my $valid_num_exceptions = 0;
    my $wait_sec = 0;
    my $client_ext_args = "";

    if ($test_num == 1) {
        $test_name = "initial_pool_thread";
        $find_this = "DTP_Task::svc() New thread created";
        $expected_cnt = 5;
    }
    elsif ($test_num == 2) {
        $test_name = "min_pool_thread";
        $find_this = "DTP_Task::svc() Existing thread expiring";
        $expected_cnt = 5;
        $wait_sec = 15;
    }
    elsif ($test_num == 3) {
        $test_name = "max_request_queue_depth";
        $num_clients = 15;
        $valid_num_exceptions = 5;
        $client_ext_args = "-e 0 -n $num_clients -t max_queue -g $valid_num_exceptions";
    }
    elsif ($test_num == 4) {
        $test_name = "max_pool_thread";
        $find_this = "Growing threadcount.";
        $expected_cnt = 4;
        $num_clients = 10;
        $valid_num_exceptions = 5;
        $client_ext_args = "-e 0 -n $num_clients";
    }
    else {
        print STDERR "ERROR: invalid test num $test_num\n";
        exit 1;
    }

    print "\nRunning Test $test_num, $test_name....\n";

    my $lfname = "server_test" . $test_num . ".log";
    my $scname = "svc" . $test_num . ".conf";
    my $server_iorfile = $server->LocalFile ($iorbase);
    my $client_iorfile = $client->LocalFile ($iorbase);
    my $server_logfile = $server->LocalFile ($lfname);
    my $svc_conf = $server->LocalFile($scname);

    # copy the configuation file
    if ($server->PutFile ($scname) == -1) {
        print STDERR "ERROR: cannot set file <$svc_conf>\n";
        return 1;
    }

    $server->DeleteFile($lfname);

    my $SV = $server->CreateProcess ("server", " -ORBDebugLevel 5 -ORBSvcConf $svc_conf -ORBLogFile $server_logfile -s 3 -o $server_iorfile");
    my $SC = $client->CreateProcess ("client", "-k file://$client_iorfile -s");

    my $CLT = $client->CreateProcess ("client", "-k file://$client_iorfile $client_ext_args") if ($test_num > 2);

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
    if ($client->PutFile ($iorbase) == -1) {
        print STDERR "ERROR: cannot set file <$client_iorfile>\n";
        $SV->Kill (); $SV->TimedWait (1);
        exit 1;
    }

    if ($wait_sec > 0) {
        # Sleep here for more than the timeout to let the threads die off and log.
        print STDERR "INFO: sleeping $wait_sec seconds\n";
        sleep($wait_sec);
    }

    if ($test_num > 2) {
        $client_status = $CLT->SpawnWaitKill ($client->ProcessStopWaitInterval() * $num_clients);
        if ($client_status != 0) {
            print STDERR "ERROR: client $i returned $client_status\n";
        }
    }

    $client_status = $SC->SpawnWaitKill ($client->ProcessStopWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
    }

    my $found_cnt = 0;
    if ($expected_cnt > 0) {
        if ($server->GetFile ($lfname) == -1) {
            print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }
        print STDERR "looking for $expected_cnt instances of \"$find_this\"\n";
        $found_cnt = count_strings ($lfname, $find_this);
    }

    if ($found_cnt != $expected_cnt) {
        print STDERR "ERROR: $test_name test failed w/$found_cnt instead of $expected_cnt\n";
        $status = 1;
    }
    elsif ($deletelogs) {
        $server->DeleteFile($lfname);
        unlink ($lfname);
      }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }

    $server->DeleteFile($iorbase);
    $client->DeleteFile($iorbase);

    return($status);
}

# Each test below will start up a server and optionally some clients.
# When a server gets a call it will sleep some number of seconds before responding.
# The -s parameter controls how many seconds it will sleep.
# The order is mandatory and must not contain spaces. It is defined as follows:
# min_pool_threads
# initial_pool_threads
# max_pool_threads
# thread_stack_size
# thread_timeout (in seconds)
# max_queue_request_depth

for ($i = 0; $i < 4; $i++) {
    $status += run_test ($i + 1);
}
exit $status;

