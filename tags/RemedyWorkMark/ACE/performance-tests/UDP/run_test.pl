eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$SV = new PerlACE::Process ("udp_test", "-r");
$CL = new PerlACE::Process ("udp_test", "-t -n 10000 localhost");

$status = 0;

$SV->Spawn ();

sleep 5;

$client = $CL->SpawnWaitKill (60);

$server = $SV->WaitKill (5);

if ($server != 0) {
    print "ERROR: server returned $server\n";
    $status = 1;
}

if ($client != 0) {
    print "ERROR: client returned $client\n";
    $status = 1;
}

exit $status;
