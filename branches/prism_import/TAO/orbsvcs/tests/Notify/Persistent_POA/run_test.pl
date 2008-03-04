eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;

my $ior = PerlACE::LocalFile("supplier.ior");
my $namingior = PerlACE::LocalFile("naming.ior");
my $notifyior = PerlACE::LocalFile("notify.ior");
my $notify2ior = PerlACE::LocalFile("notify2.ior");
my $notify_conf = PerlACE::LocalFile("../Blocking/notify$PerlACE::svcconf_ext");

my $port = PerlACE::random_port();
my $nsport = PerlACE::random_port();
my $NS = new PerlACE::Process("../../../Naming_Service/Naming_Service",
                              "-ORBEndpoint iiop://localhost:$port " .
                              "-o $namingior");
my $TS = new PerlACE::Process("../../../Notify_Service/Notify_Service",
                              "-ORBEndpoint iiop://localhost:$nsport " .
                              "-ORBInitRef NameService=iioploc://" .
                              "localhost:$port/NameService " .
                              "-IORoutput $notifyior -ORBSvcConf " .
                              "$notify_conf");
my $STS = new PerlACE::Process("Structured_Supplier",
                               "-ORBInitRef NameService=iioploc://" .
                               "localhost:$port/NameService");
my $STC = new PerlACE::Process("../Blocking/Structured_Consumer",
                               "-ORBInitRef NameService=iioploc://localhost:" .
                               "$port/NameService");

unlink($ior, $notifyior, $notify2ior, $namingior);

$NS->Spawn();
if (PerlACE::waitforfile_timed($namingior, 20) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $NS->Kill();
  exit(1);
}

print "*********** Starting the first Notify_Service  ***********\n";
$TS->Spawn();
if (PerlACE::waitforfile_timed($notifyior, 20) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

print "*********** Starting the notification supplier ***********\n";
$STS->Spawn();

print "*********** Allowing time to register the ec   ***********\n";
sleep(3);

print "*********** Killing the first Notify_Service   ***********\n";
$TS->Kill();
sleep(1);

print "*********** Starting the second Notify_Service ***********\n";
my $args = $TS->Arguments();
my $l = index($args, $notifyior);
substr($args, $l, length($notifyior)) = $notify2ior;
$TS->Arguments($args);
$TS->Spawn();
if (PerlACE::waitforfile_timed($notify2ior, 20) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $STS->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

if (PerlACE::waitforfile_timed($ior, 5) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n";
  $STS->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

print "*********** Starting the notification consumer ***********\n";
my $client = $STC->SpawnWaitKill(10);
if ($client != 0) {
  $STS->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}
my $server = $STS->WaitKill(5);
if ($server != 0) {
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

$TS->Kill();
$NS->Kill();

unlink($ior, $notifyior, $notify2ior, $namingior);
exit(0);
