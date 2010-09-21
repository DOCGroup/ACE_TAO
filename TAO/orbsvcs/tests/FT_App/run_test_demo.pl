eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# Purpose:
#   Integration test for all FT services.
#
# Command line options:
#   --debug_build  use exes from this directory
#        if not specified use exes from ./release
#   --no_simulate
#        use real IOGR-based recovery.
#   -v  display test progress messages (repeating option increases verbosity
# Process being tested:
#   FT_ReplicationManager
#   Fault_Detector
#   Fault_Notifier
#
# Processes used in test:
#       FT_Replica * 3
#           implements GenericFactory interface to create TestReplicas
#           TestReplica implements TestReplica interface.
#           TestReplica implements PullMonitorable interface.
#       FT_Client
#           client for TestReplica interface.
#           client for PullMonitorable.
#       Object Group Creator
#           Creates groups of objects.
#
# Test Scenario
#
#   1)        Start the ReplicationManager (RM),
#   2)        Start the Fault Notification Server(FN)
#   2.1)      FN registers with RM.
#   2.2)      RM registers as consumer with FN
#   3)        Start FaultDetectorFactory at location shire (FD@shire)
#   3.1)      FD@shire registers with RM
#   4)        Start FaultDetectorFactory at location bree (FD@bree)
#   4.1)      FD@bree registers with RM
#   5)        Start Replica Factory at location shire (RF@shire) that can create hobbits
#   5.1)      RF@shire registers with RM to create hobbit@shire
#   6)        Start Replica Factory at location bree (RF@bree) that can create hobbits and elves.
#   6.1)      RF@bree registers with RM to create hobbit@bree
#   6.1)      RF@bree registers with RM to create elf@bree
#   6)        Start Replica Factory at location rivendell (RF@rivendell) that can create elves.
#   6.1)      RF@bree registers with RM to create elf@rivendell
#   7)        Start ObjectGroupCreator (OGC)
#   7.1)      OGC calls RM to create group of hobbits (IOGR1)
#   7.1.1)    OGC calls RF@shire to create hobbit@shire[1]
#   7.1.1.1)  OGC calls FD@shire to create FaultDetector for hobbit@shire[1]
#   7.1.1.2)  OGC adds hobbit@shire[1] to IOGR1.
#   7.1.2)    OGC calls RF@bree to craate hobbit@bree[1]
#   7.1.2.1)  OGC calls FD@bree to create FaultDetector for hobbit@bree[1]
#   7.1.2.2)  OGC adds hobbit@bree[1] to IOGR1.
#   7.2)      OGC calls RM to create group of elves (IOGR2)
#   7.2.1)    OGC calls RF@bree to create elf@bree[2]
#   7.2.1.1)  OGC calls FD@bree to create FaultDetector for elf@bree[2]
#   7.2.1.2)  OGC adds elf@bree[2] to IOGR2.
#   7.2.2)    OGC calls RF@rivendell to create elf@rivendell[1]
#   7.2.2.1)  OGC calls FD@shire to create FaultDetector for elf@rivendell[1]
#   7.2.2.2)  OGC adds elf@rivendell[1] to IOGR2.
#   7.3)      OGC calls RM to create group of hobbits (IOGR3)
#   7.3.1)    OGC calls RF@shire to create hobbit@shire[2]
#   7.3.1.1)  OGC calls FD@shire to create FaultDetector for hobbit@shire[2]
#   7.3.1.2)  OGC adds hobbit@shire[2] to IOGR2.
#   7.3.2)    OGC calls RF@bree to craate hobbit@bree[3]
#   7.3.2.1)  OGC calls FD@bree to create FaultDetector for hobbit@bree[3]
#   7.3.2.2)  OGC adds hobbit@bree[3] to IOGR3.
#   8)        Start client1 with IOGR1
#   8.1)      Hobbit@shire[1] fails. FD sends notification FN.
#   8.1.1)    FN sends notification to RM.
#   8.1.2)    RM removes hobbit@shire[1] from IOGR1.
#   8.2)      Client1 terminates itself and hobbit@bree[1].
#   9)        Start Cient2 with IOGR2.
#   9.1)      Repeat 8.1 & 8.2 using IOGR3.
#   10)       Start Cient3 with IOGR3.
#   10.1)     Repeat 8.1 & 8.2 using IOGR3.
#   11)       System manages to shut itself down.
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

########################
#command line options
#set defaults:

