# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$nsiorfile   = PerlACE::LocalFile("ns.ior");
$messiorfile = PerlACE::LocalFile("Messenger.ior");
$arg_ns_ref = "-ORBInitRef NameService=file://$nsiorfile";
unlink $nsiorfile;
unlink $messiorfile;

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $nsiorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file $nsiorfile\n";
    $NS->Kill(); 
    exit 1;
}

# start MessengerServer
$S = new PerlACE::Process("MessengerServer", $arg_ns_ref);
$S->Spawn();

# Wait for the MessengerServer
if (PerlACE::waitforfile_timed ($messiorfile, 10) == -1) {
   print STDERR "ERROR: Timed out waiting for $messiorfile\n";
   $S->Kill();
   $NS->Kill ();
   exit 1;
}
# start MessengerConsumer
$MC = new PerlACE::Process("MessengerConsumer", $arg_ns_ref);
$MC->Spawn();

# start MessengerClient
$C = new PerlACE::Process("MessengerClient", $arg_ns_ref);
if ($C->SpawnWaitKill(10) != 0) {
     $MC->Kill();
     $S->Kill();
     $NS->Kill();
     exit (1);
}

$MC->Kill();
$S->Kill();
$NS->Kill();

unlink $nsiorfile;
unlink $messiorfile;

exit 0;
