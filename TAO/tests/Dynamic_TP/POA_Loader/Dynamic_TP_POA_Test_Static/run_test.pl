eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;


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

sub test_1
{
# Test 1:
# This is a test for the creation of initial_pool_threads.
# The test will start up a server with default pool values
# and will result in 5 initial threads being started.

    my $server = shift;
    my $client = shift;
    my $iorbase = shift;
    my $deletelogs = shift;
    my $status = 0;

    print "\nRunning Test 1....\n";
    $test_num=1;

    my $lfname = "server_test" . $test_num . ".log";
    my $scname = "svc" . $test_num . ".conf";
    my $server_iorfile = $server->LocalFile ($iorbase);
    my $client_iorfile = $client->LocalFile ($iorbase);
    my $server_logfile = $server->LocalFile ($lfname);
    my $svc_conf = $server->LocalFile($scname);

    $server->DeleteFile($lfname);

    $SV = $server->CreateProcess ("server", " -ORBDebugLevel 5 -ORBLogFile $server_logfile -s 3 -p {-1,5,-1,0,60,0} -o $server_iorfile");
    $SC = $client->CreateProcess ("client", "-k file://$client_iorfile -s");

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

    $client_status = $SC->SpawnWaitKill ($client->ProcessStopWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
    }

     # Now find the spawned threads in the log file.

    $find_this="DTP_Task::svc() New thread created.";
    $found_cnt=0;

    my($found_cnt) = count_strings($server_logfile,$find_this);

    if ($found_cnt != 5) {
       print STDERR "ERROR: initial_pool_thread test failed w/$found_cnt instead of 5\n";
       $status = 1;
    }
    elsif ($deletelogs) {
        $server->DeleteFile($lfname);
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

sub test_2
{
    # Test 2:
    # This is a test for showing a process maintaining a min_pool_threads after
    # the creation of initial_pool_threads is higher. This test will also
    # exercise the thread_idle_time (timeout).
    # The test will start up a server with 10 init_pool_threads and 5 min_pool_threads.
    # After 10 seconds, 5 threads should expire leaving the minimum 5 threads.


    my $server = shift;
    my $client = shift;
    my $iorbase = shift;
    my $deletelogs = shift;
    my $status = 0;


    print "\nRunning Test 2....\n";
    $test_num=2;
    my $lfname = "server_test" . $test_num . ".log";
    my $scname = "svc" . $test_num . ".conf";
    my $server_iorfile = $server->LocalFile ($iorbase);
    my $client_iorfile = $client->LocalFile ($iorbase);
    my $server_logfile = $server->LocalFile ($lfname);
    my $svc_conf = $server->LocalFile($scname);

    $server->DeleteFile($lfname);

    $SV = $server->CreateProcess ("server", " -ORBDebugLevel 5 -ORBLogFile $server_logfile -s 3 -p {5,10,-1,0,10,0} -o $server_iorfile");
    $SC = $client->CreateProcess ("client", "-k file://$client_iorfile -s");

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

    # Sleep here for more than the timeout to let the threads die off and log.
    print STDERR "INFO: sleeping 15 seconds\n";
    sleep(15);

    $client_status = $SC->SpawnWaitKill ($client->ProcessStopWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
    }
     # Now find the spawned threads in the log file.

    $find_this="DTP_Task::svc() Existing thread expiring.";
    $found_cnt=0;

    my($found_cnt) = count_strings($server_logfile,$find_this);

    if ($found_cnt != 5) {
       print STDERR "ERROR: min_pool_thread test failed w/$found_cnt instead of 5\n";
       $status = 1;
    }
    elsif ($deletelogs) {
        $server->DeleteFile($lfname);
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

sub test_3
{
    # Test 3:
    # This is a test for showing a process maintaining a max_request_queue_depth after
    # more clients request service than the queue allows for.
    # The test will start up a server with a max_request_queue_depth of 10 and
    # will issue calls from 15 clients. There should be 5 CORBA exceptions found
    # in the log after the run.
    #
    my $server = shift;
    my $client = shift;
    my $iorbase = shift;
    my $deletelogs = shift;
    my $status = 0;

    print "\nRunning Test 3....\n";
    $test_num=3;
    $num_clients=15;
    my $lfname = "server_test" . $test_num . ".log";
    my $scname = "svc" . $test_num . ".conf";
    my $server_iorfile = $server->LocalFile ($iorbase);
    my $client_iorfile = $client->LocalFile ($iorbase);
    my $server_logfile = $server->LocalFile ($lfname);
    my $svc_conf = $server->LocalFile($scname);

    $server->DeleteFile($lfname);

    $SV = $server->CreateProcess ("server", " -ORBDebugLevel 5 -ORBLogFile $server_logfile -s 3 -p {-1,15,-1,0,10,10} -o $server_iorfile");

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

    for ($i = 0; $i < $num_clients; $i++) {
        $CLS[$i] = $client->CreateProcess ("client", "-c $i -k file://$client_iorfile -e 0");
        $CLS[$i]->Spawn ();
    }

    my $valid_num_exceptions=5;
    my $num_exceptions=0;

    for ($i = 0; $i < $num_clients; $i++) {

        $client_status = $CLS[$i]->WaitKill ($client->ProcessStopWaitInterval());

        if ($client_status != 0) {
            $num_exceptions++;
            print STDERR "STATUS: client $i returned $client_status\n";
        }
    }

    $SC = $client->CreateProcess ("client", "-k file://$client_iorfile -s");
    $client_status = $SC->SpawnWaitKill ($client->ProcessStopWaitInterval());
    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
    }

    if ($num_exceptions != $valid_num_exceptions)
    {
      print STDERR "ERROR: max_request_queue_depth test failed w/$num_exceptions instead of $valid_num_exceptions\n";
      $status = 1;
    }
    elsif ($deletelogs) {
        $server->DeleteFile($lfname);
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

sub test_4
{

    # Test 4:
    # This is a test for showing a process maintaining a max_pool_threads after
    # more clients request service than the queue allows for.
    # The test will start up a server with a max_request_queue_depth of 10 and
    # will issue calls from 10 clients. There should be 5 CORBA exceptions found
    # in the log after the run.
    #

    my $server = shift;
    my $client = shift;
    my $iorbase = shift;
    my $deletelogs = shift;
    my $status = 0;

    print "\nRunning Test 4....\n";
    $test_num=4;
    $num_clients=10;
    my $lfname = "server_test" . $test_num . ".log";
    my $scname = "svc" . $test_num . ".conf";
    my $server_iorfile = $server->LocalFile ($iorbase);
    my $client_iorfile = $client->LocalFile ($iorbase);
    my $server_logfile = $server->LocalFile ($lfname);
    my $svc_conf = $server->LocalFile($scname);

    $server->DeleteFile($lfname);

    $SV = $server->CreateProcess ("server", " -ORBDebugLevel 5 -ORBLogFile $server_logfile -s 5 -p {-1,1,5,0,60,10} -o $server_iorfile");
    $SC = $client->CreateProcess ("client", "-k file://$client_iorfile -s");

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

    for ($i = 0; $i < $num_clients; $i++) {
        $CLS[$i] = $client->CreateProcess ("client", "-c $i -k file://$client_iorfile");
        $CLS[$i]->Spawn ();
    }


    for ($i = 0; $i < $num_clients; $i++) {

        $client_status = $CLS[$i]->WaitKill ($client->ProcessStopWaitInterval());

        if ($client_status != 0) {
            print STDERR "ERROR: client $i returned $client_status\n";
        }
    }

    $client_status = $SC->SpawnWaitKill ($client->ProcessStopWaitInterval());

     # Now find the spawned threads in the log file.

    $find_this="Growing threadcount.";
    $found_cnt=0;
    $valid_cnt=4;

    my($found_cnt) = count_strings($server_logfile,$find_this);

    if ($found_cnt != $valid_cnt) {
       print STDERR "ERROR: max_pool_thread test failed w/$found_cnt instead of $valid_cnt\n";
       $status = 1;
    }
    elsif ($deletelogs) {
        $server->DeleteFile($lfname);
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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
my $deletelogs = 1;

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $status = 0;

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

  while ()
  {
      $status = test_1($server, $client, $iorbase, $deletelogs);
      last if ($status);
      $status = test_2($server, $client, $iorbase, $deletelogs);
      last if ($status);
      $status = test_3($server, $client, $iorbase, $deletelogs);
      last if ($status);
      $status = test_4($server, $client, $iorbase, $deletelogs);
      last;
  }

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
