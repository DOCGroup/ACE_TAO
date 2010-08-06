eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# Purpose:
#       To test the FaultConsumer
#
# Process being tested:
#       Fault_Consumer
#           implements FaultConsumer interface.
# Processes used in test:
#       FT_Replica
#           implements TestReplica interface.
#           implements PullMonitorable.
#       Fault_Detector
#         implements FaultDetectorFactory interface
#         implements PullMonitorable interface
#       FT_Client
#           client for TestReplica interface.
#           client for PullMonitorable.
#       Fault_Notifier
#           implements the FaultNotifier interface.
#           propagates fault reports to the FaultConsumer.
#
# Test Scenario (***Test: marks behavior being tested):
#   Phase 1:
#     Start two FT_Replicas
#       FT_Replicas write TestReplica IORs (FR#1 and FR#2) to files
#     Start the Fault_Detector
#       Fault_Detector writes its IOR (FDF) to a file
#     Start the Fault_Notifier
#       Fault_Notifier writes its IOR (FN) to a file.
#     Start the Fault_Consumer
#       Subscribes to the Fault_Notifier for fault reports.
#   Phase 2:
#     Wait for IORs: FR#1, FR#2, FDF, and FN
#     Start the StubAnalyzer giving it IORS: FR#1, FR#2 FDF, FN
#      StubAnalyzer calls FDF to create a FaultDetector
#       for each Replica.
#      StubAnalyzer subscribes to Fault_Notifier
#      StubAnalyzer writes dummy message(READY) to a file.
#   Phase 3:
#     Wait for READY
#     Start FT_Client giving it IORS: FR#1 and FR#2. [1]
#       FT_Client interacts with FR#1.
#       FT_Client asks FR#1 to fault.  It does so.
#       FT_Client notices fault and switches to FR#2. [1]
#       FD#1 notices fault and notifies Fault_Notifier
#       FD#1 terminates
#       ***Test: Fault_Notifier forwards notification to StubAnalyzer
#       StubAnalyzer prints notification.
#       FT_Client interacts with FR#2.
#       FT_Client asks FR#2 to shut down.
#       FT_Client shuts down.
#       FD#2 notices FR2 is gone, interprets this
#         as a fault, and sends notification to Fault_Notifier
#       FD#2 terminates.
#       ***Test: Fault_Notifier forwards notification to StubAnalyzer
#       StubAnalyzer prints notification.
#     Phase 4: shutting down.
#       All FaultDetectors have terminated so the FaultDetectorFactory
#        honors the "quit-on-idle" option on it's command line and exits.
#       StubAnalyzer compares # fault notifications to # replicas. When
#        they match, it "knows" that the test is over, so it shuts down.
#        As it does so, it disconnects its fault consumers from the FaultNotifier.
#       FaultNotifier notices the last fault consumer disconnecting and exits because
#        the "quit-on-idle" option was specified on the command line.
#     Phase 5: housekeeping
#       Wait for all processes to terminate.
#       Check termination status.
#       Delete temp files.
#
# [1] Client mediated fault tolerance.  These points will
#     change when IOGR support is available.
use lib "$ENV{ACE_ROOT}/bin";
#use lib '$ENV{ACE_ROOT}/bin';
use PerlACE::Run_Test;

########################
#command line options
#set defaults:
my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = 0;    # 0: use exes from Release directories
my($simulated) = 1;       # 1: use "client simulated" fault tolerance

foreach $i (@ARGV) {
  if ($i eq "--debug_build")
  {
    $debug_builds = 1;
  }
  elsif ($i eq "--no_simulate")  # reverse this once we have FT ORB support
  {
    $simulated = 0;
  }
  elsif ($i eq "-v")
  {
    $verbose += 1;
  }
}


my($build_directory) = "/Release";
if ( $debug_builds ) {
  $build_directory = "";
}

if ( $verbose > 1) {
  print "verbose: $verbose\n";
  print "debug_builds: $debug_builds -> $build_directory\n";
  print "simulated: $simulated\n";
}


#define temp files
my($factory1_ior) = PerlACE::LocalFile ("factory1.ior");
my($factory2_ior) = PerlACE::LocalFile ("factory2.ior");
my($replica1_ior) = PerlACE::LocalFile ("replica1.ior");
my($replica2_ior) = PerlACE::LocalFile ("replica2.ior");
my($detector_ior) = PerlACE::LocalFile ("detector.ior");
my($notifier_ior) = PerlACE::LocalFile ("notifier.ior");
my($ready_file) = PerlACE::LocalFile ("ready.file");
my($client_data) = PerlACE::LocalFile ("persistent.dat");

#discard junk from previous tests
unlink $factory1_ior;
unlink $factory2_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $detector_ior;
unlink $notifier_ior;
unlink $ready_file;
unlink $client_data;

my ($rm_endpoint) = "-ORBEndpoint iiop://localhost:2833";
my ($rm_initref) = "-ORBInitRef ReplicationManager=corbaloc::localhost:2833/ReplicationManager";