my($status) = 0;
my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = ($^O eq 'MSWin32' ? 0 : 1);;    # 0: use exes from Release directories
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
my $fault_detector1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $fault_detector2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $fault_notifier = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $ft_replica1 = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $ft_replica2 = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";
my $ft_replica3 = PerlACE::TestTarget::create_target (8) || die "Create target 8 failed\n";
my $ft_creator = PerlACE::TestTarget::create_target (9) || die "Create target 9 failed\n";
my $client1 = PerlACE::TestTarget::create_target (10) || die "Create target 10 failed\n";
my $client2 = PerlACE::TestTarget::create_target (11) || die "Create target 11 failed\n";
my $client3 = PerlACE::TestTarget::create_target (12) || die "Create target 12 failed\n";

my($role1) = "hobbit";
my($role2) = "elf";

my($location1) = "shire";
my($location2) = "bree";
my($location3) = "rivendell";

#define temp files
my($rm_ior) = "rm.ior";
my($factory1_ior) = "factory1.ior";
my($factory2_ior) = "factory2.ior";
my($factory3_ior) = "factory3.ior";
my($detector1_ior) = "detector1.ior";
my($detector2_ior) = "detector2.ior";
my($notifier_ior) = "notifier.ior";

my($replica1_ior) = "${role1}_${location1}_0.ior";
my($replica2_ior) = "${role1}_${location2}_0.ior";
my($replica3_ior) = "${role2}_${location2}_1.ior";
my($replica4_ior) = "${role2}_${location3}_0.ior";
my($replica5_ior) = "${role1}_${location1}_1.ior";
my($replica6_ior) = "${role1}_${location2}_2.ior";

my($replica1_iogr) = "${role1}_0.iogr";
my($replica2_iogr) = "${role2}_1.iogr";
my($replica3_iogr) = "${role1}_2.iogr";

my($client_data) = "persistent.dat";

# Files for rp_manager
my $rp_manager_ior = $rp_manager->LocalFile ($rm_ior);
$rp_manager->DeleteFile ($rm_ior);

# Files for fault_detector1
my $fault_detector1_ior = $fault_detector1->LocalFile ($detector1_ior);
$fault_detector1->DeleteFile ($detector1_ior);

# Files for fault_detector2
my $fault_detector2_ior = $fault_detector2->LocalFile ($detector2_ior);
$fault_detector2->DeleteFile ($detector2_ior);

# Files for fault_notifier
my $fault_notifier_ior = $fault_notifier->LocalFile ($notifier_ior);
$fault_notifier->DeleteFile ($notifier_ior);

# Files for ft_replica1
my $ft_replica1_factory1_ior = $ft_replica1->LocalFile ($factory1_ior);
my $ft_replica1_client_data = $ft_replica1->LocalFile ($client_data);

$ft_replica1->DeleteFile ($factory1_ior);
$ft_replica1->DeleteFile ($client_data);

# Files for ft_replica2
my $ft_replica2_factory2_ior = $ft_replica2->LocalFile ($factory2_ior);
my $ft_replica2_client_data = $ft_replica2->LocalFile ($client_data);

$ft_replica2->DeleteFile ($factory2_ior);
$ft_replica2->DeleteFile ($client_data);

# Files for ft_replica3
my $ft_replica3_factory3_ior = $ft_replica3->LocalFile ($factory3_ior);
my $ft_replica3_client_data = $ft_replica3->LocalFile ($client_data);

$ft_replica3->DeleteFile ($factory3_ior);
$ft_replica3->DeleteFile ($client_data);

# Files for ft_creator
my $creator_replica1_ior = $ft_creator->LocalFile ($replica1_ior);
my $creator_replica2_ior = $ft_creator->LocalFile ($replica2_ior);
my $creator_replica3_ior = $ft_creator->LocalFile ($replica3_ior);
my $creator_replica4_ior = $ft_creator->LocalFile ($replica4_ior);
my $creator_replica5_ior = $ft_creator->LocalFile ($replica5_ior);
my $creator_replica6_ior = $ft_creator->LocalFile ($replica6_ior);

my $creator_replica1_iogr = $ft_creator->LocalFile ($replica1_iogr);
my $creator_replica2_iogr = $ft_creator->LocalFile ($replica2_iogr);
my $creator_replica3_iogr = $ft_creator->LocalFile ($replica3_iogr);

$ft_creator->DeleteFile ($replica1_ior);
$ft_creator->DeleteFile ($replica2_ior);
$ft_creator->DeleteFile ($replica3_ior);
$ft_creator->DeleteFile ($replica4_ior);
$ft_creator->DeleteFile ($replica5_ior);
$ft_creator->DeleteFile ($replica6_ior);

$ft_creator->DeleteFile ($replica1_iogr);
$ft_creator->DeleteFile ($replica2_iogr);
$ft_creator->DeleteFile ($replica3_iogr);

