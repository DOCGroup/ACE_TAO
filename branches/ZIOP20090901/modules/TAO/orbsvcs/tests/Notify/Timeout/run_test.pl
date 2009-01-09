eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;
PerlACE::add_lib_path ('../lib');

PerlACE::check_privilege_group();

my $ior = PerlACE::LocalFile('supplier.ior');
my $namingior = PerlACE::LocalFile('naming.ior');
my $notifyior = PerlACE::LocalFile('notify.ior');
my $nscorbaloc = "-ORBInitRef NameService=file://$namingior";
my $NS = new PerlACE::Process("../../../Naming_Service/Naming_Service",
                              "-o $namingior");
my $TS = new PerlACE::Process("../../../Notify_Service/Notify_Service",
                              "$nscorbaloc " .
                              "-IORoutput $notifyior -Timeout 750");
my $STS = new PerlACE::Process("Structured_Supplier",
                              "$nscorbaloc");
my $STC = new PerlACE::Process("Structured_Consumer",
                              "$nscorbaloc");

unlink($ior, $notifyior, $namingior);

$NS->Spawn();
if (PerlACE::waitforfile_timed(
                 $namingior,
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $NS->Kill();
  exit(1);
}

$TS->Spawn();
if (PerlACE::waitforfile_timed(
                 $notifyior,
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

my $server = $STS->Spawn();
if ($server != 0) {
  print STDERR "ERROR: starting the consumer\n";
  $STS->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

## Wait for the supplier to create the event channel in
## the Notify_Service and register it with the Name Service
sleep(2);

my $client = $STC->SpawnWaitKill(20);
if ($client != 0) {
  print STDERR "ERROR: waiting for the supplier\n";
  $STS->Kill();
  $STC->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

$server = $STS->WaitKill(10);
if ($server != 0) {
  print STDERR "ERROR: waiting for the consumer\n";
  $STS->Kill();
  $STC->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}


$TS->Kill();
$NS->Kill();

unlink($ior, $notifyior, $namingior);
exit(0);
