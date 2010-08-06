eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$SV = new PerlACE::Process ("server", "-ORBListenEndpoints iiop://:4177;diop://:15177");

$CL = new PerlACE::Process ("client", "-ORBInitRef UDPTest=corbaloc:diop:localhost:15177/UDPTest -ORBDefaultInitRef corbaloc:iiop:localhost:4177 -ORBDebugLevel 10");

$server = $SV->Spawn ();

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    exit 1;
}

sleep (3);
$client = $CL->SpawnWaitKill (90);
#$client = $CL->Spawn ();
if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (10);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

#unlink $iorfile;

exit $status;
