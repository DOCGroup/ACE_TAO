eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use File::stat;

# amount of delay between running the servers

$sleeptime = 2;
$status = 0;

$nsior = PerlACE::LocalFile ("ns.ior");
$input = PerlACE::LocalFile ("test_input");

$debug = 0;

unlink $nsior, $test_input;

# generate test stream data
$input = PerlACE::generate_test_file("test_input", 102400);

for ($i = 0; $i <= $#ARGV; $i++)
{
    if ($ARGV[$i] eq "-h" || $ARGV[$i] eq "-?")
    {
        print STDERR "\nusage:  run_test\n";

        print STDERR "\t-h shows options menu\n";

        print STDERR "\t-p: Transport protocols defaults to (";
        for $protocol (@protocols)
        {
            print STDERR "$protocol, ";
        }
        print STDERR ")\n";

	print STDERR "\t-d: Debug Level defaults to 0";

        print STDERR "\n";

	exit;
    }
    elsif ($ARGV[$i] eq "-p")
    {
	@protocols = split (',', $ARGV[$i + 1]);
	$i++;
    }
    elsif ($ARGV[$i] eq "-d")
    {
	$debug = $ARGV[$i + 1];
	$i++;
    }
}

$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service", "-o $nsior");

print STDERR "Starting Naming Service\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsior, 100) == -1)
{
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill ();
    exit 1;
}


$output_file = "TCP_output";

for $protocol (@protocols)
{

    $output_file = $protocol."_output";

    if ($protocol eq "RTP_UDP")
    {
	$protocol = "RTP/UDP";
    }

    $SV = new PerlACE::Process ("server", "-ORBInitRef NameService=file://$nsior -ORBDebugLevel ".$debug." -f ".$output_file);
    $CL = new PerlACE::Process ("ftp", "-ORBInitRef NameService=file://$nsior -ORBDebugLevel ".$debug." -p ".$protocol." -f $input");

    print STDERR "Using ".$protocol."\n";
    print STDERR "Starting Receiver\n";

    $SV->Spawn ();

    sleep $sleeptime;

    print STDERR "Starting Sender\n";

    $sender = $CL->SpawnWaitKill (200);

    if ($sender != 0) {
	print STDERR "ERROR: sender returned $sender\n";
	$status = 1;
    }

    $receiver = $SV->TerminateWaitKill (200);

    if ($receiver != 0) {
	print STDERR "ERROR: receiver returned $receiver\n";
	$status = 1;
    }
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: Naming Service returned $nserver\n";
    $status = 1;
}

unlink $nsior;
unlink $output, $input;

exit $status;
