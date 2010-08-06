eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

use strict;

PerlACE::add_lib_path ('../lib');

PerlACE::check_privilege_group();

my $ior = PerlACE::LocalFile('test_monitor.ior');
my $monitorior = PerlACE::LocalFile('monitor.ior');
my $namingior = PerlACE::LocalFile('naming.ior');
my $notifyior = PerlACE::LocalFile('notify.ior');
my $ready = PerlACE::LocalFile('ready.txt');
my $notify_conf = PerlACE::LocalFile("notify$PerlACE::svcconf_ext");
my $port = PerlACE::random_port();
my $mc_conf= "dynamic TAO_MonitorAndControl Service_Object * ".
    "TAO_CosNotification_MC:_make_TAO_MonitorAndControl () \\\"-o ".
    "monitor.ior -ORBArg \\\"-ORBInitRef ".
    "NameService=corbaloc:iiop:localhost:$port/NameService\\\"\\\"";
my $nscorbaloc = "-ORBInitRef NameService=corbaloc:iiop:" .
                 "localhost:$port/NameService";
my $NS = new PerlACE::Process("../../../Naming_Service/Naming_Service",
                              "-ORBEndpoint iiop://localhost:$port " .
                              "-o $namingior");
my $TS = new PerlACE::Process("../../../Notify_Service/Notify_Service",
                              "$nscorbaloc " .
                              "-IORoutput $notifyior -ORBSvcConf " .
                              "$notify_conf -ORBSvcConfDirective \"$mc_conf\"");
my $MON = new PerlACE::Process("test_monitor",
                               "-k file://$monitorior");
my $STS = new PerlACE::Process("Structured_Supplier",
                              "$nscorbaloc");
my $STC = new PerlACE::Process("Structured_Consumer",
                              "$nscorbaloc");

unlink($ior, $monitorior, $notifyior, $namingior, $ready);

print $NS->CommandLine()."\n";
$NS->Spawn();
if (PerlACE::waitforfile_timed(
                 $namingior,
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the naming service to start\n";
  $NS->Kill();
  exit(1);
}

print $TS->CommandLine()."\n";
$TS->Spawn();
if (PerlACE::waitforfile_timed(
                 $notifyior,
                 $PerlACE::wait_interval_for_process_creation * 2) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

print $MON->CommandLine()."\n";
$MON->Spawn();
if (PerlACE::waitforfile_timed(
                 $ior,
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the supplier to start\n";
  $MON->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

print $STC->CommandLine()."\n";
my $client = $STC->Spawn();
if ($client != 0) {
  print STDERR "ERROR: starting the consumer\n";
  $STC->Kill();
  $MON->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

## Wait for the consumer to create the event channel in
## the Notify_Service and register it with the Name Service
if (PerlACE::waitforfile_timed(
                 $ready,
                 $PerlACE::wait_interval_for_process_creation) == -1) {
  print STDERR "ERROR: waiting for the consumer to start\n";
  $STC->Kill();
  $MON->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

print $STS->CommandLine()."\n";
my $server = $STS->SpawnWaitKill(30);
if ($server != 0) {
  print STDERR "ERROR: waiting for the supplier\n";
  $STS->Kill();
  $STC->Kill();
  $MON->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

$client = $STC->WaitKill(60);
if ($client != 0) {
  print STDERR "ERROR: waiting for the consumer\n";
  $STC->Kill();
  $MON->Kill();
  $TS->Kill();
  $NS->Kill();
  exit(1);
}

$MON->Kill();
$TS->Kill();
$NS->Kill();

unlink($ior, $monitorior, $notifyior, $namingior, $ready);
exit(0);
