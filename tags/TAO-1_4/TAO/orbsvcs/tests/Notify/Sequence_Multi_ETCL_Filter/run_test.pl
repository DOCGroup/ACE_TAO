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

print "************** Running Sequence Consumer ************\n";

unlink $ior;
$SES->Spawn ();

if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $SES->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
}

$SEC->Arguments($client_args);
$client = $SEC->SpawnWaitKill (60);

$SES->Kill ();
if ($client != 0) {
  print STDERR "ERROR: Sequence_Consumer did not run properly\n";
  $status = 1;
}

$TS->Kill ();
$NS->Kill ();

unlink $ior;
unlink $notifyior;


exit $status;
