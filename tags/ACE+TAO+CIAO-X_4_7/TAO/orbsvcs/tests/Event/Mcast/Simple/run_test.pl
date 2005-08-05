eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs Simple EC Mcast example.  It starts
# consumer, supplier and two (federated) Event Channel servers.
# See README for more details.

use lib '../../../../../../bin';
use PerlACE::Run_Test;

###############################################################
# Configuration parameters
###############################################################

# String indicating end of test output.
$test_terminator = "###########################################";

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 10;

# Variables for command-line arguments to client and server
# executables.
$consumer_iorfile = PerlACE::LocalFile ("consumer-ec.ior");
$supplier_iorfile = PerlACE::LocalFile ("supplier-ec.ior");

@consumer_conffile = (PerlACE::LocalFile ("consumer-ec.conf")
                      , PerlACE::LocalFile ("udp-consumer-ec.conf"));

@supplier_conffile = (PerlACE::LocalFile ("supplier-ec.conf")
                      , PerlACE::LocalFile ("udp-supplier-ec.conf"));

@test_comments =
    ("Test 1: Mcast Handler",
     "Test 2: UDP Handler");

#################################################################
# Subs
#################################################################

sub run_test
{
    my $index = shift;

    print STDERR " $test_comments[$index]\n\n";

    # Supplier EC.
    if (run_ec (0, $supplier_conffile[$index], $supplier_iorfile) != 0) {
        kill_processes (); return -1;
    }
    
    # Consumer EC.
    if (run_ec (1, $consumer_conffile[$index], $consumer_iorfile) != 0) {
        kill_processes (); return -1;
    }
    
    # Consumer.
    $ps[2] =
        new PerlACE::Process ("consumer",
                              "-ORBInitRef Event_Service=file://$consumer_iorfile");
    if ($ps[2]->Spawn () == -1) {
        kill_processes (); return -1;
    }
    
    # Give consumer a chance to connect to its EC before firing off the supplier.
    sleep ($sleeptime);
    
    # Supplier.
    $ps[3] =
        new PerlACE::Process ("supplier",
                          "-ORBInitRef Event_Service=file://$supplier_iorfile");
    if ($ps[3]->Spawn () == -1) {
        kill_processes (); return -1;
    }

    # Shutdown.
    return kill_processes ();
}

# Start Event Channel server.
sub run_ec
{
    my $ec_id = $_[0];
    my $conf_file = $_[1];
    my $ior_file = $_[2];

    unlink $ior_file;

    $ps[$ec_id] = new PerlACE::Process ("gateway-ec",
                                        "-ORBsvcconf $conf_file -i $ior_file");
    if ($ps[$ec_id]->Spawn () == -1) {
        return 1;
    }

    if (PerlACE::waitforfile_timed ($ior_file, $sleeptime) == -1)
    {
        print STDERR "ERROR: cannot find IOR file <$ior_file>\n";
        $ps[$ec_id]->Kill ();
        return 1;
    }

    return 0;
}

# Wait for and kill, if necessary, all started processes.
sub kill_processes
{
    my $result = 0;

    foreach $p (@ps)
    {
        if ($p->WaitKill (60) != 0) {
            $result = -1;
        }
    }

    if ($result == -1) {
        print STDERR "ERROR detected\n";
        return -1;
    }

    return 0;
}

sub redirect_output
{
    my $rundate = POSIX::strftime("%Y_%m_%d_%H_%M", localtime);
    $output_file = PerlACE::LocalFile ("run_test_$rundate");

    open (OLDOUT, ">&STDOUT");
    open (STDOUT, ">$output_file") or die "can't redirect stdout: $!";
    open (OLDERR, ">&STDERR");
    open (STDERR, ">&STDOUT") or die "can't redirect stderror: $!";
}

sub restore_output
{
    # Restore output facilities.
    close (STDERR);
    close (STDOUT);
    open (STDOUT, ">&OLDOUT");
    open (STDERR, ">&OLDERR");
}

sub analyze_results
{
    my $test_number = shift;
    print STDERR " $test_comments[$test_number]\n\n";

    if (! open (TEST_OUTPUT, "<$output_file"))
    {
        print STDERR "ERROR: Could not open $output_file\n";
        return -1;
    }

    my $output_start = $test_comments[$test_number];
    while (<TEST_OUTPUT>)
    {
        # Skip output not pertinent to this test.
        last if m/\Q$output_start\E/;
    }

    my $status = 0;
    my $events_received = 0;
    while (<TEST_OUTPUT>)
    {
        last if m/$test_terminator/;

        if (m/Received event/)
        {
            $events_received += 1;
        }
        elsif (m/consumer received 100 events/
               || m/^\s+$/)
        {
            #skip this line - we do our own counting.
        }
        else
        {
            $status = -1;
            print STDERR $_;
        }
    }
    close (TEST_OUTPUT);
    
    if ($events_received != 100)
    {
        print STDERR "ERROR: "
            ."consumer received $events_received events, expected 100\n";
    }
    
    if ($events_received == 100 && $status == 0) {
        print STDERR "SUCCESS\n";
        return 0;
    }
 
    print STDERR "ERROR - check $output_file for full output.\n";
    return -1;
}

##############################################################
# Run executables.
##############################################################

$status = 0;

redirect_output ();

for ($i = 0; $i < 2; $i++)
{
    if (run_test ($i) == -1) {
        $status = 1;
    }

    print STDERR "$test_terminator\n\n";
}

restore_output ();

for ($i = 0; $i < 2; $i++)
{
    if (analyze_results ($i) == -1) {
        $status = 1;
    }

    print STDERR "\n";
}

exit $status;


