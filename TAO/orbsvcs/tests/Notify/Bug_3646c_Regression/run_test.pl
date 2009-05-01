eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib');

use strict;

my $status = 0;
my $port = 3000;
my $namingior = PerlACE::LocalFile("naming.ior");
my $notifyior = PerlACE::LocalFile("notify.ior");

my $NS = new PerlACE::Process("../../../Naming_Service/Naming_Service",
                              "-ORBEndpoint iiop://localhost:$port " .
                              "-o $namingior");
my $TS = new PerlACE::Process("server",
                              "-ORBInitRef NameService=iioploc://" .
                              "localhost:$port/NameService " .
                              "-IORoutput $notifyior");
my $STC1 = new PerlACE::Process("Consumer",
                                "-ORBInitRef NameService=iioploc://" .
                                "localhost:$port/NameService");

unlink($notifyior, $namingior);

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

$STC1->Spawn();

sleep(2);

my $client = $STC1->WaitKill(5);
if ($client != 0) {
  $status = 1;
}

$TS->Kill();
$NS->Kill();

unlink($notifyior, $namingior);

exit($status);
