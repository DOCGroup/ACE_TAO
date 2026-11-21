eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

# This is a Perl script that runs Simple EC Mcast example.  It starts
# consumer, supplier and two (federated) Event Channel servers.
# See README for more details.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
$test->AddLibPath ("../Common");
###############################################################
# Configuration parameters
###############################################################

my $rundate = POSIX::strftime("%Y_%m_%d_%H_%M", localtime);
my @output_files = ();
for ($i = 0; $i < 2; $i++) {
    for ($j = 0; $j < 4; $j++) {
        $output_files[$i][$j] = $test->LocalFile ("run_test_".$i."_".$j."_".$rundate);
    }
}

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
my $sleeptime = 10;

# Variables for command-line arguments to client and server
# executables.
$consumer_iorfile = $test->LocalFile ("consumer-ec.ior");
$supplier_iorfile = $test->LocalFile ("supplier-ec.ior");

@consumer_conffile = ($test->LocalFile ("consumer-ec.conf"),
                      $test->LocalFile ("udp-consumer-ec.conf"));

@supplier_conffile = ($test->LocalFile ("supplier-ec.conf"),
                      $test->LocalFile ("udp-supplier-ec.conf"));

@test_comments = ("Test 1: Mcast Handler", "Test 2: UDP Handler");

#################################################################
# Subs
#################################################################

sub run_test {
    my $index = shift;

    print STDERR " $test_comments[$index]\n\n";

    # Supplier EC.
    if (run_ec ($index, 0, $supplier_conffile[$index], $supplier_iorfile) != 0) {
        kill_processes ();
        return -1;
    }

    # Consumer EC.
    if (run_ec ($index, 1, $consumer_conffile[$index], $consumer_iorfile) != 0) {
        kill_processes ();
        return -1;
    }

    # Consumer.
    $ps[2] = $test->CreateProcess ("consumer", "-ORBLogFile $output_files[$index][2] ".
                                               "-ORBInitRef Event_Service=file://$consumer_iorfile");
    if ($ps[2]->Spawn () == -1) {
        kill_processes ();
        return -1;
    }

    # Give consumer a chance to connect to its EC before firing off the supplier.
    sleep ($sleeptime);

    # Supplier.
    $ps[3] = $test->CreateProcess ("supplier", "-ORBLogFile $output_files[$index][3] ".
                                               "-ORBInitRef Event_Service=file://$supplier_iorfile");
    if ($ps[3]->Spawn () == -1) {
        kill_processes ();
        return -1;
    }

    # Shutdown.
    return kill_processes ();
}

# Start Event Channel server.
sub run_ec {
    my $test_index = $_[0];
    my $ec_id = $_[1];
    my $conf_file = $_[2];
    my $ior_file = $_[3];

    $test->DeleteFile ($ior_file);

    $ps[$ec_id] = $test->CreateProcess ("gateway-ec", "-ORBLogFile $output_files[$test_index][$ec_id] ".
                                                      "-ORBsvcconf $conf_file -i $ior_file");
    if ($ps[$ec_id]->Spawn () == -1) {
        return 1;
    }

    if ($test->WaitForFileTimed ($ior_file, $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find IOR file <$ior_file>\n";
        $ps[$ec_id]->Kill (); $ps[$ec_id]->TimedWait (1);
        return 1;
    }

    return 0;
}

# Wait for and kill, if necessary, all started processes.
sub kill_processes {
    my $result = 0;

    foreach $p (@ps) {
        if ($p->WaitKill ($test->ProcessStopWaitInterval() + 45) != 0) {
            $result = -1;
        }
    }

    if ($result == -1) {
        print STDERR "ERROR detected\n";
        return -1;
    }

    return 0;
}

sub analyze_results {
    my $test_number = $_[0];
    print STDERR " $test_comments[$test_number]\n\n";

    my $output_file = $output_files[$test_number][2];
    if (! open (TEST_OUTPUT, "<$output_file")) {
        print STDERR "ERROR: Could not open $output_file\n";
        return -1;
    }

    my $status = 0;
    my $events_received = 0;
    while (<TEST_OUTPUT>) {
        if (m/Received event/) {
            $events_received += 1;
        }
        elsif (m/consumer received 100 events/ || m/^\s+$/) {
            #skip this line - we do our own counting.
        }
        else {
            $status = -1;
            print STDERR $_;
        }
    }

    close (TEST_OUTPUT);

    if ($events_received != 100) {
        print STDERR "ERROR: consumer received $events_received events, expected 100\n";
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

for ($i = 0; $i < 2; $i++) {
    if (run_test ($i) == -1) {
        $status = 1;
    }

    print STDERR "###########################################\n\n";
}

for ($i = 0; $i < 2; $i++) {
    if (analyze_results ($i) == -1) {
        $status = 1;
    }
    print STDERR "\n";
}

exit $status;
