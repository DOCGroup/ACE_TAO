eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 600;
$startup_timeout = 60;
$naming_ior = PerlACE::LocalFile ("naming.ior");
$colocated_conf = PerlACE::LocalFile ("colocated.conf");
$status = 0;

$Naming = new PerlACE::Process ("../../../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Colocated = new PerlACE::Process ("../../../Driver/Notify_Tests_Driver");

$Colocated_Args = "-ORBInitRef NameService=file://$naming_ior -ORBSvcConf $colocated_conf";

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

$Colocated->Arguments ($Colocated_Args);
$args = $Colocated->Arguments ();
print STDERR "Running Colocated with arguments: $args\n";
$status = $Colocated->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Colocated returned $status\n";
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }

$Naming->Kill ();
unlink $naming_ior;

exit $status;
