eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$DANCE_ROOT = "$ENV{'DANCE_ROOT'}";

$daemons_running = 0;
$em_running = 0;
$daemons = 2;
@ports = ( 10001, 20001 );
@iorbases = ( "NodeApp1.ior", "NodeApp2.ior" );
@iorfiles = 0;
$status = 0;
$dat_file = "NodeManagerMap.dat";
$controller_exec = "$CIAO_ROOT/examples/Display/RateGen/controller";

$E = 0;
$EM = 0;

$tg = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";

$ior_rategenbase = "RateGen.ior";
$ior_rategenfile = 0;
$ior_gpsbase = "GPS.ior";
$ior_displaybase = "NavDisplay.ior";
$ior_displayfile = 0;
$ior_embase = "EM.ior";
$ior_emfile = 0;

sub init_ior_files {
    $ior_emfile = $tg->LocalFile ($ior_embase);
    $ior_rategenfile = $tg->LocalFile ($ior_rategenbase);
    $tg->LocalFile ($ior_gpsbase);
    $ior_displayfile = $tg->LocalFile ($ior_displaybase);
    for ($i = 0; $i < $daemons; ++$i) {
        $iorfiles[$i] = $tg->LocalFile ($iorbases[$i]);
    }
    delete_ior_files ();
}


# Delete if there are any .ior files.
sub delete_ior_files {
    for ($i = 0; $i < $daemons; ++$i) {
        $tg->DeleteFile ($iorbases[$i]);
    }
    $tg->DeleteFile ($ior_embase);
    $tg->DeleteFile ($ior_rategenbase);
    $tg->DeleteFile ($ior_gpsbase);
    $tg->DeleteFile ($ior_displaybase);
}

sub kill_node_daemons {
    for ($i = 0; $i < $daemons; ++$i) {
        $Daemons[$i]->Kill (); $Daemons[$i]->TimedWait (1);
    }
}

sub kill_open_processes {
    if ($daemons_running == 1) {
        kill_node_daemons ();
    }

    if ($em_running == 1) {
        $EM->Kill ();
        $EM->TimedWait (1);
    }
}

sub run_node_daemons {
    for ($i = 0; $i < $daemons; ++$i) {
        $iorfile = $iorfiles[$i];
        $port = $ports[$i];

        $iiop = "iiop://localhost:$port";
        $node_app = "$CIAO_ROOT/bin/ciao_componentserver";

        $d_cmd = "$DANCE_ROOT/bin/dance_node_manager";
        $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile -t 30";

        print "Run dance_node_manager with $d_param\n";

        $Daemons[$i] = $tg->CreateProcess ($d_cmd, $d_param);
        $Daemons[$i]->Spawn ();

        if ($tg->WaitForFileTimed($iorbases[$i],
                                     $tg->ProcessStartWaitInterval ()) == -1) {
            print STDERR
                "ERROR: The ior file of node daemon $i could not be found\n";
            for (; $i > 0; --$i) {
                $Daemons[$i]->Kill (); $Daemons[$i]->TimedWait (1);
            }
            return -1;
        }
    }
    $daemons_running = 1;
    return 0;
}

delete_ior_files ();

# Invoke node daemons.
print "Invoking node daemons\n";
$status = run_node_daemons ();

if ($status != 0) {
    print STDERR "ERROR: Unable to execute the node daemons\n";
    exit 1;
}

# Invoke execution manager.
print "Invoking execution manager\n";
$EM = $tg->CreateProcess ("$CIAO_ROOT/bin/dance_execution_manager",
                            "-o $ior_emfile -i $dat_file");
$EM->Spawn ();

if ($tg->WaitForFileTimed($ior_embase,
                              $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR
        "ERROR: The ior file of execution manager could not be found\n";
    kill_open_processes ();
    exit 1;
}

$em_running = 1;

# Invoke executor - start the application -.
print "Invoking executor - start the application -\n";
$E = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_plan_launcher",
                        "-p flattened_deploymentplan.cdp -k file://$ior_emfile -o DAM.ior");

$E->SpawnWaitKill ($tg->ProcessStopWaitInterval ());

if ($tg->WaitForFileTimed($ior_gpsbase,
                              $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: The ior file of GPS could not be found\n";
    kill_open_processes ();
    exit 1;
}

if ($tg->WaitForFileTimed($ior_rategenbase,
                              $tg->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: The ior file of RateGen could not be found\n";
    kill_open_processes ();
    exit 1;
}

print "Invoking the controller\n";
$controller = $tg->CreateProcess ("$controller_exec", "-k file://$ior_rategenfile");
$result = $controller->SpawnWaitKill ($tg->ProcessStopWaitInterval ());
if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

# put some delay here.
sleep (20);

# invoking the controller again to stop the rategen
print "Invoking the controller to stop RateGen\n";
$controller = $tg->CreateProcess ("$controller_exec", "-k file://$ior_rategenfile -f");
$result = $controller->SpawnWaitKill ($tg->ProcessStopWaitInterval ());
if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

# delay a bit more
sleep (2);

# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
$E = $tg->CreateProcess ("$DANCE_ROOT/bin/dance_plan_launcher",
                        "-k file://$ior_emfile -i file://DAM.ior");
$E->SpawnWaitKill ($tg->ProcessStopWaitInterval ());

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
