eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::check_privilege_group();

# ThreadPool Example
#

$experiment_timeout = 60;
$startup_timeout = 60;
$max_events = 10;
$ec_tp = 0;
$pc_tp = 0;
$ps1_tp = 1;
$ps2_tp = 1;
$debug = 0;

$notify_conf = PerlACE::LocalFile ("notify.conf");
$notify_ior = PerlACE::LocalFile ("notify.ior");

$naming_ior = PerlACE::LocalFile ("naming.ior");

$supplier_ior = PerlACE::LocalFile ("supplier.ior");

$supplier_conf = PerlACE::LocalFile ("client.conf");

$consumer_conf = PerlACE::LocalFile ("client.conf");

$consumer2_conf = PerlACE::LocalFile ("client.conf");

$status = 0;

$Naming = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../Notify_Service/Notify_Service");

$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior -ORBSvcConf $notify_conf -ORBDebugLevel $debug";

$Supplier = new PerlACE::Process ("Supplier");

$Supplier_Args = "-ORBInitRef NameService=file://$naming_ior -ORBSvcConf $supplier_conf -IORoutput $supplier_ior -Consumers 2 -EventChannel_ThreadPool $ec_tp -MaxEvents $max_events -ProxyConsumer_ThreadPool $pc_tp -ORBDebugLevel $debug";

$Consumer = new PerlACE::Process ("Consumer");

$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior -ORBSvcConf $consumer_conf  -MaxEvents $max_events -Delay 1 -ProxySupplier_ThreadPool $ps1_tp -ORBDebugLevel $debug";

$Consumer2 = new PerlACE::Process ("Consumer");

$Consumer2_Args = "-ORBInitRef NameService=file://$naming_ior -ORBSvcConf $consumer2_conf  -MaxEvents $max_events -ProxySupplier_ThreadPool $ps2_tp -ORBDebugLevel $debug";

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

unlink $supplier_ior;
$Supplier->Arguments ($Supplier_Args);
$args = $Supplier->Arguments ();
print STDERR "Running Supplier with arguments: $args\n";
$Supplier->Spawn ();

if (PerlACE::waitforfile_timed ($supplier_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n"; 
  $Notification->Kill ();
  $Naming->Kill ();
  exit 1;
}

$Consumer->Arguments ($Consumer_Args);
$args = $Consumer->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";
$status = $Consumer->Spawn ();

if ($status != 0)
  {
    print STDERR "ERROR: Consumer returned $status\n";
    $Supplier->Kill ();
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }

$Consumer2->Arguments ($Consumer2_Args);
$args = $Consumer2->Arguments ();
print STDERR "Running Consumer2 with arguments: $args\n";
$status = $Consumer2->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Consumer2 returned $status\n";
    $Supplier->Kill ();
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }

$Consumer->Wait ();
$Supplier->Wait ();

unlink $supplier_ior;

$Notification->Kill ();
unlink $notify_ior;

$Naming->Kill ();
unlink $naming_ior;

exit $status;
