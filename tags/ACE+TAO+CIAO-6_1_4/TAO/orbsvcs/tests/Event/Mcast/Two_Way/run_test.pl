eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs Simple EC Mcast example.  It starts
# consumer, supplier and two (federated) Event Channel servers.
# See README for more details.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use POSIX;

$status = 0;

# Configuration parameters

# Number of Event Channel/Application pairs that will be started up.
my $apps_number = 3;

$test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$test->AddLibPath ("../Common");
# Variables for command-line arguments to client and server executables.
my $ec_ior  = "ec.ior";
my $gw_conf = "gateway.conf";

my $rundate = POSIX::strftime("%Y_%m_%d_%H_%M", localtime);
my @test_results = ();
for ($i = 0; $i < 2*$apps_number; $i++) {
    $test_results[$i] = $test->LocalFile ("run_test_".$i.$rundate);
}

my $test_gw_conf = $test->LocalFile ($gw_conf);
my $test_ec_ior = $test->LocalFile ($ec_ior);
$test->DeleteFile ($ec_ior);

my @ps = ();

sub run_test {
    for ($i = 0; $i < $apps_number; $i++) {
        $test->DeleteFile ($ec_ior.$i);
    }

    # Start federated Event Channels.
    for ($i = 0; $i < $apps_number; $i++) {
        if (run_ec ($i) != 0) {
            kill_processes ();
            return -1;
        }
    }

    # Start EC clients.
    for ($i = 0; $i < $apps_number; $i++) {
        my $ps_index = $apps_number + $i;
        $ps[$ps_index] = $test->CreateProcess ("application", "-ORBLogFile $test_results[$ps_index] -d ".
                                               "-ORBInitRef Event_Service=file://$test_ec_ior.$ps_index");
        if ($ps[$ps_index]->Spawn () == -1) {
            print STDERR "ERROR detected clients Spawn\n";
            kill_processes ();
            return -1;
        }
    }

    # Shutdown.
    $status = wait_kill_processes ();

    for ($i = 0; $i < $apps_number; $i++) {
        $test->DeleteFile ($ec_ior.$i);
    }

    return $status;
}

# Start Event Channel server.
sub run_ec {
    my $id = $_[0];
    my $ior_file = $test_ec_ior.$id;
    my $ior = $ec_ior.$id;

    $ps[$id] = $test->CreateProcess ("gateway-ec", "-ORBLogFile $test_results[$id] ".
                                                   "-ORBsvcconf $test_gw_conf -i $ior_file");
    if ($ps[$id]->Spawn () == -1) {
        print STDERR "ERROR detected channels Spawn\n";
        return 1;
    }

    if ($test->WaitForFileTimed ($ior, $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find IOR file <$ior_file>\n";
        $ps[$id]->Kill (); $ps[$id]->TimedWait (1);
        return 1;
    }

    return 0;
}

# Wait for and kill, if necessary, all started processes.
sub wait_kill_processes
{
    my $result = 0;
    foreach $p (@ps) {
        if ($p->WaitKill ($test->ProcessStopWaitInterval() + 45) != 0) {
            print STDERR "ERROR detected\n";
            $result = 1;
        }
    }

    return 0;
}

sub kill_processes
{
    foreach $p (@ps) {
        $p->Kill (); $p->TimedWait (1);
    }

    return 0;
}

sub analyze_results {
    my $filename = @$_[0];
    if (! open (TEST_OUTPUT, "<$filename")) {
        print STDERR "ERROR: Could not open $filename\n";
        return -1;
    }

    my $status = 0;
    while (<TEST_OUTPUT>) {
        if (m/total of/) {
            chomp $_;
            my @words = split (/ /, $_);
            my $pid = $words[3];
            my $h = $words[7];
            $heartbeats{$pid} += $h;
        }
        elsif (m/Received following heartbeats/) {
            #skip this line - we do our own counting.
        }
        else {
            $status = -1;
            print STDERR $_;
        }
    }
    close (TEST_OUTPUT);

    foreach $pid (keys %heartbeats) {
        my $ideal_heartbeats = $apps_number * 50;
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
            "Received $received_heartbeats heartbeats from pid $pid";
        print STDERR "$range_note\n";
    }

    if ($status == 0) {
        print STDERR "SUCCESS\n";
        return 0;
    }

    print STDERR "ERROR - check $filename for full output.\n";
    return -1;
}

##############################################################
# Run executables.
##############################################################

if (run_test () == -1) {
    $status = 1;
}

for ($i = 0; $i < 2*$apps_number; $i++) {
    if (analyze_results ($test_results[$i]) == -1) {
        $status = 1;
    }
}

exit $status;

