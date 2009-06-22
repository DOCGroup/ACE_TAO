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
$domain1 = "domain1";
$domain2 = "domain2";
$expect_pass = "0";
$expect_fail = "1";

unlink $notifyior;

$port = PerlACE::uniqueid () + 10001;
$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://localhost:$port");
$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=corbaloc::" .
                            "localhost:$port/NameService " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf");
$SES = new PerlACE::Process ("Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService -d $domain1");
$SEC = new PerlACE::Process ("Consumer");

$client_args = "-ORBInitRef NameService=corbaloc::localhost:" .
               "$port/NameService -f $expect_pass";

$NS->Spawn ();
$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, 20) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

print "************** Running Consumer 1 ************\n";

unlink $ior;
$SES->Spawn ();

if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $SES->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}

$SEC->Arguments($client_args);
$client = $SEC->SpawnWaitKill (60);
if ($client != 0) {
  print STDERR "ERROR: Consumer did not run properly\n";
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  $status = 1;
  exit 1;
}

$server = $SES->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

unlink $ior;




$SES = new PerlACE::Process ("Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService -d $domain2");
$SEC = new PerlACE::Process ("Consumer");

$client_args = "-ORBInitRef NameService=corbaloc::localhost:" .
               "$port/NameService -f $expect_fail";

print "************** Running Consumer 2 ************\n";

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
if ($client != 0) {
  print STDERR "ERROR: Consumer did not run properly\n";
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  $status = 1;
}
$server = $SES->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

$TS->Kill ();

unlink $ior2;
unlink $notifyior;






$notify_conf = PerlACE::LocalFile ("notify2$PerlACE::svcconf_ext");

$TS = new PerlACE::Process ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=corbaloc::" .
                            "localhost:$port/NameService " .
                            "-IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf");
$SES = new PerlACE::Process ("Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService -d $domain1");
$SEC = new PerlACE::Process ("Consumer");

$client_args = "-ORBInitRef NameService=corbaloc::localhost:" .
               "$port/NameService -f $expect_pass";

$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, 20) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill ();
    $NS->Kill ();
    exit 1;
}

print "************** Running Consumer 3 ************\n";

unlink $ior;
$SES->Spawn ();

if (PerlACE::waitforfile_timed ($ior, 20) == -1) {
    print STDERR "ERROR: waiting for the supplier to start\n";
    $SES->Kill ();
    $TS->Kill ();
    $NS->Kill ();
    $status = 1;
    exit 1;
}

$SEC->Arguments($client_args);
$client = $SEC->SpawnWaitKill (60);
if ($client != 0) {
  print STDERR "ERROR: Consumer did not run properly\n";
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  $status = 1;
  exit 1;
}

$server = $SES->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

unlink $ior;




$SES = new PerlACE::Process ("Supplier",
                             "-ORBInitRef NameService=iioploc://" .
                             "localhost:$port/NameService -d $domain2");
$SEC = new PerlACE::Process ("Consumer");

$client_args = "-ORBInitRef NameService=corbaloc::localhost:" .
               "$port/NameService -f $expect_pass";

print "************** Running Consumer 4 ************\n";

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
if ($client != 0) {
  print STDERR "ERROR: Consumer did not run properly\n";
  $SES->Kill ();
  $TS->Kill ();
  $NS->Kill ();
  $status = 1;
}
$server = $SES->WaitKill(5);
if ($server != 0) {
  $TS->Kill ();
  $NS->Kill ();
  exit 1;
}

$TS->Kill ();
$NS->Kill ();

unlink $ior2;
unlink $notifyior;

exit $status;
