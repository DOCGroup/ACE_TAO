eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$file = PerlACE::LocalFile ("test.ior");

unlink $file;

$SV = new PerlACE::Process ("server", "-ORBobjrefstyle url -o $file");
$CL = new PerlACE::Process ("client", "-ORBobjrefstyle url -f file://$file");

print STDERR "\n\n==== Running interceptor Benchmark test\n";

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $file;

exit 0;
