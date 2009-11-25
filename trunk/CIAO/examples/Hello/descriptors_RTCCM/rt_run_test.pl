eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;
$CIAO_ROOT = $PerlACE::CIAO_ROOT;
$DAnCE = "$CIAO_ROOT/DAnCE";

$daemons_running = 0;
$em_running = 0;
$daemons = 2;
@ports = ( 60001, 60002 );
@iorfiles = ( "NodeApp1.ior", "NodeApp2.ior" );
$status = 0;
$dat_file = "NodeManagerMap.dat";
$cdp_file = "DeploymentPlan.cdp";
$controller_exec = "$CIAO_ROOT/examples/Hello/Sender/starter";

PerlACE::add_lib_path ('../lib');

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
}

sub run_node_daemons {
  for ($i = 0; $i < $daemons; ++$i)
  {
      $iorfile = $iorfiles[$i];
      $port = $ports[$i];

      $iiop = "iiop://localhost:$port";
      $node_app = "$CIAO_ROOT/bin/NodeApplication";

      $d_cmd = "$DANCE_ROOT/bin/dance_node_manager";

      # NON-RT
#      $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile";

      # RT enabled
      $d_param = "-ORBEndpoint $iiop -s $node_app -a \"-r\" -o $iorfile";

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
                        "-p rt-example.cdp -k file://EM.ior -o DAM.ior");

$E->SpawnWaitKill (60);

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

print "Invoking the controller\n";
$controller = new PerlACE::Process ("$controller_exec", "-k file://Sender.ior");
$result = $controller->SpawnWaitKill (60);

if ($result != 0) {
    print STDERR "ERROR: The controller returned $result\n";
    $status = 1;
}

# Invoke executor - stop the application -.
print "Invoking executor - stop the application -\n";
$E =
  new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
                        "-k file://EM.ior -i file://DAM.ior");
$E->SpawnWaitKill (60);

print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
