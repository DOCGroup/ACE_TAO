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
#use lib '$ENV{ACE_ROOT}/bin';
use PerlACE::Run_Test;

########################
#command line options
#set defaults:
my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = ($^O eq 'MSWin32' ? 0 : 1);;    # 0: use exes from Release directories
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

my($role1) = "hobbit";
my($role2) = "elf";

my($location1) = "shire";
my($location2) = "bree";
my($location3) = "rivendell";

my ($rm_endpoint) = "-ORBEndpoint iiop://localhost:2833";
my ($rm_initref) = "-ORBInitRef ReplicationManager=corbaloc::localhost:2833/ReplicationManager";

#define temp files
my($rm_ior) = PerlACE::LocalFile ("rm.ior");
my($notifier_ior) = PerlACE::LocalFile ("notifier.ior");
my($detector1_ior) = PerlACE::LocalFile ("detector1.ior");
my($detector2_ior) = PerlACE::LocalFile ("detector2.ior");
my($factory1_ior) = PerlACE::LocalFile ("factory1.ior");
my($factory2_ior) = PerlACE::LocalFile ("factory2.ior");
my($factory3_ior) = PerlACE::LocalFile ("factory3.ior");
my($replica1_ior) = PerlACE::LocalFile ("${role1}_${location1}_0.ior");
my($replica2_ior) = PerlACE::LocalFile ("${role1}_${location2}_0.ior");
my($replica3_ior) = PerlACE::LocalFile ("${role2}_${location2}_1.ior");
my($replica4_ior) = PerlACE::LocalFile ("${role2}_${location3}_0.ior");
my($replica5_ior) = PerlACE::LocalFile ("${role1}_${location1}_1.ior");
my($replica6_ior) = PerlACE::LocalFile ("${role1}_${location2}_2.ior");

my($replica1_iogr) = PerlACE::LocalFile ("${role1}_0.iogr");
my($replica2_iogr) = PerlACE::LocalFile ("${role2}_1.iogr");
my($replica3_iogr) = PerlACE::LocalFile ("${role1}_2.iogr");

my($client_data) = PerlACE::LocalFile ("persistent.dat");

#discard junk from previous tests
unlink $rm_ior;
unlink $notifier_ior;
unlink $detector1_ior;
unlink $detector2_ior;
unlink $factory1_ior;
unlink $factory2_ior;
unlink $factory3_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $replica3_ior;
unlink $replica4_ior;
unlink $replica5_ior;
unlink $replica6_ior;
unlink $replica1_iogr;
unlink $replica2_iogr;

unlink $client_data;

my($status) = 0;

my($RM) = new PerlACE::Process ("$ENV{'TAO_ROOT'}/orbsvcs/FT_ReplicationManager$build_directory/FT_ReplicationManager", "-o $rm_ior $rm_endpoint");
my($RMC) = new PerlACE::Process (".$build_directory/replmgr_controller", "$rm_initref -x");
my($NOT) = new PerlACE::Process ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Notifier$build_directory/Fault_Notifier", "$rm_initref -o $notifier_ior -q");
my($DET1) = new PerlACE::Process ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Detector$build_directory/Fault_Detector", "$rm_initref -o $detector1_ior -l $location1 -q");
my($DET2) = new PerlACE::Process ("$ENV{'TAO_ROOT'}/orbsvcs/Fault_Detector$build_directory/Fault_Detector", "$rm_initref -o $detector2_ior -l $location2 -q");
my($FAC1) = new PerlACE::Process (".$build_directory/ft_replica", "$rm_initref -o $factory1_ior -l $location1 -i $role1 -q");
my($FAC2) = new PerlACE::Process (".$build_directory/ft_replica", "$rm_initref -o $factory2_ior -l $location2 -i $role1 -i $role2 -q");
my($FAC3) = new PerlACE::Process (".$build_directory/ft_replica", "$rm_initref -o $factory3_ior -l $location3 -i $role2 -q");

