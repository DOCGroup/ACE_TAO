eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$NS_ior = PerlACE::LocalFile ("NameService.ior");

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service", "-o $NS_ior");
$T = new PerlACE::Process ("EC_Multiple", 
                           "-ORBInitRef NameService=file://$NS_ior"
                           ." -s local");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($NS_ior, 5) == -1) {
    print STDERR "ERROR: waiting for naming service IOR file\n";
    $NS->Kill ();
    exit 1;
}

# This is a very simple test, no multiple consumers and no gateways.
$test = $T->SpawnWaitKill (60);


if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: naming service returned $nserver\n";
    $status = 1;
}

exit $status;
