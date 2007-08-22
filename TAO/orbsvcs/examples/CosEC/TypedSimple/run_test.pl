eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$nsiorfile = PerlACE::LocalFile ("ns.ior");
$ifriorfile = PerlACE::LocalFile ("ifr.ior");
$eciorfile = PerlACE::LocalFile ("ec.ior");
$consiorfile = PerlACE::LocalFile ("Consumer.ior");

unlink $nsiorfile;
unlink $ifriorfile;
unlink $eciorfile;
unlink $consiorfile;


$IF = new PerlACE::Process ($ENV{"TAO_ROOT"}."/orbsvcs/IFR_Service/IFR_Service",
                            "-o $ifriorfile");
$TI = new PerlACE::Process ($ENV{"ACE_ROOT"}."/bin/tao_ifr",
                            "-ORBInitRef InterfaceRepository=file://$ifriorfile " .
                            "-I".$ENV{"TAO_ROOT"}."/orbsvcs Country.idl");
$NS = new PerlACE::Process ($ENV{"TAO_ROOT"}."/orbsvcs/Naming_Service/Naming_Service",
                            "-o $nsiorfile");
$CE = new PerlACE::Process ($ENV{"TAO_ROOT"}."/orbsvcs/CosEvent_Service/CosEvent_Service",
                            "-n CountryEventChannel -r -t -d -o $eciorfile " .
                            "-ORBInitRef InterfaceRepository=file://$ifriorfile " .
                            "-ORBInitRef NameService=file://$nsiorfile ");
$C = new PerlACE::Process ("Consumer", "-ORBInitRef NameService=file://$nsiorfile ");
$S = new PerlACE::Process ("Supplier", "-ORBInitRef NameService=file://$nsiorfile ");

$IF->Spawn ();

if (PerlACE::waitforfile_timed ($ifriorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$ifriorfile>\n";
    $IF->Kill ();
    exit 1;
}

$TI->SpawnWaitKill (60);

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($nsiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$nsiorfile>\n";
    $IF->Kill ();
    $NS->Kill ();
    exit 1;
}

$CE->Spawn ();

if (PerlACE::waitforfile_timed ($eciorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$eciorfile>\n";
    $IF->Kill ();
    $NS->Kill ();
    $CE->Kill ();
    exit 1;
}

$C->Spawn ();

if (PerlACE::waitforfile_timed ($consiorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$consiorfile>\n";
    $IF->Kill ();
    $NS->Kill ();
    $CE->Kill ();
    $C->Kill ();
    exit 1;
}

$supplier = $S->SpawnWaitKill (120);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

$server = $C->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: consumer returned $server\n";
    $status = 1;
}

$server = $CE->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: CosEvent_Service returned $server\n";
    $status = 1;
}

$server = $NS->TerminateWaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: name service returned $server\n";
    $status = 1;
}

$server = $IF->TerminateWaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: IFR_Service returned $server\n";
    $status = 1;
}

unlink $nsiorfile;
unlink $ifriorfile;
unlink $eciorfile;
unlink $consiorfile;

exit $status;
