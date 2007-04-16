eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# run_test.pl,v 1.5 2006/01/31 21:54:08 wotte Exp
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$DAnCE = "$ENV{'CIAO_ROOT'}/DAnCE";
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$ns_running = 0;
$status = 0;
$dat_file = "NodeDetails.dat";
$cdp_file = "RACE.cdp";

$nsior = PerlACE::LocalFile ("ns.ior");

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
    unlink PerlACE::LocalFile ("ns.ior");
}

delete_ior_files ();


# Invoke naming service

$NS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Naming_Service/Naming_Service", "-m 0 -ORBEndpoint iiop://localhost:40000 -o ns.ior");

$NS->Spawn ();

 print STDERR "Starting Naming Service\n";

 if (PerlACE::waitforfile_timed ($nsior, 10) == -1)
 {
     print STDERR "ERROR: cannot find naming service IOR file\n";
     $NS->Kill ();
     exit 1;
 }

# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:localhost:40000/NameService";

$ns_running = 1;

$iiop = "iiop://localhost:50000";
$node_app = "$CIAO_ROOT/bin/NodeApplication";
$d_cmd = "$CIAO_ROOT/bin/NodeManager";
$iorfile = "RACE.ior";
$d_param = "-n -ORBEndpoint $iiop -s $node_app -o $iorfile -d 60";
$space = new PerlACE::Process ($d_cmd, $d_param);
$space->Spawn ();


# Invoke execution manager.
print "Invoking execution manager\n";
 $EM = new PerlACE::Process ("$CIAO_ROOT/bin/Execution_Manager",
                             " -n -o EM.ior -i $dat_file");
 $EM->Spawn ();

 if (PerlACE::waitforfile_timed ("EM.ior",
                         $PerlACE::wait_interval_for_process_creation) == -1) {
     print STDERR
       "ERROR: The ior file of execution manager could not be found\n";
     kill_open_processes ();
     exit 1;
 }

$em_running = 1;

# print " Invoke executor - start the application -."
print "Invoking executor - start the application -\n";
$E =
  new PerlACE::Process ("$CIAO_ROOT/bin/plan_launcher",
                        "-p $cdp_file -k file://EM.ior");

$E->SpawnWaitKill (5000);


print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

delete_ior_files ();
kill_open_processes ();

exit $status;
