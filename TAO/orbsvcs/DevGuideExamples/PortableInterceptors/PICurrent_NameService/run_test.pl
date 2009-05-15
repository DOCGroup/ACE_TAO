# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$nsiorfile = PerlACE::LocalFile ("ns.ior");
unlink $nsiorfile;
$PORT=9876;

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $nsiorfile -ORBEndpoint iiop://localhost:$PORT");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file $nsiorfile\n";
    $NS->Kill(); 
    exit 1;
}

# start MessengerServer
$IREF = "-ORBInitRef NameService=iioploc://localhost:$PORT/NameService";

# start MessengerServer
$S = new PerlACE::Process("MessengerServer", $IREF);
$S->Spawn();

# start MessengerClient
sleep(1);
$C = new PerlACE::Process("MessengerClient", $IREF);
if ($C->SpawnWaitKill(15) != 0) {
     $S->Kill();
     $NS->Kill();
     exit(1);
}
# clean-up 

$S->Kill();
$NS->Kill();

exit 0;



