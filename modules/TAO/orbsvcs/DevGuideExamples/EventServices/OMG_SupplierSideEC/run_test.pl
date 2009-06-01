# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("ns.ior");
$arg_ns_ref = "-ORBInitRef NameService=file://$iorfile";

unlink $iorfile;

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $iorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $NS->Kill(); 
    exit 1;
}

# start EchoEventSupplier  
$S = new PerlACE::Process("EchoEventSupplier", $arg_ns_ref);
$S->Spawn();

# Allow time for the supplier to register with the Naming Service
sleep(2);

# start EchoEventConsumer  
$C = new PerlACE::Process("EchoEventConsumer", $arg_ns_ref);
$CRET = $C->SpawnWaitKill(60);

$S->Kill();
$NS->Kill();

unlink $iorfile;

if ($CRET != 0) {
    print STDERR "ERROR: Client returned <$CRET>\n";
    exit 1 ;
}  


exit 0;


