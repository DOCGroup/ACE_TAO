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
$supplier_conf = PerlACE::LocalFile ("supplier.conf");
$consumer_conf = PerlACE::LocalFile ("consumer.conf");
$status = 0;

$Supplier = new PerlACE::Process ("../Driver/Notify_Tests_Driver");

$Supplier_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $supplier_ior -ORBSvcConf $supplier_conf";
#$Supplier_Args = "-ORBInitRef NameService=file://$naming_ior -IORoutput $supplier_ior -ORBSvcConf $supplier_conf -ORBDebugLevel 1";

if (PerlACE::waitforfile_timed ($notify_ior, $startup_timeout) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  exit 1;
}

unlink $supplier_ior;
$Supplier->Arguments ($Supplier_Args);
$args = $Supplier->Arguments ();
print STDERR "Running Supplier with arguments: $args\n";
$status = $Supplier->SpawnWaitKill ($experiment_timeout);

if ($status != 0)
  {
    print STDERR "ERROR: Supplier returned $status\n";
    $Supplier->Kill ();
    exit 1;
  }

$Supplier->Kill ();
unlink $supplier_ior;

exit $status;
