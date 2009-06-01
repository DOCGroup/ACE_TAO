# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;


$TARGETHOSTNAME = "localhost";
$def_port = 2809;

$nsiorfile = PerlACE::LocalFile ("ns.ior");
$arg_ns_ref = "-ORBInitRef NameService=file://$nsiorfile";

unlink $nsiorfile;

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $nsiorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$nsiorfile>\n";
    $NS->Kill(); 
    exit 1;
}

# start MessengerServer
$S = new PerlACE::Process("MessengerServer", $arg_ns_ref);
$S->Spawn();

# Give the server some time
# to bind the object reference
# with the Naming Service
sleep 3;
 
# start MessengerClient
$C = new PerlACE::Process("MessengerClient", $arg_ns_ref);  

if ($C->SpawnWaitKill(15) != 0) {
   print STDERR "ERROR: client failed\n";
   $S->Kill();
   $NS->Kill();
   exit 1;
}

# clean-up 

$C->Kill();
$S->Kill();
$NS->Kill();

unlink $nsiorfile;

exit 0;



