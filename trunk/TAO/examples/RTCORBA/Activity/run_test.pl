eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$startup_timeout = 60;
$experiment_timeout = 60;
$naming_ior = PerlACE::LocalFile ("naming.ior");
$server_flag_file = PerlACE::LocalFile ("server.flag");
$server_conf = PerlACE::LocalFile ("server.conf");
$client_conf = PerlACE::LocalFile ("client.conf");
 
$Naming = new PerlACE::Process ("../../../orbsvcs/Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Activity_Client = new PerlACE::Process ("activity",
                                         "-ORBInitRef NameService=file://$naming_ior " .
                                         "-ORBSvcConf $client_conf");

$Activity_Server = new PerlACE::Process ("activity",
                                         "-ORBInitRef NameService=file://$naming_ior " .
                                         "-ORBSvcConf $server_conf " .
                                         "-Started_Flag $server_flag_file");

unlink $naming_ior;
unlink $server_flag_file;

print STDERR "Running Naming_Service\n";

$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}


print STDERR "Running Activity Server\n";
$Activity_Server->Spawn ();

if (PerlACE::waitforfile_timed ($server_flag_file, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the server to start\n";
  $Activity_Server->Kill ();
  $Naming->Kill ();
  exit 1;
}

print STDERR "Running Activity Client\n";

$status = $Activity_Client->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Client Activity returned $status\n";
  }

$Activity_Server->Kill ();
$Naming->Kill ();

exit $status;
