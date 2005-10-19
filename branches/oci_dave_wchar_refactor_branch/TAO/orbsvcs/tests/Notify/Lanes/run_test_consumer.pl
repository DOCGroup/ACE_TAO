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
$supplier_ior = PerlACE::LocalFile ("supplier.ior");
$supplier_conf = PerlACE::LocalFile ("supplier.conf");
$consumer_conf = PerlACE::LocalFile ("consumer.conf");
$status = 0;

$Consumer = new PerlACE::Process ("../Driver/Notify_Tests_Driver");

#$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$supplier_ior -ORBSvcConf $consumer_conf";
$Consumer_Args = "-ORBInitRef NameService=file://$naming_ior -IORinput file://$supplier_ior -ORBSvcConf $consumer_conf -ORBDebugLevel 1";

if (PerlACE::waitforfile_timed ($supplier_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n";
  $Supplier->Kill ();
  exit 1;
}

$Consumer->Arguments ($Consumer_Args);
$args = $Consumer->Arguments ();
print STDERR "Running Consumer with arguments: $args\n";
$status = $Consumer->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Consumer returned $status\n";
    exit 1;
  }


exit $status;
