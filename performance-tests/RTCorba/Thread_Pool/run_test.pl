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
$debug_level = '0';
$iorbase = "server.ior";

$min_rate = 100;
$max_rate = 250;
$rate_increment = 25;

for ($rate = $min_rate, $i = 0;
     $rate <= $max_rate;
     $rate += $rate_increment, $i += 1) {
   $rates[$i] = $rate;
}

$min_work = 25;
$max_work = 85;
$work_increment = 5;

for ($work = $min_work, $i = 0;
     $work <= $max_work;
     $work += $work_increment, $i += 1) {
    $works[$i] = $work;
}

$min_thread = 0;
$max_thread = 10;
$thread_increment = 1;

for ($thread = $min_thread, $i = 0;
     $thread <= $max_thread;
     $thread += $thread_increment, $i += 1) {
    $threads[$i] = $thread;
}

$min_pool_priority = 1;
$max_pool_priority = 91;
$pool_priority_increment = 30;

for ($pool_priority = $min_pool_priority, $i = 0;
     $pool_priority <= $max_pool_priority;
     $pool_priority += $pool_priority_increment, $i += 1) {
    $pool_priorities[$i] = $pool_priority;
}

@workers = (1, 2, 3, 5, 10, 15, 20);

@test_types = (
    "rates",
    "work",
    "workers",
    "workers-2",
    "work-nolanes",
    "work-lanes-increase",
    "work-lanes-decrease",
    "thread-nolanes",
    "thread-lanes-increase",
    "thread-lanes-decrease",
    "thread-nolanes-with-slack",
    "thread-lanes-increase-with-slack",
    "thread-lanes-decrease-with-slack");

for $pool_priority (@pool_priorities) {
    $test_type = "work-pool-".$pool_priority;
    push @test_types, $test_type;
}

$work = 30;
$work_with_slack = 28;
$time_for_test = 10;
$max_throughput_timeout = 5;
$pool_threads = 3;
$native_priorities = 1;
$run_server = 1;

# Parse the arguments
for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") {
        print STDERR "\nusage:  run_test\n";
        print STDERR "\t-h shows options menu\n";

        print STDERR "\t-tests: defaults to (";
        for $test_type (@test_types) {
            print STDERR "$test_type, ";
        }
        print STDERR ")\n";

        print STDERR "\t-rates: defaults to (";
        for $rate (@rates) {
            print STDERR "$rate, ";
        }
        print STDERR ")\n";

        print STDERR "\t-works: defaults to (";
        for $work (@works) {
            print STDERR "$work, ";
        }
        print STDERR ")\n";

        print STDERR "\t-workers: defaults to (";
        for $worker (@workers) {
            print STDERR "$worker, ";
        }
        print STDERR ")\n";

        print STDERR "\t-threads: defaults to (";
        for $thread (@threads) {
            print STDERR "$thread, ";
        }
        print STDERR ")\n";

        print STDERR "\t-pool-priorities: defaults to (";
        for $pool_priority (@pool_priorities) {
            print STDERR "$pool_priority, ";
        }
        print STDERR ")\n";

        print STDERR "\t-native-priorities for work pool tests: ".
            "defaults to $native_priorities\n";

        print STDERR "\t-run-server: defaults to $run_server\n";

        print STDERR "\n";

        exit;
    }
    elsif ($ARGV[$i] eq "-tests") {
        $i++;
        @test_types = split (',', $ARGV[$i]);
    }
    elsif ($ARGV[$i] eq "-rates") {
        $i++;
        @rates = split (',', $ARGV[$i]);
    }
    elsif ($ARGV[$i] eq "-works") {
        $i++;
        @works = split (',', $ARGV[$i]);
    }
    elsif ($ARGV[$i] eq "-workers") {
        $i++;
        @workers = split (',', $ARGV[$i]);
    }
    elsif ($ARGV[$i] eq "-threads") {
        $i++;
        @threads = split (',', $ARGV[$i]);
    }
    elsif ($ARGV[$i] eq "-pool-priorities") {
        $i++;
        @pool_priorities = split (',', $ARGV[$i]);
    }
    elsif ($ARGV[$i] eq "-native-priorities") {
        $i++;
        $native_priorities = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-run-server") {
        $i++;
        $run_server = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-o") {
        $i++;
        $iorbase = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-w") {
        $i++;
        $work = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-t") {
        $i++;
        $time_for_test = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-z") {
        $i++;
        $max_throughput_timeout = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-s") {
        $i++;
        $pool_threads = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-debug") {
        $i++;
        $debug_level = '10';
    }
    else {
        $extra_args .= " " . $ARGV[$i];
    }
}

