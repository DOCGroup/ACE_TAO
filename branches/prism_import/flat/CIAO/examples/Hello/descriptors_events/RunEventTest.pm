# $Id$
# -*- perl -*-

package RunEventTest;

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
$daemons_running = 0;
$em_running = 0;
$ns_running = 0;
$status = 0;
$dat_file = "NodeManagerMap.dat";
$cdp_file = "flattened_deploymentplan_events.cdp";
$controller_exec = "$CIAO_ROOT/examples/Hello/Sender/starter";
$sender_ior = "Sender.ior";
$receiver_ior = "Receiver.ior";
$nodes;
$E = 0;
$EM = 0;

$nsior = PerlACE::LocalFile ("ns.ior");
unlink $nsior;

sub parse_dat_file {
    my $node_list; # array of hashes, containing node info
    my $lines = 0;

    # read .dat file and parse it for node name, ip and port
    open (my $fh, @_[0]) or die "could not open @_[0]";
    while(<$fh>) {
	if (m{(\w+)\s+corbaloc:iiop:(\w*):([0-9]*)/NodeManager}) {
	    # store the three values in the according row
	    # of the array of hashest
            $node_list->[$lines]->{"node"} = $1; 
            $node_list->[$lines]->{"ip"} = $2;
            $node_list->[$lines++]->{"port"} = $3;
	}
    }
    close ($fh) or die "could not close @_[0]";

    return $node_list;
}

# Delete if there are any .ior files.
sub delete_ior_files {
    for ($i = 0; $i < scalar(@$nodes); ++$i) {
        unlink $nodes->[$i]->{"node"} . ".ior";
    }
    unlink PerlACE::LocalFile ("EM.ior");
    unlink PerlACE::LocalFile ($receiver_ior);
    unlink PerlACE::LocalFile ($sender_ior);
    unlink PerlACE::LocalFile ("DAM.ior");
    unlink PerlACE::LocalFile ("ns.ior");
}

sub kill_node_daemons {
  for ($i = 0; $i < scalar(@$nodes); ++$i) {
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
  for ($i = 0; $i < scalar(@$nodes); ++$i)
  {
      $iorfile = $nodes->[$i]->{"node"} . ".ior";
      $iiop = "iiop://" . $nodes->[$i]->{"ip"} . ":" . $nodes->[$i]->{"port"};
      $node_app = "$CIAO_ROOT/bin/NodeApplication";

      $d_cmd = "$CIAO_ROOT/bin/NodeManager";
      $d_param = "-ORBEndpoint $iiop -s $node_app -o $iorfile -d 30";
      
      print $d_cmd . " " . $d_param . "\n";
      $Daemons[$i] = new PerlACE::Process ($d_cmd, $d_param);
      $result = $Daemons[$i]->Spawn ();
      push(@processes, $Daemons[$i]);

      if (PerlACE::waitforfile_timed ($iorfile,
                          $PerlACE::wait_interval_for_process_creation) == -1) {
          print STDERR
            "ERROR: The ior file of node daemon $i could not be found\n";
          for (; $i >= 0; --$i) {
            $Daemons[$i]->Kill (); $Daemons[$i]->TimedWait (1);
          }
          return -1;
      }
  }
  return 0;
}


sub run_test {
  # first argument will be interpreted as .cdp file
  $cdp_file = shift;
  # second argument will be interpreted as .dat file
  $dat_file = shift;

  $nodes = parse_dat_file ($dat_file);

  delete_ior_files ();

  # Invoke naming service
  $NS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", "-m 0 -ORBEndpoint iiop://localhost:60003 -o ns.ior");
  $NS->Spawn ();

  print STDERR "Starting Naming Service\n";

  if (PerlACE::waitforfile_timed ($nsior, $PerlACE::wait_interval_for_process_creation) == -1){
      print STDERR "ERROR: cannot find naming service IOR file\n";
      $NS->Kill ();
      return 1;
  }

  $ns_running = 1;

  # Invoke node daemons.
  print "Invoking node daemons\n";
  $status = run_node_daemons ();

  if ($status != 0) {
      print STDERR "ERROR: Unable to execute the node daemons\n";
      kill_open_processes ();
      return 1;
  }

  $daemons_running = 1;

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
      return 1;
  }

  $em_running = 1;

  # Invoke executor - start the application -.
  print "Invoking executor - start the application -\n";
  $E =
      new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
			    "-p $cdp_file -k file://EM.ior -o DAM.ior");

  $E->SpawnWaitKill (5000);

  if (PerlACE::waitforfile_timed (
		 $receiver_ior,
		 $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: The ior file of receiver could not be found\n";
      kill_open_processes ();
      return 1;
  }

  if (PerlACE::waitforfile_timed ($sender_ior,
                 $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: The ior file of sender could not be found\n";
      kill_open_processes ();
      return 1;
  }

  print "Invoking the controller\n";
  $controller = new PerlACE::Process ("$controller_exec", "-k file://$sender_ior");
  $result = $controller->SpawnWaitKill (3000);

  if ($result != 0) {
      print STDERR "ERROR: The controller returned $result\n";
      $status = 1;
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

  return $status;
}

1;
