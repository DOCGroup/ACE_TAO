eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$ns_ior   = PerlACE::LocalFile ("ns.ior");
$conffile = PerlACE::LocalFile ("ec" . "$PerlACE::svcconf_ext");

unlink $ns_ior;

$NS = new PerlACE::Process ("../../../Naming_Service/Naming_Service",
                           "-o $ns_ior ");

$T = new PerlACE::Process ("Service",
                           "-ORBInitRef NameService=file://$ns_ior "
                           . "-ORBsvcconf $conffile");

$C = new PerlACE::Process ("Consumer",
                           "-ORBInitRef NameService=file://$ns_ior ");

$S = new PerlACE::Process ("Supplier",
                           "-ORBInitRef NameService=file://$ns_ior ");



print STDOUT "Starting name server\n";
$NS->Spawn ();

if (PerlACE::waitforfile_timed ($ns_ior, 15) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill (); 
    exit 1;
}

print STDOUT "Starting event server\n";
$T->Spawn ();

sleep 2;

print STDOUT "Starting consumer\n";
$C->Spawn ();

sleep 1;

print STDOUT "Starting supplier\n";
$supplier = $S->SpawnWaitKill (120);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

$consumer = $C->WaitKill (15);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

$service = $T->TerminateWaitKill (5);

if ($service != 0) {
    print STDERR "ERROR: service returned $service\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: name server returned $nserver\n";
    $status = 1;
}

unlink $ns_ior;

exit $status;
