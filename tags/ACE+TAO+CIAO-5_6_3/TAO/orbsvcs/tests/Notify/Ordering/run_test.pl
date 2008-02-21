eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

PerlACE::check_privilege_group();

$ior = PerlACE::LocalFile ("supplier.ior");
$notifyior = PerlACE::LocalFile ("notify.ior");
$naming_ior = PerlACE::LocalFile ("naming.ior");
$notify_conf = PerlACE::LocalFile ("notify$PerlACE::svcconf_ext");
$status = 0;
$deadline = 0;

foreach my $arg (@ARGV) {
  if ($arg eq "-d") {
    $deadline = 1;
  }
  else {
    print "Usage: $0 [-d]\n" .
          "       -d specifies that deadline discarding be tested.\n";
    exit(0);
  }
}

$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port -o $naming_ior");
$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=iioploc://" .
                            "localhost:$port/NameService " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf");
$STS = new PerlACE::Process ("Structured_Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService");
$STC = new PerlACE::Process ("Structured_Consumer");

$SES = new PerlACE::Process ("Sequence_Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService");
$SEC = new PerlACE::Process ("Sequence_Consumer");

$client_args = "-ORBInitRef NameService=iioploc://localhost:" .
               "$port/NameService";

unlink $notifyior;
unlink $naming_ior;

$NS->Spawn ();
if (PerlACE::waitforfile_timed ($naming_ior, $PerlACE::wait_interval_for_process_creation) == -1) {
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

if ($deadline) {
  ## @@todo : Add combinations of deadline ordering.
}

# Although the TAO notify service supports OrderPolicy on the supplier side
# QoS, this setting typically serves no practical purpose, and is not testable.
# This is because we have no way to force the supplier-side queue to back up, and
# the OrderPolicy will have no affect.
# Therefore we don't test setting this policy on the supplier side.

print "**** Structured Supplier (fifo) -> Structured Consumer (none) *****\n";
unlink $ior;
$STS->Arguments($STS->Arguments() . " -d fifo");
$STS->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $STS->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}
$STC->Arguments($client_args . " -d fifo");
$client = $STC->SpawnWaitKill (20);
if ($client != 0) {
  $STS->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}
$server = $STS->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

print "**** Structured Supplier (fifo) -> Structured Consumer (priority) *****\n";
unlink $ior;
$STS->Arguments($STS->Arguments() . " -d fifo");
$STS->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $STS->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}
$STC->Arguments($client_args . " -d priority -o");
$client = $STC->SpawnWaitKill (20);
if ($client != 0) {
  $STS->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}
$server = $STS->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

print "**** Structured Supplier (fifo) -> Sequence Consumer (priority) *****\n";
unlink $ior;
$STS->Arguments($STS->Arguments() . " -d fifo");
$STS->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $STS->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}
$SEC->Arguments($client_args . " -d priority -o");
$client = $SEC->SpawnWaitKill (20);
if ($client != 0) {
  $STS->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}
$server = $STS->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

print "**** Sequence Supplier (fifo) -> Structured Consumer (priority) *****\n";
unlink $ior;
$SES->Arguments($SES->Arguments() . " -d fifo");
$SES->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    $SES->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}
$STC->Arguments($client_args . " -d priority -o");
$client = $STC->SpawnWaitKill (20);
if ($client != 0) {
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}
$server = $SES->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

print "**** Sequence Supplier (fifo) -> Sequence Consumer (priority) *****\n";
unlink $ior;
$SES->Arguments($SES->Arguments() . " -d fifo");
$SES->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    $SES->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}
$SEC->Arguments($client_args . " -d priority -o");
$client = $SEC->SpawnWaitKill (20);
if ($client != 0) {
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}
$server = $SES->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}


$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notifyior;
unlink $naming_ior;

exit $status;
