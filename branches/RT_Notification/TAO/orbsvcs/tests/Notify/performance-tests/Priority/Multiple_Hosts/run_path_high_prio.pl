eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 600;
$startup_timeout = 60;
$notify_conf = PerlACE::LocalFile ("notify.conf");
$notify_ior = PerlACE::LocalFile ("notify.ior");
$naming_ior = PerlACE::LocalFile ("naming.ior");
$path_high_prio_ior = PerlACE::LocalFile ("path_high_prio.ior");
$path_low_prio_ior = PerlACE::LocalFile ("path_low_prio.ior");
$path_high_prio_conf = PerlACE::LocalFile ("path_high_prio.conf");

$status = 0;

$Path_high_prio = new PerlACE::Process ("../../../Driver/Notify_Tests_Driver");

$Path_high_prio_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$path_low_prio_ior -IORoutput $path_high_prio_ior -ORBSvcConf $path_high_prio_conf";

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

if (PerlACE::waitforfile_timed ($notify_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $Notification->Kill ();
  $Naming->Kill ();
  exit 1;
}

#if (PerlACE::waitforfile_timed ($path_low_prio_ior, $startup_timeout) == -1) {
#  print STDERR "ERROR: waiting for the Low Prio Path to start\n";
#  $Notification->Kill ();
#  $Naming->Kill ();
#  exit 1;
#}

unlink $path_high_prio_ior;
$Path_high_prio->Arguments ($Path_high_prio_Args);
$args = $Path_high_prio->Arguments ();
print STDERR "Running Path_high_prio with arguments: $args\n";

$status=$Path_high_prio->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: High Prio Path test returned $status\n";
    $Path_high_prio->Kill ();
    exit 1;
  }

unlink $path_high_prio_ior;

exit $status;
