# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$nsref="-ORBInitRef NameService=iiop://localhost:2809/NameService";
$nsior = "ns.ior";
$ior="url.ior";
unlink $ior;

# start Naming Service
unlink($nsior);
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-ORBEndPoint iiop://localhost:2809 -o $nsior");
$NS->Spawn();

if (PerlACE::waitforfile_timed ($nsior, 10) == -1) {
  print STDERR "ERROR: cannot find IOR file <$nsior>\n";
  $NS->Kill ();
  exit 1;     
}

# start MessengerServer
$MessengerServer = "MessengerServer";
$S = new PerlACE::Process($MessengerServer, $nsref );
$S->Spawn();
if (PerlACE::waitforfile_timed ($ior, 15) == -1) {
    print STDERR "ERROR: cannot find file $ior\n";
    $NS->Kill(); 
    $S->Kill(); 
    exit 1;
}
 
# start MessengerClient
$C = new PerlACE::Process("MessengerClient", $nsref);  
if ($C->SpawnWaitKill(15) != 0) {
   print STDERR "ERROR: client failed\n";
   $S->Kill();
   $NS->Kill();
   exit 1;
}

# clean-up 
$S->Kill();
$NS->Kill();
unlink $ior;

exit 0;



