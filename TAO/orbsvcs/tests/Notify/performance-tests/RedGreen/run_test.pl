eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../bin";
use PerlACE::Run_Test;

$experiment_timeout = 60;
$startup_timeout = 60;
$notifyior = PerlACE::LocalFile ("notify.ior");
$namingior = PerlACE::LocalFile ("naming.ior");
$experiment = "RedGreen";
$status = 0;

unlink $notifyior;
unlink $namingior;

$Naming = new PerlACE::Process ("../../../../Naming_Service/Naming_Service",
                                "-o $namingior");
$Notification = new PerlACE::Process ("../../../../Notify_Service/Notify_Service",
                                      "-ORBInitRef NameService=file://$namingior " .
                                      "-IORoutput $notifyior");
@test_configs =
  (
   "lookup.conf",
   "",
   );

$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($namingior, $startup_timeout) == -1) {
    print STDERR "ERROR: waiting for the naming service to start\n";
    $Naming->Kill ();
    exit 1;
}

$Notification->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, $startup_timeout) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
}

for $config (@test_configs)
  {
    print STDERR "\nTesting $experiment with config file = $config ....\n\n";
    $test = new PerlACE::Process ("./$experiment",
                                  "-ORBInitRef NameService=file://$namingior -ORBSvcConf $config");
    $test->Spawn ();

    $status = $test->WaitKill ($experiment_timeout);

    if ($status != 0)
    {
        print STDERR "ERROR: $name returned $status\n";
        break;
    }
  }


$Notification->Kill ();
$Naming->Kill ();

unlink $namingior;
unlink $notifyior;

exit $status;
