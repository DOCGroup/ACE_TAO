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
use POSIX;

###############################################################
# Configuration parameters
###############################################################

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$sleeptime = 10;

# Variables for command-line arguments to client and server
# executables.
$consumer_iorfile = PerlACE::LocalFile ("consumer-ec.ior");
$supplier_iorfile = PerlACE::LocalFile ("supplier-ec.ior");
$consumer_conffile = PerlACE::LocalFile ("consumer-ec.conf");
$supplier_conffile = PerlACE::LocalFile ("supplier-ec.conf");


#################################################################
# Subs
#################################################################

sub run_test
{
    # Supplier EC.
    if (run_ec (0, $supplier_conffile, $supplier_iorfile) != 0) {
        kill_processes (); return -1;
    }
    
    # Consumer EC.
    if (run_ec (1, $consumer_conffile, $consumer_iorfile) != 0) {
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

sub restore_output
{
    # Restore output facilities.
    close (STDERR);
    close (STDOUT);
    open (STDOUT, ">&OLDOUT");
    open (STDERR, ">&OLDERR");
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

sub analyze_results
{
    if (! open (TEST_OUTPUT, "<$output_file"))
    {
        print STDERR "ERROR: Could not open $output_file\n";
        return -1;
    }

    my $status = 0;
    my $a_events = 0;
    my $b_events = 0;
    my $c_events = 0;
    while (<TEST_OUTPUT>)
    {
        if (m/Received event A/)
        {
            $a_events += 1;
        }
        elsif (m/Received event B/)
        {
            $b_events += 1;
        }
        elsif (m/Received event C/)
        {
            $c_events += 1;
        }
        elsif (m/Received 100 events/
               || m/SUCCESS/)
        {
            #skip this line - we do our own counting.
        }
        else
        {
            print STDERR $_;
            $status = -1;
        }
    }
    close (TEST_OUTPUT);
    
    my $all_received = 1;
    if ($a_events != 100
        || $b_events != 100
        || $c_events != 100)
    {
        $all_received = 0;
        print STDERR "ERROR: consumer received\n"
            ."\t$a_events A events, expected 100\n"
                ."\t$b_events B events, expected 100\n"
                    ."\t$c_events C events, expected 100\n";
    }
    
    if ($all_received && $status == 0) {
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

if (run_test () == -1) {
    $status = 1;
}

restore_output ();

if (analyze_results () == -1) {
    $status = 1;
}

exit $status;

