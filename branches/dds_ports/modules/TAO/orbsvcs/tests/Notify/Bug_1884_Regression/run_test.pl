eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use strict;

my $persistent_test = 0;
my $notifyior = PerlACE::LocalFile("ecf.ior");
my $notify2ior = PerlACE::LocalFile("ecf2.ior");
my $notify_conf = PerlACE::LocalFile("ecf$PerlACE::svcconf_ext");
my $port = PerlACE::uniqueid () + 10005;
my $nts_ref = "NotifyService=iioploc://localhost:$port/NotifyService";
my $svcconf = "";
my $consumer_runtime = 5;
my $persistent_file = PerlACE::LocalFile ("persistency.notif.xml");
my $persistent_file_backup = PerlACE::LocalFile ("persistency.notif.000");


if ($#ARGV >= 0 && $ARGV[0] eq '-p') {
  $persistent_test = 1;
  $svcconf = " -ORBSvcConf $notify_conf";
  $consumer_runtime = 20;
}
  
my $NTS = new PerlACE::Process("../../../Notify_Service/Notify_Service",
                              "-ORBDebugLevel 1 ".
                              "-NoNameSvc -IORoutput $notifyior $svcconf " .
                              "-ORBEndpoint iiop://localhost:$port");
my $SUPPLIER = new PerlACE::Process("supplier", "$nts_ref");
my $CONSUMER = new PerlACE::Process("consumer", "$nts_ref -t $consumer_runtime");
my $FILTER = new PerlACE::Process("filter", "$nts_ref");

unlink($notifyior, $persistent_file, $persistent_file_backup);


print "\n*********** Starting the Notify_Service  ***********\n\n";
$NTS->Spawn();
if (PerlACE::waitforfile_timed($notifyior, 20) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $NTS->Kill();
  exit(1);
}

if ($persistent_test == 0)
{
  print "\n*********** Starting the filter test ***********\n\n";
  my $filter = $FILTER->SpawnWaitKill (10);
  if ($filter != 0) {
    $NTS->Kill();
    exit(1);
  }

  print "\n*********** Filter test passed ***********\n\n";
}

print "\n*********** Starting the notification Consumer ***********\n\n";
print STDERR $CONSUMER->CommandLine (). "\n";

my $client = $CONSUMER->Spawn();
if ($client  != 0) {
  $NTS->Kill();
  exit(1);
}

sleep(5);

if ($persistent_test == 1)
{
  print "*********** Killing the first Notify_Service   ***********\n";
  $NTS->Kill();
  sleep(1);

  unlink ($notifyior);
  
  print "*********** Starting the second Notify_Service ***********\n";
print STDERR $NTS->CommandLine (). "\n";
  $NTS->Spawn();
  if (PerlACE::waitforfile_timed($notifyior, 20) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $SUPPLIER->Kill();
    exit(1);
  }
  
 sleep(5); 
}

print "\n*********** Starting the notification Supplier ***********\n\n";
print STDERR $SUPPLIER->CommandLine (). "\n";

my $server = $SUPPLIER->Spawn();
if ($server  != 0) {
  $NTS->Kill();
  $CONSUMER->Kill();
  exit(1);
}

$server = $SUPPLIER->WaitKill(10);
if ($server  != 0) {
  $NTS->Kill();
  $CONSUMER->Kill();
  exit(1);
}

$client = $CONSUMER->WaitKill($consumer_runtime + 10);
if ($client  != 0) {
  $NTS->Kill();
  exit(1);
}

$NTS->Kill();

#unlink($notifyior, $persistent_file, $persistent_file_backup);
exit(0);
