eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$inputfile = PerlACE::LocalFile ("test.ior");
$thrserverfile = PerlACE::LocalFile ("thr_server.ior");

unlink $inputfile;
unlink $thrserverfile;
$status = 0;

$CL = new PerlACE::Process ("client");
$SV = new PerlACE::Process ("server");

$CL->Spawn ();

if (PerlACE::waitforfile_timed ($thrserverfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$thrserverfile>\n";
    exit 1;
}

$SV->SpawnWaitKill (30);

$client = $CL->WaitKill (5);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $inputfile;
unlink $thrserverfile;

exit $status;
