eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 on Chorus 4.0

$NS_ior = PerlACE::LocalFile ("NameService.ior");
$sleeptime = 8;
$status = 0;

$NS = new PerlACE::Process ("../../Naming_Service/Naming_Service",
                            "-ORBNameServicePort $port -o $NS_ior");
$ES = new PerlACE::Process ("../../Event_Service/Event_Service",
                            "-ORBInitRef NameService=file://$NS_ior -t new");
$C  = new PerlACE::Process ("ECM_Consumer", 
                            "-ORBInitRef NameService=file://$NS_ior");
$S  = new PerlACE::Process ("ECM_Supplier",
                            "-ORBInitRef NameService=file://$NS_ior");

$NS->Spawn ();

if (PerlACE::waitforfile_timed ($NS_ior, 10) == -1) {
    print STDERR "ERROR: waiting for naming service IOR file\n";
    $NS->Kill (); 
    exit 1;
}

$ES->Spawn ();

sleep $sleeptime;

$C->Spawn ();

sleep $sleeptime;

$S->Spawn ();

$consumer = $C->WaitKill (60);

if ($consumer != 0) {
    print STDERR "ERROR: consumer returned $consumer\n";
    $status = 1;
}

$supplier = $S->WaitKill (60);

if ($supplier == -1) {
    print STDERR "ERROR: supplier returned $supplier\n";
    $status = 1;
}

$nserver = $NS->TerminateWaitKill (5);

if ($nserver != 0) {
    print STDERR "ERROR: nameserver returned $nserver\n";
    $status = 1;
}

$eserver = $ES->TerminateWaitKill (5);

if ($eserver != 0) {
    print STDERR "ERROR: eventserver returned $eserver\n";
    $status = 1;
}

unlink $NS_ior;

exit $status;
