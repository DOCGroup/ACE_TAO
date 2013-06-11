eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use POSIX;

# This is a Perl script that runs Simple EC Mcast example.  It starts
# consumer, supplier and two (federated) Event Channel servers.
# See README for more details.


###############################################################
# Configuration parameters
###############################################################

# Amount of delay (in seconds) between starting a server and a client
# to allow proper server initialization.
$status = 0;

$sleeptime = 10;

my $test = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$test->AddLibPath ("../Common");

# Variables for command-line arguments to client and server
# executables.
my $supplier_ior = "supplier-ec.ior";
my $consumer_ior = "consumer-ec.ior";
my $supplier_conf = "supplier-ec.conf";
my $consumer_conf = "consumer-ec.conf";

my $test_supplier_ior = $test->LocalFile ($supplier_ior);
my $test_consumer_ior = $test->LocalFile ($consumer_ior);
$test->DeleteFile ($supplier_ior);
$test->DeleteFile ($consumer_ior);

my $test_supplier_conf = $test->LocalFile ($supplier_conf);
my $test_consumer_conf = $test->LocalFile ($consumer_conf);

my @outputs = ();
for ($i = 0; $i < 4; $i++) {
    $outputs[$i] = "run_test_".$i.".log";
    $output_files[$i] = $test->LocalFile ($outputs[$i]);
    $test->DeleteFile ($outputs[$i]);
}

#Processes creation
$EC_SP = $test->CreateProcess ("gateway-ec", "-ORBsvcconf $test_supplier_conf ".
                                             "-i $test_supplier_ior -ORBLogFile $output_files[0]");
$EC_CM = $test->CreateProcess ("gateway-ec", "-ORBsvcconf $test_consumer_conf ".
                                             "-i $test_consumer_ior -ORBLogFile $output_files[1]");
$CM = $test->CreateProcess ("consumer", "-ORBInitRef Event_Service=file://$test_consumer_ior ".
                                        "-ORBLogFile $output_files[2]");
$SP = $test->CreateProcess ("supplier", "-ORBInitRef Event_Service=file://$test_supplier_ior ".
                                        "-ORBLogFile $output_files[3]");

#################################################################
# Subs
#################################################################

sub run_test
{
    # Start Event Channel server.
    # Supplier EC.
    my $process_status = $EC_SP->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: ec_supplier returned $process_status\n";
        return -1;
    }

    if ($test->WaitForFileTimed ($supplier_ior,
                                 $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$test_supplier_ior>\n";
        $EC_SP->Kill (); $EC_SP->TimedWait (1);
        return -1;
    }

    # Consumer EC.
    $process_status = $EC_CM->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: ec_consumer returned $process_status\n";
        return -1;
    }

    if ($test->WaitForFileTimed ($consumer_ior,
                                 $test->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$test_consumer_ior>\n";
        $EC_SP->Kill (); $EC_SP->TimedWait (1);
        $EC_CM->Kill (); $EC_CM->TimedWait (1);
        return -1;
    }

    # Consumer.
    $process_status = $CM->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

    if ($process_status != 0) {
        print STDERR "ERROR: consumer returned $process_status\n";
        $status = 1;
        kill_processes ();
        return -1;
    }

    # Give consumer a chance to connect to its EC before firing off the supplier.
    sleep ($sleeptime);

    # Supplier.
    $process_status = $SP->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

    if ($process_status != 0) {
        print STDERR "ERROR: supplier returned $process_status\n";
        $status = 1;
        kill_processes ();
        return $status;
    }

    # Shutdown.
    return kill_processes ();
}

# Wait for and kill, if necessary, all started processes.
sub kill_processes {
    my $result = 0;

    my $process_status = $EC_SP->WaitKill ($test->ProcessStopWaitInterval() + 45);

    if ($process_status != 0) {
        print STDERR "ERROR: ec_supplier returned $process_status\n";
        $result = 1;
    }

    $process_status = $EC_CM->WaitKill ($test->ProcessStopWaitInterval() + 45);

    if ($process_status != 0) {
        print STDERR "ERROR: ec_consumer returned $process_status\n";
        $result = 1;
    }

    $process_status = $SP->WaitKill ($test->ProcessStopWaitInterval() + 45);

    if ($process_status != 0) {
        print STDERR "ERROR: supplier returned $process_status\n";
        $result = 1;
    }

    $process_status = $CM->WaitKill ($test->ProcessStopWaitInterval() + 45);

    if ($process_status != 0) {
        print STDERR "ERROR: consumer returned $process_status\n";
        $result = 1;
    }

    if ($result != 0) {
        print STDERR "ERROR detected\n";
        return -1;
    }

    return 0;
}

sub analyze_results {
    my $status = 0;
    my $a_events = 0;
    my $b_events = 0;
    my $c_events = 0;

    foreach $output_file (@output_files) {
        if (! open (TEST_OUTPUT, "<$output_file")) {
            print STDERR "ERROR: Could not open $output_file\n";
            return -1;
        }

        while (<TEST_OUTPUT>) {
            if (m/Received event A/) {
                $a_events += 1;
            }
            elsif (m/Received event B/) {
                $b_events += 1;
            }
            elsif (m/Received event C/) {
                $c_events += 1;
            }
            elsif (m/Received 100 events/ || m/SUCCESS/) {
                #skip this line - we do our own counting.
            }
            else {
                print STDERR $_;
                $status = -1;
            }
        }
        close (TEST_OUTPUT);
    }

    my $all_received = 1;
    if ($a_events != 100 || $b_events != 100 || $c_events != 100) {
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

if (run_test () != 0) {
    $status = 1;
}

if (analyze_results () == -1) {
    $status = 1;
}

$test->DeleteFile ($supplier_ior);
$test->DeleteFile ($consumer_ior);
$test->DeleteFile ($outputs[0]);
$test->DeleteFile ($outputs[1]);
$test->DeleteFile ($outputs[2]);
$test->DeleteFile ($outputs[3]);

exit $status;