# Files for client1
my $client1_replica1_ior = $client1->LocalFile ($replica1_ior);
my $client1_replica2_ior = $client1->LocalFile ($replica2_ior);
my $client1_replica1_iogr = $client1->LocalFile ($replica1_iogr);

$client1->DeleteFile ($replica1_ior);
$client1->DeleteFile ($replica2_ior);
$client1->DeleteFile ($replica1_iogr);

# Files for client2
my $client2_replica3_ior = $client2->LocalFile ($replica3_ior);
my $client2_replica4_ior = $client2->LocalFile ($replica4_ior);
my $client2_replica2_iogr = $client2->LocalFile ($replica2_iogr);

$client2->DeleteFile ($replica3_ior);
$client2->DeleteFile ($replica4_ior);
$client2->DeleteFile ($replica2_iogr);

# Files for client3
my $client3_replica5_ior = $client3->LocalFile ($replica5_ior);
my $client3_replica6_ior = $client3->LocalFile ($replica6_ior);
my $client3_replica3_iogr = $client3->LocalFile ($replica3_iogr);

$client3->DeleteFile ($replica5_ior);
$client3->DeleteFile ($replica6_ior);
$client3->DeleteFile ($replica3_iogr);

$hostname = $rp_manager->HostName ();
$port = $rp_manager->RandomPort ();

$RM = $rp_manager->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/FT_ReplicationManager" .
                                  "$build_directory/tao_ft_replicationmanager",
                                  "-o $rp_manager_ior " .
                                  "-ORBEndpoint iiop://$hostname:$port");

$RMC = $rp_manager_controller->CreateProcess (".$build_directory/replmgr_controller" ,
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-x");

$NOT = $fault_notifier->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Notifier" .
                                       "$build_directory/Fault_Notifier" ,
                                        "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                        "-o $fault_notifier_ior -q");

$DET1 = $fault_detector1->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Detector" .
                                       "$build_directory/Fault_Detector" ,
                                       "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                       "-o $fault_detector1_ior " .
                                       "-l $location1 -q");

$DET2 = $fault_detector2->CreateProcess ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Detector" .
                                       "$build_directory/Fault_Detector" ,
                                       "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                       "-o $fault_detector1_ior " .
                                       "-l $location2 -q");

$REP1 = $ft_replica1->CreateProcess (".$build_directory/ft_replica" ,
                                     "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                     "-o $ft_replica1_factory1_ior ".
                                     "-l $location1 -i $role1 -q -p $ft_replica1_client_data");

$REP2 = $ft_replica2->CreateProcess (".$build_directory/ft_replica" ,
                                     "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                     "-o $ft_replica2_factory2_ior ".
                                     "-l $location2 -i $role1 -i $role2 -q -p $ft_replica2_client_data");

$REP3 = $ft_replica3->CreateProcess (".$build_directory/ft_replica" ,
                                     "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                     "-o $ft_replica3_factory3_ior ".
                                     "-l $location3 -i $role2 -q -p $ft_replica3_client_data");

my($OGC);
my($CL1);
my($CL2);
my($CL3);
if ($simulated) {
    print "\nTEST: Preparing Client Mediated Fault Tolerance test.\n" if ($verbose);
    $OGC = $ft_creator->CreateProcess (".$build_directory/ft_create" ,
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-n -r $role1 -r $role2 -r $role1 -u $role3 -i");

    $CL1 = $client1->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client1_replica1_ior " .
                                 "-f file://$client1_replica2_ior " .
                                 "-c testscript");

    $CL2 = $client2->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client2_replica3_ior " .
                                 "-f file://$client2_replica4_ior " .
                                 "-c testscript");

    $CL3 = $client3->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client3_replica5_ior " .
                                 "-f file://$client3_replica6_ior " .
                                 "-c testscript");



}else{
    print "\nTEST: Preparing IOGR based test.\n" if ($verbose);
    $OGC = $ft_creator->CreateProcess (".$build_directory/ft_create" ,
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-n -r $role1 -r $role2 -r $role1 -u $role3 -g");

    $CL1 = $client1->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client1_replica1_iogr " .
                                 "-c testscript");

    $CL2 = $client2->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client2_replica2_iogr " .
                                 "-c testscript");

    $CL3 = $client3->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client3_replica3_iogr " .
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

#######################
# Start FaultNotifier
print "\nTEST: starting notifier " . $NOT->CommandLine . "\n" if ($verbose);
$NOT->Spawn ();

add_to_kills ($NOT);

