eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

#
# @@ Run this script from ../descriptors/ instead.
#

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$status = 0;
$daemon_base = "daemon.ior";
$am_base = "am.ior";
$daemon_ior = $tg->LocalFile ("daemon.ior");
$am_ior = $tg->LocalFile ("am.ior");

$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$tg->DeleteFile ($daemon_base);
$tg->DeleteFile ($am_base);

# CIAO Daemon command line arguments
$daemon_args = "-c $CIAO_ROOT/performance-tests/Benchmark/RoundTrip/svc.conf -ORBEndpoint iiop://localhost:10000 -o $daemon_ior -i CIAO_Installation_Data.ini -n $CIAO_ROOT/tools/ComponentServer/ComponentServer";

$assembly_manager_args = "-o $am_ior -c test.dat";

$ad_args = " -k file://$am_ior -a Test-IOR.cad";

# CIAO daemon process definition
$DS = $tg->CreateProcess ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args");

## Starting up the CIAO daemon
$DS->Spawn ();
if ($tg->WaitForFileTimed($daemon_base,
                          $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
    $DS->Kill ();
    exit 1;
}


$AM = $tg->CreateProcess("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Manager",
                           $assembly_manager_args);
$AM->Spawn ();
if ($tg->WaitForFileTimed($am_base,
                          $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: Could not find assembly ior file <$am_ior>\n";
    $AM->Kill ();
    exit 1;
}

$AD = $tg->CreateProcess("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Deployer",
                           $ad_args);
$AD->Spawn ();

sleep (5);

#Start the client to send the trigger message
$CL = $tg->CreateProcess ("../RoundTripClient/client", "");
$CL->SpawnWaitKill($tg->ProcessStartWaitInterval ());

$AM->Kill ();
$AD->Kill ();
$DS->Kill ();

$tg->DeleteFile ($daemon_base);
$tg->DeleteFile ($am_base);

exit $status;
