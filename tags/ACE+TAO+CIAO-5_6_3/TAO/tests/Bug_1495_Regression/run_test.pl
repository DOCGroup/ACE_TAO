eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$inputfile = PerlACE::LocalFile ("test.ior");
$thrserverfile = PerlACE::LocalFile ("thr_server.ior");

unlink $inputfile;
unlink $thrserverfile;
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $CL = new PerlACE::ProcessVX ("client");
}
else {
    $CL = new PerlACE::Process ("client");
}
$SV = new PerlACE::Process ("server", "-o $inputfile -i file://$thrserverfile");

$client = $CL->Spawn ();

if (PerlACE::waitforfile_timed ($thrserverfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$thrserverfile>\n";
    exit 1;
}

$server = $SV->SpawnWaitKill (30);

$client = $CL->WaitKill (5);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $inputfile;
unlink $thrserverfile;

exit $status;
