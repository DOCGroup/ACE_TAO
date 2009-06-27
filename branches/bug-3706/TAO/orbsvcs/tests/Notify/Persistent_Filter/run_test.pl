eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;


$experiment_timeout = 60;
$startup_timeout = 60;

$notify_ior = PerlACE::LocalFile ("notify.ior");

$naming_ior = PerlACE::LocalFile ("naming.ior");

$svc_conf = PerlACE::LocalFile ("NS.conf");

$persistent_prefix = "persistency.notif";

$status = 0;

$Naming = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../Notify_Service/Notify_Service");
#-ORBDebugLevel 10 -ORBVerboseLogging 1 
$Notify_Args = "-ORBSvcConf $svc_conf -ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior ";

$Supplier = new PerlACE::Process ("supplier");

$Supplier_Args = "-ORBInitRef NameService=file://$naming_ior";

$Consumer = new PerlACE::Process ("consumer");

$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior";

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

sub run_test
{
  my $client_args = shift;

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

  sleep (5);
  
  $Supplier->Arguments ($client_args . $Supplier_Args);
  $Consumer->Arguments ($client_args . $Consumer_Args);
  $args = $Supplier->Arguments ();
  print STDERR "Running Supplier with arguments: $args\n";
  $args = $Consumer->Arguments ();
  print STDERR "Running Consumer with arguments: $args\n";

  $status = $Supplier->Spawn ();

  if ($status != 0)
    {
      print STDERR "ERROR: Supplier Spawn returned $status\n";
      $Notification->Kill ();
      $Naming->Kill ();
      exit 1;
    }
    
  $status = $Consumer->Spawn ();

  if ($status != 0)
    {
      print STDERR "ERROR: Consumer returned $status\n";
      $Supplier->Kill ();
      $Notification->Kill ();
      $Naming->Kill ();
      exit 1;
    }

  $status = $Consumer->WaitKill ($experiment_timeout);

  if ($status != 0)
    {
      print STDERR "ERROR: Consumer WaitKill returned $status\n";
      $Consumer->Kill ();    
      $Supplier->Kill ();
      $Notification->Kill ();
      $Naming->Kill ();
      exit 1;
    }

  $status = $Supplier->WaitKill ($experiment_timeout);

  if ($status != 0)
    {
      print STDERR "ERROR: Supplier WaitKill returned $status\n";
      $Supplier->Kill ();
      $Notification->Kill ();
      $Naming->Kill ();
      exit 1;
    }

  $Notification->Kill ();
  unlink $notify_ior;
}

unlink <$persistent_prefix.*>;

run_test ();
run_test ("-m -p ");


$Naming->Kill ();
unlink $naming_ior;

exit $status;
