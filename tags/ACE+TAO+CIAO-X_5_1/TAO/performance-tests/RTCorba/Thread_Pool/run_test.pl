eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

require Process;
use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$experiment_timeout = 300;
$iorfile_timeout = 10;

$min_rate = 100; 
$max_rate = 250;
$rate_increment = 25;

for ($rate = $min_rate, $i = 0; 
     $rate <= $max_rate; 
     $rate += $rate_increment, $i += 1)
{
   @rates[$i] = $rate;
}

$min_work = 25; 
$max_work = 85;
$work_increment = 5;

for ($work = $min_work, $i = 0; 
     $work <= $max_work; 
     $work += $work_increment, $i += 1)
{
   @works[$i] = $work;
}

$min_thread = 0; 
$max_thread = 10;
$thread_increment = 1;

for ($thread = $min_thread, $i = 0; 
     $thread <= $max_thread; 
     $thread += $thread_increment, $i += 1)
{
   @threads[$i] = $thread;
}

$min_pool_priority = 1; 
$max_pool_priority = 91;
$pool_priority_increment = 30;

for ($pool_priority = $min_pool_priority, $i = 0; 
     $pool_priority <= $max_pool_priority; 
     $pool_priority += $pool_priority_increment, $i += 1)
{
   @pool_priorities[$i] = $pool_priority;
}

@workers = (1, 2, 3, 5, 10, 15, 20);

$results_directory = "results";

@test_types = 
    (
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
     "thread-lanes-decrease-with-slack",
     );

for $pool_priority (@pool_priorities)
{
    $test_type = "work-pool-".$pool_priority;
    push @test_types, $test_type;
}

$iorfile = "ior";
$work = 30;
$work_with_slack = 28;
$time_for_test = 10;
$max_throughput_timeout = 5;
$pool_threads = 3;
$native_priorities = 1;
$run_server = 1;

