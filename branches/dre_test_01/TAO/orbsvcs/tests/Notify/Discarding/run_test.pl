eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("supplier.ior");
$notifyior = PerlACE::LocalFile ("notify.ior");
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

unlink $notifyior;

$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port");
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
$NS->Spawn ();
$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, 20) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

@policies = ("fifo", "priority", "lifo");
if ($deadline) {
  push(@policies, "deadline");
}

@server_opts = ("", "", "", " -d");
for($i = 0; $i <= $#policies; $i++) {
  $discard_policy = $policies[$i];
  print "************* Running Structured Consumer with the " .
        "$discard_policy policy ***********\n";

  unlink $ior;
  $STS->Arguments($STS->Arguments() . $server_opts[$i]);
  $STS->Spawn ();

  if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
      print STDERR "ERROR: waiting for the supplier to start\n";
      $STS->Kill ();
      $TS->Kill ();
      $NS->Kill ();
      $status = 1;
      last;
  }

  $STC->Arguments($client_args . " -d $discard_policy");
  $client = $STC->SpawnWaitKill (60);

  $STS->Kill ();
  if ($client != 0) {
    print STDERR "ERROR: Structured_Consumer did not run properly\n";
    $status = 1;
    last;
  }
}

if ($status == 0) {
  for($i = 0; $i <= $#policies; $i++) {
    $discard_policy = $policies[$i];
    print "************** Running Sequence Consumer with the " .
          "$discard_policy policy ************\n";

    unlink $ior;
    $SES->Arguments($SES->Arguments() . $server_opts[$i]);
    $SES->Spawn ();

    if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
        print STDERR "ERROR: waiting for the supplier to start\n";
        $SES->Kill ();
        $TS->Kill ();
        $NS->Kill ();
        $status = 1;
        last;
    }

    $SEC->Arguments($client_args . " -d $discard_policy");
    $client = $SEC->SpawnWaitKill (60);

    $SES->Kill ();
    if ($client != 0) {
      print STDERR "ERROR: Sequence_Consumer did not run properly\n";
      $status = 1;
      last;
    }
  }
}

$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notifyior;


exit $status;
