eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# Purpose:
#       To test the FaultNotifier
#
# Process being tested:
#       Fault_Notifier
#           implements FaultNotifier interface.
# Processes used in test:
#       ReplicationManager
#
#       FT_Replica
#           implements TestReplica interface.
#           implements PullMonitorable.
#       Fault_Detector
#         implements FaultDetectorFactory interface
#         implements PullMonitorable interface
#       FT_Client
#           client for TestReplica interface.
#           client for PullMonitorable.
#       StubAnalyzer
#           Subscribes to Fault_Notfier
#
# Test Scenario (***Test: marks behavior being tested):
#   Phase 1:
#     Start two FT_Replicas
#       FT_Replicas write TestReplica IORs (FR#1 and FR#2) to files
#     Start the Fault_Detector
#       Fault_Detector writes its IOR (FDF) to a file
#     Start the Fault_Notifier
#       Fault_Notifier writes its IOR (FN) to a file.
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
use PerlACE::TestTarget;

########################
#command line options
#set defaults:

my($status) = 0;
my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = 0;    # 0: use exes from Release directories
my($simulated) = 1;       # 1: use "client simulated" fault tolerance


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

foreach $i (@ARGV) {
    if ($i eq "--debug_build"){
        $debug_builds = 1;
    }
    elsif ($i eq "--no_simulate"){  # reverse this once we have FT ORB support
        $simulated = 0;
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

my $rp_manager = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $rp_manager_controller = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ft_replica1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $ft_replica2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $fault_detector = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $fault_notifier = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $fault_analyzer = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";
my $client = PerlACE::TestTarget::create_target (8) || die "Create target 8 failed\n";

#define temp files

$hostname = $rp_manager->HostName ();
$port = $rp_manager->RandomPort ();

my($rm_ior) = "rm.ior";
my($factory1_ior) = "factory1.ior";
my($factory2_ior) = "factory2.ior";
my($replica1_ior) = "replica1.ior";
my($replica2_ior) = "replica2.ior";
my($detector_ior) = "detector.ior";
my($notifier_ior) = "notifier.ior";
my($client_data) = "persistent.dat";
my($ready_file) = "ready.file";

# Files for rp_manager
my $rp_manager_ior = $rp_manager->LocalFile ($rm_ior);
$rp_manager->DeleteFile ($rm_ior);

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
$fault_notifier->DeleteFile ($notifier_ior);

# Files for fault_analyzer
my $analyzer_replica1_ior = $fault_analyzer->LocalFile ($replica1_ior);
my $analyzer_replica2_ior = $fault_analyzer->LocalFile ($replica2_ior);
my $analyzer_notifier_ior = $fault_analyzer->LocalFile ($notifier_ior);
my $analyzer_detector_ior = $fault_analyzer->LocalFile ($detector_ior);
my $analyzer_ready_file = $fault_analyzer->LocalFile ($ready_file);

$fault_analyzer->DeleteFile ($replica1_ior);
$fault_analyzer->DeleteFile ($replica2_ior);
$fault_analyzer->DeleteFile ($notifier_ior);
$fault_analyzer->DeleteFile ($detector_ior);
$fault_analyzer->DeleteFile ($ready_file);

# Files for client
my $client_replica1_ior = $client->LocalFile ($replica1_ior);
my $client_replica2_ior = $client->LocalFile ($replica2_ior);

$client->DeleteFile ($replica1_ior);
$client->DeleteFile ($replica2_ior);


### Create process
$RM = $rp_manager->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/FT_ReplicationManager" .
                                  "$build_directory/tao_ft_replicationmanager",
                                  "-o $rp_manager_ior " .
                                  "-ORBEndpoint iiop://$hostname:$port");

$RMC = $rp_manager_controller->CreateProcess (".$build_directory/replmgr_controller" ,
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-x");

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
                                       "$build_directory/Fault_Notifier" ,
                                        "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                        "-o $fault_notifier_ior -q");

$ANA = $fault_analyzer->CreateProcess (".$build_directory/ft_analyzer",
                                        "-o $analyzer_ready_file " .
                                        "-n file://$analyzer_notifier_ior " .
                                        "-d file://$analyzer_detector_ior " .
                                        "-r file://$analyzer_replica1_ior " .
                                        "-r file://$analyzer_replica2_ior -q");

my($CL);
if ($simulated) {
    print "\nTEST: Preparing Client Mediated Fault Tolerance test.\n" if ($verbose);
    $CL = $client->CreateProcess (".$build_directory/ft_client",
                                  "-f file://$client_replica1_ior " .
                                  "-f file://$client_replica2_ior " .
                                  "-c testscript");

}else{
    print "\nTEST: Preparing IOGR based test.\n" if ($verbose);
    $CL = $client->CreateProcess (".$build_directory/ft_client",
                                  "-f file://$client_replica1_ior " .
                                  "-c testscript");
}

#######################
# ReplicationManager

print "\nTEST: starting ReplicationManager " . $RM->CommandLine . "\n" if ($verbose);
$RM->Spawn ();

add_to_kills ($RM);

print "TEST: waiting for registry's IOR\n" if ($verbose);

if ($rp_manager->WaitForFileTimed ($rm_ior,
                               $rp_manager->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$rp_manager_ior>\n";
    exit_and_kill (1);
}

##########
# Notifier
print "\nTEST: starting notifier " . $NOT->CommandLine . "\n" if ($verbose);
$NOT->Spawn ();

add_to_kills ($NOT);

print "TEST: waiting for notifier's IOR\n" if ($verbose);
if ($fault_notifier->WaitForFileTimed ($notifier_ior,
                               $fault_notifier->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_notifier_ior>\n";
    exit_and_kill (1);
}
# Get/Put $notifier_ior: $analyzer_notifier_ior
if ($fault_notifier->GetFile ($notifier_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$fault_notifier_ior>\n";
    exit_and_kill (1);
}
if ($fault_analyzer->PutFile ($notifier_ior) == -1) {
    print STDERR "ERROR: cannot set file <$analyzer_notifier_ior>\n";
    exit_and_kill (1);
}

##########
# Detector
print "\nTEST: starting detector factory " . $DET->CommandLine . "\n" if ($verbose);

$DET->Spawn ();

add_to_kills ($DET);

print "TEST: waiting for detector's IOR\n" if ($verbose);
if ($fault_detector->WaitForFileTimed ($detector_ior,
                               $fault_detector->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_detector_ior>\n";
    exit_and_kill (1);
}

# Get/Put $detector_ior: $analyzer_detector_ior
if ($fault_detector->GetFile ($detector_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$fault_detector_ior>\n";
    exit_and_kill (1);
}
if ($fault_analyzer->PutFile ($detector_ior) == -1) {
    print STDERR "ERROR: cannot set file <$analyzer_detector_ior>\n";
    exit_and_kill (1);
}


###########
# Replica 1
print "\nTEST: starting replica1 " . $REP1->CommandLine . "\n" if ($verbose);

$REP1->Spawn ();

add_to_kills ($REP1);

print "TEST: waiting for replica 1's IOR\n" if ($verbose);
if ($ft_replica1->WaitForFileTimed ($replica1_ior,
                               $ft_replica1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_replica1_replica1_ior>\n";
    exit_and_kill (1);
}
# Get/Put $replica1_ior: analyzer_replica1_ior $client_replica1_ior
if ($ft_replica1->GetFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_replica1_replica1_ior>\n";
    exit_and_kill (1);
}
if ($fault_analyzer->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$analyzer_replica1_ior>\n";
    exit_and_kill (1);
}
if ($client->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_replica1_ior>\n";
    exit_and_kill (1);
}

###########
# Replica 2
print "\nTEST: starting replica2 " . $REP2->CommandLine . "\n" if ($verbose);
$REP2->Spawn ();

add_to_kills ($REP2);

print "TEST: waiting for replica 2's IOR\n" if ($verbose);
if ($ft_replica2->WaitForFileTimed ($replica2_ior,
                               $ft_replica2->ProcessStartWaitInterval()) == -1) {
    print STDERR "TEST ERROR: cannot find file <$ft_replica2_replica2_ior>\n";
    exit_and_kill (1);
}

# Get/Put $replica2_ior: analyzer_replica2_ior $client_replica2_ior
if ($ft_replica2->GetFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_replica2_replica2_ior>\n";
    exit_and_kill (1);
}
if ($fault_analyzer->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$analyzer_replica2_ior>\n";
    exit_and_kill (1);
}
if ($client->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_replica2_ior>\n";
    exit_and_kill (1);
}

##########
# Analyzer
print "\nTEST: starting analyzer " . $ANA->CommandLine . "\n" if ($verbose);
$ANA->Spawn ();

add_to_kills ($ANA);

print "TEST: waiting for READY.FILE from analyzer\n" if ($verbose);

if ($fault_analyzer->WaitForFileTimed ($ready_file,
                               $fault_analyzer->ProcessStartWaitInterval()) == -1) {
    print STDERR "TEST ERROR: cannot find file <$analyzer_ready_file>\n";
    exit_and_kill (1);
}

########
# Client
print "\nTEST: starting client " . $CL->CommandLine . "\n" if ($verbose);

$status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($status != 0) {
    print STDERR "TEST ERROR: client returned $status\n";
    $status = 1;
}

print "\nTEST: wait for replica 1.\n" if ($verbose);

$status = $REP1->WaitKill ($ft_replica1->ProcessStopWaitInterval());
if ($status != 0) {
    print STDERR "TEST ERROR: replica returned $status\n";
    $status = 1;
}
print "\nTEST: wait for replica 2.\n" if ($verbose);

$status = $REP2->WaitKill ($ft_replica2->ProcessStopWaitInterval());
if ($status != 0) {
    print STDERR "TEST ERROR: replica returned $status\n";
    $status = 1;
}

print "\nTEST: wait for detector factory to leave.\n" if ($verbose);
$status = $DET->WaitKill ($fault_detector->ProcessStopWaitInterval());
if ($status != 0) {
    print STDERR "TEST ERROR: detector returned $status\n";
    $status = 1;
}

print "\nTEST: wait for analyzer to leave.\n" if ($verbose);
$status = $ANA->WaitKill ($fault_analyzer->ProcessStopWaitInterval());
if ($status != 0) {
    print STDERR "TEST ERROR: analyzer returned $status\n";
    $status = 1;
}

print "\nTEST: shutting down the replication manager.\n" if ($verbose);
$status = $RMC->SpawnWaitKill ($rp_manager_controller->ProcessStartWaitInterval() + 285);
if ($status != 0) {
    print STDERR "TEST ERROR: replication manager controller returned $status\n";
    $status = 1;
}

print "\nTEST: wait for ReplicationManager.\n" if ($verbose);
$status = $RM->WaitKill ($rp_manager->ProcessStopWaitInterval());
if ($status != 0) {
    print STDERR "TEST ERROR: ReplicationManager returned $status\n";
    $status = 1;
}

print "\nTEST: wait for notifier to leave.\n" if ($verbose);
$status = $NOT->WaitKill ($fault_notifier->ProcessStopWaitInterval());
if ($status != 0) {
    print STDERR "TEST ERROR: notifier returned $status\n";
    $status = 1;
}

print "\nTEST: releasing scratch files.\n" if ($verbose);

# Files for rp_manager
$rp_manager->DeleteFile ($rm_ior);

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

# Files for fault_analyzer
$fault_analyzer->DeleteFile ($replica1_ior);
$fault_analyzer->DeleteFile ($replica2_ior);
$fault_analyzer->DeleteFile ($notifier_ior);
$fault_analyzer->DeleteFile ($detector_ior);
$fault_analyzer->DeleteFile ($ready_file);

# Files for client
$client->DeleteFile ($replica1_ior);
$client->DeleteFile ($replica2_ior);

exit $status;
