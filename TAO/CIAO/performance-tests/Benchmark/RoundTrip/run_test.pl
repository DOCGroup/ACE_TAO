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
$svr_ior = PerlACE::LocalFile ("server.ior");
$home_ior = PerlACE::LocalFile ("test.ior");

$ACE_ROOT=$ENV{'ACE_ROOT'};
$CIAO_ROOT=$ENV{'CIAO_ROOT'};

unlink $daemon_ior;
unlink $svr_ior;
unlink $home_ior;

# CIAO Daemon command line arguments
$daemon_args = "-o $daemon_ior -i CIAO_Installation_Data.ini -n $CIAO_ROOT/tools/ComponentServer/ComponentServer";

# CIAO Daemon Controller location:
$controller = "$CIAO_ROOT/tools/Daemon/DaemonController";

# Daemon controller common command line arguments
$common_args = "-ORBInitRef CIAODaemon=file://$daemon_ior";

# Daemon controller start_home command
$start_args = "start_home -s RoundTrip.csd -m $home_ior -c $svr_ior";

# Daemon controller end_home command
$end_args = "end_home -c file://$svr_ior";

# Daemon controller shutdown command
$shutdown_args = "shutdown";

# Client program command line arguments
$cl_args = "";

# CIAO daemon process definition
$DS = new PerlACE::Process ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args");

# Client process definition
$CL = new PerlACE::Process ("../RoundTrip/client",
                            "$cl_args");

## Starting up the CIAO daemon
$DS->Spawn ();
if (PerlACE::waitforfile_timed ($daemon_ior, 15) == -1) {
    print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
    $DS->Kill ();
    exit 1;
}

## Starting up a ComponentServer running the Roundtrip home.
$DC = new PerlACE::Process ("$controller",
                            "$common_args $start_args");

$DC->SpawnWaitKill (60);
if (PerlACE::waitforfile_timed ($home_ior, 15) == -1) {
    print STDERR "ERROR: Could not find home ior file <$home_ior>\n";
    $DS->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

## Terminating the ComponentServer running the RateGen home.
$DC = new PerlACE::Process ("$controller",
                            "$common_args $end_args");

$ctrl = $DC->SpawnWaitKill (60);
if ($ctrl != 0) {
    print STDERR "ERROR: Fail to end component server\n";
    $DS->Kill ();
    exit 1;
}

## Terminating the ComponentServer running the RateGen home.
$DC = new PerlACE::Process ("$controller",
                            "$common_args $shutdown_args");

$ctrl = $DC->SpawnWaitKill (60);
if ($ctrl != 0) {
    print STDERR "ERROR: Fail to shutdown CIAODaemon\n";
    $DS->Kill ();
    exit 1;
}

$ctrl = $DS->WaitKill (60);
if ($ctrl != 0) {
    print STDERR "ERROR: CIAODaemon didn't shutdown gracefully $ctrl\n";
    $DS->Kill ();
    exit 1;
}

unlink $daemon_ior;
unlink $svr_ior;
unlink $home_ior;

exit $status;
