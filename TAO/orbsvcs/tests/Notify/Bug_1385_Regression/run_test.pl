eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

use strict;

PerlACE::check_privilege_group();

my $ior = PerlACE::LocalFile("supplier.ior");
my $namingior = PerlACE::LocalFile("naming.ior");
my $notifyior = PerlACE::LocalFile("notify.ior");
my $notify_conf = PerlACE::LocalFile("notify$PerlACE::svcconf_ext");
my $status = 0;
my $port = PerlACE::random_port();

my $NS = new PerlACE::Process("../../../Naming_Service/Naming_Service",
                              "-ORBEndpoint iiop://localhost:$port " .
                              "-o $namingior");
my $TS = new PerlACE::Process("../../../Notify_Service/Notify_Service",
                              "-ORBInitRef NameService=iioploc://" .
                              "localhost:$port/NameService " .
                              "-IORoutput $notifyior -ORBSvcConf " .
                              "$notify_conf");
my $STS = new PerlACE::Process("Structured_Supplier",
                               "-ORBInitRef NameService=iioploc://" .
                               "localhost:$port/NameService");
my $STC = new PerlACE::Process("Structured_Consumer",
                               "-ORBInitRef " .
                               "NameService=iioploc://localhost:" .
                               "$port/NameService");

unlink($ior, $notifyior, $namingior);

$NS->Spawn();
if (PerlACE::waitforfile_timed($namingior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $NS->Kill();
  exit(1);
}

$TS->Spawn();
if (PerlACE::waitforfile_timed($notifyior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

unlink $ior;
$STS->Arguments($STS->Arguments());
$STS->Spawn();
if (PerlACE::waitforfile_timed($ior, $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n";
  $STS->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

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

unlink($ior, $notifyior, $namingior);
exit($status);
