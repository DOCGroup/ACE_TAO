eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 600;
$startup_timeout = 600;
$notify_conf = PerlACE::LocalFile ("notify.conf");
$notify_ior = PerlACE::LocalFile ("notify.ior");
$naming_ior = PerlACE::LocalFile ("naming.ior");
$status = 0;

$Naming = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../Notify_Service/Notify_Service");

#$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior -ORBSvcConf $notify_conf";
$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior -ORBSvcConf $notify_conf -ORBDebugLevel 1";

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
$status = $Notification->SpawnWaitKill ($experiment_timeout);


$Notification->Kill ();
unlink $notify_ior;

$Naming->Kill ();
unlink $naming_ior;

exit $status;
