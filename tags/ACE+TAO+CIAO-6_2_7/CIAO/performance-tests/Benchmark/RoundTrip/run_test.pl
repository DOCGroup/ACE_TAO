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

$status = 0;

$daemon_base = "daemon.ior";
$svr_base = "server.ior";
$home_base = "test.ior";

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$daemon_ior = $tg->LocalFile ($daemon_base);
$svr_ior = $tg->LocalFile ($svr_base);
$home_ior = $tg->LocalFile ($home_base);

$CIAO_ROOT=$ENV{'CIAO_ROOT'};

$tg->DeleteFile ($daemon_base);
$tg->DeleteFile ($svr_base);
$tg->DeleteFile ($home_base);

# CIAO Daemon command line arguments
$daemon_args = "-c ../RoundTrip/svc.conf -o $daemon_ior -i CIAO_Installation_Data.ini -n $CIAO_ROOT/tools/ComponentServer/ComponentServer";

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
$cl_args = "-ORBSvcConf ../RoundTrip/svc.conf -i 300000";

# CIAO daemon process definition
$DS = $tg->CreateProcess ("$CIAO_ROOT/tools/Daemon/CIAO_Daemon",
                            "$daemon_args");

# Client process definition
$CL = $tg->CreateProcess ("../RoundTrip/client",
                            $cl_args);

## Starting up the CIAO daemon
$DS->Spawn ();
if ($tg->WaitForFileTimed($daemon_base,
                          $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: Could not find daemon ior file <$daemon_ior>\n";
    $DS->Kill ();
    exit 1;
}

## Starting up a ComponentServer running the Roundtrip home.
$DC = $tg->CreateProcess ("$controller",
                          "$common_args $start_args");

$DC->SpawnWaitKill ($tg->ProcessStartWaitInterval ());
if ($tg->WaitForFileTimed($home_base,
                          $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: Could not find home ior file <$home_ior>\n";
    $DS->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill ($tg->ProcessStartWaitInterval ());

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

## Terminating the ComponentServer running.
$DC = $tg->CreateProcess ("$controller",
                            "$common_args $end_args");

$ctrl = $DC->SpawnWaitKill ($tg->ProcessStartWaitInterval ());
if ($ctrl != 0) {
    print STDERR "ERROR: Fail to end component server\n";
    $DC->Kill ();
    exit 1;
}

## Terminating the ComponentServer running the RateGen home.
$DC = $tg->CreateProcess ("$controller",
                          "$common_args $shutdown_args");

$ctrl = $DC->SpawnWaitKill ($tg->ProcessStartWaitInterval ());
if ($ctrl != 0) {
    print STDERR "ERROR: Fail to shutdown CIAODaemon\n";
    $DS->Kill ();
    exit 1;
}

$ctrl = $DS->WaitKill ($tg->ProcessStopWaitInterval ());
if ($ctrl != 0) {
    print STDERR "ERROR: CIAODaemon didn't shutdown gracefully $ctrl\n";
    $DS->Kill ();
    exit 1;
}

$tg->DeleteFile ($daemon_base);
$tg->DeleteFile ($svr_base);
$tg->DeleteFile ($home_base);

exit $status;
