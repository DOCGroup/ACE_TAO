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
# Test Scenario (***Test: marks behavior being tested):
#
#   First the test starts a factory registry,
#   Then starts three factories.  Each factory exists at a separate location.
#   The locations are named shire, bree, and rivendell.  In a "real" system
#   these locations would be on separate computers.
#
#   The factory at the shire location knows how to create hobbits.
#   The factory at the bree location knows how to create hobbits, elves, and humans.
#   The factory at rivendell can create elves.
#   Hobbits, elves, and humans are actually TestReplica objects.  A creation parameter
#   included as part of the registration information specifies which species they are.
#
#   ***Test: The factories register themselves with the factory registry.  Registration
#   information includes the location, the type of object created, a object reference
#   to the factory and set of parameters to be passed to the factory's create_object
#   method.
#
#   An object group creator is started and asked to create three object groups:
#   a group of hobbits, a group of elves; and another group of hobbits.
#
#   ***Test: the object group creator asks the registry for the set of factories
#   that can create the type of object needed, then uses the create_object method
#   for each factory to create the actual object.
#
#   [temporary until IOGR's are working:  The object group creator writes the
#   IOR's of the create objects to files -- using a file naming convention to
#   distinguish members of the group.  It will be enhanced to assemble these
#   IORs into an IOGR and either write the IOGR to a file or register it with
#   the Naming Service.]
#
#   The object group creator is also told to unregister all factories that create humans.
#   ***Test: It does so using the unregister_factory_by_type method.
#
#   Three clients are started, one at a time.  Each client is given a reference
#   to an object group
#
#   [temporary until IOGRs and transparent reinvocaton work:  each client is
#   given references to the members of the group and manages its own recovery
#   (client mediated fault tolerance)]
#
#   Each client sends a few requests to first member of the object group.  Eventually
#   this member fails and the requests are then rerouted to the second (and last)
#   member of the group.
#
#   When a clients job is done, it exits and asks the remaining group member to
#   exit, too.
#
#   The factories are run with the quit-on-idle option so when the last object
#   created at that location goes away, the factory exits.
#
#   ***Test: As it exits the factory unregisters itself with the factory registry.
#   ***Test: A command line option determines whether it uses a single
#   unregister_factory_by_location call, or separate unregister_factory calles for
#   each type of object created.  In this test, the shire location uses unregister_factory,
#   and bree and rivendell use unregister_factory_by_location.
#
#   The factory registry is also run with the quit-on-idle option so when the last
#   factory unregisters itself, the factory registry shuts down to end the test.

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

my($species1) = "hobbit";
my($species2) = "elf";
my($species3) = "human";
my($location1) = "shire";
my($location2) = "bree";
my($location3) = "rivendell";
my($location4) = "rohan";

#define temp files
my($registry_ior) = PerlACE::LocalFile ("registry.ior");
my($factory1_ior) = PerlACE::LocalFile ("factory1.ior");
my($factory2_ior) = PerlACE::LocalFile ("factory2.ior");
my($factory3_ior) = PerlACE::LocalFile ("factory3.ior");
my($replica1_ior) = PerlACE::LocalFile ("${species1}_${location1}_0.ior");
my($replica2_ior) = PerlACE::LocalFile ("${species1}_${location2}_0.ior");
my($replica3_ior) = PerlACE::LocalFile ("${species2}_${location2}_1.ior");
my($replica4_ior) = PerlACE::LocalFile ("${species2}_${location3}_0.ior");
my($replica5_ior) = PerlACE::LocalFile ("${species1}_${location1}_1.ior");
my($replica6_ior) = PerlACE::LocalFile ("${species1}_${location2}_2.ior");

my($replica1_iogr) = PerlACE::LocalFile ("${species1}_0.iogr");
my($replica2_iogr) = PerlACE::LocalFile ("${species2}_1.iogr");
my($replica3_iogr) = PerlACE::LocalFile ("${species1}_2.iogr");

my($client_data) = PerlACE::LocalFile ("persistent.dat");

#discard junk from previous tests
unlink $registry_ior;
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

my($REG) = new PerlACE::Process (".$build_directory/ft_registry", "-o $registry_ior -q");
my($FAC1) = new PerlACE::Process (".$build_directory/ft_replica", "-o $factory1_ior -f file://$registry_ior -l $location1 -i $species1 -q");
my($FAC2) = new PerlACE::Process (".$build_directory/ft_replica", "-o $factory2_ior -f file://$registry_ior -l $location2 -i $species1 -i $species2 -i $species3 -q -u");
my($FAC3) = new PerlACE::Process (".$build_directory/ft_replica", "-o $factory3_ior -f file://$registry_ior -l $location3 -i $species2 -q -u");
#intermediate test: resolve factory IOR as file: url
#my($CTR) = new PerlACE::Process (".$build_directory/ft_create", "-f file://$registry_ior -t $species1 -t $species2 -t $species1 -k $species3");
#ultimate test (for now): use ORBInitRef/RIR to find ReplicationManager (or factory registry DBA replication manager):
my($CTR) = new PerlACE::Process (".$build_directory/ft_create", "-ORBInitRef ReplicationManager=file://$registry_ior -t $species1 -t $species2 -t $species1 -k $species3");

