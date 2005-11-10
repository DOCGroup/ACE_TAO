eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 60;
$startup_timeout = 60;

$notify_ior = PerlACE::LocalFile ("notify.ior");

$naming_ior = PerlACE::LocalFile ("naming.ior");

$status = 0;

$Naming = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../Notify_Service/Notify_Service");

$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior ";

$Filter = new PerlACE::Process ("Filter");

$Filter_Args = "-ORBInitRef NameService=file://$naming_ior";

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

unlink $notify_ior;
$Notification->Arguments ($Notify_Args);
$args = $Notification->Arguments ();
print STDERR "Running Notification with arguments: $args\n";
$Notification->Spawn ();

if (PerlACE::waitforfile_timed ($notify_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $Notification->Kill ();
  $Naming->Kill ();
  exit 1;
}

$Filter->Arguments ($Filter_Args);
$args = $Filter->Arguments ();
print STDERR "Running Filter with arguments: $args\n";
$status = $Filter->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Filter returned $status\n";
    $Filter->Kill ();
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }

$Notification->Kill ();
unlink $notify_ior;

$Naming->Kill ();
unlink $naming_ior;

exit $status;
