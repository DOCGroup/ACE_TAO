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
# Number of Event Channel/Application pairs that will be started up.
$number_of_applications = 3;

# Variables for command-line arguments to client and server
# executables.
$iorfile_base = "ec.ior";
$conffile = PerlACE::LocalFile ("gateway.conf");

#################################################################
# Subs
#################################################################

sub run_test
{
    # Start federated Event Channels.
    for ($i = 0; $i < $number_of_applications; $i++)
    {
        if (run_ec ($i, $conffile, $iorfile_base.$i) != 0) {
            kill_processes (); return -1;
        }
    }

    # Start EC clients.
    for ($i = 0; $i < $number_of_applications; $i++)
    {
        my $ps_index = $number_of_applications + $i;
        my $args = "-d -ORBInitRef Event_Service=file://$iorfile_base$i";
        $ps[$ps_index] =
            new PerlACE::Process ("application", $args);
        if ($ps[$ps_index]->Spawn () == -1) {
            kill_processes (); return -1;
        }
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
    while (<TEST_OUTPUT>)
    {
        if (m/total of/)
        {
            chomp $_;
            my @words = split (/ /, $_);
            my $pid = $words[3];
            my $h = $words[7];
            $heartbeats{$pid} += $h;
        }
        elsif (m/Received following heartbeats/)
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
    
    foreach $pid (keys %heartbeats)
    {
        my $ideal_heartbeats = $number_of_applications * 50;
        my $received_heartbeats = $heartbeats{$pid};
        my $in_range = (($ideal_heartbeats - $received_heartbeats)
                        <= $number_of_applications);
        my $range_note = " (";
        if (! $in_range) {
            $range_note .= "NOT ";
            $status = -1;
        }
        $range_note.=
            "within range of $number_of_applications from $ideal_heartbeats)";
        print STDERR
            "Received $received_heartbeats "
                ."heartbeats from pid $pid";
        print STDERR "$range_note\n";
    }

    if ($status == 0) {
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