my($CL1);
my($CL2);
my($CL3);
if (simulated) {
  $CL1 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica1_ior -f file://$replica2_ior -c testscript");
  $CL2 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica3_ior -f file://$replica4_ior -c testscript");
  $CL3 = new PerlACE::Process (".$build_directory/ft_client", "-f file://$replica5_ior -f file://$replica6_ior -c testscript");
}else{
  #todo figure out how to get iogr
  $CL1 = new PerlACE::Process (".$build_directory/ft_client", "-f $replica1_iogr -c testscript");
  $CL2 = new PerlACE::Process (".$build_directory/ft_client", "-f $replica2_iogr -c testscript");
  $CL3 = new PerlACE::Process (".$build_directory/ft_client", "-f $replica3_iogr -c testscript");
}

#######################
# Start FactoryRegistry

print "\nTEST: starting registry " . $REG->CommandLine . "\n" if ($verbose);
$REG->Spawn ();

print "TEST: waiting for registry's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($registry_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$registry_ior>\n";
    $REG->Kill (); $REG->TimedWait (1);
    exit 1;
}

#################
# Start Factories

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

print "\nTEST: starting factory 3 " . $FAC3->CommandLine . "\n" if ($verbose);
$FAC3->Spawn ();

print "TEST: waiting for factory 3's IOR\n" if ($verbose);
if (PerlACE::waitforfile_timed ($factory3_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$factory3_ior>\n";
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $REG->Kill (); $REG->TimedWait (1);
    $FAC3->Kill (); $FAC3->TimedWait (1);
    exit 1;
}

######################
# Create object groups

print "\nTEST: starting object group creator " . $CTR->CommandLine . "\n" if ($verbose);
$CTR->Spawn ();

print "TEST: waiting for Replica IOR files from object group creator\n" if ($verbose);
if (PerlACE::waitforfile_timed ($replica1_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica1_ior>\n";
    $status = 1;
}
elsif (PerlACE::waitforfile_timed ($replica2_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica2_ior> \n";
    $status = 1;
}
elsif (PerlACE::waitforfile_timed ($replica3_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica3_ior> \n";
    $status = 1;
}
elsif (PerlACE::waitforfile_timed ($replica4_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica4_ior> \n";
    $status = 1;
}
elsif (PerlACE::waitforfile_timed ($replica5_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica5_ior> \n";
    $status = 1;
}
elsif (PerlACE::waitforfile_timed ($replica6_ior, 5) == -1){
    print STDERR "ERROR: cannot find file <$replica6_ior> \n";
    $status = 1;
}

if($status != 0){
    $FAC3->Kill (); $FAC3->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $REG->Kill (); $REG->TimedWait (1);
    $CTR->Kill (); $CTR->TimedWait(1);
    exit 1;
}

print "\nTEST: wait for object group creator.\n" if ($verbose);
$config = $CTR->WaitKill (5);
if ($config != 0) {
    print STDERR "ERROR: configuration manager returned $config\n";
    $FAC3->Kill (); $FAC3->TimedWait (1);
    $FAC2->Kill (); $FAC2->TimedWait (1);
    $FAC1->Kill (); $FAC1->TimedWait (1);
    $REG->Kill (); $REG->TimedWait (1);
    exit 1;
}


#############
# Run clients

print "\nTEST: starting client " . $CL1->CommandLine . "\n" if ($verbose);
$client = $CL1->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print "\nTEST: starting client again " . $CL2->CommandLine . "\n" if ($verbose);
$client2 = $CL2->SpawnWaitKill (60);

if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
}

print "\nTEST: starting client, one more time with feeling " . $CL3->CommandLine . "\n" if ($verbose);
$client3 = $CL3->SpawnWaitKill (60);

if ($client3 != 0) {
    print STDERR "ERROR: client returned $client3\n";
    $status = 1;
}

######################
# Clean house and exit

print "\nTEST: wait for factory 1.\n" if ($verbose);
$factory1 = $FAC1->WaitKill (30);
if ($factory1 != 0) {
    print STDERR "ERROR: replica returned $factory 1\n";
    $status = 1;
}

print "\nTEST: wait for factory 2.\n" if ($verbose);
$factory2 = $FAC2->WaitKill (30);
if ($factory2 != 0) {
    print STDERR "ERROR: factory 2 returned $factory2\n";
    $status = 1;
}

print "\nTEST: wait for factory 3.\n" if ($verbose);
$factory3 = $FAC3->WaitKill (30);
if ($factory3 != 0) {
    print STDERR "ERROR: factory 3 returned $factory3\n";
    $status = 1;
}

print "\nTEST: wait for FactoryRegistry.\n" if ($verbose);
$registry = $REG->WaitKill (30);
if ($registry != 0) {
    print STDERR "ERROR: FactoryRegistry returned $registry\n";
    $status = 1;
}

print "\nTEST: releasing scratch files.\n" if ($verbose);
unlink $registry_ior;
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


exit $status;
