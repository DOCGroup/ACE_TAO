# -*- perl -*-
# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$CIAO_ROOT = $ENV {'CIAO_ROOT'};

$ORBdebuglevel = 0;
$protocols = "-ORBEndpoint \'iiop://\'";
$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$ior_ambase       = "assembly_manager.ior";
$ior_daemonbase   = "daemon.ior";
$ior_senderbase   = "sender.ior";
$ior_receiverbase = "receiver.ior";
$ior_assemblybase = "assembly.ior";

$ior_amfile       = $tg->LocalFile ($ior_ambase);
$tg->LocalFile ($ior_daemonbase);
$tg->LocalFile ($ior_senderbase);
$tg->LocalFile ($ior_receiverbase);
$tg->LocalFile ($ior_assemblybase);

$daemon_1 = $tg->CreateProcess ("${CIAO_ROOT}/tools/Daemon/CIAO_Daemon",
                                  "-ORBEndpoint iiop://localhost:20000 " .
                                  "-n \"${CIAO_ROOT}/tools/RTComponentServer/RTComponentServer -ORBdebuglevel $ORBdebuglevel\"");

$daemon_2 = $tg->CreateProcess ("${CIAO_ROOT}/tools/Daemon/CIAO_Daemon",
                                  "-ORBEndpoint iiop://localhost:12000 " .
                                  "-n \"${CIAO_ROOT}/tools/RTComponentServer/RTComponentServer -ORBdebuglevel $ORBdebuglevel $protocols\"");

$assembly_manager_args = "-o $ior_amfile -c test.dat";
$assembly_manager = $tg->CreateProcess ("${CIAO_ROOT}/tools/Assembly_Deployer/Assembly_Manager", "$assembly_manager_args");

$assembly_deployer_args = "-k file://$ior_amfile -a remote.cad -o assembly";
$assembly_deployer = $tg->CreateProcess ("${CIAO_ROOT}/tools/Assembly_Deployer/Assembly_Deployer", "$assembly_deployer_args");

$controller = $tg->CreateProcess ("../Controller/Controller", "-x 1 -z 1");

# Remove all ior files
$tg->DeleteFile ($ior_ambase);
$tg->DeleteFile ($ior_daemonbase);
$tg->DeleteFile ($ior_senderbase);
$tg->DeleteFile ($ior_receiverbase);
$tg->DeleteFile ($ior_assemblybase);

# Start the daemons.
$daemon_1->Spawn ();
$daemon_2->Spawn ();

# Start the assembly manager
$assembly_manager->Spawn ();

# Wait till the assembly manager finishes writing the IOR
$tg->WaitForFileTimed($ior_ambase, $tg->ProcessStartWaitInterval ());

# Start the Assembly_Deployer
$assembly_deployer->Spawn ();

# Wait till the sender and receiver to finish writing their IORs
$tg->WaitForFileTimed($ior_senderbase, $tg->ProcessStartWaitInterval ());
$tg->WaitForFileTimed($ior_receiverbase, $tg->ProcessStartWaitInterval ());

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
$tg->DeleteFile ($ior_ambase);
$tg->DeleteFile ($ior_daemonbase);
$tg->DeleteFile ($ior_senderbase);
$tg->DeleteFile ($ior_receiverbase);
$tg->DeleteFile ($ior_assemblybase);
