eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$NS_ior = PerlACE::LocalFile ("NameService.ior");
$sleeptime = 10;
$status = 0;

unlink $NS_ior;

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service",
                            "-o $NS_ior ");
$ES = new PerlACE::Process ("../../Event_Service/Event_Service",
                            "-ORBInitRef NameService=file://$NS_ior -t new");
$T  = new PerlACE::Process ("Event_Latency",
                            "-ORBInitRef NameService=file://$NS_ior"
                            . " -j -m 100");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($NS_ior, 10) == -1) {
    print STDERR "ERROR: waiting for naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

$ES->Spawn ();

sleep $sleeptime;

$test = $T->SpawnWaitKill (60);

if ($test != 0) {
    print STDERR "ERROR: test returned $test\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: name server returned $nserver\n";
    $status = 1;
}

$eserver = $ES->TerminateWaitKill (5);

if ($eserver != 0) {
    print STDERR "ERROR: event server returned $eserver\n";
    $status = 1;
}

unlink $NS_ior;

exit $status;
