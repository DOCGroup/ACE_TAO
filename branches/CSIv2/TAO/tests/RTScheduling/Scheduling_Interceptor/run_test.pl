eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

# amount of delay between running the servers

$sleeptime = 2;
$status = 0;

$SV = new PerlACE::Process ("test_server", "-f test.ior");
$CL = new PerlACE::Process ("test_client", "-f test.ior");

print STDERR "Starting Server\n";

$SV->Spawn ();

sleep $sleeptime;

print STDERR "Starting Client\n";

$sender = $CL->SpawnWaitKill (200);

if ($sender != 0) {
    print STDERR "ERROR: sender returned $sender\n";
    $status = 1;
}

$receiver = $SV->TerminateWaitKill (5);

if ($receiver != 0) {
    print STDERR "ERROR: receiver returned $receiver\n";
    $status = 1;
}

exit $status;
