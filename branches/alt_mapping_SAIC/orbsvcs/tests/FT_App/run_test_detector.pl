eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# Purpose:
#       To test the FaultDetectorFactory and FaultDetectors
#
# Command line options:
#   --debug_build  use exes from this directory
#        if not specified use exes from ./release
#   -v  display test progress messages (repeating option increases verbosity
#
# Process being tested:
#   Fault_Detector
#     implements FaultDetectorFactory interface
#     implements PullMonitorable interface
#
# Processes used in test:
#   FT_Replica
#     implements TestReplica interface.
#     implements PullMonitorable.
#   StubNotifier
#     implements FaultNotifier interface (as a stub.)
#     implements PullMonitorable.
#     client for FaultDetectorFactory interface.
#   FT_Client
#     client for TestReplica interface.
#     client for PullMonitorable.
#
# Test Scenario ( ***Test: marks behavior being tested):
#     Start two FT_Replicas
#       FT_Replicas write TestReplica IORs (FR#1 and FR#2) to files
#     Start the Fault_Detector
#       Fault_Detector writes its IOR (FDF) to a file
#     Wait for IORs: FR#1, FR#2, and FDF
#     Start the StubNotifier giving it IORS: FR#1, FR#2 and FDF
#      StubNotifier calls FDF to create a FaultDetector
#       for each Replica.
#      StubNotifier writes FaultNotifier IOR (FN) to a file.
#     Wait for IOR: FN
#     Start FT_Client giving it IORS: FR#1 and FR#2.
#       FT_Client interacts with FR#1.
#       FT_Client asks FR#1 to fault.  It does so.
#       FT_Client notices fault and switches to FR#2.
#       ***Test: FD#1 notices fault and notifies StubNotifier
#       ***Test: FD#1 terminates
#       FT_Client interacts with FR#2.
#       FT_Client asks FR#2 to shut down.
#       FT_Client shuts down.
#       ***Test: FD#2 notices FR2 is gone, interprets this
#         as a fault, and notifies StubNotifier.
#       ***Test: FD#2 terminates.
#     Shut down
#       ***Test: FDF is idle, so it terminates.
#       StubNotifier sees FDF terminate, so it terminates
#     Cleanup
#       Wait for all processes to terminate.
#       Check termination status.
#       Delete temp files.
#
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

########################
#command line options
#set defaults:
my($status) = 0;
my($verbose) = 0;         # 1: report perl actions before executing them; 2 display settings from command line
my($debug_builds) = 0;    # 0: use exes from Release directories
my($simulated) = 1;       # 1: use "client simulated" fault tolerance

foreach $i (@ARGV) {
    if ($i eq "--debug_build"){
      $debug_builds = 1;
    }
    elsif ($i eq "-v"){
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

# List of the proccess which must be killed before exit with error
@kill_list = ();

sub add_to_kills{
    $goal = shift;
    @kill_list = (@kill_list, $goal);
}
sub exit_and_kill{
    $status = shift;
    foreach $goal (@kill_list){
        $goal->Kill (); $goal->TimedWait (1);
    }
    exit $status;
}

my $ft_replica1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ft_replica2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $fault_detector = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $fault_notifier = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $client = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";


$hostname = $fault_detector->HostName ();
$port = $fault_detector->RandomPort ();

#define temp files
my($factory1_ior) = "factory1.ior";
my($factory2_ior) = "factory2.ior";
my($replica1_ior) = "replica1.ior";
my($replica2_ior) = "replica2.ior";
my($detector_ior) = "detector.ior";
my($notifier_ior) = "notifier.ior";
my($client_data) = "persistent.dat";

# Files for ft_replica1
my $ft_replica1_factory1_ior = $ft_replica1->LocalFile ($factory1_ior);
my $ft_replica1_replica1_ior = $ft_replica1->LocalFile ($replica1_ior);
my $ft_replica1_client_data = $ft_replica1->LocalFile ($client_data);

$ft_replica1->DeleteFile ($factory1_ior);
$ft_replica1->DeleteFile ($replica1_ior);
$ft_replica1->DeleteFile ($client_data);

# Files for ft_replica2
my $ft_replica2_factory2_ior = $ft_replica2->LocalFile ($factory2_ior);
my $ft_replica2_replica2_ior = $ft_replica2->LocalFile ($replica2_ior);
my $ft_replica2_client_data = $ft_replica2->LocalFile ($client_data);

$ft_replica2->DeleteFile ($factory2_ior);
$ft_replica2->DeleteFile ($replica2_ior);
$ft_replica2->DeleteFile ($client_data);

# Files for fault_detector
my $fault_detector_ior = $fault_detector->LocalFile ($detector_ior);
$fault_detector->DeleteFile ($detector_ior);

# Files for fault_notifier
my $fault_notifier_ior = $fault_notifier->LocalFile ($notifier_ior);
my $notifier_replica1_ior = $fault_notifier->LocalFile ($replica1_ior);
my $notifier_replica2_ior = $fault_notifier->LocalFile ($replica2_ior);
my $notifier_detector_ior = $fault_notifier->LocalFile ($detector_ior);

$fault_notifier->DeleteFile ($notifier_ior);
$fault_notifier->DeleteFile ($replica1_ior);
$fault_notifier->DeleteFile ($replica2_ior);
$fault_notifier->DeleteFile ($detector_ior);

# Files for client
my $client_replica1_ior = $client->LocalFile ($replica1_ior);
my $client_replica2_ior = $client->LocalFile ($replica2_ior);

$client->DeleteFile ($replica1_ior);
$client->DeleteFile ($replica2_ior);

### Create process
$REP1 = $ft_replica1->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica1_factory1_ior ".
                                 "-f none -t $ft_replica1_replica1_ior " .
                                 "-l loc1 -i type1 -q -p $ft_replica1_client_data");

$REP2 = $ft_replica2->CreateProcess (".$build_directory/ft_replica" ,
                                     "-o $ft_replica2_factory2_ior ".
                                     "-f none -t $ft_replica2_replica2_ior " .
                                     "-l loc2 -i type1 -q -p $ft_replica2_client_data");

$DET = $fault_detector->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Detector" .
                                       "$build_directory/tao_fault_detector" ,
                                       "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                       "-o $fault_detector_ior -q");

$NOT = $fault_notifier->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Notifier" .
                                       "$build_directory/tao_fault_notifier" ,
                                        "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                        "-o $fault_notifier_ior -q " .
                                        "-d file://$notifier_detector_ior " .
                                        "-r file://$notifier_replica1_ior -r file://$notifier_replica2_ior");

