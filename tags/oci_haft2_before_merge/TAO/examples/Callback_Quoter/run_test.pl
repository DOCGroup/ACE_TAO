eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$ns_ior = PerlACE::LocalFile ("NameService.ior");
$sleeptime = 5;

unlink $ns_ior;

$NS = new PerlACE::Process ("../../orbsvcs/Naming_Service/Naming_Service", "-o $ns_ior");
$N = new PerlACE::Process ("notifier", "-ORBInitRef NameService=file://$ns_ior");
$C = new PerlACE::Process ("consumer", "-ORBInitRef NameService=file://$ns_ior -t 12 -a TAO");
$S = new PerlACE::Process ("supplier", "-ORBInitRef NameService=file://$ns_ior -iexample.stocks");

print STDERR "================ Remote test\n";

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($ns_ior, 5) == -1) {
    print STDERR "ERROR: cannot find file <$ns_ior>\n";
    $NS->Kill (); 
    exit 1;
}


$N->Spawn ();

sleep $sleeptime;

$C->Spawn ();

sleep $sleeptime;

$supplier = $S->SpawnWaitKill (60);

if ($supplier != 0) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

$server = $C->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: consumer returned $server\n";
    $status = 1;
}

$server = $N->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: notifier returned $server\n";
    $status = 1;
}

$server = $NS->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: naming service returned $server\n";
    $status = 1;
}

unlink $ns_ior;

exit $status;
