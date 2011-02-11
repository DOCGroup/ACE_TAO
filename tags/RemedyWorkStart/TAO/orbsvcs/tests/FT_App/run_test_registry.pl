eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# Purpose:
#   To test the FactoryRegistry either as a stand-alone application or as part
#   of the replication manager
#
# Command line options:
#   --debug_build  use exes from this directory
#        if not specified use exes from ./release
#   -r use ReplicationManager rather than stand-alone factory
#   --no_simulate
#        use real IOGR-based recovery.
#   -v  display test progress messages (repeating option increases verbosity
#
# Process being tested:
#       FT_Registry
#           implements PortableGroup::FactoryRegistry interface.
#  or
#       FT_ReplicationManager (if -r option is used)
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
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

########################
#command line options
#set defaults:
my($status) = 0;
my($verbose) = 0;         # 1: report perl actions before executing them
my($debug_builds) = 0;    # 0: use exes from Release directories
my($simulated) = 1;       # 1: use "client simulated" fault tolerance

foreach $i (@ARGV) {
    if ($i eq "--debug_build"){
        $debug_builds = 1;
    }
    elsif ($i eq "-r"){ # use RegistrationManager
        $use_rm = 1
    }
    elsif ($i eq "--no_simulate"){  # reverse this once we have FT ORB support
        $simulated = 0;
    }
    elsif ($i eq "-v"){
        $verbose += 1;
    }
    else{
        print "unknown option $i.  Expecting: --debug_build, -r --no_simulate, -v\n";
        exit(-1);
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

my $rp_manager = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $rp_manager_controller = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $ft_replica1 = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $ft_replica2 = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $ft_replica3 = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $ft_creator = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";
my $client1 = PerlACE::TestTarget::create_target (7) || die "Create target 7 failed\n";
my $client2 = PerlACE::TestTarget::create_target (8) || die "Create target 8 failed\n";
my $client3 = PerlACE::TestTarget::create_target (9) || die "Create target 9 failed\n";
my $ft_registry = PerlACE::TestTarget::create_target (10) || die "Create target 10 failed\n";

my($role1) = "hobbit";
my($role2) = "elf";
my($role3) = "human";

my($location1) = "shire";
my($location2) = "bree";
my($location3) = "rivendell";
my($location4) = "rohan";

my($rm_ior) = "rm.ior";
my($registry_ior) = "registry.ior";

my($factory1_ior) = "factory1.ior";
my($factory2_ior) = "factory2.ior";
my($factory3_ior) = "factory3.ior";

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

# Files for ft_registry
my $ft_registry_registry_ior = $ft_registry->LocalFile ($registry_ior);
$ft_registry->DeleteFile ($registry_ior);

# Files for rp_manager_controller

my ($rp_manager_controller_registry_ior);
if(!$use_rm){
    $rp_manager_controller_registry_ior = $rp_manager_controller->LocalFile ($registry_ior);
    $rp_manager_controller->DeleteFile ($registry_ior);
}

# Files for ft_replica1
my $ft_replica1_factory1_ior = $ft_replica1->LocalFile ($factory1_ior);
my $ft_replica1_client_data = $ft_replica1->LocalFile ($client_data);

$ft_replica1->DeleteFile ($factory1_ior);
$ft_replica1->DeleteFile ($client_data);

my($ft_replica1_registry_ior);

if(!$use_rm){
    $ft_replica1_registry_ior = $ft_replica1->LocalFile ($registry_ior);
    $ft_replica1->DeleteFile ($registry_ior);
}

# Files for ft_replica2
my $ft_replica2_factory2_ior = $ft_replica2->LocalFile ($factory2_ior);
my $ft_replica2_client_data = $ft_replica2->LocalFile ($client_data);

$ft_replica2->DeleteFile ($factory2_ior);
$ft_replica2->DeleteFile ($client_data);

my($ft_replica2_registry_ior);
if(!$use_rm){
    $ft_replica2_registry_ior = $ft_replica2->LocalFile ($registry_ior);
    $ft_replica2->DeleteFile ($registry_ior);
}


# Files for ft_replica3
my $ft_replica3_factory3_ior = $ft_replica3->LocalFile ($factory3_ior);
my $ft_replica3_client_data = $ft_replica3->LocalFile ($client_data);

$ft_replica3->DeleteFile ($factory3_ior);
$ft_replica3->DeleteFile ($client_data);

my($ft_replica3_registry_ior);
if(!$use_rm){
    $ft_replica3_registry_ior = $ft_replica3->LocalFile ($registry_ior);
    $ft_replica3->DeleteFile ($registry_ior);
}

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

my($ft_creator_registry_ior);
if(!$use_rm){
    $ft_creator_registry_ior = $ft_creator->LocalFile ($registry_ior);
    $ft_creator->DeleteFile ($registry_ior);
}

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
my($RMC);
if ($use_rm){
    $RMC = $rp_manager_controller->CreateProcess (".$build_directory/replmgr_controller" ,
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-x");
}
else{
    $RMC = $rp_manager_controller->CreateProcess (".$build_directory/replmgr_controller" ,
                                 "-f file://$rp_manager_controller_registry_ior " .
                                 "-x");
}

$REG = $ft_registry->CreateProcess (".$build_directory/ft_registry" ,
                                 "-o $ft_registry_registry_ior " .
                                 "-q");


my($REP1);
my($REP2);
my($REP3);

if ($use_rm){
    $REP1 = $ft_replica1->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica1_factory1_ior ".
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-l $location1 -i $role1 -q -p $ft_replica1_client_data");

    $REP2 = $ft_replica2->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica2_factory2_ior ".
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-l $location2 ".
                                 "-i $role1 -i $role2 -i $role3 -q -u " .
                                 "-p $ft_replica2_client_data");

    $REP3 = $ft_replica3->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica3_factory3_ior ".
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-l $location3 -i $role2 -q -u -p $ft_replica1_client_data");

    $CTR = $ft_creator->CreateProcess (".$build_directory/ft_create" ,
                                 "-ORBInitRef ReplicationManager=corbaloc::$hostname:$port/ReplicationManager " .
                                 "-n -r $role1 -r $role2 -r $role1 -u $role3 -i");

}
else{
    $REP1 = $ft_replica1->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica1_factory1_ior ".
                                 "-f file://$ft_replica1_registry_ior " .
                                 "-l $location1 -i $role1 -q -p $ft_replica1_client_data");

    $REP2 = $ft_replica2->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica2_factory2_ior ".
                                 "-f file://$ft_replica2_registry_ior " .
                                 "-l $location2 ".
                                 "-i $role1 -i $role2 -i $role3 -q -u " .
                                 "-p $ft_replica2_client_data");

    $REP3 = $ft_replica3->CreateProcess (".$build_directory/ft_replica" ,
                                 "-o $ft_replica3_factory3_ior ".
                                 "-f file://$ft_replica3_registry_ior " .
                                 "-l $location3 -i $role2 -q -u -p $ft_replica1_client_data");

    $CTR = $ft_creator->CreateProcess (".$build_directory/ft_create" ,
                                 "-f file://$ft_creator_registry_ior " .
                                 "-n -r $role1 -r $role2 -r $role1 -u $role3 -i");
}

