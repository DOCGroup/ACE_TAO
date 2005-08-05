eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../../bin";
use PerlACE::Run_Test;
use File::Copy;

$experiment_timeout = 600;
$startup_timeout = 600;
$naming_ior = PerlACE::LocalFile ("naming.ior");
$consumer_ior = PerlACE::LocalFile ("consumer.ior");
$supplier_conf = PerlACE::LocalFile ("supplier.conf");
$consumer_conf = PerlACE::LocalFile ("consumer.conf");
$status = 0;

$Naming = new PerlACE::Process ("../../../../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Supplier = new PerlACE::Process ("../../../../Driver/Notify_Tests_Driver");

$Supplier_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$consumer_ior -ORBSvcConf $supplier_conf";

$Consumer = new PerlACE::Process ("../../../../Driver/Notify_Tests_Driver");

#$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $consumer_ior -ORBSvcConf $consumer_conf -ORBDebugLevel 1";
$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $consumer_ior -ORBSvcConf $consumer_conf";

unlink $naming_ior;
$Naming->Spawn ();

if (PerlACE::waitforfile_timed ($naming_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $Naming->Kill ();
  exit 1;
}

unlink $consumer_ior;
$Consumer->Arguments ($Consumer_Args);
$args = $Consumer->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";
$status = $Consumer->Spawn ();

if (PerlACE::waitforfile_timed ($consumer_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the consumer to start\n";
  $Naming->Kill ();
  exit 1;
}

$Supplier->Arguments ($Supplier_Args);
$args = $Supplier->Arguments ();
print STDERR "Running Supplier with arguments: $args\n";
$Supplier->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Supplier returned $status\n";
    $Supplier->Kill ();
    $Notification->Kill ();
    $Naming->Kill ();
    exit 1;
  }


$Consumer->Wait ();
unlink $consumer_ior;

$Naming->Kill ();
unlink $naming_ior;

if ($#ARGV > -1)
  {
    $results_directory = $ARGV[0];
    print STDERR "Saving results to $results_directory\n";

    mkdir $results_directory, 0777;

    @list=glob("*.dat");
    for $file (@list)
      {
        copy ("$file", "$results_directory/$file");
      }

    @list=glob("*.conf");
    for $file (@list)
      {
        copy ("$file", "$results_directory/$file");
      }
  }

exit $status;
