eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# run_test.pl,v 1.5 2006/01/31 21:54:08 wotte Exp
# -*- perl -*-

use lib "$ENV{'ACE_ROOT'}/bin";
use PerlACE::Run_Test;

$DAnCE = "$ENV{'ACE_ROOT'}/TAO/CIAO/DAnCE";
$CIAO_ROOT = "$ENV{'CIAO_ROOT'}";
$TAO_ROOT = "$ENV{'TAO_ROOT'}";
$dat_file = "NodeDetails.dat";
$cdp_file = "RACE.cdp";
$NS_HOST = "blade29.isislab.vanderbilt.edu";
$NS_PORT = 60003;


# Set up NamingService environment
$ENV{"NameServiceIOR"} = "corbaloc:iiop:$NS_HOST:$NS_PORT/NameService";

# Invoke execution manager.
print "Invoking execution manager\n";
$EM = new PerlACE::Process ("$DAnCE/ExecutionManager/Execution_Manager",
                            "-n -o EM.ior -i $dat_file");
$EM->Spawn ();

# if (PerlACE::waitforfile_timed ("EM.ior",
#                         $PerlACE::wait_interval_for_process_creation) == -1) {
#     print STDERR
#       "ERROR: The ior file of execution manager could not be found\n";
#     exit 1;
# }

$em_running = 1;

# Invoke executor - start the application -.
print "Invoking executor - start the application -\n";
$E =
  new PerlACE::Process ("$DAnCE/Plan_Launcher/plan_launcher",
                        "-p $cdp_file -n file://EM.ior");
 
$E->SpawnWaitKill (5000000);


print "Executor returned.\n";
print "Shutting down rest of the processes.\n";

exit $status;