# Parse the arguments
for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") 
    {
        print STDERR "\nusage:  run_test\n";

        print STDERR "\t-h shows options menu\n";

        print STDERR "\t-tests: defaults to (";
        for $test_type (@test_types)
        {
            print STDERR "$test_type, ";
        }
        print STDERR ")\n";

        print STDERR "\t-rates: defaults to (";
        for $rate (@rates)
        {
            print STDERR "$rate, ";
        }
        print STDERR ")\n";

        print STDERR "\t-works: defaults to (";
        for $work (@works)
        {
            print STDERR "$work, ";
        }
        print STDERR ")\n";

        print STDERR "\t-workers: defaults to (";
        for $worker (@workers)
        {
            print STDERR "$worker, ";
        }
        print STDERR ")\n";

        print STDERR "\t-threads: defaults to (";
        for $thread (@threads)
        {
            print STDERR "$thread, ";
        }
        print STDERR ")\n";

        print STDERR "\t-pool-priorities: defaults to (";
        for $pool_priority (@pool_priorities)
        {
            print STDERR "$pool_priority, ";
        }
        print STDERR ")\n";

        print STDERR "\t-native-priorities for work pool tests: defaults to $native_priorities\n";

        print STDERR "\t-run-server: defaults to $run_server\n";

        print STDERR "\n";

        $CL = new PerlACE::Process ("client", "-h");
        $CL->Spawn ();
        $CL->WaitKill (5);
        
        $SV = new PerlACE::Process ("server", "-h");
        $SV->Spawn ();
        $SV->WaitKill (5);

        exit;
    }
    elsif ($ARGV[$i] eq "-tests") 
    {
        @test_types = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-rates") 
    {
        @rates = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-works") 
    {
        @works = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-workers") 
    {
        @workers = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-threads") 
    {
        @threads = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-pool-priorities") 
    {
        @pool_priorities = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-native-priorities") 
    {
        $i++;
        $native_priorities = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-run-server") 
    {
        $i++;
        $run_server = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-o") 
    {
        $extra_args .= " " . $ARGV[$i];
        $i++;
        $iorfile = $ARGV[$i];
        $extra_args .= " " . $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-w") 
    {
        $i++;
        $work = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-t") 
    {
        $i++;
        $time_for_test = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-z") 
    {
        $i++;
        $max_throughput_timeout = $ARGV[$i];
    }
    elsif ($ARGV[$i] eq "-s") 
    {
        $i++;
        $pool_threads = $ARGV[$i];
    }
    else
    {
        $extra_args .= " " . $ARGV[$i];
    }
}

$fixed_client_args = "-w $work -t $time_for_test -z $max_throughput_timeout";

@configurations = 
    (
     {
         description => "work",
         server => "-n 1", 
     },
     {
         description => "rates",
         server => "-n 1", 
     },
     {
         description => "workers",
         server => "-n 1", 
     },
     {
         description => "workers-2",
         server => "-n 1 -s 2", 
     },
     {
         description => "work-nolanes",
         server => "-s $pool_threads -l one-zero-lane -b one-full-band", 
     },
     {
         description => "work-lanes-increase",
         server => "-n 3", 
     },
     {
         description => "work-lanes-decrease",
         server => "-n 3", 
     },
     {
         description => "thread-nolanes",
         server => "-s $pool_threads -l one-zero-lane -b one-full-band", 
     },
     {
         description => "thread-lanes-increase",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort", 
     },
     {
         description => "thread-lanes-decrease",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort", 
     },
     {
         description => "thread-nolanes-with-slack",
         server => "-s $pool_threads -l one-zero-lane -b one-full-band", 
     },
     {
         description => "thread-lanes-increase-with-slack",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort", 
     },
     {
         description => "thread-lanes-decrease-with-slack",
         server => "-l three-lanes-with-best-effort -b three-bands-with-best-effort", 
     },
     );

for $test (@configurations)
{
    #
    # setup work test
    #
    if ($test->{description} eq "work")
    {
	$i = 0;
	for $work (@works)
        {
            $test->{clients}[$i] = "-w $work -c 1 -r empty-file -t $time_for_test -z $max_throughput_timeout";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup rates test
    #
    elsif ($test->{description} eq "rates")
    {
        $i = 0;
        for $rate (@rates)
        {
            $test->{clients}[$i] = "-r $rate $fixed_client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup workers test
    #
    elsif ($test->{description} eq "workers" or
           $test->{description} eq "workers-2")
    {
        $i = 0;
        for $worker (@workers)
        {
            $test->{clients}[$i] = "-c $worker -r empty-file $fixed_client_args";
            $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup work-nolanes test
    #
    elsif ($test->{description} eq "work-nolanes")
    {
	$i = 0;
	for $work (@works)
        {
            $test->{clients}[$i] = "-w $work -r increasing-rates -u 1000 -t $time_for_test -z $max_throughput_timeout";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup work-lanes-increase test
    #
    elsif ($test->{description} eq "work-lanes-increase")
    {
	$i = 0;
	for $work (@works)
        {
            $test->{clients}[$i] = "-w $work -r increasing-rates -t $time_for_test -z $max_throughput_timeout";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup work-lanes-decrease test
    #
    elsif ($test->{description} eq "work-lanes-decrease")
    {
	$i = 0;
	for $work (@works)
        {
            $test->{clients}[$i] = "-w $work -r decreasing-rates -t $time_for_test -z $max_throughput_timeout";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup thread-nolanes test
    #
    elsif ($test->{description} eq "thread-nolanes" or
           $test->{description} eq "thread-nolanes-with-slack")
    {
        $client_args = "-t $time_for_test -z $max_throughput_timeout";

        if ($test->{description} eq "thread-nolanes")
        {
            $client_args .= "-w $work";
        }
        else
        {
            $client_args .= "-w $work_with_slack";
        }            
        
	$i = 0;
	for $thread (@threads)
        {
            $test->{clients}[$i] = "-c $thread -r increasing-rates -u 1000 $client_args";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup thread-lanes-increase test
    #
    elsif ($test->{description} eq "thread-lanes-increase" or
           $test->{description} eq "thread-lanes-increase-with-slack")
    {
        $client_args = "-t $time_for_test -z $max_throughput_timeout";

        if ($test->{description} eq "thread-lanes-increase")
        {
            $client_args .= "-w $work";
        }
        else
        {
            $client_args .= "-w $work_with_slack";
        }            
        
	$i = 0;
	for $thread (@threads)
        {
            $test->{clients}[$i] = "-c $thread -r increasing-rates $client_args";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }

    #
    # setup thread-lanes-decrease test
    #
    elsif ($test->{description} eq "thread-lanes-decrease" or
           $test->{description} eq "thread-lanes-decrease-with-slack")
    {
        $client_args = "-t $time_for_test -z $max_throughput_timeout";

        if ($test->{description} eq "thread-lanes-decrease")
        {
            $client_args .= "-w $work";
        }
        else
        {
            $client_args .= "-w $work_with_slack";
        }            
        
	$i = 0;
	for $thread (@threads)
        {
            $test->{clients}[$i] = "-c $thread -r decreasing-rates $client_args";
	    $i++;
        }

        $test->{clients}[$i - 1] .= " -x 1";
    }
}

for $pool_priority (@pool_priorities)
{
    $new_configuration = {};
    if ($native_priorities)
    {
        $pool_args = "-ORBsvcconf native-svc$PerlACE::svcconf_ext -p invocation-priorities-native";
    }
    $test_type = "work-pool-".$pool_priority;
    $new_configuration->{description} = $test_type;
    $new_configuration->{server} = "-f $pool_priority -s $pool_threads $pool_args";

    $i = 0;
    for $work (@works)
    {
	$new_configuration->{clients}[$i] = "-w $work -r increasing-rates -t $time_for_test -z $max_throughput_timeout $pool_args";
	$i++;
    }

    $new_configuration->{clients}[$i - 1] .= " -x 1";
    
    push @configurations, $new_configuration;
}

sub run_client
{
    print STDOUT "\n______________________________________________________\n";

    print STDOUT "\nclient @_\n";

    print STDOUT "______________________________________________________\n";

    $CL = new PerlACE::Process ("client", @_);

    $CL->Spawn ();
    
    $client = $CL->WaitKill ($experiment_timeout);

    if ($client != 0) 
    {
        print STDERR "ERROR: client returned $client\n";
        $status = 1;
        zap_server (1);
    }
}

sub run_server 
{
    print STDOUT "\n______________________________________________________\n";

    print STDOUT "\nserver @_\n";

    print STDOUT "______________________________________________________\n";

    $SV = new PerlACE::Process ("server", @_);

    $SV->Spawn ();

    if (PerlACE::waitforfile_timed ($iorfile, $iorfile_timeout) == -1)
    {
        print STDERR "ERROR: cannot find ior file: $iorfile\n";
        $status = 1;
        zap_server (1);
    }
}

sub zap_server
{
    $server = $SV->WaitKill (5);
    
    if ($server != 0) 
    {
        print STDERR "ERROR: server returned $server\n";
        $status = 1;
    }

    unlink $iorfile;
    
    if ($_[0] || $status)
    {
        exit $status;
    }
}

unlink $iorfile;

mkdir $results_directory, 0777;

print STDOUT "\n______________________________________________________\n";

for $test (@configurations)
{
    $run_configuration = 0;
    for $test_type (@test_types)
    {
        if ($test_type eq $test->{description})
        {
            $run_configuration = 1;
        }
    }
    
    if ($run_configuration == 0)
    {
        next;
    }

    $output_file = $results_directory . $DIR_SEPARATOR . $test->{description};

    print STDOUT "\nResults of $test->{description} test being redirected to $output_file\n";

    print STDOUT "______________________________________________________\n";

    open (OLDOUT, ">&STDOUT");
    open (STDOUT, ">$output_file") 
        or die "can't redirect stdout: $!";
    open (OLDERR, ">&STDERR");
    open (STDERR, ">&STDOUT") 
        or die "can't redirect stderror: $!";

    if ($run_server == 1)
    {
        run_server ($test->{server} . $extra_args);
    }

    my $clients = $test->{clients};
    for $args (@$clients)
    {
        run_client ($args . $extra_args);
    }

    if ($run_server == 1)
    {
        zap_server (0);
    }

    close (STDERR);
    close (STDOUT);
    open (STDOUT, ">&OLDOUT");
    open (STDERR, ">&OLDERR");
}

exit $status;