my($OGC);
my($CL1);
my($CL2);
my($CL3);
if ($simulated) {
  print "\nTEST: Preparing Client Mediated Fault Tolerance test.\n" if ($verbose);
  $OGC= new PerlACE::Process (".$build_directory/ft_create", "$rm_initref  -r $role1 -r $role2 -r $role1 -i -n");
  $CL1 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica1_ior -f file://$replica2_ior -c testscript");
  $CL2 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica3_ior -f file://$replica4_ior -c testscript");
  $CL3 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica5_ior -f file://$replica6_ior -c testscript");
}else{
  print "\nTEST: Preparing IOGR based test.\n" if ($verbose);
  $OGC = new PerlACE::Process (".$build_directory/ft_create", "$rm_initref  -r $role1 -r $role2 -r $role1 -g -n");
  $CL1 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica1_iogr -c testscript");
  $CL2 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica2_iogr -c testscript");
  $CL3 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica3_iogr -c testscript");
}

#######################
# Start ReplicationManager

print "\nTEST: Starting ReplicationManager " . $RM->CommandLine . "\n" if ($verbose);
$RM->Spawn ();

print "TEST: waiting for registry's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($rm_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$rm_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    exit 1;
}

#######################
# Start FaultNotifier
print "\nTEST: Starting FaultNotifier " . $NOT->CommandLine . "\n" if ($verbose);
$NOT->Spawn ();

print "TEST: waiting for FaultNotifier's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($notifier_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$notifier_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    exit 1;
}


##############################
# Start FaultDetectorFactory 1
print "\nTEST: Starting FaultDetectorFactory at $location1 " . $DET1->CommandLine . "\n" if ($verbose);
$DET1->Spawn ();

print "TEST: waiting for FaultDetector's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($detector1_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$detector1_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    $DET1->Kill (); $DET1->TimedWait (1);
    exit 1;
}

##############################
# Start FaultDetectorFactory 2
print "\nTEST: Starting FaultDetectorFactory at $location2 " . $DET2->CommandLine . "\n" if ($verbose);
$DET2->Spawn ();

print "TEST: waiting for FaultDetector's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($detector2_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$detector2_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    $DET1->Kill (); $DET1->TimedWait (1);
    $DET2->Kill (); $DET2->TimedWait (1);
    exit 1;
}


#################
# Start Factories

print "\nTEST: Starting replica factory at $location1 " . $FAC1->CommandLine . "\n" if ($verbose);
$FAC1->Spawn ();

print "TEST: waiting for factory 1's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($factory1_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$factory1_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    $DET1->Kill (); $DET1->TimedWait (1);
    $DET2->Kill (); $DET2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    exit 1;
}

print "\nTEST: Starting replica factory at $location2 " . $FAC2->CommandLine . "\n" if ($verbose);
$FAC2->Spawn ();

print "TEST: waiting for factory 2's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($factory2_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$factory2_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    $DET1->Kill (); $DET1->TimedWait (1);
    $DET2->Kill (); $DET2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    exit 1;
}

print "\nTEST: Starting replica factory at $location3 " . $FAC3->CommandLine . "\n" if ($verbose);
$FAC3->Spawn ();

print "TEST: waiting for factory 3's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($factory3_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "TEST ERROR: cannot find file <$factory3_ior>\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    $DET1->Kill (); $DET1->TimedWait (1);
    $DET2->Kill (); $DET2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $FAC3->Kill (); $FAC3->TimedWait (1);
    exit 1;
}

######################
# Create object groups

print "\nTEST: Starting object group creator " . $OGC->CommandLine . "\n" if ($verbose);
$OGC->Spawn ();

