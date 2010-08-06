# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
use Env(ACE_ROOT);
use Env (TAO_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

my $ec_ior = PerlACE::LocalFile ("ec.ior");
my $notifyior = PerlACE::LocalFile ("notify.ior");
my $notify_conf = PerlACE::LocalFile ("notify$PerlACE::svcconf_ext");

my $TS = new PerlACE::Process ("$TAO_ROOT/orbsvcs/Notify_Service/Notify_Service",
                            "-boot -orbendpoint iiop://:8888 -NoNameSvc -IORoutput $notifyior -ORBSvcConf " .
                            "$notify_conf");
my $SUP = new PerlACE::Process ("supplier");
my $CONS = new PerlACE::Process ("consumer");

unlink $ec_ior;
unlink $notifyior;

$TS->Spawn ();

if (PerlACE::waitforfile_timed ($notifyior, $PerlACE::wait_interval_for_process_creation) == -1) {
    $TS->Kill ();
    exit 1;
}

print "****** Running consumer ******\n";

## The consumer takes one argument indicating
## how many events to receive before disconnecting.

$CONS->Arguments("5");
my $client = $CONS->Spawn();
if ($client != 0) {
  $TS->Kill ();
  exit 1;
}

## The supplier needs wait after the consumer creates the event channel.
if (PerlACE::waitforfile_timed ($ec_ior, 20) == -1) {
    $TS->Kill ();
    $client->Kill ();
    exit 1;
}

## The supplier takes two arguments.
## The first indicates how many events to push.
## The second indicates the payload size. (The payload is sequence<int>)

print "****** Running supplier ******\n";

$SUP->Arguments("10 5");
my $server = $SUP->Spawn();
if ($server != 0) {
  $TS->Kill();
  $CONS->Kill();
  exit 1;
}

$CONS->WaitKill(30);


$server = $SUP->WaitKill(30);
if ($server != 0) {
  $TS->Kill();
  $CONS->Kill();
  exit 1;
}

$TS->Kill ();

unlink $ec_ior;
unlink $notifyior;
exit 0;
