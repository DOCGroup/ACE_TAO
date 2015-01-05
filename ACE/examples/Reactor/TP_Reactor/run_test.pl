eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

$SV = new PerlACE::Process ("server", "");
$CL1 = new PerlACE::Process ("client", "80 100");
$CL2 = new PerlACE::Process ("client", "80 100");
$SV->Spawn ();

sleep (1);

$client1 = $CL1->Spawn ();

if ($client1 != 0) {
    print STDERR "ERROR: client 1 returned $client1\n";
    $status = 1;
}

$client2 = $CL2->Spawn ();

if ($client2 != 0) {
    print STDERR "ERROR: client 2 returned $client2\n";
    $status = 1;
}

$server = $SV->WaitKill (1000);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

exit $status;
