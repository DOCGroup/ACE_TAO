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
$supplier_ior = PerlACE::LocalFile ("supplier.ior");
$status = 0;

@tests = 
  (
   {
    description => "no threads",
    config => "notify_nothreads$PerlACE::svcconf_ext", 
    supplier => " -c 10 -f",
    consumer => " -c 2 -f -e 48",
   },
   {
    description => "multi-threaded dispatching",
    config => "notify_mtdispatching$PerlACE::svcconf_ext", 
    supplier => " ",
    consumer => " ",
   },
   {
    description => "multi-threaded proxy consumers",
    config => "notify_mtsource$PerlACE::svcconf_ext", 
    supplier => " ",
    consumer => " -c 2 -f",
   },
   {
    description => "multi-threaded proxy suppliers",
    config => "notify_mtlistener$PerlACE::svcconf_ext", 
    supplier => " -c 10 -f",
    consumer => " -c 1 -e 60",
   },
  );

@tests2 = 
  (
   {
    description => "multi-threaded dispatching",
    config => "notify_mtdispatching$PerlACE::svcconf_ext", 
    supplier => " ",
    consumer => " ",
   }
  );

$Naming = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../Notify_Service/Notify_Service");

$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior ";

$Supplier = new PerlACE::Process ("Structured_Supplier",
                                  "-ORBInitRef NameService=file://$naming_ior ");

$Consumer = new PerlACE::Process ("Structured_Consumer",
                                  "-ORBInitRef NameService=file://$naming_ior ");

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

for $test (@tests)
  {
    print STDERR "\nTesting $test->{description} ....\n\n";

    unlink $notify_ior;
    $Notification->Arguments ($Notify_Args . " -ORBSvcConf $test->{config}");
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
    $Supplier->Arguments ($Supplier->Arguments () . $test->{supplier});
    $args = $Supplier->Arguments ();
    print STDERR "Running Supplier with arguments: $args\n";
    $Supplier->Spawn ();
    
    if (PerlACE::waitforfile_timed ($supplier_ior, $startup_timeout) == -1) {
      print STDERR "ERROR: waiting for the supplier to start\n";
      $Supplier->Kill ();
      $Notification->Kill ();
      $Naming->Kill ();
      exit 1;
    }
    
    $Consumer->Arguments ($Consumer->Arguments () . $test->{consumer});
    $args = $Consumer->Arguments ();
    print STDERR "Running Consumer with arguments: $args\n";
    $status = $Consumer->SpawnWaitKill ($experiment_timeout);

    if ($status != 0) 
      {
        print STDERR "ERROR: Consumer returned $status\n";
        $Supplier->Kill ();
        $Notification->Kill ();
        $Naming->Kill ();
        exit 1;
      }

    $Supplier->Kill ();
    unlink $supplier_ior;

    $Notification->Kill ();
    unlink $notify_ior;
  }
    
$Naming->Kill ();
unlink $naming_ior;

exit $status;
