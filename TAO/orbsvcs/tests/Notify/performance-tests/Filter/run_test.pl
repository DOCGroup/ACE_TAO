eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../../lib');

PerlACE::check_privilege_group();

$ior = PerlACE::LocalFile ("supplier.ior");
$namingior = PerlACE::LocalFile ("naming.ior");
$notifyior = PerlACE::LocalFile ("notify.ior");
$notify_conf = PerlACE::LocalFile ("notify$PerlACE::svcconf_ext");
$status = 0;

$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port " .
                            "-o $namingior");
$TS = new PerlACE::Process ("../../../../Notify_Service/Notify_Service",
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

unlink $ior;
unlink $notifyior;
unlink $namingior;

$client_args = "-ORBInitRef NameService=iioploc://localhost:" .
               "$port/NameService";

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

######################################################################
print "**** Structured Supplier -> 2 Structured Consumers ****\n";
$STS->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $STS->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

$STC->Arguments($client_args);
print $STC->CommandLine ()."\n";
$client = $STC->SpawnWaitKill (300);
print "\n";
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


######################################################################
print "**** Structured Supplier -> Structured Consumer (filtered) ****\n";

unlink $ior;
$STS->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $STS->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

$STC->Arguments($client_args . " -f -c 1");
$client = $STC->SpawnWaitKill (60);
print "\n";
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

######################################################################
print "**** Structured Supplier -> Sequence Consumer (filtered) ****\n";

unlink $ior;
$STS->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n";
  $STS->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

$SEC->Arguments($client_args . " -f -c 1");
$client = $SEC->SpawnWaitKill (60);
print "\n";
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

######################################################################
print "**** Sequence Supplier -> 2 Sequence Consumers ****\n";

unlink $ior;
$SES->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $SES->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

$SEC->Arguments($client_args);
$client = $SEC->SpawnWaitKill (60);
print "\n";
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

######################################################################
print "**** Sequence Supplier -> Sequence Consumer (filtered) ****\n";

unlink $ior;
$SES->Spawn ();
if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n";
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

$SEC->Arguments($client_args . " -f -c 1");
$client = $SEC->SpawnWaitKill (60);
print "\n";
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
unlink $namingior;


exit $status;
