eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# Purpose:
#       To test the FactoryRegistry
#
# Process being tested:
#       FT_Registry
#           implements PortableGroup::FactoryRegistry interface.
# Processes used in test:
#       FT_Replica * 2
#           implements GenericFactory interface to create TestReplicas
#           TestReplica implements TestReplica interface.
#           TestReplica implements PullMonitorable interface.
#       FT_Client
#           client for TestReplica interface.
#           client for PullMonitorable.
#       StubConfiguratonManager
#           Orchestrates the test.
#
# Test Scenario (***Test: marks behavior being tested):
#   Phase 1:
#     Start FactoryRegistry (FR)
#       FR writes IOR to file.
#   Phase 2:
#     Wait for IOR: FR.
#     Start two FT_ReplicaFactories giving them FR IOR.
#         Specifying same type_id, different location.
#       ***Test: FT_ReplicaFactories register with FactoryRegistry.
#       RFs write IORs to files (used for synchronization purposes only).
#   Phase 3:
#     Wait for IORS: RF1, RF2
#     Start StubConfiguratonManager given type_id and IORs: FR
#       ***Test: StubConfiguratonManager asks FR for factory-by-type.
#           Receives list of factories(RF1 RF2).
#       StubConfiguratonManager uses create_object for each factory to
#           create TestReplicas (TR1 and TR2).
#       StubConfiguratonManager writes TestReplica IORs (TR1 and TR2) to files
#       StubConfigurationManger exits.  It's job is done!
#   Phase 4:
#     Wait for IORS: TR1 and TR2
#     Start FT_Client giving it TR1 and TR2. [1]
#       FT_Client interacts with TR1.
#       FT_Client asks TR1 to fault.  It does so
#       FR1 is idle.  It honors quit-on-idle option.  As it exits, it unregisters from FR.
#       FT_Client notices fault and switches to TR2. [1]
#       FT_Client interacts with TR2.
#       FT_Client asks TR2 to shut down.
#       FT_Client shuts down.
#       FR2 notices TR2 is gone and honors it's quit-on-idle option.  It unregisters from FR.
#				All factories have unregistered from FR.  FR honors its quit-on-idle option.
#   Phase 7: housekeeping
#       Wait for all processes to terminate.
#       Check termination status.
#       Delete temp files.
#
# [1] Client mediated fault tolerance.  These points will
#     change when IOGR support is available.
use lib '../../../../bin';
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

my($type_id) = "test_replica";
my($location1) = "hither";
my($location2) = "yon";

#define temp files
my($registry_ior) = PerlACE::LocalFile ("registry.ior");
my($factory1_ior) = PerlACE::LocalFile ("factory1.ior");
my($factory2_ior) = PerlACE::LocalFile ("factory2.ior");
my($replica1_ior) = PerlACE::LocalFile ("${location1}_$type_id.ior");
my($replica2_ior) = PerlACE::LocalFile ("${location2}_$type_id.ior");
my($client_data) = PerlACE::LocalFile ("persistent.dat");

#discard junk from previous tests
unlink $registry_ior;
unlink $factory1_ior;
unlink $factory2_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $client_data;

my($status) = 0;

my($REG) = new PerlACE::Process (".$build_directory/ft_registry", "-o $registry_ior -q");
my($FAC1) = new PerlACE::Process (".$build_directory/ft_replica", "-o $factory1_ior -f $registry_ior -l $location1 -i $type_id -q");
my($FAC2) = new PerlACE::Process (".$build_directory/ft_replica", "-o $factory2_ior -f $registry_ior -l $location2 -i $type_id -q");
my($CFG) = new PerlACE::Process (".$build_directory/ft_config", "-t $type_id -f $registry_ior ");

my($CL);
if (simulated) {
  $CL = new PerlACE::Process (".$build_directory/ft_client", "-f $replica1_ior,$replica2_ior -c testscript");
}else{
  #todo figure out how to get iogr
  $CL = new PerlACE::Process (".$build_directory/ft_client", "-f $replica1_iogr -c testscript");
}

#########
# Phase 1

print "\nTEST: starting registry " . $REG->CommandLine . "\n" if ($verbose);
$REG->Spawn ();

print "TEST: waiting for registry's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($registry_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$registry_ior>\n";
    $REG->Kill (); $REG->TimedWait (1);
    exit 1;
}

#########
# Phase 2

print "\nTEST: starting factory 1 " . $FAC1->CommandLine . "\n" if ($verbose);
$FAC1->Spawn ();

print "TEST: waiting for factory 1's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($factory1_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$factory1_ior>\n";
    $REG->Kill (); $REG->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    exit 1;
}

print "\nTEST: starting factory 2 " . $FAC2->CommandLine . "\n" if ($verbose);
$FAC2->Spawn ();

print "TEST: waiting for factory 2's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($factory2_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$factory2_ior>\n";
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $REG->Kill (); $REG->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    exit 1;
}


#########
# Phase 3

print "\nTEST: starting configuration manager " . $CFG->CommandLine . "\n" if ($verbose);
$CFG->Spawn ();

print "TEST: waiting for Replica 1 IOR file from configuration manager\n" if ($verbose);
if (PerlACE::waitforfile_timed ($replica1_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica1_ior>\n";
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $REG->Kill (); $REG->TimedWait (1);
    $CFG->Kill (); $CFG->TimedWait(1);
    exit 1;
}
if (PerlACE::waitforfile_timed ($replica2_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica2_ior> \n";
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $REG->Kill (); $REG->TimedWait (1);
    $CFG->Kill (); $CFG->TimedWait(1);
    exit 1;
}

#########
# Phase 4

print "\nTEST: starting client " . $CL->CommandLine . "\n" if ($verbose);
$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

#########
# Phase 4

print "\nTEST: wait for factory 1.\n" if ($verbose);
$factory1 = $FAC1->WaitKill (5);
if ($factory1 != 0) {
    print STDERR "ERROR: replica returned $factory 1\n";
    $status = 1;
}

print "\nTEST: wait for factory 2.\n" if ($verbose);
$factory2 = $FAC2->WaitKill (5);
if ($factory2 != 0) {
    print STDERR "ERROR: factory 2 returned $factory2\n";
    $status = 1;
}

print "\nTEST: wait for configuration manager to leave.\n" if ($verbose);
$config = $CFG->WaitKill (5);
if ($config != 0) {
    print STDERR "ERROR: configuration manager returned $config\n";
    $status = 1;
}

print "\nTEST: wait for FactoryRegistry to leave.\n" if ($verbose);
$registry = $REG->WaitKill (5);
if ($registry != 0) {
    print STDERR "ERROR: FactoryRegistry returned $registry\n";
    $status = 1;
}


print "\nTEST: releasing scratch files.\n" if ($verbose);
unlink $registry_ior;
unlink $factory1_ior;
unlink $factory2_ior;
unlink $replica1_ior;
unlink $replica2_ior;
unlink $client_data;

exit $status;
