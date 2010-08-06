# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlACE::Run_Test;

$nsiorfile   = PerlACE::LocalFile ("ns.ior");
$ifriorfile  = PerlACE::LocalFile ("ifr.ior");
$esiorfile   = PerlACE::LocalFile ("es.ior");
$consiorfile = PerlACE::LocalFile ("Consumer.ior");
$arg_ns_ref  = "-ORBInitRef NameService=file://$nsiorfile";
$arg_ifr_ref = "-ORBInitRef InterfaceRepository=file://$ifriorfile";

unlink $nsiorfile;
unlink $ifriorfile;
unlink $esiorfile;
unlink $consiorfile;

# start the Interface Repository Service
$IFRService = "$ENV{TAO_ROOT}/orbsvcs/IFR_Service/IFR_Service";
$IF = new PerlACE::Process ($IFRService, "-o $ifriorfile");
$IF->Spawn ();
if (PerlACE::waitforfile_timed ($ifriorfile, 15) == -1) {
    print STDERR "ERROR: cannot find file <$ifriorfile>\n";
    $IF->Kill ();
    exit 1;
}

# load the IFR with the Messenger interface info
$TAO_IFR = "$ENV{ACE_ROOT}/bin/tao_ifr";
$TI = new PerlACE::Process ($TAO_IFR,
                            "$arg_ifr_ref Messenger.idl");
$TI->SpawnWaitKill (60);

# start Naming Service
$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service";
$NS = new PerlACE::Process($NameService, "-o $nsiorfile");
$NS->Spawn();
if (PerlACE::waitforfile_timed ($nsiorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$nsiorfile>\n";
    $NS->Kill();
    $IF->Kill ();
    exit 1;
}

# start Event Service
$EventService = "$ENV{TAO_ROOT}/orbsvcs/CosEvent_Service/CosEvent_Service";
$ES = new PerlACE::Process($EventService,
			   "-t -o $esiorfile $arg_ns_ref $arg_ifr_ref");
$ES->Spawn();
if (PerlACE::waitforfile_timed ($esiorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$esiorfile>\n";
    $ES->Kill();
    $NS->Kill ();
    $IF->Kill ();
    unlink $nsiorfile;
    exit 1;
}

# start Consumer
$C = new PerlACE::Process("Consumer", "$arg_ns_ref");
$C->Spawn();
if (PerlACE::waitforfile_timed ($consiorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$consiorfile>\n";
    $ES->Kill();
    $NS->Kill ();
    $IF->Kill ();
    $C->Kill ();
    exit 1;
}

# start Supplier
$S = new PerlACE::Process("Supplier", "$arg_ns_ref");
$S->Spawn();

$CRET = $C->WaitKill(60);
$S->Kill();
$NS->Kill();
$ES->Kill();
$IF->Kill();

unlink $nsiorfile;
unlink $ifriorfile;
unlink $esiorfile;
unlink $consiorfile;

if ($CRET != 0) {
    print STDERR "ERROR: Client returned <$CRET>\n";
    exit 1 ;
}

exit 0;
