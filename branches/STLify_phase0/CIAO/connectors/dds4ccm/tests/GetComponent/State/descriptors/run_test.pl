eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::TestTarget;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DANCE_ROOT = "$ENV{'DANCE_ROOT'}";

$daemons_running = 0;
$em_running = 0;
$ns_running = 0;

$nr_daemon = 2;
@ports = ( 60001, 60002 );
@iorbases = ( "Sender.ior", "Receiver.ior" );
@iorfiles = 0;
@nodenames = ( "SenderNode", "ReceiverNode" );

# ior files other than daemon
# ior files other than daemon
$ior_nsbase = "ns.ior";
$ior_nsfile = 0;
$ior_embase = "EM.ior";
$ior_emfile = 0;

#  Processes
$E = 0;
$EM = 0;
$NS = 0;
@DEAMONS = 0;

# targets
@tg_daemons = 0;
$tg_naming = 0;
$tg_exe_man = 0;
$tg_executor = 0;

$status = 0;
$cdp_file = "Plan.cdp";

sub create_targets {
    #   naming service
    $tg_naming = PerlACE::TestTarget::create_target (1) || die "Create target for ns failed\n";
    $tg_naming->AddLibPath ('../lib');
    #   daemon
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $tg_daemons[$i] = PerlACE::TestTarget::create_target ($i+1) || die "Create target for deamon $i failed\n";
        $tg_daemons[$i]->AddLibPath ('../lib');
    }
    #   execution manager
    $tg_exe_man = PerlACE::TestTarget::create_target (1) || die "Create target for EM failed\n";
    $tg_exe_man->AddLibPath ('../lib');
    #   executor (plan_launcher)
    $tg_executor = PerlACE::TestTarget::create_target (1) || die "Create target for executor failed\n";
    $tg_executor->AddLibPath ('../lib');
}

sub init_ior_files {
    $ior_nsfile = $tg_naming->LocalFile ($ior_nsbase);
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
    $tg_naming->DeleteFile ($ior_nsbase);
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

    if ($ns_running == 1) {
        $NS->Kill (); $NS->TimedWait (1);
    }
    # in case shutdown did not perform as expected
    $tg_executor->KillAll ('dance_locality_manager');
}

sub run_node_daemons {
    for ($i = 0; $i < $nr_daemon; ++$i) {
        $iorbase = $iorbases[$i];
        $iorfile = $iorfiles[$i];
        $port = $ports[$i];
        $nodename = $nodenames[$i];
        $iiop = "iiop://localhost:$port";
        $node_app = $tg_daemons[$i]->GetArchDir("$DANCE_ROOT/bin/") . "dance_locality_manager";

        $d_cmd = "$DANCE_ROOT/bin/dance_node_manager";
        $d_param = "-ORBEndpoint $iiop -s $node_app -n $nodename=$iorfile -t 30 --domain-nc corbaloc:rir:/NameService";

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
    return 0;
}

create_targets ();
init_ior_files ();

# Invoke naming service

$NS = $tg_naming->CreateProcess ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", " -ORBEndpoint iiop://localhost:60003 -o $ior_nsfile");

$ns_status = $NS->Spawn ();

if ($ns_status != 0) {
    print STDERR "ERROR: Unable to execute the naming service\n";
    kill_open_processes ();
    exit 1;
}

print STDERR "Starting Naming Service with  -ORBEndpoint iiop://localhost:60003 -o ns.ior\n";

if ($tg_naming->WaitForFileTimed ($ior_nsbase,
                                  $tg_naming->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$ns_running = 1;
# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:localhost:60003/NameService";

# Invoke node daemon.
print "Invoking node daemon\n";
$status = run_node_daemons ();

if ($status != 0) {
    print STDERR "ERROR: Unable to execute the node daemon\n";
    kill_open_processes ();
    exit 1;
}

$daemons_running = 1;

# Invoke execution manager.
print "Invoking execution manager (dance_execution_manager.exe) with -e$ior_emfile\n";
$EM = $tg_exe_man->CreateProcess ("$DANCE_ROOT/bin/dance_execution_manager",
                                    "-e$ior_emfile --domain-nc corbaloc:rir:/NameService");
$em_status = $EM->Spawn ();

if ($em_status != 0) {
    print STDERR "ERROR: dance_execution_manager returned $em_status";
    exit 1;
}

if ($tg_exe_man->WaitForFileTimed ($ior_embase,
                                $tg_exe_man->ProcessStartWaitInterval ()) == -1) {
    print STDERR
        "ERROR: The ior file of execution manager could not be found\n";
    kill_open_processes ();
    exit 1;
}

$em_running = 1;

# Invoke executor - start the application -.
print "Invoking executor - launch the application -\n";

print "Start dance_plan_launcher.exe with -x $cdp_file -k file://$ior_emfile\n";
$E = $tg_executor->CreateProcess ("$DANCE_ROOT/bin/dance_plan_launcher",
                        "-x $cdp_file -k file://$ior_emfile");
$pl_status = $E->SpawnWaitKill (2 * $tg_executor->ProcessStartWaitInterval ());

if ($pl_status != 0) {
    print STDERR "ERROR: dance_plan_launcher returned $pl_status\n";
    kill_open_processes ();
    exit 1;
}

for ($i = 0; $i < $nr_daemon; ++$i) {
    if ($tg_daemons[$i]->WaitForFileTimed ($iorbases[$i],
                            $tg_daemons[$i]->ProcessStopWaitInterval ()) == -1) {
        print STDERR "ERROR: The ior file of daemon $i could not be found\n";
        kill_open_processes ();
        exit 1;
    }
}

print "Sleeping 5 seconds to allow task to complete\n";
sleep (5);

# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
print "by running dance_plan_launcher.exe with -k file://$ior_emfile -x $cdp_file -q\n";

$E = $tg_executor->CreateProcess ("$DANCE_ROOT/bin/dance_plan_launcher",
                        "-k file://$ior_emfile -x $cdp_file -s");
$pl_status = $E->SpawnWaitKill ($tg_executor->ProcessStartWaitInterval ());

if ($pl_status != 0) {
    print STDERR "ERROR: dance_plan_launcher returned $pl_status\n";
    kill_open_processes ();
    exit 1;
}

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
