eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../../../bin";
use PerlACE::Run_Test;

$status = 0;
$nsior = PerlACE::LocalFile ("ns.ior");
$hfior = PerlACE::LocalFile ("hf.ior");

unlink $nsior;
unlink $hfior;

# Naming_Service command line arguments
$ns_args = "-o $nsior";

# Component Server command line arguments
$cs_args = "-ORBInitRef NameService=file://$nsior -i config -o $hfior";

# Client program command line arguments
$cl_args = "-ORBInitRef HomeFinder=file://$hfior";

# Naming_Service process definition
$NS = new PerlACE::Process ("../../../../orbsvcs/Naming_Service/Naming_Service",
                            "$ns_args");

# Component Server process definition
$CS = new PerlACE::Process ("../../../../../bin/Simple_Component_Server",
                            "$cs_args");

# Client process definition
$CL = new PerlACE::Process ("client",
                            "$cl_args");

$NS->Spawn ();
if (PerlACE::waitforfile_timed ($nsior, 15) == -1) {
    print STDERR "ERROR: Could not find file <$nsior>\n";
    $NS->Kill ();
    exit 1;
}

$CS->Spawn ();
if (PerlACE::waitforfile_timed ($hfior, 15) == -1) {
    print STDERR "ERROR: Could not find file <$hfior>\n";
    $CS->Kill ();
    $NS->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$CS->Kill ();
$NS->Kill ();

unlink $nsior;
unlink $hfior;

exit $status;