my($CL1) = $client1->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client1_replica1_ior " .
                                 "-f file://$client1_replica2_ior " .
                                 "-c testscript");

my($CL2) = $client2->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client2_replica3_ior " .
                                 "-f file://$client2_replica4_ior " .
                                 "-c testscript");

my($CL3) = $client3->CreateProcess (".$build_directory/ft_client" ,
                                 "-f file://$client3_replica5_ior " .
                                 "-f file://$client3_replica6_ior " .
                                 "-c testscript");


if ($simulated) {
    print "\nTEST: Preparing Client Mediated Fault Tolerance test.\n" if ($verbose);
}else{
    print "\nTEST: Preparing IOGR based test.\n" if ($verbose);
    $CL1->Arguments ("-f file://$client1_replica1_iogr -c testscript");
    $CL2->Arguments ("-f file://$client2_replica2_iogr -c testscript");
    $CL3->Arguments ("-f file://$client3_replica3_iogr -c testscript");
}

#######################
# Start FactoryRegistry
if ($use_rm){
    print "\nTEST: starting ReplicationManager " . $RM->CommandLine . "\n" if ($verbose);
    $RM->Spawn ();
    add_to_kills ($RM);

    print "TEST: waiting for registry's IOR\n" if ($verbose);

    if ($rp_manager->WaitForFileTimed ($rm_ior,
                               $rp_manager->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$rp_manager_ior>\n";
        exit_and_kill (1);
    }
}
else{
    print "\nTEST: starting registry " . $REG->CommandLine . "\n" if ($verbose);
    $REG->Spawn ();

    add_to_kills ($REG);

    print "TEST: waiting for registry's IOR\n" if ($verbose);

    if ($ft_registry->WaitForFileTimed ($registry_ior,
                               $ft_registry->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$rp_manager_ior>\n";
        exit_and_kill (1);
    }

    if ($ft_registry->GetFile ($registry_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$ft_registry_registry_ior\n";
        exit_and_kill (1);
    }
    if ($ft_replica1->PutFile ($registry_ior) == -1) {
        print STDERR "ERROR: cannot set file <$ft_replica1_registry_ior>\n";
        exit_and_kill (1);
    }
    if ($ft_replica2->PutFile ($registry_ior) == -1) {
        print STDERR "ERROR: cannot set file <$ft_replica2_registry_ior>\n";
        exit_and_kill (1);
    }
    if ($ft_replica3->PutFile ($registry_ior) == -1) {
        print STDERR "ERROR: cannot set file <$ft_replica3_registry_ior>\n";
        exit_and_kill (1);
    }
    if ($ft_creator->PutFile ($registry_ior) == -1) {
        print STDERR "ERROR: cannot set file <$ft_creator_registry_ior>\n";
        exit_and_kill (1);
    }
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

print "\nTEST: starting object group creator " . $CTR->CommandLine . "\n" if ($verbose);
$CTR->Spawn ();

add_to_kills ($CTR);

print "TEST: waiting for Replica IOR files from object group creator\n" if ($verbose);

if ($ft_creator->WaitForFileTimed ($replica1_ior,
                               $ft_creator->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_creator_replica1_ior>\n";
    exit_and_kill (1);
}

if ($ft_creator->WaitForFileTimed ($replica2_ior,
                               $ft_creator->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_creator_replica2_ior>\n";
    exit_and_kill (1);
}

if ($ft_creator->WaitForFileTimed ($replica3_ior,
                               $ft_creator->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_creator_replica3_ior>\n";
    exit_and_kill (1);
}

if ($ft_creator->WaitForFileTimed ($replica4_ior,
                               $ft_creator->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_creator_replica4_ior>\n";
    exit_and_kill (1);
}

if ($ft_creator->WaitForFileTimed ($replica5_ior,
                               $ft_creator->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_creator_replica5_ior>\n";
    exit_and_kill (1);
}

if ($ft_creator->WaitForFileTimed ($replica6_ior,
                               $ft_creator->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ft_creator_replica6_ior>\n";
    exit_and_kill (1);
}

# Get/Put files:
# replica1_ior: client1
if ($ft_creator->GetFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_creator_replica1_ior\n";
    exit_and_kill (1);
}
if ($client1->PutFile ($replica1_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client1_replica1_ior>\n";
    exit_and_kill (1);
}
# replica2_ior: client1
if ($ft_creator->GetFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_creator_replica2_ior\n";
    exit_and_kill (1);
}
if ($client1->PutFile ($replica2_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client1_replica2_ior>\n";
    exit_and_kill (1);
}
# replica3_ior: client2
if ($ft_creator->GetFile ($replica3_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_creator_replica3_ior\n";
    exit_and_kill (1);
}
if ($client2->PutFile ($replica3_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client2_replica3_ior>\n";
    exit_and_kill (1);
}
# replica4_ior: client2
if ($ft_creator->GetFile ($replica4_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_creator_replica4_ior\n";
    exit_and_kill (1);
}
if ($client2->PutFile ($replica4_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client2_replica4_ior>\n";
    exit_and_kill (1);
}
# replica5_ior: client3
if ($ft_creator->GetFile ($replica5_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_creator_replica5_ior\n";
    exit_and_kill (1);
}
if ($client3->PutFile ($replica5_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client3_replica5_ior>\n";
    exit_and_kill (1);
}
# replica6_ior: client3
if ($ft_creator->GetFile ($replica6_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ft_creator_replica6_ior\n";
    exit_and_kill (1);
}
if ($client3->PutFile ($replica6_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client3_replica6_ior>\n";
    exit_and_kill (1);
}

print "\nTEST: wait for object group creator.\n" if ($verbose);
$status_creator = $CTR->WaitKill ($ft_creator->ProcessStopWaitInterval());

if ($status_creator != 0) {
    print STDERR "TEST ERROR: configuration manager returned $status_creator\n";
    exit_and_kill(1);
}
#############
# Run clients

print "\nTEST: starting client " . $CL1->CommandLine . "\n" if ($verbose);
$status_client = $CL1->SpawnWaitKill ($client1->ProcessStartWaitInterval() + 45);
if ($status_client != 0) {
    print STDERR "TEST ERROR: client returned $status_client\n";
    exit_and_kill(1);
}

print "\nTEST: starting client again " . $CL2->CommandLine . "\n" if ($verbose);

$status_client = $CL2->SpawnWaitKill ($client2->ProcessStartWaitInterval() + 45);
if ($status_client != 0) {
    print STDERR "TEST ERROR: client returned $status_client\n";
    exit_and_kill(1);
}

print "\nTEST: starting client, one more time with feeling " . $CL3->CommandLine . "\n" if ($verbose);
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
    exit_and_kill(1);
}
print "\nTEST: wait for factory 2.\n" if ($verbose);
$status_replica = $REP2->WaitKill ($ft_replica2->ProcessStopWaitInterval() + 15);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    exit_and_kill(1);
}

print "\nTEST: wait for factory 3.\n" if ($verbose);
$status_replica = $REP3->WaitKill ($ft_replica3->ProcessStopWaitInterval() + 15);
if ($status_replica != 0) {
    print STDERR "TEST ERROR: replica returned $status_replica\n";
    exit_and_kill(1);
}

if ($use_rm){
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
}
else{
    print "\nTEST: wait for FactoryRegistry.\n" if ($verbose);
    $status_reg = $REG->WaitKill ($ft_registry->ProcessStopWaitInterval() + 15);
    if ($status_reg != 0) {
        print STDERR "TEST ERROR: FactoryRegistry returned $status_reg\n";
        $status = 1;
    }
}

print "\nTEST: releasing scratch files.\n" if ($verbose);

# Files for rp_manager
$rp_manager->DeleteFile ($rm_ior);

# Files for ft_registry
$ft_registry->DeleteFile ($registry_ior);

if(!$use_rm){
    $rp_manager_controller->DeleteFile ($registry_ior);
}

# Files for ft_replica1
$ft_replica1->DeleteFile ($factory1_ior);
$ft_replica1->DeleteFile ($client_data);

if(!$use_rm){
    $ft_replica1->DeleteFile ($registry_ior);
}

# Files for ft_replica2
$ft_replica2->DeleteFile ($factory2_ior);
$ft_replica2->DeleteFile ($client_data);

if(!$use_rm){
    $ft_replica2->DeleteFile ($registry_ior);
}
# Files for ft_replica3
$ft_replica3->DeleteFile ($factory3_ior);
$ft_replica3->DeleteFile ($client_data);

if(!$use_rm){
    $ft_replica3->DeleteFile ($registry_ior);
}

# Files for ft_creator
if(!$use_rm){
    $ft_creator->DeleteFile ($registry_ior);
}

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
