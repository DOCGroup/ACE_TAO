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
$path_med_prio_ior = PerlACE::LocalFile ("path_med_prio.ior");
$path_med_prio_conf = PerlACE::LocalFile ("path_med_prio.conf");
$path_high_prio_ior = PerlACE::LocalFile ("path_high_prio.ior");

$status = 0;

$Path_med_prio = new PerlACE::Process ("../../../Driver/Notify_Tests_Driver");

$Path_med_prio_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$path_high_prio_ior -IORoutput $path_med_prio_ior -ORBSvcConf $path_med_prio_conf";

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

#if (PerlACE::waitforfile_timed ($path_high_prio_ior, $startup_timeout) == -1) {
#  print STDERR "ERROR: waiting for the High Prio Path to start\n";
#  $Notification->Kill ();
#  $Naming->Kill ();
#  exit 1;
#}

unlink $path_med_prio_ior;
$Path_med_prio->Arguments ($Path_med_prio_Args);
$args = $Path_med_prio->Arguments ();
print STDERR "Running Path_med_prio with arguments: $args\n";

$status=$Path_med_prio->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Med Prio Path test returned $status\n";
    $Path_med_prio->Kill ();
    exit 1;
  }

unlink $path_med_prio_ior;

exit $status;