my($status) = 0;

my($REP1) = new PerlACE::Process (".$build_directory/ft_replica", "-f none -o $factory1_ior -t $replica1_ior -l loc1 -i type1 -q");
my($REP2) = new PerlACE::Process (".$build_directory/ft_replica", "-f none -o $factory2_ior -t $replica2_ior -l loc2 -i type1 -q");
my($DET) = new PerlACE::Process ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Detector$build_directory/Fault_Detector", "-r -o $detector_ior -q");
my($NOT) = new PerlACE::Process ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Notifier$build_directory/Fault_Notifier", "-r -o $notifier_ior -q");
my($CONS) = new PerlACE::Process (".$build_directory/ft_fault_consumer", "-o $ready_file -n file://$notifier_ior -q -d file://$detector_ior -r file://$replica1_ior -r file://$replica2_ior");

my($CL);
if ($simulated) {
  print "\nTEST: Preparing Client Mediated Fault Tolerance test.\n" if ($verbose);
  $CL = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica1_ior -f file://$replica2_ior -c testscript");
}else{
  print "\nTEST: Preparing IOGR based test.\n" if ($verbose);
  $CL = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica1_iogr -c testscript");
}

print "TEST: starting replica1 " . $REP1->CommandLine . "\n" if ($verbose);
$REP1->Spawn ();

print "TEST: waiting for replica 1's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($replica1_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$replica1_ior>\n";
    $REP1->Kill (); $REP1->TimedWait (1);
    exit 1;
}

print "\nTEST: starting replica2 " . $REP2->CommandLine . "\n" if ($verbose);
$REP2->Spawn ();

print "TEST: waiting for replica 2's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($replica2_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$replica2_ior>\n";
    $REP1->Kill (); $REP1->TimedWait (1);
    $REP2->Kill (); $REP2->TimedWait (1);
    exit 1;
}

print "\nTEST: starting detector factory " . $DET->CommandLine . "\n" if ($verbose);
$DET->Spawn ();

print "TEST: waiting for detector's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($detector_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$detector_ior>\n";
    $REP1->Kill (); $REP1->TimedWait (1);
    $REP2->Kill (); $REP2->TimedWait (1);
    $DET->Kill (); $DET->TimedWait(1);
    exit 1;
}

print "\nTEST: starting notifier " . $NOT->CommandLine . "\n" if ($verbose);
$NOT->Spawn ();

print "TEST: waiting for notifier's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($notifier_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$notifier_ior>\n";
    $REP1->Kill (); $REP1->TimedWait (1);
    $REP2->Kill (); $REP2->TimedWait (1);
    $DET->Kill (); $DET->TimedWait(1);
    $NOT->Kill (); $NOT->TimedWait(1);
    exit 1;
}

print "\nTEST: starting fault consumer " . $CONS->CommandLine . "\n" if ($verbose);
$CONS->Spawn ();

print "TEST: waiting for READY.FILE from fault consumer\n" if ($verbose);
if (PerlACE::waitforfile_timed ($ready_file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$ready_file>\n";
    $REP1->Kill (); $REP1->TimedWait (1);
    $REP2->Kill (); $REP2->TimedWait (1);
    $DET->Kill (); $DET->TimedWait(1);
    $NOT->Kill (); $NOT->TimedWait(1);
    $CONS->Kill (); $CONS->TimedWait(1);
    exit 1;
}

print "\nTEST: starting client " . $CL->CommandLine . "\n" if ($verbose);
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "TEST ERROR: client returned $client\n";
    $status = 1;
}

print "\nTEST: wait for replica 1.\n" if ($verbose);
$replica1 = $REP1->WaitKill (5);
if ($replica1 != 0) {
    print STDERR "TEST ERROR: replica returned $replica1\n";
    $status = 1;
}

print "\nTEST: wait for replica 2.\n" if ($verbose);
$replica2 = $REP2->WaitKill (5);
if ($replica2 != 0) {
    print STDERR "TEST ERROR: replica returned $replica2\n";
    $status = 1;
}

print "\nTEST: wait for detector factory to leave.\n" if ($verbose);
$detector = $DET->WaitKill (20);
if ($detector != 0) {
    print STDERR "TEST ERROR: detector returned $detector\n";
    $status = 1;
}

print "\nTEST: wait for notifier to leave.\n" if ($verbose);
$notifier = $NOT->WaitKill (20);
if ($notifier != 0) {
    print STDERR "TEST ERROR: notifier returned $notifier\n";
    $status = 1;
}

print "\nTEST: wait for fault consumer to leave.\n" if ($verbose);
$consumer = $CONS->WaitKill (20);
if ($consumer != 0) {
    print STDERR "TEST ERROR: fault consumer returned $consumer\n";
    $status = 1;
}

print "\nTEST: releasing scratch files.\n" if ($verbose);
unlink $replica1_ior;
unlink $replica2_ior;
unlink $detector_ior;
unlink $notifier_ior;
unlink $ready_file;

#client's work file
unlink $client_data;

exit $status;
