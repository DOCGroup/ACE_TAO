eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../../../../bin";
use PerlACE::Run_Test;
use File::Copy;
use Getopt::Std;

# -n notify.conf -s supplier.conf -c consumer.conf -o output_dir -h
getopts ("n:s:c:o:h");

if ($opt_h)
{
    print STDERR "-n notify.conf -s supplier.conf -c consumer.conf -o output_dir -h\n";
    exit 0;
}

$experiment_timeout = 120;
$startup_timeout = 120;

if ($opt_n)
{
    $notify_conf = PerlACE::LocalFile ($opt_n);

}else
{
    $notify_conf = PerlACE::LocalFile ("notify.conf");
}

if ($opt_s)
{
    $supplier_conf = PerlACE::LocalFile ($opt_s);

}else
{
    $supplier_conf = PerlACE::LocalFile ("supplier.conf");
}

if ($opt_c)
{
    $consumer_conf = PerlACE::LocalFile ($opt_c);

}else
{
    $consumer_conf = PerlACE::LocalFile ("consumer.conf");
}

$notify_ior = PerlACE::LocalFile ("notify.ior");

$naming_ior = PerlACE::LocalFile ("naming.ior");

$supplier_ior = PerlACE::LocalFile ("supplier.ior");

@list=glob("*.dat");
for $file (@list)
{
  unlink $file or die "Could not delete $file";
}

$status = 0;

$Naming = new PerlACE::Process ("../../../../../../Naming_Service/Naming_Service",
                                "-o $naming_ior");

$Notification = new PerlACE::Process ("../../../../../../Notify_Service/Notify_Service");

$Notify_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $notify_ior -ORBSvcConf $notify_conf";

$Supplier = new PerlACE::Process ("../../../../Driver/Notify_Tests_Driver");

$Supplier_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $supplier_ior -ORBSvcConf $supplier_conf -Skip_Priority_Levels_Check";

$Consumer = new PerlACE::Process ("../../../../Driver/Notify_Tests_Driver");

$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$supplier_ior -ORBSvcConf $consumer_conf -Skip_Priority_Levels_Check";

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
  $Supplier->Kill ();
  $Notification->Kill ();
  $Naming->Kill ();
  exit 1;
}

unlink $consumer_ior;
$Consumer->Arguments ($Consumer_Args);
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

unlink $consumer_ior;

$Supplier->Kill ();
unlink $supplier_ior;

$Notification->Kill ();
unlink $notify_ior;

$Naming->Kill ();
unlink $naming_ior;

if ($opt_o)
  {
      $results_directory = PerlACE::LocalFile ($opt_o);

      if (! -e $results_directory)
      {
          mkdir $results_directory, 0777;
      }

      print STDERR "Saving results to $results_directory\n";

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
