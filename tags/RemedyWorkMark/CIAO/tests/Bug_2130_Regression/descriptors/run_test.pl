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

$nr_daemon = 1;
@ports = ( 30000 );
@iorbases = ( "NodeApp.ior" );
@iorfiles = 0;

$controller_exec = "../SEC_CheckPoint/controller";

$ior_embase = "EM.ior";
$ior_emfile = 0;

#  Processes
$E = 0;
$EM = 0;
@DEAMONS = 0;

# targets
@tg_daemons = 0;
$tg_exe_man = 0;
$tg_executor = 0;

$status = 0;
$dat_file = "TestNodeManagerMap.cdd";
$cdp_file = "DeploymentPlan.cdp";

sub create_targets {
    #   daemon
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $tg_daemons[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target for daemon $i failed\n";
        $tg_daemons[$i]->AddLibPath ('../SEC_CheckPoint');
    }
    #   execution manager
    $tg_exe_man = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
    $tg_exe_man->AddLibPath ('../SEC_CheckPoint');
    #   executor (plan_launcher)
    $tg_executor = PerlACE::TestTarget::create_target (1) || die "Create target for executor failed\n";
    $tg_executor->AddLibPath ('../SEC_CheckPoint');
}

sub init_ior_files {
    $ior_emfile = $tg_exe_man->LocalFile ($ior_embase);
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $iorfiles[$i] = $tg_daemons[$i]->LocalFile ($iorbases[$i]);
    }
    delete_ior_files ();
}

# Delete if there are any .ior files.
sub delete_ior_files {
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $tg_daemons[$i]->DeleteFile ($iorbases[$i]);
    }
    $tg_exe_man->DeleteFile ($ior_embase);
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $iorfiles[$i] = $tg_daemons[$i]->LocalFile ($iorbases[$i]);
    }
}

sub kill_node_daemon {
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $DEAMONS[$i]->Kill (); $DEAMONS[$i]->TimedWait (1);
    }
}

sub kill_open_processes {
    if ($daemons_running == 1) {
        kill_node_daemon ();
    }

    if ($em_running == 1) {
        $EM->Kill (); $EM->TimedWait (1);
    }
    # in case shutdown did not perform as expected
    $tg_executor->KillAll ('dance_locality_manager');
}

sub run_node_daemons {
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $iorbase = $iorbases[$i];
        $iorfile = $iorfiles[$i];
        $port = $ports[$i];
        $iiop = "iiop://localhost:$port";
        $node_app = $tg_daemons[$i]->GetArchDir("$DANCE_ROOT/bin/") . "dance_locality_manager";

        $d_cmd = "$DANCE_ROOT/bin/dance_node_manager";
        $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile -a \"-r\"";

        print "Run dance_node_manager with $d_param\n";

        $DEAMONS[$i] = $tg_daemons[$i]->CreateProcess ($d_cmd, $d_param);
        $DEAMONS[$i]->Spawn ();

        if ($tg_daemons[$i]->WaitForFileTimed($iorbase,
                                        $tg_daemons[$i]->ProcessStartWaitInterval ()) == -1) {
            print STDERR
                "ERROR: The ior $iorfile file of node daemon $i could not be found\n";
            for (; $i >= 0; --$i) {
                $DEAMONS[$i]->Kill (); $DEAMONS[$i]->TimedWait (1);
            }
            return -1;
        }
    }
    $daemons_running = 1;
    return 0;
}

create_targets ();
init_ior_files ();


# Invoke node daemons.
print "Invoking node daemons\n";
$status = run_node_daemons ();

if ($status != 0) {
    print STDERR "ERROR: Unable to execute the node daemons\n";
    exit 1;
}

# Invoke execution manager.
print "Invoking execution manager\n";
$EM = $tg_exe_man->CreateProcess ("$CIAO_ROOT/bin/Execution_Manager",
                            "-o $ior_emfile -i -cdd $dat_file");
$EM->Spawn ();

if ($tg_exe_man->WaitForFileTimed ($ior_embase,
                                $tg_exe_man->ProcessStartWaitInterval ()) == -1) {
    print STDERR
      "ERROR: The ior file of execution manager could not be found\n";
    kill_open_processes ();
    exit 1;
}

$em_running = 1;

# Invoke executor - start the application -.
print "Invoking executor - start the application -\n";
$E = $tg_executor->CreateProcess ("$CIAO_ROOT/bin/plan_launcher",
                        "-p $cdp_file -k file://$ior_emfile -o DAM.ior");

$E->SpawnWaitKill ($tg_executor->ProcessStartWaitInterval ());


if ($tg_exe_man->WaitForFileTimed ("TSEC_CheckPoint.ior",
                                $tg_exe_man->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: The ior file of sender could not be found\n";
    kill_open_processes ();
    exit 1;
}

print "Running testcase 1\n";

$controller = $tg_daemons[0]->CreateProcess ("$controller_exec", "-i 1 -k file://TSEC_CheckPoint.ior -t 1 -l 100000");
$result = $controller->SpawnWaitKill ($tg_daemons[0]->ProcessStopWaitInterval ());

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}


print "Running testcase 2\n";

$controller = $tg_daemons[0]->CreateProcess ("$controller_exec", "-i 2 -k file://TSEC_CheckPoint.ior -t 1 -l 100000");
$result = $controller->SpawnWaitKill ($tg_daemons[0]->ProcessStopWaitInterval ());

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

print "Running testcase 3\n";

$controller = $tg_daemons[0]->CreateProcess ("$controller_exec", "-k file://TSEC_CheckPoint.ior -t 2 -l 100000");
$result = $controller->SpawnWaitKill ($tg_daemons[0]->ProcessStopWaitInterval ());

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}


# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
$E = $tg_executor->CreateProcess ("$CIAO_ROOT/bin/plan_launcher",
                        "-k file://EM.ior -i file://DAM.ior");
$E->SpawnWaitKill ($tg_executor->ProcessStartWaitInterval ());

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
