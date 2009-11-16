eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBEndpoint iiop://127.0.0.1:4711");
}
else {
    $SV = new PerlACE::Process ("server", "-ORBEndpoint iiop://127.0.0.1:4711");
}
$CL = new PerlACE::Process ("client", "");

$SV->Spawn ();

$client = $CL->Spawn (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

sleep (5);

print STDERR "Kill server\n";
$server = $SV->Kill ();

sleep (5);

print STDERR "Restart server\n";
$SV->Spawn ();

sleep (30);

exit $status;