print "TEST: waiting for notifier's IOR\n" if ($verbose);
if ($fault_notifier->WaitForFileTimed ($notifier_ior,
                               $fault_notifier->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_notifier_ior>\n";
    exit_and_kill (1);
}

##############################
# Start FaultDetectorFactory 1
print "\nTEST: Starting FaultDetectorFactory at $location1 " . $DET1->CommandLine . "\n" if ($verbose);
$DET1->Spawn ();

add_to_kills ($DET1);

print "TEST: waiting for FaultDetector's IOR\n" if ($verbose);
if ($fault_detector1->WaitForFileTimed ($detector1_ior,
                               $fault_detector1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_detector1_ior>\n";
    exit_and_kill (1);
}

##############################
# Start FaultDetectorFactory 2
print "\nTEST: Starting FaultDetectorFactory at $location2 " . $DET2->CommandLine . "\n" if ($verbose);
$DET2->Spawn ();

add_to_kills ($DET2);

print "TEST: waiting for FaultDetector's IOR\n" if ($verbose);
if ($fault_detector2->WaitForFileTimed ($detector2_ior,
                               $fault_detector2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$fault_detector1_ior>\n";
    exit_and_kill (1);
}

#################
# Start Factories

print "\nTEST: starting factory 1 " . $REP1->CommandLine . "\n" if ($verbose);
$REP1->Spawn ();

add_to_kills ($REP1);

print "TEST: waiting for factory 1's IOR\n" if ($verbose);
if ($ft_replica1->WaitForFileTimed ($factory1_ior,
                               $ft_replica1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_replica1_factory1_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: starting factory 2 " . $REP2->CommandLine . "\n" if ($verbose);
$REP2->Spawn ();

add_to_kills ($REP2);

print "TEST: waiting for factory 2's IOR\n" if ($verbose);
if ($ft_replica2->WaitForFileTimed ($factory2_ior,
                               $ft_replica2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_replica2_factory2_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: starting factory 3 " . $REP3->CommandLine . "\n" if ($verbose);
$REP3->Spawn ();

add_to_kills ($REP3);

print "TEST: waiting for factory 3's IOR\n" if ($verbose);
if ($ft_replica3->WaitForFileTimed ($factory3_ior,
                               $ft_replica3->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_replica3_factory3_ior>\n";
    exit_and_kill (1);
}
######################
# Create object groups
print "\nTEST: Starting object group creator " . $OGC->CommandLine . "\n" if ($verbose);
$OGC->Spawn ();

print "\nTEST: wait for object group creator.\n" if ($verbose);

$status_creator = $OGC->WaitKill ($ft_creator->ProcessStartWaitInterval());

if ($status_creator != 0) {
    print STDERR "TEST ERROR: Object Group Creator returned $status_creator\n";
    exit_and_kill (1);
}
# Get/Put files:
# replica1_ior: client1
if ($ft_creator->GetFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$creator_replica1_ior\n";
    exit_and_kill (1);
}
if ($client1->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client1_replica1_ior>\n";
    exit_and_kill (1);
}
# replica2_ior: client1
if ($ft_creator->GetFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$creator_replica2_ior\n";
    exit_and_kill (1);
}
if ($client1->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client1_replica2_ior>\n";
    exit_and_kill (1);
}
# replica3_ior: client2
if ($ft_creator->GetFile ($replica3_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$creator_replica3_ior\n";
    exit_and_kill (1);
}
if ($client2->PutFile ($replica3_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client2_replica3_ior>\n";
    exit_and_kill (1);
}
# replica4_ior: client2
if ($ft_creator->GetFile ($replica4_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$creator_replica4_ior\n";
    exit_and_kill (1);
}
if ($client2->PutFile ($replica4_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client2_replica4_ior>\n";
    exit_and_kill (1);
}
# replica5_ior: client3
if ($ft_creator->GetFile ($replica5_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$creator_replica5_ior\n";
    exit_and_kill (1);
}
if ($client3->PutFile ($replica5_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client3_replica5_ior>\n";
    exit_and_kill (1);
}
# replica6_ior: client3
if ($ft_creator->GetFile ($replica6_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$creator_replica6_ior\n";
    exit_and_kill (1);
}
if ($client3->PutFile ($replica6_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client3_replica6_ior>\n";
    exit_and_kill (1);
}

#############
# Run clients

print "\nTEST: Starting client using first group of hobbits " . $CL1->CommandLine . "\n" if ($verbose);
$status_client = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);
if ($status_client != 0) {
    print STDERR "TEST ERROR: client returned $status_client\n";
    exit_and_kill(1);
}

print "\nTEST: Starting client using group of elves " . $CL2->CommandLine . "\n" if ($verbose);
$status_client = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 45);
if ($status_client != 0) {
    print STDERR "TEST ERROR: client returned $status_client\n";
    exit_and_kill(1);
}

print "\nTEST: Starting client using second group of hobbits " . $CL3->CommandLine . "\n" if ($verbose);
$status_client = $CL3->SpawnWaitKill ($client3->ProcessStartWaitInterval() + 45);
if ($status_client != 0) {
    print STDERR "TEST ERROR: client returned $status_client\n";
    exit_and_kill(1);
}

######################
# Clean house and exit

print "\nTEST: wait for factory 1.\n" if ($verbose);
$status_replica = $REP1->WaitKill ($ft_replica1->ProcessStopWaitInterval() + 15);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    $status = 1;
}
print "\nTEST: wait for factory 2.\n" if ($verbose);
$status_replica = $REP2->WaitKill ($ft_replica2->ProcessStopWaitInterval() + 15);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    $status = 1;
}

print "\nTEST: wait for factory 3.\n" if ($verbose);
$status_replica = $REP3->WaitKill ($ft_replica3->ProcessStopWaitInterval() + 15);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    $status = 1;
}
print "\nTEST: wait for FaultDetectorFactory 1.\n" if ($verbose);
$status_detector = $DET1->WaitKill ($fault_detector1->ProcessStopWaitInterval() + 15);
if ($status_detector != 0) {
    print STDERR "TEST ERROR: FaultDetectorFactory returned $status_detector\n";
    $status = 1;
}
print "\nTEST: wait for FaultDetectorFactory 2.\n" if ($verbose);
$status_detector = $DET2->WaitKill ($fault_detector2->ProcessStopWaitInterval() + 15);
if ($status_detector != 0) {
    print STDERR "TEST ERROR: FaultDetectorFactory returned $status_detector\n";
    $status = 1;
}

print "\nTEST: shutting down the replication manager.\n" if ($verbose);
$status_controller = $RMC->SpawnWaitKill ($rp_manager_controller->ProcessStartWaitInterval() + 285);
if ($status_controller != 0) {
    print STDERR "TEST ERROR: replication manager controller returned $status_controller\n";
    $status = 1;
}
print "\nTEST: wait for ReplicationManager.\n" if ($verbose);
$status_repmgr = $RM->WaitKill ($rp_manager->ProcessStopWaitInterval() + 15);
if ($status_repmgr != 0) {
    print STDERR "TEST ERROR: ReplicationManager returned $status_repmgr\n";
    $status = 1;
}

print "\nTEST: wait for FaultNotifier.\n" if ($verbose);
$status_not = $NOT->WaitKill ($fault_notifier->ProcessStopWaitInterval());
if ($status_not != 0) {
    print STDERR "TEST ERROR: notifier returned $status_not\n";
    $status = 1;
}

print "\nTEST: releasing scratch files.\n" if ($verbose);

# Files for rp_manager
$rp_manager->DeleteFile ($rm_ior);

# Files for fault_detector1
$fault_detector1->DeleteFile ($detector1_ior);

# Files for fault_detector2
$fault_detector2->DeleteFile ($detector2_ior);

# Files for fault_notifier
$fault_notifier->DeleteFile ($notifier_ior);

# Files for ft_replica1
$ft_replica1->DeleteFile ($factory1_ior);
$ft_replica1->DeleteFile ($client_data);

# Files for ft_replica2
$ft_replica2->DeleteFile ($factory2_ior);
$ft_replica2->DeleteFile ($client_data);

# Files for ft_replica3
$ft_replica3->DeleteFile ($factory3_ior);
$ft_replica3->DeleteFile ($client_data);

# Files for ft_creator
$ft_creator->DeleteFile ($replica1_ior);
$ft_creator->DeleteFile ($replica2_ior);
$ft_creator->DeleteFile ($replica3_ior);
$ft_creator->DeleteFile ($replica4_ior);
$ft_creator->DeleteFile ($replica5_ior);
$ft_creator->DeleteFile ($replica6_ior);

$ft_creator->DeleteFile ($replica1_iogr);
$ft_creator->DeleteFile ($replica2_iogr);
$ft_creator->DeleteFile ($replica3_iogr);

# Files for client1
$client1->DeleteFile ($replica1_ior);
$client1->DeleteFile ($replica2_ior);
$client1->DeleteFile ($replica1_iogr);

# Files for client2
$client2->DeleteFile ($replica3_ior);
$client2->DeleteFile ($replica4_ior);
$client2->DeleteFile ($replica2_iogr);

# Files for client3
$client3->DeleteFile ($replica5_ior);
$client3->DeleteFile ($replica6_ior);
$client3->DeleteFile ($replica3_iogr);

exit $status;