$fixed_client_args = "-w $work -t $time_for_test -z $max_throughput_timeout";

@configurations =
    ({
        description => "work",
        server => "-n 1",
     },{
         description => "rates",
         server => "-n 1",
     },{
         description => "workers",
         server => "-n 1",
     },{
         description => "workers-2",
         server => "-n 1 -s 2",
     },{
         description => "work-nolanes",
         server => "-s $pool_threads -l one-zero-lane -b one-full-band",
     },{
         description => "work-lanes-increase",
         server => "-n 3",
     },{
         description => "work-lanes-decrease",
         server => "-n 3",
     },{
         description => "thread-nolanes",
         server => "-s $pool_threads -l one-zero-lane -b one-full-band",
     },{
         description => "thread-lanes-increase",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort",
     },{
         description => "thread-lanes-decrease",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort",
     },{
         description => "thread-nolanes-with-slack",
         server => "-s $pool_threads -l one-zero-lane -b one-full-band",
     },{
         description => "thread-lanes-increase-with-slack",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort",
     },{
         description => "thread-lanes-decrease-with-slack",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort",
     });

for $test (@configurations) {
    #
    # setup work test
    #
    if ($test->{description} eq "work") {
        $i = 0;
        for $work (@works) {
            $test->{clients}[$i] = "-w $work -c 1 -r empty-file ".
                "-t $time_for_test -z $max_throughput_timeout";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup rates test
    #
    elsif ($test->{description} eq "rates") {
        $i = 0;
        for $rate (@rates) {
            $test->{clients}[$i] = "-r $rate $fixed_client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup workers test
    #
    elsif ($test->{description} eq "workers" or
           $test->{description} eq "workers-2") {
        $i = 0;
        for $worker (@workers) {
            $test->{clients}[$i] = "-c $worker -r empty-file $fixed_client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup work-nolanes test
    #
    elsif ($test->{description} eq "work-nolanes") {
        $i = 0;
        for $work (@works) {
            $test->{clients}[$i] = "-w $work -r increasing-rates -u 1000 ".
                "-t $time_for_test -z $max_throughput_timeout";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup work-lanes-increase test
    #
    elsif ($test->{description} eq "work-lanes-increase") {
        $i = 0;
        for $work (@works) {
            $test->{clients}[$i] = "-w $work -r increasing-rates ".
                "-t $time_for_test -z $max_throughput_timeout";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup work-lanes-decrease test
    #
    elsif ($test->{description} eq "work-lanes-decrease") {
        $i = 0;
        for $work (@works) {
            $test->{clients}[$i] = "-w $work -r decreasing-rates ".
                "-t $time_for_test -z $max_throughput_timeout";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup thread-nolanes test
    #
    elsif ($test->{description} eq "thread-nolanes" or
           $test->{description} eq "thread-nolanes-with-slack") {
        $client_args = "-t $time_for_test -z $max_throughput_timeout";

        if ($test->{description} eq "thread-nolanes") {
            $client_args .= "-w $work";
        }
        else {
            $client_args .= "-w $work_with_slack";
        }

        $i = 0;
        for $thread (@threads) {
            $test->{clients}[$i] = "-c $thread -r increasing-rates ".
                "-u 1000 $client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup thread-lanes-increase test
    #
    elsif ($test->{description} eq "thread-lanes-increase" or
           $test->{description} eq "thread-lanes-increase-with-slack") {
        $client_args = "-t $time_for_test -z $max_throughput_timeout";

        if ($test->{description} eq "thread-lanes-increase") {
            $client_args .= "-w $work";
        }
        else {
            $client_args .= "-w $work_with_slack";
        }

        $i = 0;
        for $thread (@threads) {
            $test->{clients}[$i] = "-c $thread -r increasing-rates $client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup thread-lanes-decrease test
    #
    elsif ($test->{description} eq "thread-lanes-decrease" or
           $test->{description} eq "thread-lanes-decrease-with-slack") {
        $client_args = "-t $time_for_test -z $max_throughput_timeout";

        if ($test->{description} eq "thread-lanes-decrease") {
            $client_args .= "-w $work";
        }
        else {
            $client_args .= "-w $work_with_slack";
        }

        $i = 0;
        for $thread (@threads) {
            $test->{clients}[$i] = "-c $thread -r decreasing-rates $client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }
}

for $pool_priority (@pool_priorities) {
    $new_configuration = {};
    if ($native_priorities) {
        $confbase = "native-svc$PerlACE::svcconf_ext";
        $server_cnffile = $server->LocalFile ($confbase);
        $client_cnffile = $client->LocalFile ($confbase);
        $sv_pool_args =
            "-ORBsvcconf $server_cnffile -p invocation-priorities-native";
        $cl_pool_args =
            "-ORBsvcconf $client_cnffile -p invocation-priorities-native";
    }
    $test_type = "work-pool-" . $pool_priority;
    $new_configuration->{description} = $test_type;
    $new_configuration->{server} =
        "-f $pool_priority -s $pool_threads $sv_pool_args";

    $i = 0;
    for $work (@works) {
        $new_configuration->{clients}[$i] = "-w $work -r increasing-rates ".
            "-t $time_for_test -z $max_throughput_timeout $cl_pool_args";
        $i++;
    }

    $new_configuration->{clients}[$i - 1] .= " -x 1";

    push @configurations, $new_configuration;
}

my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$SV = $server->CreateProcess ("server");
$CL = $client->CreateProcess ("client");

$server_commons = "-ORBdebuglevel $debug_level -o $server_iorfile";
$client_commons = "-k file://$client_iorfile";

print STDOUT "\n______________________________________________________\n";

for $test (@configurations) {
    $run_configuration = 1;
    for $test_type (@test_types) {
        if ($test_type eq $test->{description}) {
            $run_configuration = 1;
        }
    }

    if ($run_configuration == 0) {
        next;
    }

    print STDOUT "\nRunning $test->{description}\n";
    print STDOUT "______________________________________________________\n";

    if ($run_server == 1) {
        $SV->Arguments ("$test->{server} $server_commons $extra_args");

        print STDOUT "\n______________________________________________________";
        print STDOUT "\n\nserver " . $SV->Arguments() . "\n";
        print STDOUT "______________________________________________________\n";
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
    }

    my $clients = $test->{clients};
    for $args (@$clients) {
        $CL->Arguments ("$args $client_commons $extra_args");
        print "Client " . $CL->Arguments () . "\n";

        print STDOUT "\n______________________________________________________";
        print STDOUT "\n\nclient " . $CL->Arguments() . "\n";
        print STDOUT "______________________________________________________\n";

        $client_status =
            $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 285);

        if ($client_status != 0) {
            print STDERR "ERROR: client returned $client_status\n";
            $SV->Kill (); $SV->TimedWait (1);
            exit 1;
        }
    }

    if ($run_server == 1) {
        $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

        if ($server_status != 0) {
            print STDERR "ERROR: server returned $server_status\n";
            exit 1;
        }

        $server->DeleteFile($iorbase);
        $client->DeleteFile($iorbase);
    }
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
