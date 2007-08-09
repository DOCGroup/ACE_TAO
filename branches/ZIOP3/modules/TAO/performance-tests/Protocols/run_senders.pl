eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

require Process;
use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

# Type of test to run
$test_type = "PACED";

# Estimated execution of each test
$execution_time = 100;

# Expected throughput of the network (in Mbps)
$expected_throughput = 100;

#
# Assigns default rates
#
$min_rate = 25; 
$max_rate = 100;
$rate_increment = 25;

@rates[0] = 5;

for ($rate = $min_rate, $i = 1; 
     $rate <= $max_rate; 
     $rate += $rate_increment, $i += 1)
{
   @rates[$i] = $rate;
}


#
# Assigns default message sizes
#
$min_message_size = 1 * 1000; 
$max_message_size = 64 * 1000;

@message_sizes[0] = 0;

for ($message_size = $min_message_size, $i = 1; 
     $message_size <= $max_message_size; 
     $message_size *= 2, $i += 1)
{
   @message_sizes[$i] = $message_size;
}


#
# Assigns default protocols
#
@protocols = 
    (
     "IIOP", 
     "DIOP",
     "SCIOP",
     );


#
# Parse the arguments
#
for ($i = 0; $i <= $#ARGV; $i++) {
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?") 
    {
        print STDERR "\nusage:  run_senders.pl\n";

        print STDERR "\t-h shows options menu\n";

        print STDERR "\t-message_sizes: defaults to (";
        for $message_size (@message_sizes)
        {
            print STDERR "$message_size, ";
        }
        print STDERR ")\n";

        print STDERR "\t-rates: defaults to (";
        for $rate (@rates)
        {
            print STDERR "$rate, ";
        }
        print STDERR ")\n";

        print STDERR "\t-protocols: defaults to (";
        for $protocol (@protocols)
        {
            print STDERR "$protocol, ";
        }
        print STDERR ")\n";

	print STDERR "\t-execution_time: defaults to $execution_time\n";

	print STDERR "\t-test_type: defaults to $test_type (valid options are PACED, THROUGHPUT, and LATENCY)\n";

        print STDERR "\n";

        $CL = new PerlACE::Process ("sender", "-h");
        $CL->Spawn ();
        $CL->WaitKill (5);
        
        $SV = new PerlACE::Process ("receiver", "-h");
        $SV->Spawn ();
        $SV->WaitKill (5);

        exit;
    }
    elsif ($ARGV[$i] eq "-message_sizes") 
    {
        @message_sizes = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-rates") 
    {
        @rates = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-protocols") 
    {
        @protocols = split (',', $ARGV[$i + 1]);
        $i++;
    }
    elsif ($ARGV[$i] eq "-execution_time") 
    {
	$execution_time = $ARGV[$i + 1];
        $i++;	
    }
    elsif ($ARGV[$i] eq "-test_type") 
    {
	$test_type = $ARGV[$i + 1];
        $i++;

	# Rate is irrelevant in THROUGHPUT and LATENCY tests.
	if ($test_type eq "THROUGHPUT" ||
	    $test_type eq "LATENCY")
	{
	    @rates = split (',', "1");
	}
    }
    else
    {
        $extra_args .= " " . $ARGV[$i];
    }
}

$i = 0;

#
# This loop form the arguments
#
for $protocol (@protocols)
{
    for $rate (@rates)
    {
	for $message_size (@message_sizes)
	{
	    # DIOP does not support messages beyond 8K
	    next if ($protocol eq "DIOP" && $message_size > (8 * 1000));

	    # Since DIOP is unreliable, it doesn't make sense to use it for THROUGHPUT and LATENCY tests.
	    next if ($protocol eq "DIOP" && ($test_type eq "THROUGHPUT" || $test_type eq "LATENCY"));
	    
	    # Assign iterations
	    if ($test_type eq "PACED")
	    {
		$iterations = $execution_time * $rate;
	    }
	    elsif ($test_type eq "THROUGHPUT")
	    {
		# Skip message size of zero since we will never to able get any throughput
		next if ($message_size == 0);
		$iterations = $expected_throughput * 1000 * 1000 * $execution_time / $message_size / 8;
	    }
	    elsif ($test_type eq "LATENCY")
	    {
		# Latency iteration are fixed.  Using
		# expected_throughput is really not a good measure.
		$iterations = 10000;
	    }

	    @configs[$i] = "-i $iterations -p $protocol -r $rate -s $message_size";
	    $i++;
	}
    }
}


if ($test_type eq "PACED")
{
    $test = 0;

    # Sender-side stats are not interesting when pacing
    $print_statistics = 0;
}
elsif ($test_type eq "THROUGHPUT")
{
    $test = 1;
    $print_statistics = 1;
}
elsif ($test_type eq "LATENCY")
{
    $test = 2;
    $print_statistics = 1;
}


for $config (@configs)
{
    $command = "./sender -a $test -t $print_statistics $config $extra_args";
    print  "$command \n";
    system ($command);
}

