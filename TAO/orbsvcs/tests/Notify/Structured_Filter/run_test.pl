eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("supplier.ior");
$notifyior = PerlACE::LocalFile ("notify.ior");

$status = 0;

$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port");
$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=iioploc://" .
                            "localhost:$port/NameService " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf" );
$STS = new PerlACE::Process ("Structured_Supplier");

$STC = new PerlACE::Process ("Structured_Consumer");

$args = " -ORBInitRef NameService=iioploc://localhost:$port/NameService ";
$cargs = "-f -n100 -c2 ";

print "*************** Running Structured Client Filter Test ****************\n";

# Start Naming Service
$NS->Spawn ();
$TS->Spawn ();

sleep 2;

$STS->Arguments($args);
$STS->Spawn ();

if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
      print STDERR "ERROR: waiting for the supplier to start\n";
      $STS->Kill ();
      $TS->Kill ();
      $NS->Kill ();
      $status = 1;
}

sleep 2;

$STC->Arguments($args . $cargs);
$client = $STC->SpawnWaitKill(60);

$STS->Kill();
if ($client !=0) {
  print STDERR "ERROR: Structured_Consumer did not run properly\n";
    $status = 1
}

$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notifyior;


print "*************** Running Structured Client No Filters Test ****************\n";

$cargs = "-n100 -c2 ";

# Start Naming Service
$NS->Spawn ();
$TS->Spawn ();

sleep 2;

$STS->Arguments($args);
$STS->Spawn ();

if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
      print STDERR "ERROR: waiting for the supplier to start\n";
      $STS->Kill ();
      $TS->Kill ();
      $NS->Kill ();
      $status = 1;
}

sleep 2;

$STC->Arguments($args . $cargs);
$client = $STC->SpawnWaitKill(60);

$STS->Kill();
if ($client !=0) {
  print STDERR "ERROR: Structured_Consumer did not run properly\n";
    $status = 1
}

$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notifyior;

exit $status;
