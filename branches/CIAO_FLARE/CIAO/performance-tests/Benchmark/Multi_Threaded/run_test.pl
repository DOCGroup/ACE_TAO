eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

#
# @@ Run this script from ../descriptors/ instead.
#

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$status = 0;
$daemon_ior = PerlACE::LocalFile ("daemon.ior");
$am_ior = PerlACE::LocalFile ("am.ior");

$ACE_ROOT=$ENV{'ACE_ROOT'};
$CIAO_ROOT=$ENV{'CIAO_ROOT'};

unlink $daemon_ior;
unlink $am_ior;

# CIAO Daemon command line arguments
$daemon_args = "-c $CIAO_ROOT/tools/ComponentServer/svc.conf -ORBEndpoint iiop://localhost:10000 -o $daemon_ior -i CIAO_Installation_Data.ini -n $CIAO_ROOT/tools/ComponentServer/ComponentServer";

# CIAO Daemon command line arguments
$daemon_args2 = "-c $CIAO_ROOT/tools/ComponentServer/svc.conf -ORBEndpoint iiop://localhost:12000 -o $daemon_ior -i CIAO_Installation_Data.ini -n $CIAO_ROOT/tools/ComponentServer/ComponentServer";

$assembly_manager_args = "-o $am_ior -c test.dat";

$ad_args = " -k file://$am_ior -a Multi_Threaded.cad";

# CIAO daemon process definition
$DS = new PerlACE::Process ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args");

## Starting up the CIAO daemon
$DS->Spawn ();
if (PerlACE::waitforfile_timed ($daemon_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
    $DS->Kill ();
    exit 1;
}

# CIAO daemon process definition
$DS2 = new PerlACE::Process ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args2");

## Starting up the CIAO daemon
$DS2->Spawn ();
if (PerlACE::waitforfile_timed ($daemon_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
    $DS->Kill ();
    exit 1;
}

$AM = new PerlACE::Process("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Manager",
                           $assembly_manager_args);
$AM->Spawn ();
if (PerlACE::waitforfile_timed ($am_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: Could not find assembly ior file <$am_ior>\n";
    $AM->Kill ();
    exit 1;
}

$AD = new PerlACE::Process("$CIAO_ROOT/tools/Assembly_Deployer/Assembly_Deployer",
                           $ad_args);
$AD->Spawn ();

sleep (5);

#Start the client to send the trigger message
$CL = new PerlACE::Process ("../Multi_Threaded/client", "");
$CL->SpawnWaitKill(60);

$AM->Kill ();
$AD->Kill ();
$DS->Kill ();
$CL->Kill ();
$DS2->Kill ();

unlink $daemon_ior;
unlink $am_ior;

exit $status;