print "\nTEST: wait for object group creator.\n" if ($verbose);
$config = $OGC->WaitKill (30);
if ($config != 0) {
    print STDERR "TEST ERROR: Object Group Creator returned $config\n";
    $RM->Kill (); $RM->TimedWait (1);
    $NOT->Kill (); $NOT->TimedWait (1);
    $DET1->Kill (); $DET1->TimedWait (1);
    $DET2->Kill (); $DET2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $FAC3->Kill (); $FAC3->TimedWait (1);
    exit 1;
}

#############
# Run clients

print "\nTEST: Starting client using first group of hobbits " . $CL1->CommandLine . "\n" if ($verbose);
$client = $CL1->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "TEST ERROR: client returned $client\n";
    $status = 1;
}

print "\nTEST: Starting client using group of elves " . $CL2->CommandLine . "\n" if ($verbose);
$client2 = $CL2->SpawnWaitKill (60);

if ($client2 != 0) {
    print STDERR "TEST ERROR: client returned $client2\n";
    $status = 1;
}

print "\nTEST: Starting client using second group of hobbits " . $CL3->CommandLine . "\n" if ($verbose);
$client3 = $CL3->SpawnWaitKill (60);

if ($client3 != 0) {
    print STDERR "TEST ERROR: client returned $client3\n";
    $status = 1;
}

######################
# Clean house and exit

print "\nTEST: wait for factory 1.\n" if ($verbose);
$factory1 = $FAC1->WaitKill (30);
if ($factory1 != 0) {
    print STDERR "TEST ERROR: replica returned $factory 1\n";
    $status = 1;
}

print "\nTEST: wait for factory 2.\n" if ($verbose);
$factory2 = $FAC2->WaitKill (30);
if ($factory2 != 0) {
    print STDERR "TEST ERROR: factory 2 returned $factory2\n";
    $status = 1;
}

print "\nTEST: wait for factory 3.\n" if ($verbose);
$factory3 = $FAC3->WaitKill (30);
if ($factory3 != 0) {
    print STDERR "TEST ERROR: factory 3 returned $factory3\n";
    $status = 1;
}

print "\nTEST: wait for FaultDetectorFactory 1.\n" if ($verbose);
$detector1 = $DET1->WaitKill (30);
if ($detector1 != 0) {
    print STDERR "TEST ERROR: FaultDetectorFactory returned $detector1\n";
    $status = 1;
}

print "\nTEST: wait for FaultDetectorFactory 2.\n" if ($verbose);
$detector2 = $DET2->WaitKill (30);
if ($detector2 != 0) {
    print STDERR "TEST ERROR: FaultDetectorFactory returned $detector2\n";
    $status = 1;
}

print "\nTEST: shutting down the replication manager.\n" if ($verbose);
$controller = $RMC->SpawnWaitKill (300);
if ($controller != 0) {
    print STDERR "TEST ERROR: replication manager controller returned $controller\n";
    $status = 1;
}

print "\nTEST: wait for ReplicationManager.\n" if ($verbose);
#$RM->Kill ();
$repmgr = $RM->WaitKill (30);
if ($repmgr != 0) {
    print STDERR "TEST ERROR: ReplicationManager returned $repmgr\n";
    $status = 1;
}

print "\nTEST: wait for FaultNotifier.\n" if ($verbose);
$notifier = $NOT->WaitKill (30);
if ($notifier != 0) {
    print STDERR "TEST ERROR: FaultNotifier returned $notifier\n";
    $status = 1;
}

if ($status == 0 && 0) {
  print "\nTEST: releasing scratch files.\n" if ($verbose);
  unlink $rm_ior;
  unlink $detector1_ior;
  unlink $detector2_ior;
  unlink $notifier_ior;
  unlink $factory1_ior;
  unlink $factory2_ior;
  unlink $factory3_ior;
  unlink $replica1_ior;
  unlink $replica2_ior;
  unlink $replica3_ior;
  unlink $replica4_ior;
  unlink $replica5_ior;
  unlink $replica6_ior;
  unlink $replica1_iogr;
  unlink $replica2_iogr;

  unlink $client_data;
}


exit $status;
