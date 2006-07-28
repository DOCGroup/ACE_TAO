eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$inputfile = PerlACE::LocalFile ("test.ior");
$thrserverfile = PerlACE::LocalFile ("thr_server.ior");
$middleserverfile = PerlACE::LocalFile ("middle.ior");

unlink $inputfile;
unlink $thrserverfile;
unlink $middleserverfile;
$status = 0;

$CL = new PerlACE::Process ("client", "-i file://$inputfile -o $thrserverfile");
$SV = new PerlACE::Process ("server", "-i file://$middleserverfile -o $inputfile");
$ML = new PerlACE::Process ("server", "-i file://$thrserverfile -o $middleserverfile");

$CL->Spawn ();

if (PerlACE::waitforfile_timed ($thrserverfile, 2) == -1) {
    print STDERR "ERROR: cannot find file <$thrserverfile>\n";
    exit 1;
}

$ML->Spawn();

if (PerlACE::waitforfile_timed ($middleserverfile, 2) == -1) {
    print STDERR "ERROR: cannot find file <$middleserverfile>\n";
    exit 1;
}

$server = $SV->SpawnWaitKill (50);

$client = $CL->WaitKill (5);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$middle = $ML->WaitKill (5);

if ($middle != 0) {
    print STDERR "ERROR: middle server returned $server\n";
    $status = 1;
}

unlink $inputfile;
unlink $thrserverfile;
unlink $middleserverfile;

exit $status;
