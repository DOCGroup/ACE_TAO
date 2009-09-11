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
my $port = PerlACE::uniqueid () + 10005;
my $nts_ref = "NotifyService=iioploc://localhost:$port/NotifyService";
my $svcconf = "";
my $consumer_runtime = 10;
  
my $NTS = new PerlACE::Process("../../../Notify_Service/Notify_Service",
                              "-ORBDebugLevel 1 ".
                              "-NoNameSvc -IORoutput $notifyior $svcconf " .
                              "-ORBEndpoint iiop://localhost:$port");
my $SUPPLIER = new PerlACE::Process("supplier", "$nts_ref -ORBDebugLevel 1");
my $CONSUMER = new PerlACE::Process("consumer", "$nts_ref -t $consumer_runtime");

unlink($notifyior);


print "\n*********** Starting the Notify_Service  ***********\n\n";
print $NTS->CommandLine ()."\n";

$NTS->Spawn();
if (PerlACE::waitforfile_timed($notifyior, 20) == -1) {
  print STDERR "ERROR: waiting for the notify service to start\n";
  $NTS->Kill();
  exit(1);
}

print "\n*********** Starting the notification Consumer ***********\n\n";
print STDERR $CONSUMER->CommandLine (). "\n";

my $client = $CONSUMER->Spawn();
if ($client  != 0) {
  $NTS->Kill();
  exit(1);
}

sleep(5);

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

$client = $CONSUMER->WaitKill(10);
if ($client  != 0) {
  $NTS->Kill();
  exit(1);
}

$NTS->Kill();

unlink($notifyior);
exit(0);
