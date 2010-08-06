# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$nsiorfile = PerlACE::LocalFile ("ns.ior");
$esiorfile = PerlACE::LocalFile ("es.ior");
$arg_ns_ref = "-ORBInitRef NameService=file://$nsiorfile";

unlink $nsiorfile;
unlink $esiorfile;

# start Naming Service

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $nsiorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$nsiorfile>\n";
    $NS->Kill(); 
    exit 1;
}

# start Event Service
$EventService = "$ENV{TAO_ROOT}/orbsvcs/Event_Service/Event_Service";
$ES = new PerlACE::Process($EventService, "-o $esiorfile $arg_ns_ref");
$ES->Spawn();
if (PerlACE::waitforfile_timed ($esiorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$esiorfile>\n";
    $ES->Kill(); 
    unlink $nsiorfile;
    exit 1;
}

# start EchoEventSupplier  
$S = new PerlACE::Process("EchoEventSupplier", $arg_ns_ref);
$S->Spawn();

# start EchoEventConsumer  
$C = new PerlACE::Process("EchoEventConsumer", $arg_ns_ref);
$C->Spawn();

$CRET = $C->WaitKill(60);
$S->Kill();
$NS->Kill();
$ES->Kill();

unlink $nsiorfile;
unlink $esiorfile;

if ($CRET != 0) {
    print STDERR "ERROR: Client returned <$CRET>\n";
    exit 1 ;
}  

exit 0;


