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
$notify_conf = PerlACE::LocalFile ("notify$PerlACE::svcconf_ext");
$status = 0;


$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port");
$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=iioploc://" .
                            "localhost:$port/NameService " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf");
$SES = new PerlACE::Process ("Sequence_Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService");
$SEC = new PerlACE::Process ("Sequence_Consumer");

$client_args = "-ORBInitRef NameService=iioploc://localhost:" .
               "$port/NameService";
$NS->Spawn ();

unlink $notifyior;
$TS->Spawn ();
if (PerlACE::waitforfile_timed ($notifyior, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

@low = (40, 20, 20, 20);
@high = (40, 21, 21, 21);
@constraintList = (
  "\"Number == 20 or Number == 10\"",
  "\"Number == 20 or Index == 7\"",
  "\"Number == 20 or Index == 4\"",
  "\"Number == 10 or (Number == 20 and Index == 1)\"",
);

$i = 0;

foreach $constraintString (@constraintList) {
  print "<*><*><*> Constraint String: $constraintString  <*><*><*>\n";

  unlink $ior;
  $SES->Spawn ();
  if (PerlACE::waitforfile_timed ($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
      print STDERR "ERROR: waiting for the supplier to start\n";
      $SES->Kill ();
      $TS->Kill ();
      $NS->Kill ();
      $status = 1;
      last;
  }

  $SEC->Arguments($client_args . " -l $low[$i] -h $high[$i] -c $constraintString");

  $i = $i + 1;

  $client = $SEC->SpawnWaitKill (60);
  if ($client != 0) {
    print STDERR "ERROR: Sequence_Consumer did not run properly\n";
    $status = 1;
    last;
  }
  $server = $SES->WaitKill(5);
  if ($server != 0) {
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
  }
}

$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notifyior;


exit $status;