$CL = $client->CreateProcess (".$build_directory/ft_client",
                                  "-f file://$client_replica1_ior " .
                                  "-f file://$client_replica2_ior " .
                                  "-c testscript");


print "TEST: starting replica1 ". $REP1->CommandLine . "\n" if ($verbose);
$REP1->Spawn ();

add_to_kills ($REP1);

print "TEST: waiting for replica 1's IOR\n" if ($verbose);
if ($ft_replica1->WaitForFileTimed ($replica1_ior,
                               $ft_replica1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_replica1_replica1_ior>\n";
    exit_and_kill (1);
}
# Get/Put $replica1_ior: notifier_replica1_ior $client_replica1_ior
if ($ft_replica1->GetFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_replica1_replica1_ior>\n";
    exit_and_kill (1);
}
if ($fault_notifier->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$notifier_replica1_ior>\n";
    exit_and_kill (1);
}
if ($client->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_replica1_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: starting replica2 " . $REP2->CommandLine . "\n" if ($verbose);
$REP2->Spawn ();

add_to_kills ($REP2);

print "TEST: waiting for replica 2's IOR\n" if ($verbose);
if ($ft_replica2->WaitForFileTimed ($replica2_ior,
                               $ft_replica2->ProcessStartWaitInterval()) == -1) {
    print STDERR "TEST ERROR: cannot find file <$ft_replica2_replica2_ior>\n";
    exit_and_kill (1);
}
# Get/Put $replica2_ior: notifier_replica2_ior $client_replica2_ior
if ($ft_replica2->GetFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_replica2_replica2_ior>\n";
    exit_and_kill (1);
}
if ($fault_notifier->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$notifier_replica2_ior>\n";
    exit_and_kill (1);
}
if ($client->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_replica2_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: starting detector factory " . $DET->CommandLine . "\n" if ($verbose);
$DET->Spawn ();

add_to_kills ($DET);

print "TEST: waiting for detector's IOR\n" if ($verbose);
if ($fault_detector->WaitForFileTimed ($detector_ior,
                               $fault_detector->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_detector_ior>\n";
    exit_and_kill (1);
}

# Get/Put $detector_ior: $notifier_detector_ior
if ($fault_detector->GetFile ($detector_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$fault_detector_ior>\n";
    exit_and_kill (1);
}
if ($fault_notifier->PutFile ($detector_ior) == -1) {
    print STDERR "ERROR: cannot set file <$notifier_detector_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: starting notifier " . $NOT->CommandLine . "\n" if ($verbose);
$NOT->Spawn ();

add_to_kills ($NOT);

print "TEST: waiting for notifier's IOR\n" if ($verbose);
if ($fault_notifier->WaitForFileTimed ($notifier_ior,
                               $fault_notifier->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_notifier_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: starting client." . $CL->CommandLine . "\n" if ($verbose);

$status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($status != 0) {
    print STDERR "TEST ERROR: client returned $status\n";
    $status = 1;
}

print "\nTEST: wait for replica 1.\n" if ($verbose);
$status_replica = $REP1->WaitKill ($ft_replica1->ProcessStopWaitInterval() + 45);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    $status = 1;
}
print "\nTEST: wait for replica 2.\n" if ($verbose);
$status_replica = $REP2->WaitKill ($ft_replica2->ProcessStopWaitInterval() + 45);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    $status = 1;
}

print "\nTEST: wait for detector factory to leave.\n" if ($verbose);
$status_det = $DET->WaitKill ($fault_detector->ProcessStopWaitInterval() + 45);
if ($status_det != 0) {
    print STDERR "TEST ERROR: detector returned $status_det\n";
    $status = 1;
}

print "\nTEST: wait for notifier to leave.\n" if ($verbose);
$status_not = $NOT->WaitKill ($fault_notifier->ProcessStopWaitInterval() + 45);
if ($status_not != 0) {
    print STDERR "TEST ERROR: notifier returned $status_not\n";
    $status = 1;
}

print "\nTEST: releasing scratch files.\n" if ($verbose);
# Files for ft_replica1
$ft_replica1->DeleteFile ($factory1_ior);
$ft_replica1->DeleteFile ($replica1_ior);
$ft_replica1->DeleteFile ($client_data);

# Files for ft_replica2
$ft_replica2->DeleteFile ($factory2_ior);
$ft_replica2->DeleteFile ($replica2_ior);
$ft_replica2->DeleteFile ($client_data);

# Files for fault_detector
$fault_detector->DeleteFile ($detector_ior);

# Files for fault_notifier
$fault_notifier->DeleteFile ($notifier_ior);
$fault_notifier->DeleteFile ($replica1_ior);
$fault_notifier->DeleteFile ($replica2_ior);
$fault_notifier->DeleteFile ($detector_ior);

# Files for client
$client->DeleteFile ($replica1_ior);
$client->DeleteFile ($replica2_ior);

exit $status;
