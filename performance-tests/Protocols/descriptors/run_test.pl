# -*- perl -*-
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$ACE_ROOT = $ENV {'ACE_ROOT'};
$CIAO_ROOT = $ENV {'CIAO_ROOT'};

$ORBdebuglevel = 0;
$protocols = "-ORBEndpoint \'iiop://\'";
#$protocols = "-ORBEndpoint \'iiop://;diop://:5678;sciop://\'";

$daemon_1 = new PerlACE::Process ("${CIAO_ROOT}/tools/Daemon/CIAO_Daemon",
                                  "-ORBEndpoint iiop://localhost:20000 " .
                                  "-n \"${CIAO_ROOT}/tools/RTComponentServer/RTComponentServer -ORBdebuglevel $ORBdebuglevel\"");

$daemon_2 = new PerlACE::Process ("${CIAO_ROOT}/tools/Daemon/CIAO_Daemon",
                                  "-ORBEndpoint iiop://localhost:12000 " .
                                  "-n \"${CIAO_ROOT}/tools/RTComponentServer/RTComponentServer -ORBdebuglevel $ORBdebuglevel $protocols\"");

$assembly_manager_args = "-o assembly_manager.ior -c test.dat";
$assembly_manager = new PerlACE::Process ("${CIAO_ROOT}/tools/Assembly_Deployer/Assembly_Manager", "$assembly_manager_args");

$assembly_deployer_args = "-k file://assembly_manager.ior -a remote.cad -o assembly";
$assembly_deployer = new PerlACE::Process ("${CIAO_ROOT}/tools/Assembly_Deployer/Assembly_Deployer", "$assembly_deployer_args");

$controller = new PerlACE::Process ("../Controller/Controller", "-x 1 -z 1");

# Remove all ior files
unlink "assembly_manager.ior";
unlink "daemon.ior";
unlink "receiver.ior";
unlink "sender.ior";
unlink "assembly";

# Start the daemons.
$daemon_1->Spawn ();
$daemon_2->Spawn ();

# Start the assembly manager
$assembly_manager->Spawn ();

# Wait till the assembly manager finishes writing the IOR
PerlACE::waitforfile ("assembly_manager.ior");

# Start the Assembly_Deployer
$assembly_deployer->Spawn ();

# Wait till the sender and receiver to finish writing their IORs
PerlACE::waitforfile ("sender.ior");
PerlACE::waitforfile ("receiver.ior");

# Now start the controller
$controller->Spawn ();

# Wait for controller to get done.
$controller->Wait ();
$controller->{RUNNING} = 0;

# Kill daemons.
$daemon_1->Kill ();
$daemon_2->Kill ();
$assembly_manager->Kill ();
$assembly_deployer->Kill ();

# Remove all ior files
unlink "assembly_manager.ior";
unlink "daemon.ior";
unlink "receiver.ior";
unlink "sender.ior";
unlink "assembly";

