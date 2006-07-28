eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-a [ff01::1]:12345 -ORBEndpoint iiop://[::1]:23456");
}
else {
    $SV = new PerlACE::Process ("server", "-a [ff01::1]:12345 -ORBEndpoint iiop://[::1]:23456");
}
$CL = new PerlACE::Process ("client", "-ORBInitRef MCASTServer=mcast://[ff01::1]:12345::2/MCASTServer");

$SV->Spawn ();

sleep 3;

$client = $CL->SpawnWaitKill (20);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (10);

exit $status;
