eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DAnCE = "$ENV{'DANCE_ROOT'}";

$daemons_running = 0;
$em_running = 0;
$ns_running = 0;

$daemons = 2;
@ports = ( 60001, 60002 );
@iorfiles = ( "NodeApp1.ior", "NodeApp2.ior" );
@nodenames = ( "Sender", "Receiver" );

$status = 0;
$dat_file = "NodeManagerMap.dat";
$cdp_file = "DeploymentPlan_without_ns.cdp";

$controller_exec = "$CIAO_ROOT/examples/Hello/Sender/starter";

PerlACE::add_lib_path ('../lib');

unlink $nsior;

$E = 0;
$EM = 0;

# Delete if there are any .ior files.
sub delete_ior_files {
    for ($i = 0; $i < $daemons; ++$i) {
        unlink $iorfiles[$i];
    }
    unlink PerlACE::LocalFile ("EM.ior");
    unlink PerlACE::LocalFile ("Receiver.ior");
    unlink PerlACE::LocalFile ("Sender.ior");
    unlink PerlACE::LocalFile ("DAM.ior");
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

  if ($ns_running == 1) {
    $NS->Kill ();
    $NS->TimedWait (1);
  }

}

sub run_node_daemons {
  for ($i = 0; $i < $daemons; ++$i)
  {
      $iorfile = $iorfiles[$i];
      $port = $ports[$i];
      $nodename = $nodenames[$i];
      $iiop = "iiop://localhost:$port";
      $node_app = "$CIAO_ROOT/bin/ciao_componentserver";

      $d_cmd = "$DAnCE/bin/dance_node_manager";
      $d_param = "-ORBEndpoint $iiop -s $node_app -n $nodename=$iorfile -t 30";

      print "Run dance_node_manager with $d_param\n";

      $Daemons[$i] = new PerlACE::Process ($d_cmd, $d_param);
      $result = $Daemons[$i]->Spawn ();
      push(@processes, $Daemons[$i]);

      if (PerlACE::waitforfile_timed ($iorfile,
                                      30) == -1) {
          print STDERR
            "ERROR: The ior $iorfile file of node daemon $i could not be found\n";
          for (; $i >= 0; --$i) {
            $Daemons[$i]->Kill (); $Daemons[$i]->TimedWait (1);
          }
          return -1;
      }
  }
  return 0;
}

delete_ior_files ();  

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
print "Invoking execution manager (dance_execution_manager.exe) with -eEM.ior --node-map $dat_file\n";
$EM = new PerlACE::Process ("$DAnCE/bin/dance_execution_manager",
                            "-eEM.ior --node-map $dat_file");
$EM->Spawn ();

if (PerlACE::waitforfile_timed ("EM.ior",
                                $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR
      "ERROR: The ior file of execution manager could not be found\n";
    kill_open_processes ();
    exit 1;
}

$em_running = 1;

# Invoke executor - start the application -.
print "Invoking executor - start the application -\n";

print "Start dance_plan_launcher.exe with -x DeploymentPlan_without_ns.cdp -k file://EM.ior\n";
$E =
  new PerlACE::Process ("$DAnCE/bin/dance_plan_launcher",
                        "-x DeploymentPlan_without_ns.cdp -k file://EM.ior");

$E->SpawnWaitKill (5000);

if (PerlACE::waitforfile_timed (
      "Receiver.ior",
      $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: The ior file of receiver could not be found\n";
    kill_open_processes ();
    exit 1;
}

if (PerlACE::waitforfile_timed ("Sender.ior",
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: The ior file of sender could not be found\n";
    kill_open_processes ();
    exit 1;
}

print "Invoking the controller ($controller_exec -k file://Sender.ior)\n";
$controller = new PerlACE::Process ("$controller_exec", "-k file://Sender.ior");
$result = $controller->SpawnWaitKill (3000);

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
print "by running dance_plan_launcher.exe with -k file://EM.ior -x DeploymentPlan.cdp -q\n";

$E =
  new PerlACE::Process ("$DAnCE/bin/dance_plan_launcher",
                        "-k file://EM.ior -x DeploymentPlan.cdp -q");
$E->SpawnWaitKill (3000);

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
