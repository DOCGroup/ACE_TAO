eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("supplier.ior");
$notifyior = PerlACE::LocalFile ("notify.ior");
$notify_conf = PerlACE::LocalFile ("notify.conf");
$status = 0;

unlink $notifyior;

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
$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, 20) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

if ($status == 0) {
    $discard_policy = fifo;

@low = (6, 3, 6, 3, 1);
@high = (7, 4, 7, 4, 2);
@constraintList = (
  "\"Number == 20 or Number == 10\"",
  "\"Number == 20 or Index == 7\"",
  "\"Number == 20 or Number == 10\"",
  "\"Number == 20 or Index == 4\"",
  "\"Number == 20 and Index == 1\"",
);

    $i = 0;
    print "************** Running Sequence Consumer with the " .
          "$discard_policy policy ************\n";

foreach $constraintString (@constraintList) {
       print "<*><*><*> Constraint String: $constraintString  <*><*><*>\n";

    unlink $ior;
    $SES->Spawn ();

    if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
        print STDERR "ERROR: waiting for the supplier to start\n";
        $SES->Kill ();
        $TS->Kill ();
        $NS->Kill ();
        $status = 1;
        last;
    }

    $SEC->Arguments($client_args . " -l $low[$i] -h $high[$i] -d $discard_policy -c $constraintString");

    $i = $i + 1;

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
