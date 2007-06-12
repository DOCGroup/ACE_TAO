eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
  & eval 'exec perl -S $0 $argv:q'
  if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
$daemons_running = 0;
$em_running = 0;
$ns_running = 0;
$pl_running = 0;
$daemons = 2;
@ports = ( 10000, 20000 );
@iorfiles = ( "NodeManager1.ior", "NodeManager2.ior" );
@componentiorfiles = ("BMDevice.ior", "BMDisplay.ior", "BMClosedED.ior", "EC.ior");

$status = 0;
$dat_file = "BasicSP.dat";
$cdp_file = "BasicSP.cdp";
$emior_file = "EM.ior";
$nsior_file = "ns.ior";
$damior_file = "DAM.ior";
$controller_exec = "$CIAO_ROOT/examples/BasicSP/EC/controller";

$nsior = PerlACE::LocalFile ($nsior_file);
$emior = PerlACE::LocalFile ($emior_file);
$damior = PerlACE::LocalFile ($damior_file);

$PL = 0;
$EM = 0;

# Delete if there are any .ior files.
sub delete_ior_files {
    for ($i = 0; $i < @iorfiles; ++$i) {
        unlink $iorfiles[$i];
    }
    for ($i = 0; $i < @componentiorfiles; ++$i) {
        unlink $componentiorfiles[$i];
    }
    unlink $nsior;
    unlink $emior;
    unlink $damior;
}

sub run_node_daemons {
    for ($i = 0; $i < @iorfiles; ++$i) {
        $iorfile = $iorfiles[$i];
        $port = $ports[$i];

        $iiop = "iiop://localhost:$port";
        $node_app = "$CIAO_ROOT/bin/NodeApplication";

        $d_cmd = "$CIAO_ROOT/bin/NodeManager";
        $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile -d 30";

        $Daemons[$i] = new PerlACE::Process ($d_cmd, $d_param);
        $result = $Daemons[$i]->Spawn ();
        push(@processes, $Daemons[$i]);

        if (PerlACE::waitforfile_timed ($iorfile,
                                        $PerlACE::wait_interval_for_process_creation) == -1) {
            print STDERR
              "ERROR: The ior file of node daemon $i could not be found\n";
            for (; $i >= 0; --$i) {
                $Daemons[$i]->Kill ();
                $Daemons[$i]->TimedWait (1);
            }
            return -1;
        }
    }
    return 0;
}

sub kill_node_daemons {
    for ($i = 0; $i < $daemons; ++$i) {
        $Daemons[$i]->Kill ();
        $Daemons[$i]->TimedWait (1);
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

    if ($ns_running == 1) {
        $NS->Kill ();
        $NS->TimedWait (1);
    }

    if ($pl1_running == 1) {
        $PL1->Kill();
        $PL1->TimedWait (1);
    }

    if ($pl2_running == 1) {
        $PL2->Kill();
        $PL2->TimedWait (1);
    }

    delete_ior_files();
}

delete_ior_files ();


# Invoke naming service

$NS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", "-m 0 -ORBEndpoint iiop://localhost:60003 -o ns.ior");

$NS->Spawn ();

print STDERR "Starting Naming Service\n";

if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find naming service IOR file\n";
    $NS->Kill ();
    exit 1;
}

$ns_running = 1;

# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:localhost:60003/NameService";

# Invoke node daemons.
print "Invoking node daemons\n";
$status = run_node_daemons ();

if ($status != 0) {
    print STDERR "ERROR: Unable to execute the node daemons\n";
    kill_open_processes ();
    exit 1;
}

$daemons_running = 1;

# Invoke execution manager.
print "Invoking execution manager\n";
$EM = new PerlACE::Process ("$CIAO_ROOT/bin/Execution_Manager",
                            "-o $emior_file -i $dat_file");
$EM->Spawn ();

if (PerlACE::waitforfile_timed ("$emior_file",
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR
      "ERROR: The ior file of execution manager could not be found\n";
    kill_open_processes ();
    exit 1;
}

$em_running = 1;

# Deploy the application
print "Invoking plan_launcher - deploy the application -\n";
$PL1 =
  new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
                        "-p $cdp_file -k file://$emior_file -o $damior_file");
$PL1->Spawn ();

$pl1_running = 1;

if (PerlACE::waitforfile_timed ("$damior_file",
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR
      "ERROR: The ior file of domain application manager could not be found\n";
    kill_open_processes ();
    exit 1;
}


sleep (10);

# Invoke executor - start the application -.
print "Starting the application\n";
$controller = new PerlACE::Process ("$controller_exec",
                                    "-k file://$componentiorfiles[3] -o");
$result = $controller->SpawnWaitKill (3000);

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

sleep (10);

# Invoke executor - stop the application -.
print "Stopping the application\n";
$controller = new PerlACE::Process ("$controller_exec",
                                    "-k file://$componentiorfiles[3] -f");
$result = $controller->SpawnWaitKill (3000);

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

# Invoke executor - stop the application -.
print "Invoking plan_launcher - tearing down the application\n";
$PL2 =
  new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
                        "-k file://$emior_file -p $cdp_file -i file://$damior_file");
$PL2->Spawn();

$pl2_running = 1;

sleep(5);

kill_open_processes();
