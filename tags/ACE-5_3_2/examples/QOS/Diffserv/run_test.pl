eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("ior");

unlink $iorfile;
$status = 0;

$CL = new PerlACE::Process ("diffserv_test", "localhost 20002");
$SV = new PerlACE::Process ("server");

$server = $SV->Spawn ();
$SV->TimedWait(2);

$client = $CL->SpawnWaitKill (200);

if ($client != 0) {
    $time = localtime;
    print STDERR "ERROR: client returned $client at $time\n";
    $status = 1;
}

$server = $SV->WaitKill (400);

if ($server != 0) {
    $time = localtime;
    print STDERR "ERROR: server returned $server at $time\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
