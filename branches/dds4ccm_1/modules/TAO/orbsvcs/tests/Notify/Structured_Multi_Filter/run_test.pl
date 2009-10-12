eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use strict;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

PerlACE::check_privilege_group();

my $ior = PerlACE::LocalFile ("supplier.ior");
my $namingior = PerlACE::LocalFile ("naming.ior");
my $notifyior = PerlACE::LocalFile ("notify.ior");
my $notify_conf = PerlACE::LocalFile ("notify$PerlACE::svcconf_ext");

my $status = 0;

my $port = PerlACE::uniqueid () + 10005;
my $NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port " .
                            "-o $namingior");
my $TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=iioploc://" .
                            "localhost:$port/NameService " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf" );
my $STS = new PerlACE::Process ("Structured_Supplier");

my $STC = new PerlACE::Process ("Structured_Consumer");

my $args = " -ORBInitRef NameService=iioploc://localhost:$port/NameService ";

my @ops = (undef, "AND", "OR");

unlink $ior;
unlink $notifyior;
unlink $namingior;

$NS->Spawn ();
if (PerlACE::waitforfile_timed ($namingior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the naming service to start\n";
    $NS->Kill ();
    exit 1;
}

$TS->Spawn ();
if (PerlACE::waitforfile_timed ($notifyior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

foreach my $supplier_op (@ops)
{
  foreach my $consumer_op (@ops)
  {
    my $supplier_args = "";
    if (defined $supplier_op) {
      $supplier_args .= " -f $supplier_op";
    }

    my $consumer_args = "";
    if (defined $supplier_op) {
      $consumer_args .= " -s $supplier_op";
    }
    if (defined $consumer_op) {
      $consumer_args .= " -f $consumer_op";
    }

    print "\n**** Testing with $consumer_args ****\n";

    $STS->Arguments($supplier_args . $args);
    $STS->Spawn ();
    if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: waiting for the supplier to start\n";
      $STS->Kill ();
      $status = 1;
      last;
    }

    $STC->Arguments($consumer_args . $args);
    my $client = $STC->SpawnWaitKill(20);
    if ($client != 0) {
      print STDERR "ERROR: Structured_Consumer did not run properly\n";
      $status = 1;
      last;
    }
    my $server = $STS->WaitKill(5);
    if ($server != 0) {
      $TS->Kill ();
      $NS->Kill ();
      exit 1;
    }

    unlink $ior;
  }
  if ($status == 1) {
    last;
  }
}

$TS->Kill ();
$NS->Kill ();

unlink $notifyior;
unlink $namingior;

exit $status;
