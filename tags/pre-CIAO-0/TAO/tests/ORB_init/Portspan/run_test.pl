# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

use lib "../../../../bin";
use PerlACE::Run_Test;
require ACEutils;

$P1 = new PerlACE::Process("server", "-ORBEndpoint iiop://:5555/portspan=2");
$P2 = new PerlACE::Process("server", "-ORBEndpoint iiop://:5555/portspan=2");
$P3 = new PerlACE::Process("server", "-ORBEndpoint iiop://:5555/portspan=2");
$test1 = $P1->Spawn();
if ($test1 != 0) {
    print STDERR "ERROR: Couldn't start first server. Err:$test1\n";
    $P1->Kill();
    exit 1;
}

$test2 = $P2->Spawn();
if ($test2 != 0) {
    print STDERR "ERROR: Couldn't start second server. Err:$test2\n";
    $P1->Kill();
    $P2->Kill();
    exit 1;
}

$test3 = $P3->SpawnWaitKill(5);
if ($test3 == 0) {
    print STDERR "ERROR: Last server didn't fail! Err:$test3\n";
    $P1->Kill();
    $P2->Kill();
    $P3->Kill();
    exit 1;
}

$P1->Kill();
$P2->Kill();
$P3->Kill();

print STDOUT "Portspan test completed successfully.\n";

exit 0;
