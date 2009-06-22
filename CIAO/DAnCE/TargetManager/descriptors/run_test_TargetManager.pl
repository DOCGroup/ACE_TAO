eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$DAnCE = "$ENV{'DANCE_ROOT'}";

$daemons_running = 0;
$em_running = 0;
$daemons = 2;
@ports = ( 40000, 30000 );
@iorfiles = ( "NodeApp1.ior", "NodeApp2.ior" );
$status = 0;
$dat_file = "NodeDetails.dat";
$cdp_file = "flattened_DeploymentPlan.cdp";

$E = 0;
$EM = 0;

# Delete if there are any .ior files.
sub delete_ior_files {
    for ($i = 0; $i < $daemons; ++$i) {
        unlink $iorfiles[$i];
    }
    unlink PerlACE::LocalFile ("EM.ior");
    unlink PerlACE::LocalFile ("TargetManager.ior");
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
}

sub run_node_daemons {
  for ($i = 0; $i < $daemons; ++$i)
  {
      $iorfile = $iorfiles[$i];
      $port = $ports[$i];

      $iiop = "iiop://localhost:$port";
      $node_app = "$CIAO_ROOT/bin/NodeApplication";

      $d_cmd = "$DANCE_ROOT/bin/dance_node_manager";
      $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile";

      $Daemons[$i] = new PerlACE::Process ($d_cmd, $d_param);
      $result = $Daemons[$i]->Spawn ();
      push(@processes, $Daemons[$i]);

      if (PerlACE::waitforfile_timed ($iorfile,
                          $PerlACE::wait_interval_for_process_creation) == -1) {
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

$ns_running = 1;

# Invoke execution manager.
print "Invoking execution manager\n";
$EM = new PerlACE::Process ("$CIAO_ROOT/bin/Execution_Manager",
                            "-o EM.ior -i $dat_file");
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
$E =
  new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
                        "-p flattened_deploymentplan.cdp -k file://EM.ior -o DAM.ior");

$E->SpawnWaitKill (5000);

if (PerlACE::waitforfile_timed (
      "TargetManager.ior",
      $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: The ior file of receiver could not be found\n";
    kill_open_processes ();
    exit 1;
}

print "Waiting for 15 secs then invoking client ===\n";
sleep (15);

print "Making 5 such calls \n";

$call_num = 5;

for ($i = 0; $i < $call_num; ++$i)
{
  print "Invoking the client\n";
  $tmclient = new PerlACE::Process ("../CmpClient", "-t file://TargetManager.ior");
  $result = $tmclient->SpawnWaitKill (3000);

  if ($result != 0) {
      print STDERR "ERROR: The client returned $result\n";
      $status = 1;
  }
  sleep (15);
}

# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
$E =
  new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
                        "-k file://EM.ior -i file://DAM.ior");
$E->SpawnWaitKill (3000);

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
